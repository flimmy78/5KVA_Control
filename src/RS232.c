/*
 * RS232.c
 *
 *  Created on: Feb 21, 2012
 *      Author: Mike
 */

#include "DSP2833x_Device.h"     // Device Header Include File
#include "Timer.h"
#include "DA_Driver.h"
#include "usDelay.h"
#include "RS232.h"
#include "CRC16.h"
#include "main.h"
#define TRUE	1
#define FALSE	0
typedef int Bool;

void push_buf(RsBufObj *RsBuf, Uint16 InData);
Uint16 pop_buf(RsBufObj *RsBuf);
void tx_RS232(RS232Obj *RS232);
void tx_word(RS232Obj *RS232, Uint16 word);
void tx_frame_word(RS232Obj *RS232, Uint16 word);
void tx_a_Frame(RS232Obj *RS232);
void rx_RS232(RS232Obj *Rs232);
void rest_Rx(RS232Obj *Rs232);
Bool delOverLap(RS232Obj *RS232);
Uint16 CRC16(Uint16 * message, Uint16 length, Uint16 crc);
void parser_rx_data(RS232Obj *RS232Prt, Uint16 Rxbuf[]);
void rxapp_RS232();
RS232Obj RS232;
void scia_msg(char *msg);
extern char *msg;
extern float DCVolRef, GridActiveCur, GridReactiveCur;
Uint16 GUIdata[12] = { 3805, 5000, 10000, 0, 500, 495, 505, 1203, 1154, 1256, 0,
		0 };
void init_SCI(void) {
	Uint16 i;
	//Init GPIO
	EALLOW;
	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;   // Enable pull-up for GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	  // Enable pull-up for GPIO29 (SCITXDA)
	// Set qualification for selected pins to asynch only
	// Inputs are synchronized to SYSCLKOUT by default.
	// This will select asynch (no qualification) for the selected pins.
	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
	//Configure SCI-A pins using GPIO regs
	// This specifies which of the possible GPIO pins will be SCI functional pins.
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1; // Configure GPIO28 for SCIRXDA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1; // Configure GPIO29 for SCITXDA operation
	//enable SCI module clk
	SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   // SCI-A
	EDIS;
	//Init FIFO
	SciaRegs.SCIFFTX.all = 0xE040;
	SciaRegs.SCIFFRX.all = 0x204f;
	SciaRegs.SCIFFCT.all = 0x0;
	//config SCI regs
	SciaRegs.SCIPRI.all = 0x08;
	SciaRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
									// No parity,8 char bits,
									// async mode, idle-line protocol
	//enable Tx and Rx
	SciaRegs.SCICTL1.bit.TXENA = 1;
	SciaRegs.SCICTL1.bit.RXENA = 1;
	//set Baud rate
	SciaRegs.SCIHBAUD = ((Uint16) (RS232_BAUDRATE_9600 & 0xFF00)) >> 8;
	SciaRegs.SCILBAUD = (Uint16) (RS232_BAUDRATE_9600 & 0x00FF);
	SciaRegs.SCICTL1.all = 0x0023;     // Relinquish SCI from Reset

	//Init SCI obj
	init_RS_BUF(&RS232.TxBuf);
	RS232.TxFrameCnt = 0;
	RS232.RxCnt = 0;
	RS232.RxStatus = RX_IDEL;
	RS232.TimeStamp = 0;
	RS232.rx = &rx_RS232;
	RS232.tx = &tx_RS232;
	RS232.tx_frame_word = &tx_frame_word;
	RS232.tx_a_Frame = &tx_a_Frame;
	RS232.rest_Rx = &rest_Rx;
	for (i = 0; i < RS232_BUF_DEPTH >> 1; i++) {
		RS232.RxBuf[i] = 0;
		RS232.TxFrameBuf[i] = 0;
	}

}

Bool delOverLap(RS232Obj *RS232) {
	Uint16 Length, NewLength, i;
	Bool RS = FALSE;
	//if find any overlap frame then delete!
	Length = (RS232->RxBuf[0] & 0x00ff) >> 1;
	if (RS232->RxCnt > Length) {
		NewLength = (RS232->RxBuf[Length] & 0x00ff) >> 1;
		if (RS232->RxCnt >= (Length + NewLength)) {
			//Now we know there is overlap.
			//try to left shift all rest data and overwrite the overlap one
			RS232->RxCnt -= NewLength;
			for (i = 0; i < RS232->RxCnt - Length; i++) {
				RS232->RxBuf[Length + i] = RS232->RxBuf[Length + NewLength + i];
				RS = TRUE;
			}
		}
	}
	return RS;
}

void rx_RS232(RS232Obj *RS232) {
	Uint16 RxNum;
	//Unit Length,CRC ;
	RxNum = SciaRegs.SCIFFRX.bit.RXFFST;

	if (RxNum == 6) {
		//set start info
		//	if(RS232->RxStatus != RX_START){
		//		RS232->RxStatus = RX_START ;
		//	}

		RS232->RxStatus = RX_READY;
		RxNum >>= 1;
		//read data
		while (RxNum > 0) {
			RS232->RxBuf[RS232->RxCnt] = ((((Uint16) SciaRegs.SCIRXBUF.bit.RXDT)
					<< 8) & 0xff00); //Hi Byte
			RS232->RxBuf[RS232->RxCnt] = (SciaRegs.SCIRXBUF.bit.RXDT & 0x00ff)
					| (RS232->RxBuf[RS232->RxCnt]);
			;	//Low byte
			RS232->RxCnt++;
			RxNum--;
		}
	}
	//parse data
	/*	if(RS232->RxStatus==RX_START){
	 if(RS232->RxCnt>=1){	//when get the length info of this frame
	 Length = RS232->RxBuf[0] & 0x00ff ;	//length is in bytes
	 //now is in word
	 Length = Length>>1 ;
	 if(RS232->RxCnt >= Length){
	 //CRC check ;
	 CRC = RS232->RxBuf[Length-1] ;
	 if(CRC == CRC16(RS232->RxBuf,(Length-1)*2,0xffff)){
	 RS232->RxStatus = RX_READY ;
	 }
	 else{
	 RS232->RxStatus = RX_CRC_ERROR ;
	 }
	 while(delOverLap(RS232)){} ;
	 }
	 }
	 }*/

}

void rest_Rx(RS232Obj *Rs232) {
	Uint16 i;
	for (i = 0; i < RS232.RxCnt; i++) {
		RS232.RxBuf[i] = 0;
	}
	RS232.RxCnt = 0;
	RS232.RxStatus = RX_IDEL;
	RS232.TimeStamp = 0;
}

void tx_RS232(RS232Obj *RS232) {
	Uint16 temp, i;
	//first get available TX FIFO
	temp = 16 - SciaRegs.SCIFFTX.bit.TXFFST;
	//get what's left in the Tx Buf
	if (temp > RS232->TxBuf.Cnt) {
		temp = RS232->TxBuf.Cnt;
	}
	//then write into FIFO
	for (i = 0; i < temp; i++) {
		SciaRegs.SCITXBUF = RS232->TxBuf.pop(&RS232->TxBuf) & 0x00ff;
	}
}

void rxapp_RS232() {
	/*	if(RS232.RxStatus == RX_CRC_ERROR){
	 RS232.tx_frame_word(&RS232,0x0004) ;	//Error request repeat
	 RS232.tx_a_Frame(&RS232) ;	//transmit the frame ;
	 //reset RX
	 RS232.rest_Rx(&RS232) ;
	 }*/

	if (RS232.RxStatus == RX_READY) {
		//
		parser_rx_data(&RS232, RS232.RxBuf);
		RS232.tx_a_Frame(&RS232);	//transmit the frame ;
		RS232.rest_Rx(&RS232);
	}

}

void parser_rx_data(RS232Obj *RS232Prt, Uint16 Rxbuf[]) {
	//First fetch the Data length

	Uint16 CmdType;
	Aword TempWord;
	int i = 0;
	//check if is a read cmmd 
	TempWord.all = Rxbuf[0];
	CmdType = TempWord.bits.Hi;

	switch (CmdType) {
	case 0x3: //read
	{
		//first update
		RS232Prt->tx_frame_word(RS232Prt, 0x0301);
		RS232Prt->tx_frame_word(RS232Prt, 0x0101);
		for (i = 0; i < 12; i++) {
			RS232Prt->tx_frame_word(RS232Prt, GUIdata[i]);

		}
		break;
	}
	case 0x2: //write
	{
		switch (TempWord.bits.Lo) {
		case 0x01:              // Receive data in bus monitor mode
		{
			DCVolRef = Rxbuf[1] / 10.0;
			if (DCVolRef > 400) {
				DCVolRef = 400;                //Limiter for DC voltage ref
			}
			if (DCVolRef < 300) {
				DCVolRef = 300;
			}
			if (Rxbuf[2] & 0x8000) {
				GridReactiveCur = (((Rxbuf[2] >> 8) & 0x00ff - 128) * 256
						+ (Rxbuf[2] & 0x00ff)) / (-100.0);
			} else {
				GridReactiveCur = Rxbuf[2] / 100.0;
			}

			if (GridReactiveCur > 10) {
				GridReactiveCur = 10;         //Limiter for reactive current ref
			}
			if (GridReactiveCur < -10) {
				GridReactiveCur = -10;
			}
			RS232Prt->tx_frame_word(RS232Prt, 0x0201);
			RS232Prt->tx_frame_word(RS232Prt, 0x0101);
			break;
		}
		case 0x02:              // Receive data in power dispatch mode
		{

			if (Rxbuf[1] & 0x8000) {
				GridActiveCur = (((Rxbuf[1] >> 8) & 0x00ff - 128) * 256
						+ (Rxbuf[1] & 0x00ff)) / (-100.0);
			} else {
				GridActiveCur = Rxbuf[1] / 100.0;
			}

			if (GridActiveCur > 10) {
				GridActiveCur = 10;             //Limiter for active current ref
			}
			if (GridActiveCur < -10) {
				GridActiveCur = -10;
			}
			if (Rxbuf[2] & 0x8000) {
				GridReactiveCur = (((Rxbuf[2] >> 8) & 0x00ff - 128) * 256
						+ (Rxbuf[2] & 0x00ff)) / (-100.0);
			} else {
				GridReactiveCur = Rxbuf[2] / 100.0;
			}
			if (GridReactiveCur > 10) {
				GridReactiveCur = 10;         //Limiter for reactive current ref
			}
			if (GridReactiveCur < -10) {
				GridReactiveCur = -10;
			}
			RS232Prt->tx_frame_word(RS232Prt, 0x0202);
			RS232Prt->tx_frame_word(RS232Prt, 0x0101);
			break;
		}
		}

		break;
	}
	case 0x1: //cmd
	{
		switch (TempWord.bits.Lo) {
		case 0x00:                // STOP Command
		{
			ProFlag.bit.OrderOn = 0;
			//LEDGon();
			//LEDRoff();
			ProFlag.bit.StartUp = 0;
			RS232Prt->tx_frame_word(RS232Prt, 0x0100);
			RS232Prt->tx_frame_word(RS232Prt, 0x0101);
			break;

		}
		case 0x01:              // Run Command bus monitor
		{
			ProFlag.bit.OrderOn = 1;
			ProFlag.bit.Mode = 0;
			//LEDRon();
			//LEDGoff();
			RS232Prt->tx_frame_word(RS232Prt, 0x0101);
			RS232Prt->tx_frame_word(RS232Prt, 0x0101);
			break;
		}
		case 0x02:              // Run Command power dispatch
		{
			ProFlag.bit.OrderOn = 1;
			ProFlag.bit.Mode = 1;
			//LEDRon();
			//LEDGoff();
			RS232Prt->tx_frame_word(RS232Prt, 0x0102);
			RS232Prt->tx_frame_word(RS232Prt, 0x0101);
			break;
		}
		}

		break;
	}
	}

}

void tx_word(RS232Obj *RS232, Uint16 word) {
	RS232->TxBuf.push(&RS232->TxBuf, (word & 0xff00) >> 8);
	RS232->TxBuf.push(&RS232->TxBuf, (word & 0x00ff));
}

void tx_a_Frame(RS232Obj *RS232) {
	Uint16 i;
	//first calculate CRC
	/*	if(RS232->TxFrameCnt>0){
	 RS232->TxFrameBuf[RS232->TxFrameCnt] = CRC16(RS232->TxFrameBuf,RS232->TxFrameCnt<<1,0xffff) ;
	 RS232->TxFrameCnt++ ;
	 }*/
	//transmit
	for (i = 0; i < RS232->TxFrameCnt; i++) {
		tx_word(RS232, RS232->TxFrameBuf[i]);
		RS232->TxFrameBuf[i] = 0;
	}
	RS232->TxFrameCnt = 0;
}

void tx_frame_word(RS232Obj *RS232, Uint16 word) {
	if (RS232->TxFrameCnt < (RS232_BUF_DEPTH >> 1)) {
		RS232->TxFrameBuf[RS232->TxFrameCnt] = word;
		RS232->TxFrameCnt++;
	}
}

//========================Buffer Function==============================================
void init_RS_BUF(RsBufObj *RsBuf) {
	int i;
	RsBuf->Cnt = 0;
	for (i = 0; i < RS232_BUF_DEPTH; i++) {
		RsBuf->Buf[i] = 0;
	}
	RsBuf->pop = &pop_buf;
	RsBuf->push = &push_buf;
}

void push_buf(RsBufObj *RsBuf, Uint16 InData) {
	if (RsBuf->Cnt < RS232_BUF_DEPTH) {
		RsBuf->Buf[RsBuf->Cnt] = InData;
		RsBuf->Cnt++;
	}
}

Uint16 pop_buf(RsBufObj *RsBuf) {
	Uint16 Temp, i;
	Temp = 0;
	if (RsBuf->Cnt > 0 && RsBuf->Cnt <= RS232_BUF_DEPTH) {
		Temp = RsBuf->Buf[0];
		for (i = 0; i < RsBuf->Cnt - 1; i++) {
			RsBuf->Buf[i] = RsBuf->Buf[i + 1];
		}
		RsBuf->Buf[RsBuf->Cnt - 1] = 0;
		RsBuf->Cnt--;
	}
	return Temp;
}

//=========================CRC16===========================================================
//ROUTINE:
//DESCRIPTION:
//message: the address of message string
//length: length of message string in bytes to calculate crc16
//crcal: to input the initial value of crc16 calculation buffer. If it is the first time to
//       calculate the string CRC value, the initial value shall be 0xffff.
Uint16 CRC16(Uint16 * message, Uint16 length, Uint16 crc) {
	Uint16 i, j, data, crcal; /* loop variable */

	i = 0;
	crcal = crc;

	while (i < length) {
		data = *(message + (i >> 1));
		if (i & 0x0001)
			j = (data & 0x00ff) ^ ((crcal & 0xff00) >> 8);		//low byte
		else
			j = ((data & 0xff00) >> 8) ^ ((crcal & 0xff00) >> 8);	//high byte
		crcal = ((tbcrch[j & 0x00ff] ^ (crcal & 0x00ff)) << 8) & 0xff00;//MSB
		crcal = (tbcrcl[j & 0x00ff] & 0x00ff) | crcal;					//LSB
		i++;
	}
	return crcal;
}

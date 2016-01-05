/*
 * CanBus.c
 *
 *  Created on: Sep 9, 2015
 *      Author: LongQi
 */
#include "CanBus.h"

// Global variable

struct MBOX can_msg;
struct ECAN_REGS ECanbShadow;
int16 new_data = FALSE;

void configureEcanB(void) {

	can_msg.MDL.all = 0;
	can_msg.MDH.all = 0;
	can_msg.MSGID.all = 0;

	InitECanb();

	//InitECanbGpio();
	//******************InitECanbGpio() begin******************
	EALLOW;

	/* Enable internal pull-up for the selected CAN pins */
	// Pull-ups can be enabled or disabled by the user.
	// This will enable the pullups for the specified pins.
	// Comment out other unwanted lines.
	//	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;	  // Enable pull-up for GPIO8  (CANTXB)
	//  GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   // Enable pull-up for GPIO12 (CANTXB)
	//  GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up for GPIO16 (CANTXB)
	GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;   // Enable pull-up for GPIO20 (CANTXB)

	//	GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;	  // Enable pull-up for GPIO10 (CANRXB)
	//  GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;   // Enable pull-up for GPIO13 (CANRXB)
	//  GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up for GPIO17 (CANRXB)
	GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;   // Enable pull-up for GPIO21 (CANRXB)

	/* Set qualification for selected CAN pins to asynch only */
	// Inputs are synchronized to SYSCLKOUT by default.
	// This will select asynch (no qualification) for the selected pins.
	// Comment out other unwanted lines.
	//    GpioCtrlRegs.GPAQSEL1.bit.GPIO10 = 3; // Asynch qual for GPIO10 (CANRXB)
	//  GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; // Asynch qual for GPIO13 (CANRXB)
	//  GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch qual for GPIO17 (CANRXB)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3; // Asynch qual for GPIO21 (CANRXB)

	/* Configure eCAN-B pins using GPIO regs*/
	// This specifies which of the possible GPIO pins will be eCAN functional pins.
	//	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 2;   // Configure GPIO8 for CANTXB operation
	//  GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2;  // Configure GPIO12 for CANTXB operation
	//  GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;  // Configure GPIO16 for CANTXB operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 3; // Configure GPIO20 for CANTXB operation

	//	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 2;  // Configure GPIO10 for CANRXB operation
	//  GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 2;  // Configure GPIO13 for CANRXB operation
	//  GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;  // Configure GPIO17 for CANRXB operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 3; // Configure GPIO21 for CANRXB operation

	EDIS;
	//******************InitECanbGpio() end******************

	EALLOW;
#if (CPU_FRQ_150MHZ)                       // CPU_FRQ_150MHz is defined in DSP2833x_Examples.h
	/* The following block for all 150 MHz SYSCLKOUT (75 MHz CAN clock) - default. Bit rate = 1 Mbps
	 See Note at end of file */
	ECanbShadow.CANBTC.bit.BRPREG = 4; // 4 for 1MHz, 9 for 500KHz, 19 for 250KHz
	ECanbShadow.CANBTC.bit.TSEG2REG = 2;
	ECanbShadow.CANBTC.bit.TSEG1REG = 10;
#endif
	EDIS;

	EALLOW;
	PieVectTable.ECAN1INTB = &ecan1_intb_isr;		//ECAN interrupt
	EDIS;

	IER |= M_INT9;
	PieCtrlRegs.PIEIER9.bit.INTx8 = 1;  // ECAN-B
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

	// Enable global Interrupts and higher priority real-time debug events:
	EINT;
	// Enable Global interrupt INTM
	ERTM;
	// Enable Global realtime interrupt DBGM

	// Mailboxs can be written to 16-bits or 32-bits at a time
	// Write to the MSGID field of TRANSMIT mailboxes MBOX0 - 15
	ECanbMboxes.MBOX0.MSGID.all = BBB_ID;
	ECanbMboxes.MBOX1.MSGID.all = RES_ID;
	ECanbMboxes.MBOX2.MSGID.all = BIC_ID;
	ECanbMboxes.MBOX3.MSGID.all = PV_ID;
	ECanbMboxes.MBOX4.MSGID.all = BAT_ID;
	ECanbMboxes.MBOX5.MSGID.all = WIN_ID;
	ECanbMboxes.MBOX6.MSGID.all = RES_HB_ID;
	ECanbMboxes.MBOX7.MSGID.all = BIC_HB_ID;
	ECanbMboxes.MBOX8.MSGID.all = PV_HB_ID;
	ECanbMboxes.MBOX9.MSGID.all = BAT_HB_ID;
	ECanbMboxes.MBOX10.MSGID.all = WIN_HB_ID;
	ECanbMboxes.MBOX11.MSGID.all = 0x9555AAAB;
	ECanbMboxes.MBOX12.MSGID.all = 0x9555AAAC;
	ECanbMboxes.MBOX13.MSGID.all = 0x9555AAAD;
	ECanbMboxes.MBOX14.MSGID.all = 0x9555AAAE;
	ECanbMboxes.MBOX15.MSGID.all = 0x9555AAAF;

	// Write to the MSGID field of RECEIVE mailboxes MBOX16 - 31
	ECanbMboxes.MBOX16.MSGID.all = BBB_ID;
	ECanbMboxes.MBOX17.MSGID.all = RES_ID;
	ECanbMboxes.MBOX18.MSGID.all = BIC_ID;
	ECanbMboxes.MBOX19.MSGID.all = PV_ID;
	ECanbMboxes.MBOX20.MSGID.all = BAT_ID;
	ECanbMboxes.MBOX21.MSGID.all = WIN_ID;
	ECanbMboxes.MBOX22.MSGID.all = RES_HB_ID;
	ECanbMboxes.MBOX23.MSGID.all = BIC_HB_ID;
	ECanbMboxes.MBOX24.MSGID.all = PV_HB_ID;
	ECanbMboxes.MBOX25.MSGID.all = BAT_HB_ID;
	ECanbMboxes.MBOX26.MSGID.all = WIN_HB_ID;
	ECanbMboxes.MBOX27.MSGID.all = 0x9555AAAB;
	ECanbMboxes.MBOX28.MSGID.all = 0x9555AAAC;
	ECanbMboxes.MBOX29.MSGID.all = 0x9555AAAD;
	ECanbMboxes.MBOX30.MSGID.all = 0x9555AAAE;
	ECanbMboxes.MBOX31.MSGID.all = 0x9555AAAF;

	// Configure Mailboxes 0-15 as Tx, 16-31 as Rx
	// Since this write is to the entire register (instead of a bit
	// field) a shadow register is not required.
	ECanbRegs.CANMD.all = 0xFFFF0000;

	// Enable all Mailboxes */
	// Since this write is to the entire register (instead of a bit
	// field) a shadow register is not required.
	ECanbRegs.CANME.all = 0xFFFFFFFF; // after this message id can not be changed

	// Specify that 8 bits will be sent/received
	ECanbMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX2.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX3.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX4.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX5.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX6.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX7.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX8.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX9.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX10.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX11.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX12.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX13.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX14.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX15.MSGCTRL.bit.DLC = 8;

	// enable sys clock for ecanb
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 1;    // eCAN-B
	EDIS;

	// Configure the eCAN for self test mode
	// Enable the enhanced features of the eCAN.
	EALLOW;
	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.STM = 1;    // Configure CAN for self-test mode
	ECanbShadow.CANMC.bit.STM = 0;    // Configure CAN for normal mode
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
	EDIS;

	//Initial for interupt for receiving event only
	EALLOW;
	ECanbRegs.CANMIM.all = 0xFFFF0000;
	EDIS;

	EALLOW;
	ECanbRegs.CANMIL.all = 0xFFFF0000;
	EDIS;

	EALLOW;
	ECanbShadow.CANGIF1.all = ECanbRegs.CANGIF1.all;
	ECanbShadow.CANGIF1.bit.GMIF1 = 1;
	ECanbRegs.CANGIF1.all = ECanbShadow.CANGIF1.all;
	EDIS;

	EALLOW;
	ECanbShadow.CANGIM.all = ECanbRegs.CANGIM.all;
	ECanbShadow.CANGIM.bit.GIL = 1;
	ECanbShadow.CANGIM.bit.I1EN = 1;
	ECanbRegs.CANGIM.all = ECanbShadow.CANGIM.all;
	EDIS;
}

void send_data(int16 MBXnbr, char data_index, union CAN_DATA_UNION *data) {

	volatile struct MBOX *Mailbox;
//  for(MBXnbr)
//    {
	Mailbox = &ECanbMboxes.MBOX0 + MBXnbr;

	Mailbox->MDL.word.LOW_WORD = data->c2[0];
	Mailbox->MDL.word.HI_WORD = data->c2[1];

	Mailbox->MDH.byte.BYTE4 = data_index;
	Mailbox->MDH.byte.BYTE5 = 0x00;
	Mailbox->MDH.byte.BYTE6 = 0x00;
	Mailbox->MDH.byte.BYTE7 = 0x00;
//    }
//******************used for transmit begin*****************
	ECanbRegs.CANTRS.all = 0x1 << MBXnbr;  // Set TRS for the transmit mailboxes

//	do {
//		ECanbShadow.CANTA.all = ECanbRegs.CANTA.all;
//	} while (ECanbShadow.CANTA.all == 0);   // Wait for TA5 bit to be set..

//	ECanbRegs.CANTA.all = 0x0000FFFF;   // Clear all TAn
//******************used for transmit end*****************
}

// This function reads out the contents of the indicated
// by the Mailbox number (MBXnbr).
static void mailbox_read(int16 MBXnbr) {
	volatile struct MBOX *Mailbox;
	Mailbox = &ECanbMboxes.MBOX0 + MBXnbr;
	can_msg.MDL.all = Mailbox->MDL.all; // = 0x9555AAAn (n is the MBX number)
	can_msg.MDH.all = Mailbox->MDH.all; // = 0x89ABCDEF (a constant)
	can_msg.MSGID.all = Mailbox->MSGID.all; // = 0x9555AAAn (n is the MBX number)

}

interrupt void ecan1_intb_isr(void) {
	int j;
	for (j = 16; j < 32; j++) {         // Read 16 mailboxes
		if ((ECanbRegs.CANRMP.all >> j) && 0x1) { // only read the mail box which received message

			mailbox_read(j);       // This func reads the indicated mailbox data

			ECanbShadow.CANRMP.all = ((Uint32) 1 << j);
			ECanbRegs.CANRMP.all = ECanbShadow.CANRMP.all; // clear the Received-Message-Pending Register
		}
	}

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
	new_data = TRUE;
}


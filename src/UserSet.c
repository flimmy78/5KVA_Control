//###########################################################################
//    User function file
// DSP 28335 firmware for 20kW three-level PFC
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x.h"        // DSP2833x headerfile

/*==========================================================================================
 PWM
 Relay 1,2
 LED
 Push Button
 ===========================================================================================*/

void UserSetGPIO() {

	EALLOW;

	//Initiate for PWMs of rectifier
	//PWM6 --> Phase A
	//PWM2 --> Phase B
	//PWM1 --> Phase C

	//Output PWM signals MUX

	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;  //EPWM1A
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;  //EPWM1B
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;  //EPWM2A
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;  //EPWM2B
	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;  //EPWM6A
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;  //EPWM6B

	//PWM direction
	GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;   //Output pin for EPWM1A
	GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;

	//IO pull-up
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;     //Disable pull-up on GPIO0 (EPWM1A)
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;

	//GPIO12 --> PWM Enable
	//GPIO24 --> Relay 1
	//GPIO13 --> Relay 2
	//GPIO26 --> Blinking LED
	//GPIO14 --> Run LED
	//GPIO15 --> Stop LED
	//GPIO60 --> Run Button
	//GPIO61 --> Stop Button
	//

	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;  //GPIO12
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;  //GPIO13
	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;  //GPIO14
	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;  //GPIO15
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;  //GPIO24
	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;  //GPIO24
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;  //GPIO26
	GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;  // GPIO60 as On button
	GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;  // GPIO61 as Off button

	GpioCtrlRegs.GPADIR.bit.GPIO12 = 1; 	//Output
	GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;

	GpioCtrlRegs.GPBDIR.bit.GPIO60 = 0;  // GPIO60 = INPUT
	GpioCtrlRegs.GPBDIR.bit.GPIO61 = 0;  // GPIO61 = INPUT

	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // Enable pull-up on GPIO12
	GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;

	GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;
	GpioCtrlRegs.GPBPUD.bit.GPIO61 = 0;

	EDIS;

}

/*==========================================================================================
 ����pinΪADCA0~3��ADCB0~5��ʮ�����������������EWPM4����ADC��ADC��������DMA�������
 ===========================================================================================*/

void UserSetADC(void) {
	extern void DSP28x_usDelay(Uint32 Count);

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
	ADC_cal();
	EDIS;

	AdcRegs.ADCTRL3.all = 0x00E0;  // Power up bandgap/reference/ADC circuits
	DELAY_US(ADC_usDELAY);         // Delay before converting ADC channels

	AdcRegs.ADCTRL1.bit.ACQ_PS = 3;          // S/H width in ADC module periods 
	AdcRegs.ADCTRL1.bit.CPS = 0;             // ADCTRL1[7] = 0 
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 1; // ADCLK = HSPCLK/[2^(ADCCLKPS)*(ADCTRL1[7] + 1)] = 25MHz/[2*(0 + 1)] = 12.5MHz
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;    // 0 Non-Cascaded Mode; 1 Cascaded Mode
	AdcRegs.ADCTRL1.bit.CONT_RUN = 1;        //Operate in continue mode
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;    // ??INT SEQ1,????DMA
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;  // EPWM4 trigger SOC
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;        //Reset Sequencer1 to state CONV00
	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;

//################################################################################
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 5;    // ISA -> A5
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 4;    // ISB -> A4
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 1;    // ISB -> A1

	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 2;    // VC1 -> A2
	AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 15;    // VC2 -> B7

	AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 7;    // VSA -> A7

	AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 6;    // VSB -> A6
	AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 3;    // VSC -> A3
	//###############################################################################
	//AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 7;   // CurrentW in ADCINA7 --> Result8
	//AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 9;   // Termal in  --> Result9
//	AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 12;   // IbmS in ADCINB4 --> Result10
//	AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 13;   // IcmS in ADCINB5 --> Result11

//	AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 6;    // Nothing input   --> Result6
//	AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 7;    // Nothing input   --> Result7
//	AdcRegs.ADCCHSELSEQ4.bit.CONV14 = 14;   // Nothing input   --> Result14
//	AdcRegs.ADCCHSELSEQ4.bit.CONV15 = 15;   // Nothing input   --> Result15

	AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 7;  // 10 conversions for every SOC

}

/*==========================================================================================
 PWM6 --> Phase A
 PWM1 --> Phase B
 PWM2 --> Phase C

 ===========================================================================================*/

void UserSetEPWM() {
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
	EDIS;

	EPwm1Regs.TBPRD = EPWMPeriod;                         //Period of TB = 10kHz
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1; // TBCLK = SYSCLKOUT /(HSPCLKDIV ��CLKDIV) = 150MHz / (1 ��1)
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;       //
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
	EPwm1Regs.TBPHS.all = 0;
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Count updown

	EPwm2Regs.TBPRD = EPWMPeriod;                           //
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1; // TBCLK = SYSCLKOUT /(HSPCLKDIV ��CLKDIV)
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;        //
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;            //
	EPwm2Regs.TBPHS.all = 0;                         //
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Count updown

	EPwm6Regs.TBPRD = EPWMPeriod;                         //Period of TB = 10kHz
	EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1; // TBCLK = SYSCLKOUT /(HSPCLKDIV ��CLKDIV)
	EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
	EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;          //#########################
	EPwm6Regs.TBPHS.all = 0;                          //
	EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Count updown

	EPwm4Regs.TBPRD = EPWMPeriod / 10;                    //Period of TB = 10kHz
	EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1; // TBCLK = SYSCLKOUT /(HSPCLKDIV ��CLKDIV)
	EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;
	EPwm4Regs.TBPHS.all = 0;
	EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Count updown

	EPwm5Regs.TBPRD = EPWMPeriod / 2;                     //Period of TB = 10kHz
	EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1; // TBCLK = SYSCLKOUT /(HSPCLKDIV ��CLKDIV)
	EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
	EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm5Regs.TBCTL.bit.PHSEN = TB_DISABLE;          //#########################
	EPwm5Regs.TBPHS.all = 0;
	EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Count updown

	//����Ƚ�ģ���趨
	EPwm1Regs.TBCTR = 0;  //�����ֵΪ0
	EPwm1Regs.CMPA.half.CMPA = EPWMPeriod / 1;
	EPwm1Regs.CMPB = EPWMPeriod / 2;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD; // load on CTR=Zero
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD; // load on CTR=Zero
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm2Regs.TBCTR = 0;  //�����ֵΪ0
	EPwm2Regs.CMPA.half.CMPA = EPWMPeriod / 3;
	EPwm2Regs.CMPB = EPWMPeriod / 4;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD; // load on CTR=Zero
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD; // load on CTR=Zero
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm6Regs.TBCTR = 0;  //�����ֵΪ0
	EPwm6Regs.CMPA.half.CMPA = EPWMPeriod / 2;
	EPwm6Regs.CMPB = EPWMPeriod / 3;
	EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD; // load on CTR=Zero
	EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD; // load on CTR=Zero
	EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	//  EPwm5Regs.TBCTR = 0;  //�����ֵΪ0 //#################
	//   EPwm5Regs.CMPA.half.CMPA = 0;
	//   EPwm5Regs.CMPB = EPWMPeriod/4;
	//   EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
	//   EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;
	//   EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	//    EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;//######################

// AQģ���趨
	EPwm1Regs.AQCTLA.bit.CAD = AQ_SET; // When triangle is larger than modulation ref, PWM ON
	EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	EPwm1Regs.AQCTLB.bit.CBD = AQ_SET; //#######################
	EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR; //#########################

	EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;
	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	EPwm2Regs.AQCTLB.bit.CBD = AQ_SET; //#######################
	EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR; //#########################

	EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;
	EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	EPwm6Regs.AQCTLB.bit.CBD = AQ_SET; //#######################
	EPwm6Regs.AQCTLB.bit.CBU = AQ_CLEAR; //#########################

	//  EPwm5Regs.AQCTLA.bit.CAD = AQ_SET; //#######################
	//  EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;//################
	// EPwm5Regs.AQCTLB.bit.CAD = AQ_SET; //#######################
	//  EPwm5Regs.AQCTLB.bit.CAU = AQ_CLEAR;//#########################
//DB����ģ���趨

	EPwm1Regs.DBCTL.bit.OUT_MODE = 0; //###############6
	EPwm2Regs.DBCTL.bit.OUT_MODE = 0; //###############6
	EPwm6Regs.DBCTL.bit.OUT_MODE = 0; //###############6

	/* EPwm2Regs.DBCTL.bit.IN_MODE  = DBA_ALL;
	 EPwm2Regs.DBCTL.bit.POLSEL   = DB_ACTV_HIC;
	 EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	 EPwm2Regs.DBRED = DeadTime;                        // �������ӳ٣�����ʱ����Ϊ2us
	 EPwm2Regs.DBFED = DeadTime;                        // �½����ӳ٣�����ʱ����Ϊ2us

	 EPwm6Regs.DBCTL.bit.IN_MODE  = DBA_ALL;
	 EPwm6Regs.DBCTL.bit.POLSEL   = DB_ACTV_HIC;
	 EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	 EPwm6Regs.DBRED = DeadTime;                        // �������ӳ٣�����ʱ����Ϊ2us
	 EPwm6Regs.DBFED = DeadTime;                        // �½����ӳ٣�����ʱ����Ϊ2us*/

	//  EPwm5Regs.DBCTL.bit.OUT_MODE = 0;//#################
//TZģ���趨�����
	EALLOW;
	EPwm1Regs.TZSEL.bit.CBC1 = 1;                  //TZ1Ϊ����Դ
	EPwm2Regs.TZSEL.bit.CBC1 = 1;
	EPwm6Regs.TZSEL.bit.CBC1 = 1;
	// EPwm5Regs.TZSEL.bit.CBC1 = 1;  //##########

	EPwm1Regs.TZCTL.bit.TZA = 3;                  //�¼�����EPWMA����õ�
	EPwm1Regs.TZCTL.bit.TZB = 3;
	EPwm2Regs.TZCTL.bit.TZA = 3;
	EPwm2Regs.TZCTL.bit.TZB = 3;
	EPwm6Regs.TZCTL.bit.TZA = 3;
	EPwm6Regs.TZCTL.bit.TZB = 3;

	// EPwm5Regs.TZCTL.bit.TZA = 3; //###############
	// EPwm5Regs.TZCTL.bit.TZB = 3;//###########

	EPwm1Regs.TZFRC.bit.OST = 1; //Forces a one-shot trip event and sets the TZFLG[OST] bit.
	EPwm2Regs.TZFRC.bit.OST = 1;
	EPwm6Regs.TZFRC.bit.OST = 1;
	//  EPwm5Regs.TZFRC.bit.OST = 1;//##############
	EDIS;

//�¼��趨

	/*

	 EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // 001 Enable event time-base counter equal to zero. (TBCTR = 0x0000)
	 EPwm1Regs.ETSEL.bit.INTEN = 1;                // 1 Enable EPWMx_INT generation
	 //EPwm1Regs.ETPS.bit.INTPRD = ET_2ND;           // Generate INT on 1st event

	 EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event

	 */

	EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO; // 001 Enable event time-base counter equal to zero. (TBCTR = 0x0000)
	EPwm5Regs.ETSEL.bit.INTEN = 1;              // 1 Enable EPWMx_INT generation
	//EPwm1Regs.ETPS.bit.INTPRD = ET_2ND;           // Generate INT on 1st event

	EPwm5Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event

	EPwm4Regs.ETSEL.bit.SOCAEN = 1; // 1 Enable the ADC Start of Conversion A (EPWMxSOCA) Pulse, Enable EPWMxSOCA pulse.
	EPwm4Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;     //ZERO ����ADC_A
	EPwm4Regs.ETPS.bit.SOCAPRD = ET_1ST;

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;      // start all the TB clocks
	EDIS;
}

/*==========================================================================================
 ����1Ϊ�������䣬����11Ϊ��������

 ===========================================================================================*/
void UserSetCana() {
	struct ECAN_REGS ECanaShadow;

	EALLOW;
	ECanaRegs.CANMIL.all = 0x00000000;             //�����ж����ȼ�
	ECanaShadow.CANGIM.all = ECanaRegs.CANGIM.all; //
	ECanaShadow.CANGIM.all = 0x00000000;
	ECanaShadow.CANGIM.bit.EPIM = 1;
	ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;
	EDIS;

	ECanaRegs.CANME.all = 0x00000000;                 //disable mailbox
	ECanaMboxes.MBOX1.MSGID.all = 0x00080000;	// Sending   Mailbox Flag : 2
	ECanaMboxes.MBOX11.MSGID.all = 0x00040000;	// Receiving Mailbox Flag : 1

	ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 5;					// 5 bytes
	ECanaMboxes.MBOX11.MSGCTRL.bit.DLC = 5;
	ECanaMboxes.MBOX1.MDL.byte.BYTE0 = 0;
	ECanaMboxes.MBOX1.MDL.byte.BYTE1 = 0;
	ECanaMboxes.MBOX1.MDL.byte.BYTE2 = 0;
	ECanaMboxes.MBOX1.MDL.byte.BYTE3 = 0;
	ECanaMboxes.MBOX1.MDH.byte.BYTE4 = 0;
	ECanaMboxes.MBOX11.MDL.byte.BYTE0 = 0;
	ECanaMboxes.MBOX11.MDL.byte.BYTE1 = 0;
	ECanaMboxes.MBOX11.MDL.byte.BYTE2 = 0;
	ECanaMboxes.MBOX11.MDL.byte.BYTE3 = 0;
	ECanaMboxes.MBOX11.MDH.byte.BYTE4 = 0;

	ECanaRegs.CANMD.all = 0xFFFFFC00;// Mail 0~9 are for sending, 10~31 for receiving
	ECanaRegs.CANME.all = 0x00000802;						// Enable Mailbox

	//EALLOW;
	//	ECanaRegs.CANMIM.all = 0xFFFFFFFF;
	//EDIS;
}

/*==========================================================================================
 ��ʼ���Զ���ı�������������Ƶ�ʡ�PI�������������ȵ�

 ===========================================================================================*/

void UserSetVar()

{
	MainState = FirstStart;           //�����״̬Ϊ��һ������
	ProFlag.all = 0x0000;        //�����״̬��־λ

	memset((void *) &ECanData, 0, sizeof(ECanData));

	memset((void *) &GridPLLPID, 0, sizeof(GridPLLPID));

	memset((void *) &SampRes, 0, sizeof(SampRes));             //����Ĵ�����ʼ��Ϊ0
	memset((void *) &GridVoltagePLL, 0, sizeof(GridVoltagePLL)); //�����ѹ�����ѹ��Ϣ
	GridVoltagePLL.Fre = 50;
	memset((void *) &BriVolDQ, 0, sizeof(BriVolDQ));             //�ű������ѹ��Ϣ
	BriVolDQ.Fre = 50;
	memset((void *) &BriVolPID_D, 0, sizeof(BriVolPID_D));       //�ű۵�ѹD��PID
	memset((void *) &BriVolPID_Q, 0, sizeof(BriVolPID_Q));       //�ű۵�ѹQ��PID
	memset((void *) &IndCurPID_D, 0, sizeof(IndCurPID_D));     //�����е���D��PID
	memset((void *) &IndCurPID_Q, 0, sizeof(IndCurPID_Q));     //�����е���Q��PID
	memset((void *) &SPWMGen, 0, sizeof(SPWMGen));             //SPWM�����Ϣ�Ĵ���
	SPWMGen.Fre = 50;
	memset((void *) &CurReference, 0, sizeof(CurReference));     //�������ָ��
	CurReference.Fre = 50;
	memset((void *) &IndCurDQ, 0, sizeof(IndCurDQ));             //ʵ�������е���
	IndCurDQ.Fre = 50;
	memset((void *) &VoltageControl, 0, sizeof(VoltageControl));
	memset((void *) &FuncCounter, 0, sizeof(FuncCounter));       // ���������
	memset((void *) &OutVolLoopPID, 0, sizeof(OutVolLoopPID));  //��ѹ�⻷PID
	memset((void *) &FrePowerDroop, 0, sizeof(FrePowerDroop));
	memset((void *) &VolRePowerDrp, 0, sizeof(VolRePowerDrp));
	memset((void *) &DeadTime_Res, 0, sizeof(DeadTime_Res));    //�����Ĵ���
	memset((void *) &DTCom, 0, sizeof(DTCom));

}

/*   end of file*/


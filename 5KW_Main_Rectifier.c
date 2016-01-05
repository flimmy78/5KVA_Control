//**********************************************
//DSP 28335 firmware for 20kW three-level PFC
//Created by TQNam 1/4/2015
//Last modified  18/8/2015
//**********************************************


#include "DSP2833x_Device.h"     	// DSP2833x Headerfile
#include "DSP2833x.h"        // DSP2833x headerfile
#include "main.h"
#include "RS232.h"

#pragma DATA_SECTION(DMA_Buf1,"DMARAML4");
  volatile Uint16 DMA_Buf1[50] = {0};
#pragma DATA_SECTION(ADC_Result,"DMARAML4");
  volatile float ADC_Result[10] = {0};

#pragma CODE_SECTION(User_EPWM1_isr, "ramfuncs");          // ePWM interrupt
#pragma CODE_SECTION(cpu_timer0_isr, "ramfuncs");
#pragma CODE_SECTION(adc_isr, "ramfuncs");


interrupt void cpu_timer1_isr(void);
interrupt void cpu_timer2_isr(void);
interrupt void User_EPWM1_isr(void);
interrupt void cpu_timer0_isr(void);
interrupt void adc_isr(void);
interrupt void scic_rx_isr(void);
interrupt void ecan1_intb_isr(void);


void Offset_ADC(void);
void PLL(void);
void Closeloop(void);
void TwoLoop(void);
void Scan_button(void);
void InitSpiA();
void set_DA(Uint16 Value,Uint16 CHN);
void InitSciaGpio();
void scic_init(void);
void scic_fifo_init(void);
void scia_xmit(int a);
void scia_msg(char *msg);
void rxapp_RS232();
void transmit_SCI(void);
void Config_ECan(void);
void Send_DATA_Inverter(Uint16 a);
void Send_data_eCAN(void);
void Turn_off_Converter(void);
void Check_SCI(void);


int16 j=0;
int16 m=0;
Uint16 n=0;


extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

extern Uint16 GUIdata[12];

int16 t_cnt0 = 0;

int REC_ON = 0, REC_OFF = 1;

int ON_BUTTON = 1, OFF_BUTTON = 0, Inverter_On = 0;

Uint32 ADC_cnt = 0;

Uint32 PWM_cnt = 0;

Uint16 ReceivedChar;

char *msg;

float Vcap_ref = 0;

float Ia = 0, Ib = 0, Ic = 0;

float Vga = 0, Vgb = 0, Vgc = 0;

float VC1 = 0, VC2 = 0;


float Va_abs, Vb_abs, Vc_abs;


float Va_abs1, Vb_abs1, Vc_abs1;


float Ia_abs, Ib_abs, Ic_abs;


//float Ipeak_limit = 0, V_limit = 0;

float VGD, VGQ;

float VGD1, VGQ1;

float VGD_LPF = 0, VGQ_LPF = 0;

float Vga_PLL, Vgb_PLL, Vgc_PLL;

float wgrid = 0;

float theta_e = 0;				//PHASE ANGLE

float sin_theta1 = 0, cos_theta1 = 0, sin_theta2 = 0, cos_theta2 = 0, sin_theta3 = 0, cos_theta3 = 0;

float ERR_VGQ = 0, ERR_VGQ1 = 0;

float KpPLL = 3, KiPLL = 100;

float OUT_PLL = 0;

Uint16 Init_Timer = 0;

Uint16 Offset_done = 0;

Uint16 Pre_charge_cnt = 0, Pre_charge_start = 0;

Uint16 Pre_charge_done = 0;

Uint16 ECAN_send = 0;

float Vdc_ref_init = 0;

Uint16 T1_start = 0;

Uint16 SCI_cnt = 0, SCI_Receive = 0;

Uint16 SCI_data;

Uint16 INT_Timer2 = 0, Start_Control = 0;


struct ECAN_REGS ECanbShadow;

Uint16 Int_PWM = 0;
Uint16 Int_ADC = 0;
Uint16 Int_CAN = 0;
Uint16 Int_SCI = 0;
Uint16 Int_Timer0 = 0;
Uint16 Int_Timer1 = 0;

Uint16 CAN_error = 0, CAN_receive = 0;

Uint16 Send_CAN_request = 0, send_CAN_done = 0;

// Global variable for this example
Uint32  ErrorCount = 0;
Uint32  PassCount = 0;
Uint32  Transfer = 0;
Uint32  Receive = 0;

Uint32 Counter = 0;

Uint32  TestMbox1 = 0;
Uint32  TestMbox2 = 0;
Uint32  TestMbox3 = 0;


void main(void)
{


  InitSysCtrl();    //DSP28_PLLCR = 10, DSP28_DIVSEL = 1/2 
  
  DINT;
  
  MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart); //copy flash program into RAM

  InitFlash();     //

  UserSetGPIO();   //Define IO function and MUX selection 
  

  UserSetInt();   // Initialize interrupt and enable interrupt

  UserSetADC();   //Initialization and set of ADC

  UserSetDMA();   //set DMA module

  InitCpuTimers();

  ConfigCpuTimer(&CpuTimer0, 150, 5);   	//Timer0 	5us

  ConfigCpuTimer(&CpuTimer1, 150, 500000);		//Timer1 	500ms

  ConfigCpuTimer(&CpuTimer2, 150, 5000);		//Timer2 	5ms	to close relay

  //StartCpuTimer0();                        //Start Timer0

  StartCpuTimer1();

  StopCpuTimer0();

  StopCpuTimer2();

  //EnableDog();                            //Enable WatchDog£¬overflow time is 6.67ns*516*32*256=28.2ms
  
  UserSetEPWM();                         //set and start EPWM

  //UserSetVar();                     //Initialize user's variables

  InitSpiA();

  InitECanbGpio();

  InitECanb(); // Initialize eCAN-B module

  Config_ECan();

  InitScicGpio();		//Initialize SCIC module

  scic_fifo_init();

  scic_init();




  //DAC for setting protection


    set_DA(850,0);		//DAC=850 ~ 1.2V output DAC ~ 360 peak voltage protection

    						 //K = 2.3

    //set_DA(923,1);		//DAC=923 ~ 1.35V output DAC ~ 55A current protection

    	  	  	  	  	  	  //K = 16.78

    set_DA(1020,1);		//temporary



    set_DA(990,2);		//DAC=850 ~ 1.4V output DAC ~ 420 DC-link voltage protection

    	  	  	  	  	  	  //K = 2.357

    //set_DA(923,3);		//DAC=923 ~ 1.35V output DAC ~ 55A current protection


    set_DA(1020,3);		//DAC=923 ~ 1.35V output DAC ~ 55A current protection

    	  	  	  	  	  	  //temporary


    set_DA(1000,4);		//Temprature protection



    set_DA(0,5);		//DC-link voltage protection



  	REC_ON = 0;
  	REC_OFF = 1;

  	DISABLE_ALL_PWM();

    LED_RUN_OFF();
  	LED_STOP_ON();

  	Clear_check_pin();

  	Turn_off_Converter();



  	while(1)

  	{

  		if(Offset_done == 0)			//Wait for ADC offset complete
		{

  			Turn_off_Converter();

			if(Init_Timer > 5)		//Delay 2.5s
			{

				Offset_ADC();

				Init_Timer = 0;

				Offset_done = 1;
			}


		}

  		else
  		{

  		if ((VC1>V_limit)|(VC2>V_limit))    //trip if Vc1,Vc2 > 420V
		{
  			Turn_off_Converter();
		}


		if ((Ia_abs>Ipeak_limit)|(Ib_abs>Ipeak_limit)|(Ic_abs>Ipeak_limit))      //trip if overcurrent 50A
		{
			Turn_off_Converter();
		}


		Scan_button();


		//if((CAN_error == 1)&&(CAN_receive == 0))		//Error CAN communication, time out

		/*

		if(CAN_error == 1)
		{

			CAN_error = 0;

			Turn_off_Converter();

			//Pre_charge_done = 0;

			//Send_DATA_Inverter(0);		//No CAN connection, no need to send over CAN
		}

  		*/

		Check_SCI();


		if((Pre_charge_done == 0)&&(Pre_charge_start == 1)&&(Pre_charge_cnt >= 2))			//5 seconds for pre-charge period
		{

		Pre_charge_done = 1;

		Pre_charge_start = 0;


		Send_DATA_Inverter(1);		//Send data over CAN

		StartCpuTimer2();			//Start Timer 2

		REC_ON = 1;

		REC_OFF = 0;

		LED_RUN_ON();

		LED_STOP_OFF();

		Vcap_ref = (VC1 + VC2)/2;

		}

  		}


  	}

}


void Check_SCI()
{
	if((Int_SCI == 1)&&(SCI_data == 10))
	{

		Int_SCI = 0;

		Pre_charge_start = 1;

		//CLOSE_RELAY_2();		//Close Main Relay

		//ENABLE_ALL_PWM();

		CLOSE_RELAY_1();		//Close Pre-charge Relay


	}
	else if((Int_SCI == 1)&&(SCI_data == 13))
	{

		Int_SCI = 0;

		REC_ON = 0;

		REC_OFF = 1;

		DISABLE_ALL_PWM();

		OPEN_RELAY_2();

		OPEN_RELAY_1();


		if(Pre_charge_done == 1)
		{
			Pre_charge_done = 0;

			Send_DATA_Inverter(0);
		}

		Turn_off_Converter();


		Vcap_ref = 0;

	}

}



void Scan_button()
{

	ON_BUTTON = GpioDataRegs.GPBDAT.bit.GPIO61;		//Scan On button

	OFF_BUTTON = GpioDataRegs.GPBDAT.bit.GPIO60;	//Scan Off button


	if((ON_BUTTON == 0)&&(OFF_BUTTON == 1))
	{

		Pre_charge_start = 1;

		//CLOSE_RELAY_2();		//Close Main Relay

		//ENABLE_ALL_PWM();

		CLOSE_RELAY_1();		//Close Pre-charge Relay


	}

	else if(OFF_BUTTON == 0)
	{

		REC_ON = 0;

		REC_OFF = 1;

		DISABLE_ALL_PWM();

		OPEN_RELAY_2();

		OPEN_RELAY_1();


		if(Pre_charge_done == 1)
		{
			Pre_charge_done = 0;

			Send_DATA_Inverter(0);
		}

		Turn_off_Converter();


		Vcap_ref = 0;

	}
}


void Turn_off_Converter(void)
{

	REC_ON = 0;

	REC_OFF = 1;

	DISABLE_ALL_PWM();

	OPEN_RELAY_2();

	OPEN_RELAY_1();

	LED_RUN_OFF();

	LED_STOP_ON();

	Vcap_ref = 0;

	Pre_charge_start = 0;		//For pre-charge

	Pre_charge_done = 0;

	Pre_charge_cnt = 0;

	Inverter_On = 0;

	CAN_receive = 0;

	Start_Control = 0;

	OutVolLoopPID1.PIDU = 0;

	OutVolLoopPID2.PIDU = 0;

	IndCurPID1.PIDU = 0;

	IndCurPID2.PIDU = 0;

	IndCurPID3.PIDU = 0;

	IndCurPID4.PIDU = 0;

	IndCurPID5.PIDU = 0;

	IndCurPID6.PIDU = 0;

	//Offset_done = 0;			//No adc offset again, if DC-link exist, controller can go wrong

}



void Offset_ADC()
{


    int16 kk = 0;

      for(kk=0;kk<10;kk++)
      {
        ADC_Result[kk] = (DMA_Buf1[kk]+DMA_Buf1[kk+10]+DMA_Buf1[kk+20]+DMA_Buf1[kk+30]+DMA_Buf1[kk+40])*0.2;

      }

      OffSetRes.OS_IndCurAn = (float)ADC_Result[0];
      OffSetRes.OS_IndCurBn = (float)ADC_Result[1];
      OffSetRes.OS_IndCurCn = (float)ADC_Result[2];

      OffSetRes.OS_DCBusVol_1 = (float)ADC_Result[3];
      OffSetRes.OS_DCBusVol_2 = (float)ADC_Result[4];

      OffSetRes.OS_VolUn =  (float)ADC_Result[5];
      OffSetRes.OS_VolVn =  (float)ADC_Result[6];
      OffSetRes.OS_VolWn =   (float)ADC_Result[7];


 }


/*==========================================================================================
 Calculate the average value of 5 ADC£¬then minus the offset£¬get the sampling voltage/current value of last switching period
===========================================================================================*/ 

void ADC_Conversion()
{
  int16 k = 0;
  
  for(k=0;k<8;k++)
  {
    ADC_Result[k] = (DMA_Buf1[k]+DMA_Buf1[k+10]+DMA_Buf1[k+20]+DMA_Buf1[k+30]+DMA_Buf1[k+40])*0.2;

  }


  //Clear_check_pin();


  //Scale all current and voltage sensor again

 
  SampRes1.IndCurAn = KcurA * ((float)ADC_Result[0] - OffSetRes.OS_IndCurAn);
  SampRes1.IndCurBn = KcurB * ((float)ADC_Result[1] - OffSetRes.OS_IndCurBn);
  SampRes1.IndCurCn = KcurC * ((float)ADC_Result[2] - OffSetRes.OS_IndCurCn);

  SampRes1.DCBusVol_1n = Kdc_samp1 * ((float)ADC_Result[3] - OffSetRes.OS_DCBusVol_1);
  SampRes1.DCBusVol_2n = Kdc_samp2 * ((float)ADC_Result[4] - OffSetRes.OS_DCBusVol_2);

  SampRes1.VolUn =  -Kac_sampU * ((float)ADC_Result[5] - OffSetRes.OS_VolUn);  			//Minus due to reverse ADC pin on DSP control board
  SampRes1.VolVn =  Kac_sampV * ((float)ADC_Result[6] - OffSetRes.OS_VolVn);
  SampRes1.VolWn =  Kac_sampW * ((float)ADC_Result[7] - OffSetRes.OS_VolWn);




	VC1 = SampRes1.DCBusVol_1n;

	VC2 = SampRes1.DCBusVol_2n;


	Vga = SampRes1.VolUn;
	Vgb = SampRes1.VolVn;
	Vgc = SampRes1.VolWn;



	if(Vga >= 0)
		Va_abs = Vga;
	else
		Va_abs = -Vga;


	if(Vgb >= 0)
	  		Vb_abs = Vgb;
	  	else
	  		Vb_abs = -Vgb;


	if(Vgc >= 0)
	  		Vc_abs = Vgc;
	  	else
	  		Vc_abs = -Vgc;



	//Va_abs = abs(Vga);
	//Vb_abs = abs(Vgb);
	//Vc_abs = abs(Vgc);



	Ia = SampRes1.IndCurAn;
	Ib = SampRes1.IndCurBn;
	Ic = SampRes1.IndCurCn;


	//Ic = -(Ia + Ib);


	if(Ia >= 0)
		Ia_abs = Ia;
	else
		Ia_abs = -Ia;

	if(Ib >= 0)
	  		Ib_abs = Ib;
	  	else
	  		Ib_abs = -Ib;

	if(Ic >= 0)
	  		Ic_abs = Ic;
	  	else
	  		Ic_abs = -Ic;



	//Ia_abs = abs(Ia);
	//Ib_abs = abs(Ib);
	//Ic_abs = abs(Ic);



}



void PLL()
{

		//ABC-->DQ

		VGD = (Vga*cos_theta1 + Vgb*cos_theta2 + Vgc*cos_theta3)*0.666667;

		VGQ = (Vga*sin_theta1 + Vgb*sin_theta2 + Vgc*sin_theta3)*0.666667;


		//LPF

		VGD_LPF = 0.9979078*VGD_LPF + 0.0010461021*(VGD + VGD1);		//10Hz at 30kHz

		VGQ_LPF = 0.9979078*VGQ_LPF + 0.0010461021*(VGQ + VGQ1);		//10Hz at 30kHz


		//VGD_LPF = 0.99843044*VGD_LPF + 0.0007847818*(VGD + VGD1);		//10Hz at 40kHz

		//VGQ_LPF = 0.99843044*VGQ_LPF + 0.0007847818*(VGQ + VGQ1);		//10Hz at 40kHz


		VGD1 = VGD;

		VGQ1 = VGQ;


		ERR_VGQ1 = ERR_VGQ;

		ERR_VGQ = -VGQ_LPF;

		OUT_PLL = OUT_PLL + KpPLL*(ERR_VGQ - ERR_VGQ1) + KiPLL*T_SW*ERR_VGQ;

		if(OUT_PLL > 500)
		{
			OUT_PLL = 500;
		}
		if(OUT_PLL < 0)
		{
			OUT_PLL = 0;
		}

		wgrid = OUT_PLL;

		theta_e = theta_e + wgrid*T_SW;

		if (theta_e >= 6.2831853)
		{
		theta_e = theta_e - 6.2831853;
		}


		sin_theta1 = sin(theta_e);
		cos_theta1 = cos(theta_e);

		sin_theta2 = sin(theta_e - 6.2831853/3);
		cos_theta2 = cos(theta_e - 6.2831853/3);

		sin_theta3 = sin(theta_e + 6.2831853/3);
		cos_theta3 = cos(theta_e + 6.2831853/3);


		// DQ --> ABC

		Vga_PLL = VGD_LPF*cos_theta1 + VGQ_LPF*sin_theta1;

		Vgb_PLL = VGD_LPF*cos_theta2 + VGQ_LPF*sin_theta2;

		Vgc_PLL = VGD_LPF*cos_theta3 + VGQ_LPF*sin_theta3;


		//Test with PLL


		if(Vga_PLL >= 0)
			Va_abs1 = Vga_PLL;
		else
			Va_abs1 = -Vga_PLL;


		if(Vgb_PLL >= 0)
				Vb_abs1 = Vgb_PLL;
			else
				Vb_abs1 = -Vgb_PLL;


		if(Vgc_PLL >= 0)
				Vc_abs1 = Vgc_PLL;
			else
				Vc_abs1 = -Vgc_PLL;


}


interrupt void cpu_timer0_isr(void)
{

	//5us

	Int_Timer0 = 1;

	CAN_error = 1;

	t_cnt0++;

	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



interrupt void cpu_timer1_isr(void)
{

	// 500ms period
	transmit_SCI();

	BLINK_LED();

	if(Offset_done == 0)			//Wait for ADC offset complete
	{
		Init_Timer++;
	}

	if(Pre_charge_start == 1)
		{

			Pre_charge_cnt++;

		}

   CpuTimer1.InterruptCount++;
   // The CPU acknowledges the interrupt.
   EDIS;
}




interrupt void cpu_timer2_isr(void)
{

	EALLOW;

	INT_Timer2 = 1;

	Start_Control = 1;

	StopCpuTimer2();

   CpuTimer2.InterruptCount++;
   // The CPU acknowledges the interrupt.
   EDIS;
}




interrupt void User_EPWM1_isr(void)
{

	//GpioDataRegs.GPASET.bit.GPIO13 = 1;		//Check calculation time

	//Scan_button();


	if(Offset_done == 1)
	{

	Set_check_pin();

	ADC_Conversion();

	//PLL();

	Va_abs1 = Va_abs;

	Vb_abs1 = Vb_abs;

	Vc_abs1 = Vc_abs;


	//Clear_check_pin();


	/*

	Scan_button();


	if((Pre_charge_start == 1)&&(Pre_charge_cnt > 20))			//10 seconds for pre-charge period
	{

		Pre_charge_done = 1;

		Pre_charge_start = 0;

		//Pre_charge_cnt = 11;

		Send_DATA_Inverter(1);

		Vcap_ref = (VC1 + VC2)/2;

	}


	*/

	//Inverter_On = 1;


	//if((REC_ON == 1)&&(Inverter_On == 1))		//Load have output

	if((REC_ON == 1)&&(Start_Control == 1))		//Delay 1ms
	{

		//T1_start++;



		if(Vcap_ref < 360)
		{

			Vcap_ref = Vcap_ref + 0.001; 			// Increase 60V reference within 1.5s
															//300V --> 360V

		}


		Closeloop();				//Closed-loop control

		ENABLE_ALL_PWM();

		CLOSE_RELAY_2();		//Close Main Relay


	}


	/*

	if((REC_ON == 1)&&(REC_OFF == 0))
			{
				//DC_voltage_control();
				//PI_current_control();
				TwoLoop();
			}


	*/


	}


   //GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;		//Check calculation time

	Clear_check_pin();

	PWM_cnt++;

	if(PWM_cnt > 50000)
	{PWM_cnt = 0;}


   EPwm5Regs.ETCLR.bit.INT = 1;               //Clear interrupt flag

   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}



interrupt void adc_isr(void)
{

	//ADC_done++;

	//OUTPUT VOLTAGE MEASUREMENT



	ADC_cnt++;

	if(ADC_cnt > 50000)
		{ADC_cnt = 0;}

	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1; // Clear INT SEQ1 bit
	// Acknowledge interrupt to PIE
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



void Send_DATA_Inverter(Uint16 a)
{

	ECanbMboxes.MBOX0.MDL.all = 0x9555AAA0;
	ECanbMboxes.MBOX0.MDH.all = a;

	ECanbRegs.CANTRS.bit.TRS0 = 1;  // Set TRS for T0 transmit mailboxes


	//Without Timer

	/*

	Set_check_pin();

	do
		{
		    ECanbShadow.CANTA.all = ECanbRegs.CANTA.all;

		} while(ECanbShadow.CANTA.bit.TA0 != 1); 		// Wait for CCE bit to be  cleared.





	//while(ECanbRegs.CANTA.all != 1 ) {}  // Wait for all TAn bits to be set..

	ECanbRegs.CANTA.bit.TA0 = 1;   // Clear all TAn


	do
			{
			    ECanbShadow.CANTA.all = ECanbRegs.CANTA.all;
			} while(ECanbShadow.CANTA.bit.TA0 != 0); 		// Wait for CCE bit to be  cleared.


	Clear_check_pin();

	*/



	//With Timer

	//Set_check_pin();

	StartCpuTimer0();                        //Start Timer0

		do
			{
			    ECanbShadow.CANTA.all = ECanbRegs.CANTA.all;

			} while((ECanbShadow.CANTA.bit.TA0 != 1)&&(Int_Timer0 == 0)); 		// Wait for CCE bit to be  cleared.

		Int_Timer0 = 0;

		StopCpuTimer0();                        //Start Timer0

		//Clear_check_pin();

		//while(ECanbRegs.CANTA.all != 1 ) {}  // Wait for all TAn bits to be set..

		ECanbRegs.CANTA.bit.TA0 = 1;   // Clear all TAn


		StartCpuTimer0();                        //Start Timer0


		do
				{
				    ECanbShadow.CANTA.all = ECanbRegs.CANTA.all;
				} while((ECanbShadow.CANTA.bit.TA0 != 0)&&(Int_Timer0 == 0)); 		// Wait for CCE bit to be  cleared.


		Int_Timer0 = 0;


		StopCpuTimer0();



	ECAN_send++;

}




/*float Cal_RMS(float sampdata[200])
{
  float rms=0;
  int count=0;
  for(count = 0; count < 200; count++ )
  {
    rms += sampdata[count] * sampdata[count] ;
  }

  rms = rms / 200;
  rms = sqrt(rms);
  return(rms);
} */




// Transmit a character from the SCI
void scia_xmit(int a)
{
    while (ScicRegs.SCIFFTX.bit.TXFFST !=0) {}
    ScicRegs.SCITXBUF=a;

}

void scia_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scia_xmit(msg[i]);
        i++;
    }
}


void transmit_SCI()
{
	//int j;

	//ScicRegs.SCITXBUF = (Uint16)(15);		//Address

	//while (ScicRegs.SCIFFTX.bit.TXFFST !=0) {}
	    ScicRegs.SCITXBUF= 15;

	//ScicRegs.SCITXBUF = (Uint16)(11);		//Number of bytes = 12

	//while (ScicRegs.SCIFFTX.bit.TXFFST !=0) {}
		ScicRegs.SCITXBUF= 11;

	//ScicRegs.SCITXBUF = (Uint16)(VLA_RMS);		//Voltage
	//ScicRegs.SCITXBUF = (Uint16)(VLB_RMS);
	//ScicRegs.SCITXBUF = (Uint16)(VLC_RMS);
	//ScicRegs.SCITXBUF = (Uint16)(ILA_RMS*10);	//Current
	//ScicRegs.SCITXBUF = (Uint16)(ILA_RMS*10);
	//ScicRegs.SCITXBUF = (Uint16)(ILA_RMS*10);
	//ScicRegs.SCITXBUF = (Uint16)(PLOAD/100);	//Active power
	//ScicRegs.SCITXBUF = (Uint16)(PLOAD)%100;
	//ScicRegs.SCITXBUF = (Uint16)(QLOAD/100);	//Reactive power
	//ScicRegs.SCITXBUF = (Uint16)(QLOAD)%100;
	//ScicRegs.SCITXBUF = (Uint16)(INV_ON);		//INV On = 1, Off = 0

	SCI_cnt++;

}


interrupt void scic_rx_isr(void)
{

	//int SCI_data;

	SCI_data = ScicRegs.SCIRXBUF.all;

	SCI_Receive++;

	//ScicRegs.SCITXBUF = SCI_data;		//Send data

	Int_SCI = 1;

	//while (ScicRegs.SCIFFTX.bit.TXFFST !=0) {}
		    ScicRegs.SCITXBUF= SCI_data;



	ScicRegs.SCIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag

	ScicRegs.SCIFFRX.bit.RXFIFORESET=1;

	PieCtrlRegs.PIEIFR8.bit.INTx5 = 0;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;       // Issue PIE ack

}


// This function reads out the contents of the indicated
// by the Mailbox number (MBXnbr).
void mailbox_read(int16 MBXnbr)
{
   volatile struct MBOX *Mailbox;
   Mailbox = &ECanbMboxes.MBOX0 + MBXnbr;
   TestMbox1 = Mailbox->MDL.all; // = 0x9555AAAn (n is the MBX number)
   TestMbox2 = Mailbox->MDH.all; // = 0x89ABCDEF (a constant)
   TestMbox3 = Mailbox->MSGID.all;// = 0x9555AAAn (n is the MBX number)


   //TestMbox1 = ECanbMboxes.MBOX0.MDL.all; // = 0x9555AAAn (n is the MBX number)
   //TestMbox2 = ECanbMboxes.MBOX0.MDH.all; // = 0x89ABCDEF (a constant)
   //TestMbox3 = ECanbMboxes.MBOX0.MSGID.all;// = 0x9555AAAn (n is the MBX number)


} // MSGID of a rcv MBX is transmitted as the MDL data.


void mailbox_check(int32 T1, int32 T2, int32 T3)
{


    if((T1 != T3))
    {
       ErrorCount++;
    }
    else
    {
       PassCount++;

       if(T2 == 1)
       {
    	   Inverter_On = 1;

       }
       else if(T2 == 0)
       {

    	   Inverter_On = 0;

    	   Turn_off_Converter();

       }
    }
}


interrupt void ecan1_intb_isr(void)
{


	mailbox_read(2);         // This func reads the indicated mailbox data

	mailbox_check(TestMbox1,TestMbox2,TestMbox3); // Checks the received data


	Receive++;

	CAN_receive = 1;


	ECanbRegs.CANRMP.bit.RMP2 = 1;   // Clear all TAn

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;


}



void InitScicGpio()
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.
// This will enable the pullups for the specified pins.

	GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;	   // Enable pull-up for GPIO63 (SCITXDC)

/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.

	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO62 (SCIRXDC)

	//GpioCtrlRegs.GPBQSEL2.bit.GPIO63 = 3;  // Asynch input GPIO62 (SCIRXDC)

/* Configure SCI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.

	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO62 for SCIRXDC operation

	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;   // Configure GPIO63 for SCITXDC operation


	GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;  // SCITXDC = OUTPUT

	GpioCtrlRegs.GPBDIR.bit.GPIO62 = 0;  // SCIRXDC = INPUT


    EDIS;



    EALLOW;

	SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 1;   // SCI-C

	EDIS;
}



void scic_init(void)
{

    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

 	ScicRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	ScicRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	ScicRegs.SCICTL2.all =0x0002;

	//ScicRegs.SCICTL2.bit.TXINTENA =1;
	ScicRegs.SCICTL2.bit.RXBKINTENA =1;		//RX interrupt enable

	ScicRegs.SCIHBAUD    =0x0000;
    ScicRegs.SCILBAUD    = 243;		//baud rate 19200

    //ScicRegs.SCIHBAUD    =0x0001;
    //ScicRegs.SCILBAUD    = 231;		//baud rate 9600

    //ScicRegs.SCIHBAUD    =0x0000;
    //ScicRegs.SCILBAUD    =0x0028;		//baud rate 115200

	//ScicRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back

	ScicRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
}


// Initalize the SCI FIFO
void scic_fifo_init(void)
{


	ScicRegs.SCIFFTX.all=0xE040;

    ScicRegs.SCIFFRX.all=0x0021;		//interrupt when receive 1 byte
    ScicRegs.SCIFFCT.all=0x0;
	ScicRegs.SCIFFRX.bit.RXFIFORESET=1;
}


/*==========================================================================================
 The DMA module is lauched after ADC sampling, 5 ADC per switching period£¬each ADC sample 10 channel input voltage/current using DMA to transfer data
===========================================================================================*/

void UserSetDMA()
{
  DMAInitialize();      //Initialize DMA

  User_DMA_Des = &DMA_Buf1[0];
  User_DMA_Source = &AdcMirror.ADCRESULT0;  //Get the data source address and destination address

  DMACH1AddrConfig(User_DMA_Des,User_DMA_Source);
  DMACH1BurstConfig(9,1,1);               //every burst transfer 9+1 data;source address incremental;destination addr incremental
  DMACH1TransferConfig(4,-9,1);  //   every transfer have 4+1 burst, source address reset after each burst, destination addr increase
  DMACH1WrapConfig(0,0,4,0);	  //source address wrap after each burst,destination addr wrap after each 5 burst;
  DMACH1ModeConfig(DMA_SEQ1INT,PERINT_ENABLE,ONESHOT_DISABLE,CONT_ENABLE,SYNC_DISABLE,SYNC_SRC,
	                 OVRFLOW_DISABLE,SIXTEEN_BIT,CHINT_END,CHINT_DISABLE);
  //EOC enable DMA, continue mode,addr reset after transfer, wait for trigger signal
  StartDMACH1();
}




/*==========================================================================================
Initialize Interrupt Vector Table, Set EPWM1 interrupt vector
===========================================================================================*/

void UserSetInt()
{

  InitPieCtrl();           //Disable PIE interrupt, Clear interrupt flag

  IER = 0x0000;           //Disable CPU interrupt, Clear interrupt flag
  IFR = 0x0000;

  InitPieVectTable();     //// This function initializes the PIE vector table to a known state.



   EALLOW;
   PieVectTable.EPWM5_INT = &User_EPWM1_isr;		//PWM5 interrupt	//New
   PieVectTable.TINT0 = &cpu_timer0_isr;			//Timer 0
   PieVectTable.SCIRXINTC = &scic_rx_isr;			//SCI-C
   PieVectTable.XINT13 = &cpu_timer1_isr;
   PieVectTable.TINT2 = &cpu_timer2_isr;
   PieVectTable.ECAN1INTB = &ecan1_intb_isr;		//ECAN interrupt

   EDIS;

   	IER |= M_INT1;
   	IER |= M_INT3;
   	IER |= M_INT8;	//RXD-C
   	IER |= M_INT13;
   	IER |= M_INT14;
   	IER |= M_INT9;

   	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;	//timer0
   	PieCtrlRegs.PIEIER3.bit.INTx5 = 1;  	//enable ePWM5_INT		//New
   	PieCtrlRegs.PIEIER8.bit.INTx5 = 1;  // RXD-C
   	PieCtrlRegs.PIEIER9.bit.INTx8 = 1;  // ECAN-B



   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;


   // Enable global Interrupts and higher priority real-time debug events:
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM
}


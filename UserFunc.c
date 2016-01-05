
//###########################################################################
//    User function file
//###########################################################################


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x.h"            // DSP2833x headerfile
#include "main.h"


/*==========================================================================================
   DQ method for PLL£¬PI control to make Vq to be zero£¬get the grid voltage phase angle and frequency
===========================================================================================*/ 
extern float Vcap_ref, GridActiveCur, GridReactiveCur;

extern Uint16 GUIdata[12];

extern float theta_e;				//PHASE ANGLE

//float sin_theta, cos_theta;

//float Vmax = 100, Imax = 20;

float Vacontrol1 = 0, Vacontrol2 = 0, Vbcontrol1 = 0, Vbcontrol2 = 0, Vccontrol1 = 0, Vccontrol2 = 0;

//float Limit = 500;

extern float Ia, Ib, Ic;

extern float Vga, Vgb, Vgc;

extern float VC1, VC2;

extern float Va_abs, Vb_abs, Vc_abs;

extern float Va_abs1, Vb_abs1, Vc_abs1;

extern float Ia_abs, Ib_abs, Ic_abs;

extern float Vga_PLL, Vgb_PLL, Vgc_PLL;

extern float sin_theta1, cos_theta1, sin_theta2, cos_theta2, sin_theta3, cos_theta3;


void InverterOn()
{ 
  EALLOW;                              
  EPwm1Regs.TZCLR.bit.OST= 1;
  EPwm2Regs.TZCLR.bit.OST = 1;
  EPwm6Regs.TZCLR.bit.OST = 1;
  EDIS;

}
void InverterOff()
{

  EALLOW;                             
  EPwm1Regs.TZFRC.bit.OST= 1;
  EPwm2Regs.TZFRC.bit.OST = 1;
  EPwm6Regs.TZFRC.bit.OST = 1;
  EDIS;
}



void Closeloop()
{



	OutVolLoopPID1.err = Vcap_ref - (VC1 + VC2)/2;

	//OutVolLoopPID1.err = Vcap_ref*Vcap_ref - (VC1 + VC2)*(VC1 + VC2)/4;


  //OutVolLoopPID1.err = OutVolLoopPID1.Ref - SampRes1.DCBusVol_1n;

	OutVolLoopPID1.PIDU = OutVolLoopPID1.PIDU + VLKp* (OutVolLoopPID1.err - OutVolLoopPID1.err1)
                      + VLKi* T_SW * OutVolLoopPID1.err;

  if(OutVolLoopPID1.PIDU > Imax)     //Peak current reference
     OutVolLoopPID1.PIDU = Imax;

  if(OutVolLoopPID1.PIDU < 0)
     OutVolLoopPID1.PIDU =0;

  OutVolLoopPID1.err1 = OutVolLoopPID1.err ;

  //OutVolLoopPID1.PIDU1 = OutVolLoopPID1.PIDU;



//OutVolLoopPID2.Ref = DCVolRef;

OutVolLoopPID2.err = Vcap_ref - (VC1 + VC2)/2;

//OutVolLoopPID2.err = Vcap_ref*Vcap_ref - (VC1 + VC2)*(VC1 + VC2)/4;

//OutVolLoopPID2.err = OutVolLoopPID2.Ref - SampRes1.DCBusVol_2n;

OutVolLoopPID2.PIDU = OutVolLoopPID2.PIDU + VLKp* (OutVolLoopPID2.err - OutVolLoopPID2.err1)
                    + VLKi* T_SW * OutVolLoopPID2.err;

if(OutVolLoopPID2.PIDU > Imax)     	//Peak current reference
   OutVolLoopPID2.PIDU = Imax;

if(OutVolLoopPID2.PIDU < 0)
   OutVolLoopPID2.PIDU =0;

OutVolLoopPID2.err1 = OutVolLoopPID2.err ;



  //Nam

  //Virtual supply voltage generation

  //update output voltage phase angle

/*


  	theta_e = theta_e + 314.159*T_SW;

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




  	Vga = 70*cos_theta1;

  	Vgb = 70*cos_theta2;

  	Vgc = 70*cos_theta3;



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



  	OutVolLoopPID1.PIDU = 25;

  	OutVolLoopPID2.PIDU = 25;

  	*/


//####### Phase_A
IndCurPID1.Ref = OutVolLoopPID1.PIDU*Va_abs1/Vmax;

//IndCurPID1.err = IndCurPID1.Ref - abs(SampRes1.IndCurAf);

IndCurPID1.err = IndCurPID1.Ref - Ia_abs;

IndCurPID1.PIDU = IndCurPID1.PIDU + CLKp * (IndCurPID1.err - IndCurPID1.err1)
                   + CLKi* T_SW * IndCurPID1.err;

		if(IndCurPID1.PIDU > Limit)

			IndCurPID1.PIDU = Limit;

		if(IndCurPID1.PIDU < -Limit)

			IndCurPID1.PIDU = -Limit;




		Vacontrol1 = IndCurPID1.PIDU + EPWMPeriod*(1 - Va_abs/Vcap_ref);


//Vacontrol1 = IndCurPID1.PIDU;

		if(Vacontrol1 > EPWMPeriod)
		{
		Vacontrol1 = EPWMPeriod;
		}
		if(Vacontrol1 < 0)
		{
		Vacontrol1 = 0;
		}

//IndCurPID1.PIDU1 = IndCurPID1.PIDU;
IndCurPID1.err1 = IndCurPID1.err ;



IndCurPID2.Ref = OutVolLoopPID2.PIDU*Va_abs1/Vmax;

//IndCurPID2.err = IndCurPID2.Ref - abs(SampRes1.IndCurAf);

IndCurPID2.err = IndCurPID2.Ref - Ia_abs;

IndCurPID2.PIDU = IndCurPID2.PIDU + CLKp * (IndCurPID2.err - IndCurPID2.err1)
                   + CLKi* T_SW * IndCurPID2.err;


		if(IndCurPID2.PIDU > Limit)

			IndCurPID2.PIDU = Limit;

		if(IndCurPID2.PIDU < -Limit)

			IndCurPID2.PIDU = -Limit;


		Vacontrol2 = IndCurPID2.PIDU + EPWMPeriod*(1 - Va_abs/Vcap_ref);



//Vacontrol2 = IndCurPID2.PIDU;


		if(Vacontrol2 > EPWMPeriod)
		{
		Vacontrol2 = EPWMPeriod;
		}
		if(Vacontrol2 < 0)
		{
		Vacontrol2 = 0;
		}



//IndCurPID2.PIDU1 = IndCurPID2.PIDU;
IndCurPID2.err1 = IndCurPID2.err ;


//####### Phase_B
IndCurPID3.Ref = OutVolLoopPID1.PIDU*Vb_abs1/Vmax;

//IndCurPID3.err = IndCurPID3.Ref - abs(SampRes1.IndCurBf);

IndCurPID3.err = IndCurPID3.Ref - Ib_abs;

IndCurPID3.PIDU = IndCurPID3.PIDU + CLKp * (IndCurPID3.err - IndCurPID3.err1)
                   + CLKi* T_SW * IndCurPID3.err;


	if(IndCurPID3.PIDU > Limit)

			IndCurPID3.PIDU = Limit;

		if(IndCurPID3.PIDU < -Limit)

			IndCurPID3.PIDU = -Limit;


		Vbcontrol1 = IndCurPID3.PIDU + EPWMPeriod*(1 - Vb_abs/Vcap_ref);



//Vbcontrol1 = IndCurPID3.PIDU;


		if(Vbcontrol1 > EPWMPeriod)
		{
		Vbcontrol1 = EPWMPeriod;
		}
		if(Vbcontrol1 < 0)
		{
		Vbcontrol1 = 0;
		}


//IndCurPID3.PIDU1 = IndCurPID3.PIDU;
IndCurPID3.err1 = IndCurPID3.err ;



IndCurPID4.Ref = OutVolLoopPID2.PIDU*Vb_abs1/Vmax;

//IndCurPID4.err = IndCurPID4.Ref - abs(SampRes1.IndCurBf);

IndCurPID4.err = IndCurPID4.Ref - Ib_abs;

IndCurPID4.PIDU = IndCurPID4.PIDU + CLKp * (IndCurPID4.err - IndCurPID4.err1)
                   + CLKi* T_SW * IndCurPID4.err;

		if(IndCurPID4.PIDU > Limit)

			IndCurPID4.PIDU = Limit;

		if(IndCurPID4.PIDU < -Limit)

			IndCurPID4.PIDU = -Limit;


		Vbcontrol2 = IndCurPID4.PIDU + EPWMPeriod*(1 - Vb_abs/Vcap_ref);



//Vbcontrol2 = IndCurPID4.PIDU;


		if(Vbcontrol2 > EPWMPeriod)
		{
		Vbcontrol2 = EPWMPeriod;
		}
		if(Vbcontrol2 < 0)
		{
		Vbcontrol2 = 0;
		}

//IndCurPID4.PIDU1 = IndCurPID4.PIDU;
IndCurPID4.err1 = IndCurPID4.err ;



//####### Phase_C
IndCurPID5.Ref = OutVolLoopPID1.PIDU*Vc_abs1/Vmax;

//IndCurPID5.err = IndCurPID5.Ref - abs(SampRes1.IndCurCf);

IndCurPID5.err = IndCurPID5.Ref - Ic_abs;

IndCurPID5.PIDU = IndCurPID5.PIDU + CLKp * (IndCurPID5.err - IndCurPID5.err1)
                   + CLKi* T_SW * IndCurPID5.err;


		if(IndCurPID5.PIDU > Limit)

			IndCurPID5.PIDU = Limit;

		if(IndCurPID5.PIDU < -Limit)

			IndCurPID5.PIDU = -Limit;


		Vccontrol1 = IndCurPID5.PIDU + EPWMPeriod*(1 - Vc_abs/Vcap_ref);



//Vccontrol1 = IndCurPID5.PIDU;


		if(Vccontrol1 > EPWMPeriod)
		{
		Vccontrol1 = EPWMPeriod;
		}
		if(Vccontrol1 < 0)
		{
		Vccontrol1 = 0;
		}


//IndCurPID5.PIDU1 = IndCurPID5.PIDU;
IndCurPID5.err1 = IndCurPID5.err ;


IndCurPID6.Ref = OutVolLoopPID2.PIDU*Vc_abs1/Vmax;

//IndCurPID6.err = IndCurPID6.Ref - abs(SampRes1.IndCurCf);

IndCurPID6.err = IndCurPID6.Ref - Ic_abs;

IndCurPID6.PIDU = IndCurPID6.PIDU + CLKp * (IndCurPID6.err - IndCurPID6.err1)
                   + CLKi* T_SW * IndCurPID6.err;


		if(IndCurPID6.PIDU > Limit)

			IndCurPID6.PIDU = Limit;

		if(IndCurPID6.PIDU < -Limit)

			IndCurPID6.PIDU = -Limit;



		Vccontrol2 = IndCurPID6.PIDU + EPWMPeriod*(1 - Vc_abs/Vcap_ref);



//Vccontrol2 = IndCurPID6.PIDU;


		if(Vccontrol2 > EPWMPeriod)
		{
		Vccontrol2 = EPWMPeriod;
		}
		if(Vccontrol2 < 0)
		{
		Vccontrol2 = 0;
		}

//IndCurPID6.PIDU1 = IndCurPID6.PIDU;
IndCurPID6.err1 = IndCurPID6.err ;



	  //PWM6 --> Phase A
      //PWM2 --> Phase B
      //PWM1 --> Phase C

		//PWMA for below switch
		//PWMB for upper switch

		//Check direction of voltage



		if (Vga < 0)
		{EPwm6Regs.CMPA.half.CMPA = Vacontrol2;
        EPwm6Regs.CMPB = 0;}
		else
		{EPwm6Regs.CMPA.half.CMPA = 0;
		EPwm6Regs.CMPB = Vacontrol1;}


		if (Vgb < 0)
        {EPwm2Regs.CMPA.half.CMPA = Vbcontrol2;
        EPwm2Regs.CMPB = 0;}
		else
        {EPwm2Regs.CMPA.half.CMPA = 0;
        EPwm2Regs.CMPB = Vbcontrol1;}


		if (Vgc < 0)
        {EPwm1Regs.CMPA.half.CMPA = Vccontrol2;
        EPwm1Regs.CMPB = 0;}
		else
        {EPwm1Regs.CMPA.half.CMPA = 0;
        EPwm1Regs.CMPB = Vccontrol1;}


     //Test PWM signals

     /*

     EPwm1Regs.CMPA.half.CMPA = EPWMPeriod/2;
        EPwm1Regs.CMPB = EPWMPeriod/3;

        EPwm2Regs.CMPA.half.CMPA = EPWMPeriod/2;
           EPwm2Regs.CMPB = EPWMPeriod/3;

     EPwm6Regs.CMPA.half.CMPA = EPWMPeriod/2;
        EPwm6Regs.CMPB = EPWMPeriod/3;

	*/


}




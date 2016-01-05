
//###########################################################################
//    User Global Variables declare Header file 
//###########################################################################

#ifndef __LJGLOBALVARS_H__
#define __LJGLOBALVARS_H__

#include "DSP2833x_Device.h"
#include "DSP2833x.h"


//###############fsw = 40kHz Nam

  //#define T_SW 0.000025            //###############fsw = 40kHz

  //#define EPWMPeriod 1875     //###################### fsw=40kHz Nam


//###############fsw = 30kHz Nam

 //#define T_SW 0.00003333            //###############fsw = 30kHz Nam

 //#define EPWMPeriod 2500     //#################### fsw=30kHz Nam


//###############fsw = 20kHz Nam

#define T_SW 0.000025            //###############fsw = 20kHz

#define EPWMPeriod 3750     //###################### fsw=20kHz Nam

#define PD 1875				//fsample = 40kHz


// Current controller gains

//#define CLKp 55      //############Good performance at 40kHz
//#define CLKi 10     //############Good performance


#define CLKp 70      //############Test current controller

#define CLKi 10     //############


// DC-link voltage controller gains


#define VLKp 0.3 		//############Good performance
#define VLKi 30   	// ############Good performance


//#define VLKp 0.2 		//############Test value
//#define VLKi 40   		// ############Test value


//#define VLKp 0.8 		//############Test value

//#define VLKi 50   		// ############Test value


//#define VLKp 0.0025 		// Kp = 1.2/360

//#define VLKi 0.08333   		// Ki = 30/360


#define Vmax 325			//Peak input voltage


#define Vmax 325			//Peak input voltage


#define Imax 45.5		//peak input current max 31.5 RMS


#define Imax 20		//peak input current max 31.5 RMS
//todo double define

#define Ipeak_limit 58			//Set limit for software overcurrent/voltage protection


#define V_limit 440


#define Limit 2000			//Limit PI control output



#define ADC_usDELAY  5000L
#define ModeSWPeriod 1000      //


// Scale for ADC conversions



//#define Kdc_samp1 0.2040   //  ############ ###############      //Correction for DC-link voltage
//#define Kdc_samp2 0.276    //####################



#define Kdc_samp1 0.2106   //  ############ ###############      //Correction for DC-link voltage

#define Kdc_samp2 0.26191

//0.276    //####################


//#define Kac_sampU 0.2198    //############			//Correction for rectifier
//#define Kac_sampV 0.2211    //############
//#define Kac_sampW 0.22155    //############

#define Kac_sampU 0.2121    //############ correction for inverter
#define Kac_sampV 0.2121    //############
#define Kac_sampW 0.2112    //############


//#define Kac_sampU 0.2105    //############ correction for inverter
//#define Kac_sampV 0.2105    //############
//#define Kac_sampW 0.2112    //############

#define Kac_samp 0.24488     // ##########################      //


#define Kcur     0.028955  //##########     //Nam 		//change resistor at amplifier from 5.1k -> 20k
#define KcurA     0.028955
#define KcurB     0.028955
#define KcurC     0.028955



/***********************************************************************/

//CAN bus

#define ECanReply       0x00C0        //
#define ECanInfoOnly    0x003F        //

#define ECanZero32      0x00000000    //
#define ECan_InvOn      0x0001        //
#define ECan_InvOff     0x0002        //

#define ECan_Num        0x0008        //

#define ECan_DV_info    0x0006        //
#define ECan_DI_info    0x0007
#define ECan_DP_info    0x0008
#define ECan_Fau_info   0x0002
#define ECan_AV_info    0x0009
#define ECan_AI_info    0x000A
#define ECan_AP_info    0x000B
#define ECan_AQ_info    0x0013



extern Uint32 FaultDisplay;  //

struct  ECanData_STRUCT
{
	Uint32 DV;
	Uint32 DI;
	Uint32 DP;
	Uint32 Fau;
	Uint32 AV;
	Uint32 AI;
	Uint32 AP;
	Uint32 AQ;

	Uint16 i;
};

extern volatile struct ECanData_STRUCT ECanData;      //

/***********************************************************************/


// Machine status definition
enum DeviceState
{
	FirstStart,             //
	DCBusDetect,            //
	ACBusDetect,            //
	PhaseLocking,            //
	PhaseLocked,
	SoftStart,              //
	Debug,                  //
	OpenLoopRun,            //
	CloseLoopRun,           //
	Error,                   //
	OrderOff                 //
};

extern volatile enum DeviceState MainState;
/***********************************************************************/


/***********************************************************************/


// Fault status definition
enum FaultState
{
	Normal,             
	DCBusFault,           
	ACBusFault,            
	OverCurrent,       
	PLLFault
};

extern volatile enum FaultState FaultType;
/***********************************************************************/

//

#define SwON  0     //
#define SwOFF 1     //

struct SwVal_BITS                 //
{
	Uint16 Sw1:1;                 //��־λ1�� ����Sw1�ļ�ֵ��0Ϊ����1Ϊ�أ�
	Uint16 Sw2:1;                 //��־λ2�� ����Sw2�ļ�ֵ��0Ϊ����1Ϊ�أ�
	Uint16 Sw3:1;                 //��־λ3�� ����Sw3�ļ�ֵ��0Ϊ����1Ϊ�أ�
	Uint16 Sw4:1;                 //��־λ4�� ����Sw4�ļ�ֵ��0Ϊ����1Ϊ�أ�
	Uint16 Sw5:1;                 //��־λ5�� ����Sw5�ļ�ֵ��0Ϊ����1Ϊ�أ�
	Uint16 rsvd6:1;               //��־λ6�� ��ʱ����
	Uint16 rsvd7:1;               //��־λ7�� ��ʱ����
	Uint16 rsvd8:1;               //��־λ8�� ��ʱ����
	Uint16 rsvd9:1;               //��־λ9�� ��ʱ����
	Uint16 rsvd10:1;              //��־λ10�� ��ʱ����
	Uint16 rsvd11:1;              //��־λ11�� ��ʱ����
	Uint16 rsvd12:1;              //��־λ12�� ��ʱ����
	Uint16 rsvd13:1;              //��־λ13�� ��ʱ����
	Uint16 rsvd14:1;              //
	Uint16 rsvd15:1;              //
	Uint16 rsvd16:1;              //
};

union SwVal_Var
{
	Uint16            all;
	struct SwVal_BITS bit;
};
/***********************************************************************/
//Inverter status flag
struct ProFlg_BITS
{
	Uint16 ADCOffSet:1;            //��־λ1��ADCƫ����������ɱ�־��1Ϊ���
	Uint16 DCBusVol:1;             //��־λ2��ֱ��ĸ�ߵ�ѹ���־λ��1Ϊ��
	Uint16 ACBusVol:1;             //��־λ3������ĸ�ߵ�ѹ���
	Uint16 PhaseLocked:1;          //��־λ4������ɹ���־λ��1Ϊ�ɹ�
	Uint16 SoftStart:1;            //��־λ5����������ɱ�־λ
	Uint16 StartUp:1;              //��־λ9��������ɱ�־
	Uint16 Restart:1;               //��־λ10������������־������Can
	Uint16 PhaseFault:1;               //������͵�����λ�����־
	Uint16 FanOn:1;               //��־λ12�����ȿ�����־λ
	Uint16 FreFault:1;               //Ƶ�ʴ���
	Uint16 Mode:1;               //��־λ15��ռ�ձ�ƫ����K0����0.5��־λ��0��δ����1������
	Uint16 RelaySt:1;              //��־λ16���Ӵ���״̬λ��0Ϊ�Ͽ���1Ϊ�պϣ�

	Uint16 ACOverCur:1;            //��е���������־
	Uint16 DCOverCur:1;            //ֱ���������־
	Uint16 HardFault:1;            //Ӳ��������־

	Uint16 OrderOn:1;              //Turn ON command from the key
};

union ProFlg_Var
{
	Uint32            all;
	struct ProFlg_BITS bit;
};


extern volatile union ProFlg_Var ProFlag;
/***********************************************************************/

//  �����������

struct SampRes_STRUCT            // ���Դ�Ų�����Ľṹ�壬��n����Ϊδ��������ֵ����fΪ�����ĸ�����ֵ
{
	float    IndCurAn;           
	float    IndCurBn;          
	float    IndCurCn;           
	float    DCBusVoln;          
	float    CapVolABn;        
	float    CapVolBCn;          
	float    IndCurUn;           
	float    IndCurVn;           
	float    IndCurWn;           
	float    Temperaturen;      

	float     IndCurAf;         
	float     IndCurBf;        
	float     IndCurCf;        
	float     DCBusVolf;        
	float     CapVolAf;         
	float     CapVolBf;        
	float     CapVolCf;        
	float     IndCurUf;        
	float     IndCurVf;        
	float     IndCurWf;         
    float    Temperaturef;     
};

struct SampRes_STRUCT1            // ###################################

{   float    IndCurAn;
	float    IndCurBn;
	float    IndCurCn;
	float    DCBusVol_1n;
	float    DCBusVol_2n;
	float    VolUn;
	float    VolVn;
	float    VolWn;


	float    IndCurAf;
	float    IndCurBf;
	float    IndCurCf;
	float    DCBusVol_1f;
	float    DCBusVol_2f;
	float    VolUf;
	float    VolVf;
	float    VolWf;


};

extern volatile struct SampRes_STRUCT SampRes;      //��Ų�����
extern volatile struct  SampRes_STRUCT1 SampRes1; //###################################
struct OffSet_STRUCT                
{
	float    OS_IndCurAn;         
	float    OS_IndCurBn;           
	float    OS_IndCurCn;          
	float    OS_DCBusVol_1;
	float    OS_DCBusVol_2;
	float    OS_CapVolBCn;          
	float    OS_VolUn;
	float    OS_VolVn;
	float    OS_VolWn;
	float    OS_Temperaturen;       

};

extern volatile struct OffSet_STRUCT OffSetRes;   

/***********************************************************************/

struct SigGeneVars_STRUCT      // ���Բ����
{
	int16 n;                   // ��������
	
	float nwt;                 // ģ���������

	int16 InSig;               // �����ź�
	int16 InSiga;              // �����ź�a��
	int16 InSigb;              // �����ź�b��
	int16 InSigc;              // �����ź�c��
};

extern volatile struct SigGeneVars_STRUCT SigGeneVars;
/***********************************************************************/

struct DQTrans_STRUCT      // ��ת�任�ṹ��
{
	float wt;        // ���ٶ�
	float Phase;     //��λ
	float Fre;          //��תƵ��

	float32 a;         // ���ྲֹ���ϵ����
	float32 b;
	float32 c;

	float32 d;         // ������ת���ϵ����
	float32 q;
	float32 o;

	float32 dfilt;     // �������˲������
	float32 qfilt;
	float32 ofilt;

 };
extern volatile struct DQTrans_STRUCT GridVoltagePLL;  //�����ѹ����ṹ��
extern volatile struct DQTrans_STRUCT GridVoltagePLL1;  //�����ѹ����ṹ��
extern volatile struct DQTrans_STRUCT BriVolDQ;        //�ű۵�ѹDQ�ṹ��
extern volatile struct DQTrans_STRUCT SoftStartDQ;     //������DQ�ṹ��
extern volatile struct DQTrans_STRUCT SPWMGen;        //SPWM����Ĵ���
extern volatile struct DQTrans_STRUCT CurReference;    //��������ο�ֵ
extern volatile struct DQTrans_STRUCT IndCurDQ;        //��е����Ĵ���
extern volatile struct DQTrans_STRUCT DTCom;
/***********************************************************************/

struct FuncCounter_STRUCT            //����ʱ�������
{
  Uint16 CurLoopCount;               //�����ջ�������������
  Uint16 FreDetCount;                //����Ƶ�ʼ�������
  Uint16 DCBusVolCount;              //ֱ����ĸ�ߵ�ѹ
  Uint16 ACBusCount;                 //�������ѹ���
  Uint16 STCount;                    //�ű�������
  Uint16 ACCurCount;                 //������������
  Uint16 DCCurCount;                 //ֱ����������
  Uint16 CurUnbalCount;              //������ƽ����
  Uint16 RelayCount;                 //�̵�������
  Uint16 EcanCount;                  //CanͨѶ������
  Uint16 OffSetCount;     
  Uint16 FanOpeCount;               //��������������  
  Uint16 DrpModeCount1;              //�л�Droopģʽ������  
  Uint16 DrpModeCount2;       
};
extern volatile struct FuncCounter_STRUCT FuncCounter;
/***********************************************************************/

typedef struct       // PID�㷨�ṹ��
{
	float Ref;       // PID�㷨�ο�ֵ

/*
    ����PID�����ǻ�������PID����ʽ���õ�:
	u(t) = Kp*e(t) + Ki*int(e(t)) + Kd*diff(e(t))          (1)
	���У�e(t),int(e(t)),diff(e(t))�ֱ�Ϊ�������������΢���
	��PID���ʽ�����ϱ任��D(s) = Kp + Ki*1/s + Kd*s��������ɢ����ת��Ϊ��ַ��̣�
	u(k) = u(k-1) + a0*e(k) - a1*e(k-1) + a2*e(k-2)        (2)
	����,��ɢ����ΪT��ʵ�ʴ����� T = 100us)����
	a0 = Kp + Ki*T + Kd/T; a1 = Kp + 2*Kd/T; a2 = Kd/T.
	PIDCalc�ӳ�����У�����ã�2������ʽ���м���.
*/

	float pos;       // ���ԣ���ͬ��·��ȡ���Բ�ͬ����ϸ��Matlab�����е�
	                 // ��������ͼ����5d��5q��·�������ǲ�ͬ�ģ��ʴ˶���pos����
					 // ���Ա��������ԣ���ֵ������ļ��ԣ��ֱ�Ϊ1��-1���ɳ�ʼ��ʱ��ֵ��
	float err;       // ��ǰ�����err(k)
	float err1;      // ǰ������err(k-1)
	float err2;      // ǰ������err(k-2)

	float PIDU;      // ��ǰ��PID���������
	float PIDU1;     // ��һ��PID���������

}PID_STRUCT;

extern volatile PID_STRUCT GridPLLPID;       //���໷PID
extern volatile PID_STRUCT BriVolPID_D;      //�ű۵�ѹD�����PID
extern volatile PID_STRUCT BriVolPID_Q;      //�ű۵�ѹQ�����PID
extern volatile PID_STRUCT IndCurPID_D;       //��е���D�����PID
extern volatile PID_STRUCT IndCurPID_Q;       //��е���Q�����PID
extern volatile PID_STRUCT OutVolLoopPID;     //��������ѹ�⻷PID
extern volatile PID_STRUCT FrePowerDroop;     //Ƶ���й��´�PID
extern volatile PID_STRUCT VolRePowerDrp;     //��ѹ�޹��´�PID
extern volatile PID_STRUCT IndCurPID1;          //######################################
extern volatile PID_STRUCT IndCurPID2;          //######################################
extern volatile PID_STRUCT IndCurPID3;          //######################################
extern volatile PID_STRUCT IndCurPID4;          //######################################
extern volatile PID_STRUCT IndCurPID5;          //######################################
extern volatile PID_STRUCT IndCurPID6;          //######################################
extern volatile PID_STRUCT OutVolLoopPID1;               //#########################
extern volatile PID_STRUCT OutVolLoopPID2;                //#########################


struct InverterPara_STRUCT
{
     float ActivePowerCur;                  //�й�����
     float ActivePowerCurFilt;
	 float ReactivePowerCur;                //�޹�����
     float ActivePowerFreVar;               //�ù�ʹƵ��ƫ��50Hz��
};
extern volatile struct InverterPara_STRUCT VoltageControl;

//����DMAģ���ADC����
extern volatile Uint16 *User_DMA_Des,*User_DMA_Source;

struct DeadTime_STRUCT            //���������Ĵ���
{
  float IndCurAf;            //��������������˲���
  float IndCurBf;
  float IndCurCf;

  float IndCurdf;           //��������dq����
  float IndCurqf;

  int DTComA;              //����������ֵ
  int DTComB;
  int DTComC;
};
extern volatile struct DeadTime_STRUCT DeadTime_Res;

// ��������
extern void UserSetGPIO(void);               //IO�趨
extern void UserSetInt(void);                //�ж��Զ���
extern void UserSetCana();                   //CanAģ���Զ���
extern void EnableDog(void);                 //ʹ�ܿ��Ź�
extern void KickDog();                       //
extern void UserSetEPWM();              //EPWMģ���Զ���
extern void User_DQ2abc(volatile struct DQTrans_STRUCT *Trans);
extern void User_abc2DQ(volatile struct DQTrans_STRUCT *Trans);
extern void UserSetADC();                   
extern void UserSetDMA();
extern void Phase_Lock();               //�������ISR�е���
extern void SVPWM_Generate();           //SVPWM�������CurCloseLoop()�е���
extern void AcquireOffSet();           //���ֱ������ƫ��
extern void ACBusVolDet();            // �����ѹ���
extern void UserSetVar();             // ��ʼ���Զ������
extern void GridFreDet();             //����Ƶ�ʼ��
extern void DCBusVolDet();            //ֱ����ѹ���
extern void SoftStartFunc();          //�ű۵�ѹ����������
extern void InverterOn();             //��ͨռ�ձȣ�Ӳ������������
extern void InverterOff();
extern void CurrentDet();             //������ֱ����������
extern void CurCloseLoop();           //�����ջ�
extern void OutVolLoop();             //��������ѹ�⻷
extern void GridCurClose();           //��������ջ�
extern void SendUseECan(Uint32 SendData, Uint16 SendInfo);
extern void RelayOperation();
extern void CurUnbalDet();
extern void ReceiveUseECan();
extern float LowPassFilter(float PreOut,float Input, float CutFre);          //һ�׵�ͨ�˲���
extern void STPIDCom(volatile PID_STRUCT *ComCon, float Reference, float Sense);    //�ű۵�ѹ������
extern void demofun(void);
extern void PowerFreLoop(void);                                         //�й�Ƶ�ʿ��ƺ���
extern void  EcanDataSend(void);
extern void FanOperation(void);
extern void HardFaultDet(void);
extern void RePowerVolLoop(void);    //�޹���ѹ���ƻ�·
extern void DeadTimeCom(void);      //��������
#endif



//=============================================
//end of file
//==============================================

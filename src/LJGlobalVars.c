//###########################################################################
//    User Global Variables definetion file 
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x.h"        // DSP2833x headerfile

volatile enum DeviceState MainState;            //��������״̬����
volatile enum FaultState FaultType;
volatile union ProFlg_Var ProFlag;                //��ų���״̬��ر���
volatile struct SampRes_STRUCT SampRes;           //��Ų�����
volatile struct SampRes_STRUCT1 SampRes1;         //###########################

volatile struct OffSet_STRUCT OffSetRes;           //����ƫ����
volatile struct SigGeneVars_STRUCT SigGeneVars;   //ģ����Բ��β������Ⱥ

volatile struct ECanData_STRUCT ECanData;          //ECanͨѶ����

volatile Uint16 *User_DMA_Des, *User_DMA_Source;    //DMAԴ��Ŀ���ַ

//  ���໷��������
volatile PID_STRUCT GridPLLPID = { 0, 0, 0, 0, 0, 0, 0 };//��������źţ�PI�����
volatile struct DQTrans_STRUCT GridVoltagePLL = { 0, 50, 0, 0, 0, 0, 0, 0, 0, 0,
		0 };  //���������ѹ��dqֵ��Ƶ�ʣ���λ
volatile struct DQTrans_STRUCT GridVoltagePLL1 = { 0, 50, 0, 0, 0, 0, 0, 0, 0,
		0, 0 };  //###############
//�������������ű۵�ѹ���ơ����������������
volatile struct DQTrans_STRUCT SoftStartDQ = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //�����ű۵�ѹ������
volatile struct DQTrans_STRUCT BriVolDQ = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // �ű������ѹ����DQֵ
volatile PID_STRUCT BriVolPID_D = { 0, 0, 0, 0, 0, 0, 0 };      //�ű۵�ѹD�����PID
volatile PID_STRUCT BriVolPID_Q = { 0, 0, 0, 0, 0, 0, 0 };      //�ű۵�ѹQ�����PID
volatile PID_STRUCT IndCurPID_D = { 0, 0, 0, 0, 0, 0, 0 };     //��е���D�����PID
volatile PID_STRUCT IndCurPID_Q = { 0, 0, 0, 0, 0, 0, 0 };     //��е���Q�����PID

volatile PID_STRUCT IndCurPID1 = { 0, 0, 0, 0, 0, 0, 0 }; //#################################
volatile PID_STRUCT IndCurPID2 = { 0, 0, 0, 0, 0, 0, 0 }; //#################################
volatile PID_STRUCT IndCurPID3 = { 0, 0, 0, 0, 0, 0, 0 }; //#################################
volatile PID_STRUCT IndCurPID4 = { 0, 0, 0, 0, 0, 0, 0 }; //#################################
volatile PID_STRUCT IndCurPID5 = { 0, 0, 0, 0, 0, 0, 0 }; //#################################
volatile PID_STRUCT IndCurPID6 = { 0, 0, 0, 0, 0, 0, 0 }; //#################################

volatile PID_STRUCT FrePowerDroop = { 0, 0, 0, 0, 0, 0, 0 };
volatile PID_STRUCT VolRePowerDrp = { 0, 0, 0, 0, 0, 0, 0 };                  //
volatile struct DQTrans_STRUCT SPWMGen = { 0, 0, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile struct DQTrans_STRUCT CurReference = { 0, 0, 50, 0, 0, 0, 0, 0, 0, 0,
		0, 0 };  //��������ο�ֵ
volatile struct DQTrans_STRUCT IndCurDQ =
		{ 0, 0, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0 };     //��е�������ֵ
volatile struct InverterPara_STRUCT VoltageControl = { 0 }; //�й�-Ƶ�ʣ��޹�-��ѹ���ƼĴ���
volatile struct FuncCounter_STRUCT FuncCounter;
volatile PID_STRUCT OutVolLoopPID;
volatile PID_STRUCT OutVolLoopPID1;               //#########################
volatile PID_STRUCT OutVolLoopPID2;                //#########################

volatile struct DeadTime_STRUCT DeadTime_Res;                        //�����Ĵ���
volatile struct DQTrans_STRUCT DTCom;


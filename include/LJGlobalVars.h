
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
	Uint16 Sw1:1;                 //标志位1， 开关Sw1的键值（0为开，1为关）
	Uint16 Sw2:1;                 //标志位2， 开关Sw2的键值（0为开，1为关）
	Uint16 Sw3:1;                 //标志位3， 开关Sw3的键值（0为开，1为关）
	Uint16 Sw4:1;                 //标志位4， 开关Sw4的键值（0为开，1为关）
	Uint16 Sw5:1;                 //标志位5， 开关Sw5的键值（0为开，1为关）
	Uint16 rsvd6:1;               //标志位6， 暂时保留
	Uint16 rsvd7:1;               //标志位7， 暂时保留
	Uint16 rsvd8:1;               //标志位8， 暂时保留
	Uint16 rsvd9:1;               //标志位9， 暂时保留
	Uint16 rsvd10:1;              //标志位10， 暂时保留
	Uint16 rsvd11:1;              //标志位11， 暂时保留
	Uint16 rsvd12:1;              //标志位12， 暂时保留
	Uint16 rsvd13:1;              //标志位13， 暂时保留
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
	Uint16 ADCOffSet:1;            //标志位1，ADC偏移量采样完成标志，1为完成
	Uint16 DCBusVol:1;             //标志位2，直流母线电压正常标志位，1为正常
	Uint16 ACBusVol:1;             //标志位3，电网母线电压检测
	Uint16 PhaseLocked:1;          //标志位4，锁相成功标志位，1为成功
	Uint16 SoftStart:1;            //标志位5，软启动完成标志位
	Uint16 StartUp:1;              //标志位9，启动完成标志
	Uint16 Restart:1;               //标志位10，故障重启标志，来自Can
	Uint16 PhaseFault:1;               //逆变器和电网相位差过大标志
	Uint16 FanOn:1;               //标志位12，风扇开启标志位
	Uint16 FreFault:1;               //频率错误
	Uint16 Mode:1;               //标志位15，占空比偏置量K0到达0.5标志位，0：未到，1：到达
	Uint16 RelaySt:1;              //标志位16，接触器状态位（0为断开，1为闭合）

	Uint16 ACOverCur:1;            //电感电流过流标志
	Uint16 DCOverCur:1;            //直流侧过流标志
	Uint16 HardFault:1;            //硬件保护标志

	Uint16 OrderOn:1;              //Turn ON command from the key
};

union ProFlg_Var
{
	Uint32            all;
	struct ProFlg_BITS bit;
};


extern volatile union ProFlg_Var ProFlag;
/***********************************************************************/

//  采样变量定义

struct SampRes_STRUCT            // 用以存放采样结果的结构体，带n变量为未换算整型值，带f为换算后的浮点真值
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

extern volatile struct SampRes_STRUCT SampRes;      //存放采样结果
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

struct SigGeneVars_STRUCT      // 测试波形
{
	int16 n;                   // 数点计数器
	
	float nwt;                 // 模拟锁相相角

	int16 InSig;               // 单相信号
	int16 InSiga;              // 三相信号a相
	int16 InSigb;              // 三相信号b相
	int16 InSigc;              // 三相信号c相
};

extern volatile struct SigGeneVars_STRUCT SigGeneVars;
/***********************************************************************/

struct DQTrans_STRUCT      // 旋转变换结构体
{
	float wt;        // 角速度
	float Phase;     //相位
	float Fre;          //旋转频率

	float32 a;         // 三相静止坐标系变量
	float32 b;
	float32 c;

	float32 d;         // 三相旋转坐标系变量
	float32 q;
	float32 o;

	float32 dfilt;     // 经数字滤波后的量
	float32 qfilt;
	float32 ofilt;

 };
extern volatile struct DQTrans_STRUCT GridVoltagePLL;  //电网电压锁相结构体
extern volatile struct DQTrans_STRUCT GridVoltagePLL1;  //电网电压锁相结构体
extern volatile struct DQTrans_STRUCT BriVolDQ;        //桥臂电压DQ结构体
extern volatile struct DQTrans_STRUCT SoftStartDQ;     //软启动DQ结构体
extern volatile struct DQTrans_STRUCT SPWMGen;        //SPWM发生寄存器
extern volatile struct DQTrans_STRUCT CurReference;    //输出电流参考值
extern volatile struct DQTrans_STRUCT IndCurDQ;        //电感电流寄存器
extern volatile struct DQTrans_STRUCT DTCom;
/***********************************************************************/

struct FuncCounter_STRUCT            //函数时基计数器
{
  Uint16 CurLoopCount;               //电流闭环软启动计数器
  Uint16 FreDetCount;                //电网频率检测计数器
  Uint16 DCBusVolCount;              //直流侧母线电压
  Uint16 ACBusCount;                 //交流侧电压检测
  Uint16 STCount;                    //桥臂软启动
  Uint16 ACCurCount;                 //交流侧电流检测
  Uint16 DCCurCount;                 //直流侧电流检测
  Uint16 CurUnbalCount;              //电流不平衡检测
  Uint16 RelayCount;                 //继电器控制
  Uint16 EcanCount;                  //Can通讯计数器
  Uint16 OffSetCount;     
  Uint16 FanOpeCount;               //风扇启动计数器  
  Uint16 DrpModeCount1;              //切换Droop模式计数器  
  Uint16 DrpModeCount2;       
};
extern volatile struct FuncCounter_STRUCT FuncCounter;
/***********************************************************************/

typedef struct       // PID算法结构体
{
	float Ref;       // PID算法参考值

/*
    这里PID参数是基于如下PID基本形式设置的:
	u(t) = Kp*e(t) + Ki*int(e(t)) + Kd*diff(e(t))          (1)
	其中，e(t),int(e(t)),diff(e(t))分别为误差项、误差积分项、误差微分项。
	将PID表达式作拉氏变换：D(s) = Kp + Ki*1/s + Kd*s，将其离散化并转化为差分方程：
	u(k) = u(k-1) + a0*e(k) - a1*e(k-1) + a2*e(k-2)        (2)
	其中,离散周期为T（实际代码中 T = 100us)，则
	a0 = Kp + Ki*T + Kd/T; a1 = Kp + 2*Kd/T; a2 = Kd/T.
	PIDCalc子程序段中，拟采用（2）的形式进行计算.
*/

	float pos;       // 误差极性，不同环路获取误差极性不同，详细见Matlab仿真中的
	                 // 控制器框图，如5d、5q环路的误差极性是不同的，故此定义pos变量
					 // 用以表征该特性，该值根据误差的极性，分别为1或-1，由初始化时赋值。
	float err;       // 当前拍误差err(k)
	float err1;      // 前两拍误差，err(k-1)
	float err2;      // 前三拍误差，err(k-2)

	float PIDU;      // 当前拍PID控制器输出
	float PIDU1;     // 上一拍PID控制器输出

}PID_STRUCT;

extern volatile PID_STRUCT GridPLLPID;       //锁相环PID
extern volatile PID_STRUCT BriVolPID_D;      //桥臂电压D轴控制PID
extern volatile PID_STRUCT BriVolPID_Q;      //桥臂电压Q轴控制PID
extern volatile PID_STRUCT IndCurPID_D;       //电感电流D轴控制PID
extern volatile PID_STRUCT IndCurPID_Q;       //电感电流Q轴控制PID
extern volatile PID_STRUCT OutVolLoopPID;     //整流器电压外环PID
extern volatile PID_STRUCT FrePowerDroop;     //频率有功下垂PID
extern volatile PID_STRUCT VolRePowerDrp;     //电压无功下垂PID
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
     float ActivePowerCur;                  //有功电流
     float ActivePowerCurFilt;
	 float ReactivePowerCur;                //无功电流
     float ActivePowerFreVar;               //用功使频率偏移50Hz量
};
extern volatile struct InverterPara_STRUCT VoltageControl;

//用于DMA模块和ADC采样
extern volatile Uint16 *User_DMA_Des,*User_DMA_Source;

struct DeadTime_STRUCT            //死区补偿变量寄存器
{
  float IndCurAf;            //三相电流采样，经过滤波后
  float IndCurBf;
  float IndCurCf;

  float IndCurdf;           //电流采样dq变量
  float IndCurqf;

  int DTComA;              //三相死区补偿数值
  int DTComB;
  int DTComC;
};
extern volatile struct DeadTime_STRUCT DeadTime_Res;

// 函数声明
extern void UserSetGPIO(void);               //IO设定
extern void UserSetInt(void);                //中断自定义
extern void UserSetCana();                   //CanA模块自定义
extern void EnableDog(void);                 //使能看门狗
extern void KickDog();                       //
extern void UserSetEPWM();              //EPWM模块自定义
extern void User_DQ2abc(volatile struct DQTrans_STRUCT *Trans);
extern void User_abc2DQ(volatile struct DQTrans_STRUCT *Trans);
extern void UserSetADC();                   
extern void UserSetDMA();
extern void Phase_Lock();               //锁相程序，ISR中调用
extern void SVPWM_Generate();           //SVPWM发生程序，CurCloseLoop()中调用
extern void AcquireOffSet();           //获得直流补偿偏置
extern void ACBusVolDet();            // 电网电压检测
extern void UserSetVar();             // 初始化自定义参数
extern void GridFreDet();             //电网频率检测
extern void DCBusVolDet();            //直流电压检测
extern void SoftStartFunc();          //桥臂电压软启动函数
extern void InverterOn();             //开通占空比，硬件保护，风扇
extern void InverterOff();
extern void CurrentDet();             //交流和直流侧电流检测
extern void CurCloseLoop();           //电流闭环
extern void OutVolLoop();             //整流器电压外环
extern void GridCurClose();           //并网电流闭环
extern void SendUseECan(Uint32 SendData, Uint16 SendInfo);
extern void RelayOperation();
extern void CurUnbalDet();
extern void ReceiveUseECan();
extern float LowPassFilter(float PreOut,float Input, float CutFre);          //一阶低通滤波器
extern void STPIDCom(volatile PID_STRUCT *ComCon, float Reference, float Sense);    //桥臂电压软启动
extern void demofun(void);
extern void PowerFreLoop(void);                                         //有功频率控制函数
extern void  EcanDataSend(void);
extern void FanOperation(void);
extern void HardFaultDet(void);
extern void RePowerVolLoop(void);    //无功电压控制环路
extern void DeadTimeCom(void);      //死区补偿函数
#endif



//=============================================
//end of file
//==============================================

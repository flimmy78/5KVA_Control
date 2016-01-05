
//###########################################################################
//    User Global Variables definetion file 
//###########################################################################

   #include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
   #include "DSP2833x.h"        // DSP2833x headerfile

   volatile enum   DeviceState MainState;            //程序主机状态变量
   volatile enum   FaultState  FaultType;
   volatile union  ProFlg_Var ProFlag;                //存放程序状态相关变量
   volatile struct SampRes_STRUCT SampRes;           //存放采样结果
   volatile struct SampRes_STRUCT1 SampRes1;         //###########################

   volatile struct OffSet_STRUCT OffSetRes;           //采样偏置量
   volatile struct SigGeneVars_STRUCT SigGeneVars;   //模拟测试波形产生变量群

   volatile struct ECanData_STRUCT ECanData;          //ECan通讯变量

   volatile Uint16 *User_DMA_Des,*User_DMA_Source;    //DMA源和目标地址
      
   //  锁相环变量定义                          
   volatile PID_STRUCT GridPLLPID = {0,0,0,0,0,0,0};	//锁相误差信号，PI输出量				      
   volatile struct DQTrans_STRUCT GridVoltagePLL = {0,50,0,0,0,0,0,0,0,0,0};  //锁相输入电压，dq值，频率，相位
   volatile struct DQTrans_STRUCT GridVoltagePLL1 = {0,50,0,0,0,0,0,0,0,0,0};  //###############
   //软启动变量、桥臂电压控制、并网电流变量定义
   volatile struct DQTrans_STRUCT SoftStartDQ = {0,0,0,0,0,0,0,0,0,0,0};     //用于桥臂电压软启动
   volatile struct DQTrans_STRUCT BriVolDQ = {0,0,0,0,0,0,0,0,0,0,0};       // 桥臂输出电压及其DQ值
   volatile PID_STRUCT BriVolPID_D = {0,0,0,0,0,0,0};                       //桥臂电压D轴控制PID
   volatile PID_STRUCT BriVolPID_Q = {0,0,0,0,0,0,0};                       //桥臂电压Q轴控制PID
   volatile PID_STRUCT IndCurPID_D = {0,0,0,0,0,0,0};                       //电感电流D轴控制PID
   volatile PID_STRUCT IndCurPID_Q = {0,0,0,0,0,0,0};                       //电感电流Q轴控制PID

   volatile PID_STRUCT IndCurPID1 = {0,0,0,0,0,0,0};   //#################################
   volatile PID_STRUCT IndCurPID2 = {0,0,0,0,0,0,0};   //#################################
   volatile PID_STRUCT IndCurPID3 = {0,0,0,0,0,0,0};   //#################################
   volatile PID_STRUCT IndCurPID4 = {0,0,0,0,0,0,0};   //#################################
   volatile PID_STRUCT IndCurPID5 = {0,0,0,0,0,0,0};   //#################################
   volatile PID_STRUCT IndCurPID6 = {0,0,0,0,0,0,0};   //#################################

   volatile PID_STRUCT FrePowerDroop = {0,0,0,0,0,0,0};
   volatile PID_STRUCT VolRePowerDrp = {0,0,0,0,0,0,0};                     //
   volatile struct DQTrans_STRUCT SPWMGen = {0,0,50,0,0,0,0,0,0,0,0,0};
   volatile struct DQTrans_STRUCT CurReference = {0,0,50,0,0,0,0,0,0,0,0,0};  //并网电流参考值     
   volatile struct DQTrans_STRUCT IndCurDQ =  {0,0,50,0,0,0,0,0,0,0,0,0};     //电感电流采样值
   volatile struct InverterPara_STRUCT VoltageControl = {0};                //有功-频率，无功-电压控制寄存器
   volatile struct FuncCounter_STRUCT FuncCounter; 
   volatile PID_STRUCT OutVolLoopPID;
   volatile PID_STRUCT OutVolLoopPID1;               //#########################
   volatile PID_STRUCT OutVolLoopPID2;                //#########################

   volatile struct DeadTime_STRUCT DeadTime_Res;                            //死区补偿寄存器
   volatile struct DQTrans_STRUCT DTCom;


/*****************************************************************************
  * @file       : detection_control
  * @brief      : null
  * @note       : null
  * @version    : V1.0
  * @date       : 2017-7-20 15:39:54
******************************************************************************/


#ifndef _DETECTION_CONTROL_H_
#define _DETECTION_CONTROL_H_


//.H
/*============条件编译========================================================*/

/*============标准库头文件====================================================*/

/*============非标准库头文件==================================================*/
#include "m_include.h"
/*============全局 常量声明 extern ===========================================*/

/*============全局 宏声明=====================================================*/

#define PIN_LOCK_EN_ON              P30=1;
#define PIN_LOCK_EN_OFF             P30=0;
#define PIN_LOCK_POWER_READ         P05

#define PIN_BUZZER_EN_ON            P17=1;
#define PIN_BUZZER_EN_OFF           P17=0;

#define PIN_GPRS_POWER_L            P12=0;
#define PIN_GPRS_POWER_H            P12=1;
#define PIN_GPRS_RESET_H            P11=1;
#define PIN_GPRS_RESET_L            P11=0;


#define DISCONNET_CNT_MAX       300
#define HEART_CNT_MAX           5

#define KEY_WAKEUP_CNT_MAX      3000//0.1s 按键唤醒时间
#define TIME_WAKEUP_CNT_MAX     600//0.1s 定时唤醒时间

#define SynTimeCnt_MAX          100//600//0.1s 运行数据同步时间



/*============全局 数据类型声明===============================================*/
enum
{
    //step

    STEP_POWER_UP = 0, //上电0
    STEP_CHECK_UART,//检查串口通讯1
    STEP_FIX_BAUD,//固定波特率2
    STEP_BASE_INIT,//基础设置3
    STEP_CHECK_CARD,//检查SIM卡4
    STEP_CHECK_GSM,//注册GSM5
    STEP_CHECK_GPRS,//注册GPRS6
    STEP_ENABLE_PDP,//激活PDP场景7
    STEP_CREAT_CONNET,//建立连接8
    STEP_CONNETED,//已连接9
    STEP_POWER_DOWN,//掉电重启10

};

enum
{
    /*
    "IP INITIAL" 初始化
    "IP START" 启动任务
    "IP CONFIG" 配置场景
    "IP IND" 激活 GPRS/CSD 场景中
    "IP GPRSACT" 接收场景配置
    "IP STATUS" 获得本地 IP 地址（参考 AT+QILOCIP 命令）
    "TCP CONNECTING" TCP 连接中
    "UDP CONNECTING" UDP 连接中
    "IP CLOSE" TCP/UDP 连接关闭
    "CONNECT OK" TCP/UDP 连接成功
    "PDP DEACT" GPRS/CSD 场景异常关闭
    */
    TCP_STATE_NONE = 0,//0
    TCP_STATE_INITIAL,//1
    TCP_STATE_START,//2
    TCP_STATE_CONFIG,//3
    TCP_STATE_IND,//4
    TCP_STATE_GPRSACT,//5
    TCP_STATE_STATUS,//6
    TCP_STATE_CONNECTING,//7
    TCP_STATE_CLOSE,//8
    TCP_STATE_CONNECTED,//9
    TCP_STATE_DEACT,//10

};


typedef struct
{
    uint8_t Step;

    uint8_t Ack_OK;
    uint8_t Ack_CPIN_READY;
    uint8_t Ack_GSM_REGISTER;//注册
    uint8_t Ack_GPRS_REGISTER;//注册
    uint8_t Ack_GPRS_ATTACH;//附着
    uint8_t Ack_TCP_STATE;//TCP状态
    uint8_t Ack_TCP_SEND_OK;//发送完成
    uint8_t Ack_TCP_REC_DATA;//收到数据

    uint8_t TCP_CONNETED;//TCP连接状态 1-CONNETED 0-DISCONNETED

    uint16_t disconnet_cnt;//离线倒计时
    uint8_t heart_cnt;//心跳

    uint8_t send_data_flag;
    uint8_t send_data_buf[64];
    uint8_t send_data_len;

} Gprs_Struct;

typedef enum
{
    //继电器枚举
    SW1 = 0,
    SW2,
    SW_MAX

} SW_Enum;

typedef struct
{

    uint16_t Time_Limit_Half_Hour;//试用限制

    uint8_t MCU_96bit_UUID[12];//芯片ID

    uint8_t Lock_OnOff;//开关  0关1开
    uint8_t Lock_Set;//控制标志位

    uint8_t BatteryCapacity;    //电量百分比
    uint8_t WorkMode;           //工作状态
    uint8_t Error_Open;         //开锁故障
    uint8_t Error_Close;        //关锁故障
    uint8_t Error_BatteryLow;   //电量故障

    uint8_t Buzzer_OnOff;//开关  0关1开

    uint8_t Key_WakeUp;//1-按键唤醒
    uint16_t SynTimeCnt;//同步时间计数器
    
    uint16_t WakeUp_Cnt;//唤醒计数器

} Control_Struct;


/*============全局 变量声明 extern ===========================================*/
extern Gprs_Struct M26_Gprs;
extern idata Control_Struct Control;

/*============全局 函数原型声明===============================================*/
extern void gpio_init( void );
extern void uart0_timer3_init( uint32_t u32Baudrate );
extern void adc_init( void );
extern void exti_init( void );



extern uint16_t adc_get_channel_value( uint8_t ch );
extern uint8_t check_sum_add_8( uint8_t* buf, uint8_t len );
extern void RunDataUpDate( void );
extern char Lock_Control_Thread( thread_t* pt );
extern void read_mcu_96bit_uuid( uint8_t *id_buf );
extern void control_message_synchronize( uint8_t sw_in );
extern void Lock_Control( uint8_t Lock_OnOff );



extern char Time_10ms_Thread( thread_t* pt );
extern char Time_100ms_Thread( thread_t* pt );
extern char Gprs_Send_data_Thread( thread_t* pt );
extern char Gprs_Thread( thread_t* pt );
extern char Led_Thread( thread_t* pt );
extern char Time_Limit_Thread( thread_t* pt );

extern char Log_Thread( thread_t* pt );

#endif


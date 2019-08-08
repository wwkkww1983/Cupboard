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

#define PIN_LOCK_EN_ON              P15=1;
#define PIN_LOCK_EN_OFF             P15=0;
#define PIN_LOCK_POWER_READ         P17

#define PIN_BUZZER_EN_ON            P03=1;
#define PIN_BUZZER_EN_OFF           P03=0;

#define PIN_GPRS_POWER_L            P12=0;
#define PIN_GPRS_POWER_H            P12=1;
#define PIN_GPRS_RESET_H            P11=1;
#define PIN_GPRS_RESET_L            P11=0;
#define PIN_GPRS_EN_ON            	P01=1;
#define PIN_GPRS_EN_OFF            	P01=0;

#define PIN_LED_BLUE_ON            	P10=1;
#define PIN_LED_BLUE_OFF           	P10=0;

#define PIN_USB_POWER_READ          P05
#define PIN_TOUCH_READ              P30


#define KEY_WAKEUP_CNT_MAX          600//0.1s 按键唤醒时间
#define TIME_WAKEUP_CNT_MAX         600//0.1s 定时唤醒时间

/*============全局 数据类型声明===============================================*/

typedef struct
{

    uint16_t Time_Limit_Half_Hour;//试用限制

    uint8_t MCU_96bit_UUID[12];//芯片ID

    uint8_t Lock_OnOff;//开关  0关1开
    uint8_t Lock_Set;//控制标志位

    uint16_t BatteryVolt;//电池电压 MV

    uint8_t BatteryCapacity;    //电量百分比
    uint8_t Lock_State;           //锁状态 1关锁 0-开锁
    uint8_t Error_Open;         //开锁故障
    uint8_t Error_Close;        //关锁故障
    uint8_t Error_BatteryLow;   //电量故障

    uint8_t Buzzer_OnOff;//开关  0关1开

    uint8_t Key_WakeUp;//1-按键唤醒
    uint16_t WakeUp_Cnt;//唤醒计数器
        
    uint16_t SynTimeCnt;//同步时间计数器

	uint8_t Protocol_A_Info;
	uint8_t Protocol_B_Open;
	uint8_t Protocol_C_OpenOk;
	uint8_t Protocol_D_OpenError;
	uint8_t Protocol_E_Lock;

    uint8_t IMEI_Buffer[16];
    uint8_t IMEI_Get;
    
    uint8_t TCP_Conneted;
    
} Control_Struct;


/*============全局 变量声明 extern ===========================================*/
extern idata Control_Struct Control;
extern bit BIT_TMP;
/*============全局 函数原型声明===============================================*/

extern uint16_t adc_get_channel_value( uint8_t ch );
extern uint8_t check_sum_add_8( uint8_t* buf, uint8_t len );
extern void read_mcu_96bit_uuid( uint8_t *id_buf );
extern void Lock_Control( uint8_t Lock_OnOff );

extern void HAL_Init(void);
extern ThreadStatus TaskInit(void);
#endif


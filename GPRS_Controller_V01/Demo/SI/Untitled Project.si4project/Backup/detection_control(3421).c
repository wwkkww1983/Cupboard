/*****************************************************************************
  * @file       : detection_control
  * @brief      : null
  * @note       : null
  * @version    : V1.0
  * @date       : 2018-1-17
******************************************************************************/

//.C
/*============包含标准库头文件================================================*/

/*============包含非标准库头文件==============================================*/
#include "m_include.h"
#include "detection_control.h"
#include "stdio.h"
/*============文件内部 常量定义 const ========================================*/
const uint8_t battery_percent_table[] =
{
    100 ,
    90  ,
    80  ,
    70  ,
    60  ,
    50  ,
    40  ,
    30  ,
    20  ,
    15  ,
    10  ,
    5   ,
    0   ,
};


const uint16_t battery_volt_table[] =
{
    4100,
    4080,
    4000,
    3930,
    3870,
    3820,
    3790,
    3770,
    3730,
    3700,
    3680,
    3500,
    3400,
};

/*============文件内部 宏定义 define =========================================*/



/*============文件内部 数据类型定义 typedef ==================================*/


/*============文件内部 局部变量定义 static ===================================*/

/*============文件内部 全局变量定 ============================================*/
Gprs_Struct M26_Gprs;
idata Control_Struct Control;


/*============文件内部 函数原型定义 static ===================================*/


static void gpio_init( void )
{

    Set_All_GPIO_Quasi_Mode;

    //输出
    P30_PushPull_Mode;//锁开关
    P17_PushPull_Mode;//蜂鸣器

    P11_PushPull_Mode;//通信模块复位
    P12_PushPull_Mode;//通信模块电源

    //输入
    P05_Input_Mode;//锁的状态
    P01_Input_Mode;//触摸按键
    P00_Input_Mode;//充电芯片 CHRG
    P10_Input_Mode;//充电芯片 SDBY

}

void exti_init( void )
{
    //EXTI
    P01_Input_Mode;
    Enable_INT_Port0;
    Enable_BIT1_FallEdge_Trig;

}



void uart0_timer3_init( uint32_t u32Baudrate )
{

    P0M1 &= ~SET_BIT6;
    P0M2 &= ~SET_BIT6;
    P0M1 &= ~SET_BIT7;
    P0M2 &= ~SET_BIT7;

    SCON = 0x50;//UART0 Mode1,REN=1,TI=1
    PCON |= SET_BIT7; //UART0 Double Rate Enable
    T3CON &= 0xF8; //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
    T3CON |= SET_BIT5; //UART0 baud rate clock source = Timer3

    RH3 = ( 65536 - ( 1000000 / u32Baudrate ) - 1 ) >> 8; /*16 MHz */
    RL3 = ( 65536 - ( 1000000 / u32Baudrate ) - 1 ); /*16 MHz */

    T3CON |= SET_BIT3; //Trigger Timer3

    P06 = 1;
    P07 = 1;

    ES = 1;

    //RI = 1;
    TI = 1;


}


void adc_init( void )
{
    Enable_ADC_AIN5;//P04
}














uint16_t adc_get_channel_value( uint8_t ch )
{
    uint8_t t = 0;
    uint16_t buf = 0;

    ADCS = 0;
    ADCF = 0;
    ADCCON0 |= ch;
    ADCS = 1;

    while( ( ADCF == 0 ) && ( t < 250 ) )
    {
        t++;
    }
    ADCF = 0;
    buf = ( uint16_t ) ( ADCRH * 16 );
    buf = buf | ( ADCRL & 0x0f );

    return buf;

}



uint8_t check_sum_add_8( uint8_t* buf, uint8_t len )
{
    uint8_t i = 0;
    uint8_t Sum = 0;

    for( i = 0; i < len; i++ )
    {
        Sum += *buf++;
    }

    return Sum;
}


//进入低功耗模式
void enter_stop_mode( void )
{


//小循环
ORG_Idel_IN:

    set_EPI;//使能引脚中断

    set_PD;//进入掉电模式

    if( ( Control.Key_WakeUp != 1 ) ) //非按键唤醒且未到定时同步，则继续小循环
    {
        goto ORG_Idel_IN;//小循环
    }


    if( Control.Key_WakeUp == 1 )
    {
        //按键唤醒
        Control.Key_WakeUp = 0;
        Control.WakeUp_Cnt = KEY_WAKEUP_CNT_MAX;
        RunDataUpDate();
    }
    else
    {
        //定时唤醒
        Control.WakeUp_Cnt = TIME_WAKEUP_CNT_MAX;
    }


}


void Gprs_Disconnet_Cnt( void )
{

    if( M26_Gprs.disconnet_cnt > 0 )
    {

        M26_Gprs.disconnet_cnt--;
        if( M26_Gprs.disconnet_cnt == 0 )
        {
            M26_Gprs.TCP_CONNETED = 0;
        }

    }

}

void Lock_Control( uint8_t Lock_OnOff )
{
    Control.Lock_OnOff = Lock_OnOff;
    Control.Lock_Set = 1;
}

//100ms
void Lock_State_Check( void )
{
    static uint8_t cnt;
    static uint8_t cnt2;

    if( Control.Lock_Set == 0 )
    {

        if( PIN_LOCK_POWER_READ == 0 ) //关锁
        {

            if( cnt < 0xff )
            {
                cnt++;
            }

            if( cnt == 15 )
            {
                cnt2 = 0;
                Control.Lock_State = 1;//1关锁 0-开锁
                //Lock_Control( 0 );
                LockAck();
            }

        }
        else //未锁上
        {

            if( cnt2 < 0xff )
            {
                cnt2++;
            }

            if( cnt2 == 15 )
            {
                cnt = 0;
                Control.Lock_State = 0;//1关锁 0-开锁
            }


        }

    }
}


void Buzzer_Control( void )
{

    if( Control.Buzzer_OnOff == 1 )
    {
        PIN_BUZZER_EN_ON
    }
    else
    {
        PIN_BUZZER_EN_OFF
    }

}


char Lock_Control_Thread( thread_t* pt )
{
    Thread_BEGIN

    while( true )
    {
        static uint8_t cnt;
        static uint8_t check_sum_buf;

        //等待信号量
        Thread_Wait_Until( Control.Lock_Set );

        Thread_Sleep( 10 );

        if( Control.Lock_OnOff == 1 )
        {

            PIN_LOCK_EN_ON
            Thread_Sleep( 900 ); //转动时间
            PIN_LOCK_EN_OFF

            Thread_Sleep( 10 );
            if( PIN_LOCK_POWER_READ == 1 ) //成功 开锁
            {
                Control.Error_Open = 0;
            }
            else
            {
                Control.Error_Open = 1;
            }

        }
        Control.Lock_Set = 0;

        CtrlDataAck( Control.Error_Open);


    }

    Thread_END

}


void read_mcu_96bit_uuid( uint8_t *id_buf )
{

    uint8_t i;
    uint8_t EA_BUF;

    EA_BUF = EA;
    EA = 0;

    //IAPEN
    TA = 0xAA;
    TA = 0x55;
    CHPCON |= SET_BIT0;

    //96bit =12byte
    for( i = 0; i < 12; i++ )
    {

        IAPAL = i;//
        IAPAH = 0x00;

        IAPCN = 0x04;//

        TA = 0xAA;
        TA = 0x55;
        IAPTRG |= SET_BIT0;

        id_buf[i] = IAPFD;

    }

    //IAPEN
    TA = 0xAA;
    TA = 0x55;
    CHPCON &= ~SET_BIT0;

    EA = EA_BUF;

}

void RunDataUpDate( void )
{

    /*
    发送明文    info@@$$@@入网号@@$$@@当前电量（int）@@$$@@当前门的状态
            如： info@@$$@@8899-88922-778@@$$@@98@@$$@@1
            表示向服务器报告：我的入网号是8899-88922-778
    我当前电量是 98%  门的状态1表示关，0表示开
    */

    uint8_t index = 0;
    uint8_t check_sum_buf = 0;
    uint8_t cnt = 0;

    if(M26_Gprs.send_data_flag == 0)
    {
        //服务器数据应答-
        memset( M26_Gprs.send_data_buf, 0, sizeof( M26_Gprs.send_data_buf ) );
        //359759002514931

        UART0_Tx_String("info@@$$@@");
        UART0_Tx_String(Control.IMEI_Buffer);//UART0_Tx_Data

        UART0_Tx_String("@@$$@@");
        UART0_Tx_Num(Control.BatteryCapacity);

        UART0_Tx_String("@@$$@@");
        UART0_Tx_Num(Control.Lock_State);

        UART0_Tx_String("\r\n");


        M26_Gprs.send_data_flag = 1;
        //服务器数据应答-

    }


}

//open_state 1开锁失败 0-开锁成功
void CtrlDataAck( open_state )
{

    /*
    C．你向服务器发送 openok@@$$@@你的入网号  表示你开锁成功。
    D．你向服务器发送 openerror@@$$@@你的入网号 表示开锁失败
    */



    //服务器数据应答-
    memset( M26_Gprs.send_data_buf, 0, sizeof( M26_Gprs.send_data_buf ) );
    //359759002514931

    if(Control.Error_Open==0)
    {
        UART0_Tx_String("openok@@$$@@");
        UART0_Tx_String(Control.IMEI_Buffer);//UART0_Tx_Data

        UART0_Tx_String("\r\n");
    }
    else
    {
        UART0_Tx_String("openerror@@$$@@");
        UART0_Tx_String(Control.IMEI_Buffer);//UART0_Tx_Data

        UART0_Tx_String("\r\n");
    }


    M26_Gprs.send_data_flag = 1;
    //服务器数据应答-



}


void LockAck( open_state )
{

    /*
    E．你向服务器发送 lock@@$$@@你的入网号  表示报告服务器柜门已关
    */


    //服务器数据应答-
    memset( M26_Gprs.send_data_buf, 0, sizeof( M26_Gprs.send_data_buf ) );


    UART0_Tx_String("lock@@$$@@");
    UART0_Tx_String(Control.IMEI_Buffer);//UART0_Tx_Data

    UART0_Tx_String("\r\n");

    M26_Gprs.send_data_flag = 1;
    //服务器数据应答-



}




//100ms
void Battery_Volt_Detec( void )
{
    uint8_t i;

    static uint8_t cnt;
    static uint8_t cnt1;

    Control.BatteryVolt = adc_get_channel_value( 5 ) * 1.08;

    for( i = 0; i < 13; i++ )
    {

        if( Control.BatteryVolt >= battery_volt_table[i] )
        {
            break;
        }

    }

    Control.BatteryCapacity = battery_percent_table[i];

    if(Control.BatteryCapacity<=5)
    {
        cnt++;
        cnt1=0;

        if(cnt>=20)
        {
            cnt=0;
            Control.Error_BatteryLow=2;
        }

    }
    else if(Control.BatteryCapacity<=15)
    {

        cnt=0;
        cnt1++;

        if(cnt1>=20)
        {
            cnt1=0;
            Control.Error_BatteryLow=1;
        }

    }
    else
    {
        cnt=0;
        cnt1=0;
        Control.Error_BatteryLow=0;
    }

}



/*

char Demo_Thread(thread_t* pt)
{
    Thread_BEGIN

    while(true)
    {


        Thread_Sleep(10);

    }

    Thread_END

}


*/


char Time_10ms_Thread( thread_t* pt )
{
    Thread_BEGIN

    while( true )
    {

        Key_Scan();
        UART0_Rx();
        Thread_Sleep( 10 );

    }

    Thread_END

}


char Time_100ms_Thread( thread_t* pt )
{
    Thread_BEGIN

    //硬件上电操作
    Control.WakeUp_Cnt = KEY_WAKEUP_CNT_MAX;

    memcpy(Control.IMEI_Buffer,"359759002514931",15);

    while( true )
    {
        key_value_for_mode();
        Buzzer_Control();//蜂鸣器控制
        Battery_Volt_Detec();//电量检测
        Lock_State_Check();//锁状态检测

        //        Gprs_Disconnet_Cnt();

        Control.SynTimeCnt++;
        if( Control.SynTimeCnt >= SynTimeCnt_MAX )
        {
            Control.SynTimeCnt = 0;
            RunDataUpDate();
        }

        if( ( Control.Lock_OnOff == 0 ) && ( Control.Lock_State == 0 ) )
        {
            if( Control.WakeUp_Cnt )
                Control.WakeUp_Cnt--;//低功耗控制
        }


        if( ( P01 == 0 ) || ( Control.Lock_OnOff == 1 ) ) //触发按键 使用中
        {
            Control.WakeUp_Cnt = KEY_WAKEUP_CNT_MAX;
        }

        if( Control.WakeUp_Cnt == 0 ) //进入待机
        {

            //进入前的操作 关闭通信模块 蜂鸣器
            M26_Gprs.TCP_CONNETED = 0;
            PIN_GPRS_POWER_L
            Thread_Sleep( 1700 );//off
            PIN_GPRS_POWER_H
            PIN_BUZZER_EN_OFF

            Thread_Sleep( 100 );
            enter_stop_mode();

            //退出后的操作 开启通信模块
            Thread_Sleep( 100 );

            PIN_GPRS_POWER_L
            Thread_Sleep( 2200 );//on
            PIN_GPRS_POWER_H

        }

        Thread_Sleep( 100 );

    }

    Thread_END

}

char Gprs_Send_data_Thread( thread_t* pt )
{
    Thread_BEGIN

    PIN_GPRS_RESET_H

    PIN_GPRS_POWER_L
    Thread_Sleep( 2200 );
    PIN_GPRS_POWER_H

    while( true )
    {
        static uint8_t Try_Cnt;

        //等待信号量
        Thread_Wait_Until( M26_Gprs.send_data_flag & M26_Gprs.TCP_CONNETED );

        Thread_Sleep( 100 );

        M26_Gprs.Ack_TCP_SEND_OK = 0;
        for( Try_Cnt = 0; Try_Cnt < 1; Try_Cnt++ )
        {
            Thread_Sleep( 50 ); //间隔

            UART0_Tx_Data( M26_Gprs.send_data_buf, M26_Gprs.send_data_len );
            Thread_Sleep( 900 ); //间隔

            if( M26_Gprs.Ack_TCP_SEND_OK )
            {
                break;
            }

        }

        M26_Gprs.send_data_flag = 0;

    }

    Thread_END

}

char Time_Limit_Thread( thread_t* pt )
{
    Thread_BEGIN

    while( true )
    {

#if (TIME_LIMIT==1)


        if( Control.Time_Limit_Half_Hour < 0xfff0 )
        {
            Control.Time_Limit_Half_Hour++;
        }

        if( Control.Time_Limit_Half_Hour > TIME_LIMIT_HALF_HOUR_MAX )
        {

            while( 1 )
            {
                //关闭所有负载
                //喂狗

                //WdogRst();
                Thread_Sleep( 1000 );
            }

        }

#endif // TIME_LIMIT


        Thread_Sleep( MINUTES( 30 ) );

    }

    Thread_END

}


char Log_Thread( thread_t* pt )
{
    Thread_BEGIN

    Thread_Sleep( 500 );
    UART0_Tx_String( "\r\n Log_Thread " );
    while( true )
    {
#if 0
        UART0_Tx_String( "\r\n\r\n Control.BatteryVolt " );
        UART0_Tx_Num( Control.BatteryVolt );

        UART0_Tx_String( "\r\n Control.BatteryCapacity " );
        UART0_Tx_Num( Control.BatteryCapacity );

        UART0_Tx_String( "\r\n Control.Lock_State " );
        UART0_Tx_Num( Control.Lock_State );

        UART0_Tx_String( "\r\n Control.Error_Open " );
        UART0_Tx_Num( Control.Error_Open );

        UART0_Tx_String( "\r\n Control.Error_Close " );
        UART0_Tx_Num( Control.Error_Close );

        UART0_Tx_String( "\r\n Control.Error_BatteryLow " );
        UART0_Tx_Num( Control.Error_BatteryLow );

#endif

        Thread_Sleep( 1000 );

    }

    Thread_END

}






//硬件初始化
void HAL_Init(void)
{
    gpio_init();
    exti_init();
    adc_init();
    uart0_timer3_init( 19200 );//19200
}


void TaskInit(void)
{

    Thread_Start( Time_10ms_Thread, &threadInfo.status );
    Thread_Start( Time_100ms_Thread, &threadInfo.status );
    //Thread_Start(Gprs_Thread, &threadInfo.status);
    Thread_Start( Gprs_Send_data_Thread, &threadInfo.status );
    // Thread_Start(Time_Limit_Thread, &threadInfo.status);
    Thread_Start( Lock_Control_Thread, &threadInfo.status );

    Thread_Start( Log_Thread, &threadInfo.status );

}






/*******************************************************************************
  * @file       : main
  * @brief      : 2G柜锁
  * @note       :
                    flash:  18KB

                    data:   127KB
                    idata:  127KB
                    xdata:  766KB

  * @version    : V1.0
  * @date       : 2018-1-1
*******************************************************************************/

#include "m_include.h"

/*


*/


/*******************************************************************************
  * @name       : main
  * @brief      : null
  * @note       : null
  * @param      : null
  * @retval     : null
  * @date       : 2018-1-16
*******************************************************************************/
void main( void )
{
    //initial
    gpio_init();
    exti_init();
    adc_init();
    uart0_timer3_init( 19200 );//19200

    read_mcu_96bit_uuid( Control.MCU_96bit_UUID );

    SysTickInit();//最后

    //add
    Thread_Start( Time_10ms_Thread, &threadInfo.status );
    Thread_Start( Time_100ms_Thread, &threadInfo.status );
    //Thread_Start(Gprs_Thread, &threadInfo.status);
    Thread_Start( Gprs_Send_data_Thread, &threadInfo.status );
    //Thread_Start(Led_Thread, &threadInfo.status);
    // Thread_Start(Time_Limit_Thread, &threadInfo.status);
    Thread_Start( Lock_Control_Thread, &threadInfo.status );

    Thread_Start( Log_Thread, &threadInfo.status );


    //run
    TaskRun();

    while( 1 )
    {
        ;
    }

}



void Timer2_ISR( void ) interrupt 5
{
    TF2 = 0;//Clear Timer2 Interrupt Flag
    gU32Systick++;
}


void Uart0_ISR( void ) interrupt 4
{

    UART0_Rx_Isr();

    if( TI )
    {
        TI = 0;
        UART_0.TI_FLAG = 1;
    }

}


void PinInterrupt_ISR ( void ) interrupt 7
{
    PIF = 0x00;
    Control.Key_WakeUp = 1;
    clr_EPI;
}



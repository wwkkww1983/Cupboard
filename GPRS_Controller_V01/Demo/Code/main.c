/*******************************************************************************
  * @file       : main
  * @brief      : 
  * @note       :
                    flash:  18KB
                    data:   127KB
                    idata:  127KB
                    xdata:  766KB

  * @version    : V1.0
  * @date       : 2018-1-1
*******************************************************************************/

#include "m_include.h"


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

    HAL_Init();
    read_mcu_96bit_uuid( Control.MCU_96bit_UUID );
    SysTickInit();

    TaskInit();
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
    clr_EPI;
    
}

void INT0_ISR ( void ) interrupt 0
{
    Control.Key_WakeUp = 1;
    clr_EX0;
}

void INT1_ISR ( void ) interrupt 2
{
    Control.Key_WakeUp = 1;
    clr_EX1;
}


#include "systick.h"
#include "m_include.h"
/*---------------------------------------------------------------------------*/
/** \brief
 *
 * \param
 * \param
 * \return
 *
 */
volatile u32 gU32Systick;
/*---------------------------------------------------------------------------*/
/** \brief  systick init
 *
 * \param
 * \param
 * \return
 *
 */

void SysTickInit(void)
{
    /* SYSTICK configuration */
    
    WdogInit();
    //
    //1ms
    DI();
    //add you code here
    
    //TIMER2_DIV_4 TIMER2_Auto_Reload_Delay_Mode
    T2MOD=0x98;
    T2CON=0x00;

    RCMP2L = 65536-4000;
    RCMP2H = (65536-4000)>>8;
    TL2 = 0;
    TH2 = 0;

    EIE|= SET_BIT7;
    TR2=1;

    EI();
}

/*---------------------------------------------------------------------------*/
/** \brief  watchdog init
 *
 * \param
 * \param
 * \return
 *
 */
void WdogInit(void)
{
   //add you code here
}
/*---------------------------------------------------------------------------*/
/** \brief  watchdog reset
 *
 * \param
 * \param
 * \return
 *
 */
void WdogRst(void)
{
    //add you code here
}

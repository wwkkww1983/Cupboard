/*****************************************************************************
  * @file       : key
  * @brief      : null
  * @note       : null
  * @version    : V1.0
  * @date       : 2017-7-18 14:16:53
******************************************************************************/

//.C
/*============包含标准库头文件================================================*/

/*============包含非标准库头文件==============================================*/
#include "m_include.h"
#include "key.h"
/*============文件内部 常量定义 const=========================================*/

/*============文件内部 宏定义 define==========================================*/

/*============文件内部 数据类型定义 typedef===================================*/


/*============文件内部 局部变量定义 static====================================*/

/*============文件内部 全局变量定义 extern ===================================*/
Key_Struct      Key;
/*============文件内部 函数原型定义 static====================================*/

static uint8_t Key_Input( uint8_t num )
{

    if( num == KEY_SET )
    {
        if( Key.Ad_Key_Value == 0 )
        {
            return Key_Pressed;
        }
        else
        {
            return Key_Realsed;
        }
    }

    return Key_Realsed;
}

static void Key_Value_Deal( void )
{

    uint8_t i;

    for( i = 0; i < KEY_MAX_NUM; i++ )
    {

        if( ( Key.Pressed_Time[i] >= SHORT_TIME ) && ( Key.State[i] == Key_Realsed ) )
        {

            if( Key.Pressed_Time[i] < LONG1S_TIME ) //短按
            {
                Key.Value = i * 2 + 1;
            }
            else
            {
                Key.Value = i * 2 + 2;
            }

        }

    }

}

/******************************************************************************
 * Function Name: Key_Scan
 * Description  : This function is used to read all the keys state  10ms
 * Arguments    : None
 * parameter    : None
 * Return Value : None
 ******************************************************************************/
void Key_Scan( void )
{
    uint8_t i;
    uint8_t Key_Input_State;

    //如果是触摸，则在此读取一次键值
    Key.Ad_Key_Value = P01;

    for( i = 0; i < KEY_MAX_NUM; i++ )
    {
        Key_Input_State = Key_Input( i );

        if( Key_Input_State == Key_Pressed )
        {
            Key.Realse_cnt[i] = 0;

            if( ++Key.Pressed_cnt[i] > Pressed_jitter )  //按下消抖
            {
                if( Key.Pressed_cnt[i] >= 0xfe )  //防止溢出
                {
                    Key.Pressed_cnt[i] = 0xfe;
                }

                Key.State[i] = Key_Pressed;

                Key.Pressed_Time[i]++;

                if( Key.Pressed_Time[i] >= 0xfe )  //防止溢出
                {
                    Key.Pressed_Time[i] = 0xfe;
                }


            }
        }
        else
        {
            Key.Pressed_cnt[i] = 0;

            if( ++Key.Realse_cnt[i] > Release_jitter )
            {

                Key.State[i] = Key_Realsed;

                if( Key.Realse_cnt[i] >= 0xfe )  //防止溢出
                {
                    Key.Realse_cnt[i] = 0xfe;
                }

                Key_Value_Deal();//识别按键类型

                Key.Pressed_Time[i] = 0;

            }
        }
    }

}



/*****************************************************************************
  * @name       : key_value_for_mode
  * @brief      : 该模式下，键值的处理
  * @note       : null
  * @param      : null
  * @retval     : null
  * @date       : 2017-7-20 15:39:46
******************************************************************************/
void key_value_for_mode( void )
{

    switch( Key.Value )
    {

        case KEY_SET_SHORT:
        {

            break;
        }

        case KEY_SET_LONG1S:
        {

            break;
        }


        default:
            break;

    }

    Key.Value = 0x00; //按键清零

}





/*****************************************************************************
  * @file       : key
  * @brief      : null
  * @note       : null
  * @version    : V1.0
  * @date       : 2017-7-18 14:16:12
******************************************************************************/

#ifndef _KEY_H_
#define _KEY_H_

//.H
/*============条件编译========================================================*/

/*============标准库头文件====================================================*/

/*============非标准库头文件==================================================*/

/*============全局 常量声明 extern============================================*/

/*============全局 宏声明=====================================================*/

typedef enum
{
    KEY_SET = 0,
    KEY_MAX_NUM
} KEY_ENUM;


//按键状态
#define Key_Pressed                 0//按下
#define Key_Realsed                 1//松开

#define Pressed_jitter              3//消抖时间，单位10ms
#define Release_jitter              3

#define SHORT_TIME                  2
#define LONG1S_TIME                 150

/*============全局 数据类型声明===============================================*/

typedef enum
{

    KEY_SET_SHORT = 1,//1
    KEY_SET_LONG1S,//2


} KEY_VALUE_e;

typedef struct
{

    /*
    Value 十进制
    按键号（十位）  1-25
    数值（个位）    1:短按  2:长按3秒  3:长按10秒

    如：11，表示键1短按  12，表示键1长按3秒  13，表示键1长按10秒
        21，表示键2短按  22，表示键2长按3秒  23，表示键2长按10秒
    */

    uint8_t Value;
    // uint8_t     Enable[KEY_MAX_NUM];//使能位
    uint8_t State[KEY_MAX_NUM];         //状态：Key_Pressed/Key_Realsed
    uint8_t Pressed_cnt[KEY_MAX_NUM];  //累计变量：用于消抖
    uint8_t Realse_cnt[KEY_MAX_NUM];   //累计变量：用于松手
    uint8_t Pressed_Time[KEY_MAX_NUM]; //实际按下的时间
    //uint16_t Realse_Time[KEY_MAX_NUM];  //实际松手的时间

    uint16_t Ad_Key_Value;//AD型按键的值
    //uint16_t Touch_Key_Value;//触摸型按键的值

} Key_Struct;
/*============全局 变量声明 extern============================================*/
extern Key_Struct Key;
/*============全局 函数原型声明===============================================*/

void Key_Scan( void );
void key_value_for_mode( void );


#endif

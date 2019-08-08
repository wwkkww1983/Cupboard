/*****************************************************************************
  * @file       : m_include
  * @brief      : null
  * @note       : null
  * @version    : V1.0
  * @date       : 2017-5-9
******************************************************************************/


#ifndef _M_INCLUDE_H_
#define _M_INCLUDE_H_


//.H
/*============条件编译========================================================*/

/*============标准库头文件====================================================*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "N76E003.h"

/*============非标准库头文件==================================================*/
#include "main.h"
#include "typedef.h"
#include "thread.h"
#include "Function_Define.h"
#include "SFR_Macro.h"

#include "detection_control.h"
//#include "key.h"
#include "uart0.h"


/*============全局 常量声明 extern============================================*/

/*============全局 宏声明=====================================================*/

#define SET_BIT0        0x01
#define SET_BIT1        0x02
#define SET_BIT2        0x04
#define SET_BIT3        0x08
#define SET_BIT4        0x10
#define SET_BIT5        0x20
#define SET_BIT6        0x40
#define SET_BIT7        0x80

#define TIME_LIMIT      0//量产时，注释此语句
#define TIME_LIMIT_HALF_HOUR_MAX  1500 //30天


/*============全局 数据类型声明===============================================*/
typedef union
{
    uint8_t byte;
    struct
    {
        uint8_t b0: 1;
        uint8_t b1: 1;
        uint8_t b2: 1;
        uint8_t b3: 1;
        uint8_t b4: 1;
        uint8_t b5: 1;
        uint8_t b6: 1;
        uint8_t b7: 1;
    } bits;
} Union_Byte;


/*============全局 变量声明 extern============================================*/

/*============全局 函数原型声明===============================================*/


#endif


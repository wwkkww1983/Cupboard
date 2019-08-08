/*****************************************************************************
  * @file       : uart0
  * @brief      : null
  * @note       : null
  * @version    : V1.0
  * @date       : 2017-5-25
******************************************************************************/


#ifndef _UART0_H_
#define _UART0_H_


//.H
/*============条件编译========================================================*/
/*============标准库头文件====================================================*/
/*============非标准库头文件==================================================*/
/*============全局 常量声明 extern============================================*/
/*============全局 宏声明=====================================================*/
#define RX_TIME_OUT         3
#define RX_LENGTH           128
#define TX_LENGTH           128

#define     TYPE_OFFSET         1
#define     LEN_OFFSET          2
#define     DATA1_OFFSET        3
#define     DATA2_OFFSET        4



/*============全局 数据类型声明===============================================*/
typedef struct
{
    uint8_t F_RxSynch;//标志位：是否收到同步头

    //uint8_t RxDataLen;//数据包中表示实际有效的数据
    uint8_t RxCnt;//计数：串口接收到的所有数据
    uint8_t RxBuf[RX_LENGTH];//接收数据数组
    uint8_t RxTimeOutCnt;//接收超时累计
    uint8_t RxEnd;//一帧数据接收结束

    uint8_t TI_FLAG;


} UART_Struct;

/*============全局 变量声明 extern============================================*/
extern UART_Struct UART_0;

/*============全局 函数原型声明===============================================*/
void UART0_Tx_String( uint8_t* buf );
void UART0_Tx_Data( uint8_t* buf, uint8_t len );
void UART0_Rx( void );
void UART0_Rx_Isr( void );
void UART0_Tx_Byte( uint8_t buf );
void UART0_Tx_Num( uint16_t num );


#endif


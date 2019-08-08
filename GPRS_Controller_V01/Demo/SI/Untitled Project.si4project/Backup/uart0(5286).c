/*****************************************************************************
  * @file       : uart0
  * @brief      : null
  * @note       : null
  * @version    : V1.0
  * @date       : 2017-5-25
******************************************************************************/


/*

A.	客户端连接后 首先要做一件事：将自已信息上报服务器

发送明文    info@@$$@@入网号@@$$@@当前电量（int）@@$$@@当前门的状态
        如： info@@$$@@8899-88922-778@@$$@@98@@$$@@1
        表示向服务器报告：我的入网号是8899-88922-778  我当前电量是 98%  门的状态1表示关，0表示开

只有上报了信息服务器才能审核你的身份并查看是否有你的锁要开。

B. 服务器可以测试向你发送  open@@$$@@你的入网号  表示要求你开锁。

C．你向服务器发送 openok@@$$@@你的入网号  表示你开锁成功。

D．你向服务器发送 openerror@@$$@@你的入网号 表示开锁失败。  

E．你向服务器发送 lock@@$$@@你的入网号  表示报告服务器柜门已关

F. 服务器收到你的任何消息会回复你“[你发上来的消息] ok” 这条消息你可以不做任何处理。

*/


















//.C
/*============包含标准库头文件================================================*/
/*============包含非标准库头文件==============================================*/
#include "m_include.h"
#include "uart0.h"
/*============文件内部 常量定义 const ========================================*/


/*============文件内部 宏定义 define ==========================================*/



/*============文件内部 数据类型定义 typedef ==================================*/

/*============文件内部 局部变量定义 static ===================================*/
UART_Struct UART_0;

/*============文件内部 全局变量定义 ==========================================*/

/*============文件内部 函数原型定义 static ===================================*/

/*******************************************************************************
  * @name       : UART0_Buf_Find
  * @brief      : 查找源数组中是否有目标数组，返回匹配的位置
  * @note       : null
  * @param      : @src-源比较数组 @src_len-源比较数组长度 @dest-目标比较数组 @dest_len-目标比较数组长度
  * @retval     : 匹配位置， @小于0 无匹配  @大于等于0 有匹配
  * @author     : 2018-1-20
*******************************************************************************/
int UART0_Buf_Find( uint8_t* src, int src_len, uint8_t* dest, int dest_len )
{
    int i, j, c;

    if( src_len < dest_len )
    {
        return -1;
    }

    for( i = 0; i < src_len; i++ )
    {
        if( src[i] == dest[0] )
        {
            c = 0;

            for( j = 0; j < dest_len; j++ )
            {
                if( src[i + j] == dest[j] )
                {
                    c++;

                    if( c == dest_len )
                    {
                        return i;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    return -2;
}

//发送一个字节
void UART0_Tx_Byte( uint8_t buf )
{

    uint16_t t = 0;

    UART_0.TI_FLAG = 0;
    SBUF = buf;

    while( ( UART_0.TI_FLAG == 0 ) && ( t < 0XFFF0 ) )
    {
        t++;
    }

    UART_0.TI_FLAG = 0;

}

//发送字符串
void UART0_Tx_String( uint8_t* buf )
{
    uint8_t i = 0;
    uint16_t t = 0;


    while( *( buf + i ) )
    {
        UART_0.TI_FLAG = 0;
        SBUF = *( buf + i );

        while( ( UART_0.TI_FLAG == 0 ) && ( t < 0XFFF0 ) )
        {
            t++;
        }

        i++;
        UART_0.TI_FLAG = 0;
    }

}

//发送数组
void UART0_Tx_Data( uint8_t* buf, uint8_t len )
{

    uint8_t i = 0;
    uint16_t t = 0;

    for( i = 0; i < len; i++ )
    {
        UART_0.TI_FLAG = 0; //在串口中断中，当发送完成后置1
        SBUF = *( buf + i );

        //baute
        //OK:9600 19200
        //ERR:>19200


        /*
        9600--   t测试等待约 713         0XFFF0
        19200--  t测试等待约 355         0XFFF0
        38400--  t测试等待约 178
        115200-- t测试等待约 60
        */
        while( ( UART_0.TI_FLAG == 0 ) && ( t < 0XFFF0 ) ) //  ERR:0XFFF 4096
        {
            t++;
        }

        UART_0.TI_FLAG = 0;
    }

}

//发送ASCII码数字
void UART0_Tx_Num( uint16_t num )
{
    uint8_t buf[5];

    if( num < 10 )
    {
        buf[0] = ( num / 1 % 10 ) + 0x30;
        UART0_Tx_Data( buf, 1 );
    }
    else if( num < 100 )
    {
        buf[0] = ( num / 10 % 10 ) + 0x30;
        buf[1] = ( num / 1 % 10 ) + 0x30;
        UART0_Tx_Data( buf, 2 );
    }
    else if( num < 1000 )
    {

        buf[0] = ( num / 100 % 10 ) + 0x30;
        buf[1] = ( num / 10 % 10 ) + 0x30;
        buf[2] = ( num / 1 % 10 ) + 0x30;

        UART0_Tx_Data( buf, 3 );

    }
    else if( num < 10000 )
    {
        buf[0] = ( num / 1000 % 10 ) + 0x30;
        buf[1] = ( num / 100 % 10 ) + 0x30;
        buf[2] = ( num / 10 % 10 ) + 0x30;
        buf[3] = ( num / 1 % 10 ) + 0x30;

        UART0_Tx_Data( buf, 4 );

    }
    else if( num < 100000 )
    {
        buf[0] = ( num / 10000 % 10 ) + 0x30;
        buf[1] = ( num / 1000 % 10 ) + 0x30;
        buf[2] = ( num / 100 % 10 ) + 0x30;
        buf[3] = ( num / 10 % 10 ) + 0x30;
        buf[4] = ( num / 1 % 10 ) + 0x30;

        UART0_Tx_Data( buf, 5 );

    }



}











/*******************************************************************************
  * @name       : UART0_Rx 10ms
  * @brief      : deal UART receive data & time out check
  * @note       : null
  * @param      : null
  * @retval     : null
  * @author     : 2017-3-3
*******************************************************************************/
void UART0_Rx( void )
{
    uint8_t index = 0;
    uint8_t check_sum_buf = 0;
    uint8_t check_sum_index = 0;
    uint8_t find_head = 0;
    uint8_t cnt = 0;

    //time out check
    if( UART_0.RxTimeOutCnt < 0xFF )
    {
        UART_0.RxTimeOutCnt++;

        if( UART_0.RxTimeOutCnt == RX_TIME_OUT ) //3*10ms
        {

            //如果是OS,加临界区
            //deal receive data
            if( UART_0.F_RxSynch == 1 )
            {

                if( UART0_Buf_Find( UART_0.RxBuf, UART_0.RxCnt, "CONNECT OK", 10 )  >= 0 )
                {
                    M26_Gprs.TCP_CONNETED = 1;
                    M26_Gprs.disconnet_cnt = DISCONNET_CNT_MAX;
                }
                
                
                if( UART0_Buf_Find( UART_0.RxBuf, UART_0.RxCnt, "open", 4 )  >= 0 )
                {
                    M26_Gprs.TCP_CONNETED = 1;
                    M26_Gprs.disconnet_cnt = DISCONNET_CNT_MAX;
                    
                    Lock_Control( 1 );
                    
                }
                
                
#if 0
                //接收到数据
                for( index = 0; index < UART_0.RxCnt ; index++ )
                {
                    if( UART_0.RxBuf[index] == 0xBB )
                    {
                        find_head = 1;
                        M26_Gprs.TCP_CONNETED = 1;
                        M26_Gprs.disconnet_cnt = DISCONNET_CNT_MAX;

                        break;
                    }

                }

                if( find_head == 1 )
                {
                    find_head = 0;

                    check_sum_buf = check_sum_add_8( &UART_0.RxBuf[index + TYPE_OFFSET], UART_0.RxBuf[index + LEN_OFFSET] + 2 );
                    check_sum_index = index + UART_0.RxBuf[index + LEN_OFFSET] + 2 + 1;

                    if( check_sum_buf == UART_0.RxBuf[check_sum_index] )
                    {
                        /*
                        0x00-开关
                        0x01-状态
                        0x02-MAC
                        0x03-蜂鸣器

                        */
                        switch( UART_0.RxBuf[index + TYPE_OFFSET] )
                        {

                            case 0x00:
                            {
                                Lock_Control( UART_0.RxBuf[index + DATA1_OFFSET] );
                                break;
                            }

                            case 0x01:
                            {
                                RunDataUpDate();
                                break;
                            }

                            case 0x02:
                            {

                                //服务器数据应答-
                                cnt = 0;
                                memset( M26_Gprs.send_data_buf, 0, sizeof( M26_Gprs.send_data_buf ) );
                                M26_Gprs.send_data_buf[cnt++] = 0xBB; //head
                                M26_Gprs.send_data_buf[cnt++] = 0x82; //type
                                M26_Gprs.send_data_buf[cnt++] = 0x06; //len

                                M26_Gprs.send_data_buf[cnt++] = Control.MCU_96bit_UUID[0];
                                M26_Gprs.send_data_buf[cnt++] = Control.MCU_96bit_UUID[1];
                                M26_Gprs.send_data_buf[cnt++] = Control.MCU_96bit_UUID[2];
                                M26_Gprs.send_data_buf[cnt++] = Control.MCU_96bit_UUID[3];
                                M26_Gprs.send_data_buf[cnt++] = Control.MCU_96bit_UUID[4];
                                M26_Gprs.send_data_buf[cnt++] = Control.MCU_96bit_UUID[5];

                                check_sum_buf = check_sum_add_8( &M26_Gprs.send_data_buf[1], M26_Gprs.send_data_buf[2] + 2 );
                                M26_Gprs.send_data_buf[cnt++] = check_sum_buf; //crc
                                M26_Gprs.send_data_buf[cnt++] = 0x0D;
                                M26_Gprs.send_data_buf[cnt++] = 0x0A;

                                M26_Gprs.send_data_len = cnt;
                                M26_Gprs.send_data_flag = 1;
                                //服务器数据应答-

                                break;
                            }

                            case 0x03:
                            {

                                //51 49 52 44 3A 31 31 36 2E 32 32 38 2E 31 34 36 2E 32 35 30 3A 37 30 37 30 2C 54 43 50 2C 33 36 20 BB 04 01 01
                                Control.Buzzer_OnOff = UART_0.RxBuf[index + DATA1_OFFSET];

                                //服务器数据应答-
                                cnt = 0;
                                memset( M26_Gprs.send_data_buf, 0, sizeof( M26_Gprs.send_data_buf ) );
                                M26_Gprs.send_data_buf[cnt++] = 0xBB; //head
                                M26_Gprs.send_data_buf[cnt++] = 0x83; //type
                                M26_Gprs.send_data_buf[cnt++] = 0x02; //len

                                M26_Gprs.send_data_buf[cnt++] = Control.Buzzer_OnOff;
                                M26_Gprs.send_data_buf[cnt++] = 0x01;

                                check_sum_buf = check_sum_add_8( &M26_Gprs.send_data_buf[1], M26_Gprs.send_data_buf[2] + 2 );
                                M26_Gprs.send_data_buf[cnt++] = check_sum_buf; //crc
                                M26_Gprs.send_data_buf[cnt++] = 0x0D;
                                M26_Gprs.send_data_buf[cnt++] = 0x0A;

                                M26_Gprs.send_data_len = cnt;
                                M26_Gprs.send_data_flag = 1;
                                //服务器数据应答-


                                break;
                            }


                            default:
                                break;

                        }

                    }


                    if( UART_0.RxBuf[index + TYPE_OFFSET] <= 5 )
                    {

                    }

                }
                
#endif              
                

            }


            //time out,clear,for next correct rx
            memset( UART_0.RxBuf, 0, sizeof( UART_0.RxBuf ) );
            UART_0.F_RxSynch = 0;
            UART_0.RxCnt = 0;
            //如果是OS,加临界区

        }
    }

}

/*******************************************************************************
  * @name       : UART0_Rx_Isr
  * @brief      : receive the data,check the synchronize head
  * @note       : call by UART rx interrupt service
  * @param      : null
  * @retval     : null
  * @author     : 2017-3-3
*******************************************************************************/
void UART0_Rx_Isr( void )
{

    // RX
    if( RI )
    {
        RI = 0;
        UART_0.RxBuf[UART_0.RxCnt] = SBUF; //get rx data

        UART_0.RxCnt++;
        UART_0.RxTimeOutCnt = 0;

        UART_0.F_RxSynch = 1;


    }

}






/*****************************************************************************
  * @file       : at24c02
  * @brief      : 低位存放高字节
  * @note       : null
  * @version    : V1.0
  * @date       : 2017-7-25
******************************************************************************/

/*
EEPROM

0.factory_flag--0xAA
1.sw_memory_flag-- bits  0=off 1=on   bit0~bit7==sw1~sw8
2.sw1_cmd-- 0=off 1=on
3.sw2_cmd-- 0=off 1=on
4.sw3_cmd-- 0=off 1=on
5.sw4_cmd-- 0=off 1=on
6.sw5_cmd-- 0=off 1=on
7.sw6_cmd-- 0=off 1=on
8.sw7_cmd-- 0=off 1=on
9.sw8_cmd-- 0=off 1=on

14.time_limit Hight8Byte
15.time_limit Low8Byte


*/



//.C
/*============包含标准库头文件================================================*/

/*============包含非标准库头文件==============================================*/
#include "m_include.h"
#include "at24c02.h"
#include "test.h"
/*============文件内部 常量定义 const ========================================*/

/*============文件内部 宏定义 define =========================================*/
//IO定义
#define SDA_R()         P14//P14

#define SDA_H           P14=1;//P14
#define SDA_L           P14=0;

#define SCL_H           P13=1;//P13
#define SCL_L           P13=0;

#define IIC_Add         0xA0    //器件地址
/*============文件内部 数据类型定义 typedef ==================================*/

/*============文件内部 局部变量定义 static ===================================*/

/*============文件内部 全局变量定 ============================================*/
xdata AT24C02_Struct at24c02;
/*============文件内部 函数原型定义 static ===================================*/

/*****************************************************************************
  * @name       : null
  * @brief      : null
  * @note       : null
  * @param      : null
  * @retval     : null
  * @date       : 2017-7-26
******************************************************************************/

static void delay10us( void );
static void I2C_Start( void );
static void I2C_Stop( void );
static void Ack( void );
static void NoAck( void );
static uint8_t Test_Ack( void );
static void SendData( uint8_t buffer );
static uint8_t ReceiveData( void );


static void delay10us( void )
{
    uint8_t i;

    i = 40;

    while( i )
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        i--;
    }

}

//**********************************************
//送起始位 sda=1->0
static void I2C_Start( void )
{
    SDA_H;
    SCL_H;
    delay10us();
    SDA_L;
    delay10us();
    SCL_L;
}

//************************************************
//送停止位 sda=0->1
static void I2C_Stop( void )
{
    SDA_L;
    delay10us();
    SCL_H;
    delay10us();
    SDA_H;
}

//************************************************
//主应答
static void Ack( void )
{
    /*
    //设置SDA 口为输出
    SDA_L;
    SCL_L;
    delay10us();
    SCL_H;
    delay10us();
    SCL_L;
    SDA_H;
    */

    SDA_L;
    delay10us();
    SCL_H;
    delay10us();
    SCL_L;


}

//************************************************
//主不应答
static void NoAck( void )
{
    /*
    //设置SDA 口为输出
    SDA_H;
    SCL_L;
    delay10us();
    SCL_H;
    delay10us();
    SDA_H;
    SCL_L;
    */

    SDA_H;
    delay10us();
    SCL_H;
    delay10us();
    SCL_L;
    delay10us();


}

// 检测 SDA是否回ACK
static uint8_t Test_Ack( void )
{
    //设置SDA 口为输入
    uint8_t ACK_Flag = 0;

    SCL_L;
    SDA_H;
    delay10us();
    SCL_H;
    delay10us();

    /*
    if(SDA_R()==0)
        ACK_Flag = 1;
    else
        ACK_Flag = 0;
    */

    SCL_L;

    ACK_Flag = 1;//忽略应答

    return ACK_Flag;
}

//*************************************************
//字节发送程序
//发送(可以是数据也可是地址)，送完后接收从应答
//不考虑从应答位
static void SendData( uint8_t buffer )
{
    uint8_t BitCnt = 8; //一字节8位

    //设置SDA 口为输出
    do
    {
        SCL_L;
        delay10us();

        if( ( buffer & 0x80 ) == 0 ) //判断最高位是0还是1
        {
            SDA_L;
        }
        else
        {
            SDA_H;
        }

        SCL_H;
        delay10us();
        buffer = buffer << 1; //将buffer中的数据左移一位
        BitCnt--;
    }
    while( BitCnt );

    SCL_L;
}

//**************************************************
//字节接收程序
//接收器件传来的数据，此程序应配合应答函数使用
static uint8_t ReceiveData( void )
{
    uint8_t BitCnt = 8, IIC_RX_Data = 0;

    SDA_H;//读入数据 设置SDA 口为输入

    do
    {
        SCL_L;
        delay10us();
        IIC_RX_Data <<= 1; //数据左移一位
        BitCnt--;
        SCL_H;
        delay10us();

        if( SDA_R() == 1 )
            IIC_RX_Data = IIC_RX_Data | 0x01; //低位置1
        else
            IIC_RX_Data = IIC_RX_Data & 0x0fe; //低位清0
    }
    while( BitCnt );

    SCL_L;
    return IIC_RX_Data;
}



/*****************************************************************************
  * @name       : at24c02_write_byte_reg
  * @brief      : 向指定寄存器写入一个字节
  * @note       : null
  * @param      : null
  * @retval     : null
  * @date       : 2017-7-26
******************************************************************************/
uint8_t at24c02_write_byte_reg( uint8_t reg, uint8_t wdata )
{

    I2C_Start();  //启动I2C
    SendData( IIC_Add ); //发送器件地址

    if( !Test_Ack() )
    {

        return( 0 );
    }

    SendData( reg ); //写入寄存器地址

    if( !Test_Ack() )
    {

        return( 0 );
    }

    SendData( wdata ); //写入数据

    if( !Test_Ack() )
    {

        return( 0 );
    }

    I2C_Stop();

    return( 1 );


}


/*****************************************************************************
  * @name       : at24c02_read_byte_reg
  * @brief      : 连续读
  * @note       : null
  * @param      : reg:寄存器地址 *buf：保存数据的数组指针  length：长度
  * @retval     : null
  * @date       : 2017-7-26
******************************************************************************/
uint8_t at24c02_read_byte_reg( uint8_t reg, uint8_t* buf, uint8_t length )
{
    uint8_t i;

    I2C_Start();  //启动I2C
    SendData( IIC_Add ); //发送器件地址

    if( !Test_Ack() )
    {

        return( 0 );
    }

    SendData( reg ); //写入寄存器地址

    if( !Test_Ack() )
    {

        return( 0 );
    }

    delay10us();
    delay10us();
    delay10us();

    I2C_Start();// 启动I2C
    SendData( ( IIC_Add ) | 0x01 ); //发送器件地址

    if( !Test_Ack() )
    {

        return( 0 );
    }

    for( i = 0; i < length - 1; i++ ) //读取字节数据
    {
        *( buf + i ) = ReceiveData(); //读取数据
        Ack();
    }

    *( buf + length - 1 ) = ReceiveData();
    NoAck();
    I2C_Stop();

    return( 1 );

}


/*****************************************************************************
  * @name       : at24c02_write_flow
  * @brief      : 将 write_data数组以一定间隔写到EEP  10ms
  * @note       : null
  * @param      : null
  * @retval     : null
  * @date       : 2017-7-26
******************************************************************************/
void at24c02_write_flow( void )
{

    if( at24c02.update ) //更新
    {

        at24c02_write_byte_reg( at24c02.step, at24c02.write_data[at24c02.step] );
        at24c02.step++;

        if( at24c02.step >= BUFF_SIZE ) //耗时N*10ms
        {
            //结束
            at24c02.step = 0;
            at24c02.update = 0;

        }

    }

}


/*****************************************************************************
  * @name       : at24c02_update_data
  * @brief      : 将实际数据更新到write_data数组 在需要的地方调用
  * @note       : null
  * @param      : reset:1：将首次上电标志清零，设置成第一次出厂
  * @retval     : null
  * @date       : 2017-7-26
******************************************************************************/
void at24c02_update_data( uint8_t reset )
{
    uint8_t i;


    // if(at24c02.update!=1)
    {

        at24c02.update = 1;
        at24c02.step = 0;

        memset( at24c02.write_data, 0, BUFF_SIZE );

        if( reset )
        {

        }
        else
        {
            at24c02.write_data[0] = 0xAA;
            at24c02.write_data[1] = Control.sw_memory_flag;

            for( i = 0; i < 8; i++ )
            {

                if( Control.sw_memory_flag & ( 1 << i ) )
                {
                    at24c02.write_data[i + 2] = Control.sw_cmd[i];
                }

            }


        }


    }

}


/*****************************************************************************
  * @name       : at24c02_read_all
  * @brief      : 开机读取全部数据 at24c02_init调用
  * @note       : null
  * @param      : null
  * @retval     : null
  * @date       : 2017-7-26
******************************************************************************/
static void at24c02_read_all( void )
{

    at24c02_read_byte_reg( 0x00, at24c02.read_data, BUFF_SIZE );


    if( at24c02.read_data[0] == 0xAA )
    {
        //非首次上电
        Control.sw_memory_flag = at24c02.read_data[1];

        Control.sw_cmd[SW1] = at24c02.read_data[2];
        Control.sw_cmd[SW2] = at24c02.read_data[3];
        Control.sw_cmd[SW3] = at24c02.read_data[4];
        Control.sw_cmd[SW4] = at24c02.read_data[5];
        Control.sw_cmd[SW5] = at24c02.read_data[6];
        Control.sw_cmd[SW6] = at24c02.read_data[7];
        Control.sw_cmd[SW7] = at24c02.read_data[8];
        Control.sw_cmd[SW8] = at24c02.read_data[9];

    }
    else
    {
        //首次上电
        Control.sw_memory_flag = 0xFF;

        Control.sw_cmd[SW1] = 0x00;
        Control.sw_cmd[SW2] = 0x00;
        Control.sw_cmd[SW3] = 0x00;
        Control.sw_cmd[SW4] = 0x00;
        Control.sw_cmd[SW5] = 0x00;
        Control.sw_cmd[SW6] = 0x00;
        Control.sw_cmd[SW7] = 0x00;
        Control.sw_cmd[SW8] = 0x00;

        at24c02_update_data( 0 );

    }


}


/*****************************************************************************
  * @name       : at24c02_factory_reset
  * @brief      : 将出厂数据更新到write_data数组
  * @note       : null
  * @param      : null
  * @retval     : null
  * @date       : 2017-7-26
******************************************************************************/
void at24c02_factory_reset( void )
{
    /*

    TDS1.Value = 999;
    TDS2.Value = 0;

    filter.ro_life_work_time_min = 0;
    filter.ro_life_poweron_time_min = 0;
    filter.ro_life_percent = 100;

    filter.c_life_work_time_min = 0;
    filter.c_life_poweron_time_min = 0;
    filter.c_life_percent = 100;

    filter.uf_life_work_time_min = 0;
    filter.uf_life_poweron_time_min = 0;
    filter.uf_life_percent = 100;

    bt_sensor_in.toc = 0;
    bt_sensor_in.cod = 0;
    bt_sensor_in.uv254 = 0;

    bt_sensor_out.toc = 0;
    bt_sensor_out.cod = 0;
    bt_sensor_out.uv254 = 0;

    Control.total_water_flow = 0;
    Control.total_water_flow_wifi = 0;

    filter.ro_filter_end = 0;
    filter.c_filter_end = 0;
    filter.uf_filter_end = 0;

    at24c02_update_data( 1 );

    */

}




/*****************************************************************************
  * @name       : at24c02_init
  * @brief      : null
  * @note       : null
  * @param      : null
  * @retval     : null
  * @date       : 2017-7-26
******************************************************************************/
void at24c02_init( void )
{

    //开漏输出
    //上电时保证两总线为高

    //SCL
    P1M1 |= SET_BIT3;
    P1M2 |= SET_BIT3;
    //SDA
    P1M1 |= SET_BIT4;
    P1M2 |= SET_BIT4;

    SCL_H;
    SDA_H;

    at24c02_read_all();
}






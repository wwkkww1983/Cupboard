#ifndef _TYPEDEF_H
#define _TYPEDEF_H
/******************************************************************************/
#include <stdlib.h>
#include <string.h>
//#include <stdio.h>
/******************************************************************************/
#include "N76E003.h"
/******************************************************************************/
typedef     unsigned char           uint8_t;    
typedef     signed char             int8_t;
typedef     unsigned int            uint16_t;
typedef     signed int              int16_t;
typedef     unsigned long int       uint32_t;
typedef     signed long int         int32_t;


#ifndef  u64
    #define  u64  uint64_t
#endif

#ifndef  s64
    #define  s64  int64_t
#endif

#ifndef  u32
    #define  u32   uint32_t
#endif

#ifndef  s32
    #define  s32   int32_t
#endif

#ifndef  u16
    #define  u16	uint16_t
#endif

#ifndef  s16
    #define  s16	int16_t
#endif

#ifndef  u8
    #define  u8   uint8_t
#endif

#ifndef  s8
    #define  s8   int8_t
#endif

#ifndef byte
    #define byte   u8
#endif

#ifndef Byte
    #define Byte   u8
#endif

#ifndef sbyte
    #define sbyte  s8
#endif

#ifndef Sbyte
    #define Sbyte  s8
#endif

#ifndef BOOL
    #define BOOL   u8
#endif

#ifndef bool
    #define bool   u8
#endif

/******************************************************************************/
#ifndef U8_MAX
    #define U8_MAX  (255)
#endif

#ifndef S8_MAX
    #define S8_MAX  (127)
#endif

#ifndef S8_MIN
    #define S8_MIN  (-128)
#endif

#ifndef U16_MAX
    #define U16_MAX (65535u)
#endif

#ifndef S16_MAX
    #define S16_MAX (32767)
#endif

#ifndef S16_MIN
    #define S16_MIN (-32768)
#endif

#ifndef U32_MAX
    #define U32_MAX (4294967295uL)
#endif

#ifndef S32_MAX
    #define S32_MAX (2147483647)
#endif

#ifndef S32_MIN
    #define S32_MIN (-2147483648uL)
#endif
/******************************************************************************/
#ifndef ON
    #define ON         (1)
#endif

#ifndef OFF
    #define OFF        (0)
#endif

#ifndef true
    #define true       (1)
#endif

#ifndef false
    #define false      (0)
#endif

#ifndef TRUE
    #define TRUE       (1)
#endif

#ifndef FALSE
    #define FALSE      (0)
#endif

#ifndef OK
    #define OK         (1)
#endif

#ifndef ERROR
    #define ERROR      (0)
#endif

#ifndef Err
    #define Err        (0)
#endif


#ifndef SUCCESS
    #define SUCCESS    (1)
#endif

#ifndef FAILED
    #define FAILED     (0)
#endif

#ifndef EN
    #define EN         (1)
#endif

#ifndef DIS
    #define DIS        (0)
#endif

#ifndef null
    #define null        (0)
#endif

#ifndef NULL
    #define NULL 0      (0)
#endif
/******************************************************************************/
typedef const char* string;
#define   SECONDS(x)            (x*1000ul)
#define   MINUTES(x)            (x*60ul*1000ul)
//#define   offset(TYPE,MEMBER)   ((u32)&((TYPE *)0)->MEMBER)
#define   ARR_SIZE(a)           (u16)((sizeof((a)) / sizeof((a[0]))))
#define   FSIZ(type, field)     sizeof(((type*)0)->field)
#define   FPOS(type, field)     ((u32) &((type*)0)->field)
#define   MEM_B(x)              (*((byte*) (x)))
#define   MEM_W(x)              (*((u16*) (x)))
#define   MAX(x, y)             (((x) > (y)) ? (x) : (y) )
#define   MIN(x, y)             (((x) < (y)) ? (x) : (y) )
#define   FLIPW(ray)            ((((u16) (ray)[0]) * 256) + (ray)[1] )
#define   FLOPW(ray, val)       (ray)[0] = ((val) / 256);(ray)[1] = ((val) & 0xFF);
#define   B_PTR( var )          ((byte*) (void *) &(var))
#define   W_PTR( var )          ((word*) (void *) &(var))
#define   WORD_LO(xxx)          ((byte) ((word)(xxx) & 255))
#define   WORD_HI(xxx)          ((byte) ((word)(xxx) >> 8))
#define   UPCASE(c)             (((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c))
#define   DECCHK(c)             ((c) >= '0' && (c) <= '9')
#define   HEXCHK( c )           (((c) >= '0' && (c) <= '9') ||((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))
#define   inp(port)             (*((volatile byte *) (port)))
#define   inpw(port)            (*((volatile word *) (port)))
#define   inpdw(port)           (*((volatile dword *)(port)))
#define   outp(port, val)       (*((volatile byte *) (port)) = ((byte) (val)))
#define   outpw(port, val)      (*((volatile word *) (port)) = ((word) (val)))
#define   outpdw(port, val)     (*((volatile dword *) (port)) = ((dword) (val)))
/******************************************************************************/
#define DI()  do{EA = 0;} while (0)
#define EI()  do{EA = 1;} while (0)
#include <intrins.h>
#define NOP()  _nop_()
/******************************************************************************/
#define SetSem(VAR) ((VAR) = TRUE)
#define ClrSem(VAR) ((VAR) = FALSE)
#define SetSem_IT(VAR) {DI();(VAR) = TRUE; EI();}
#define ClrSem_IT(VAR) {DI();(VAR) = FALSE;EI();}
#define GetIntVal(val, IntVal) {DI(); val = IntVal; EI();};
#define SetIntVal(IntVal, val) {DI(); IntVal = val; EI();};
/*============================== Handling bits ====================================*/
/*-----------------------------------------------------------------------------
Method : I
Description : Handle the bit from the character variables.
Comments :    The different parameters of commands are
              - VAR : Name of the character variable where the bit is located.
              - Place : Bit position in the variable (7 6 5 4 3 2 1 0)
              - Value : Can be 0 (reset bit) or not 0 (set bit)
              The "MskBit" command allows to select some bits in a source
              variables and copy it in a destination var (return the value).
              The "ValBit" command returns the value of a bit in a char
              variable: the bit is reset if it returns 0 else the bit is set.
              This method generates not an optimised code yet.
-----------------------------------------------------------------------------*/
#define SetBit(VAR,Place)         ( (VAR) |= (uint8_t)((uint8_t)1<<(uint8_t)(Place)) )
#define ClrBit(VAR,Place)         ( (VAR) &= (uint8_t)((uint8_t)((uint8_t)1<<(uint8_t)(Place))^(uint8_t)255) )

#define SetBit16(VAR,Place)         ( (VAR) |= (u16)((u16)1<<(u16)(Place)) )
#define ClrBit16(VAR,Place)         ( (VAR) &= (u16)((u16)((u16)1<<(u16)(Place))^(u16)0xFFFF) )

#define ChgBit(VAR,Place)         ( (VAR) ^= (uint8_t)((uint8_t)1<<(uint8_t)(Place)) )
#define AffBit(VAR,Place,Value)   ((Value) ? \
                                   ((VAR) |= ((uint8_t)1<<(Place))) : \
                                   ((VAR) &= (((uint8_t)1<<(Place))^(uint8_t)255)))
#define MskBit(Dest,Msk,Src)      ( (Dest) = ((Msk) & (Src)) | ((~(Msk)) & (Dest)) )

#define ValBit(VAR,Place)         ((uint8_t)(VAR) & (uint8_t)((uint8_t)1<<(uint8_t)(Place)))
#define ValBit16(VAR,Place)       ((uint16_t)(VAR) & (uint16_t)((uint16_t)1<<(uint16_t)(Place)))

#define BYTE_0(n)                 ((uint8_t)((n) & (uint8_t)0xFF))        /*!< Returns the low byte of the 32-bit value */
#define BYTE_1(n)                 ((uint8_t)(BYTE_0((n) >> (uint8_t)8)))  /*!< Returns the second byte of the 32-bit value */
#define BYTE_2(n)                 ((uint8_t)(BYTE_0((n) >> (uint8_t)16))) /*!< Returns the third byte of the 32-bit value */
#define BYTE_3(n)                 ((uint8_t)(BYTE_0((n) >> (uint8_t)24))) /*!< Returns the high byte of the 32-bit value */
/******************************************************************************/

#endif

#ifndef SEM_H_INCLUDED
#define SEM_H_INCLUDED
#include "typedef.h"
/*-----------------------------------------------------------------------------------*/
typedef enum
{
    BASE_SEM = 0,

    MAX_SEM
} SEM_ID;
/*-----------------------------------------------------------------------------------*/
typedef enum
{
    BASE_MUTEX = 0,

    MAX_MUTEX
} MUTEX_ID;
/*-----------------------------------------------------------------------------------*/

extern u8   SEM[];
extern BOOL MUTEX[];

BOOL getSem(u8* k);
/*-----------------------------------------------------------------------------------*/

#endif // SEM_H_INCLUDED

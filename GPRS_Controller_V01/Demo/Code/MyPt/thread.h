#ifndef __THREAD_H__
#define __THREAD_H__
/*   MyPt V1.2 HeT WangDong 2017-12-22 */
/*--------------------------------------------------------------------------------------------------------------------------------*/
#include "typedef.h"
#include "pt.h"
#include "sem.h"
#include "systick.h"
/*--------------------------------------------------------------------------------------------------------------------------------*/
typedef struct pt thread_t;

typedef char (*pFUNC)(thread_t* pt);

typedef struct timer_t
{
    u32 start;
    u32 interval;

} timer_t;

typedef enum
{
    THREAD_WAITING = 0,
    THREAD_YIELDED,
    THREAD_EXITED,
    THREAD_ENDED,
    THREAD_SUSPENDED,
    THREAD_OK,
    THREAD_ERR

} ThreadStatus ;

typedef struct
{
    pFUNC func;
    thread_t pt;
    ThreadStatus  status;
    u8 suspended;

} pt_thread;

typedef struct
{
    ThreadStatus status;
    u8  SizeofThreadBytes;
    u16 SizeofTaskBytes;
    u8  Index;
    u8  MaxThreadIndex;
    u8  MaxThread;

} ptThreadInfo;
/*--------------------------------------------------------------------------------------------------------------------------------*/
#define Thread_Init(pt)                      PT_INIT(pt)
#define Thread_BEGIN                         static timer_t dly;PT_BEGIN(pt);
#define Thread_END                           PT_END(pt);
#define Thread_Restart()                     PT_RESTART(pt)
#define Thread_Exit()                        PT_EXIT(pt)
#define Thread_Yield()                       PT_YIELD(pt)
#define Thread_TIMEOUT()                     timer_expired(&dly)
#define Thread_Interrupt()                   do{while(1){Thread_Yield();}}while(0)
#define Thread_Wait_Until(cond)              PT_WAIT_UNTIL(pt, cond)
#define Thread_Sleep(SysTick)                do{timer_set((&dly),(SysTick));Thread_Wait_Until(timer_expired(&dly));}while(0)
#define Thread_Sleep_Until(SysTick, cond)    do{timer_set((&dly),(SysTick));Thread_Wait_Until(cond||timer_expired(&dly));}while(0)
#define Thread_Wait_Sem(x)                   do{Thread_Wait_Until(x);ClrSem(x);}while(0)
#define Thread_Abort()                       Thread_Exit()
#define Thread_Join(x)                       do{Thread_Wait_Until(getThreadStatus(x) == THREAD_EXITED);}while(0)
#define Thread_Wait_Mutex(x)                 do{Thread_Wait_Until(!x);SetSem(x);}while(0)
#define Thread_IsAlive(x)                   (getThreadStatus(x) < THREAD_EXITED)
#define Thread_Wait_Until_IT(condition)      PT_WAIT_UNTIL_IT(pt, condition)
#define Thread_Sleep_Until_IT(SysTick, con)  do{timer_set((&dly),(SysTick));Thread_Wait_Until_IT(con||timer_expired(&dly));}while(0)
#define PT_WAIT_UNTIL_IT(pt, condition)\
    do {\
        DI();\
        LC_SET((pt)->lc);\
        if(!(condition)) {\
            EI();\
            return PT_WAITING;\
        }\
        EI();\
    } while(0)
/*--------------------------------------------------------------------------------------------------------------------------------*/
#define MAX_THREADS    (8)
#define RstWatchDog()  WdogRst()
/*--------------------------------------------------------------------------------------------------------------------------------*/
extern ptThreadInfo threadInfo;
void   timer_set(timer_t* t, u32 interval);
BOOL   timer_expired(timer_t* t);
void   Thread_Start(pFUNC func, ThreadStatus* status);
void   Thread_Suspend(pFUNC func, ThreadStatus* status);
void   Thread_Resume(pFUNC func, ThreadStatus* status);
ThreadStatus getThreadStatus(pFUNC func);
void TaskRun(void);

//#define TEST_VA_ARGS(...)   char abcdefg[] = {__VA_ARGS__, 0}
/*--------------------------------------------------------------------------------------------------------------------------------*/
#endif /* __THREAD_H__ */

/** @} */
/** @} */


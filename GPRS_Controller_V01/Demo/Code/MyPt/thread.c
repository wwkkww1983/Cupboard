#include "thread.h"
#include "m_include.h"
/*---------------------------------------------------------------------------*/
static  pt_thread gTask[MAX_THREADS + 1u] ;//
ptThreadInfo threadInfo = {THREAD_WAITING, 0, 0, 0, MAX_THREADS};
/*---------------------------------------------------------------------------*/
void TaskRun(void)
{
	while(true)
    {
        static  pt_thread* p;
        threadInfo.Index = 0;
        for(p = gTask; p->func ; p++, threadInfo.Index++)
        {
            if(p->status < THREAD_EXITED)
            {
                p->status = p->func(&p->pt);
                RstWatchDog();
            }
        }
    }
}
/*---------------------------------------------------------------------------*/
/** \brief
 *
 * \param
 * \param
 * \return
 *
 */
BOOL timer_expired(timer_t* t)
{
     u32 clock ;
    DI();
    clock = gU32Systick;
    EI();
    clock -= t->start;

    if(clock >= t->interval)
    {
        return true;
    }

    return false;
}
/*---------------------------------------------------------------------------*/
/** \brief
 *
 * \param
 * \param
 * \return
 *
 */

void timer_set(timer_t* t, u32 interval)
{
    t->interval = interval;
    DI();
    t->start = gU32Systick;
    EI();
}
/*---------------------------------------------------------------------------*/
/** \brief
 *
 * \param
 * \param
 * \return
 *
 */

void Thread_Start(pFUNC func, ThreadStatus* status)
{
     u8 threadIndex = 0 ;

// First find a gap in the array (if there is one)
    while((gTask[threadIndex].func != NULL) && (gTask[threadIndex].func != func) && (threadIndex < MAX_THREADS))
    {
        threadIndex++;
    }

    if(gTask[threadIndex].func == func)
    {
        gTask[threadIndex].pt.lc = 0;
        gTask[threadIndex].status = THREAD_WAITING;
        gTask[threadIndex].suspended = false;
        *status = THREAD_OK;
        return ;
    }

    if(threadIndex == MAX_THREADS)
    {
        *status = THREAD_ERR;
        return ;
    }

// If we're here, there is a space in the task array
    gTask[threadIndex].func = func;
    gTask[threadIndex].pt.lc = 0;
    gTask[threadIndex].status = THREAD_WAITING;
    gTask[threadIndex].suspended = false;
    threadInfo.MaxThreadIndex = threadIndex;          // return position of task (to allow later deletion)
    threadInfo.SizeofTaskBytes = sizeof(gTask);
    threadInfo.SizeofThreadBytes = sizeof(pt_thread);
    threadInfo.MaxThread = ARR_SIZE(gTask) - 1;
    *status = THREAD_OK;
}
/*---------------------------------------------------------------------------*/
/** \brief
 *
 * \param
 * \param
 * \return
 *
 */
ThreadStatus getThreadStatus(pFUNC func)
{
     u8 threadIndex = 0;

    while((gTask[threadIndex].func != NULL)  && (gTask[threadIndex].func != func) && (threadIndex < MAX_THREADS))
    {
        threadIndex++;
    }

    if(gTask[threadIndex].func == func)
    {
        return gTask[threadIndex].status;
    }

    return THREAD_ERR;
}
/*---------------------------------------------------------------------------*/
/** \brief
 *
 * \param
 * \param
 * \return
 *
 */
void Thread_Suspend(pFUNC  func, ThreadStatus* status)
{
     u8 threadIndex = 0;

    while((gTask[threadIndex].func != NULL) && (gTask[threadIndex].func != func) && (threadIndex < MAX_THREADS))
    {
        threadIndex++;
    }

    if(gTask[threadIndex].func == func)
    {
        if(gTask[threadIndex].status != THREAD_SUSPENDED && gTask[threadIndex].suspended == false)
        {
            gTask[threadIndex].status = THREAD_SUSPENDED;
            gTask[threadIndex].suspended = threadInfo.Index;
            *status = THREAD_OK;
            return;
        }
    }

    *status = THREAD_ERR;
}
/*---------------------------------------------------------------------------*/
/** \brief
 *
 * \param
 * \param
 * \return
 *
 */
void Thread_Resume(pFUNC func, ThreadStatus* status)
{
     u8 threadIndex = 0;

    while((gTask[threadIndex].func != NULL) && (gTask[threadIndex].func != func) && (threadIndex < MAX_THREADS))
    {
        threadIndex++;
    }

    if(gTask[threadIndex].func == func)
    {
        if(gTask[threadIndex].status == THREAD_SUSPENDED && gTask[threadIndex].suspended == threadInfo.Index)
        {
            gTask[threadIndex].status = THREAD_WAITING;
            gTask[threadIndex].suspended = false;
            *status = THREAD_OK;
            return;
        }
    }

    *status = THREAD_ERR;
}

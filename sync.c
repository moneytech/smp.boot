/*
 * =====================================================================================
 *
 *       Filename:  sync.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  19.10.2011 16:11:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Georg Wassen (gw) (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "sync.h"




void mutex_init(mutex_t *m)
{
    *m = MUTEX_INITIALIZER;
} 

void mutex_lock(mutex_t *m)
{
    while (!__sync_bool_compare_and_swap(m, 1, 0)) {};
}

int mutex_trylock(mutex_t *m)
{
    return (__sync_bool_compare_and_swap(m, 1, 0));
}

void mutex_unlock(mutex_t *m)
{
    *m = 1;
}



void barrier_init(barrier_t *b, int max)
{
    *b = BARRIER_INITIALIZER(max);
}

void barrier(barrier_t *b)
{
    unsigned e = b->epoch;
    unsigned c = __sync_add_and_fetch(&(b->cnt), 1);
    if (c == b->max) {
        /* release by incrementing epoch */
        b->cnt = 0;
        b->epoch++;
    } else {
        /* wait for epoch to be incremented */
        while (e == b->epoch) {};
    }
}


void flag_init(flag_t *flag)
{
    *flag = FLAG_INITIALIZER;
}

void flag_signal(flag_t *flag)
{
    __sync_add_and_fetch(&flag->flag, 1);
}
      
void flag_wait(flag_t *flag)
{
    unsigned n = flag->next + 1;
    while (flag->flag < n) {};
    __sync_add_and_fetch(&flag->next, 1);
}

int flag_trywait(flag_t *flag)
{
    unsigned n = flag->next + 1;
    if (flag->flag < n) {
        return 0;
    } else {
        __sync_add_and_fetch(&flag->next, 1);
        return 1;
    }
}
      

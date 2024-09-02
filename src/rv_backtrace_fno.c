/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-21     WangShun     the first version
 * 2024-08-30     WangShun     add addr2line function
 */

#include "../include/rvbacktrace.h"

rt_uint32_t _rt_susrstack;
rt_uint32_t _rt_eusrstack;
rt_thread_t _backtrace_thread;
unsigned int rvstack_frame[STACK_FRAME_LEN]; // stack frame
unsigned int rvstack_frame_len; // stack frame len

static void walk_stackframe(void)
{
    rt_uint32_t num = 0;
    _backtrace_thread = rt_thread_self(); //    get current thread
    _rt_susrstack = (rt_uint32_t)(uintptr_t)_backtrace_thread->stack_addr; // stack start address
    _rt_eusrstack = (rt_uint32_t)(uintptr_t)(_backtrace_thread->stack_addr + _backtrace_thread->stack_size); // stack end address

    unsigned long sp, fp, ra, pc; // stack pointer, frame pointer, return address, program counter
    struct stackframe *frame;

    const register unsigned long current_sp __asm__("sp"); //   get current stack pointer
    sp = current_sp;
    fp = (unsigned long)__builtin_frame_address(0); //  get current frame pointer
    while (1)
    {
        frame = (struct stackframe *)(fp - BACKTRACE_LEN); //   get frame pointer

        if ((rt_uint32_t *)frame > (rt_uint32_t *)(uintptr_t)_rt_eusrstack)
        {
            rvstack_frame_len = num;
            return;
        }

        sp = fp;  // get stack pointer
        fp = frame->s_fp; // get frame pointer
        ra = frame->s_ra; // get return address
        pc = frame->s_ra - 4; // get program counter

        //  print stack interval, return address, program counter
        rt_kprintf("[%d]Stack interval :[0x%016lx - 0x%016lx]  ra 0x%016lx pc 0x%016lx\n", num, sp, fp, ra, pc);
        rvstack_frame[num] = pc; // save stack frame address
        num++;
    }
}

//  backtrace function
void rv_backtrace_fno(void)
{
    rt_kprintf("\r\n---- RV_Backtrace Call Frame Start: ----\r\n");
    rt_kprintf("###Please consider the value of ra as accurate and the value of sp as only for reference###\n");
    walk_stackframe();
    rt_kprintf("---- RV_Backtrace Call Frame End:----\r\n");
    rvbacktrace_addr2line((rt_uint32_t *)&rvstack_frame[0]); // addr2line function
    rt_kprintf("\r\n");
}

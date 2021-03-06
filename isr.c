/*
 * =====================================================================================
 *
 *       Filename:  isr.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05.09.2011 13:05:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Georg Wassen (gw) (wassen@lfbs.rwth-aachen.de), 
 *        Company:  Lehrstuhl für Betriebssysteme (Chair for Operating Systems)
 *                  RWTH Aachen University
 *
 * Copyright (c) 2011, Georg Wassen, RWTH Aachen University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the University nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * =====================================================================================
 */
#include "system.h"
#include "smp.h"

#define IFV   if (VERBOSE > 0 || VERBOSE_ISR > 0)
#define IFVV  if (VERBOSE > 1 || VERBOSE_ISR > 1)

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void isr47();
extern void isr128();
extern void isr160();
extern void isr161();
extern void isr162();
extern void isr163();
extern void isr164();
extern void isr165();
extern void isr166();
extern void isr167();
extern void isr168();
extern void isr169();


/*
 * GDT.Code is the Selector for the Code segment.
 * As "GDT.Code" is not a valid symbol in C (b/c of the dot),
 * an alternative symbol "GDT_CODE" is defined and exported in assembler.
 * That symbol is not a Pointer, but we're only interested in its integer value.
 * As C can only import symbols pointing to something, a pseudo integer is
 * declared and a Macro "GDT_Code_Sel" extracts the value (address) and
 * casts it to unsigned short (via unsigned long to avoid warnings).
 */
extern int GDT_Code;
#define GDT_Code_Sel ((unsigned short)(((unsigned long)&GDT_Code)&0xFFFF))

void isr_install()
{
    idt_set_gate(0,  (ptr_t)isr0,  GDT_Code_Sel, 0x8E);
    idt_set_gate(1,  (ptr_t)isr1,  GDT_Code_Sel, 0x8E);
    idt_set_gate(2,  (ptr_t)isr2,  GDT_Code_Sel, 0x8E);
    idt_set_gate(3,  (ptr_t)isr3,  GDT_Code_Sel, 0x8E);
    idt_set_gate(4,  (ptr_t)isr4,  GDT_Code_Sel, 0x8E);
    idt_set_gate(5,  (ptr_t)isr5,  GDT_Code_Sel, 0x8E);
    idt_set_gate(6,  (ptr_t)isr6,  GDT_Code_Sel, 0x8E);
    idt_set_gate(7,  (ptr_t)isr7,  GDT_Code_Sel, 0x8E);
    idt_set_gate(8,  (ptr_t)isr8,  GDT_Code_Sel, 0x8E);
    idt_set_gate(9,  (ptr_t)isr9,  GDT_Code_Sel, 0x8E);
    idt_set_gate(10, (ptr_t)isr10, GDT_Code_Sel, 0x8E);
    idt_set_gate(11, (ptr_t)isr11, GDT_Code_Sel, 0x8E);
    idt_set_gate(12, (ptr_t)isr12, GDT_Code_Sel, 0x8E);
    idt_set_gate(13, (ptr_t)isr13, GDT_Code_Sel, 0x8E);
    idt_set_gate(14, (ptr_t)isr14, GDT_Code_Sel, 0x8E);
    idt_set_gate(15, (ptr_t)isr15, GDT_Code_Sel, 0x8E);
    idt_set_gate(16, (ptr_t)isr16, GDT_Code_Sel, 0x8E);
    idt_set_gate(17, (ptr_t)isr17, GDT_Code_Sel, 0x8E);
    idt_set_gate(18, (ptr_t)isr18, GDT_Code_Sel, 0x8E);
    idt_set_gate(19, (ptr_t)isr19, GDT_Code_Sel, 0x8E);
    idt_set_gate(20, (ptr_t)isr20, GDT_Code_Sel, 0x8E);
    idt_set_gate(21, (ptr_t)isr21, GDT_Code_Sel, 0x8E);
    idt_set_gate(22, (ptr_t)isr22, GDT_Code_Sel, 0x8E);
    idt_set_gate(23, (ptr_t)isr23, GDT_Code_Sel, 0x8E);
    idt_set_gate(24, (ptr_t)isr24, GDT_Code_Sel, 0x8E);
    idt_set_gate(25, (ptr_t)isr25, GDT_Code_Sel, 0x8E);
    idt_set_gate(26, (ptr_t)isr26, GDT_Code_Sel, 0x8E);
    idt_set_gate(27, (ptr_t)isr27, GDT_Code_Sel, 0x8E);
    idt_set_gate(28, (ptr_t)isr28, GDT_Code_Sel, 0x8E);
    idt_set_gate(29, (ptr_t)isr29, GDT_Code_Sel, 0x8E);
    idt_set_gate(30, (ptr_t)isr30, GDT_Code_Sel, 0x8E);
    idt_set_gate(31, (ptr_t)isr31, GDT_Code_Sel, 0x8E);

    idt_set_gate(0x80, (ptr_t)isr128, GDT_Code_Sel, 0x8E);

    idt_set_gate(0xA0, (ptr_t)isr160, GDT_Code_Sel, 0xA0);
    idt_set_gate(0xA1, (ptr_t)isr161, GDT_Code_Sel, 0xA1);
    idt_set_gate(0xA2, (ptr_t)isr162, GDT_Code_Sel, 0xA2);
    idt_set_gate(0xA3, (ptr_t)isr163, GDT_Code_Sel, 0xA3);
    idt_set_gate(0xA4, (ptr_t)isr164, GDT_Code_Sel, 0xA4);
    idt_set_gate(0xA5, (ptr_t)isr165, GDT_Code_Sel, 0xA5);
    idt_set_gate(0xA6, (ptr_t)isr166, GDT_Code_Sel, 0xA6);
    idt_set_gate(0xA7, (ptr_t)isr167, GDT_Code_Sel, 0xA7);
    idt_set_gate(0xA8, (ptr_t)isr168, GDT_Code_Sel, 0xA8);
    idt_set_gate(0xA9, (ptr_t)isr169, GDT_Code_Sel, 0xA9);

}

char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

#define printf printf_nomutex
extern volatile unsigned cpu_online;
void int_handler(struct regs *r)
{
    unsigned bak = cpu_online;

    if (r->int_no < 32) {
        printf("|\n");
        printf("| CPU %u\n", my_cpu_info()->cpu_id);
        printf("| Exception: %s (%u)\n", exception_messages[r->int_no], r->int_no);
#       if __x86_64__
            printf("| ip: 0x%x, sp:0x%x\n", r->rip, r->rsp);
            //struct regs
            //{
            //    unsigned long long cr2, cr3, gs, fs, es, ds;				/* pushed the segs last */
            //    unsigned long long r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rbp, _zero, rbx, rdx, rcx, rax ;
            //    unsigned long long int_no, err_code;			/* our 'push byte #' and ecodes do this */
            //    unsigned long long rip, cs, rflags, rsp, ss;			/* pushed by the processor automatically */
            //} __attribute__((packed));
#       else
            printf("| ip: 0x%x, sp:0x%x\n", r->eip, r->esp);
            //struct regs
            //{
            //    unsigned int cr2, cr3, gs, fs, es, ds;
            //    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
            //    unsigned int int_no, err_code;
            //    unsigned int eip, cx, eflags, useresp, ss;
            //};
#       endif
        if (r->int_no == 14) {
            printf("| page-fault");
            if ((r->err_code & (1<<1)) == 1) printf("(rd):"); else printf("(wr):");
            if ((r->err_code & (1<<0)) == 0) printf("not-present "); else printf("protection ");
            if ((r->err_code & (1<<2)) == 0) printf("ring 0 "); else printf("ring 3 ");
            if ((r->err_code & (1<<3)) == 1) printf("reserved ");
            if ((r->err_code & (1<<3)) == 1) printf("instr.fetch ");
            printf("\n");
            printf("| address: 0x%x\n", r->cr2);
        }
        if (r->int_no != 15) {
            printf("| System halted.\n");
            while(1) {__asm__ volatile ("hlt"); };
        }
    } else {
        IFV printf("/----------------------------------------\n");
        IFV printf("| CPU %u\n", my_cpu_info()->cpu_id);
        IFV printf("| Interrupt: %u / 0x%x\n", r->int_no, r->int_no);
        IFV printf("\\----------------------------------------\n");
    }
    switch (r->int_no) {
        case 0x3F : break;
        case 0x80 : break;
    }
    /*
     * EOI
     */
    apic_eoi();
    cpu_online = bak;
}




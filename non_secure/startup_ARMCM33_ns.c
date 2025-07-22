/*
 * Non-secure C startup file for ARM Cortex-M33
 * Based on boot_ns.S
 */
#include <stdint.h>

/* Forward declaration of handlers */
void Reset_Handler(void);
void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((weak, alias("Default_Handler")));

void Interrupt0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Interrupt1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Interrupt2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Interrupt3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Interrupt4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Interrupt5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Interrupt6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Interrupt7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Interrupt8_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Interrupt9_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* Stack top defined by linker script */
extern void __StackTop(void);

/* Vector table */
__attribute__((section(".vectors")))
void (*const g_interrupt_vector[])(void)  = {
    &__StackTop,            /* Top of Stack */
    Reset_Handler,          /* Reset Handler */
    NMI_Handler,            /* NMI Handler */
    HardFault_Handler,      /* Hard Fault Handler */
    MemManage_Handler,      /* MPU Fault Handler */
    BusFault_Handler,       /* Bus Fault Handler */
    UsageFault_Handler,     /* Usage Fault Handler */
    0,                      /* Secure Fault Handler */
    0,                      /* Reserved */
    0,                      /* Reserved */
    0,                      /* Reserved */
    SVC_Handler,            /* SVCall Handler */
    DebugMon_Handler,       /* Debug Monitor Handler */
    0,                      /* Reserved */
    PendSV_Handler,         /* PendSV Handler */
    SysTick_Handler,        /* SysTick Handler */
    Interrupt0_Handler,     /* Interrupt 0 */
    Interrupt1_Handler,     /* Interrupt 1 */
    Interrupt2_Handler,     /* Interrupt 2 */
    Interrupt3_Handler,     /* Interrupt 3 */
    Interrupt4_Handler,     /* Interrupt 4 */
    Interrupt5_Handler,     /* Interrupt 5 */
    Interrupt6_Handler,     /* Interrupt 6 */
    Interrupt7_Handler,     /* Interrupt 7 */
    Interrupt8_Handler,     /* Interrupt 8 */
    Interrupt9_Handler      /* Interrupt 9 */
    /* ... space for more interrupts if needed */
};

/* Default handler */
void Default_Handler(void) {
    while (1) {}
}

/* Reset handler */
void Reset_Handler(void) {
    /* Optionally set up stack limit, etc. */
    /* Call main */
    extern int main(void);
    main();
    while (1) {}
}

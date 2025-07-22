/*
 * Secure C startup file for ARM Cortex-M33
 * Based on startup_ARMCM33.S
 */
#include <ARMCM33_DSP_FP_TZ.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*------------------- Memory Protection Controller
 * -----------------------------*/
typedef struct /* see "ARM CoreLink SSE-200 Subsystem Technical Reference Manual
                  r1p0" */
{
  __IOM uint32_t CTRL; /* Offset: 0x000 (R/W) Control Register */
  uint32_t RESERVED0[3];
  __IM uint32_t BLK_MAX;  /* Offset: 0x010 (R/ ) Block Maximum Register */
  __IM uint32_t BLK_CFG;  /* Offset: 0x014 (R/ ) Block Configuration Register */
  __IOM uint32_t BLK_IDX; /* Offset: 0x018 (R/W) Block Index Register */
  __IOM uint32_t BLK_LUT; /* Offset: 0x01C (R/W) Block Lookup Tabe Register */
  __IM uint32_t INT_STAT; /* Offset: 0x020 (R/ ) Interrupt Status Register */
  __OM uint32_t INT_CLEAR; /* Offset: 0x024 ( /W) Interrupt Clear Register */
  __IOM uint32_t INT_EN;   /* Offset: 0x028 (R/W) Interrupt Enable Register */
  __IM uint32_t INT_INFO1; /* Offset: 0x02C (R/ ) Interrupt Info1 Register */
  __IM uint32_t INT_INFO2; /* Offset: 0x030 (R/ ) Interrupt Info2 Register */
  __OM uint32_t INT_SET;   /* Offset: 0x034 ( /W) Interrupt Set Register */
} MPC_TypeDef;

#define MPC_SRAM1 ((MPC_TypeDef *)0x58007000) /* Control for 0x[01]0000000 */
#define MPC_SRAM2 ((MPC_TypeDef *)0x58008000) /* Control for 0x[01]0000000 */
#define MPC_SRAM3 ((MPC_TypeDef *)0x58009000) /* Control for 0x[01]0000000 */
// https://developer.arm.com/documentation/101104/0200/programmers-model/base-element/secure-privilege-control-block

extern void initialise_monitor_handles(void);
extern int main(void);

void mpc_init(MPC_TypeDef *mem, size_t start, size_t end);

/* Forward declaration of handlers */
void Reset_Handler(void);
void SecureFault_Handler(void);
void HardFault_Handler(void);

void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

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
extern void __StackTop();
extern uint32_t __StackLimit;

/* Vector table */
__attribute__((section(".vectors"))) void (*const g_interrupt_vector_s[])(
    void) = {
    &__StackTop,         /* Top of Stack */
    Reset_Handler,       /* Reset Handler */
    NMI_Handler,         /* NMI Handler */
    HardFault_Handler,   /* Hard Fault Handler */
    MemManage_Handler,   /* MPU Fault Handler */
    BusFault_Handler,    /* Bus Fault Handler */
    UsageFault_Handler,  /* Usage Fault Handler */
    SecureFault_Handler, /* Secure Fault Handler */
    0,                   /* Reserved */
    0,                   /* Reserved */
    0,                   /* Reserved */
    SVC_Handler,         /* SVCall Handler */
    DebugMon_Handler,    /* Debug Monitor Handler */
    0,                   /* Reserved */
    PendSV_Handler,      /* PendSV Handler */
    SysTick_Handler,     /* SysTick Handler */
    Interrupt0_Handler,  /* Interrupt 0 */
    Interrupt1_Handler,  /* Interrupt 1 */
    Interrupt2_Handler,  /* Interrupt 2 */
    Interrupt3_Handler,  /* Interrupt 3 */
    Interrupt4_Handler,  /* Interrupt 4 */
    Interrupt5_Handler,  /* Interrupt 5 */
    Interrupt6_Handler,  /* Interrupt 6 */
    Interrupt7_Handler,  /* Interrupt 7 */
    Interrupt8_Handler,  /* Interrupt 8 */
    Interrupt9_Handler   /* Interrupt 9 */
                         /* ... space for more interrupts if needed */
};

/* Default handler */
void Default_Handler(void) {
  while (1) {
  }
}

void SecureFault_Handler(void) {
  while (1) {
  }
}

void HardFault_Handler(void) {
    volatile uint32_t hfsr = *((volatile uint32_t *)0xE000ED2C); // HardFault Status Register
    volatile uint32_t cfsr = *((volatile uint32_t *)0xE000ED28); // Configurable Fault Status Register
    volatile uint32_t mmfar = *((volatile uint32_t *)0xE000ED34); // MemManage Fault Address Register
    volatile uint32_t bfar = *((volatile uint32_t *)0xE000ED38); // BusFault Address Register
    volatile uint32_t shcsr = *((volatile uint32_t *)0xE000ED24); // System Handler Control and State Register

    printf("HardFault detected!\n");
    printf("HFSR:  0x%08lx\n", hfsr);
    printf("CFSR:  0x%08lx\n", cfsr);
    printf("MMFAR: 0x%08lx\n", mmfar);
    printf("BFAR:  0x%08lx\n", bfar);
    printf("SHCSR: 0x%08lx\n", shcsr);

    if (hfsr & (1 << 30)) {
        printf(" - Forced HardFault\n");
    }
    if (cfsr & 0xFF) {
        printf(" - MemManage Fault\n");
    }
    if ((cfsr >> 8) & 0xFF) {
        printf(" - BusFault\n");
    }
    if ((cfsr >> 16) & 0xFFFF) {
        printf(" - UsageFault\n");
    }
    if (shcsr & (1 << 16)) {
        printf(" - MemManage Fault enabled\n");
    }
    if (shcsr & (1 << 17)) {
        printf(" - BusFault enabled\n");
    }
    if (shcsr & (1 << 18)) {
        printf(" - UsageFault enabled\n");
    }
    // Decode BFAR (BusFault Address Register)
    if ((cfsr >> 15) & 0x1) {
        printf(" - BFAR valid: BusFault occurred at address 0x%08lx\n", bfar);
    } else {
        printf(" - BFAR not valid (no precise BusFault address)\n");
    }

    // Dump registers from the stack frame
    uint32_t *stacked_regs = (uint32_t *)__builtin_frame_address(0);
    printf("Register dump at HardFault:\n");
    printf(" R0  = 0x%08lx\n", stacked_regs[0]);
    printf(" R1  = 0x%08lx\n", stacked_regs[1]);
    printf(" R2  = 0x%08lx\n", stacked_regs[2]);
    printf(" R3  = 0x%08lx\n", stacked_regs[3]);
    printf(" R12 = 0x%08lx\n", stacked_regs[4]);
    printf(" LR  = 0x%08lx\n", stacked_regs[5]);
    printf(" PC  = 0x%08lx\n", stacked_regs[6]);
    printf(" xPSR= 0x%08lx\n", stacked_regs[7]);

    printf("---- Program End - HARD FAULT ----\n\n");
    exit(-1);
}

/* Reset handler */
void Reset_Handler(void) {
/* Set up stack limit (MSPLIM) if needed */
#ifdef __ARM_FEATURE_CMSE
  __asm volatile("ldr r0, =__StackLimit\n\tmsr msplim, r0" : : : "r0");
#endif

  /* Initialize .data section */
  extern uint32_t __data_start__;
  extern uint32_t __data_end__;
  extern uint32_t __etext;
  uint32_t *src = &__etext;
  uint32_t *dst = &__data_start__;
  while (dst < &__data_end__) {
    *dst++ = *src++;
  }

  /* Zero .bss section */
  extern uint32_t __bss_start__;
  extern uint32_t __bss_end__;
  dst = &__bss_start__;
  while (dst < &__bss_end__) {
    *dst++ = 0;
  }

  SystemInit();

  initialise_monitor_handles();
  /* code memory, first 2MB secure, second 2MB non-secure */
  mpc_init(MPC_SRAM1, 0x00200000, 0x00400000);
  /* secure data memory, whole block as secure  */
  mpc_init(MPC_SRAM2, 0x00000000, 0x00000000);
  /* non secure data memory, whole block as non-secure */
  mpc_init(MPC_SRAM3, 0x00000000, 0x00200000);

  main();

  exit(0);
}

void mpc_init(MPC_TypeDef *mem, size_t start, size_t end) {
  uint32_t blockMax = mem->BLK_MAX;
  uint32_t blockSize = mem->BLK_CFG & 0xF;
  uint32_t blockSizeByte = 1 << (blockSize + 5);

  printf("MPC Config: block max=%lx, block size=0x%lx (%lu bytes)\n", blockMax, blockSize, blockSizeByte);

  // Calculate block indices for start and end
  uint32_t block_start_idx = start / blockSizeByte;
  uint32_t block_end_idx = (end + blockSizeByte - 1) / blockSizeByte; // inclusive

  mem->CTRL |= (1 << 4); // Enable MPC
  mem->CTRL &= ~(1 << 8); // No autoincrement

  // Set each block's security attribute
  for (uint32_t group = 0; group <= blockMax; group++) {
    uint32_t lut = 0x0;
    for (uint32_t bit = 0; bit < 32; bit++) {
      uint32_t block_idx = group * 32 + bit;
      if (block_idx >= block_start_idx && block_idx < block_end_idx) {
        lut |= (1U << bit); // non-secure
      }
    }
    mem->BLK_IDX = group;
    mem->BLK_LUT = lut;
  }

  // Print block config
  for (uint32_t group = 0; group <= blockMax; group++) {
    mem->BLK_IDX = group;
    printf("BLK_LUT[%02lx]: 0x%08lx\n", group, mem->BLK_LUT);
  }

  mem->CTRL &= ~(1 << 8); // No autoincrement
}

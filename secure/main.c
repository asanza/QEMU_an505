#include "uart.h"
#include "ARMCM33_DSP_FP_TZ.h"
#include "partition_ARMCM33.h"

#include <arm_cmse.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void printCmseAddressInfo(uint32_t addr)
{
    cmse_address_info_t c = cmse_TTA((void*) addr);
    printf("Address: 0x%lx\n", addr);
    printf(" |mpu_region: %x\n", c.flags.mpu_region);
    printf(" |sau_region: %x\n", c.flags.sau_region);
    printf(" |mpu_region_valid: %x\n", c.flags.mpu_region_valid);
    printf(" |sau_region_valid: %x\n", c.flags.sau_region_valid);
    printf(" |read_ok: %x\n", c.flags.read_ok);
    printf(" |readwrite_ok: %x\n", c.flags.readwrite_ok);
    printf(" |nonsecure_read_ok: %x\n", c.flags.nonsecure_read_ok);
    printf(" |nonsecure_readwrite_ok: %x\n", c.flags.nonsecure_readwrite_ok);
    printf(" |secure: %x\n", c.flags.secure);
    printf(" |idau_region_valid: %x\n", c.flags.idau_region_valid);
    printf(" |idau_region: %x\n", c.flags.idau_region);
}

void __aeabi_unwind_cpp_pr0()
{
}

/* Non-Secure Callable functions */
typedef int __attribute__((cmse_nonsecure_call)) ns_func_void(void);

int __attribute__((cmse_nonsecure_entry)) sec_sum(int idx, int m)
{
    printf("On the secure world %d + %d\n", idx, m);
    return idx + m;
}

void nonsecure_init(void) {
  /* Set the address of the Vector Table of the Non-Secure */
  uint32_t *vtor = (uint32_t *) TZ_VTOR_TABLE_ADDR;
  SCB_NS->VTOR = (uint32_t) TZ_VTOR_TABLE_ADDR;

  /* 
   * Update the Non-Secure Stack Pointer
   * (first element of the Vector Table)
   */
  __TZ_set_MSP_NS(*vtor);
  __TZ_set_PRIMASK_NS(0x0);
  __TZ_set_MSPLIM_NS(*vtor - 0x10000);

  /*
   * Jump to the reset handler (Second element of the Vector Table)
   * of the Non-Secure
   */
    ns_func_void *ns_reset = (ns_func_void*) (*(vtor + 1));
    printf("--- Non Secure VTOR Info ----\n");
    printCmseAddressInfo((uint32_t) ns_reset);
    printCmseAddressInfo((uint32_t) *vtor - 4);
    printf("---- Calling Non Secure App ----\n");
    ns_reset();
}

/* Secure main */
int main(void)
{
    uart_init();
    printString("Start\n");
 
    volatile uint32_t* spcb = (uint32_t*) (0x50080000 + 0x14);
    *spcb |= 1 | 2;
    printf("SPCB content 0x%lx\n", *spcb);
    /* 
    
    The Non-secure Callable Configuration register allows software to define
     callable regions of memory. The register can do this if the Secure Code
     region is 0x1000_0000 to 0x1FFF_FFFF and the Secure RAM region is
      0x3000_0000 to 0x3FFF_FFFF.
    */
    *spcb |= 1 | 2;
    printf("SPCB content 0x%lx\n", *spcb);

    printCmseAddressInfo(SAU_INIT_START1);
    printCmseAddressInfo(SAU_INIT_END1);
    printCmseAddressInfo(SAU_INIT_START2);
    printCmseAddressInfo(SAU_INIT_END2);
    printCmseAddressInfo(SAU_INIT_START3);
    printCmseAddressInfo(SAU_INIT_END3);
    printCmseAddressInfo(SAU_INIT_START4);
    printCmseAddressInfo(SAU_INIT_END4);
  
    // Print the whole SAU configuration
    printf("\n--- SAU Configuration ---\n");
    char buf[100];
    printf("SAU_CTRL   : 0x%lx\n", SAU->CTRL);
    printf("SAU_TYPE   : 0x%lx\n", SAU->TYPE);
    for (int i = 0; i < 8; ++i) {
        SAU->RNR = i;
        sprintf(buf, "SAU_RBAR[%d] : 0x%lx\n", i, SAU->RBAR);
        printString(buf);
        sprintf(buf, "SAU_RLAR[%d] : 0x%lx\n", i, SAU->RLAR);
        printString(buf);
    }
    printf("--- End SAU Configuration ---\n\n");

    /* Jump to Non-Secure main address */
    nonsecure_init();

    while (1)
    {
      __NOP();
      printf("Should not happen..\n");
    }
    return -1;
}

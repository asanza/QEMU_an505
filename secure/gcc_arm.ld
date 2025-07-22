__STACK_SIZE = 0x00100000;

MEMORY
{
 /* Flash NS and S share the same 4MB of memory, they are aliased. */
   FLASH_NS    (rx)  : ORIGIN = 0x00200000, LENGTH = 2M /* idau non secure*/
   FLASH_S     (rx)  : ORIGIN = 0x10000000, LENGTH = 2M /* idau secure */
   RAM_NS      (rwx) : ORIGIN = 0x28200000, LENGTH = 2M /* idau nonsecure */
   RAM_S       (rwx) : ORIGIN = 0x38000000, LENGTH = 2M /* idau secure */
   RAM1_NS     (rwx) : ORIGIN = 0x20000000, LENGTH = 32K /* idau nonsecure*/
   PSRAM       (rwx) : ORIGIN = 0x80000000, LENGTH = 16M /* idau nonsecure */
}

/* Entry Point (reset) */
ENTRY(Reset_Handler)

SECTIONS
{
  .text : 
  {
    PROVIDE(__Vectors = .);
    /* Make sure the vector is the first point of the project. */
    KEEP(*(.vectors))
   
    . += 1024;
   
    *(.text)

    KEEP(*(.init))
    KEEP(*(.fini))

    /* .ctors */
    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)

    /* .dtors */
    *crtbegin.o(.dtors)
    *crtbegin?.o(.dtors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
    *(SORT(.dtors.*))
    *(.dtors)

    *(.rodata*)

    KEEP(*(.eh_frame*))
  } > FLASH_S

  .ARM.extab :
  {
      *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH_S

  __exidx_start = .;
  .ARM.exidx :
  {
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > FLASH_S
  __exidx_end = .;

    /*
   * SG veneers:
   * All SG veneers are placed in the special output section .gnu.sgstubs.
   * Its start address must be set, either with the command line option 
   * '--section-start' or in a linker script, to indicate where to place these
   * veneers in memory.
   */
  .gnu.sgstubs :
  {
    _ld_veneer_base = .;
    *(.gnu.sgstubs*)
    . = ALIGN(32);
    _ld_veneer_limit = .;
  } > FLASH_S


  __etext = .;

  .data : AT (__etext)
  {
      __data_start__ = .;
      PROVIDE(_sdata = __data_start__);
      *(vtable)
      *(.data*)

      . = ALIGN(4);
      /* preinit data */
      PROVIDE_HIDDEN (__preinit_array_start = .);
      KEEP(*(.preinit_array))
      PROVIDE_HIDDEN (__preinit_array_end = .);

      . = ALIGN(4);
      /* init data */
      PROVIDE_HIDDEN (__init_array_start = .);
      KEEP(*(SORT(.init_array.*)))
      KEEP(*(.init_array))
      PROVIDE_HIDDEN (__init_array_end = .);


      . = ALIGN(4);
      /* finit data */
      PROVIDE_HIDDEN (__fini_array_start = .);
      KEEP(*(SORT(.fini_array.*)))
      KEEP(*(.fini_array))
      PROVIDE_HIDDEN (__fini_array_end = .);

      KEEP(*(.jcr*))
      . = ALIGN(4);
      /* All data end */
      __data_end__ = .;
      PROVIDE(_edata = __data_end__);
  } > RAM_S

  .bss :
  {
    . = ALIGN(4);
    __bss_start__ = .;
    PROVIDE(_sbss = __bss_start__);
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    PROVIDE(_ebss = __bss_end__);
  } > RAM_S

  /* Set __StackTop for the vector table first index. */
  __StackTop = ORIGIN(RAM_S) + LENGTH(RAM_S);
  /*
   * Stack is descending, meaning that putting values in the stack will decrease
   * so the limit of the stack is less than the base size address.
   */
  __StackLimit = __StackTop - __STACK_SIZE;

  /* Put some space between stack and heap */
  . = ALIGN(8);
  . = . + 0x2000;

  _end = .;
  PROVIDE(end = .);
}

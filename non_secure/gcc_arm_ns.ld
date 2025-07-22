__STACK_SIZE = 0x00001000;

MEMORY
{
   FLASH_NS     (rx)  : ORIGIN = 0x00200000, LENGTH = 2M
   RAM_NS       (rwx) : ORIGIN = 0x28200000, LENGTH = 2M
}

/* Entry Point (reset) */
ENTRY(Reset_Handler)

SECTIONS
{
  .text : 
  {
    . = ALIGN(8);
    PROVIDE(__Vectors = .);
    /* Make sure the vector is the first point of the project. */
    KEEP(*(.vectors))
    *(.text)
  } > FLASH_NS

  .ARM.exidx :
  {
    __exidx_start = .;
     *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
  } > FLASH_NS
  /* define a global symbol _etext just after the last code byte */
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
  } > RAM_NS

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
  } > RAM_NS

  .heap : {
    __heap_start__ = .;
    end = __heap_start__;
    _end = end;
    __end = end;
    KEEP(*(.heap))
    __heap_end__ = .;
    __HeapLimit = __heap_end__;
  } > RAM_NS



  /* Set __StackTop for the vector table first index. */
  __StackTop = ORIGIN(RAM_NS) + LENGTH(RAM_NS);
  /*
   * Stack is descending, meaning that putting values in the stack will decrease
   * so the limit of the stack is less than the base size address.
   */
  __StackLimit = __StackTop - __STACK_SIZE;
}

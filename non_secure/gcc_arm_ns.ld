__STACK_SIZE = 0x00100000;

MEMORY
{
   FLASH_NS     (rx)  : ORIGIN = 0x00200000, LENGTH = 512K
   FLASH_NS_END (rx)  : ORIGIN = 0x00260000, LENGTH = 512K
   RAM_NS       (rwx) : ORIGIN = 0x28200000, LENGTH = 32K
}

/* Entry Point (reset) */
ENTRY(Reset_Handler)

SECTIONS
{
  .text : 
  {
    PROVIDE(__VECTORS = .);
    /* Make sure the vector is the first point of the project. */
    KEEP(*(.vectors))
    *(.text)
    *(.data)
  	*(.bss) 
  } > FLASH_NS

  .ARM.exidx :
  {
    __exidx_start = .;
     *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
  } > FLASH_NS
  /* define a global symbol _etext just after the last code byte */
  _etext = .;

  .heap : {
    __heap_start__ = .;
    end = __heap_start__;
    _end = end;
    __end = end;
    KEEP(*(.heap))
    __heap_end__ = .;
    __HeapLimit = __heap_end__;
  } > FLASH_NS



  /* Set __StackTop for the vector table first index. */
  __StackTop = ORIGIN(FLASH_NS_END) + LENGTH(FLASH_NS_END);
  /*
   * Stack is descending, meaning that putting values in the stack will decrease
   * so the limit of the stack is less than the base size address.
   */
  __StackLimit = __StackTop - __STACK_SIZE;
}

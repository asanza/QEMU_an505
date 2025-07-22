__STACK_SIZE = 0x00100000;

MEMORY
{
 /* Flash is actually 224M */
   FLASH_S     (rx)  : ORIGIN = 0x10000000, LENGTH = 512K
   FLASH_S_END (rx)  : ORIGIN = 0x10020000, LENGTH = 32K
   RAM_S       (rwx) : ORIGIN = 0x38000000, LENGTH = 64K
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
    *(.text)
  } > FLASH_S

  .data :
  {
    *(.data)
  } > RAM_S AT > FLASH_S

  .bss :
  {
    *(.bss)
  } > RAM_S

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

  /* Set __StackTop for the vector table first index. */
  __StackTop = ORIGIN(FLASH_S_END) + LENGTH(FLASH_S_END);
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

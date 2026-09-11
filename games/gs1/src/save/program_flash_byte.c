#include "scene.h"
#include "abi/save/program_flash_byte.h"
/*
 * Program one byte of flash: issue the unlock sequence, store the byte,
 * then call through the word at 0x02004C00.
 */
#include "flash.h"

/*
 * Sys_SetMode names a `bx rN` slot: the call is indirect through the
 * register that slot selects, and the trailing argument is the callee
 * address rather than a parameter of the callee.
 */

/*
 * The word at 0x02004C00 is the address called, not data. The source byte
 * is read again for the call rather than reused from the store.
 */
u16 ProgramFlashByte(u8 *source, u8 *destination)
{
  s32 *status;
  *(volatile u8 *) 0x0E005555 = 0xAA;
  *(volatile u8 *) 0x0E002AAA = 0x55;
  *(volatile u8 *) 0x0E005555 = 0xA0;
  *destination = *source;
  status = (s32 *) 0x02004C00;
  return Sys_SetMode(1, destination, *source, *status);
}

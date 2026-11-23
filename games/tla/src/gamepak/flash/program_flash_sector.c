/// @file
/// Programming a sector of the Macronix and Panasonic chips.

#include "gamepak/flash.h"
#include "gamepak/flash_data.h"
#include "system/io_reg.h"

/// Erases 4 KB sector `sector` and programs it from source a byte at a time,
/// stopping at the first byte that fails.
u16 ProgramFlashSector(u16 sector, u8 *source) {
  u8 savedCode[64];
  u16 result;
  u8 *destination;

  if ((u32)sector > FLASH_LAST_SECTOR)
    return FLASH_ERROR_NO_SECTOR;

  result = EraseFlashSector(sector);
  if (result != 0)
    return result;

  // The byte reader WaitForFlashWrite polls with lives in savedCode until this
  // function returns.
  CopyFlashReadRoutineToRam(savedCode);
  REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | gFlash->wait[0];
  gFlashNumRemainingBytes = gFlash->sector.size;
  destination = (u8 *)((sector << gFlash->sector.shift) + FLASH_BASE);

  while (gFlashNumRemainingBytes != 0) {
    result = ProgramFlashByte(source, destination);
    if (result != 0)
      break;
    gFlashNumRemainingBytes -= 1;
    source++;
    destination++;
  }

  REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;

  return result;
}

/// @file
/// Programming a sector of the Macronix and Panasonic chips.

#include "../../../include/gamepak/flash.h"
#include "../../../include/gamepak/flash_data.h"
#include "../../../include/system/io_reg.h"

/// Erases a standard sector and programs bytes until completion or an error.
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
  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | gFlash->wait[0];
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

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | WAITCNT_SRAM_8;

  return result;
}

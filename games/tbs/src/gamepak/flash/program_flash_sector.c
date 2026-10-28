#include "../../../include/gamepak/flash.h"
#include "../../../include/gamepak/flash_data.h"
#include "../../../include/system/io_reg.h"

extern u16 gFlashNumRemainingBytes;

/// Erases a standard sector and programs bytes until completion or an error.
/// The program routine (gProgramFlashSector) of the Macronix and Panasonic
/// chips, which Save_WriteSector calls with 4 KB at `source`. Returns 0, 0x80ff
/// for a sector above 15, the erase's error, or 0xc001 for the first byte that
/// did not read back in time; gFlashNumRemainingBytes is left at the bytes not
/// written.
u16 ProgramFlashSector(u16 sector, u8 *source) {
  u8 savedCode[64];
  u16 result;
  u8 *destination;

  if ((u32)sector > 15)
    return 0x80FF;

  result = EraseFlashSector(sector);
  if (result != 0)
    return result;

  // The byte reader WaitForFlashWrite polls with lives in savedCode until
  // this function returns. The SRAM wait (REG_WAITCNT bits 1..0) is the
  // chip's own while programming, 3 after.
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

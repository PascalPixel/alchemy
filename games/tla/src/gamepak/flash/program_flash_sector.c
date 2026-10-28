#include "../../../include/gamepak/flash.h"
#include "../../../include/gamepak/flash_data.h"
#include "../../../include/system/io_reg.h"

extern u16 gFlashNumRemainingBytes;

/// Erases a standard sector and programs bytes until completion or an error.
u16 ProgramFlashSector(u16 sector, u8 *source) {
  u8 savedCode[64];
  u16 result;
  u8 *destination;

  if ((u32)sector > 15)
    return 0x80FF;

  result = EraseFlashSector(sector);
  if (result != 0)
    return result;

  CopyFlashReadRoutineToRam(savedCode);
  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | gFlash->wait[0];
  gFlashNumRemainingBytes = gFlash->sector.size;
  destination = (u8 *)((sector << gFlash->sector.shift) + 0x0E000000);

  while (gFlashNumRemainingBytes != 0) {
    result = ProgramFlashByte(source, destination);
    if (result != 0)
      break;
    gFlashNumRemainingBytes -= 1;
    source++;
    destination++;
  }

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | 3;

  return result;
}

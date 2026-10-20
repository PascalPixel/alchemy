#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"
#include "../../../include/system/io_reg.h"

extern FlashWaitProc gFlash_Handler3;

/// Erases the Atmel chip and waits for completion with its configured timing.
/// Returns 0, or 0xc003 when byte 0 has not read 0xff within the chip-erase
/// time. Atmel's erase-chip routine (gFlash_Handler1); nothing in ☀️ calls
/// through that cell.
u16 EraseAtmelFlashChip(void) {
  u8 savedCode[64];
  u16 result;
  volatile u8 *command;
  volatile u8 *unlock;

  CopyFlashReadRoutineToRam(savedCode);

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | gFlash_ChipAtmelLayout.type.wait[0];

  command = (volatile u8 *)0x0E005555;
  *command = 0xAA;
  unlock = (volatile u8 *)0x0E002AAA;
  *unlock = 0x55;
  *command = 0x80;
  *command = 0xAA;
  *unlock = 0x55;
  *command = 0x10;

  result = gFlash_Handler3(3, (u8 *)0x0E000000, 255);

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | 3;

  return result;
}

/// Erases one small Atmel sector by programming every byte to 0xff: one of
/// gFlash_ChipAtmelLayout's 512 sectors of 128 bytes, written with interrupts
/// off. Returns 0, or the wait's error with its phase made 2, 0xc002, so that
/// it reads as an erase error.
u16 EraseAtmelFlashSector(u16 sector) {
  struct FlashSetupInfo *info = &gFlash_ChipAtmelLayout;
  u8 *cursor;
  u16 savedIme;
  s32 bytesRemaining;
  u16 result;

  cursor = (u8 *)(0x0E000000 + (sector << info->type.sector.shift));
  savedIme = REG_IME;
  REG_IME = 0;
  *(volatile u8 *)0x0E005555 = 0xAA;
  *(volatile u8 *)0x0E002AAA = 0x55;
  *(volatile u8 *)0x0E005555 = 0xA0;
  bytesRemaining = info->type.sector.size;
  while (bytesRemaining != 0) {
    *cursor = 0xFF;
    cursor++;
    bytesRemaining--;
  }
  cursor--;
  REG_IME = savedIme;
  result = gFlash_Handler3(1, cursor, 0xFF);
  if (result != 0) {
    result = (result & 0xFF00) | 2;
  }
  return result;
}

/// Erases thirty-two small sectors, trying each sector at most twice: the
/// 4 KB of standard sector `sector`. Atmel's erase-sector routine
/// (gEraseFlashSector). Returns 0, 0x80ff for a sector above 15, or the error
/// of a small sector's second failed try, the sectors after it left as they
/// were.
u16 EraseAtmelFlashBlock(u16 sector) {
  // Both structured retry-loop trials add four bytes to the native EN body.
  u8 savedCode[64];
  u16 result;
  u16 smallSector;
  u16 attemptsRemaining;
  u16 sectorsErased;

  if ((u32)sector > 15)
    return 0x80ff;

  CopyFlashReadRoutineToRam(savedCode);
  REG_WAITCNT = (REG_WAITCNT & 0xfffc) | gFlash_ChipAtmelLayout.type.wait[0];

  smallSector = sector << 4;
  smallSector = (u16)(smallSector << 1);
  sectorsErased = 0;
nextSector:
  attemptsRemaining = 2;
  goto eraseAttempt;
retryErase:
  attemptsRemaining -= 1;
  if (attemptsRemaining == 0)
    goto advanceSector;
eraseAttempt:
  result = EraseAtmelFlashSector(smallSector);
  if (result != 0)
    goto retryErase;
advanceSector:
  smallSector += 1;
  if (result == 0) {
    sectorsErased += 1;
    if ((u32)sectorsErased <= 31)
      goto nextSector;
  }
  REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;
  return result;
}

/// Programs a small Atmel sector with interrupts disabled, then polls its last
/// byte. Writes 128 bytes from `source` after one program command, with no
/// erase first. Returns 0, or 0xc001 when the last byte does not read back
/// in time.
u16 ProgramAtmelFlashSector(u32 sector, const u8 *source) {
  const u8 *input;
  volatile u8 *destination;
  u32 savedIme;
  s32 remaining;

  input = source;
  sector = (u16)sector;
  destination =
      (volatile u8 *)(0x0e000000 +
                      (sector << gFlash_ChipAtmelLayout.type.sector.shift));
  savedIme = REG_IME;
  REG_IME = 0;
  *(volatile u8 *)0x0e005555 = 0xaa;
  *(volatile u8 *)0x0e002aaa = 0x55;
  *(volatile u8 *)0x0e005555 = 0xa0;

  remaining = gFlash_ChipAtmelLayout.type.sector.size;
  if (remaining != 0) {
    do {
      *destination++ = *input++;
      remaining--;
    } while (remaining != 0);
  }

  destination--;
  input--;
  REG_IME = savedIme;
  return gFlash_Handler3(1, destination, *input);
}

extern u16 gFlashNumRemainingBytes;

/// Programs an Atmel block in small sectors, trying each sector at most twice.
/// Writes standard sector `sector` (4 KB) from `source` 128 bytes at a time,
/// gFlashNumRemainingBytes counting what is left. Atmel's program routine
/// (gFlash_Handler0), which SaveState_WriteWorkspaceSlot calls. Returns 0,
/// 0x80ff for a sector above 15, or the error of the small sector that failed
/// twice.
u16 ProgramAtmelFlashBlock(u16 sector, u8 *source) {
  // The ordinary decrementing retry loop shortens the native EN body by 16
  // bytes.
  u8 savedCode[64];
  u16 result;
  u16 smallSector;
  u16 attemptsRemaining;

  if ((u32)sector > 15)
    return 0x80ff;

  CopyFlashReadRoutineToRam(savedCode);
  REG_WAITCNT = (REG_WAITCNT & 0xfffc) | gFlash_ChipAtmelLayout.type.wait[0];

  smallSector = sector << 5;
  gFlashNumRemainingBytes = gFlash_ChipAtmel.type.sector.size;

  while (gFlashNumRemainingBytes != 0) {
    attemptsRemaining = 2;
    goto programAttempt;
    do {
      attemptsRemaining -= 1;
      if (attemptsRemaining == 0)
        break;
    programAttempt:
      result = ProgramAtmelFlashSector(smallSector, source);
    } while (result != 0);

    if (result != 0)
      break;

    gFlashNumRemainingBytes -= gFlash_ChipAtmelLayout.type.sector.size;
    source += gFlash_ChipAtmelLayout.type.sector.size;
    smallSector += 1;
  }

  REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;
  return result;
}

/// @file
/// Erasing and programming the Atmel chip, whose small 128-byte sectors are
/// erased thirty-two to a standard sector.

#include "../../../include/gamepak/flash.h"
#include "../../../include/gamepak/flash_data.h"
#include "../../../include/system/io_reg.h"

/// Erases the Atmel chip and waits for completion with its configured timing.
/// Returns 0, or a chip-erase timeout when byte 0 has not read erased within
/// the chip-erase time. Atmel's erase-chip routine (gEraseFlashChip); nothing
/// in the game calls through that cell.
u16 EraseAtmelFlashChip(void) {
  u8 savedCode[64];
  u16 result;
  volatile u8 *command;
  volatile u8 *unlock;

  CopyFlashReadRoutineToRam(savedCode);

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | gFlash_ChipAtmelLayout.type.wait[0];

  command = (volatile u8 *)(FLASH_BASE + FLASH_COMMAND_ADDRESS);
  *command = FLASH_UNLOCK_1;
  unlock = (volatile u8 *)(FLASH_BASE + FLASH_UNLOCK_ADDRESS);
  *unlock = FLASH_UNLOCK_2;
  *command = FLASH_COMMAND_ERASE;
  *command = FLASH_UNLOCK_1;
  *unlock = FLASH_UNLOCK_2;
  *command = FLASH_COMMAND_ERASE_CHIP;

  result = gWaitForFlashWrite(FLASH_PHASE_ERASE_CHIP, (u8 *)FLASH_BASE,
                              FLASH_ERASED_BYTE);

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | WAITCNT_SRAM_8;

  return result;
}

/// Erases one small Atmel sector by programming every byte to
/// FLASH_ERASED_BYTE: one of gFlash_ChipAtmelLayout's 512 sectors of 128 bytes,
/// written with interrupts off. Returns 0, or the wait's error with its phase
/// made FLASH_PHASE_ERASE_SECTOR, so that it reads as an erase error.
u16 EraseAtmelFlashSector(u16 sector) {
  struct FlashSetupInfo *info = &gFlash_ChipAtmelLayout;
  u8 *cursor;
  s32 bytesRemaining;
  u16 result;

  cursor = (u8 *)(FLASH_BASE + (sector << info->type.sector.shift));
  {
    u16 savedIme = REG_IME;

    REG_IME = 0;
    FLASH_SEND_COMMAND(FLASH_COMMAND_PROGRAM);
    bytesRemaining = info->type.sector.size;
    while (bytesRemaining != 0) {
      *cursor = FLASH_ERASED_BYTE;
      cursor++;
      bytesRemaining--;
    }
    cursor--;
    REG_IME = savedIme;
    result = gWaitForFlashWrite(FLASH_PHASE_PROGRAM, cursor, FLASH_ERASED_BYTE);
    if (result != 0) {
      result = (result & 0xFF00) | FLASH_PHASE_ERASE_SECTOR;
    }
    return result;
  }
}

/// Erases thirty-two small sectors, trying each sector at most twice: the
/// 4 KB of standard sector `sector`. Atmel's erase-sector routine
/// (gEraseFlashSector). Returns 0, FLASH_ERROR_NO_SECTOR for a sector above 15,
/// or the error of a small sector's second failed try, the sectors after it
/// left as they were.
u16 EraseAtmelFlashBlock(u16 sector) {
  u8 savedCode[64];
  u16 result;
  u16 smallSector;
  u16 attemptsRemaining;
  u16 sectorsErased;

  if ((u32)sector > FLASH_LAST_SECTOR)
    return FLASH_ERROR_NO_SECTOR;

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
  REG_WAITCNT = (REG_WAITCNT & 0xfffc) | WAITCNT_SRAM_8;
  return result;
}

/// Programs a small Atmel sector with interrupts disabled, then polls its last
/// byte. Writes 128 bytes from `source` after one program command, with no
/// erase first. Returns 0, or a program timeout when the last byte does not
/// read back in time.
u16 ProgramAtmelFlashSector(u32 sector, const u8 *source) {
  const u8 *input;
  volatile u8 *destination;
  s32 i;

  input = source;
  sector = (u16)sector;
  destination =
      (volatile u8 *)(FLASH_BASE +
                      (sector << gFlash_ChipAtmelLayout.type.sector.shift));
  {
    u32 savedIme = REG_IME;

    REG_IME = 0;
    FLASH_SEND_COMMAND(FLASH_COMMAND_PROGRAM);

    for (i = 0; i < gFlash_ChipAtmelLayout.type.sector.size; i++) {
      *destination++ = *input++;
    }

    destination--;
    input--;
    REG_IME = savedIme;
    return gWaitForFlashWrite(FLASH_PHASE_PROGRAM, destination, *input);
  }
}

/// Programs an Atmel block in small sectors, trying each sector at most twice.
/// Writes standard sector `sector` (4 KB) from `source` 128 bytes at a time,
/// gFlashNumRemainingBytes counting what is left. Atmel's program routine
/// (gProgramFlashSector), which Save_WriteSector calls. Returns 0,
/// FLASH_ERROR_NO_SECTOR for a sector above 15, or the error of the small
/// sector that failed twice.
u16 ProgramAtmelFlashBlock(u16 sector, u8 *source) {
  u8 savedCode[64];
  u16 result;
  u16 smallSector;
  u16 attemptsRemaining;

  if ((u32)sector > FLASH_LAST_SECTOR)
    return FLASH_ERROR_NO_SECTOR;

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

  REG_WAITCNT = (REG_WAITCNT & 0xfffc) | WAITCNT_SRAM_8;
  return result;
}

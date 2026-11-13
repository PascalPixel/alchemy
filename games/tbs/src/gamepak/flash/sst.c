/// @file
/// Erasing and programming the SST chip, the routines also installed for an
/// unknown chip; every chip but Atmel erases through them.

#include "../../../include/gamepak/flash.h"
#include "../../../include/gamepak/flash_data.h"
#include "../../../include/system/io_reg.h"

u16 _call_via_r3(s32 mode, u8 *address, u8 expected,
                 FlashWaitProc waitForWrite);

/// Issues the chip-erase command and waits for an erased byte. Returns 0, or
/// a chip-erase timeout when byte 0 has not read erased within the chip-erase
/// time. The erase-chip routine (gEraseFlashChip) of every chip but Atmel;
/// nothing in the game calls through that cell.
u16 EraseFlashChip(void) {
  u8 savedCode[64];
  u16 result;
  FlashWaitProc *waitForWrite;

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | gFlash->wait[0];

  FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE);
  FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE_CHIP);

  CopyFlashReadRoutineToRam(savedCode);

  waitForWrite = &gWaitForFlashWrite;
  result = _call_via_r3(FLASH_PHASE_ERASE_CHIP, (u8 *)FLASH_BASE,
                        FLASH_ERASED_BYTE, *waitForWrite);

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | WAITCNT_SRAM_8;

  return result;
}

/// Erases one of sixteen sectors, restoring the cartridge wait setting
/// afterward: the chip's own SRAM wait while erasing, 3 after. Returns 0,
/// FLASH_ERROR_NO_SECTOR for a sector above 15, or a sector-erase timeout when
/// the sector's first byte has not read erased within the sector-erase time.
/// The erase-sector routine (gEraseFlashSector) of every chip but Atmel, and
/// the first step of ProgramFlashSector and ProgramFlashSectorWithEraseRetry.
u16 EraseFlashSector(u16 sector) {
  u8 savedCode[64];
  u16 result;
  u16 setting;
  FlashWaitProc *waitForWrite;
  const struct FlashType *chip;
  volatile u16 *waitcnt;
  volatile u8 *command;
  volatile u8 *unlock;
  volatile u8 *target;

  if (sector > FLASH_LAST_SECTOR) {
    return FLASH_ERROR_NO_SECTOR;
  }

  waitcnt = &REG_WAITCNT;
  setting = *waitcnt & 0xFFFC;
  chip = gFlash;
  *waitcnt = setting | chip->wait[0];

  target = (volatile u8 *)((sector << chip->sector.shift) + FLASH_BASE);

  command = (volatile u8 *)(FLASH_BASE + FLASH_COMMAND_ADDRESS);
  *command = FLASH_UNLOCK_1;
  unlock = (volatile u8 *)(FLASH_BASE + FLASH_UNLOCK_ADDRESS);
  *unlock = FLASH_UNLOCK_2;
  *command = FLASH_COMMAND_ERASE;
  *command = FLASH_UNLOCK_1;
  *unlock = FLASH_UNLOCK_2;
  *target = FLASH_COMMAND_ERASE_SECTOR;

  CopyFlashReadRoutineToRam(savedCode);

  waitForWrite = &gWaitForFlashWrite;
  result = _call_via_r3(FLASH_PHASE_ERASE_SECTOR, (u8 *)target,
                        FLASH_ERASED_BYTE, *waitForWrite);

  *waitcnt = (*waitcnt & 0xFFFC) | WAITCNT_SRAM_8;

  return result;
}

/// Issues a byte-program command and waits for its destination to match.
/// Returns 0, or a program timeout when the byte does not read back in time.
/// The caller sets the wait state and the RAM byte reader first.
u16 ProgramFlashByte(u8 *source, u8 *destination) {
  FlashWaitProc *waitForWrite;

  FLASH_SEND_COMMAND(FLASH_COMMAND_PROGRAM);
  *destination = *source;
  waitForWrite = &gWaitForFlashWrite;
  return (*waitForWrite)(FLASH_PHASE_PROGRAM, destination, *source);
}

/// Erases and verifies a sector with retries, then programs its bytes. The
/// program routine (gProgramFlashSector) of the SST and unknown chips, which
/// Save_WriteSector calls with 4 KB at `source`. Returns 0,
/// FLASH_ERROR_NO_SECTOR for a sector above 15, the last erase or check error
/// after 81 failed tries (FLASH_ERROR_NOT_ERASED for a sector that would not
/// read blank), or a program timeout for the first byte that did not read back
/// in time.
u16 ProgramFlashSectorWithEraseRetry(u16 sector, u8 *source) {
  u8 ramCode[96];
  u16 status;
  u16 remaining;
  u16 *sourceCode;
  u16 *destinationCode;
  u16 *counter;
  u8 attempt;
  u8 index;
  u8 rounds;
  const struct FlashType *chip;
  volatile u16 *waitcnt;
  u16 setting;
  u8 *destination;

  if (sector > FLASH_LAST_SECTOR) {
    return FLASH_ERROR_NO_SECTOR;
  }

  destination = (u8 *)((sector << gFlash->sector.shift) + FLASH_BASE);

  // Run the blank check from a copy in ramCode; the byte reader replaces it
  // there once the sector is blank.
  sourceCode = (u16 *)((s32)CountRemainingErasedFlashBytes ^ 1);
  destinationCode = (u16 *)ramCode;
  remaining =
      (u16)((s32)RunFlashEraseVerifier - (s32)CountRemainingErasedFlashBytes);
  while (remaining != 0) {
    *destinationCode = *sourceCode;
    sourceCode++;
    destinationCode++;
    remaining = remaining - 2;
  }

  // Retry until erase verification succeeds or all 81 attempts fail.
  attempt = 0;
  for (;;) {
    status = EraseFlashSector(sector);
    if (status == 0) {
      status = RunFlashEraseVerifier(destination, (s32 (*)(u8 *))(ramCode + 1));
      if (status == 0) {
        break;
      }
    }
    attempt = attempt + 1;
    if (attempt == 81) {
      return status;
    }
  }

  // A previously failed erase receives six more conditioning erases; one that
  // was blank at once receives one.
  // XXX (no evidence): why a sector that already reads blank is erased again;
  // the reason lies in the SST chip's behaviour, which neither the ROM nor a
  // render shows.
  rounds = 1;
  if (attempt != 0) {
    rounds = 6;
  }
  index = 1;
  while (index <= rounds) {
    EraseFlashSector(sector);
    index = index + 1;
  }

  CopyFlashReadRoutineToRam(ramCode);

  waitcnt = &REG_WAITCNT;
  setting = *waitcnt & 0xFFFC;
  chip = gFlash;
  *waitcnt = setting | chip->wait[0];

  // Program the verified sector, stopping at the first failed byte.
  gFlashNumRemainingBytes = chip->sector.size;
  counter = &gFlashNumRemainingBytes;
  while (*counter != 0) {
    status = ProgramFlashByte(source, destination);
    if (status != 0) {
      break;
    }
    *counter = *counter - 1;
    source++;
    destination++;
  }

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | WAITCNT_SRAM_8;

  return status;
}

/// Returns how many bytes remain when the first non-erased byte is found: 0
/// when the sector at cursor reads erased throughout. Runs from a RAM copy
/// (ProgramFlashSectorWithEraseRetry).
s32 CountRemainingErasedFlashBytes(u8 *cursor) {
  s32 remaining;

  for (remaining = gFlash->sector.size; remaining != 0; remaining--) {
    if (*cursor++ != FLASH_ERASED_BYTE)
      break;
  }
  return remaining;
}

/// Converts a nonzero erased-byte remainder into the flash verification error:
/// returns 0 when `verify` finds the sector blank, else FLASH_ERROR_NOT_ERASED.
s32 RunFlashEraseVerifier(u8 *sector, s32 (*verify)(u8 *)) {
  if (verify(sector) != 0)
    return FLASH_ERROR_NOT_ERASED;
  return 0;
}

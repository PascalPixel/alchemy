/// @file
/// Erasing and programming the SST chip, the routines also installed for an
/// unknown chip; every chip but Atmel erases through them.

#include "../../../include/gamepak/flash.h"
#include "../../../include/gamepak/flash_data.h"
#include "../../../include/system/io_reg.h"

/// Issues the chip-erase command and waits for an erased byte.
u16 EraseFlashChip(void) {
  u8 savedCode[64];
  u16 result;

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | gFlash->wait[0];

  FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE);
  FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE_CHIP);

  CopyFlashReadRoutineToRam(savedCode);

  result = gWaitForFlashWrite(FLASH_PHASE_ERASE_CHIP, (u8 *)FLASH_BASE,
                           FLASH_ERASED_BYTE);

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | WAITCNT_SRAM_8;

  return result;
}

/// Erases one of sixteen sectors, restoring the cartridge wait setting
/// afterward: the chip's own SRAM wait while erasing, 3 after.
u16 EraseFlashSector(u16 sector) {
  u8 savedCode[64];
  u16 result;
  volatile u8 *target;

  if (sector > FLASH_LAST_SECTOR) {
    return FLASH_ERROR_NO_SECTOR;
  }

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | gFlash->wait[0];

  target = (volatile u8 *)((sector << gFlash->sector.shift) + FLASH_BASE);

  FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE);
  FLASH_WRITE(FLASH_COMMAND_ADDRESS, FLASH_UNLOCK_1);
  FLASH_WRITE(FLASH_UNLOCK_ADDRESS, FLASH_UNLOCK_2);
  *target = FLASH_COMMAND_ERASE_SECTOR;

  CopyFlashReadRoutineToRam(savedCode);

  result = gWaitForFlashWrite(FLASH_PHASE_ERASE_SECTOR, (u8 *)target,
                           FLASH_ERASED_BYTE);

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | WAITCNT_SRAM_8;

  return result;
}

/// Issues a byte-program command and waits for its destination to match.
/// Returns 0, or a program timeout when the byte does not read back in time.
u16 ProgramFlashByte(u8 *source, u8 *destination) {

  FLASH_SEND_COMMAND(FLASH_COMMAND_PROGRAM);
  *destination = *source;
  return gWaitForFlashWrite(FLASH_PHASE_PROGRAM, destination, *source);
}

/// Erases and verifies a sector with retries, then programs its bytes.
u16 ProgramFlashSectorWithEraseRetry(u16 sector, u8 *source) {
  u8 ramCode[96];
  u16 status;
  u16 remaining;
  u16 *sourceCode;
  u16 *destinationCode;
  u8 attempt;
  u8 index;
  u8 rounds;
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

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | gFlash->wait[0];

  // Program the verified sector, stopping at the first failed byte.
  gFlashNumRemainingBytes = gFlash->sector.size;
  while (gFlashNumRemainingBytes != 0) {
    status = ProgramFlashByte(source, destination);
    if (status != 0) {
      break;
    }
    gFlashNumRemainingBytes -= 1;
    source++;
    destination++;
  }

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | WAITCNT_SRAM_8;

  return status;
}

/// Returns how many bytes remain when the first non-erased byte is found: 0
/// when the sector at cursor reads erased throughout.
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

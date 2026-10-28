#include "../../../include/gamepak/flash.h"
#include "../../../include/gamepak/flash_data.h"
#include "../../../include/system/io_reg.h"

s32 CountRemainingErasedFlashBytes(u8 *ptr);
s32 RunFlashEraseVerifier(u8 *ptr, s32 (*verify)(u8 *));

extern FlashWaitProc gFlash_Handler3;

u16 _call_via_r3(s32 mode, u8 *address, u8 expected,
                 FlashWaitProc waitForWrite);

/// Issues the chip-erase command and waits for an erased byte.
u16 EraseFlashChip(void) {
  u8 savedCode[64];
  u16 result;
  FlashWaitProc *waitForWrite;

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | gFlash->wait[0];

  *(volatile u8 *)0x0E005555 = 0xAA;
  *(volatile u8 *)0x0E002AAA = 0x55;
  *(volatile u8 *)0x0E005555 = 0x80;
  *(volatile u8 *)0x0E005555 = 0xAA;
  *(volatile u8 *)0x0E002AAA = 0x55;
  *(volatile u8 *)0x0E005555 = 0x10;

  CopyFlashReadRoutineToRam(savedCode);

  waitForWrite = &gFlash_Handler3;
  result = _call_via_r3(3, (u8 *)0x0E000000, 255, *waitForWrite);

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | 3;

  return result;
}

/// Erases one of sixteen sectors, restoring the cartridge wait setting
/// afterward.
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

  if (sector > 15) {
    return 0x80FF;
  }

  waitcnt = &REG_WAITCNT;
  setting = *waitcnt & 0xFFFC;
  chip = gFlash;
  *waitcnt = setting | chip->wait[0];

  target = (volatile u8 *)((sector << chip->sector.shift) + 0x0E000000);

  command = (volatile u8 *)0x0E005555;
  *command = 0xAA;
  unlock = (volatile u8 *)0x0E002AAA;
  *unlock = 0x55;
  *command = 0x80;
  *command = 0xAA;
  *unlock = 0x55;
  *target = 0x30;

  CopyFlashReadRoutineToRam(savedCode);

  waitForWrite = &gFlash_Handler3;
  result = _call_via_r3(2, (u8 *)target, 255, *waitForWrite);

  *waitcnt = (*waitcnt & 0xFFFC) | 3;

  return result;
}

/// Issues a byte-program command and waits for its destination to match.
u16 ProgramFlashByte(u8 *source, u8 *destination) {
  FlashWaitProc *waitForWrite;

  *(volatile u8 *)0x0e005555 = 0xaa;
  *(volatile u8 *)0x0e002aaa = 0x55;
  *(volatile u8 *)0x0e005555 = 0xa0;
  *destination = *source;
  waitForWrite = &gFlash_Handler3;
  return (*waitForWrite)(1, destination, *source);
}

extern u16 gFlashNumRemainingBytes;

/// Erases and verifies a sector with retries, then programs its bytes.
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

  if (sector > 15) {
    return 0x80FF;
  }

  destination = (u8 *)((sector << gFlash->sector.shift) + 0x0E000000);

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

  // A previously failed erase receives six more conditioning erases.
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

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | 3;

  return status;
}

/// Returns how many bytes remain when the first non-erased byte is found.
s32 CountRemainingErasedFlashBytes(u8 *cursor) {
  s32 remaining;

  for (remaining = gFlash->sector.size; remaining != 0; remaining--) {
    if (*cursor++ != 0xff)
      break;
  }
  return remaining;
}

/// Converts a nonzero erased-byte remainder into the flash verification error.
s32 RunFlashEraseVerifier(u8 *sector, s32 (*verify)(u8 *)) {
  if (verify(sector) != 0)
    return 0x8004;
  return 0;
}

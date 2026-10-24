#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"
#include "../../../include/system/io_reg.h"

s32 CountRemainingErasedFlashBytes(u8 *ptr);
s32 RunFlashEraseVerifier(u8 *ptr, s32 (*verify)(u8 *));

extern FlashWaitProc gFlash_Handler3;

u16 _call_via_r3(s32 mode, u8 *address, u8 expected, FlashWaitProc wait);

/// Issues the chip-erase command and waits for an erased byte. Returns 0, or
/// 0xc003 when byte 0 has not read 0xff within the chip-erase time. The
/// erase-chip routine (gFlash_Handler1) of every chip but Atmel; nothing in
/// ☀️ calls through that cell.
u16 EraseFlashChip(void) {
  u8 savedCode[64];
  u16 result;
  FlashWaitProc *wait;

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | gFlash->wait[0];

  *(volatile u8 *)0x0E005555 = 0xAA;
  *(volatile u8 *)0x0E002AAA = 0x55;
  *(volatile u8 *)0x0E005555 = 0x80;
  *(volatile u8 *)0x0E005555 = 0xAA;
  *(volatile u8 *)0x0E002AAA = 0x55;
  *(volatile u8 *)0x0E005555 = 0x10;

  CopyFlashReadRoutineToRam(savedCode);

  wait = &gFlash_Handler3;
  result = _call_via_r3(3, (u8 *)0x0E000000, 255, *wait);

  REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | 3;

  return result;
}

/// Erases one of sixteen sectors, restoring the cartridge wait setting
/// afterward: the chip's own SRAM wait while erasing, 3 after. Returns 0,
/// 0x80ff for a sector above 15, or 0xc002 when the sector's first byte has
/// not read 0xff within the sector-erase time. The erase-sector routine
/// (gEraseFlashSector) of every chip but Atmel, and the first step of
/// ProgramFlashSector and ProgramFlashSectorWithEraseRetry.
u16 EraseFlashSector(u16 sector) {
  u8 savedCode[64];
  u16 result;
  u16 setting;
  FlashWaitProc *wait;
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

  wait = &gFlash_Handler3;
  result = _call_via_r3(2, (u8 *)target, 255, *wait);

  *waitcnt = (*waitcnt & 0xFFFC) | 3;

  return result;
}

/// Issues a byte-program command and waits for its destination to match.
/// Returns 0, or 0xc001 when the byte does not read back in time. The caller
/// sets the wait state and the RAM byte reader first.
u16 ProgramFlashByte(u8 *source, u8 *destination) {
  FlashWaitProc *wait;

  *(volatile u8 *)0x0e005555 = 0xaa;
  *(volatile u8 *)0x0e002aaa = 0x55;
  *(volatile u8 *)0x0e005555 = 0xa0;
  *destination = *source;
  wait = &gFlash_Handler3;
  return (*wait)(1, destination, *source);
}

extern u16 gFlashNumRemainingBytes;

/// Erases and verifies a sector with retries, then programs its bytes. The
/// program routine (gFlash_Handler0) of the SST and unknown chips, which
/// Save_WriteSector calls with 4 KB at `source`. Returns 0,
/// 0x80ff for a sector above 15, the last erase or check error after 81
/// failed tries (0x8004 for a sector that would not read blank), or 0xc001
/// for the first byte that did not read back in time.
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
  // was blank at once receives one. XXX (no evidence): why a sector that
  // already reads blank is erased again; the reason lies in the SST chip's
  // behaviour, which neither the ROM nor a render shows.
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

/// Returns how many bytes remain when the first non-erased byte is found: 0
/// when the sector at cursor reads 0xff throughout. Runs from a RAM copy
/// (ProgramFlashSectorWithEraseRetry).
s32 CountRemainingErasedFlashBytes(u8 *cursor) {
  s32 remaining;

  for (remaining = gFlash->sector.size; remaining != 0; remaining--) {
    if (*cursor++ != 0xff)
      break;
  }
  return remaining;
}

/// Converts a nonzero erased-byte remainder into the flash verification error:
/// returns 0 when `verify` finds the sector blank, else 0x8004.
s32 RunFlashEraseVerifier(u8 *sector, s32 (*verify)(u8 *)) {
  if (verify(sector) != 0)
    return 0x8004;
  return 0;
}

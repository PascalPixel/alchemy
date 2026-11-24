/// @file
/// The flash chip: which chip the cartridge holds (IdentifyFlash), the timer
/// that bounds each wait, and reading and verifying a sector. Erasing and
/// programming are each chip's own: sst.c, atmel.c and
/// program_flash_sector.c.

#include "gamepak/flash.h"
#include "gamepak/flash_data.h"
#include "system/io_reg.h"
#include "system/irq.h"
#include "types.h"

typedef u8 (*FlashByteRead)(u8 *address);

/// Asks the chip who made it: returns its device id above its maker id, as
/// gFlash_Chips lists them.
s32 ReadFlashId(void) {
  struct {
    u8 buffer[64];
    u16 delay;
  } locals;
  FlashByteRead readByte;
  s32 result;

  CopyFlashReadRoutineToRam(locals.buffer);
  readByte = (FlashByteRead)(locals.buffer + 1);

  FLASH_SEND_COMMAND(FLASH_COMMAND_ENTER_ID_MODE);

  // Give the chip time to answer.
  {
    volatile u16 *delay = &locals.delay;

    *delay = 20000;
    while (*delay != 0)
      *delay = *delay - 1;
  }

  result = (u16)readByte((u8 *)(FLASH_BASE + 1)) << 8;
  result |= readByte((u8 *)FLASH_BASE);

  // Leave ID mode and wait as long again.
  FLASH_SEND_COMMAND(FLASH_COMMAND_RESET);

  {
    volatile u16 *delay = &locals.delay;

    *delay = 20000;
    while (*delay != 0)
      *delay = *delay - 1;
  }
  return result;
}

/// Installs the routines and timings of the chip the cartridge holds; returns 1
/// for a chip it does not know, which gets SST's.
s32 IdentifyFlash(void) {
  u16 missing;
  u16 id;
  const struct FlashSetupInfo *const *chip;

  REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;
  id = ReadFlashId();
  chip = gFlash_Chips;
  missing = 1;
  // Only the last entry, gFlash_ChipDefault, has a maker byte of 0.
  while ((*chip)->type.id.separate.maker != 0) {
    if (id == (*chip)->type.id.joined) {
      missing = 0;
      break;
    }
    chip++;
  }
  gProgramFlashSector = (*chip)->program;
  gEraseFlashChip = (*chip)->eraseChip;
  gEraseFlashSector = (*chip)->eraseSector;
  gWaitForFlashWrite = (*chip)->wait;
  gFlashMaxTime = (*chip)->maxTime;
  gFlash = &(*chip)->type;
  return missing;
}

/// The flash timer's interrupt, about once a millisecond: counts the running
/// wait down and raises gFlashTimeoutFlag when it runs out.
void FlashTimerIntr(void) {
  if (gFlashTimerCount != 0) {
    s32 countLeft = gFlashTimerCount;

    countLeft -= 1;
    gFlashTimerCount = countLeft;
    if ((u16)countLeft == 0) {
      gFlashTimeoutFlag = 1;
    }
  }
}

/// Chooses timer timerIndex (0 to 3) to time the flash waits, and hands back
/// FlashTimerIntr in *callback for the caller to install as its interrupt;
/// returns 1 for a timer that does not exist.
s32 SetFlashTimerIntr(u8 timerIndex, void (**callback)(void)) {
  if (timerIndex > 3)
    return 1;
  gFlashTimerNum = timerIndex;
  // Each timer's counter, REG_TMnCNT_L, is 4 bytes after the one before.
  gFlashTimerReg = (u16 *)((u32)&REG_TM0CNT_L + gFlashTimerNum * 4);
  *callback = FlashTimerIntr;
  return 0;
}

/// Starts the flash timer on the wait for phase timingIndex (an enum
/// FlashPhase), enabling its interrupt and saving REG_IME.
void StartFlashTimer(u8 timingIndex) {
  // Row timingIndex of the chip's wait-time table: interrupts to count, the
  // timer's reload and its control.
  u16 *timing = gFlashMaxTime + timingIndex * 3;
  u16 *timerRegister;

  gFlashSavedIme = REG_IME;
  REG_IME = 0;
  REG_IE |= (1 << IRQ_TIMER0) << gFlashTimerNum;
  REG_IME = 1;
  gFlashTimeoutFlag = 0;
  gFlashTimerCount = *timing;
  timing++;
  // The control halfword follows the counter's (TMnCNT_H after TMnCNT_L);
  // gFlashTimerReg steps to it and back.
  timerRegister = gFlashTimerReg;
  *timerRegister = *timing;
  timerRegister++;
  gFlashTimerReg = timerRegister;
  *timerRegister = timing[1];
  timerRegister--;
  gFlashTimerReg = timerRegister;
}

/// Stops the flash timer, disables its interrupt and restores REG_IME.
void StopFlashTimer(void) {
  u16 **savedTimerAddress = &gFlashTimerReg;
  u16 *timerRegister = *savedTimerAddress;

  *timerRegister = 0;
  timerRegister++;
  *savedTimerAddress = timerRegister;
  *timerRegister = 0;
  timerRegister--;
  *savedTimerAddress = timerRegister;

  REG_IME = 0;
  REG_IE &= ~(u16)((1 << IRQ_TIMER0) << gFlashTimerNum);
  REG_IME = gFlashSavedIme;
}

/// Reads one byte of the chip. The erase and program routines copy it into RAM
/// and poll the chip through the copy while it is written.
u8 ReadFlashByte(u8 *source) { return *source; }

/// Copies the byte reader to RAM and installs its Thumb entry point.
/// `destination` takes the code from ReadFlashByte up to this function, and
/// gFlashReadRoutine points into it: the callers pass a buffer in their own
/// stack frame, so the reader is good only until that caller returns.
void CopyFlashReadRoutineToRam(void *destination) {
  u16 *destinationCode = destination;
  u16 *sourceCode;
  u16 halfwordsRemaining;

  gFlashReadRoutine = (u8 (*)(u8 *))((s32)destinationCode + 1);
  sourceCode = (u16 *)ReadFlashByte;
  sourceCode = (u16 *)((s32)sourceCode ^ 1);
  halfwordsRemaining =
      ((s32)CopyFlashReadRoutineToRam - (s32)ReadFlashByte) >> 1;
  while (halfwordsRemaining != 0) {
    *destinationCode++ = *sourceCode++;
    halfwordsRemaining--;
  }
}

/// Waits until the byte at address reads expected, which is when a program or
/// erase has finished. Returns 0, or after the phase's time limit
/// FLASH_ERROR_TIMEOUT with the phase in its low byte.
u16 WaitForFlashWrite(u8 timingIndex, volatile u8 *address, u8 expected) {
  u16 result = 0;

  StartFlashTimer(timingIndex);
  while ((u8)gFlashReadRoutine((u8 *)address) != expected) {
    if (gFlashTimeoutFlag) {
      // One last read, in case the write finished as time ran out.
      if ((u8)gFlashReadRoutine((u8 *)address) == expected)
        break;
      // A Macronix chip is sent FLASH_COMMAND_RESET.
      if (gFlash->id.joined == FLASH_ID_MACRONIX)
        FLASH_WRITE(FLASH_COMMAND_ADDRESS, FLASH_COMMAND_RESET);
      result = timingIndex | FLASH_ERROR_TIMEOUT;
      break;
    }
  }
  StopFlashTimer();
  return result;
}

/// Copies size bytes of the chip to destination; ReadFlash runs it from a copy
/// in RAM.
void ReadFlashCore(const u8 *source, u8 *destination, u32 size) {
  u32 remaining = size - 1;

  if (size != 0) {
    do {
      *destination++ = *source++;
    } while (remaining-- != 0);
  }
}

typedef void (*FlashReadCore)(const u8 *source, u8 *destination, u32 size);

/// Reads size bytes from offset in 4 KB sector `sector` into destination,
/// through a copy of ReadFlashCore in RAM.
void ReadFlash(u16 sector, u32 offset, u8 *destination, s32 size) {
  u16 code[64];
  u16 *sourceCode;
  u16 *destinationCode;
  u16 halfwordsRemaining;
  FlashReadCore core;

  REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;
  sourceCode = (u16 *)ReadFlashCore;
  sourceCode = (u16 *)((u32)sourceCode ^ 1);
  destinationCode = code;
  halfwordsRemaining = ((u32)ReadFlash - (u32)ReadFlashCore) / 2;
  while (halfwordsRemaining != 0) {
    *destinationCode++ = *sourceCode++;
    halfwordsRemaining--;
  }
  core = (FlashReadCore)((u8 *)code + 1);
  core((u8 *)FLASH_BASE + (sector << gFlash_ChipDefault.type.sector.shift) +
           offset,
       destination, size);
}

/// Compares size bytes; returns the first byte of right that differs from left,
/// or NULL when all match. VerifyFlashSector runs it from a copy in RAM.
u8 *VerifyFlashCore(u8 *left, u8 *right, u16 size) {
  u16 index = size - 1;

  while (index != (u16)-1) {
    u8 rightByte = *right;
    u8 leftByte = *left;

    left++;
    right++;
    if (rightByte != leftByte) {
      return right - 1;
    }
    index--;
  }
  return 0;
}

typedef u8 *(*FlashVerifyCore)(u8 *source, u8 *flash, u16 size);

/// Compares 4 KB sector `sector` of the chip with source; returns 0 when they
/// match, else the address of the first byte of the chip that differs.
s32 VerifyFlashSector(u16 sector, u8 *source) {
  u16 code[128];
  u16 *sourceCode;
  u16 *destinationCode;
  u16 halfwordsRemaining;
  FlashVerifyCore core;

  REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;
  sourceCode = (u16 *)VerifyFlashCore;
  sourceCode = (u16 *)((u32)sourceCode ^ 1);
  destinationCode = code;
  halfwordsRemaining = ((u32)VerifyFlashSector - (u32)VerifyFlashCore) / 2;
  while (halfwordsRemaining != 0) {
    *destinationCode++ = *sourceCode++;
    halfwordsRemaining--;
  }
  core = (FlashVerifyCore)((u8 *)code + 1);
  return (s32)core(source,
                   (u8 *)FLASH_BASE +
                       (sector << gFlash_ChipDefault.type.sector.shift),
                   (u16)gFlash_ChipDefault.type.sector.size);
}

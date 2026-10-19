#include "../../../../common/include/system/flash.h"
#include "../../../../common/include/system/flash_data.h"
#include "../../../../common/include/system/io_reg.h"
#include "types.h"

typedef u8 (*FlashByteRead)(u8 *address);

// Enters flash ID mode, waits for the chip and reads its device and
// manufacturer IDs.
s32 ReadFlashId(void) {
  struct {
    u8 buffer[64];
    u16 delay;
  } locals;
  FlashByteRead readByte;
  s32 result;

  CopyFlashReadRoutineToRam(locals.buffer);
  readByte = (FlashByteRead)(locals.buffer + 1);

  // Unlock the chip and enter its manufacturer/device identification mode.
  *(volatile u8 *)0x0e005555 = 0xaa;
  *(volatile u8 *)0x0e002aaa = 0x55;
  *(volatile u8 *)0x0e005555 = 0x90;

  {
    volatile u16 *delay = &locals.delay;

    *delay = 20000;
    while (*delay != 0)
      *delay = *delay - 1;
  }

  // The high byte identifies the device; the low byte identifies its maker.
  result = (u16)readByte((u8 *)0x0e000001) << 8;
  result |= readByte((u8 *)0x0e000000);

  *(volatile u8 *)0x0e005555 = 0xaa;
  *(volatile u8 *)0x0e002aaa = 0x55;
  *(volatile u8 *)0x0e005555 = 0xf0;

  {
    volatile u16 *delay = &locals.delay;

    *delay = 20000;
    while (*delay != 0)
      *delay = *delay - 1;
  }
  return result;
}

extern const struct FlashSetupInfo *const gFlash_Chips[];
extern FlashWaitProc gFlash_Handler3;
extern u32 gFlash_Handler0;
extern u32 gFlash_Handler1;
extern u32 gEraseFlashSector;
extern u16 *gFlash_Handler4;

// Selects matching chip handlers, falling back to the final table entry when
// unknown.
s32 IdentifyFlash(void) {
  u16 missing;
  u16 id;
  const struct FlashSetupInfo *const *chip;

  REG_WAITCNT = (REG_WAITCNT & ~3) | 3;
  id = ReadFlashId();
  chip = gFlash_Chips;
  missing = 1;
  while (((u8 *)&(*chip)->type.id)[0] != 0) {
    if (id == (*chip)->type.id) {
      missing = 0;
      break;
    }
    chip++;
  }
  // Install the selected record, including the fallback when no ID matched.
  gFlash_Handler0 = (*chip)->handlers[0];
  gFlash_Handler1 = (*chip)->handlers[1];
  gEraseFlashSector = (*chip)->handlers[2];
  gFlash_Handler3 = (*chip)->wait;
  gFlash_Handler4 = (*chip)->maxTime;
  gFlash = &(*chip)->type;
  return missing;
}

extern volatile u16 gFlashTimerCount;
extern u8 gFlashTimeoutFlag;
extern u8 gFlashTimerNum;
extern u16 *gFlashTimerReg;

// Counts down the active flash timeout and marks its expiration.
void FlashTimerIntr(void) {
  if (gFlashTimerCount != 0) {
    s32 ticksRemaining = gFlashTimerCount;

    ticksRemaining -= 1;
    gFlashTimerCount = ticksRemaining;
    if (((u32)ticksRemaining << 16) == 0) {
      gFlashTimeoutFlag = 1;
    }
  }
}

// Selects a hardware timer and returns its timeout interrupt callback.
s32 SetFlashTimerIntr(u8 timerIndex, void (**callback)(void)) {
  if (timerIndex > 3)
    return 1;
  gFlashTimerNum = timerIndex;
  gFlashTimerReg = (u16 *)(0x04000100 + gFlashTimerNum * 4);
  *callback = FlashTimerIntr;
  return 0;
}

extern u16 gFlashSavedIme;

// Starts the selected operation timeout, saving the interrupt-enable state.
void StartFlashTimer(u8 timingIndex) {
  u16 *timing = (u16 *)((u8 *)gFlash_Handler4 + timingIndex * 6);
  u16 *timerRegister;

  gFlashSavedIme = REG_IME;
  REG_IME = 0;
  REG_IE |= 8 << gFlashTimerNum;
  REG_IME = 1;
  gFlashTimeoutFlag = 0;
  gFlashTimerCount = *timing;
  timing++;
  timerRegister = gFlashTimerReg;
  *timerRegister = *timing;
  timerRegister++;
  gFlashTimerReg = timerRegister;
  *timerRegister = timing[1];
  timerRegister--;
  gFlashTimerReg = timerRegister;
}

// Stops the flash timer and restores the saved interrupt-enable state.
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
  REG_IE &= ~(u16)(8 << gFlashTimerNum);
  REG_IME = gFlashSavedIme;
}

// Reads one byte; copied into RAM so the cartridge can be polled during a
// write.
u8 ReadFlashByte(u8 *source) { return *source; }

// Copies the byte reader to RAM and installs its Thumb entry point.
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

void StartFlashTimer(u8 index);

// Polls for the expected byte until success or timeout, returning an operation
// error.
u16 WaitForFlashWrite(u8 timingIndex, volatile u8 *address, u8 expected) {
  u16 result = 0;

  StartFlashTimer(timingIndex);
  while ((u8)gFlashReadRoutine((u8 *)address) != expected) {
    if (gFlashTimeoutFlag) {
      // Allow a final successful read at the timeout boundary.
      if ((u8)gFlashReadRoutine((u8 *)address) == expected)
        break;
      if (gFlash->id == 0x1cc2)
        *(volatile u8 *)0x0e005555 = 0xf0;
      result = timingIndex | 0xc000;
      break;
    }
  }
  StopFlashTimer();
  return result;
}

// Copies flash bytes into RAM; this routine executes from a RAM copy.
void ReadFlashCore(const u8 *source, u8 *destination, u32 size) {
  u32 remaining = size - 1;

  if (size != 0) {
    do {
      *destination++ = *source++;
    } while (remaining-- != 0);
  }
}

typedef void (*FlashReadCore)(const u8 *source, u8 *destination, u32 size);

// Copies the RAM reader and reads a range using the standard sector geometry.
void ReadFlash(u16 sector, u32 offset, u8 *destination, s32 size) {
  u16 code[64];
  u16 *sourceCode;
  u16 *destinationCode;
  u16 halfwordsRemaining;
  FlashReadCore core;

  REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;
  sourceCode = (u16 *)ReadFlashCore;
  sourceCode = (u16 *)((u32)sourceCode ^ 1);
  destinationCode = code;
  halfwordsRemaining = ((u32)ReadFlash - (u32)ReadFlashCore) / 2;
  while (halfwordsRemaining != 0) {
    *destinationCode++ = *sourceCode++;
    halfwordsRemaining--;
  }
  core = (FlashReadCore)((u8 *)code + 1);
  core((u8 *)0x0e000000 + (sector << gFlash_ChipUnknown.type.sector.shift) +
           offset,
       destination, size);
}

// Returns the first differing byte in the right buffer, or NULL when all bytes
// match.
u8 *VerifyFlashCore(u8 *left, u8 *right, u16 size) {
  u16 index = size - 1;

  while (index != 0xFFFF) {
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

// Copies the verifier to RAM and compares a standard flash sector with source
// data.
s32 Flash_VerifySector(u16 sector, s32 source) {
  u16 code[128];
  u16 *sourceCode;
  u16 *destinationCode;
  u16 halfwordsRemaining;
  FlashVerifyCore core;

  REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;
  sourceCode = (u16 *)VerifyFlashCore;
  sourceCode = (u16 *)((u32)sourceCode ^ 1);
  destinationCode = code;
  halfwordsRemaining = ((u32)Flash_VerifySector - (u32)VerifyFlashCore) / 2;
  while (halfwordsRemaining != 0) {
    *destinationCode++ = *sourceCode++;
    halfwordsRemaining--;
  }
  core = (FlashVerifyCore)((u8 *)code + 1);
  return (s32)core((u8 *)source,
                   (u8 *)0x0e000000 +
                       (sector << gFlash_ChipUnknown.type.sector.shift),
                   (u16)gFlash_ChipUnknown.type.sector.size);
}

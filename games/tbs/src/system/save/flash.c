#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"
#include "../../../include/system/io_reg.h"
#include "types.h"

typedef u8 (*FlashByteRead)(u8 *address);

/// Enters flash ID mode, waits for the chip and reads its device and
/// manufacturer IDs. Returns device << 8 | maker; the makers' codes in
/// gFlash_Chips' records are 0xbf (SST), 0x1f (Atmel), 0xc2 (Macronix) and
/// 0x32 (Panasonic). Called by IdentifyFlash.
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

  // Give the chip time to answer.
  {
    volatile u16 *delay = &locals.delay;

    *delay = 20000;
    while (*delay != 0)
      *delay = *delay - 1;
  }

  // The high byte identifies the device; the low byte identifies its maker.
  result = (u16)readByte((u8 *)0x0e000001) << 8;
  result |= readByte((u8 *)0x0e000000);

  // Leave ID mode (0xf0) and wait as long again.
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

/// Selects matching chip handlers, falling back to the final table entry when
/// unknown. Returns 0 when the chip's id is one of gFlash_Chips' SST, Atmel,
/// Macronix and Panasonic records and 1 when not; either way it installs a
/// record's routines, timings and type (gFlash), for an unknown chip
/// gFlash_ChipUnknown's: SST's routines and timings. Called by
/// SaveState_InitializeWorkspace, up to eight times a frame apart, before
/// each use of the save. It rewrites three of the cells the credits borrow;
/// graphics/credits/credits.c says why that borrowing is harmless.
s32 IdentifyFlash(void) {
  u16 missing;
  u16 id;
  const struct FlashSetupInfo *const *chip;

  REG_WAITCNT = (REG_WAITCNT & ~3) | 3;
  id = ReadFlashId();
  chip = gFlash_Chips;
  missing = 1;
  // Only the last record, gFlash_ChipUnknown, has a maker byte of 0.
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

/// Counts down the active flash timeout and marks its expiration. The timer
/// interrupt handler SetFlashTimerIntr hands out; it runs about once a
/// millisecond while StartFlashTimer's timer runs, and sets gFlashTimeoutFlag
/// when gFlashTimerCount reaches 0.
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

/// Selects a hardware timer and returns its timeout interrupt callback:
/// FlashTimerIntr goes to *callback, for the caller to install as timer
/// timerIndex's interrupt handler. Returns 0, or 1 for a timer above 3 and
/// nothing changed. Called by SaveState_InitializeWorkspace with timer 2.
s32 SetFlashTimerIntr(u8 timerIndex, void (**callback)(void)) {
  if (timerIndex > 3)
    return 1;
  gFlashTimerNum = timerIndex;
  // Each timer's counter, REG_TMnCNT_L, is 4 bytes after the one before
  gFlashTimerReg = (u16 *)((u32)&REG_TM0CNT_L + gFlashTimerNum * 4);
  *callback = FlashTimerIntr;
  return 0;
}

extern u16 gFlashSavedIme;

/// Starts the selected operation timeout, saving the interrupt-enable state.
/// timingIndex is the operation's phase (WaitForFlashWrite). Interrupts are
/// on while the timer runs, whatever REG_IME was; StopFlashTimer puts the
/// saved value back.
void StartFlashTimer(u8 timingIndex) {
  // Row timingIndex of the chip's max-time table (flashsst.s, flashat.s,
  // flashmx.s, in flashset.s's flash_max_time form): interrupts to count, the
  // timer's reload and its control. Every row in ☀️ but Atmel's
  // empty row 2 reloads 0xffbd with 0xc2, an interrupt every 67 ticks of 256
  // cycles, about a millisecond.
  u16 *timing = (u16 *)((u8 *)gFlash_Handler4 + timingIndex * 6);
  u16 *timerRegister;

  gFlashSavedIme = REG_IME;
  // Timer n's interrupt is IE bit 3 + n.
  REG_IME = 0;
  REG_IE |= 8 << gFlashTimerNum;
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

/// Stops the flash timer and restores the saved interrupt-enable state.
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

/// Reads one byte; copied into RAM so the cartridge can be polled during a
/// write.
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

void StartFlashTimer(u8 index);

/// Polls for the expected byte until success or timeout, returning an operation
/// error. Every chip's wait routine (gFlash_Handler3). timingIndex is the
/// phase, 1 programming, 2 a sector erase and 3 a chip erase: it picks the
/// max-time row and is the error's low byte. Returns 0 once address reads
/// expected, else 0xc000 | timingIndex. Reads through gFlashReadRoutine, which
/// the caller must have copied to RAM (CopyFlashReadRoutineToRam).
u16 WaitForFlashWrite(u8 timingIndex, volatile u8 *address, u8 expected) {
  u16 result = 0;

  StartFlashTimer(timingIndex);
  while ((u8)gFlashReadRoutine((u8 *)address) != expected) {
    if (gFlashTimeoutFlag) {
      // Allow a final successful read at the timeout boundary.
      if ((u8)gFlashReadRoutine((u8 *)address) == expected)
        break;
      // A Macronix chip (0x1cc2) is sent the reset command, 0xf0.
      if (gFlash->id == 0x1cc2)
        *(volatile u8 *)0x0e005555 = 0xf0;
      result = timingIndex | 0xc000;
      break;
    }
  }
  StopFlashTimer();
  return result;
}

/// Copies flash bytes into RAM; this routine executes from a RAM copy.
void ReadFlashCore(const u8 *source, u8 *destination, u32 size) {
  u32 remaining = size - 1;

  if (size != 0) {
    do {
      *destination++ = *source++;
    } while (remaining-- != 0);
  }
}

typedef void (*FlashReadCore)(const u8 *source, u8 *destination, u32 size);

/// Copies the RAM reader and reads a range using the standard sector geometry.
/// Reads `size` bytes from `offset` in `sector` into `destination`, running
/// ReadFlashCore from a copy in a 128-byte local. Sectors are addressed with
/// gFlash_ChipUnknown's 4 KB geometry whatever the chip, which is the same:
/// every record gFlash can point to has 4 KB sectors. Called by the save code
/// for a sector's record and a file's summary (runtime/runtime/save.c).
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

/// Returns the first differing byte in the right buffer, or NULL when all bytes
/// match.
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

/// Copies the verifier to RAM and compares a standard flash sector with source
/// data. Returns 0 when sector `sector` holds the 4 KB at `source`, else the
/// address of its first differing byte. Called by SaveState_WriteWorkspaceSlot
/// after each write.
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

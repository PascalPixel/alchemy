#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "ramcode.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"
#include "irq.h"
#include "types.h"
#include "scene.h"
#include "resource.h"
#include "resource_ids.h"
#include "global_cells.h"
#include "cartboot.h"
#include "ram_buffer.h"
#include "serial_runtime.h"
#include "frame.h"
#include "keystate.h"
#include "oam.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "irqlock.h"
#include "iowrite.h"
#include "lib.h"
#include "fixed_math.h"
#include "affine.h"
#include "iwram_call.h"
#include "vram_block.h"
#include "string.h"
#include "low_runtime.h"
#include "vram_tab.h"
#include "static_assert.h"
#include "edition.h"
#include "transform.h"
#include "project.h"
#include "save_state.h"
#include "runtime_interfaces.h"
#include "flash.h"
#include "audio_engine_symbols.h"
#include "unit/runtime/math_table.h"
#include "unit/runtime/numtext.h"
#include "savedata.h"

#if TBS_PATCH_2
#define HALT_ON_GAMEPAK_IRQ 1
#endif

// Powers the game up: stops any HBlank DMA left running, sets the cartridge
// wait states, clears IWRAM, installs the interrupt handlers and the resident
// IWRAM code, loads the window graphics and the sound driver, then starts a
// new game. It never returns.
void System_Initialize(void) {
  {
    volatile u16 *dma0 = REG_DMA0;

    // DMA0CNT_H: drop the HBlank repeat timing, then the enable bit
    dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
    dma0[5] &= ~DMA_ENABLE;
    dma0[5];
  }
  Io_Write16(WAITCNT_GAME, &REG_WAITCNT);
  // Install the resident runtime before any service can use its work blocks.
  Dma_Fill32(0, (void *)0x03000000, 0x7800);
  Runtime_InitializeHeap();
  Runtime_InstallIwramAndIrqs();
  *(u32 *)&gIoWriteQueue.count = 0;
  gDebugTextEnabled = 0;
  gDebugMode = 0;
  gCpuLoadDisplayEnabled = 0;
  System_CopyBuildStamp();
  Ui_LoadWindowGraphics();
  DebugText_Clear();
  REG_DISPCNT = 0x140;
  Runtime_SetIrqHandler(IRQ_VBLANK, 1, (void (*)(void))System_VBlankHandler);
#if defined(HALT_ON_GAMEPAK_IRQ)
  Runtime_SetIrqHandler(IRQ_GAMEPAK, 1, IwramHalt);
#endif
  Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
  // Start audio and frame services only after the interrupt handlers are ready.
  Audio_InitializeRuntimeDefaultsFar();
  VramCache_Reset();
  Scheduler_ResetTaskTable();
  gCartridgeResetMarker = 0;
  gOamCopyEnabled = 1;
  gSleepDisabled = 0;
  WaitFrames(10);
  Game_ResetForNewGameFar(0);
}

// Waits out frames forever; the frame loop still runs the scheduled tasks.
void Runtime_IdleForever(void) {
  for (;;) {
    (void)gKeysPressed;
    WaitFrames(1);
  }
}

#if TBS_PATCH_4

enum CartridgeLanguage {
  CARTRIDGE_LANGUAGE_ENGLISH,
  CARTRIDGE_LANGUAGE_GERMAN,
  CARTRIDGE_LANGUAGE_FRENCH,
  CARTRIDGE_LANGUAGE_SPANISH,
  CARTRIDGE_LANGUAGE_ITALIAN
};

// Returns the language of this cartridge from the header's game code letter:
// 0 English, 1 German, 2 French, 3 Spanish, 4 Italian.
s32 System_GetLanguage(void) {
  s32 language;

  switch (gRom_LanguageCode) {
  case 'D':
    language = CARTRIDGE_LANGUAGE_GERMAN;
    break;
  case 'F':
    language = CARTRIDGE_LANGUAGE_FRENCH;
    break;
  case 'S':
    language = CARTRIDGE_LANGUAGE_SPANISH;
    break;
  case 'I':
    language = CARTRIDGE_LANGUAGE_ITALIAN;
    break;
  default:
    language = CARTRIDGE_LANGUAGE_ENGLISH;
    break;
  }
  return language;
}
#endif

// Reserved empty export A in the runtime far-call table; no source caller.
UNUSED void RuntimeDispatch_ReservedNoOpA(void) {}

// Reserved empty export B in the runtime far-call table; no source caller.
UNUSED void RuntimeDispatch_ReservedNoOpB(void) {}

// Reserved empty export C in the runtime far-call table; no source caller.
UNUSED void RuntimeDispatch_ReservedNoOpC(void) {}

// Reserved empty export D in the runtime far-call table; no source caller.
UNUSED void RuntimeDispatch_ReservedNoOpD(void) {}

// Reserved empty export E in the runtime far-call table; no source caller.
UNUSED void RuntimeDispatch_ReservedNoOpE(void) {}

// An exported stub that ignores its arguments and reports 0.
s32 RuntimeDispatch_ReturnZero(s32 unused0, const u8 *unused1) { return 0; }

// The build stamp's twelve digit characters copied to IWRAM, and a zero word
// after them that ends the string.
struct BuildStampCopy {
  u32 digits[3];
  u32 terminator;
};

// Copies the build stamp resource into high IWRAM at power-on.
void System_CopyBuildStamp(void) {
  const void *stamp;
  struct BuildStampCopy *copy;

  stamp = Resource_GetTableEntry((u32)&ResourceId_BuildStamp);
  copy = (struct BuildStampCopy *)&gIwramHi_Data1;
  Dma_Set(stamp, copy, (DMA_ENABLE | DMA_32BIT) << 16 | 3, REG_DMA3);
  copy->terminator = 0;
}

// An exported stub that does nothing with its argument.
void RuntimeDispatch_NoOpHook(s32 unused) {}

// Returns the ROM address of resource `index` from the resource directory.
void *Resource_GetTableEntry(s32 index) {
  return Resource_DirectoryTable[index];
}

// Resolves the Thumb long branches (BL pairs) of a block of code whose branch
// offsets are stored from the start of the block. A BL is two halfwords: the
// first carries the marker bits 0xf000 and the high eleven bits of the offset,
// the second the marker 0xf800 and the low eleven bits of the halfword offset.
// Each pair's offset is reduced by the pair's own position in the block, which
// turns it into the pc-relative offset the CPU expects. `byteLength` is the
// code size.
void Code_ResolveLongBranches(u16 *entries, s32 byteLength) {
  s32 entryCount;
  s32 entryIndex;

  entryCount = byteLength >> 1;
  entryIndex = 1;
#ifndef UBFIX
  entries++;
#endif
  if (entryIndex < entryCount) {
    u32 nextMarkerMask;
    u32 currentMarkerMask;
    u32 payloadMask = 0x7ff;
    nextMarkerMask = 0xf800;
    currentMarkerMask = 0xf000;
#ifndef UBFIX
    // UB: temporarily forms a pointer before the start of the code block.
    entries -= 2;
#endif
    // Only adjacent halfwords with both BL marker patterns are relocated.
    do {
      u16 currentEntry;
      u16 nextEntry;
      u32 nextMarkerBits;
      u32 currentMarkerBits;

#ifdef UBFIX
      nextEntry = entries[1];
#else
      nextEntry = entries[2];
#endif
      nextMarkerBits = nextEntry & nextMarkerMask;
#ifndef UBFIX
      entries++;
#endif
      if (nextMarkerBits == nextMarkerMask) {
        currentEntry = entries[0];
        currentMarkerBits = currentEntry & nextMarkerBits;
        if (currentMarkerBits == currentMarkerMask) {
          u32 packedOffset;
          u32 adjustedCurrent;
          u32 adjustedNext;

          packedOffset = (((currentEntry & payloadMask) << 12) |
                          ((nextEntry & payloadMask) * 2)) -
                         (entryIndex * 2);
          adjustedCurrent =
              ((packedOffset >> 12) & payloadMask) | currentMarkerBits;
          adjustedNext = ((packedOffset >> 1) & payloadMask) | nextMarkerBits;
          entries[0] = adjustedCurrent;
          entries[1] = adjustedNext;
        }
      }
#ifdef UBFIX
      entries++;
#endif
      entryIndex++;
    } while (entryIndex < entryCount);
  }
}

// Loads the compressed code resource `index` to `destination` and resolves
// its long branches there, with a RAM copy of the ARM routine that does what
// Code_ResolveLongBranches does.
void Resource_LoadCode(u32 index, void *destination) {
  void (*patchBranches)(void *, s32);
  s32 codeSize;

  codeSize = Resource_DecodeType01(Resource_GetTableEntry(index), destination);
  RamCode_Load(patchBranches, Resource_PatchThumbBranch,
               Resource_PatchThumbBranchCodeSize);
  patchBranches(destination, codeSize);
  MemFree(patchBranches);
}

// The handler of every interrupt nobody has claimed.
void Runtime_IgnoreInterrupt(void) {}

// Copies the resident code to IWRAM, points the BIOS interrupt vector at its
// dispatcher, installs the default handler table and enables the VBlank and
// keypad interrupts (and, outside Japan and England, the cartridge-removed
// interrupt, which halts the game).
void Runtime_InstallIwramAndIrqs(void) {
  u8 *iwram;

  // Keep interrupts masked while replacing both the dispatcher and its table.
  Io_Write16(0, &REG_IME);
  iwram = IwramIrqMain;
  Dma_Set(IwramRuntime_Rom, iwram, (DMA_ENABLE | DMA_32BIT) << 16 | 0x500,
          REG_DMA3);
  gIrqVector = iwram;
  Dma_Set(gRuntime_IrqHandlers, gIrqHandlers,
          (DMA_ENABLE | DMA_32BIT) << 16 | IRQ_COUNT, REG_DMA3);
  Io_Write16(0, &REG_DISPSTAT);
  // Keypad interrupt only when all ten keys are down: in effect off until
  // System_Initialize arms the soft-reset combination.
  Io_Write16(0xc3ff, &REG_KEYCNT);
#if defined(HALT_ON_GAMEPAK_IRQ)
  Io_Write16(INTR_FLAG_VBLANK | INTR_FLAG_KEYPAD | INTR_FLAG_GAMEPAK, &REG_IE);
#else
  Io_Write16(INTR_FLAG_VBLANK | INTR_FLAG_KEYPAD, &REG_IE);
#endif
  Io_Write16(1, &REG_IME);
}

// Restarts the game from the cartridge entry point. The marker word it leaves
// in IWRAM is cleared again by System_Initialize; nothing else reads it.
static __inline__ void System_SoftReset(void) {
  /* FAKEMATCH: the restart entry is called as returning s32, so its address is
   * built in r0 as the native does at both resets; through a void (*)(void)
   * pointer it is built in r1 (differ 4 in all six editions). */
  s32 (*reset)(void) = (s32 (*)(void))Cart_Restart;

  gCartridgeResetMarker = 0x19670704;
  Io_Write16(0, &REG_IME);
  reset();
}

// Sleeps in the BIOS Halt until the VBlank handler has run once.
#define VBlankIntrWait()                                                       \
  {                                                                            \
    gVBlankDone &= ~1;                                                         \
    do {                                                                       \
      CAMELOT_ASM("Halt, which no C compiles to")                              \
      __asm__ volatile("swi 2");                                               \
    } while (!(gVBlankDone & 1));                                              \
  }

// Installs `handler` for interrupt `irq` and enables that interrupt, or with a
// null handler disables it and installs the do-nothing handler. The three
// display interrupts are also switched in DISPSTAT; for VCount, `vcount` is
// the line it fires on.
void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler) {
  if (irq <= IRQ_GAMEPAK) {
    u32 ime;
    u32 bit;
    u32 ie;

    IRQ_LOCK(ime);
    bit = 1 << irq;
    ie = REG_IE & ~bit;
    if (handler != 0)
      ie |= bit;
    REG_IE = ie;
    if (irq <= IRQ_VCOUNT) {
      // DISPSTAT bits 3 to 5 enable the VBlank, HBlank and VCount interrupts
      u32 enable = 8 << irq;
      u32 keep = ~enable;
      u32 stat;

      if (irq == IRQ_VCOUNT) {
        enable |= vcount << 8;
        keep &= 0xff;
      }
      stat = REG_DISPSTAT & keep;
      if (handler != 0)
        stat |= enable;
      REG_DISPSTAT = stat;
    }
    if (handler != 0)
      gIrqHandlers[irq] = handler;
    else
      gIrqHandlers[irq] = Runtime_IgnoreInterrupt;
    IRQ_UNLOCK(ime);
  }
}

// Frame counts used by the sleep transitions and the CPU-load peak display.
enum {
  SLEEP_IDLE_FRAMES = 3 * 60 * 60,
  SLEEP_COMBO_FRAMES = 3 * 60,
  SLEEP_WHITE_FRAMES = 60,
  SLEEP_WAKE_FRAMES = 10,
  CPU_LOAD_PEAK_FRAMES = 30
};

// The game's frame loop: runs `frames` frames and returns. Each frame runs the
// scheduled main-loop tasks, builds the OAM list, measures CPU load for the
// debug display, counts idle time for sleep, steps the debug pause, waits for
// VBlank and then reads the keys. Holding L and R for three seconds, or three
// idle minutes with idle sleep on, puts the system into Stop until L and R are
// pressed again. A soft reset requested by the keypad interrupt happens here.
//
// Tasks may call WaitFrames deep inside their own call chains, so a caller
// whose stack has grown below the frame-wait stack top has its stack copied
// aside first, and the loop runs at that top; the copy is put back on return.
void WaitFrames(s32 frames) {
  u8 *sp;
  u32 frameIndex;
  u8 *base;
  u32 line;
  s32 delayFrame;
  s16 savedDisplayControl;
  s16 savedBackdropColor;

  CAMELOT_ASM("read the hardware stack pointer for the proven stack switch")
  __asm__ volatile("mov %0, sp" : "=r"(sp));

  // Save the caller's stack and continue at the frame-wait stack top
  base = sp;
  if (base < Ram_FrameWaitStackTop) {
    u8 *top = Ram_FrameWaitStackTop;

    gSavedStackSize = top - base;
    Dma_Set(base, gSavedStack,
            (gSavedStackSize >> 2) | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
    /* FAKEMATCH: the tied top operand retains its DMA lifetime; an input-only
     * operand adds a copy through ip. */
    CAMELOT_ASM("the stack switch, a write to sp no C can express")
    __asm__ volatile("mov sp, %0" : "+r"(top) : : "sp", "memory");
  }
  for (frameIndex = 0; frameIndex < frames; frameIndex++) {
    // Run the main-loop tasks and build this frame's sprites
    gSchedulerStatus = 1;
    Scheduler_RunPass(SCHEDULER_KEY_MAIN_LOOP);
    gSchedulerStatus = 0;
    Render_BuildOamList(
        Runtime_AllocateHeapBlock(HEAP_SLOT_FRAME_SCRATCH, 0x400));
    gFrameRenderPending = 1;
    if (gCpuLoadDisplayEnabled) {
      // CPU load: the scanline the frame's work ended on, counted from the
      // last VBlank, plus 256 for each frame it overran; the peak is held
      // for 30 frames
      line = *(u16 *)&REG_VCOUNT;
      if (line >= 160)
        line -= 160;
      else
        line += 68;
      line += (gLagFrameCount - 1) << 8;
      if (gCpuLoadTimer == 0)
        gCpuLoadPeak = 0;
      else
        gCpuLoadTimer--;
      if (gCpuLoadPeak < line) {
        gCpuLoadPeak = line;
        gCpuLoadTimer = CPU_LOAD_PEAK_FRAMES;
      }
    }
    // Count idle frames and the L+R hold that request sleep
    if (gSleepDisabled == 0) {
      if (gIdleSleepEnabled) {
        if (gKeysHeld)
          gIdleFrames = 0;
        else {
          gIdleFrames++;
          if (gIdleFrames > SLEEP_IDLE_FRAMES)
            gSleepRequested = 1;
        }
      }
      if (gKeysHeld == KEYS_SHOULDERS) {
        gSleepComboFrames++;
        if (gSleepComboFrames >= SLEEP_COMBO_FRAMES) {
          gSleepComboFrames = 0;
          gSleepRequested = 1;
        }
      } else {
        gSleepComboFrames = 0;
      }
    }
    // Debug builds: Start+Select pauses the game; while paused, A, B,
    // Select or the D-pad advances one frame and Start resumes
    if (gDebugMode) {
      for (;;) {
        if (gDebugPaused) {
          if (gKeysRepeat & (KEY_A | KEY_B | KEY_SELECT))
            break;
          if (gKeysHeld & KEYS_DPAD)
            break;
          if (gKeysRepeat & KEY_START) {
            gDebugPaused = 0;
            break;
          }
        } else {
          if (gKeysHeld != (KEY_START | KEY_SELECT))
            break;
          gDebugPaused = 1;
        }
        CAMELOT_ASM("Halt in VBlankIntrWait")
        VBlankIntrWait();
        Input_UpdateKeyRepeatAndDirection();
        if (gResetRequested) {
          gResetRequested = 0;
          System_SoftReset();
        }
      }
    }
    // Wait for VBlank, then start the next frame
    gLagFramesShown = gLagFrameCount;
    gLagFrameCount = 0;
    CAMELOT_ASM("Halt in VBlankIntrWait")
    VBlankIntrWait();
    Runtime_ReleaseHeapBlock(HEAP_SLOT_FRAME_SCRATCH);
    Render_ResetFrame();
    gFrameCount++;
    gPlayTimeFrames++;
    Input_UpdateKeyRepeatAndDirection();
    if (gSerialExchangeActive) {
      SerialRuntime_PollStatus();
      if (gSerialRuntime.mode)
        gSerialRuntime.transferEnabled = 1;
    }
    // Sleep: after a second of white screen, Stop the CPU until L and R wake
    // it, then restore the screen. A request above 1 counts down first.
    if (gSleepRequested && gSleepDisabled == 0) {
      savedDisplayControl = REG_DISPCNT;
      savedBackdropColor = PLTT_BACKDROP;
      if (gSleepRequested == 1) {
        Io_Write16(0, &REG_DISPCNT);
        Io_Write16(0x7fff, &PLTT_BACKDROP);
        for (delayFrame = 0; delayFrame < SLEEP_WHITE_FRAMES; delayFrame++) {
          CAMELOT_ASM("Halt in VBlankIntrWait")
          VBlankIntrWait();
        }
        gSleepActive = 1;
        Io_Write16(KEYCNT_SHOULDERS_WAKE, &REG_KEYCNT);
        Bios_SoundBiasOff();
        CAMELOT_ASM("Stop, which no C compiles to")
        __asm__ volatile("swi 3");
        Bios_SoundBiasOn();
        Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
        gSleepActive = 0;
        Io_Write16((u16)savedDisplayControl, &REG_DISPCNT);
        Io_Write16((u16)savedBackdropColor, &PLTT_BACKDROP);
        for (delayFrame = 0; delayFrame < SLEEP_WAKE_FRAMES; delayFrame++) {
          CAMELOT_ASM("Halt in VBlankIntrWait")
          VBlankIntrWait();
        }
        gSleepRequested = 0;
        gIdleFrames = 0;
      } else {
        gSleepRequested--;
      }
    }
    if (gResetRequested) {
      gResetRequested = 0;
      System_SoftReset();
    }
  }
  // Put the caller's stack back
  {
    u32 *size = &gSavedStackSize;

    if (*size) {
      /* FAKEMATCH: tying the old base to the hardware read keeps the eight-byte
       * frame and removes an extra copy through ip. */
      CAMELOT_ASM("read the hardware stack pointer for its restore copy")
      __asm__ volatile("mov %0, sp" : "+r"(base));
      /* FAKEMATCH: needed to match: the volatile reread keeps this saved-size
       * load separate from the conditional read (7 aligned differences
       * without this qualification). */
      base -= *(volatile u32 *)size;
      sp = base;
      CAMELOT_ASM(
          "restore the hardware stack pointer after the proven stack copy")
      __asm__ volatile("mov sp, %0" : : "r"(sp) : "sp", "memory");
      /* FAKEMATCH: clearing the old address and restoring a separate
       * pointer in complementary conditions ends the restore CSE block.
       * The conditions vanish, retaining the size-address reload in r3
       * and its load into r2. Reusing size instead of nextSize changes
       * that pair to r2/r2 (4 aligned differences, same 0x434 extent);
       * deleting the empty barrier alone gives 24 differences. */
      size = NULL;
      {
        u32 *nextSize = NULL;

        if (sp)
          nextSize = &gSavedStackSize;
        if (!sp)
          nextSize = &gSavedStackSize;
        Dma_Set(gSavedStack, base,
                (*nextSize >> 2) | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
      }
      while (DMA3->control & (DMA_ENABLE << 16))
        ;
      gSavedStackSize = 0;
    }
  }
}

// Frames a key is held before it first repeats, and between later repeats
#define KEY_REPEAT_FIRST 19
#define KEY_REPEAT_NEXT 6

// Restarts the key repeat delay, so held keys wait the full delay again.
void Input_ResetKeyRepeat(void) { gKeyRepeatTimer = KEY_REPEAT_FIRST; }

// Runs once a frame after VBlank. When the repeat timer has run out, the held
// keys repeat into gKeysRepeat. gKeysRepeatDirected is the same keys with at
// most one D-pad direction: of several held directions it keeps the one
// added last (gKeysDirection remembers it), and with all four it keeps none.
// Last, the keys newly held since the previous frame go to gKeysPressed.
void Input_UpdateKeyRepeatAndDirection(void) {
  s32 delay = gKeyRepeatTimer;
  u32 keys;
  u32 axis = 0;
  u32 directionCount;
  u32 mask;

  if (delay <= 0) {
    gKeysRepeat = gKeysHeld;
    keys = gKeysRepeat;
    if (delay == 0)
      gKeyRepeatTimer = KEY_REPEAT_NEXT;
    else
      gKeyRepeatTimer = KEY_REPEAT_FIRST;
  } else {
    gKeysRepeat = 0;
    keys = gKeysRepeat;
  }

  if (keys != 0) {
    // Count the D-pad directions in the repeat
    directionCount = 0;
    if (keys & KEY_UP)
      directionCount = 1;
    if (keys & KEY_DOWN)
      directionCount++;
    if (keys & KEY_LEFT)
      directionCount++;
    if (keys & KEY_RIGHT)
      directionCount++;

    gKeysRepeatDirected = keys;
    switch (directionCount) {
    default:
      gKeysDirection = KEYS_HORIZONTAL;
      mask = 0xffff & ~KEYS_DPAD;
      gKeysRepeatDirected &= mask;
      break;
    case 0:
      gKeysDirection = KEYS_HORIZONTAL;
      break;
    case 1:
      gKeysDirection = keys & KEYS_DPAD;
      break;
    case 2:
      if ((gKeysDirection & gKeysRepeatDirected) == 0)
        gKeysDirection = KEYS_HORIZONTAL;
      gKeysRepeatDirected &= gKeysDirection ^ 0xffff;
      break;
    case 3:
      if (gKeysDirection & KEYS_HORIZONTAL)
        axis = KEYS_HORIZONTAL;
      if (gKeysDirection & KEYS_VERTICAL)
        axis = KEYS_VERTICAL;
      mask = 0xffff ^ axis;
      gKeysDirection = keys & mask;
      gKeysRepeatDirected &= mask;
      break;
    }
  } else {
    gKeysRepeatDirected = keys;
  }

  gKeysPressed = (gKeysHeld ^ gKeysHeldLast) & gKeysHeld;
  gKeysHeldLast = gKeysHeld;
}

#define SOUND_PRESET_COUNT 5
#define SOUND_PRESET_SIZE 152

// The VBlank interrupt handler. It trades the link cable payloads, steps the
// music fades and the screen fade, and when the frame loop has finished a
// frame copies its OAM list and BG scroll registers and flushes the queued
// register writes. Then it runs the one-shot VBlank hook and the VBlank
// tasks, reads the keypad and finally mixes the next sound buffer.
void System_VBlankHandler(void) {
  u32 keys;
  void (*hook)(void);

  // Stop any HBlank DMA effect left running from the last frame
  {
    volatile u16 *dma0 = REG_DMA0;

    dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
    dma0[5] &= ~DMA_ENABLE;
    dma0[5];
  }
  if (gSerialExchangeActive != 0) {
    u16 *status = (u16 *)&gLinkStatus;
    *status = SerialRuntime_ExchangePayloads((void *)&gSerialTransfer,
                                             gSerialPeerPayloads);
    SerialRuntime_StepBlockTransfer();
  }
  MusicPlayer_StepVolumeAndPitchTowardTargetsFar();
  Fade_Update();
  if (gFrameRenderPending != 0) {
    if (gOamCopyEnabled != 0)
      Dma_Set(((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_FRAME_SCRATCH],
              OAM, (DMA_ENABLE | DMA_32BIT) << 16 | 0x100, REG_DMA3);
    Dma_Set(gBgScroll, REG_BG0HOFS, (DMA_ENABLE | DMA_32BIT) << 16 | 4,
            REG_DMA3);
    IoWriteQueue_FlushPending();
    gFrameRenderPending = 0;
  }
  // The hook runs once: it is cleared before it is called
  hook = gVBlankHook;
  if (hook != 0) {
    gVBlankHook = 0;
    hook();
  }
  Scheduler_RunPass(SCHEDULER_KEY_VBLANK);

  // Read the keypad (its bits are 0 while pressed). A newly held key restarts
  // the repeat at once; letting go of all keys restarts the full delay.
  keys = REG_KEYINPUT ^ KEYS_MASK;
  {
    u32 pressed = keys & ~gKeysHeld;

    gKeysPressed = pressed;
    gKeysPressedLatch |= pressed;
  }
  gKeysHeld = keys;
  if (keys == 0) {
    gKeyRepeatTimer = KEY_REPEAT_FIRST;
    gKeysRepeat = keys;
  } else if (gKeysHeld & (gKeysHeldAtVBlank ^ 0xffff)) {
    gKeyRepeatTimer = -1;
    gKeysRepeat = keys;
  } else if (gKeyRepeatTimer > 0) {
    gKeyRepeatTimer--;
  }
  gKeysHeldAtVBlank = keys;
  gFrameTick++;
  gLagFrameCount++;
  gVBlankDone = 1;
  SoundDriver_EnterMixer();
}

// Copies the mixer code for sound preset `preset` (0 to 4; anything else is 0)
// from ROM into its place in IWRAM. Always returns 0.
s32 Sound_LoadPresetParameters(s32 preset) {
  s32 index;

  index = preset;
  if ((u32)preset > SOUND_PRESET_COUNT - 1) {
    index = 0;
  }
  Dma_Set(IwramCodeRecords + index * SOUND_PRESET_SIZE, IwramSoundPresetCode,
          (DMA_ENABLE | DMA_32BIT) << 16 | (SOUND_PRESET_SIZE / 4), REG_DMA3);
  return 0;
}

// Reserved empty sound export A in the runtime bank; no source caller.
UNUSED void Sound_ReservedNoOpA(void) {}

// Reserved empty sound export B in the runtime bank; no source caller.
UNUSED void Sound_ReservedNoOpB(void) {}

// The keypad interrupt: A+B+Select+Start was pressed, so ask the frame loop
// for a soft reset and disarm the interrupt. During sleep the same interrupt
// only wakes the CPU.
void Input_HandleKeyInterrupt(void) {
  if (gSleepActive == 0) {
    Io_Write16(0xc3ff, &REG_KEYCNT);
    gResetRequested = 1;
  }
}

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

// The queued register writers: each adds a write of `value` to `address` that
// the VBlank handler carries out `delay` frames later (the delay sits in the
// high half of the entry's control word).
#define DEFINE_QUEUE_IO_WRITE(name, delay)                                     \
  void name(u32 address, u32 value) {                                          \
    IO_WRITE_QUEUE_PUSH(address, value, delay);                                \
  }

// Queues a register write for the next VBlank.
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay1, 1 << 16)
// Queues a register write two frames ahead.
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay2, 2 << 16)
// Queues a register write three frames ahead.
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay3, 3 << 16)
// Queues a register write five frames ahead.
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay5, 5 << 16)
// Queues a register write six frames ahead.
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay6, 6 << 16)
// Queues a register write seven frames ahead.
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay7, 7 << 16)
// Queues a register write nine frames ahead.
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay9, 9 << 16)
// Queues a register write ten frames ahead.
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay10, 10 << 16)
// Queues a register write eleven frames ahead.
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay11, 11 << 16)

// Called from VBlank: runs the queued register writes, with a copy of the
// executor routine on the stack.
void IoWriteQueue_FlushPending(void) {
  struct IoWriteQueue *queue;
  u32 count;

  queue = &gIoWriteQueue;
  count = queue->count;
  if (count != 0) {
    QueueFlushRoutine routine;
    u32 size = (u32)TransferCommandExecutor_Size;
    u32 wordCount = size >> 2;
    u32 zero = 0;
    u32 control = 0x84;

    control <<= 24;
    {
      u8 routineBytes[wordCount << 2];

      // The executor must finish before this stack buffer goes out of scope.
      routine = (QueueFlushRoutine)routineBytes;
      Dma_Set(Resource_ExecuteTransfer, (void *)routine, wordCount | control,
              REG_DMA3);
      routine(queue, count);
      queue->count = zero;
    }
  }
}

enum {
  FADE_GAME_LAYERS = 0x3e,
  FADE_LAYER_MASK = 0x3f,
  FADE_BRIGHTEN = 0x80,
  FADE_DARKEN = 0xc0,
  FADE_LEVEL_MAX = 16
};

// Steps the screen fade once a frame from VBlank: sets BLDCNT to brighten or
// darken the chosen layers and moves BLDY one step from the start level to
// the target level.
void Fade_Update(void) {
  if (gBlendDuration != 0) {
    {
      volatile u16 *blendControl;
      u32 control;

      if (gBlendBrighten != 0) {
        control = gBlendLayers | FADE_BRIGHTEN;
        blendControl = &REG_BLDCNT;
      } else {
        control = gBlendLayers | FADE_DARKEN;
        blendControl = &REG_BLDCNT;
      }
      *blendControl = control;
    }
    {
      u8 *remaining = &gBlendFramesLeft;
      s32 delta;
      s32 level;
      s32 step;

      (*remaining)--;
      level = gBlendTargetLevel;
      delta = gBlendStartLevel - gBlendTargetLevel;
      step = *remaining;
      /* FAKEMATCH: an explicit libgcc call, which clobbers memory, so the zero
       * test reloads *remaining after it as the native does. With the /
       * operator the division is a libcall block CSE sees through: the test
       * reuses the step's load and the registers shift (differ 37 in all six
       * editions). The operator matches only if every read of *remaining is
       * forced to memory by its qualifier, which trades this call for that
       * device. */
      level += __divsi3(delta * step, gBlendDuration);
      REG_BLDY = level;
      if (*remaining == 0)
        gBlendDuration = 0;
    }
  }
}

// Screen fades. Fade_Update moves the fade one step each VBlank.
// The brightness (BLDY) runs from 0, the picture as drawn, to 16, all white
// or all black.
//   gBlendBrighten   : 0 fades toward black, 1 toward white
//   gBlendLayers     : the BLDCNT layers the fade covers
//   gBlendFramesLeft : frames until the fade ends

// Fades BG1 to BG3, the sprites and the backdrop (0x3e) from the current
// target brightness to `level` over `frames` frames.
#define DEFINE_BLEND_FADE(name, brighten, level)                               \
  void name(s32 frames) {                                                      \
    gBlendBrighten = brighten;                                                 \
    gBlendLayers = FADE_GAME_LAYERS;                                           \
    gBlendStartLevel = gBlendTargetLevel;                                      \
    gBlendTargetLevel = level;                                                 \
    gBlendDuration = frames;                                                   \
    gBlendFramesLeft = gBlendDuration;                                         \
  }

// Fades out to black.
DEFINE_BLEND_FADE(Fade_ToBlack, 0, FADE_LEVEL_MAX)

// Fades in from black.
DEFINE_BLEND_FADE(Fade_FromBlack, 0, 0)

// Fades out to white.
DEFINE_BLEND_FADE(Fade_ToWhite, 1, FADE_LEVEL_MAX)

// Fades in from white.
DEFINE_BLEND_FADE(Fade_FromWhite, 1, 0)

// Starts a fade of any layers.
//   bright : 0 toward black, 1 toward white
//   layer  : the BLDCNT layers to fade (low six bits)
//   start  : the starting brightness; above 16, the current target
//   target : the brightness to end at
//   frames : how long the fade takes
void Fade_Start(s8 bright, s32 layer, u32 start, s32 target, s32 frames) {
  gBlendBrighten = bright;
  gBlendLayers = layer & FADE_LAYER_MASK;
  if (start > FADE_LEVEL_MAX) {
    gBlendStartLevel = gBlendTargetLevel;
  } else {
    gBlendStartLevel = start;
  }
  gBlendTargetLevel = target;
  gBlendFramesLeft = (gBlendDuration = frames);
}

// Waits until the running fade has finished.
void Fade_Wait(void) {
  if (gBlendDuration != 0) {
    do {
      WaitFrames(1);
    } while (gBlendDuration != 0);
  }
}

extern u8 IwramClearWords[];

s32 _call_via_r3(s32, s32, s32, s32);

// Starts a new frame of sprites: frees the affine matrices and empties the
// 256 depth slots sprites are linked into.
void Render_ResetFrame(void) {
  gObjAffineCount = 0;
  _call_via_r3(((u32)&gRenderSlotHeads), 0x400, ((u32)&gObjAffineCount),
               (u32)IwramClearWords);
}

typedef void (*OamBuildRoutine)(void *work);

enum { OBJ_AFFINE_MATRIX_COUNT = 32, RENDER_DEPTH_SLOT_COUNT = 256 };

// Hands out the next OAM affine matrix for a sprite drawn with `source`'s
// scale and rotation and fills it in. Returns the matrix number, or 0 when
// all 32 are taken.
s32 AffineMatrix_BuildForEffect(struct AffineTransform *source) {
  union AffineMatrix *matrix;
  s16 *coefficient;
  s32 xScale;
  s32 yScale;
  s32 angle;
  u8 index;

  index = gObjAffineCount;
  xScale = (s16)source->scaleX;
  yScale = (s16)source->scaleY;
  angle = source->angle;
  if (index >= OBJ_AFFINE_MATRIX_COUNT)
    return 0;

  matrix = &gObjAffineMatrices[index];
  coefficient = matrix->coefficients;
  if ((xScale == yScale || -xScale == yScale) && angle == 0) {
    // Unrotated and evenly scaled (or mirrored): only the diagonal is set
    s32 reciprocal;
    s32 xReciprocal;

    reciprocal = Iwram_SignedDivide(0x10000, yScale);
    xReciprocal = reciprocal;
    if (-xScale == yScale)
      xReciprocal = -reciprocal;

    matrix->rows[0] = (u16)xReciprocal;
    matrix->rows[1] = (u32)reciprocal << 16;
  } else {
    // The inverse of rotate-then-scale
    s32 sine;
    s32 cosine;

    sine = Trig_Sin(angle);
    cosine = Trig_Cos(angle);
    *coefficient = cosine / xScale;
    coefficient++;
    *coefficient = sine / xScale;
    coefficient++;
    *coefficient = (-sine) / yScale;
    coefficient++;
    *coefficient = cosine / yScale;
  }

  gObjAffineCount = index + 1;
  return index;
}

// Links a sprite entry into depth slot `slot` (clamped to 0..255) for this
// frame. The entry's first word becomes its link to the next entry.
void Runtime_PushSlotEntry(void *slotEntry, s32 slot) {
  void **head;
  void *previousHead;

  if (slot >= RENDER_DEPTH_SLOT_COUNT)
    slot = RENDER_DEPTH_SLOT_COUNT - 1;
  if (slot < 0)
    slot = 0;
  head = &gRenderSlotHeads[slot];
  previousHead = *head;
  *head = slotEntry;
  *(void **)slotEntry = previousHead;
}

// Builds this frame's OAM list in `work` from the sprites linked into the
// depth slots, with a RAM copy of the ARM builder.
void Render_BuildOamList(void *work) {
  OamBuildRoutine routine;

  RamCode_Load(routine, Render_BuildOamListKernel, LoadedRuntime_Size);
  routine(work);
  MemFree((void *)routine);
}

// The sprite VRAM cache. Sprite tile memory (from 0x06010000) is split into
// 512 blocks of 64 bytes, and gResourceBlockOwners records which of the 96
// cache entries owns each block. An entry whose offset is 0xffff is free; one
// whose size is 16 or less holds no VRAM.

// Finds the first run of free blocks that holds `size` bytes and gives it to
// entry `id`. Returns its byte offset in sprite VRAM, or -1 when no run is
// long enough.
s32 VramCache_AllocateBlocks(u32 id, u32 size) {
  u32 blocks;
  s32 ret;
  s32 pos;
  u32 end;
  u32 i;

  blocks = size / VRAM_BLOCK_BYTES;
  if (id >= VRAM_CACHE_ENTRY_COUNT) {
    return -1;
  }
  pos = 0;
  for (;;) {
    ret = -1;
    if (pos >= VRAM_BLOCK_COUNT) {
      return ret;
    }
    if (gResourceBlockOwners[pos] == VRAM_BLOCK_OWNER_FREE) {
      ret = pos;
      end = blocks + ret;
      while (pos < end) {
        if (gResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE) {
          goto occupied;
        }
        pos++;
      }
      for (i = 0; i < blocks; i++) {
        gResourceBlockOwners[ret + i] = id;
      }
      break;
    }
  occupied:
    // Skip an owned block by its owner's whole size
    pos += gVramBlockCache[gResourceBlockOwners[pos]].size / VRAM_BLOCK_BYTES;
  }
  ret *= VRAM_BLOCK_BYTES;
  return ret;
}

// Returns the length, in blocks, of the longest run of free blocks.
s32 VramCache_GetLongestFreeRun(void) {
  s32 run = 0;
  u8 *owner = gResourceBlockOwners;
  s32 longestRun = 0;
  s32 remaining = VRAM_BLOCK_COUNT;

  do {
    if (*owner++ != VRAM_BLOCK_OWNER_FREE) {
      run = 0;
    } else {
      run++;
      if (longestRun < run)
        longestRun = run;
    }
    remaining--;
  } while (remaining != 0);
  return longestRun;
}

// Frees every block entry `id` owns. Returns -1 when it freed any (or `id` is
// out of range) and 0 when the entry owned none.
s32 VramCache_FreeBlocks(s32 id) {
  s32 cleared = 0;
  s32 remaining;
  u8 *owner;
  u8 empty;

  if ((u32)id >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  owner = gResourceBlockOwners;
  empty = VRAM_BLOCK_OWNER_FREE;
  remaining = VRAM_BLOCK_COUNT;
  do {
    if (*owner == id) {
      *owner = empty;
      cleared++;
    }
    remaining--;
    owner++;
  } while (remaining != 0);
  if (cleared != 0)
    return -1;
  return 0;
}

// Frees cache entry `no`: its blocks go back and the entry is marked free.
s32 Resource_ResetEntry(u32 no) {
  struct VramBlockCacheEntry *entry = &gVramBlockCache[no];

  if (no >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  if (entry->offset != VRAM_CACHE_OFFSET_FREE) {
    VramCache_FreeBlocks(no);
    entry->offset |= VRAM_CACHE_OFFSET_FREE;
    entry->size = 0;
  }
  return 0;
}

// Gives back entry `no`'s blocks but keeps the entry taken, with size 1.
s32 VramCache_ReleaseVram(u32 no) {
  struct VramBlockCacheEntry *entry = &gVramBlockCache[no];

  if (no >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  if (entry->size > 16) {
    s32 one;

    VramCache_FreeBlocks(no);
    one = 1;
    entry->size = one;
  }
  return 0;
}

// Gives cache entry `no` `size` bytes of sprite VRAM and copies `src` there.
// An entry that already holds VRAM of the same size keeps it. A null `src`
// copies nothing and (void *)-1 clears the memory. Returns the first sprite
// tile number (32-byte units), or 0 when there was no room.
s32 VramCache_Load(u32 no, u32 size, const void *src) {
  struct VramBlockCacheEntry *entry;
  s32 offset;
  void *dst;

  entry = &gVramBlockCache[no];
  if (no >= VRAM_CACHE_ENTRY_COUNT)
    return 0;
  if (size > 8192)
    return 0;
  // A same-size upload keeps its existing VRAM run.
  if (entry->size > 16) {
    if (entry->size != size) {
      Resource_ResetEntry(no);
      offset = VramCache_AllocateBlocks(no, size);
    } else {
      offset = entry->offset;
    }
  } else {
    offset = VramCache_AllocateBlocks(no, size);
  }

  if (offset != -1) {
    // Commit the allocation before copying, clearing or leaving its pixels.
    dst = (void *)(0x06010000 + offset);
    entry->size = size;
    entry->offset = offset;
    if (src != 0) {
      if (src == (const void *)-1) {
        Iwram_ClearWords(dst, size);
      } else {
        Dma_Set(src, dst, (size >> 2) | (DMA_ENABLE | DMA_32BIT) << 16,
                REG_DMA3);
      }
    }
    return (u32)offset >> 5;
  }
  return 0;
}

// Frees every block and every cache entry.
void VramCache_Reset(void) {
  u32 limit = VRAM_BLOCK_COUNT - 1;
  u8 *p = gResourceBlockOwners;
  u32 n = 0;
  u32 empty = VRAM_BLOCK_OWNER_FREE;

  do {
    *p++ = empty;
    n++;
  } while (n <= limit);

  {
    struct VramBlockCacheEntry *entry = gVramBlockCache;

    n = 0;
    do {
      entry->offset |= VRAM_CACHE_OFFSET_FREE;
      entry->size = 0;
      entry++;
      n++;
    } while (n < VRAM_CACHE_ENTRY_COUNT);
  }
}

// Returns the number of the first free cache entry, or VRAM_CACHE_ENTRY_COUNT
// when all are taken.
s32 Resource_FindFreeEntry(void) {
  s32 ret = VRAM_CACHE_ENTRY_COUNT;
  s32 no;
  struct VramBlockCacheEntry *entry = gVramBlockCache;

  for (no = 0; no < VRAM_CACHE_ENTRY_COUNT; no++, entry++) {
    if (entry->offset == VRAM_CACHE_OFFSET_FREE) {
      ret = no;
      break;
    }
  }
  return ret;
}

// Takes a free cache entry, gives it `size` bytes of sprite VRAM and returns
// its number.
s32 VramCache_AllocEntry(s32 size) {
  s32 no;

  no = Resource_FindFreeEntry();
  VramCache_Load(no, size, 0);
  return no;
}

// Copies `src` again into entry `no`'s VRAM, keeping its size. Returns the
// first sprite tile number.
s32 VramCache_Reload(s32 no, s32 src) {
  return VramCache_Load(no, gVramBlockCache[no].size, (const void *)src);
}

#ifndef UBFIX
typedef s32 (*KeyCallbackFn)(void);
#endif

// The task scheduler: up to 20 routines run each frame by WaitFrames or each
// VBlank, kept sorted by their order value (see SCHEDULER_KEY_MAIN_LOOP).

// Empties the task table.
void Scheduler_ResetTaskTable(void) {
  struct SchedulerTask *task = gSchedulerTaskTable;
  s32 remaining;

  gSchedulerReady = 0;
  gSchedulerStatus = 0;
  remaining = SCHEDULER_TBS_TASK_COUNT - 1;
  do {
    task->callback = 0;
    task->state = 0xffff;
    task->mask = 0;
    task++;
    remaining--;
  } while (remaining >= 0);
  gSchedulerReady = 1;
}

// Copies `byteCount` bytes a word at a time.
void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byteCount) {
  u32 index;
  byteCount >>= 2;
  for (index = 0; index < byteCount; index++)
    *destination++ = *source++;
}

// Bubble-sorts the task table by order, highest first, so each pass runs its
// tasks from the highest order down.
void Scheduler_SortTasks(void) {
  /* FAKEMATCH: the declaration initializer repeats the first pass's assignment;
   * the native loads the table before the loop and again at the top of every
   * pass. Without it the table address is hoisted into r5 (push r7): differ 28.
   * Reusing the swap cursor, a leading pass test or the reload at the end of a
   * pass: differ 11 to 26. Ordinary struct assignment for all three copies
   * differs 60 (33 instructions for 39); without the initializer as well,
   * it differs 64 (31 for 39), measured in tbs-en. */
  struct SchedulerTask saved;
  struct SchedulerTask *task = gSchedulerTaskTable;
  s32 pass;
  s32 index;

  for (pass = SCHEDULER_TBS_TASK_COUNT - 1; pass > 1; pass--) {
    task = gSchedulerTaskTable;
    for (index = 0; index < pass; index++) {
      if ((s16)task[1].state > (s16)task->state) {
        memcpy(&saved, task, sizeof(saved));
        memcpy(task, task + 1, sizeof(saved));
        memcpy(task + 1, &saved, sizeof(saved));
      }
      task++;
    }
  }
}

// Returns the table index of the task running `callback`, or -1.
s32 Scheduler_FindCallback(SchedulerCallback callback) {
  s32 result;
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (task->callback == (u32)callback) {
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

// Schedules `callback` with `order`, or gives a task already running it the
// new order, and re-sorts the table. Returns the task's index, or -1 when
// the table is full.
s32 Scheduler_AddOrUpdateCallback(SchedulerCallback callback, s32 order) {
  u32 savedInterruptState;
  s32 index;
  struct SchedulerTask *task;
  s32 i;

  index = -1;
  task = gSchedulerTaskTable;
  (void)gSchedulerStatus;
  IRQ_LOCK(savedInterruptState);
  // Update an existing callback before looking for an unused task slot.
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (task->callback == (u32)callback) {
      task->state = order;
      index = i;
      break;
    }
  }
  task = gSchedulerTaskTable;
  if (index == -1) {
    for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
      if (task->callback == 0) {
        task->callback = (u32)callback;
        task->state = order;
        task->mask = 0;
        index = i;
        break;
      }
    }
  }
  Scheduler_SortTasks();
  IRQ_UNLOCK(savedInterruptState);
  return index;
}

// Empty scheduler callback; it leaves all runtime state unchanged.
void Scheduler_Idle(void) {}

// Alternate empty scheduler callback with the same no-op behavior.
void Scheduler_EmptyCallback(void) {}

// Unschedules the task running `callback`. Returns its old index, or -1.
s32 Scheduler_RemoveCallback(SchedulerCallback callback) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (task->callback == (u32)callback) {
      task->callback = 0;
      task->state = 0x7fff;
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

// Suspends the task running `callback`, or every task when `callback` is null,
// by making the high byte of its order odd. Returns the last index changed,
// or -1.
s32 Scheduler_SuspendTasks(SchedulerCallback callback) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (callback != 0) {
      if (task->callback != (u32)callback)
        continue;
    }
    TASK_STATE_HIGH(task) |= 1;
    result = i;
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

// Suspends every task whose routine is in an overlay (EWRAM, 0x02xxxxxx)
// unless the task is marked to keep running (flag bit 0). The menus do this
// while they are open. Returns the last index changed, or -1.
s32 Scheduler_SuspendOverlayTasks(void) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if ((task->callback >> 24) == 2 && (task->mask & 1) == 0) {
      TASK_STATE_HIGH(task) |= 1;
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

// Sets the flags of the task running `callback`; bit 0 keeps it running while
// the overlay tasks are suspended. Returns its index, or -1.
s32 Scheduler_SetTaskFlags(SchedulerCallback callback, u32 mask) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (task->callback == (u32)callback) {
      task->mask = mask;
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

// Resumes the task running `callback`, or every task when `callback` is null.
// Returns the last index changed, or -1.
s32 Scheduler_ResumeTasks(SchedulerCallback callback) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (callback == 0 || task->callback == (u32)callback) {
      TASK_STATE_HIGH(task) &= ~1;
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

// Resumes every task whose routine is in an overlay. Returns the last index
// changed, or -1.
s32 Scheduler_ResumeOverlayTasks(void) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if ((task->callback >> 24) == 2) {
      TASK_STATE_HIGH(task) &= ~1;
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

// Runs one pass of the scheduler: every task whose order's high byte matches
// `key`'s (SCHEDULER_KEY_MAIN_LOOP or SCHEDULER_KEY_VBLANK), in table order.
// Nothing runs until Scheduler_ResetTaskTable has set the table up.
void Scheduler_RunPass(s32 key) {
  struct SchedulerTask *task = gSchedulerTaskTable;
  s32 remaining;

  key >>= 8;
  if (gSchedulerReady == 1) {
    remaining = SCHEDULER_TBS_TASK_COUNT + 1;
    task--;
    for (;;) {
      if (--remaining == 0)
        return;
      task++;
      if (TASK_STATE_HIGH(task) == key) {
        // UB: callbacks return void, and the stored word is not a pointer
        // object. The native integer-return call uses r0; a void call uses
        // r3 instead (two instructions and the call relocation differ).
#ifdef UBFIX
        SchedulerCallback callback = (SchedulerCallback)task->callback;
#else
        KeyCallbackFn callback = *(KeyCallbackFn *)&task->callback;
#endif

        callback();
      }
    }
  }
}

// Advances the game's random number generator (the C library's classic
// linear congruential one) and returns 16 bits of it.
u32 Random16(void) {
  enum { MULTIPLIER = 0x41c64e6d, INCREMENT = 0x3039 };
  u32 value = gRandomSeed * MULTIPLIER + INCREMENT;

  gRandomSeed = value;
  return (value << 8) >> 16;
}

// Moves the point `position` (x, y, z) by `radius` in direction `angle` on
// the ground plane: x by the cosine, z by the sine.
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position) {
  *position++ += Iwram_MulQ16(radius, Trig_Sin(angle + 0x4000));
  position++;
  *position += Iwram_MulQ16(radius, Trig_Sin(angle));
}

// Returns the angle of the vector (x, y), in 1/65536 turns, measured from
// the y axis: 0 along +y, 0x4000 along +x. It binary-searches the tangent
// table to 1/256 of a quarter turn.
s32 ArcTan2(s32 x, s32 y) {
  const u16 *table;
  s32 value;
  s32 ratio;
  s32 result;

  if (x == 0) {
    result = 0;
  } else if (y == 0) {
    result = 0x4000;
  } else {
    ratio = y;
    if (ratio < 0)
      ratio = -ratio;
    value = x;
    if (value < 0)
      value = -value;

    ratio = (value << 8) / ratio;
    result = 0x4000;
    if (ratio <= 0xFB6A) {
      table = &gMath_TanTable[127];
      result = 0;

      value = *table;
      table -= 0x40;
      if (ratio > value) {
        result = 0x2000;
        table += 0x80;
      }
      value = *table;
      table -= 0x20;
      if (ratio > value) {
        result |= 0x1000;
        table += 0x40;
      }
      value = *table;
      table -= 0x10;
      if (ratio > value) {
        result |= 0x800;
        table += 0x20;
      }
      value = *table;
      table -= 8;
      if (ratio > value) {
        result |= 0x400;
        table += 0x10;
      }
      value = *table;
      table -= 4;
      if (ratio > value) {
        result |= 0x200;
        table += 8;
      }
      value = *table;
      table -= 2;
      if (ratio > value) {
        result |= 0x100;
        table += 4;
      }
      value = *table;
      table--;
      if (ratio > value) {
        result |= 0x80;
        table += 2;
      }
      value = *table;
      if (ratio > value)
        result |= 0x40;
    }
  }

  if (y < 0)
    result = 0x8000 - result;
  if (x < 0)
    result = -result;

  return (u16)result;
}

// Returns the integer square root of `value`, a bit at a time.
s32 Math_IntegerSqrt(s32 value) {
  s32 trial;
  s32 remainder;
  s32 bit;
  s32 result;

  remainder = value;
  result = 0;
  bit = 0xf;
  do {
    trial = (result << (bit + 1)) + (1 << (bit * 2));
    if (trial <= remainder) {
      result |= 1 << bit;
      remainder -= trial;
    }
    bit -= 1;
  } while (bit >= 0);
  return result;
}

// Square root of a 16.16 fixed-point value, in 16.16.
s32 FixedSqrt(s32 value) { return Iwram_Sqrt(value) << 8; }

// Returns the address of the basic sprite palettes.
s32 Palette_GetBasicObjColors(void) { return (s32)gSystem_BasicColorPalette; }

// Debug text: a 32x20-character screen of window font tiles in BG0, written
// from gDebugTextCursor onwards; characters are tile numbers in palette 15.

// Writes `value` as eight hexadecimal digits to the number buffer.
void NumberText_FormatHex(u32 value) {
  u8 *buffer = gNumberTextBuffer;
  const u8 *digits = gText_HexDigits;
  s32 index = 7;

  do {
    buffer[index] = digits[value & 0xF];
    value >>= 4;
    index--;
  } while (index >= 0);

  {
    u8 *terminator = gNumberTextBuffer;
    terminator[8] = 0;
  }
}

// Writes `number` to the number buffer as eleven characters: spaces, a sign
// (a space or '-') and its decimal digits, right-aligned.
void NumberText_FormatDecimal(s32 number) {
  u32 *placeValues;
  s8 *output;
  s32 placesLeft;
  u32 placeValue;
  s32 digit;
#ifdef UBFIX
  u32 magnitude;
#else
  s32 magnitude;
#endif
  s8 sign;

  digit = number;
  magnitude = digit;
  placeValues = (u32 *)gText_PowersOfTen;
  sign = ' ';
  output = gNumberTextBuffer;
  if (number < 0) {
#ifdef UBFIX
    magnitude = 0U - magnitude;
#else
    // UB: negating INT_MIN cannot be represented as a signed s32.
    magnitude = -magnitude;
#endif
    sign = '-';
  }
  // Pad with a space for each power of ten above the number
  placeValue = *placeValues++;
  placesLeft = 9;
  if (magnitude < placeValue) {
    do {
      placesLeft -= 1;
      *output++ = ' ';
      if (placesLeft == 0)
        break;
      placeValue = *placeValues++;
    } while (magnitude < placeValue);
  }
  *output++ = sign;
  // Then one digit for each remaining power of ten
  placeValues -= 1;
  if (placesLeft != 0) {
    do {
      placeValue = *placeValues++;
      digit = magnitude / placeValue;
      *output++ = digit + '0';
      magnitude -= digit * placeValue;
      placesLeft -= 1;
    } while (placesLeft != 0);
  }
  output[0] = magnitude + '0';
  output[1] = 0;
}

// Writes `count` blank characters at the debug text cursor.
void DebugText_PrintBlanks(u32 count) {
  u16 *dst = gDebugTextCursor;
  u32 index;

  for (index = 0; index < count; index++)
    *dst++ = 0xf000;
  gDebugTextCursor = dst;
}

#ifdef MODERN
// Camelot's compiler copied the task inline; a modern one calls memcpy,
// and the game links no C library
void *memcpy(void *destination, const void *source, unsigned long size) {
  u8 *to = destination;
  const u8 *from = source;

  while (size-- != 0)
    *to++ = *from++;
  return destination;
}
#endif

// Prints the string `src` (at most 32 characters) at the debug text cursor,
// wrapping from the end of the screen to its start, when debug text is on.
void DebugText_Print(const u8 *src) {
  if (gDebugTextEnabled != 0) {
    u32 addr = ((u32)&gDebugTextCursor);
    u32 c = *src;
    u16 *dst = *(u16 **)addr;
    u32 cnt = 0;
    src++;

    if (c != 0) {
      u32 mask = 0xf000;
      addr = (u32)(BG_SCREEN_BLOCK(4) + 0x500);
      do {
        *dst++ = c | mask;
        if (dst == (u16 *)addr)
          dst = (u16 *)BG_SCREEN_BLOCK(4);
        cnt++;
        if (cnt > 31)
          break;
        c = *src++;
      } while (c != 0);
      addr = ((u32)&gDebugTextCursor);
    }
    *(u16 **)addr = dst;
  }
}

// Prints the last `digits` hexadecimal digits of `value` (1 to 8; anything
// else prints all 8).
void DebugText_PrintHex(u32 value, s32 digits) {
  s32 n;

  n = digits;
  if ((n - 1) > 7U) {
    n = 8;
  }
  NumberText_FormatHex(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextHexEnd) - n));
}

// Prints the last `digits` characters of `value` in signed decimal (1 to 10;
// anything else prints 10).
void DebugText_PrintDecimal(s32 value, s32 digits) {
  s32 n;

  n = digits;
  if ((n - 1) > 9U) {
    n = 10;
  }
  NumberText_FormatDecimal(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextDecimalEnd) - n));
}

// Blanks the debug text screen, homes the cursor and points BG0 at it.
void DebugText_Clear(void) {
  Dma_Fill32(0xf000f000, BG_SCREEN_BLOCK(4), 0x500);
  gDebugTextCursor = (u16 *)BG_SCREEN_BLOCK(4);
  Io_Write16(0x400, &REG_BG0CNT);
}

// Loads the window font and frame tiles into BG character block 0, the window
// palette into BG palette 15 with its gradient colours, and the basic sprite
// palettes.
void Ui_LoadWindowGraphics(void) {
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles),
          BG_CHAR_BLOCK(0), 0x84000800, REG_DMA3);
  Dma_Set(gUi_WindowPalette, (void *)&BG_PLTT_COLOR(15, 0), 0x80000010,
          REG_DMA3);
  Io_Write16(0, &PLTT_BACKDROP);
  Io_Write16(0x4180, &BG_PLTT_COLOR(15, 4));
  Io_Write16(0x3960, &BG_PLTT_COLOR(15, 5));
  Io_Write16(0x3140, &BG_PLTT_COLOR(15, 6));
  Io_Write16(0x2920, &BG_PLTT_COLOR(15, 7));
  Io_Write16(0x49a0, &BG_PLTT_COLOR(15, 8));
  Io_Write16(0x51c0, &BG_PLTT_COLOR(15, 9));
  Io_Write16(0x59e0, &BG_PLTT_COLOR(15, 10));
  Dma_Set(gSystem_BasicColorPalette, (void *)OBJ_PLTT, 0x800000e0, REG_DMA3);
}

// Loads the basic sprite palettes (palettes 0 to 13).
void Palette_LoadBasicObjColors(void) {
  Dma_Set(gSystem_BasicColorPalette, (void *)OBJ_PLTT, 0x800000e0, REG_DMA3);
}

// The heap: two bump allocators, one in IWRAM up to Ram_IwramHeapEnd and one
// in EWRAM up to its end (0x02040000), and 62 named slots for long-lived
// blocks (enum HeapSlot). Nothing is freed one block at a time.

// Empties the heap and both pools.
void Runtime_InitializeHeap(void) {
  union HeapState *work = (union HeapState *)gWorkSlot;

  Dma_Fill32(0, work, 0x100);
  work->heap.nextIwram = gIwramHeap;
  work->heap.nextEwram = gEwramHeap;
}

// Returns the IWRAM bytes left to allocate.
s32 Runtime_GetRemainingIwram(void) {
  union HeapState *work = (union HeapState *)gWorkSlot;

  return (s32)(*(u8 (*)[]) & gCartridgeResetMarker) - (s32)work->heap.nextIwram;
}

// Returns the EWRAM bytes left to allocate.
s32 Runtime_GetRemainingEwram(void) {
  union HeapState *work = (union HeapState *)gWorkSlot;

  return 0x02040000 - (s32)work->heap.nextEwram;
}

// Returns heap slot `kind`'s block, first allocating `size` bytes for it,
// from IWRAM when they fit and EWRAM otherwise. Returns NULL when neither has
// room.
void *Runtime_AllocateHeapBlock(s32 kind, s32 size) {
  union HeapState *work;
  s32 alignedSize;
  u32 address;
  u32 nextAddress;
  u32 next;
  u32 cachedAddress;

  work = (union HeapState *)gWorkSlot;
  cachedAddress = (u32)work->slots[kind];
  if (cachedAddress == 0) {
    cachedAddress = (u32)work->heap.nextIwram;
    alignedSize = (((u32)size + 3) >> 2) * 4;
    next = cachedAddress + alignedSize;
    if (next >= (u32)Ram_IwramHeapEnd) {
      address = (u32)work->heap.nextEwram;
      nextAddress = address + alignedSize;
      if (nextAddress >= 0x02040000U) {
        return NULL;
      }
      work->heap.nextEwram = (void *)nextAddress;
      work->slots[kind] = (void *)address;
      return (void *)address;
    }
    work->heap.nextIwram = (void *)next;
    work->slots[kind] = (void *)cachedAddress;
    return (void *)cachedAddress;
  }
  return (void *)cachedAddress;
}

// As Runtime_AllocateHeapBlock, but tries EWRAM first.
void *Runtime_AllocateBlock(s32 kind, s32 size) {
  union HeapState *work;
  u32 alignedSize;
  u32 next;
  u32 address;
  u32 nextAddress;
  u32 cachedAddress;

  work = (union HeapState *)gWorkSlot;
  cachedAddress = (u32)work->slots[kind];
  if (cachedAddress == 0) {
    address = (u32)work->heap.nextEwram;
    alignedSize = (((u32)size + 3) >> 2) * 4;
    next = address + alignedSize;
    if (next >= 0x02040000U) {
      address = (u32)work->heap.nextIwram;
      nextAddress = address + alignedSize;
      if (nextAddress >= (u32)Ram_IwramHeapEnd) {
        return NULL;
      }
      work->heap.nextIwram = (void *)nextAddress;
      work->slots[kind] = (void *)address;
      return (void *)address;
    }
    work->heap.nextEwram = (void *)next;
    work->slots[kind] = (void *)address;
    return (void *)address;
  }
  return (void *)cachedAddress;
}

// Allocates `size` bytes (rounded up to words), from IWRAM when they fit and
// EWRAM otherwise. Returns NULL when neither has room.
void *MemAlloc(s32 size) {
  union HeapState *work = (union HeapState *)gWorkSlot;
  u32 nextAddress;
  u32 next;
  u32 allocationAddress;
  u32 alignedWords = ((u32)size + 3) >> 2;

  allocationAddress = (u32)work->heap.nextIwram;
  size = alignedWords << 2;
  next = allocationAddress + size;
  if (next >= (u32)Ram_IwramHeapEnd) {
    allocationAddress = (u32)work->heap.nextEwram;
    nextAddress = allocationAddress + size;
    if (nextAddress >= 0x02040000U) {
      return NULL;
    }
    work->heap.nextEwram = (void *)nextAddress;
  } else {
    work->heap.nextIwram = (void *)next;
  }
  return (void *)allocationAddress;
}

// As MemAlloc, but tries EWRAM first.
void *Runtime_BumpAllocateAlternatePool(s32 size) {
  union HeapState *work = (union HeapState *)gWorkSlot;
  u32 alternateNextAddress;
  u32 primaryNextAddress;
  u32 allocationAddress;
  u32 alignedWords = ((u32)size + 3) >> 2;

  allocationAddress = (u32)work->heap.nextEwram;
  size = alignedWords << 2;
  primaryNextAddress = allocationAddress + size;
  if (primaryNextAddress >= 0x02040000U) {
    allocationAddress = (u32)work->heap.nextIwram;
    alternateNextAddress = allocationAddress + size;
    if (alternateNextAddress >= (u32)Ram_IwramHeapEnd) {
      return NULL;
    }
    work->heap.nextIwram = (void *)alternateNextAddress;
  } else {
    work->heap.nextEwram = (void *)primaryNextAddress;
  }
  return (void *)allocationAddress;
}

// Reserved empty memory export; its historical purpose is not established.
UNUSED void RuntimeMemory_ReservedNoOp(void) {}

// The 3D transform. gTransform is twelve s32 in 16.16 fixed point (1.0 is
// 0x10000): [0] to [8] a 3x3 rotation, [9] to [11] a translation.
// Iwram_TransformMatrix multiplies the matrix it is given into gTransform.

#define MTX_DMA ((DMA_ENABLE | DMA_32BIT) << 16 | 12) // one matrix

typedef s32 (*Dot3Fn)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);

// Allocates the one-level save area and resets the transform to identity.
void Render_ResetTransformState(void) {
  gTransformStackTop = Runtime_AllocateBlock(2, sizeof(gTransform));
  gTransformStackDepth = 0;
  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_ResetCurrent();
}

// Saves the transform (one level deep; a second save is ignored).
void Transform_Push(void) {
  if (gTransformStackDepth <= 0) {
    Dma_Set(gTransform, gTransformStackTop, MTX_DMA, REG_DMA3);
    gTransformStackDepth++;
    gTransformStackTop += sizeof(gTransform);
  }
}

// Copies the transform to `dst`.
void Transform_Store(void *dst) { Dma_Set(gTransform, dst, MTX_DMA, REG_DMA3); }

// Makes the matrix at `src` the transform.
void Transform_Load(const void *src) {
  Dma_Set(src, gTransform, MTX_DMA, REG_DMA3);
}

// Restores the saved transform.
void Transform_Pop(void) {
  if (gTransformStackDepth > 0) {
    --gTransformStackDepth;
    gTransformStackTop -= sizeof(gTransform);
    Dma_Set(gTransformStackTop, gTransform, MTX_DMA, REG_DMA3);
  }
}

// Resets the transform to identity.
void SceneTransform_ResetMatrix(void) {
  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_ResetCurrent();
}

// Rotates the transform by angles[0], angles[1] and angles[2] about the X,
// Y and Z axes.
void SceneTransform_ApplyRotation(s32 *angles) {
  s32 sinX, cosX, sinY, cosY, sinZ, cosZ;
  s32 matrix[12];

  sinX = Trig_Sin(angles[0]);
  cosX = Trig_Cos(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = Trig_Cos(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = Trig_Cos(angles[2]);
  matrix[0] = Iwram_MulQ16(cosY, cosZ);
  matrix[1] = Iwram_MulQ16(cosY, sinZ);
  matrix[2] = -sinY;
  matrix[3] =
      Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), cosZ) - Iwram_MulQ16(cosX, sinZ);
  matrix[4] =
      Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), sinZ) + Iwram_MulQ16(cosX, cosZ);
  matrix[5] = Iwram_MulQ16(sinX, cosY);
  matrix[6] =
      Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), cosZ) + Iwram_MulQ16(sinX, sinZ);
  matrix[7] =
      Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), sinZ) - Iwram_MulQ16(sinX, cosZ);
  matrix[8] = Iwram_MulQ16(cosX, cosY);
  matrix[9] = 0;
  matrix[10] = 0;
  matrix[11] = 0;
  Iwram_TransformMatrix(matrix);
}

// Rotates the transform about the X axis.
void SceneTransform_ApplyPitch(s32 angle) {
  s32 matrix[12];
  s32 sine = Trig_Sin(angle);
  s32 cosine = Trig_Cos(angle);

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(matrix);
  matrix[4] = cosine;
  matrix[5] = sine;
  matrix[7] = -sine;
  matrix[8] = cosine;
  Iwram_TransformMatrix(matrix);
}

// Rotates the transform about the Y axis.
void SceneTransform_ApplyYaw(s32 angle) {
  s32 matrix[12];
  s32 sine = Trig_Sin(angle);
  s32 cosine = Trig_Cos(angle);

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(matrix);
  matrix[0] = cosine;
  matrix[2] = -sine;
  matrix[6] = sine;
  matrix[8] = cosine;
  Iwram_TransformMatrix(matrix);
}

// Rotates the transform about the Z axis.
void SceneTransform_ApplyRoll(s32 angle) {
  s32 matrix[12];
  s32 sine = Trig_Sin(angle);
  s32 cosine = Trig_Cos(angle);

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(matrix);
  matrix[0] = cosine;
  matrix[1] = sine;
  matrix[3] = -sine;
  matrix[4] = cosine;
  Iwram_TransformMatrix(matrix);
}

// Translates the transform by `position` (x, y, z in 16.16).
void SceneTransform_ApplyPosition(const s32 *position) {
  s32 matrix[12];

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(matrix);
  matrix[9] = position[0];
  matrix[10] = position[1];
  matrix[11] = position[2];
  Iwram_TransformMatrix(matrix);
}

// Scales the transform by `scale` (x, y, z factors in 16.16).
void SceneTransform_ApplyScale(const s32 *scale) {
  s32 matrix[12];

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(matrix);
  matrix[0] = scale[0];
  matrix[4] = scale[1];
  matrix[8] = scale[2];
  Iwram_TransformMatrix(matrix);
}

// Rotates by `angles` and translates by `position` in one matrix.
void SceneTransform_ApplyRotationTranslation(s32 *angles, s32 *position) {
  s32 sinX, cosX, sinY, cosY, sinZ, cosZ;
  s32 matrix[12];

  sinX = Trig_Sin(angles[0]);
  cosX = Trig_Cos(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = Trig_Cos(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = Trig_Cos(angles[2]);
  matrix[0] = Iwram_MulQ16(cosY, cosZ);
  matrix[1] = Iwram_MulQ16(cosY, sinZ);
  matrix[2] = -sinY;
  matrix[3] =
      Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), cosZ) - Iwram_MulQ16(cosX, sinZ);
  matrix[4] =
      Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), sinZ) + Iwram_MulQ16(cosX, cosZ);
  matrix[5] = Iwram_MulQ16(sinX, cosY);
  matrix[6] =
      Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), cosZ) + Iwram_MulQ16(sinX, sinZ);
  matrix[7] =
      Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), sinZ) - Iwram_MulQ16(sinX, cosZ);
  matrix[8] = Iwram_MulQ16(cosX, cosY);
  matrix[9] = position[0];
  matrix[10] = position[1];
  matrix[11] = position[2];
  Iwram_TransformMatrix(matrix);
}

// Rotates by `angles`, scales each rotation row by `scale` and translates by
// `position`, in one matrix.
void SceneTransform_ApplyScaledRotation(s32 *angles, s32 *position,
                                        s32 *scale) {
  s32 sinX, cosX, sinY, cosY, sinZ, cosZ;
  s32 axisScale;
  s32 matrix[12];

  sinX = Trig_Sin(angles[0]);
  cosX = Trig_Cos(angles[0]);
  sinY = Trig_Sin(angles[1]);
  cosY = Trig_Cos(angles[1]);
  sinZ = Trig_Sin(angles[2]);
  cosZ = Trig_Cos(angles[2]);
  axisScale = scale[0];
  matrix[0] = Iwram_MulQ16(axisScale, Iwram_MulQ16(cosY, cosZ));
  matrix[1] = Iwram_MulQ16(axisScale, Iwram_MulQ16(cosY, sinZ));
  matrix[2] = Iwram_MulQ16(axisScale, -sinY);
  axisScale = scale[1];
  matrix[3] =
      Iwram_MulQ16(axisScale, Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), cosZ) -
                                  Iwram_MulQ16(cosX, sinZ));
  matrix[4] =
      Iwram_MulQ16(axisScale, Iwram_MulQ16(Iwram_MulQ16(sinX, sinY), sinZ) +
                                  Iwram_MulQ16(cosX, cosZ));
  matrix[5] = Iwram_MulQ16(axisScale, Iwram_MulQ16(sinX, cosY));
  axisScale = scale[2];
  matrix[6] =
      Iwram_MulQ16(axisScale, Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), cosZ) +
                                  Iwram_MulQ16(sinX, sinZ));
  matrix[7] =
      Iwram_MulQ16(axisScale, Iwram_MulQ16(Iwram_MulQ16(cosX, sinY), sinZ) -
                                  Iwram_MulQ16(sinX, cosZ));
  matrix[8] = Iwram_MulQ16(axisScale, Iwram_MulQ16(cosX, cosY));
  matrix[9] = position[0];
  matrix[10] = position[1];
  matrix[11] = position[2];
  Iwram_TransformMatrix(matrix);
}

// Builds in `out` the camera matrix of an eye at `eye` looking at `target`.
// The camera's sideways axis is kept level (no Y component).
void Transform_BuildLookAt(s32 *eye, s32 *target, s32 *out) {
  u32 code[7];
  s32 dx, dy, dz;
  s32 ux;
  s32 zero = 0;
  s32 ex;
  Dot3Fn dot;
  s32 ey;
  s32 uz;
  s32 vx, vy, vz;
  s32 scale;
  s32 ez;
  s32 t;
  u32 num = 0x80000000; // 2^31, the dividend of each reciprocal

  // Copy the dot product routine to the stack and call it there
  dot = (Dot3Fn)code;
  Dma_Set(RelocatedQ16DotProduct, code, (DMA_ENABLE | DMA_32BIT) << 16 | 7,
          REG_DMA3);

  // The view axis d, pointing from the target back to the eye, normalized
  dx = target[0] - eye[0];
  dy = target[1] - eye[1];
  dz = target[2] - eye[2];
  t = Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
  scale = Iwram_UnsignedDivide(num, t) >> 15;
  scale = -scale;
  /* FAKEMATCH: the normalization keeps its CSE block; without it, the third
   * product spills before the negated axis (four reordered instructions).
   * Measured as well: uz = -dx moved below the block differs 4, and the
   * negation folded into uz = Iwram_MulQ16(-dx, scale) differs 38. */
  do {
    dx = Iwram_MulQ16(dx, scale);
    dy = Iwram_MulQ16(dy, scale);
    dz = Iwram_MulQ16(dz, scale);
    uz = -dx;
  } while (0);

  // The sideways axis u = (dz, 0, -dx), normalized
  t = Iwram_MulQ16(dy, dy);
  scale = 0x10000;
  if (scale - t > 0)
    scale = Iwram_UnsignedDivide(num, FixedSqrt(scale - t)) << 1;
  ux = Iwram_MulQ16(dz, scale);
  uz = Iwram_MulQ16(uz, scale);

  // The up axis v, the cross product of d and u, normalized
  vx = Iwram_MulQ16(dy, uz);
  vy = Iwram_MulQ16(dz, ux) - Iwram_MulQ16(dx, uz);
  vz = -Iwram_MulQ16(dy, ux);
  scale = Iwram_UnsignedDivide(num, FixedSqrt(dot(vx, vx, vy, vy, vz, vz)))
          << 1;
  vx = Iwram_MulQ16(vx, scale);
  vy = Iwram_MulQ16(vy, scale);
  vz = Iwram_MulQ16(vz, scale);

  // The three axes are the matrix columns; the translation is each axis's
  // dot product with the eye, negated
  ex = eye[0];
  ey = eye[1];
  ez = eye[2];
  out[0] = ux;
  out[3] = zero;
  out[6] = uz;
  out[9] = -dot(ex, ux, ez, uz, zero, zero);
  out[4] = vy;
  out[7] = vz;
  out[1] = vx;
  out[10] = -dot(ex, vx, ey, vy, ez, vz);
  out[2] = dx;
  out[5] = dy;
  out[8] = dz;
  out[11] = -dot(ex, dx, ey, dy, ez, dz);
}

// Makes the camera at `eye` looking at `target` the whole transform.
void Transform_SetLookAt(s32 *eye, s32 *target) {
  Transform_BuildLookAt(eye, target, gTransform);
}

// Multiplies the camera at `eye` looking at `target` into the transform.
void Transform_ApplyLookAt(s32 *eye, s32 *target) {
  s32 work[12];

  Transform_BuildLookAt(eye, target, work);
  Iwram_TransformMatrix(work);
}

// Sets the perspective projection from a vertical field of view: the focal
// length is 80 (half the screen height) over the tangent of half the angle.
// Points nearer than `nearDepth` or farther than `farDepth` are not drawn.
void Projection_SetFieldOfView(u32 fieldOfView, s32 nearDepth, s32 farDepth) {
  s32 halfAngle;
  s32 sine;
  s32 focal;

  halfAngle = (s32)(fieldOfView + (fieldOfView >> 31)) >> 1;
  sine = Trig_Sin(halfAngle);
  focal = Iwram_RatioMulQ14(sine, Trig_Cos(halfAngle) * 80);
  gProjection.near = nearDepth;
  gProjection.focal = focal;
  gProjection.far = farDepth;
}

// Sets the perspective projection's focal length and depth range directly.
void Projection_Set(u32 focal, u32 nearDepth, u32 farDepth) {
  struct Projection *projection = &gProjection;

  projection->focal = focal;
  projection->near = nearDepth;
  projection->far = farDepth;
}

// Reads three word-aligned coordinates from `point` and projects them to the
// screen: screen[0] and screen[1]
// are its pixel position and screen[2] its whole-unit depth. Returns the
// projection scale (16.16), or 0 when the point is outside the depth range,
// when only the transformed point is written.
s32 Render_ProjectPoint(const void *point, s32 *screen) {
  struct Projection *projection;
  s32 depth;
  s32 scale;
  s32 result;

  Iwram_TransformVector(point, screen);
  projection = &gProjection;
  depth = -screen[2];
  result = 0;
  if (depth >= projection->near && depth <= projection->far) {
    screen[2] = depth >> 16;
    if (projection->focal != 0) {
      u32 shifted = (u32)depth >> 11;

      scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
    } else {
      scale = 0x151eb; // about 1.32
    }
    screen[0] = projection->centerX + Iwram_MulQ16(screen[0], scale) / 0x10000;
    screen[1] = projection->centerY - Iwram_MulQ16(screen[1], scale) / 0x10000;
    result = scale;
  }
  return result;
}

// Decompresses the halfword LZ stream at `source` to `destination`, with a
// copy of the ARM decoder on the stack.
void Resource_DecompressHalfwords(const void *source, void *destination) {
  u32 words = (u32)Resource_DecompressHalfwordsCodeSize >> 2;
  u32 routine[words];
  Dma_Set(Resource_DecodeHalfwordLz, routine,
          words | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  ((void (*)(const void *, void *))routine)(source, destination);
}

// Each decoder copies its ARM routine `code` to RAM, runs it and frees the
// copy, and returns the number of bytes it decoded.
#define DEFINE_RESOURCE_DECODE(name, code, bytes)                              \
  s32 name(const void *source, void *destination) {                            \
    s32 (*routine)(const void *, void *);                                      \
    s32 result;                                                                \
                                                                               \
    RamCode_Load(routine, code, bytes);                                        \
    result = routine(source, destination);                                     \
    MemFree(routine);                                                          \
    return result;                                                             \
  }

// Decodes a type-01 resource and returns its unpacked byte count.
DEFINE_RESOURCE_DECODE(Resource_DecodeType01, Resource_DecodeType01Arm,
                       Resource_DecodeType01CodeSize)
// Decodes a type-02 resource and returns its unpacked byte count.
DEFINE_RESOURCE_DECODE(Resource_DecodeType2, Resource_Decode2,
                       Resource_Decode2CodeSize)
// Decodes a byte-oriented LZ resource and returns its unpacked byte count.
DEFINE_RESOURCE_DECODE(Resource_DecodeByteLz, Resource_DecodeByteLzArm,
                       Resource_DecodeByteLzCodeSize)

// Shading of 8-bit pixel buffers whose values are steps 0 to 63 of a colour
// ramp. Each function copies `bytes` of `buffer` to `backup`, then shades the
// buffer in place with a stack copy of its ARM kernel. `amount` is added to
// every byte of the four-byte groups at once, so it holds the step in each
// byte. The Nonzero and Partial forms only write back four-byte groups that
// can change: not all zero, or not all at the limit already.

// Raises every pixel by `amount`, stopping at 63.
void ColorBuffer_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup,
                                   u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndBrightenCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_BrightenKernel, routine,
          words | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

// Lowers every pixel by `amount`, stopping at 0.
void ColorBuffer_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup,
                                 u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndDarkenCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_DarkenKernel, routine,
          words | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

// Scales every pixel to three quarters.
void ColorBuffer_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup,
                                             u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndScaleThreeQuartersCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_ScaleThreeQuartersKernel, routine,
          words | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

// Halves every pixel.
void ColorBuffer_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndHalveCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_HalveKernel, routine,
          words | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

// As ColorBuffer_BackupAndBrighten, skipping groups already all at 63.
void ColorBuffer_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup,
                                          u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndBrightenPartialCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_BrightenPartialKernel, routine,
          words | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

// As ColorBuffer_BackupAndDarken, skipping groups already all at 0.
void ColorBuffer_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup,
                                        u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndDarkenPartialCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_DarkenPartialKernel, routine,
          words | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

// Scales every pixel to three quarters, skipping all-zero groups.
void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup,
                                                    u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_ScaleNonzeroThreeQuartersKernel, routine,
          words | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

// Halves every pixel, skipping all-zero groups.
void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes) {
  u32 words = (u32)ColorBuffer_BackupAndHalveNonzeroCodeSize >> 2;
  u32 routine[words];
  Dma_Set(ColorBuffer_HalveNonzeroKernel, routine,
          words | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

// Saving. The flash chip holds 16 sectors of 4 KB; each holds one record: a
// 16-byte header (signature, record id, checksum of the payload, sequence
// number) and its payload. A save file is records n and n + 3 (n = 0 to 2).
// Writing a record goes to a free sector before the old copy is invalidated,
// and the highest sequence number of a record id is its current copy.

enum {
  SAVE_FLASH_IDENTIFY_ATTEMPTS = 8,
  SAVE_SECTOR_COUNT = 16,
  SAVE_RECORD_NONE = 16,
  SAVE_FILE_COUNT = 3,
  SAVE_SEQUENCE_LIMIT = 65000,
  SAVE_CHECKSUM_BYTES = 0xfe8
};

// Allocates the save workspace, identifies the flash chip (up to eight
// tries) and scans all 16 sectors: a sector counts as taken when its
// signature and checksum are good, and of two copies of a record only the
// newer stays taken. Returns 1 when no flash chip answers, 0 otherwise.
s32 SaveState_InitializeWorkspace(void) {
  struct SaveWorkspace *work;
  struct SaveSlotHeader header;
  struct SaveSlotHeader *slot;
  u32 index;

  work = Runtime_AllocateBlock(HEAP_SLOT_SAVE, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  SetFlashTimerIntr(2, &gIrqHandlers[IRQ_TIMER2]);

  for (index = 0; index < SAVE_FLASH_IDENTIFY_ATTEMPTS; index++) {
    if ((u16)IdentifyFlash() == 0)
      break;
    WaitFrames(1);
  }
  if (index >= SAVE_FLASH_IDENTIFY_ATTEMPTS)
    return 1;

  for (index = 0; index < SAVE_SECTOR_COUNT; index++) {
    u32 status;
    u32 record;
    u8 id;

    work->occupied[index] = 0;
    work->recordId[index] = SAVE_RECORD_NONE;
    work->sequence[index] = 0;
    status = SaveState_ReadSlotAndCheckChecksum(index);
    START_DMA(&work->slot, &header, 0x84000004);
    WAIT_DMA();
    slot = &header;

    if (SaveState_CompareBytes(slot->signature, (u8 *)gSave_Signature, 7) != 0)
      continue;
    work->sequence[index] = slot->sequence;
    record = slot->recordId;
    id = record;
    if (id >= SAVE_RECORD_NONE)
      continue;
    if (status != 0)
      continue;

    work->occupied[index] = 1;
    work->recordId[index] = record;
    if (status < index) {
      do {
        if (work->recordId[status] == (u8)record) {
          if (work->sequence[status] < slot->sequence)
            work->occupied[status] = 0;
          else
            work->occupied[index] = 0;
        }
        status++;
      } while (status < index);
    }
  }
  return 0;
}

// Picks the free sector to write record `mode` to, at random. The last free
// sector only goes to a record that already has a copy, so replacing a save
// always has room. Returns 16 when there is no sector to use.
u32 SaveState_SelectWriteSlot(s32 mode) {
  u32 empty[SAVE_SECTOR_COUNT];
  s32 count;
  u32 index;
  u8 *slot;
  u8 value;

  slot = gSaveWorkspace->occupied;
  count = 0;
  index = 0;
  do {
    value = *slot++;
    if (value == 0) {
      empty[count++] = index;
    }
    index += 1;
  } while (index < SAVE_SECTOR_COUNT);
  index = SAVE_SECTOR_COUNT;
  if (count != 0) {
    if (count == 1) {
      index = empty[0];
      if (SaveState_FindLatestSlot(mode) == SAVE_SECTOR_COUNT) {
        index = SAVE_SECTOR_COUNT;
      }
    } else {
      index = Random16() % count;
      index = empty[index];
    }
  }
  return index;
}

// Programs the workspace's sector buffer into flash sector `sector` and
// verifies it. Returns 0 on success and 1 on failure.
u32 SaveState_WriteWorkspaceSlot(s32 sector) {
  s32 *param = &gFlash_Handler0;
  s32 result;
  struct SaveWorkspace *work;
  s32 value;

  work = gSaveWorkspace;
  value = (u16)sector;
  if ((_call_via_r3(value, (s32)&work->slot, (s32)param, *param) << 0x10) !=
      0) {
    return 1U;
  }
  result = Flash_VerifySector(value, (s32)&work->slot);
  return (u32)((0 - result) | result) >> 0x1F;
}

// Reads flash sector `index` into the workspace's sector buffer. Returns 0
// when the payload matches the header's checksum.
s32 SaveState_ReadSlotAndCheckChecksum(s32 index) {
  struct SaveWorkspace *work;
  struct SaveSlotHeader header;
  u32 checksum;

  work = gSaveWorkspace;
  ReadFlash(index, 0, work->slot.bytes, sizeof(work->slot));
  Dma_Set(&work->slot, &header, 0x84000004, REG_DMA3);
  WAIT_DMA();
  checksum = SaveState_ChecksumWorkspace();
  return (u16)checksum - header.checksum;
}

typedef u16 (*EraseSectorProc)(u16);

// Erases flash sector `sector` with the flash library's routine for this chip.
u16 SaveState_EraseSlotSector(u16 sector) {
  return ((EraseSectorProc)gEraseFlashSector)(sector);
}

// Saves 0xff0 bytes from `source` as record `recordId`: writes the new copy
// with the next sequence number, then invalidates the old one. When the
// sequence passes 65000 it restarts at 1, written back over the old sector.
// Returns 0 on success and 1 on failure.
s32 SaveState_WriteRecord(s32 recordId, void *source) {
  struct SaveWorkspace *work;
  struct SaveSlotHeader header;
  u32 current;
  u32 slot;

  work = gSaveWorkspace;
  Dma_Fill32(0, &work->slot, sizeof(work->slot));
  WAIT_DMA();
  current = SaveState_FindLatestSlot(recordId);
  slot = SaveState_SelectWriteSlot(recordId);
  if (slot >= SAVE_SECTOR_COUNT)
    return 1;

  // Build the new sector before changing which old sector counts as current.
  START_DMA(source, work->slot.record.payload, 0x840003fc);
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, 0x84000002);
  WAIT_DMA();
  header.recordId = recordId;
  header.checksum = SaveState_ChecksumWorkspace();
  header.sequence = SaveState_GetLatestSequence(recordId) + 1;
  START_DMA(&header, &work->slot.record.header, 0x84000004);
  WAIT_DMA();

  if (SaveState_WriteWorkspaceSlot(slot) != 0)
    return 1;
  if (current < SAVE_SECTOR_COUNT && SaveState_InvalidateSlot(current) != 0)
    return 1;

  if (header.sequence > SAVE_SEQUENCE_LIMIT) {
    // Restart sequence numbering only after the replacement was written.
    header.sequence = 1;
    START_DMA(&header, &work->slot.record.header, 0x84000004);
    WAIT_DMA();
    if (SaveState_WriteWorkspaceSlot(current) != 0)
      return 1;
    if (SaveState_InvalidateSlot(slot) != 0)
      return 1;
    slot = current;
  }

  work->occupied[slot] = 1;
  work->recordId[slot] = recordId;
  work->sequence[slot] = header.sequence;
  return 0;
}

// Loads the current copy of record `recordId` into `destination`. Returns 1
// when there is none and 0 otherwise.
u32 SaveState_ReadRecordPayload(s32 recordId, void *destination) {
  struct SaveWorkspace *work;
  u32 index;

  work = gSaveWorkspace;
  index = SaveState_FindLatestSlot(recordId);
  if (index >= SAVE_SECTOR_COUNT)
    return 1;
  SaveState_ReadSlotAndCheckChecksum(index);
  START_DMA(work->slot.record.payload, destination, 0x840003fc);
  WAIT_DMA();
  return 0;
}

// Invalidates the current copy of record `recordId`. Returns 0 on success
// and 1 when there is none or the write failed.
u32 SaveState_DeleteRecord(s32 recordId) {
  s32 index;
  s32 deletionResult;

  index = SaveState_FindLatestSlot(recordId);
  if ((u32)index >= SAVE_SECTOR_COUNT) {
    return 1U;
  }
  deletionResult = SaveState_InvalidateSlot(index);
  return (u32)((0 - deletionResult) | deletionResult) >> 0x1F;
}

// Sums the first 0xfe8 payload bytes; the final eight payload bytes are
// excluded.
s32 SaveState_ChecksumWorkspace(void) {
  struct SaveWorkspace *work;
  u32 limit;
  u32 offset;
  s32 sum;

  work = gSaveWorkspace;
  limit = SAVE_CHECKSUM_BYTES - 1;
  sum = 0;
  offset = 0;
  do {
    sum += work->slot.record.payload[offset + 0];
    sum += work->slot.record.payload[offset + 1];
    sum += work->slot.record.payload[offset + 2];
    sum += work->slot.record.payload[offset + 3];
    sum += work->slot.record.payload[offset + 4];
    sum += work->slot.record.payload[offset + 5];
    sum += work->slot.record.payload[offset + 6];
    sum += work->slot.record.payload[offset + 7];
    offset += 8;
  } while (offset <= limit);
  return sum;
}

// Returns the sector holding the newest copy of record `recordId`, or 16.
u32 SaveState_FindLatestSlot(s32 recordId) {
  struct SaveWorkspace *work;
  u32 latestSequence;
  u32 latestSlot;
  u32 i;

  work = gSaveWorkspace;
  latestSlot = SAVE_SECTOR_COUNT;
  latestSequence = 0;
  for (i = 0; i < SAVE_SECTOR_COUNT; i++) {
    if (work->occupied[i] != 0 && recordId == work->recordId[i]) {
      if (latestSequence < work->sequence[i]) {
        latestSequence = work->sequence[i];
        latestSlot = i;
      }
    }
  }
  return latestSlot;
}

// Overwrites sector `index` with a header for no record (id 16) and marks it
// free. Returns 0 on success and 1 on failure.
s32 SaveState_InvalidateSlot(s32 index) {
  struct SaveWorkspace *work;
  struct SaveSlotHeader header;

  work = gSaveWorkspace;
  Dma_Fill32(0, &header, sizeof(header));
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, 0x84000002);
  WAIT_DMA();
  header.recordId = SAVE_RECORD_NONE;
  header.sequence = 0;
  START_DMA(&header, &work->slot.record.header, 0x84000004);
  WAIT_DMA();
  if (SaveState_WriteWorkspaceSlot(index) != 0)
    return 1;
  work->occupied[index] = 0;
  work->recordId[index] = SAVE_RECORD_NONE;
  work->sequence[index] = 0;
  return 0;
}

// Compares `count` bytes, as memcmp does.
s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count) {
  s32 difference = 0;

  while (count != 0) {
    difference = *left - *right;
    if (difference != 0)
      break;
    count--;
    left++;
    right++;
  }
  return difference;
}

// Returns the highest sequence number among the copies of record `recordId`.
u32 SaveState_GetLatestSequence(s32 recordId) {
  struct SaveWorkspace *work;
  u32 latestSequence;
  u32 i;

  work = gSaveWorkspace;
  latestSequence = 0;
  for (i = 0; i < SAVE_SECTOR_COUNT; i++) {
    if (work->occupied[i] != 0 && recordId == work->recordId[i]) {
      if (latestSequence < work->sequence[i])
        latestSequence = work->sequence[i];
    }
  }
  return latestSequence;
}

// Reads the summary at the start of each of the three save files for the
// file select screen, and four bytes from offset 0x110 of each file's second
// record. Returns how many save files exist.
s32 SaveState_LoadSummaryRecords(void) {
  struct SaveWorkspace *work;
  struct SaveSummary *summary;
  u32 group;
  s32 count;

  work = gSaveWorkspace;
  summary = work->summary;
  count = 0;
  group = 0;
  do {
    u32 index;

    Dma_Fill32(0, summary, sizeof(*summary));
    index = SaveState_FindLatestSlot(group);
    if (index < SAVE_SECTOR_COUNT) {
      ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
      count++;
    }
    index = SaveState_FindLatestSlot(group + SAVE_FILE_COUNT);
    if (index < SAVE_SECTOR_COUNT)
      ReadFlash(index, 0x110, summary->unk38, sizeof(summary->unk38));
    else
      *(u32 *)summary->unk38 = 0;
    group++;
    summary++;
  } while (group < SAVE_FILE_COUNT);
  return count;
}

// Removes the flash timer handler and frees the save workspace. Declared as
// returning a value, it returns none (pop {r1}; bx r1).
u32 SaveState_ReleaseWorkspace(void) {
  Runtime_SetIrqHandler(IRQ_TIMER2, 0, NULL);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_SAVE);
}

// The link cable. Two consoles trade a 24-byte payload each VBlank in
// multiplayer mode; the parent clocks the transfers from timer 3, a halfword
// at a time. Bulk data rides on that exchange in 20-byte blocks (the
// BeginTransfer functions and SerialRuntime_StepBlockTransfer).

enum {
  SERIAL_PACKET_HALFWORDS = 14,
  SERIAL_PAYLOAD_BYTES = 24,
  SERIAL_BLOCK_BYTES = 20,
  SERIAL_TRANSFER_TIMEOUT_FRAMES = 600000,
  SERIAL_IDLE_HALFWORD = 0xfefe
};

// Resets the serial port into multiplayer mode, installs the serial and
// timer 3 interrupt handlers, sets up the packet buffers and starts the
// per-frame exchange.
void SerialRuntime_Initialize(void) {
  u32 interruptEnable;
  volatile u16 *ime;
  s32 index;
  struct SerialRuntime *state;

  /* FAKEMATCH: IRQ_LOCK written out with the IME address set inside the save
   * statement; through IRQ_LOCK, whose pointer is initialized first, the IME
   * load is scheduled after sub sp, #4 where the native loads it before (differ
   * 2 in tbs-en, 123 instructions either way). */
  IME_SAVE((ime = &REG_IME), interruptEnable);
  IME_DISABLE(ime);
  // Replace both interrupt handlers before configuring multiplayer transfers.
  Runtime_SetIrqHandler(IRQ_SERIAL, 0, SerialRuntime_HandleTransferInterrupt);
  Runtime_SetIrqHandler(IRQ_TIMER3, 0, SerialRuntime_HandleTransferInterrupt);
  REG_IME = 0;
  REG_IE &= 0xff3f;
  if ((REG_IF & 0x80) != 0)
    REG_IF = 0x80;
  if ((REG_IF & 0x40) != 0)
    REG_IF = 0x40;
  REG_RCNT = 0x8000;
  REG_RCNT = 0;
  REG_SIOCNT = 0x1000;
  REG_RCNT = 0;
  REG_SIOCNT = 0x2000;
  REG_SIOCNT16 = REG_SIOCNT16 | 0x4003;
  state = &gSerialRuntime;
  REG_IME = 1;
  /* FAKEMATCH: the fill word is volatile, now Dma_Fill32's own word rather than
   * a local, so the zero is stored through its address (mov r0, sp; str r3,
   * [r0]). 2026-10-13: as a plain u32 the zero is stored to [sp, #0] before the
   * address is formed (differ 6). */
  Dma_Fill32(0, state, sizeof(*state));
  state->sendIndex = -1;
  state->sendBuffer[0] = (u16 *)(state->storage + 0);
  state->sendBuffer[1] = (u16 *)(state->storage + 32);
  // Each console has separate incoming, completed and pending packet buffers.
  for (index = 0; index <= 1; index++) {
    state->incomingBuffer[index] = (u16 *)(state->storage + 64 + index * 96);
    state->readyBuffer[index] = (u16 *)(state->storage + 96 + index * 96);
    state->pendingBuffer[index] = (u16 *)(state->storage + 128 + index * 96);
  }
  REG_IME = 0;
  REG_IE |= 0x80;
  REG_IME = 1;
  gSerialExchangeActive = 1;
  gLinkExchangeState = 0;
  gSerialSendSource = 0;
  gSerialSendSize = 0;
  gSerialReceiveDest = 0;
  gSerialReceivedSize = 0;
  BattleLink_ResetTransferState();
  IRQ_UNLOCK(interruptEnable);
}

// Lets the timer start transfers again once a link mode is set.
void SerialRuntime_EnableTransferTimer(void) {
  struct SerialRuntime *state = &gSerialRuntime;

  if (state->mode != 0)
    state->transferEnabled = 1;
}

// Disables the serial and timer interrupts and stops the transfer timer.
void SerialRuntime_DisableTransfer(void) {
  struct SerialRuntime *state;

  REG_IME = 0;
  REG_IE &= 0xff3f;
  REG_IME = 1;
  REG_SIOCNT16 = 0x2003;
  REG_TM3CNT = 0x0000c963;
  REG_IF = 0xc0;
  state = &gSerialRuntime;
  state->transferEnabled = 0;
}

// Looks for the link partner and, once connected, does one exchange (as
// SerialRuntime_ExchangePayloads). When this console is the parent (SIOCNT
// reports multiplayer mode with no error) it arms timer 3 to clock the
// transfers. Returns the link status (see SerialRuntime_ExchangePayloads).
s32 SerialRuntime_PollAndTransfer(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  switch (gSerialRuntime.phase) {
  case 0: {
    u8 mask30 = control & 0x30;
    u8 mode;

    if (mask30 == 0 && (mode = control & 0x88) == 8) {
      u8 serialError = control & 4;

      if (serialError == 0 && gSerialRuntime.sendIndex == -1) {
        u32 ie;

        Io_Write16(0, &REG_IME);
        ie = REG_IE;
        Io_Write16((ie & ~0x80) | 0x40, &REG_IE);
        Io_Write16(1, &REG_IME);
        *((volatile s8 *)siocnt + 1) &= ~0x40;
        REG_IF = 0xc0;
        REG_TM3CNT = 0xc963;
        gSerialRuntime.mode = mode;
      }
    } else if (mask30 == 0) {
      break;
    }
    gSerialRuntime.phase = 1;
  }
  case 1:
    SerialRuntime_CollectReceivedPayloads(receive);
    SerialRuntime_PrepareSendPacket(send);
    break;
  }

  gSerialRuntime.sequence++;
  status = gSerialRuntime.currentMask | (gSerialRuntime.receivedMask << 8);
  if (gSerialRuntime.mode == 8)
    status |= 0x80;
  result = SerialRuntime_AddParentFlag(status);
  if (((control << 26) >> 30) > 1)
    result |= 0x2000;
  return result;
}

// As SerialRuntime_PollAndTransfer, without exchanging data: the frame loop
// calls it, the VBlank handler does the exchange.
s32 SerialRuntime_PollStatus(void) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  if (gSerialRuntime.phase == 0) {
    u8 mode = control & 0x88;

    if (mode == 8) {
      u8 serialError = control & 4;

      if (serialError == 0 && gSerialRuntime.sendIndex == -1) {
        u32 ie;

        Io_Write16(0, &REG_IME);
        ie = REG_IE;
        Io_Write16((ie & ~0x80) | 0x40, &REG_IE);
        Io_Write16(1, &REG_IME);
        *((volatile s8 *)siocnt + 1) &= ~0x40;
        REG_IF = 0xc0;
        REG_TM3CNT = 0xc963;
        gSerialRuntime.mode = mode;
      }
      gSerialRuntime.phase = 1;
    }
    gSerialRuntime.sequence++;
  }

  status = gSerialRuntime.currentMask | (gSerialRuntime.receivedMask << 8);
  if (gSerialRuntime.mode == 8)
    status |= 0x80;
  result = SerialRuntime_AddParentFlag(status);
  if (((control << 26) >> 30) > 1)
    result |= 0x2000;
  return result;
}

// Once connected (phase 1), gathers the received payloads into `receive` and
// builds the next packet from `send`. Returns the link status:
//   bits 0 to 7  : the consoles heard from this frame (bit 7 also when parent)
//   bits 8 to 15 : the consoles heard from since the link began
//   0x1000       : this console is the parent
//   0x2000       : SIOCNT's multiplayer id is 2 or more
s32 SerialRuntime_ExchangePayloads(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  if (gSerialRuntime.phase == 1) {
    SerialRuntime_CollectReceivedPayloads(receive);
    SerialRuntime_PrepareSendPacket(send);
    gSerialRuntime.sequence++;
  }
  status = gSerialRuntime.currentMask | (gSerialRuntime.receivedMask << 8);
  if (gSerialRuntime.mode == 8)
    status |= 0x80;
  result = SerialRuntime_AddParentFlag(status);
  if (((control << 26) >> 30) > 1) // SIOCNT bits 4 and 5, the multiplayer id
    result |= 0x2000;
  return result;
}

// Builds the packet to send in sendBuffer[0]: [0] the sequence number, [1]
// the consoles not yet heard from, [2] the checksum and [4] the 24-byte
// payload. The checksum is the complement of the sum of the first 28 bytes
// as halfwords. Then restarts the send position and the timer.
void SerialRuntime_PrepareSendPacket(void *payload) {
  /* FAKEMATCH: volatile keeps the second load of sr->sendBuffer[0], as the
   * bytes do; through a plain pointer CSE reuses the first (differ 8, 48
   * instructions either way). */
  volatile struct SerialRuntime *sr;
  u32 i;
  u32 sum;

  sr = SERIAL_RUNTIME;
  {
    u8 *pkt;
    u8 cur;
    u8 recv;

    pkt = (u8 *)sr->sendBuffer[0];
    pkt[0] = sr->sequence;
    cur = sr->currentMask;
    recv = sr->receivedMask;
    sum = 0;
    pkt[1] = recv ^ cur;
    *(u16 *)(pkt + 2) = sum;
    Dma_Set(payload, pkt + 4, (DMA_ENABLE | DMA_32BIT) << 16 | 6, REG_DMA3);
  }
  {
    u16 *pkt;
    u32 v;

    pkt = sr->sendBuffer[0];
    for (i = 0; i < SERIAL_PACKET_HALFWORDS; i++) {
      v = *pkt;
      pkt++;
      sum += v;
    }
  }
  {
    u32 inv;
    struct SerialPacket *pkt;

    inv = ~sum;
    pkt = (struct SerialPacket *)sr->sendBuffer[0];
    pkt->checksum = inv;
  }

  // Stop timer 3, rewind the send position and start the timer again
  if (sr->mode != 0)
    REG_TM3CNT_H = 0;
  sr->sendIndex = -1;
  if (sr->mode != 0 && sr->transferEnabled != 0)
    REG_TM3CNT_H = 0xc0; // start, with its interrupt
}

// Swaps in the packets the interrupt handler finished, then copies the
// payload of each one whose checksum is good to `payload` (24 bytes per
// console). Returns the mask of consoles heard from this frame.
u8 SerialRuntime_CollectReceivedPayloads(void *payload) {
  u8 flags[8];
  s32 i;
  u32 k;
  u16 *packet;
  u16 *swap;
  s32 sum;

  *(u32 *)&flags[4] = 0;
  // Swap completed buffers with interrupts masked, then validate outside it.
  Io_Write16(0, &REG_IME);
  for (i = 0; i <= 3; i++) {
    swap = gSerialRuntime.pendingBuffer[i];
    gSerialRuntime.pendingBuffer[i] = gSerialRuntime.readyBuffer[i];
    gSerialRuntime.readyBuffer[i] = swap;
  }
  *(u32 *)flags = *(u32 *)gSerialRuntime.channelFlags;
  *(u32 *)gSerialRuntime.channelFlags = 0;
  Io_Write16(1, &REG_IME);
  gSerialRuntime.currentMask = 0;
  for (i = 0; i <= 1; i++) {
    packet = gSerialRuntime.pendingBuffer[i];
    sum = 0;
    for (k = 0; k < SERIAL_PACKET_HALFWORDS; k++)
      sum += packet[k];
    if (flags[i] == 1 && (s16)sum == -1) {
      Dma_Set(gSerialRuntime.pendingBuffer[i] + 2,
              (u8 *)payload + i * SERIAL_PAYLOAD_BYTES, 0x84000006, REG_DMA3);
      gSerialRuntime.currentMask |= 1 << i;
    }
    if ((s16)sum == -1)
      gSerialRuntime.pendingBuffer[i][1] = ~gSerialRuntime.pendingBuffer[i][1];
  }
  gSerialRuntime.receivedMask |= gSerialRuntime.currentMask;
  return gSerialRuntime.currentMask;
}

// The serial and timer 3 interrupt: each one moves a halfword. It sends the
// next halfword of the outgoing packet (0xfefe between packets) and stores the
// halfword received from each console; a console's fourteenth halfword
// completes its packet.
// Measured plain do/while for the two receive channels adds 50 bytes to this
// function in tbs-en, so the matching single-backedge form remains.
void SerialRuntime_HandleTransferInterrupt(void) {
  struct SerialRuntime *sendState;
  struct SerialRuntime *receiveState;
  struct SerialRuntime *tailState;
  u16 recv[4];
  volatile u32 *sioControl;
  s32 channel;

  *(u64 *)recv = *(volatile u64 *)REG_SIODATA32;
  sioControl = &REG_SIOCNT;
  sendState = &gSerialRuntime;
  sendState->isParent = (*sioControl << 25) >> 31;

  if (sendState->sendIndex == -1) {
    u32 idleWord = SERIAL_IDLE_HALFWORD;
    u16 *swap;

    ((volatile u16 *)sioControl)[1] = idleWord;
    swap = sendState->sendBuffer[1];
    sendState->sendBuffer[1] = sendState->sendBuffer[0];
    sendState->sendBuffer[0] = swap;
  } else if (sendState->sendIndex >= 0) {
    ((volatile u16 *)sioControl)[1] =
        sendState->sendBuffer[1][sendState->sendIndex];
  }
  receiveState = &gSerialRuntime;
  if (receiveState->sendIndex <= SERIAL_PACKET_HALFWORDS)
    receiveState->sendIndex++;

  channel = 0;
receive_loop:
  {
    s32 receiveIndex;
    u16 *incoming;

    if (recv[channel] == SERIAL_IDLE_HALFWORD &&
        receiveState->receiveIndex[channel] >= SERIAL_PACKET_HALFWORDS) {
      receiveState->receiveIndex[channel] = -1;
    } else {
      receiveIndex = receiveState->receiveIndex[channel];
      incoming = receiveState->incomingBuffer[channel];
      incoming[receiveIndex] = recv[channel];
      if (receiveIndex == SERIAL_PACKET_HALFWORDS - 1) {
        u16 *swap = receiveState->readyBuffer[channel];

        receiveState->readyBuffer[channel] = incoming;
        receiveState->incomingBuffer[channel] = swap;
        receiveState->channelFlags[channel] |= 1;
      }
    }
  }
  tailState = &gSerialRuntime;
  if (tailState->isParent != 0)
    tailState->channelFlags[channel] |= 2;
  if (tailState->receiveIndex[channel] <= SERIAL_PACKET_HALFWORDS)
    tailState->receiveIndex[channel]++;
  channel++;
  receiveState = tailState;
  if (channel <= 1)
    goto receive_loop;

  if (tailState->mode == 8) {
    REG_TM3CNT_H = 0;
    REG_SIOCNT16 |= 0x80;
    REG_TM3CNT_H = 0xc0;
  }
}

// Stops the per-frame exchange and removes the link interrupt handlers.
void SerialRuntime_RemoveIrqHandlers(void) {
  gSerialExchangeActive = 0;
  Runtime_SetIrqHandler(IRQ_SERIAL, 0, 0);
  Runtime_SetIrqHandler(IRQ_TIMER3, 0, 0);
}

// Waits until every status bit in `mask` is set, then returns this console's
// multiplayer id.
u32 SerialRuntime_WaitForStatusMask(s32 mask) {
  if ((mask & gLinkStatus) != mask) {
    do {
      WaitFrames(1);
    } while ((mask & gLinkStatus) != mask);
  }
  return (REG_SIOCNT << 0x1A) >> 0x1E;
}

// Starts sending `size` bytes from `source` to the partner in 20-byte blocks.
// Returns -1 when a send is already running, 0 otherwise.
s32 SerialRuntime_BeginSend(void *source, s32 size) {
  struct SerialTransferState *state = &gSerialTransfer;
  u32 savedInterruptMaster;

  if (gSerialSendSource != 0)
    return -1;
  IRQ_LOCK(savedInterruptMaster);
  state->status = 0x80;
  gSerialSendSource = (s32)source;
  gSerialSendSize = size;
  gSerialBlockSequence = 0;
  state->active = 1;
  IRQ_UNLOCK(savedInterruptMaster);
  return 0;
}

// Starts receiving the partner's blocks into `destination`. Returns -1 when
// a receive is already running, 0 otherwise.
s32 SerialRuntime_BeginReceive(void *destination) {
  struct SerialTransferState *state = &gSerialTransfer;
  u32 savedInterruptMaster;

  if (gSerialReceiveDest != 0)
    return -1;
  IRQ_LOCK(savedInterruptMaster);
  state->status = 0x81;
  gSerialReceiveDest = (s32)destination;
  gSerialReceivedSize = 0;
  state->active = 1;
  gSerialBlockSequence = 0;
  IRQ_UNLOCK(savedInterruptMaster);
  return 0;
}

// Each waits a frame at a time until `pending` is 0, giving up after 600000
// frames: the send, then the receive.
#define DEFINE_SERIAL_WAIT(name, pending)                                      \
  void name(void) {                                                            \
    u32 count = 0;                                                             \
                                                                               \
    if (pending != 0) {                                                        \
      do {                                                                     \
        WaitFrames(1);                                                         \
        count++;                                                               \
      } while (count < SERIAL_TRANSFER_TIMEOUT_FRAMES && pending != 0);        \
    }                                                                          \
  }

// Waits for the current send, up to the protocol's frame timeout.
DEFINE_SERIAL_WAIT(SerialRuntime_WaitForSend, gSerialSendSource)
// Waits for the current receive, up to the protocol's frame timeout.
DEFINE_SERIAL_WAIT(SerialRuntime_WaitForReceive, gSerialReceiveDest)

// Waits until both the send and the receive have finished, giving up after
// 600000 frames.
void SerialRuntime_WaitForTransfers(void) {
  u32 count;

  count = 0;
  while (gSerialSendSource != 0 || gSerialReceiveDest != 0) {
    WaitFrames(1);
    count++;
    if (count >= SERIAL_TRANSFER_TIMEOUT_FRAMES)
      return;
  }
}

// Returns bit 0 while a send runs and bit 1 while a receive runs.
s32 SerialRuntime_GetActiveTransfers(void) {
  s32 flags;

  flags = 0;
  if (gSerialSendSource != 0) {
    flags = 1;
  }
  if (gSerialReceiveDest != 0) {
    flags |= 2;
  }
  return flags;
}

// Clears the block transfer state.
void BattleLink_ResetTransferState(void) { SerialRuntime_ResetTransferState(); }

struct OneCell {
  u16 v;
};

// Runs each VBlank after the exchange: moves the bulk transfer on by one
// 20-byte block each way. The receiver acknowledges each block's sequence
// number; when the sender sees an acknowledgement behind its own count it
// rewinds and sends the missed blocks again.
void SerialRuntime_StepBlockTransfer(void) {
  /* FAKEMATCH: gSerialSendSize and gSerialBlockSequence are volatile
   * (serial_runtime.h), so every access here is a load or store of its own.
   * Measured on tbs-en: without the first qualifier differ 2, 257 instructions
   * either way; without the second differ 258, 249 for 257. */
  struct SerialTransferState *peer;
  u32 id;
  struct SerialTransferState *own;
  u32 gap;
  /* FAKEMATCH: the halfword record is a HImode 1, which Thumb movhi loads from
   * the literal pool into r8 at entry, as native; it is not protocol state.
   * 2026-10-13: the constant 1 at both stores differs 55 (252 instructions
   * for 257), as does a u16 local, which is promoted to SImode. */
  struct OneCell one;

  id = ~((REG_SIOCNT << 26) >> 30) & 1;
  one.v = 1;
  own = &gSerialTransfer;
  peer = (struct SerialTransferState *)gSerialPeerPayloads[id];
  if ((gLinkStatus & 3) != 3)
    return;
  // Receiving advances only when the peer presents the expected sequence.
  {
    s32 *receive = &gSerialReceiveDest;
    s32 dest = *receive;
    if (dest != 0) {
      if (own->flags == 1 && (u8)(peer->peerFlags - 1) <= 1) {
        if (peer->active == (gSerialBlockSequence & 0x7f)) {
          own->active = 0;
          switch (peer->peerFlags) {
          case 1:
            Dma_Set(peer->payload, (void *)dest, 0x84000005, REG_DMA3);
            *receive += SERIAL_BLOCK_BYTES;
            gSerialReceivedSize += SERIAL_BLOCK_BYTES;
            own->status = (own->status + 1) | 0x80;
            break;
          case 2:
            Dma_Set(peer->payload, (void *)dest, 0x84000005, REG_DMA3);
            gSerialReceivedSize += SERIAL_BLOCK_BYTES;
            own->flags = 2;
            own->status = 0;
            own->active = one.v;
            break;
          }
          gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
        } else if (gSerialBlockSequence & 0x80) {
          if (own->active & 0x80) {
            own->active = one.v;
          } else if (own->active == 1) {
            own->active = 0;
            gSerialBlockSequence &= 0x7f;
          }
        } else {
          own->active = gSerialBlockSequence | 0x80;
          gSerialBlockSequence |= 0x80;
        }
      } else {
        own->active = 0;
      }
    }
  }
  {
    s32 *send = &gSerialSendSource;
    s32 src = *send;
    // A peer behind our sequence requests a rewind before the next block.
    if (src != 0) {
      if (peer->flags == 1) {
        if (peer->active & 0x80) {
          gap = (gSerialBlockSequence - peer->active) & 0x7f;
          *send = src - gap * SERIAL_BLOCK_BYTES;
          gSerialSendSize += gap * SERIAL_BLOCK_BYTES;
          gSerialBlockSequence -= gap;
          gSerialBlockSequence &= 0x7f;
        }
        if (gSerialSendSize != 0) {
          Dma_Set((void *)*send, own->payload, 0x84000005, REG_DMA3);
          gSerialSendSize += -SERIAL_BLOCK_BYTES;
          if (gSerialSendSize != 0) {
            own->peerFlags = 1;
            own->active = gSerialBlockSequence & 0x7f;
            *send += SERIAL_BLOCK_BYTES;
          } else {
            own->peerFlags = 2;
            own->active = gSerialBlockSequence & 0x7f;
            *send += SERIAL_BLOCK_BYTES;
          }
          gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
        }
      }
      if (own->peerFlags == 2 && peer->flags == 2) {
        gSerialSendSource = 0;
        own->peerFlags = 0;
        own->active = 1;
      }
    }
  }
  if (own->flags == 2) {
    if (peer->peerFlags != 2) {
      gSerialReceiveDest = 0;
      own->flags = 0;
    }
  } else {
    own->flags = 0;
    if (gSerialReceiveDest != 0)
      own->flags = 1;
  }
}

extern const u8 Rom_Start[];

// An empty routine the link test calls to waste time.
void RuntimeWait_BusyLoopTick(void) {}

// A link cable test left in the game: it shows a row of tiles, receives
// blocks into the map cell buffer and shows them as tiles, and sends a part
// of the ROM when A or B is held. It never returns.
// Measured outer receive loop with an inner completion break changes 9 text
// bytes in tbs-en: the key-address load is hoisted and the restart retargeted.
void SerialTest_Run(void) {
  u16 *tile;
  s32 value;
  s32 n;
  s32 tick;

  AudioCommand_PlayFar(3);
  SerialRuntime_Initialize();
  n = 19;
  tile = (u16 *)BG_SCREEN_BLOCK(4) + 16 * 32 + 19;
  value = (s16)0xf093;
  do {
    n--;
    *tile = value;
    tile--;
    value--;
  } while (n >= 0);
  Cpu_Fill32(0, gMapCellBuffer, 0x400);
  SerialRuntime_WaitForStatusMask(3);
restart:
  SerialRuntime_BeginReceive(gMapCellBuffer);
  for (;;) {
    if (gKeysHeld & KEY_A)
      SerialRuntime_BeginSend((void *)Cart_Restart, 0x280);
    if (gKeysHeld & KEY_B)
      SerialRuntime_BeginSend((void *)(Rom_Start + 0x1000), 0x280);
    if (gKeysHeld & KEY_START) {
      tick = 9999;
      do {
        tick--;
        RuntimeWait_BusyLoopTick();
      } while (tick >= 0);
    }
    if (gSerialReceiveDest == 0) {
      Dma_Set(gMapCellBuffer, (void *)0x06001000, 0x840000a0, REG_DMA3);
      goto restart;
    }
    WaitFrames(1);
  }
}

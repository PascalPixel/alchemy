// Part of runtime.c, the runtime object, compiled only through it.
// Interrupt handlers, the frame wait, key repeat and the VBlank handler.

#include "../runtime_internal.h"

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
            (DMA_ENABLE | DMA_32BIT) << 16 | (gSavedStackSize >> 2), REG_DMA3);
    CAMELOT_ASM("the stack switch, a write to sp no C can express")
    __asm__ volatile("mov sp, %0" : : "r"(top) : "sp", "memory");
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
        Io_Write16(RGB(31, 31, 31), &PLTT_BACKDROP);
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
       * frame and removes an extra copy through ip. As an output only ("=r"),
       * the frame is 4 bytes for 8 and the registers shift (differ 67 in
       * tbs-en). */
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
                (DMA_ENABLE | DMA_32BIT) << 16 | (*nextSize >> 2), REG_DMA3);
      }
      while (DMA3->control & (DMA_ENABLE << 16))
        ;
      gSavedStackSize = 0;
    }
  }
}

// Frames a key is held before it first repeats, and between later repeats
#define INPUT_KEY_REPEAT_FIRST 19
#define INPUT_KEY_REPEAT_NEXT 6

// Restarts the key repeat delay, so held keys wait the full delay again.
void Input_ResetKeyRepeat(void) { gKeyRepeatTimer = INPUT_KEY_REPEAT_FIRST; }

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
      gKeyRepeatTimer = INPUT_KEY_REPEAT_NEXT;
    else
      gKeyRepeatTimer = INPUT_KEY_REPEAT_FIRST;
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
    gKeyRepeatTimer = INPUT_KEY_REPEAT_FIRST;
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

#undef VBlankIntrWait
#undef INPUT_KEY_REPEAT_FIRST
#undef INPUT_KEY_REPEAT_NEXT
#undef SOUND_PRESET_COUNT
#undef SOUND_PRESET_SIZE

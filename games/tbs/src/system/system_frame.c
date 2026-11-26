/// @file
/// Interrupt handlers, the frame wait, key repeat and the VBlank handler.
// Part of system.c.

#include "system_internal.h"
#include "system/asm.h"

/// The word a soft reset leaves in gCartridgeResetMarker: a date, 4 July 1967,
/// its digits packed four bits each.
#define CARTRIDGE_RESET_MARK 0x19670704

/// DISPSTAT's low byte: its flags and interrupt enables, without the VCount
/// line above them.
#define DISPSTAT_FLAGS 0xff

/// The lines REG_VCOUNT counts in VBlank, after the DISPLAY_HEIGHT drawn: it
/// runs to DISPLAY_TOTAL_LINES - 1.
#define VBLANK_LINES (DISPLAY_TOTAL_LINES - DISPLAY_HEIGHT)

/// Every bit of a key word, as KEYINPUT's 16 bits.
#define KEY_WORD_BITS 0xffff

/// Restarts the game from the cartridge entry point. The marker word it leaves
/// in IWRAM is cleared again by System_Initialize; nothing else reads it.
static __inline__ void System_SoftReset(void) {
  s32 (*reset)(void) = (s32 (*)(void))Cart_Start;

  gCartridgeResetMarker = CARTRIDGE_RESET_MARK;
  Io_Write16(0, &REG_IME);
  reset();
}

/// Halts the CPU (the BIOS Halt) until the VBlank handler has run once.
#define VBlankIntrWait()                                                       \
  {                                                                            \
    gVBlankDone &= ~1;                                                         \
    do {                                                                       \
      BIOS_HALT();                                                             \
    } while (!(gVBlankDone & 1));                                              \
  }

/// Installs `handler` for interrupt `irq` and enables that interrupt, or with a
/// null handler disables it and installs the do-nothing handler.
void Irq_SetHandler(u32 irq, s32 vcount, InterruptHandler handler) {
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
      // The display interrupts also need their DISPSTAT enable
      u32 enable = DISPSTAT_VBLANK_INTR << irq;
      u32 keep = ~enable;
      u32 stat;

      if (irq == IRQ_VCOUNT) {
        enable |= DISPSTAT_VCOUNT_LINE(vcount);
        // The old line goes too.
        keep &= DISPSTAT_FLAGS;
      }
      stat = REG_DISPSTAT & keep;
      if (handler != 0)
        stat |= enable;
      REG_DISPSTAT = stat;
    }
    if (handler != 0)
      gIrqHandlers[irq] = handler;
    else
      gIrqHandlers[irq] = Irq_Ignore;
    IRQ_UNLOCK(ime);
  }
}

/// Durations used by the sleep transitions: the first two in frames, the last
/// two in VBlanks, since the sleep waits on VBlankIntrWait alone.
enum {
  SLEEP_IDLE_FRAMES = 3 * 60 * 60, ///< three minutes with no key held
  SLEEP_COMBO_FRAMES = 3 * 60,     ///< three seconds holding only L and R
  SLEEP_WHITE_VBLANKS = 60,        ///< the white screen shown before the Stop
  SLEEP_WAKE_VBLANKS = 10          ///< waited after waking, screen restored
};

/// Frames the debug CPU-load display holds a peak.
enum { CPU_LOAD_PEAK_FRAMES = 30 };

/// The game's frame loop: runs `frames` frames and returns.
void wait(s32 frames) {
  u8 *sp;
  u32 frameIndex;
  u8 *base;
  u32 line;
  s32 delayVBlank;
  s16 savedDisplayControl;
  s16 savedBackdropColor;

  STACK_POINTER_GET(sp);

  // Save the caller's stack and continue at the frame-wait stack top
  base = sp;
  if (base < Ram_FrameWaitStackTop) {
    u8 *top = Ram_FrameWaitStackTop;

    gSavedStackSize = top - base;
    Dma_Set(base, gSavedStack, DMA_COPY_WORDS(gSavedStackSize >> 2), REG_DMA3);
    STACK_POINTER_SET(top);
  }
  for (frameIndex = 0; frameIndex < frames; frameIndex++) {
    // Run the frame pass's tasks and build this frame's sprites
    gTasksRunning = 1;
    Task_RunPass(TASK_PASS_FRAME);
    gTasksRunning = 0;
    Oam_BuildBuffer(HeapSlot_Alloc(HEAP_SLOT_FRAME_SCRATCH, 1024));
    gFrameRenderPending = 1;
    if (gCpuLoadDisplayEnabled) {
      // CPU load: the scanline the frame's work ended on, counted from the
      // last VBlank, plus 256 for each VBlank it overran; the peak is held
      // for 30 frames
      // The meter reads the line through a plain pointer, where
      // ScreenTransition_UpdateScanline reads the volatile register.
      line = *(u16 *)&REG_VCOUNT;
      if (line >= DISPLAY_HEIGHT)
        line -= DISPLAY_HEIGHT;
      else
        line += VBLANK_LINES;
      line += (gFrameVBlanks - 1) << 8;
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
        VBlankIntrWait();
        Input_UpdateKeyRepeatAndDirection();
        if (gResetRequested) {
          gResetRequested = 0;
          System_SoftReset();
        }
      }
    }
    // Wait for VBlank, then start the next frame
    gLastFrameVBlanks = gFrameVBlanks;
    gFrameVBlanks = 0;
    VBlankIntrWait();
    HeapSlot_Release(HEAP_SLOT_FRAME_SCRATCH);
    Oam_ResetFrame();
    gFrameCount++;
    gPlayTimeFrames++;
    Input_UpdateKeyRepeatAndDirection();
    if (gSerialExchangeActive) {
      Serial_PollStatus();
      if (gSerialState.mode)
        gSerialState.transferEnabled = 1;
    }
    // Sleep: after a second of white screen, Stop the CPU until L and R wake
    // it, then restore the screen. A request above 1 counts down first.
    if (gSleepRequested && gSleepDisabled == 0) {
      savedDisplayControl = REG_DISPCNT;
      savedBackdropColor = PLTT_BACKDROP;
      if (gSleepRequested == 1) {
        Io_Write16(0, &REG_DISPCNT);
        Io_Write16(RGB_WHITE, &PLTT_BACKDROP);
        for (delayVBlank = 0; delayVBlank < SLEEP_WHITE_VBLANKS;
             delayVBlank++) {
          VBlankIntrWait();
        }
        gSleepActive = 1;
        Io_Write16(KEYCNT_SHOULDERS_WAKE, &REG_KEYCNT);
        Bios_SoundBiasOff();
        BIOS_STOP();
        Bios_SoundBiasOn();
        Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
        gSleepActive = 0;
        Io_Write16((u16)savedDisplayControl, &REG_DISPCNT);
        Io_Write16((u16)savedBackdropColor, &PLTT_BACKDROP);
        for (delayVBlank = 0; delayVBlank < SLEEP_WAKE_VBLANKS; delayVBlank++) {
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

    if (*size != 0) {
      /* FAKEMATCH(dead): updates of base that cancel out, under a test of base
       * against size, keep the old base live up to the read. */
      if (base != (u8 *)size) {
        base++;
        base--;
      }
      STACK_POINTER_GET(base);
      /* FAKEMATCH(dead): updates of size by the new base that cancel out,
       * before the size is read again and the copy's size by name. */
      size += (u32)base;
      size -= (u32)base;
      base -= *size;
      sp = base;
      STACK_POINTER_SET(sp);
      Dma_Set(gSavedStack, base, DMA_COPY_WORDS(gSavedStackSize >> 2),
              REG_DMA3);
      while (DMA3->control & DMA_BUSY)
        ;
      gSavedStackSize = 0;
    }
  }
}

/// Frames a key is held before it first repeats, and between later repeats.
#define INPUT_KEY_REPEAT_FIRST 19
#define INPUT_KEY_REPEAT_NEXT 6

/// Restarts the key repeat delay, so held keys wait the full delay again.
void Input_ResetKeyRepeat(void) { gKeyRepeatTimer = INPUT_KEY_REPEAT_FIRST; }

/// Works out this frame's repeating and newly pressed keys, once a frame after
/// VBlank (wait()). Held keys repeat after INPUT_KEY_REPEAT_FIRST frames, then
/// every INPUT_KEY_REPEAT_NEXT; gKeysRepeatDirected is that repeat with at
/// most one D-pad direction, the newer of two held and none of four.
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
      mask = KEY_WORD_BITS & ~KEYS_DPAD;
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
      gKeysRepeatDirected &= gKeysDirection ^ KEY_WORD_BITS;
      break;
    case 3:
      if (gKeysDirection & KEYS_HORIZONTAL)
        axis = KEYS_HORIZONTAL;
      if (gKeysDirection & KEYS_VERTICAL)
        axis = KEYS_VERTICAL;
      mask = KEY_WORD_BITS ^ axis;
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

#define SOUND_REVERB_CODE_COUNT 5  ///< reverb codes in IwramSoundReverbCodes
#define SOUND_REVERB_CODE_SIZE 152 ///< bytes in each reverb code

/// The VBlank interrupt handler, installed by System_Initialize.
void System_VBlankHandler(void) {
  u32 keys;
  void (*hook)(void);

  // The display writes come first, while the blank lasts: OAM can be written
  // only in a blank, and a palette or scroll changed while lines are drawn
  // shows half changed.
  Dma0_Stop();
  if (gSerialExchangeActive != 0) {
    gLinkStatus =
        Serial_ExchangePayloads((void *)&gSerialTransfer, gSerialPeerPayloads);
    Serial_StepBlockTransfer();
  }
  Sound_UpdateBgmRampsFar();
  ScreenFade_Update();
  if (gFrameRenderPending != 0) {
    if (gOamCopyEnabled != 0)
      Dma_Set(((union HeapState *)gHeapSlots)->slots[HEAP_SLOT_FRAME_SCRATCH],
              OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
    Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
    IoWriteQueue_FlushPending();
    gFrameRenderPending = 0;
  }
  // The hook runs once: it is cleared before it is called
  hook = gVBlankCallback;
  if (hook != 0) {
    gVBlankCallback = 0;
    hook();
  }
  Task_RunPass(TASK_PASS_VBLANK);

  // Read the keypad (its bits are 0 while pressed).
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
  } else if (gKeysHeld & (gKeysHeldAtVBlank ^ KEY_WORD_BITS)) {
    gKeyRepeatTimer = -1;
    gKeysRepeat = keys;
  } else if (gKeyRepeatTimer > 0) {
    gKeyRepeatTimer--;
  }
  gKeysHeldAtVBlank = keys;
  gVBlankCount++;
  gFrameVBlanks++;
  gVBlankDone = 1;
  Mixer_Enter();
}

/// Copies the mixer code for reverb code `code` (enum SoundReverbCode; anything
/// else loads SOUND_REVERB_MONO) from ROM into IwramSoundReverbCode.
s32 Sound_LoadReverbCode(s32 code) {
  s32 index;

  index = code;
  if (code < 0 || code >= SOUND_REVERB_CODE_COUNT) {
    index = 0;
  }
  Dma_Set(IwramSoundReverbCodes + index * SOUND_REVERB_CODE_SIZE,
          IwramSoundReverbCode, DMA_COPY_WORDS(SOUND_REVERB_CODE_SIZE / 4),
          REG_DMA3);
  return 0;
}

/// Unused: two empty routines after the reverb loader, which nothing calls.
UNUSED void Sound_ReservedNoOpA(void) {}
UNUSED void Sound_ReservedNoOpB(void) {}

/// The keypad interrupt, from the handler table: A+B+Select+Start was pressed,
/// so it sets gResetRequested for the frame loop and disarms the interrupt.
void Input_HandleKeyInterrupt(void) {
  if (gSleepActive == 0) {
    Io_Write16(KEYCNT_DISARMED, &REG_KEYCNT);
    gResetRequested = 1;
  }
}

#undef VBlankIntrWait
#undef CARTRIDGE_RESET_MARK
#undef DISPSTAT_FLAGS
#undef VBLANK_LINES
#undef KEY_WORD_BITS
#undef INPUT_KEY_REPEAT_FIRST
#undef INPUT_KEY_REPEAT_NEXT
#undef SOUND_REVERB_CODE_COUNT
#undef SOUND_REVERB_CODE_SIZE

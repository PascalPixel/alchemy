/// @file
/// Interrupt handlers, the frame wait, key repeat and the VBlank handler.
// Part of system.c.

#include "system_internal.h"
#include "gamepak/far_call/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/ram_buffer.h"

/// The word a soft reset leaves in gCartridgeResetMarker: a date, 4 July 1967,
/// its digits packed four bits each.
#define CARTRIDGE_RESET_MARK 0x19670704

/// DISPSTAT's low byte: its flags and interrupt enables, without the VCount
/// line above them.
#define DISPSTAT_FLAGS 0xff

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

/// Waits for the next VBlank; outside the sound test it first runs the music
/// ramps and the sound driver's VBlank update, and mixes this VBlank's sound
/// when the driver is free.
void System_WaitForFrameInterrupt(void) {
  u32 saved;
  u8 *sound;

  if (gSoundVBlankState.soundTestActive) {
    gVBlankDone &= ~1;
    for (;;)
      if (gVBlankDone & 1)
        return;
  } else {
    IRQ_LOCK(saved);
    gVBlankDone &= ~1;
    gSoundVBlankState.lastVBlank = gVBlankCount;
    gSoundVBlankState.updating = 1;
    sound = gSoundInfoPointer;
    if (SOUND_INFO_BYTE(sound, transferCountdown) <= 1)
      gSoundVBlankState.transferCountdown =
          SOUND_INFO_BYTE(sound, transferPeriod);
    else
      gSoundVBlankState.transferCountdown =
          SOUND_INFO_BYTE(sound, transferCountdown) - 1;
    REG_IME = saved;
    Sound_UpdateBgmRampsFar();
    if (SoundDriver_EnterVBlankUpdate() == 0) {
      void (*render)(s32) = IwramSoundRenderVBlank;

      render(8);
    }
    gSoundVBlankState.updating = 0;
    while (!(gVBlankDone & 1))
      ;
  }
}

/// Durations of the sleep: the first in frames, the others in VBlanks, since
/// the sleep waits on System_WaitForFrameInterrupt alone.
enum {
  SLEEP_IDLE_FRAMES = 3 * 60 * 60, ///< three minutes with no key held
  SLEEP_WHITE_VBLANKS = 10,        ///< the white screen shown before the Stop
  SLEEP_WAKE_VBLANKS = 10          ///< waited after waking, before the screen
};

/// The lines REG_VCOUNT counts in VBlank, after the DISPLAY_HEIGHT drawn: it
/// runs to DISPLAY_TOTAL_LINES - 1.
#define VBLANK_LINES (DISPLAY_TOTAL_LINES - DISPLAY_HEIGHT)

/// Restarts the cartridge: marks gCartridgeResetMarker, stops direct sound and
/// jumps to the cartridge's start with interrupts off. `entry` is the caller's
/// local for the start address.
#define System_Reset(entry)                                                    \
  {                                                                            \
    gCartridgeResetMarker = CARTRIDGE_RESET_MARK;                              \
    Sound_SuspendDirectSoundFar();                                             \
    (entry) = Cart_Start;                                                      \
    Io_Write16(0, &REG_IME);                                                   \
    ASM_CART_JUMP(entry);                                                      \
  }

#include "system/asm.h"

/// Stops the CPU and the screen until an enabled interrupt wakes them (the BIOS
/// Stop call): the sleep that L, R and Select end.
static __inline__ void Bios_Stop(void) { ASM_BIOS_STOP(); }

/// Frames the debug CPU-load display holds a peak.
enum { CPU_LOAD_PEAK_FRAMES = 30 };

/// The game's frame loop: runs `frames` frames and returns.
void wait(s32 frames) {
  u32 i;
  u32 line;
  s32 j;
  s16 display;
  s16 backdrop;
  // The start address, set at each restart.
  u32 entry;

  for (i = 0; i < (u32)frames; i++) {
    gTasksRunning = 1;
    Task_RunPass(TASK_PASS_FRAME);
    gTasksRunning = 0;
    if (gOamCopyEnabled) {
      Render_BuildOamList(HeapSlot_Alloc(HEAP_SLOT_FRAME_SCRATCH, OAM_SIZE));
    } else {
      gOamUsage.used = 0;
      gOamUsage.limit = OAM_SIZE / 4;
    }
    gFrameRenderPending = 1;
    if (gCpuLoadDisplayEnabled) {
      // CPU load: the line the frame's work ended on, counted from the last
      // VBlank, plus 256 for each VBlank it overran; the peak is held for
      // CPU_LOAD_PEAK_FRAMES.
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
    // Idle frames and the L+R+Select combination request sleep.
    if (gSleepDisabled == 0) {
      if (gIdleSleepEnabled) {
        if (gInput.held)
          gIdleFrames = 0;
        else {
          gIdleFrames++;
          if (gIdleFrames > SLEEP_IDLE_FRAMES)
            gSleepRequested = 1;
        }
      }
      if (gInput.held == (KEYS_SHOULDERS | KEY_SELECT))
        gSleepRequested = 1;
    }
    // Debug builds: Start+Select pauses the game; while paused, A, B, Select
    // or the D-pad advances one frame and Start resumes.
    if (gDebugMode) {
      for (;;) {
        if (gDebugPaused) {
          if (gInput.repeat & (KEY_A | KEY_B | KEY_SELECT))
            break;
          if (gInput.held & KEYS_DPAD)
            break;
          if (gInput.repeat & KEY_START) {
            gDebugPaused = 0;
            break;
          }
        } else {
          if (gInput.held != (KEY_START | KEY_SELECT))
            break;
          gDebugPaused = 1;
        }
        System_WaitForFrameInterrupt();
        Input_UpdateKeyRepeatAndDirection();
        if (gResetRequested) {
          gResetRequested = 0;
          System_Reset(entry);
        }
      }
    }
    gLastFrameVBlanks = gFrameVBlanks;
    gFrameVBlanks = 0;
    System_WaitForFrameInterrupt();
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
    // Sleep: after a white screen and the keys' release, Stop the CPU until L,
    // R and Select wake it, then restore the screen once they are released. A
    // request above 1 counts down first.
    if (gSleepRequested && gSleepDisabled == 0) {
      display = REG_DISPCNT;
      backdrop = PLTT_BACKDROP;
      if (gSleepRequested == 1) {
        Io_Write16(0, &REG_DISPCNT);
        Io_Write16(RGB_WHITE, &PLTT_BACKDROP);
        for (j = 0; j < SLEEP_WHITE_VBLANKS; j++)
          System_WaitForFrameInterrupt();
        while (gInput.held)
          System_WaitForFrameInterrupt();
        gSleepActive = 1;
        Io_Write16(KEYCNT_SLEEP_WAKE, &REG_KEYCNT);
        Bios_SoundBiasOff();
        Bios_Stop();
        Bios_SoundBiasOn();
        Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
        Io_Write16(0, &gSleepActive);
        for (j = 0; j < SLEEP_WAKE_VBLANKS; j++)
          System_WaitForFrameInterrupt();
        while (gInput.held)
          System_WaitForFrameInterrupt();
        Io_Write16((u16)display, &REG_DISPCNT);
        Io_Write16((u16)backdrop, &PLTT_BACKDROP);
        gSleepRequested = 0;
        gIdleFrames = 0;
      } else
        gSleepRequested--;
    }
    if (gResetRequested) {
      gResetRequested = 0;
      System_Reset(entry);
    }
  }
}

/// Frames a key is held before it first repeats, and between later repeats.
#define INPUT_KEY_REPEAT_FIRST 19
#define INPUT_KEY_REPEAT_NEXT 6

/// Every bit of a key word, as KEYINPUT's 16 bits.
#define KEY_WORD_BITS 0xffff

/// Restarts the key repeat delay, so held keys wait the full delay again.
void Input_ResetKeyRepeat(void) { gInput.repeatTimer = INPUT_KEY_REPEAT_FIRST; }

/// Turns the keys the VBlank handler read into repeats, once a frame after
/// VBlank. When the repeat timer has run out, the held keys repeat into
/// gInput.repeat.
/// gInput.direction is the same keys with at most one D-pad direction: of
/// several held directions it keeps the one added last (previousDirection
/// remembers it), and with all four it keeps none. Last, the keys newly held
/// since the previous frame go to pressed, and previousHeld takes held.
void Input_UpdateKeyRepeatAndDirection(void) {
  u32 held;
  u32 keys;
  u32 axis = 0;
  u32 directionCount;
  u32 mask;
  s32 delay;

  held = gInput.held;
  delay = gInput.repeatTimer;
  if (delay <= 0) {
    keys = held;
    gInput.repeat = held;
    if (delay == 0)
      gInput.repeatTimer = INPUT_KEY_REPEAT_NEXT;
    else
      gInput.repeatTimer = INPUT_KEY_REPEAT_FIRST;
  } else {
    keys = 0;
    gInput.repeat = 0;
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

    gInput.direction = keys;
    switch (directionCount) {
    default:
      gInput.previousDirection = KEYS_HORIZONTAL;
      mask = KEY_WORD_BITS & ~KEYS_DPAD;
      gInput.direction &= mask;
      break;
    case 0:
      gInput.previousDirection = KEYS_HORIZONTAL;
      break;
    case 1:
      gInput.previousDirection = keys & KEYS_DPAD;
      break;
    case 2:
      if ((gInput.previousDirection & gInput.direction) == 0)
        gInput.previousDirection = KEYS_HORIZONTAL;
      gInput.direction &= gInput.previousDirection ^ KEY_WORD_BITS;
      break;
    case 3:
      if (gInput.previousDirection & KEYS_HORIZONTAL)
        axis = KEYS_HORIZONTAL;
      if (gInput.previousDirection & KEYS_VERTICAL)
        axis = KEYS_VERTICAL;
      mask = KEY_WORD_BITS ^ axis;
      gInput.previousDirection = keys & mask;
      gInput.direction &= mask;
      break;
    }
  } else {
    gInput.direction = keys;
  }

  gInput.pressed = held & ~gInput.previousHeld;
  Io_Write16(held, &gInput.previousHeld);
}

#include "system/asm.h"

/// Copies 16 bytes from `source` to `destination` with one four-register load
/// and store.
static __inline__ void Copy16Bytes(const void *source,
                                   volatile void *destination) {
  ASM_REGISTER(const void *src, "r0") = source;
  ASM_REGISTER(volatile void *dst, "r1") = destination;
  ASM_COPY_WORDS_4(src, dst);
}

/// The VBlank interrupt handler, installed by System_Initialize.
void System_VBlankHandler(void) {
  u32 keys;
  u32 vblankCount;
  void (*hook)(void);

  // The display writes come first, while the blank lasts: OAM can be written
  // only in a blank, and a palette or scroll changed while lines are drawn
  // shows half changed.
  DMA0_STOP();
  if (gSerialExchangeActive != 0) {
    gLinkStatus =
        Serial_ExchangePayloads((void *)&gSerialTransfer, gSerialPeerPayloads);
    Serial_StepBlockTransfer();
  }
  SoundDriver_VSyncRefresh();
  if (gScreenFadeVBlanks != 0)
    ScreenFade_Update();
  if (gFrameRenderPending != 0) {
    gFrameRenderPending = 0;
    if (gOamUsage.used != 0)
      Dma_Set(Ram_HeapSlots->frameScratch, OAM,
              DMA_COPY_WORDS(0) + gOamUsage.used, REG_DMA3);
    Copy16Bytes(gBgScroll, REG_BG0HOFS);
    IoWriteQueue_FlushPending();
  }
  // The hook runs once: it is cleared before it is called.
  hook = gVBlankCallback;
  if (hook != 0) {
    gVBlankCallback = 0;
    hook();
  }
  Task_RunPassBeforeFrameTasks(TASK_PASS_VBLANK);

  // Read the keypad (its bits are 0 while pressed).
  keys = REG_KEYINPUT ^ KEYS_MASK;
  {
    // volatile: the code this interrupt broke into reads the same state.
    volatile struct InputState *input = &gInput;
    u32 pressed = keys & ~input->held;

    input->pressed = pressed;
    input->pendingPressed |= pressed;
    input->held = keys;
    if (keys == 0) {
      input->repeatTimer = INPUT_KEY_REPEAT_FIRST;
      input->repeat = keys;
    } else if (input->held & (input->previousVblankHeld ^ KEY_WORD_BITS)) {
      input->repeatTimer = -1;
      input->repeat = keys;
    } else if (input->repeatTimer > 0) {
      input->repeatTimer--;
    }
  }
  {
    // volatile: the frame loop waits on the count this interrupt advances.
    volatile u32 *count;
    volatile struct SoundVBlankState *sound;

    count = &gVBlankCount;
    ((volatile struct InputState *)&gInput)->previousVblankHeld = keys;
    sound = &gSoundVBlankState;
    vblankCount = *count;
    if (sound->lastVBlank != (u8)vblankCount && sound->updating == 0) {
      sound->transferCountdown =
          SOUND_INFO_BYTE(gSoundInfoPointer, transferCountdown);
      Sound_UpdateBgmRampsFar();
      if (SoundDriver_EnterVBlankUpdate() == 0) {
        void (*render)(s32) = IwramSoundRenderVBlank;

        render(8);
      }
      sound->lastVBlank = vblankCount;
    }
    *count = vblankCount + 1;
  }
  Io_Write32(gFrameVBlanks + 1, &gFrameVBlanks);
  gVBlankDone = 1;
}

#define SOUND_REVERB_CODE_COUNT 5 ///< the codes of enum SoundReverbCode

/// Where The Broken Seal copies the mixer code for reverb code `code` (enum
/// SoundReverbCode) into IWRAM, The Lost Age keeps only the range check and
/// returns 0, so its callers (the battle, the lobby, the sound test and the
/// debug menu) change nothing.
s32 Sound_LoadReverbCode(s32 code) {
  if ((u32)code > SOUND_REVERB_CODE_COUNT - 1)
    code = 0;
  return 0;
}

/// Empty: nothing calls it or its neighbour.
UNUSED void Sound_ReservedNoOpA(void) {}

/// Empty, as Sound_ReservedNoOpA.
UNUSED void Sound_ReservedNoOpB(void) {}

/// The keypad interrupt, from the handler table: A+B+Select+Start was pressed,
/// so it sets gResetRequested for the frame loop and disarms the interrupt.
void Input_HandleKeyInterrupt(void) {
  if (gSleepActive == 0) {
    Io_Write16(KEYCNT_DISARMED, &REG_KEYCNT);
    gResetRequested = 1;
  }
}

#undef CARTRIDGE_RESET_MARK
#undef VBLANK_LINES
#undef DISPSTAT_FLAGS
#undef KEY_WORD_BITS
#undef INPUT_KEY_REPEAT_FIRST
#undef INPUT_KEY_REPEAT_NEXT
#undef SOUND_REVERB_CODE_COUNT

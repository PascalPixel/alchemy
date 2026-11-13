/// @file
/// Interrupt handlers, the frame wait, key repeat and the VBlank handler.
// Part of system.c.

#include "../system_internal.h"
#include "gamepak/far_call/sound.h"
#include "ram_buffer.h"

void Irq_SetHandler(u32 irq, s32 vcount, InterruptHandler handler) {
  if (irq <= 13) {
    u32 ime;
    u32 bit;
    u32 ie;

    IRQ_LOCK(ime);
    bit = 1 << irq;
    ie = REG_IE & ~bit;
    if (handler != 0)
      ie |= bit;
    REG_IE = ie;
    if (irq <= 2) {
      u32 enable = 8 << irq;
      u32 keep = ~enable;
      u32 stat;

      if (irq == 2) {
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
      gIrqHandlers[irq] = Irq_Ignore;
    IRQ_UNLOCK(ime);
  }
}

#include "sound_driver_symbols.h"

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
    sound = gData_03007ff0;
    if (sound[4] <= 1)
      gSoundVBlankState.transferCountdown = sound[11];
    else
      gSoundVBlankState.transferCountdown = sound[4] - 1;
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

#define SLEEP_IDLE_FRAMES 10800
#define RGB_WHITE 0x7fff

static __inline__ void System_Reset(void) {
  /* FAKEMATCH(pin): entry held in r4, where the restart's bx takes it; as a
   * plain local, 83 of 326 instructions of wait differ in tla-en. */
  register u32 entry __asm__("r4");
  gCartridgeResetMarker = 0x19670704;
  m4aSoundVSyncOffFar();
  entry = Cart_Restart;
  Io_Write16(0, &REG_IME);
  CAMELOT_ASM("the cartridge restart transfers control without returning.")
  __asm__ volatile("bx %0" : : "r"(entry));
}

static __inline__ void Bios_Stop(void) {
  CAMELOT_ASM("BIOS Stop has no C instruction.")
  __asm__ volatile("swi 3");
}

void wait(s32 frames) {
  u32 i;
  u32 line;
  s32 j;
  s16 display;
  s16 backdrop;

  for (i = 0; i < (u32)frames; i++) {
    gTasksRunning = 1;
    Task_RunPass(TASK_PASS_VBLANK);
    gTasksRunning = 0;
    if (gRenderOamEnabled) {
      Render_BuildOamList(HeapSlot_Alloc(80, 0x400));
    } else {
      gOamUsage.used = 0;
      gOamUsage.limit = 256;
    }
    gFrameRenderPending = 1;
    if (gCpuLoadDisplayEnabled) {
      line = *(u16 *)&REG_VCOUNT;
      if (line >= 160)
        line -= 160;
      else
        line += 68;
      line += (gFrameVBlanks - 1) << 8;
      if (gCpuLoadTimer == 0)
        gCpuLoadPeak = 0;
      else
        gCpuLoadTimer--;
      if (gCpuLoadPeak < line) {
        gCpuLoadPeak = line;
        gCpuLoadTimer = 30;
      }
    }
    if (gSleepDisabled == 0) {
      if (gAutoSleepEnabled) {
        if (gInput.held)
          gIdleFrameCount = 0;
        else {
          gIdleFrameCount++;
          if (gIdleFrameCount > SLEEP_IDLE_FRAMES)
            gSleepRequested = 1;
        }
      }
      if (gInput.held == (KEYS_SHOULDERS | KEY_SELECT))
        gSleepRequested = 1;
    }
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
          System_Reset();
        }
      }
    }
    gLastFrameVBlanks = gFrameVBlanks;
    gFrameVBlanks = 0;
    System_WaitForFrameInterrupt();
    HeapSlot_Release(80);
    Render_ResetFrame();
    gFrameCount++;
    gFrameWaitCount++;
    Input_UpdateKeyRepeatAndDirection();
    if (gSerialExchangeActive) {
      Serial_PollStatus();
      if (gSerialWork.mode)
        gSerialWork.transferEnabled = 1;
    }
    if (gSleepRequested && gSleepDisabled == 0) {
      display = REG_DISPCNT;
      backdrop = PLTT_BACKDROP;
      if (gSleepRequested == 1) {
        Io_Write16(0, &REG_DISPCNT);
        Io_Write16(RGB_WHITE, &PLTT_BACKDROP);
        for (j = 9; j >= 0; j--)
          System_WaitForFrameInterrupt();
        while (gInput.held)
          System_WaitForFrameInterrupt();
        gSleepActive = 1;
        {
          u32 high;

          // The wake mask written as its high byte shifted up plus its low
          // byte is built in a register; as the constant, it loads from the
          // pool.
          high = KEYCNT_SLEEP_WAKE >> 8;
          /* FAKEMATCH(barrier): an empty asm on high, so the mask's first
           * instruction comes before REG_KEYCNT's address loads; without it,
           * 2 of 326 instructions differ in tla-en. */
          asm volatile("" : "+l"(high));
          REG_KEYCNT = (high << 8) + (KEYCNT_SLEEP_WAKE & 255);
        }
        Bios_SoundBiasOff();
        Bios_Stop();
        Bios_SoundBiasOn();
        Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
        Io_Write16(0, &gSleepActive);
        for (j = 9; j >= 0; j--)
          System_WaitForFrameInterrupt();
        while (gInput.held)
          System_WaitForFrameInterrupt();
        Io_Write16((u16)display, &REG_DISPCNT);
        Io_Write16((u16)backdrop, &PLTT_BACKDROP);
        gSleepRequested = 0;
        gIdleFrameCount = 0;
      } else
        gSleepRequested--;
    }
    if (gResetRequested) {
      gResetRequested = 0;
      System_Reset();
    }
  }
}

void Input_ResetKeyRepeat(void) { gInput.repeatTimer = 19; }

/// Frames a key is held before it first repeats, and between later repeats.
#define INPUT_KEY_REPEAT_FIRST 19
#define INPUT_KEY_REPEAT_NEXT 6

/// Runs once a frame after VBlank, as The Broken Seal's twin does. When the
/// repeat timer has run out, the held keys repeat into gInput.repeat.
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
  /* FAKEMATCH(barrier): an empty asm after held is read, so it loads before
   * the repeat timer; without it, 2 of 113 instructions differ in tla-en. */
  asm("");
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
      mask = 0xffff & ~KEYS_DPAD;
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
      gInput.direction &= gInput.previousDirection ^ 0xffff;
      break;
    case 3:
      if (gInput.previousDirection & KEYS_HORIZONTAL)
        axis = KEYS_HORIZONTAL;
      if (gInput.previousDirection & KEYS_VERTICAL)
        axis = KEYS_VERTICAL;
      mask = 0xffff ^ axis;
      gInput.previousDirection = keys & mask;
      gInput.direction &= mask;
      break;
    }
  } else {
    gInput.direction = keys;
  }

  gInput.pressed = held & ~gInput.previousHeld;
  /* FAKEMATCH(dead): an empty once-only block after pressed is stored, so
   * previousHeld is stored after it; without it, 2 of 113 instructions
   * differ in tla-en. */
  do {
  } while (0);
  gInput.previousHeld = held;
}

CAMELOT_ASM("ldmia and stmia of four registers: agscc's block moves stop at "
            "three, so a 16-byte struct copy compiles to 12 and 4")
static __inline__ void Copy16Bytes(const void *source,
                                   volatile void *destination) {
  register const void *src __asm__("r0") = source;
  register volatile void *dst __asm__("r1") = destination;
  __asm__ volatile("ldmia %0!, {r2, r3, r4, r5}\n\tstmia %1!, {r2, r3, r4, r5}"
                   : "+r"(src), "+r"(dst)
                   :
                   : "r2", "r3", "r4", "r5", "memory");
}

void System_VBlankHandler(void) {
  u32 keys;
  u32 vblankCount;
  void (*hook)(void);

  {
    volatile u16 *dma0 = REG_DMA0;

    dma0[5] &= ~0x3a00;
    dma0[5] &= ~0x8000;
    dma0[5];
  }
  if (gSerialExchangeActive != 0) {
    gLinkStatus =
        Serial_ExchangePayloads((void *)&gSerialTransfer, gSerialPeerPayloads);
    Serial_StepBlockTransfer();
  }
  SoundDriver_VSyncRefresh();
  if (gBlendDuration != 0)
    ScreenFade_Update();
  if (gFrameRenderPending != 0) {
    gFrameRenderPending = 0;
    if (gOamUsage.used != 0)
      Dma_Set(Ram_HeapSlots->unk4c[1], OAM, 0x84000000 + gOamUsage.used,
              REG_DMA3);
    Copy16Bytes(gBgScroll, REG_BG0HOFS);
    IoWriteQueue_FlushPending();
  }
  hook = gData_030011f8;
  if (hook != 0) {
    gData_030011f8 = 0;
    hook();
  }
  Task_RunPassBeforeVBlankTasks(TASK_PASS_FRAME);

  keys = REG_KEYINPUT ^ 0x3ff;
  {
    // The input state reached through a volatile pointer, so each access is a
    // load or store of its own; as gInput used plainly, 31 of 152
    // instructions differ in tla-en.
    volatile struct InputState *input = (void *)&gInput;
    u32 pressed = keys & ~input->held;

    input->pressed = pressed;
    input->pendingPressed |= pressed;
    input->held = keys;
    if (keys == 0) {
      input->repeatTimer = 19;
      input->repeat = keys;
    } else if (input->held & (input->previousVblankHeld ^ 0xffff)) {
      input->repeatTimer = -1;
      input->repeat = keys;
    } else if (input->repeatTimer > 0) {
      input->repeatTimer--;
    }
  }
  {
    // The VBlank count read and written through a volatile pointer; as a
    // plain pointer, 33 of 152 instructions differ in tla-en.
    volatile u32 *count;
    volatile struct SoundVBlankState *sound;

    count = &gVBlankCount;
    // The store through a volatile cast, so it comes before the sound state's
    // address is loaded; as a plain store, 2 of 152 instructions differ in
    // tla-en.
    ((volatile struct InputState *)&gInput)->previousVblankHeld = keys;
    sound = &gSoundVBlankState;
    vblankCount = *count;
    if (sound->lastVBlank != (u8)vblankCount && sound->updating == 0) {
      sound->transferCountdown = gData_03007ff0[4];
      Sound_UpdateBgmRampsFar();
      if (SoundDriver_EnterVBlankUpdate() == 0) {
        void (*render)(s32) = IwramSoundRenderVBlank;

        render(8);
      }
      sound->lastVBlank = vblankCount;
    }
    *count = vblankCount + 1;
  }
  // The frame count's step in a once-only block, so its address is loaded
  // after the VBlank count's store; as a plain step, 2 of 152 instructions
  // differ in tla-en.
  do {
    gFrameVBlanks++;
  } while (0);
  gVBlankDone = 1;
}

#define SOUND_PRESET_COUNT 5

s32 Sound_LoadReverbCode(s32 preset) {
  if ((u32)preset > SOUND_PRESET_COUNT - 1)
    preset = 0;
  return 0;
}

void Sound_ReservedNoOpA(void) {}

void Sound_ReservedNoOpB(void) {}

void Input_HandleKeyInterrupt(void) {
  if (gSleepActive == 0) {
    Io_Write16(0xc3ff, &REG_KEYCNT);
    gResetRequested = 1;
  }
}

#undef SLEEP_IDLE_FRAMES
#undef RGB_WHITE
#undef INPUT_KEY_REPEAT_FIRST
#undef INPUT_KEY_REPEAT_NEXT
#undef SOUND_PRESET_COUNT

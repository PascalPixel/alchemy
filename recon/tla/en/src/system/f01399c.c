#include "types.h"
#include "call.h"
#include "io_reg.h"
#include "dma.h"
#include "frame.h"
#include "input.h"
#include "oam.h"
#include "bg_scroll.h"
#include "serial.h"
#include "io_write_queue.h"
#include "task.h"
#include "ram_buffer.h"

struct SoundVBlankState {
  u8 lastVBlank;
  u8 unk01;
  u8 unk02;
  u8 unk03;
};

struct BgScrollRegs {
  u32 words[4];
};

extern volatile struct SoundVBlankState gData_03001138;
extern u8 *gData_03007ff0;
extern void (*gData_030011f8)(void);
extern u32 gVBlankCount;
extern u16 gData_0300121c;
extern volatile u8 gBlendDuration;

void SoundDriver_VSyncRefresh(void);
void Fade_Update(void);
void Func_081c0088(void);
s32 SoundDriver_EnterVBlankUpdate(void);
void IwramSoundRenderVBlank(s32) __attribute__((long_call));

CAMELOT_ASM("ldmia and stmia of four registers: agscc's block moves stop at "
            "three, so a 16-byte struct copy compiles to 12 and 4")
static __inline__ void Copy16Bytes(const void *source, volatile void *destination) {
  register const void *src __asm__("r0") = source;
  register volatile void *dst __asm__("r1") = destination;
  __asm__ volatile("ldmia %0!, {r2, r3, r4, r5}\n\tstmia %1!, {r2, r3, r4, r5}"
                   : "+r"(src), "+r"(dst)
                   :
                   : "r2", "r3", "r4", "r5", "memory");
}

void Func_0801399c(void) {
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
    gLinkStatus = Serial_ExchangePayloads((void *)&gSerialTransfer,
                                                 gSerialPeerPayloads);
    Serial_StepBlockTransfer();
  }
  SoundDriver_VSyncRefresh();
  if (gBlendDuration != 0)
    Fade_Update();
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
    ValueLr0((s32 (*)())hook);
  }
  Task_RunPassBeforeVBlankTasks(TASK_PASS_FRAME);

  keys = REG_KEYINPUT ^ 0x3ff;
  {
    /* FAKEMATCH(volatile): the input record read through a volatile pointer; as gInput read plainly, 31 of 152 instructions differ in tla-en. */
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
    /* FAKEMATCH(volatile): the VBlank count read through a volatile pointer; as a plain pointer, 33 of 152 instructions differ in tla-en. */
    volatile u32 *count;
    /* FAKEMATCH(volatile): the sound state volatile; as plain, 25 of 152 instructions differ in tla-en. */
    volatile struct SoundVBlankState *sound;

    count = &gVBlankCount;
    /* FAKEMATCH(volatile): this store through a volatile view; as a plain store, 2 of 152 instructions differ in tla-en. */
    ((volatile struct InputState *)&gInput)->previousVblankHeld = keys;
    sound = &gData_03001138;
    vblankCount = *count;
    if (sound->lastVBlank != (u8)vblankCount && sound->unk03 == 0) {
      sound->unk01 = gData_03007ff0[4];
      Func_081c0088();
      if (SoundDriver_EnterVBlankUpdate() == 0) {
        void (*render)(s32) = IwramSoundRenderVBlank;

        ValueLr1((s32 (*)())render, (s32)(8));
      }
      sound->lastVBlank = vblankCount;
    }
    *count = vblankCount + 1;
  }
  /* FAKEMATCH(one-pass): the frame count's step in a block that runs once; as a plain step, 2 of 152 instructions differ in tla-en. */
  do {
    gFrameVBlanks++;
  } while (0);
  gData_0300121c = 1;
}

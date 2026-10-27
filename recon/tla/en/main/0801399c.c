// Draft, differ 97 (117 before statements reordered 2026-10-26).
// Draft, differ 119 of 152: the VBlank interrupt handler, as ☀️'s
// System_VBlankHandler (games/tbs/src/runtime/runtime.c) with ⚓️'s input
// record and its once-a-frame sound step. Left, other than order: the
// native rereads gInput.held for the repeat test and gInput.repeatTimer for
// the decrement (as if the input record were volatile here; through a
// volatile pointer, differ 115, the pointer then stays in a register), keeps
// keys in r0 and gInput in r1 (swapped here), and takes the low byte of the
// VBlank count from its register (lsls/lsrs) where the draft reloads it as a
// byte. IwramSoundRenderFrame is reached through lr as a long call.
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "frame.h"
#include "input.h"
#include "oam.h"
#include "bg_scroll.h"
#include "serial_runtime.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"

struct SoundFrameState {
  u8 lastVBlank;
  u8 unk01;
  u8 unk02;
  u8 unk03;
};

struct BgScrollRegs {
  u32 words[4];
};

extern volatile struct SoundFrameState gData_03001138;
extern u8 *gData_03007ff0;
extern void (*gData_030011f8)(void);
extern u32 gVBlankCount;
extern u16 gData_0300121c;
extern volatile u8 gBlendDuration;

void Func_08016990(void);
void SoundDriver_VSyncRefresh(void);
void Fade_Update(void);
void Func_081c0088(void);
s32 SoundDriver_EnterFrameUpdate(void);
void IwramSoundRenderFrame(s32) __attribute__((long_call));

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
    gLinkStatus = SerialRuntime_ExchangePayloads((void *)&gSerialTransfer,
                                                 gSerialPeerPayloads);
    Func_08016990();
  }
  SoundDriver_VSyncRefresh();
  if (gBlendDuration != 0)
    Fade_Update();
  if (gFrameRenderPending != 0) {
    gFrameRenderPending = 0;
    if (gOamUsage.used != 0)
      Dma_Set(((void **)0x03000000)[20], OAM, 0x84000000 + gOamUsage.used,
              REG_DMA3);
    *(struct BgScrollRegs *)REG_BG0HOFS = *(struct BgScrollRegs *)gBgScroll;
    IoWriteQueue_FlushPending();
  }
  hook = gData_030011f8;
  if (hook != 0) {
    gData_030011f8 = 0;
    hook();
  }
  Scheduler_RunCallbacksBeforeBoundary(0xc80);

  keys = REG_KEYINPUT ^ 0x3ff;
  {
    u32 pressed = keys & ~gInput.held;

    gInput.pressed = pressed;
    gInput.pendingPressed |= pressed;
  }
  gInput.held = keys;
  if (keys == 0) {
    gInput.repeatTimer = 19;
    gInput.repeat = keys;
  } else if (gInput.held & (gInput.previousVblankHeld ^ 0xffff)) {
    gInput.repeatTimer = -1;
    gInput.repeat = keys;
  } else if (gInput.repeatTimer > 0) {
    gInput.repeatTimer--;
  }

  vblankCount = gVBlankCount;
  gInput.previousVblankHeld = keys;
  if (gData_03001138.lastVBlank != (u8)vblankCount && gData_03001138.unk03 == 0) {
    gData_03001138.unk01 = gData_03007ff0[4];
    Func_081c0088();
    if (SoundDriver_EnterFrameUpdate() == 0)
      IwramSoundRenderFrame(8);
    gData_03001138.lastVBlank = vblankCount;
  }
  gVBlankCount = vblankCount + 1;
  gFrameVBlanks++;
  gData_0300121c = 1;
}

// Draft, differ 73 of 227: queues the second world map picture (its tiles,
// frames and palette) and scrolls it in over 96 frames, then fades and
// restores the window palette. Left: the scroll loop's registers (the native
// keeps the count in r5 and x in r6) and a 4-byte frame this draft adds.
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "event_work.h"
#include "bg_scroll.h"
#include "animspr.h"

extern u8 gWorldMapPackedTilesB[];
extern u8 gWorldMapPackedFramesB[];
extern u8 gWorldMapPalettesB[];

void WaitFrames(s32 frames);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Func_02004aa8(void *source, void *destination);
void Func_02004ac8(void);
void Func_02004ad8(void);
void Func_02004bb8(s32 hue, s32 brightness);
void Func_02004d78(s32 color, s32 mode);
void Func_02004d88(s32 frames);
void Func_02004e30(s32 cue);

void Func_020042e0(void) {
  volatile u16 *ime;
  u8 *buffer = Runtime_BumpAllocateAlternatePool(0x4000);
  u8 *frames;
  s32 x, y, i;

  Func_02004aa8(gWorldMapPackedTilesB, buffer);
  frames = buffer + 0x2000;
  Func_02004aa8(gWorldMapPackedFramesB, frames);
  ime = &REG_IME;
  IO_WRITE_QUEUE_PUSH_IME(ime, 0x050001c0, (u32)gWorldMapPalettesB, 0x80000010);
  IO_WRITE_QUEUE_PUSH_IME(ime, 0x06000000, (u32)buffer, 0x84000400);
  IO_WRITE_QUEUE_PUSH_IME(ime, 0x06001000, (u32)frames, 0x84000800);
  QueueIoWriteDelay2(0x04000050, 0x3f41);
  QueueIoWriteDelay2(0x04000052, 0x1010);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 352;
  IO_WRITE_QUEUE_PUSH_IME(ime, 0x04000008, 0xc200, 0x20000);
  y = 0x2d60;
  x = 561;
  for (i = 95; i >= 0; i--) {
    gBgScroll[0].x = (x * 16) / 128;
    gBgScroll[0].y = -y / 128 + 352;
    WaitFrames(1);
    y += 352;
    x += 17;
  }
  Func_02004e30(164);
  WaitFrames(120);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Func_02004d78(0x7fff, 0);
  Func_02004d88(1);
  QueueIoWriteDelay2(0x04000052, 0x1000);
  WaitFrames(1);
  Func_02004d78(0, 0);
  Func_02004d88(60);
  Func_02004e30(78);
  WaitFrames(60);
  Func_02004e30(390);
  WaitFrames(120);
  Ram_HeapSlots->objectSystem->suspended = 1;
  WaitFrames(1);
  Func_02004ad8();
  Func_02004ac8();
  Func_02004bb8(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
}

// Draft, differ 25 of 227 (was 45): queues the second world map picture (its
// tiles, frames and palette) and scrolls it in over 96 frames, then fades and
// restores the window palette. The scroll's x is x * 16 kept in a temporary
// before the divide by 128 (as one expression GCC folds it to x / 8, which
// the native does not; a device on adoption). Tried before: a goto loop (58),
// a BgScroll pointer set before or inside the loop (48).
// The scroll's x goes through scrollX, a local declared first (45 to 25),
// which moves the scroll address's load into the loop as the native has it;
// x and y through two locals stored together, 68. Left: the native stores x
// only after y is computed, and keeps the closing zero in r6; the queue pushes
// differ in order by one instruction each. Permuter forms at 19 to 23 are
// one-pass blocks and temporaries.
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "bg_scroll.h"
#include "sprite.h"

extern u8 gWorldMapPackedTilesB[];
extern u8 gWorldMapPackedPicturesB[];
extern u8 gWorldMapPalettesB[];

void wait(s32 frames);
void *Heap_AllocEwram(s32 size);
void Func_02004aa8(void *source, void *destination);
void Func_02004ac8(void);
void Func_02004ad8(void);
void Func_02004bb8(s32 hue, s32 brightness);
void Func_02004d78(s32 color, s32 mode);
void Func_02004d88(s32 frames);
void Func_02004e30(s32 cue);

void Func_020042e0(void) {
  s32 scrollX;
  volatile u16 *ime;
  u8 *buffer = Heap_AllocEwram(0x4000);
  u8 *frames;
  s32 x, y, i, t;

  Func_02004aa8(gWorldMapPackedTilesB, buffer);
  frames = buffer + 0x2000;
  Func_02004aa8(gWorldMapPackedPicturesB, frames);
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
    t = x * 16;
    scrollX = t / 128;
    gBgScroll[0].x = scrollX;
    gBgScroll[0].y = -y / 128 + 352;
    wait(1);
    y += 352;
    x += 17;
  }
  Func_02004e30(164);
  wait(120);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Func_02004d78(0x7fff, 0);
  Func_02004d88(1);
  QueueIoWriteDelay2(0x04000052, 0x1000);
  wait(1);
  Func_02004d78(0, 0);
  Func_02004d88(60);
  Func_02004e30(78);
  wait(60);
  Func_02004e30(390);
  wait(120);
  Ram_HeapSlots->objectSystem->suspended = 1;
  wait(1);
  Func_02004ad8();
  Func_02004ac8();
  Func_02004bb8(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
}

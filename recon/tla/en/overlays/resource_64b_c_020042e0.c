// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it. In tla-en the
// object assembled by the build's GAS 2.10 equals the listing byte for byte,
// literal pool and relocations included (a newer assembler pads before the
// pool with a nop where the ROM has a zero halfword).
// Queues the second world map picture (its tiles, frames and palette) and
// scrolls it in over 96 frames, then fades and restores the window palette.
// What closed it (13 to 0): the scroll's y goes through scrollY, a u16 (or
// s16) local computed before either store, so the scroll address loads after
// both values, as the native has it; as an s32 or u32 local its 352 is shared
// with y's step (72). With that, the plain IO_WRITE_QUEUE_PUSH replaces the
// ime local and its barrier (IO_WRITE_QUEUE_PUSH_IME without the barrier
// orders the two pool loads the other way, 2), and mode, a function-level
// local holding 0, replaces the once-only block around the fade's argument
// (as a literal 0, the closing stores' zero lands in r5, 6). scrollX stays an
// s32 (a u16 or s16, 7). One device is left, t (tagged in the loop);
// (s32)((u32)x * 16) / 128 also matches, but its cast misstates the
// arithmetic. 561 is 33 * 17 and 0x2d60 is 33 * 352: x and y run through the
// 33rd to 128th multiples of their steps.
#include "types.h"
#include "call.h"
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
  u8 *buffer = Heap_AllocEwram(0x4000);
  u8 *frames;
  s32 x, y, i, t, scrollX;
  u16 scrollY;
  s32 mode;

  Func_02004aa8(gWorldMapPackedTilesB, buffer);
  frames = buffer + 0x2000;
  Func_02004aa8(gWorldMapPackedPicturesB, frames);
  IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)gWorldMapPalettesB, 0x80000010);
  IO_WRITE_QUEUE_PUSH(0x06000000, (u32)buffer, 0x84000400);
  IO_WRITE_QUEUE_PUSH(0x06001000, (u32)frames, 0x84000800);
  QueueIoWriteDelay2(0x04000050, 0x3f41);
  QueueIoWriteDelay2(0x04000052, 0x1010);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 352;
  IO_WRITE_QUEUE_PUSH(0x04000008, 0xc200, 0x20000);
  y = 0x2d60;
  x = 561;
  for (i = 95; i >= 0; i--) {
    /* FAKEMATCH(temporary): x * 16 held in t before the divide; as scrollX =
     * x * 16 / 128, 22 of 227 instructions differ in tla-en. */
    t = x * 16;
    scrollX = t / 128;
    scrollY = -y / 128 + 352;
    gBgScroll[0].x = scrollX;
    gBgScroll[0].y = scrollY;
    wait(1);
    y += 352;
    x += 17;
  }
  Func_02004e30(164);
  wait(120);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  // mode holds one 0 for the rest of the function; the closing scroll reset
  // stores it again.
  mode = 0;
  Func_02004d78(0x7fff, mode);
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
  Func_02004bb8(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
}

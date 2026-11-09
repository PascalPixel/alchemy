#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "bg_scroll.h"
#include "sprite.h"
#include "event_inline.h"

extern u8 gWorldMapPackedTilesB[];
extern u8 gWorldMapPackedPicturesB[];
extern u8 gWorldMapPalettesB[];

void *Heap_AllocEwram(s32 size);
void Func_02004aa8(void *source, void *destination);
void Func_02004ac8(void);
void Func_02004ad8(void);

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
  Sound_Play(164);
  wait(120);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  // mode holds one 0 for the rest of the function; the closing scroll reset
  // stores it again.
  mode = 0;
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, mode);
  BattleEffect_StartBufferInterpolation(1);
  QueueIoWriteDelay2(0x04000052, 0x1000);
  wait(1);
  BattleEffect_ApplyColorToTargetBuffer(0, 0);
  BattleEffect_StartBufferInterpolation(60);
  Sound_Play(78);
  wait(60);
  Sound_Play(390);
  wait(120);
  Ram_HeapSlots->objectSystem->suspended = 1;
  wait(1);
  Func_02004ad8();
  Func_02004ac8();
  Func_0803f9c0(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
}

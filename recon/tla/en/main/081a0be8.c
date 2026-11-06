// Near miss, differ 8 of 88 (tla-en, 2026-11-06): scheduling only; the
// IWRAM clear's size shift lands before its address load, the pack loop's
// two pointer copies land ahead of its count's shift, and the first copy
// loop sets its canvas pointer before the VRAM offset.
// Earlier: differ 43 of 88 (2026-11-05).
#include "types.h"
#include "heap_release.h"
#include "iwram_call.h"

s32 Func_08038418(s32 message, u8 *canvas, s32 align);

s32 DisplayScroll_DrawMessageLine(s32 message, s32 slot, s32 align) {
  // Unused: gives the stack frame its 32 bytes.
  u32 unused[8];
  u8 *canvas;
  u8 *src;
  u8 *dst;
  u32 *tiles;
  u32 *top;
  s32 busy;
  s32 i;
  s32 row;
  s32 offset;

  canvas = Heap_AllocEwram(0x900);
  Iwram_ClearWords(canvas, 0xc00);
  busy = Func_08038418(message, canvas, align);
  dst = canvas;
  src = canvas;
  for (i = 0; i < 0x600; i++) {
    {
      /* FAKEMATCH(temporary): the form kept; as *dst++ = src[0] | (src[1] << 4), 12 of 88 instructions differ in tla-en. */
      u8 low = src[0];
      *dst++ = low | (src[1] << 4);
    }
    src += 2;
  }
  for (i = 0, src = canvas; i <= 23; i++, src += 4) {
    tiles = (u32 *)((slot + i) * 32 + 0x06010000);
    for (row = 0; row < 8; row++) {
      *tiles++ = ((u32 *)src)[row * 24];
    }
  }
  for (offset = slot * 32, top = (u32 *)(canvas + 0x300), i = 0; i <= 23;
       i++, offset += 32, top++) {
    tiles = (u32 *)(offset + 0x06010300);
    for (row = 0; row < 8; row++) {
      *tiles++ = top[row * 24];
    }
  }
  Heap_Release(canvas);
  return busy;
}

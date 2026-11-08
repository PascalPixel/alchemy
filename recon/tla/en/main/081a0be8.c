// Differ 6 of 88 (tla-en, 2026-11-08, -mtune=gba): scheduling only. Both
// left sites set a loop-made count or offset (0x600, slot * 32) after the
// copies of canvas into src and dst, where the listing sets it first.
// 2026-11-08: no gain from the for steps or the pack loop's steps reordered
// (6 to 8), loop 2 written like loop 3 (30 to 42), both pointer orders, the
// pointers set in the for, src = dst = canvas, a once-only block before or
// round the pointer copies (8 to 12), canvas[i] for *dst++ (6), the pack loop
// indexed by i * 2 (44), bounds as <=, != or counting down (6 to 66), the
// tile address spelled three ways, and 8 permuter minutes.
// 2026-11-07: Iwram_ClearWords through an inline wrapper, as the Broken Seal's
// iwram_call.h writes it, puts the size shift after the address load (8 to 6);
// left, scheduling: the pack loop's two pointer copies ahead of its count's
// shift and in swapped order, and the first copy loop's canvas pointer.
// Earlier, differ 8 of 88 (2026-11-06): scheduling only; the
// IWRAM clear's size shift lands before its address load, the pack loop's
// two pointer copies land ahead of its count's shift, and the first copy
// loop sets its canvas pointer before the VRAM offset.
// Earlier: differ 43 of 88 (2026-11-05).
#include "types.h"
#include "heap_release.h"
#include "iwram_call.h"

// The clear goes through an inline wrapper, as the Broken Seal's iwram_call.h
// writes it: called straight through the fixed entry, the build shifts the
// size before loading the routine's address.
#undef Iwram_ClearWords
static inline s32 Iwram_ClearWords(void *destination, s32 size) {
  return Iwram_ClearWordsEntry(destination, size);
}

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
      /* FAKEMATCH(temporary): the form kept; as *dst++ = src[0] | (src[1] << 4), 10 of 88 instructions differ in tla-en. */
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

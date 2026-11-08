/// @file
/// Draws one line of the credits' scrolling text: the message is drawn into a
/// canvas of one byte a pixel, packed to two pixels a byte, and copied into
/// object tiles from slot on, its top half then its bottom half.

#include "types.h"
#include "heap_release.h"
#include "iwram_call.h"

// The clear goes through an inline wrapper, as the Broken Seal's iwram_call.h
// writes Iwram_ClearWords; through this game's direct entry, the build shifts
// the size before loading the routine's address.
static inline s32 ClearWords(void *destination, s32 size) {
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
  s32 j;
  s32 row;
  s32 offset;

  canvas = Heap_AllocEwram(0x900);
  // BUG: the clear, and the packing loop's reads below, cover 0xc00 bytes of
  // a 0x900-byte allocation.
  ClearWords(canvas, 0xc00);
  busy = Func_08038418(message, canvas, align);
  dst = canvas;
  src = canvas;
  for (j = 0; j < 0x600; j++) {
    u8 low = *src++;
    *dst++ = low | (*src++ << 4);
  }
  for (i = 0; i <= 23; i++) {
    u32 *column = (u32 *)canvas + i;

    tiles = (u32 *)((slot + i) * 32 + 0x06010000);
    for (row = 0; row < 8; row++) {
      *tiles++ = column[row * 24];
    }
  }
  for (offset = slot * 32, top = (u32 *)(canvas + 0x300), j = 0; j <= 23;
       j++, offset += 32, top++) {
    tiles = (u32 *)(offset + 0x06010300);
    for (row = 0; row < 8; row++) {
      *tiles++ = top[row * 24];
    }
  }
  Heap_Release(canvas);
  return busy;
}

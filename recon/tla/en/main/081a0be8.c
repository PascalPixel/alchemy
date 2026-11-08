// Matches (tla-en, 2026-11-08, -mtune=gba). What closed it: the pack loop
// reads its two nibbles through src++ (the low one into its own local, so the
// two reads are ordered), with its own counter j; the left-half loop reads
// canvas indexed by i rather than through a stepped pointer, so its strength
// reduction sets the VRAM offset before the canvas pointer, as the listing.
// The old `temporary` low (src[0] before the or) is gone with it.
// Earlier: differ 6 of 88 (2026-11-08), 8 (2026-11-06), 43 (2026-11-05).
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
  s32 j;
  s32 row;
  s32 offset;

  canvas = Heap_AllocEwram(0x900);
  Iwram_ClearWords(canvas, 0xc00);
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

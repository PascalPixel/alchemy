// Draft, differ 0 of 74 (tla-en, 2026-11-08); re-measured 2026-11-08 by
// --symbol: ja and de 74, es, fr and it 76, all differ 0. Ready for tla-es,
// tla-fr and tla-it only, but not through the tool: adopt main must include
// English (adopt edition needs an adopted object), and English must stay on
// its listing for the pool word below. By hand: this file without its header
// as games/tla/src/window/f042274.c, an objects.tsv row for it in es, fr and
// it in place of each one's rom.s section for Text_DrawPrefixedNumberAtOffset
// (as field/f021a84 is linked in fr and it only), English's raw row kept.
// Earlier: every instruction matches in
// all six editions, and tla-es, tla-fr and tla-it match byte for byte (their
// form sets both halfwords in the branches and pools no 0xf01e; compiled for
// each and compared with its function, only the two calls' link-time offsets
// differ), so those three can be adopted without the others (S4). tla-en,
// tla-ja and tla-de pool the word 0xfffff01e where the ROM holds 0x0000f01e,
// 88 bytes after its load (ldr r3, [pc, #88]): 2 bytes short of the ROM.
// What is left there is a compiler question (K3), not a C form:
// - A zero-extended 0xf01e is a halfword constant: as `u16 *p = output;
//   p[1] = 0xf01e;` *thumb_movhi_insn pools the word 61470, but its pool load
//   reaches 64 bytes, so the pool moves behind a branch after the store
//   (differ 5). The ROM shows the same reach: in tla-en's remaining
//   main-image listings the 90 other pooled halfwords stored by strh sit 10
//   to 62 bytes from their pools, several at 62; only this one is 90 bytes
//   away, so its load was not a movhi pool load.
// - The ROM keeps `adds r2, r4, #4` after the second strh, as the plain
//   output[1] = 0xf01e does (a constant set, split into three instructions,
//   differ 6). Every pool load is a load to the scheduler, which draws that
//   add into its delay (differ 2 without the once-only block). So the
//   original's 0xf01e was a constant set through sched2 that went to the word
//   pool (ldr, 1020 bytes' reach) instead of being split, as the 524 words
//   from 0x100 to 0xffff the remaining main-image listings pool while
//   splitting others in the same functions (0x17f loaded for a call at
//   0x08167402; adopted C pools none). alchemy-gcc splits every such SImode
//   constant after reload (*thumb_movsi_gba_const), so none reaches the pool.
// Tried 2026-11-08: (s16)0xf01e stored directly (split, differ 6); the plain
// store inside the once-only block (differ 6). The once-only block replaces
// the earlier barrier (an empty asm after the store, differ 2 without it).
// Earlier rounds (2026-10-31): the local as s16, differ 2 with the same pool
// word; as u32, differ 6; index = 1; output[index] = 0xf01e pools 61470 behind
// a new branch (differ 5). Before -mtune=gba the native loaded the fifth
// argument from the stack and the window tiles before it copied the value
// (differ 2).
// The Broken Seal's Text_DrawPrefixedNumberAtOffset in both its forms:
// English, German and Japanese set the second halfword after the branches;
// Spanish, French and Italian set both in them. The window's x and y are read
// as signed halfwords.
#include "types.h"
#include "edition.h"
#include "ram_buffer.h"
#include "window.h"
#include "render_input.h"
#include "text_render.h"

u8 *String_FormatNumber(u8 *output, s32 value, s32 width);

void Text_DrawPrefixedNumberAtOffset(s32 value, struct RenderInput *work,
                                       s32 offsetX, s32 offsetY, s32 variant) {
  u8 formatted[16];
  u16 output[8];
  u8 *text;
  s32 index;
  u32 cell;
  struct WindowWork *canvas;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  text = String_FormatNumber(formatted, value, 4);
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
  if (variant == 0) {
    output[0] = 0xf01d;
    output[1] = 0xf01e;
  } else {
    output[0] = 0xf01f;
    output[1] = 0xf005;
  }
#else
  if (variant == 0) {
    output[0] = 0xf01d;
  } else {
    output[0] = 0xf01f;
  }
  // A once-only block: its end keeps the loop's set-up after this store.
  do {
    // Held in a u16 local, 0xf01e is loaded; stored directly, it is built.
    u16 second = 0xf01e;

    output[1] = second;
  } while (0);
#endif
  for (index = 0; index <= 4; index++) {
    output[index + 2] = text[index];
  }
  output[6] = 0;
  cell = (((s16)work->y + ((u32)offsetY >> 3) + 1) << 5) +
         ((s16)work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 0x280) {
    vram = 0x06002000;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    Text_DrawStringAsTiles((u16 *)output, (u16 *)src, (u16 *)dst, phase);
  }
}

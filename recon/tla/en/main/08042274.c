// Draft, differ 0 of 74 (tla-en, 2026-11-09); re-measured 2026-11-09 by
// --symbol: ja and de 74, es, fr and it 76, all differ 0. Spanish, French and
// Italian link this C as games/tla/src/window/f042274.c (2026-11-09, by hand:
// adopt main must include English): their form sets both halfwords in the
// branches and pools no 0xf01e, and compiled for each only the two calls'
// link-time offsets differ (S4). English, German and Japanese stay on their
// listings: this form pools the word 0xfffff01e where the ROM holds
// 0x0000f01e, 88 bytes after its load (ldr r3, [pc, #88]).
// No C spelling gives that word a word load's reach under alchemy-gcc (9
// November, measured, and traced through arm.md and the passes); what is left
// is a compiler question (K3):
// - alchemy-gcc loads a pool word with ldr (1020 bytes' reach) only for
//   *thumb_movsi_insn's constant or pool alternative, a float, a doubleword
//   (an 8-byte entry) or *thumb_extendhisi2_insn reading a halfword pool
//   entry. Every SImode 61470 in a basic block is split after reload whatever
//   pattern the insn was recognized as, constants reload substitutes from a
//   REG_EQUIV included; a float's bits fold to an integer in combine. A
//   halfword pool entry for extendhisi2 needs force_const_mem, which Thumb
//   reaches only for symbols and labels: reload never forces a constant to
//   memory (PREFERRED_RELOAD_CLASS is never NO_REGS), cse only after a
//   constant SET fails to match, and & of a constant only for a constructor,
//   which goes to .rodata.
// - A halfword constant set in a register is pooled by *thumb_movhi_insn's
//   'mn' alternative whatever its size (0x80 too), as the word 0x0000xxxx (a
//   u16 constant expands to a positive HImode CONST_INT), and GAS 2.10
//   assembles its `ldrh rN, label` as ldr rN, [pc, #...]; but its reach is
//   64 bytes: `u16 *p = output; p[1] = 0xf01e;` gives the ROM's word with the
//   pool behind a new branch (differ 5). The ROM shows the same reach: in
//   tla-en's remaining main-image listings the 90 other pooled halfwords
//   stored by strh sit 10 to 62 bytes from their pools, several at 62; only
//   this one is 90 bytes away, so its load was not a movhi pool load.
// - A u16 local is promoted to a signed SImode pseudo (PROMOTE_MODE), so it
//   holds -4066, which the tune cannot build: this form, every instruction
//   matching, the word 0xfffff01e.
// - The ROM keeps `adds r2, r4, #4` after the second strh, as the plain
//   output[1] = 0xf01e does (a constant set, split into three instructions,
//   differ 6). Every pool load is a load to the scheduler, which draws that
//   add into its delay (differ 2 without the once-only block). So the
//   original's 0xf01e was most likely the plain literal store after the
//   branches (Spanish, French and Italian store literals in them), its SImode
//   61470 left to the word pool by Camelot's compiler instead of being built,
//   as the 524 words from 0x100 to 0xffff the remaining main-image listings
//   pool while building others in the same functions (0x17f loaded for a call
//   at 0x08167402; adopted C pools none).
// Measured, built (6 or 10) or pooling 0xfffff01e: a static const u16 or u32,
// a const u16 table with a constant index, an enum, a cast, a u32 or s32
// local (also set before the branches), a union local, a compound literal
// (array or union), a u32 store over both halfwords, a register u16 local, a
// float's bits through a union or a pointer (6 or 7), a 16-bit bit-field (6),
// an inline helper returning u32 (6) or u16 (this form's word, differ 0), the
// u16 local set before the branches (differ 2), the second store in both
// branches (6; through a pointer all three halfwords pool, 14), a struct
// {u16} local (its initializer goes to .rodata, read through its address,
// differ 3).
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

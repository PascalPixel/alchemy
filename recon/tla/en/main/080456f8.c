// Near miss, differ 2 of 47: after the glyph call the native builds 0x11c as
// movs r2, #142; ldr r4, [sp] (base's caller-save restore); lsls r2, r2, #1,
// where here the shift comes before the restore. 2026-10-18: 0x8e held by an
// empty asm puts the restore after the movs but moves the constant to r3
// (differ 6); pinned to r2, with the sum pinned to r3 and the shift kept in
// r2, the registers match and the shift is back ahead of the restore (differ
// 2); naming base in the asm keeps base out of the stack (differ 54). Five
// permuter minutes found nothing below 6 from the pinned form.
#include "types.h"

void UiText_LoadRemappedGlyph(u8 *base, u32 value, s32 slot_a, s32 slot_b);
s32 Resource_LoadIndexedIntoBuffer(u16 packed, u32 value);

typedef struct {
  u16 code : 10;
  u16 style : 6;
} CharacterCell;

void UiText_DrawCharacter(u8 *base, s32 index, u32 value) {
  u8 *entry;
  s32 offset;
  s32 store_offset;
  s32 load_offset;

  offset = index * 28;
  entry = base + offset + 0x104;
  UiText_LoadRemappedGlyph(base, value, index * 16, index * 16);
  store_offset = offset + 0x11C;
  *(u32 *)(base + store_offset) = value;
  /* FAKEMATCH: the empty do-while around this store only moves the
     scheduler; the reference sets the constant after the value store. */
  do {
    *(u32 *)(entry + 4) = 0x80002000;
  } while (0);
  *(u32 *)(entry + 8) = 0;
  load_offset = offset + 0x110;
  ((CharacterCell *)(entry + 8))->code =
      Resource_LoadIndexedIntoBuffer(*(u16 *)(base + load_offset), value);
}

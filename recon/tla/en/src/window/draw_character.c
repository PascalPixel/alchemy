#include "types.h"

void Text_LoadRemappedGlyph(u8 *base, u32 value, s32 slot_a, s32 slot_b);
s32 Resource_LoadIndexedIntoBuffer(u16 packed, u32 value);

typedef struct {
  u16 code : 10;
  u16 style : 6;
} CharacterCell;

void Func_080456f8(u8 *base, s32 index, u32 value) {
  u8 *entry;
  s32 offset;
  s32 store_offset;
  s32 load_offset;

  offset = index * 28;
  entry = base + offset + 0x104;
  Text_LoadRemappedGlyph(base, value, index * 16, index * 16);
  store_offset = offset + 0x11C;
  *(u32 *)(base + store_offset) = value;
  /* FAKEMATCH(one-pass): the constant stored in a once-only block, after
   * the value; as a plain store, 4 of 47 instructions differ in tla-en. */
  do {
    *(u32 *)(entry + 4) = 0x80002000;
  } while (0);
  *(u32 *)(entry + 8) = 0;
  load_offset = offset + 0x110;
  ((CharacterCell *)(entry + 8))->code =
      Resource_LoadIndexedIntoBuffer(*(u16 *)(base + load_offset), value);
}

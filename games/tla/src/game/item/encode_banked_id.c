#include "types.h"

u8 *Item_Get(u32);

// XXX: what this encodes, and whether item ids here are wider than The Broken
// Seal's 9 bits; its callers would settle it.
u32 BattleSpoils_RankItem(u32 value) {
  u32 bank = 0;
  u32 base = value & 0x1ff;
  if (base == 0)
    return 0;
  {
    u8 flags = Item_Get(base)[3];

    if (flags & 8)
      bank = 1;
    bank <<= 1;
    if (flags & 4)
      bank++;
    bank <<= 9;
    bank += base;
  }
  return bank;
}

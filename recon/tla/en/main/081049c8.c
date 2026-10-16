// Draft, differ 2 of 71 (2026-10-15), ported from The Broken Seal's
// Djinn_CountTurns: at the end of the owner loop the native loads the owner
// count (ldrb r3) before it builds the 1 for the high-register increment (movs
// r2, #1; add r8, r2); here the 1 comes first. The increment is ahead of the
// test in RTL, so a barrier cannot order them.
#include "types.h"
#include "ram_buffer.h"

struct DjinnMenuWork {
  u8 unk000[0x204];
  u16 owners[8];
  u8 unk214[1];
  u8 ownerCount;
};

struct DjinnUnit {
  u8 unk000[0xf8];
  u32 djinnAvailable[4];
  u32 djinnActive[4];
};

struct DjinnUnit *UnitGet(s32 unitId);

void Func_081049c8(u8 *counts) {
  struct DjinnMenuWork *work;
  struct DjinnUnit *unit;
  s32 ownerIndex;
  s32 element;
  s32 bit;
  s32 count;

  work = Ram_HeapSlots->menuRuntime;
  for (ownerIndex = 0; ownerIndex < work->ownerCount; ownerIndex++) {
    unit = UnitGet(work->owners[ownerIndex]);
    count = 0;
    for (element = 0; element <= 3; element++) {
      for (bit = 0; bit <= 19; bit++) {
        if (unit->djinnActive[element] & (1 << bit))
          count++;
        else if (unit->djinnAvailable[element] & (1 << bit))
          count++;
      }
    }
    counts[ownerIndex] = count;
  }
}

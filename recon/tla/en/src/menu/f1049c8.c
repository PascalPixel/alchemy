// Func_081049c8: The Broken Seal's Djinn_CountTurns.
#include "types.h"
#include "ram_buffer.h"
#include "djinn_menu.h"

struct DjinnUnit {
  u8 unk000[0xf8];
  u32 djinnAvailable[4];
  u32 djinnActive[4];
};

struct DjinnUnit *Unit_Get(s32 unitId);

void Func_081049c8(u8 *counts) {
  struct DjinnMenuWork *work;
  struct DjinnUnit *unit;
  s32 ownerIndex;
  s32 element;
  s32 bit;
  s32 count;

  work = Ram_HeapSlots->menuRuntime;
  for (ownerIndex = 0; ownerIndex < work->memberCount; ownerIndex++) {
    unit = Unit_Get(work->owners[ownerIndex]);
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

// Near miss: differ 2 of 56 (one reordered row): the native loads the shop work
// (ldr r5, [r3]) before it builds the offset 0x507; the draft builds first. The
// first block holds sub sp, so a one-pass block on the load would force the
// prologue ahead (see 0810a9ac).
#include "types.h"
#include "ram_buffer.h"

s32 Shop_CanServe(s32 entry_id, s32 kind);

s32 Shop_CountUnits(void) {
  u8 *work = Ram_HeapSlots->menuRuntime;
  u8 *base;
  s32 active = 0;
  s32 variant = (s8)work[0x507];
  s32 index = 0;
  s32 offset;

  if (active < *(s8 *)(work + 0x504)) {
    base = work + 2;
    offset = 0x4c8;
    do {
      if (Shop_CanServe(*(s16 *)(base + offset), variant) != 0)
        active++;
      index++;
      offset += 2;
    } while (index < *(s8 *)(work + 0x504));
  }

  return active;
}

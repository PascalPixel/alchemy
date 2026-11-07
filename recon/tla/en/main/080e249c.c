// Matches (tla-en, 2026-11-07) with one tagged wrapper: ids indexed through an
// identity inline helper in the last loop loads ids[i] ahead of work->kind (found
// by permute). Before: differ 18 of 160, skeleton gap 2 (2026-11-01).
// Was left: only the two calls of the last loop. The native loads ids[i] (r2)
// ahead of work->kind, with sl in r4 and #30 in r3, and at the next call has
// sl in r3 and #24, #26 in r2, r4; this draft loads kind first, through r2
// and r0, and then uses r2, r4, r3. The instructions are the same set, so
// this is reload's choice of scratch registers: lreg has kind then ids[i],
// and a native r2 and r0 both look busy when kind is reloaded.
// Tried with no gain: a named id or kind local in any order (combine moves the
// load back down), real prototypes, s16 parameters, no prototype, a static
// inline around the call (a device, measured only), event at function
// scope, declaration orders, s32 or u16 swap, every sharing of the five loop
// counters. Lead: find what puts the ids[i] load before the kind load.
// What closed the rest, all plain C: the swap saves ids[i], as written below;
// cells indexed, so loop.c makes the pointer giv; the scan shares j with the
// sort, so cse2 turns count + 1 into j + 1 (the store's r4 is loop.c's giv of
// count, kept in r4 and saved at sp + 4 across the call: no store pointer in
// the source); layer read through a struct, because a char read depends on
// that caller save and sched2 then issues it before the shifts.
// Draft: runs the field events an effect touched (heap slot 0xe0 effect
// work). Each cell in the event work's list at 0xcc0 (x, z pairs ending in
// -1) is resolved to an event id at the owner object's layer byte; the
// distinct ids are sorted ascending, and for each one Func_080ce458 finds an
// event for type 0x50000005 and the effect's kind, which runs with the
// effect's two arguments.
#include "types.h"
#include "ram_buffer.h"

struct TouchOwner {
  u8 unk00[34];
  u8 layer;
};

struct TouchWork {
  u8 unk00[16];
  struct TouchOwner *owner;
  u8 unk14[4];
  s16 argument0;
  s16 argument1;
  u8 unk1c[2];
  s16 kind;
};

struct TouchCell {
  s16 x;
  s16 z;
};

s32 Func_080dbdc8(s32 x, s32 z, s32 layer);
s32 Func_080ce458(u32 type, s32 kind, s32 id);
void Field_RunPsynergyEvent(s32 event, s32 argument0, s32 argument1);

// Only for the compiler: see the FAKEMATCH tag in Func_080e249c.
static inline s32 Index(s32 i) { return i; }

void Func_080e249c(void) {
  struct TouchWork *work = Ram_HeapSlots->effectWork;
  struct TouchCell *cells =
      (struct TouchCell *)((u8 *)Ram_HeapSlots->eventWork + 0xcc0);
  struct TouchOwner *owner = work->owner;
  s32 count = 0;
  s16 ids[24];
  s32 i, j;

  for (i = 0; cells[i].x != -1; i++) {
    s16 id = Func_080dbdc8(cells[i].x << 20, cells[i].z << 20, owner->layer);

    for (j = 0; j < count; j++) {
      if (ids[j] == id)
        break;
    }
    if (j == count)
      ids[count++] = id;
  }
  ids[count] = -1;

  if (count > 1) {
    for (i = 0; i < count - 1; i++) {
      for (j = i + 1; j < count; j++) {
        if (ids[i] > ids[j]) {
          s16 swap = ids[i];

          ids[i] = ids[j];
          ids[j] = swap;
        }
      }
    }
  }

  for (i = 0; ids[i] != -1; i++) {
    /* FAKEMATCH(wrapper): ids[Index(i)]; as ids[i], 18 of 160 instructions
     * differ in tla-en. */
    s32 event = Func_080ce458(0x50000005, work->kind, ids[Index(i)]);

    if (event != 0)
      Field_RunPsynergyEvent(event, work->argument0, work->argument1);
  }
}

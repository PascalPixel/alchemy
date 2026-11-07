/// @file
/// Runs the field events an effect touched.

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

/// Resolves each cell in the event work's touched list to an event id at the
/// owner's layer, sorts the distinct ids and runs each one's event for the
/// effect's kind.
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

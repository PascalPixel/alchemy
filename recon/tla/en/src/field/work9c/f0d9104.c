#include "types.h"
#include "actor.h"
#include "ram_buffer.h"

struct LinkAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

struct LinkEnds {
  s16 first;
  s16 second;
  s16 unk04;
  s16 layer;
};

struct LinkEntry {
  u8 unk00[12];
  struct LinkEnds ends;
  u8 unk14[12];
};

void Func_080d9194(s32 slot, s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1);

// Link two objects.
void Func_080d9104(s32 slot, s32 firstId, s32 secondId) {
  struct LinkEntry *entries = Ram_HeapSlots->unk84[6];
  s32 layer = 2;
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != 0 && second != 0) {
    struct LinkEnds *ends;
    s32 lift;

    if (first->animationKind == 1)
      layer = ((struct LinkAnimation *)first->animation)->layer;
    ends = &entries[slot].ends;
    ends->first = firstId;
    ends->second = secondId;
    ends->layer = layer;
    lift = 128;
    /* FAKEMATCH(barrier): the lift built before the stack arguments load;
     * as plain statements, 10 of 69 instructions differ in tla-en. */
    asm volatile("" : "+l"(lift));
    lift <<= 12;
    Func_080d9194(slot, first->x, first->y + lift, first->z, second->x,
                  second->y + lift, second->z);
  }
}

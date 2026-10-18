// Near miss, differ 6 of 69, instruction order only: links two objects in a
// slot of the work at Ram_HeapSlots 0x9c and hands their positions, each
// raised by 0x80000, to Func_080d9194. Func_080d8d68 is the same body calling
// Func_080d8e08 and then Object_SetMode(, 2) on both objects, and Func_080d9710
// a third sibling. Left, as in Func_080da1b8: the native loads the work before
// it copies the first id, loads the second end's y after building 0x80000 and
// adds the raise to the first y straight after; this draft loads that y first
// and adds the first raise last. An empty asm in place of the tagged barrier
// measured 8, a one-pass block before the work's load 10 to 12.
#include "types.h"
#include "object_runtime.h"
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
  struct ObjectRuntime *first = ObjectTable_Get(firstId);
  struct ObjectRuntime *second = ObjectTable_Get(secondId);

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
    /* FAKEMATCH: starts 0x80000 before the stack arguments load, as the
     * native does (differ 12 without it). */
    asm volatile("" : "+l"(lift));
    lift <<= 12;
    Func_080d9194(slot, first->x, first->y + lift, first->z, second->x,
                  second->y + lift, second->z);
  }
}

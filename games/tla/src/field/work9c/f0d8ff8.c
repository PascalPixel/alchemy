/// @file
/// A slot's chain of nodes laid along a line between two points, and
/// Func_080d9104, which links two objects.

#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "ram_buffer.h"

s16 ArcTan2(s32 y, s32 x);

struct ChainNode {
  struct ChainNode *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
};

struct ChainSlot {
  u8 unk00[4];
  s16 segments;
  u8 unk06[22];
  struct ChainNode *head;
};

struct ChainWork {
  u8 unk00[12];
  struct ChainSlot slots[8];
};

/// Lays a slot's chain of nodes along the line between two points, bowed
/// sideways by a half sine of the given height.
s32 Func_080d8ff8(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                  s32 height) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  s32 dx = toX - x;
  s32 ax = dx >> 8;
  s32 dy = toY - y;
  struct ChainSlot *slot = &work->slots[index];
  struct ChainNode *node = slot->head;
  u16 angle = ArcTan2(ax, dy >> 8);
  s32 sin = Trig_Sin(angle);
  s32 cos = Trig_Cos(angle);
  s32 segments = slot->segments;
  s32 i;
  s32 visible = 1; // One constant for its whole life.
  {
    for (i = 0; i <= segments; i++) {
      s32 bow = Iwram_MulQ16(Trig_Sin((i << 15) / segments), height);

      node->x = x + i * dx / segments + Iwram_MulQ16(bow, cos);
      node->y = y + i * dy / segments + Iwram_MulQ16(bow, sin);
      node->z = z + i * (toZ - z) / segments;
      node->visible = visible;
      node = node->next;
    }
  }
  return 0;
}

#include "actor.h"

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
    /* FAKEMATCH(barrier): an empty asm on lift keeps its 128 built after the
     * layer's store and ahead of the coordinates' loads; without it, 10 of
     * 69 instructions differ in tla-en. */
    asm volatile("" : "+l"(lift));
    lift <<= 12;
    Func_080d9194(slot, first->x, first->y + lift, first->z, second->x,
                  second->y + lift, second->z);
  }
}

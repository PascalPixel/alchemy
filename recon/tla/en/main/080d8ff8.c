// Near miss, differ 25 of 123: lays a slot's chain of nodes along the line
// between two points (heap slot 0x9c work), bowed sideways by a half sine of
// the given height. Callers: Func_080d8fa8 and Func_080d9194. Left: order in
// the prologue (the argument spills, the dx >> 8 shift built before the slot
// address in the native) and, after the z store, a fresh copy of the node
// pointer (mov r2, r8) where the native reuses r3; register choice and order,
// no instruction missing. The sums are i * d / segments, which loop.c turns
// into the native's running sums.
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

s32 Func_080d8ff8(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                  s32 height) {
  struct ChainSlot *slot =
      &((struct ChainWork *)Ram_HeapSlots->unk84[6])->slots[index];
  s32 dx = toX - x;
  s32 dy = toY - y;
  struct ChainNode *node = slot->head;
  u16 angle = ArcTan2(dx >> 8, dy >> 8);
  s32 sin = Trig_Sin(angle);
  s32 cos = Trig_Cos(angle);
  s32 segments = slot->segments;
  s32 i;
  {
    for (i = 0; i <= segments; i++) {
      s32 bow = Iwram_MulQ16(Trig_Sin((i << 15) / segments), height);

      node->x = x + i * dx / segments + Iwram_MulQ16(bow, cos);
      node->y = y + i * dy / segments + Iwram_MulQ16(bow, sin);
      node->z = z + i * (toZ - z) / segments;
      node->visible = 1;
      node = node->next;
    }
  }
  return 0;
}

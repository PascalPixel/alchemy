// Near miss, differ 34 of 126: builds a slot's chain of nodes for the heap
// slot 0x9c work, one node for each 6 units of the distance between two
// points, laid out as a flat spiral around the first point. Func_080d9104
// calls it with two objects' positions; Func_080d8ff8 later moves the nodes
// along the line. Left: register choice, angle and the link pointer swapped
// (the native keeps the link in r9 and the angle in r7, so its link stores go
// through mov r2, r9), plus the argument spills' order. Declaring the angle
// before the link changes nothing.
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "ram_buffer.h"

s32 ArcTan2(s32 y, s32 x);

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

struct ChainNode *Func_080d8d40(void);

s32 Func_080d9194(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  s32 result = 0;
  s32 dx = (toX - x) >> 8;
  s32 dy = (toY - y) >> 8;
  s32 dz = (toZ - z) >> 8;
  s32 length = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                               Iwram_MulQ16(dz, dz));
  s32 segments = length / 6 + 1;
  struct ChainSlot *slot = &work->slots[index];
  struct ChainNode **link = &slot->head;
  s32 radius = 0x40000;
  s32 angle = 0;
  s32 i;

  slot->segments = segments;
  for (i = 0; i <= segments; i++) {
    struct ChainNode *node = Func_080d8d40();
    u16 step;

    if (node == NULL) {
      result = -1;
      break;
    }
    node->x = x + Iwram_MulQ16(Trig_Cos(angle), radius);
    node->y = y;
    node->z = z + Iwram_MulQ16(Trig_Sin(angle), radius);
    node->visible = 2;
    *link = node;
    step = ArcTan2(0x60000, radius);
    angle += step;
    radius += step * 4;
    link = &node->next;
  }
  *link = NULL;
  return result;
}

/// @file
/// Builds a chain of nodes around a point.

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

/// Builds a slot's chain of nodes, one for each 6 units of the distance between
/// two points, laid out as a flat spiral around the first point.
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
  s32 angle = 0;
  s32 radius = 0x40000;
  s32 i;
  s32 visible = 2; // One constant for its whole life.

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
    node->visible = visible;
    *link = node;
    step = ArcTan2(0x60000, radius);
    angle += step;
    link = &node->next;
    radius += step * 4;
  }
  *link = NULL;
  return result;
}

#include "work9c.h"
#include "macros.h"

void Func_080d92a4(s32 index, s32 *position) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot = &work->slots[index];
  struct Work9cNode *node = slot->head;
  s32 depth;

  depth = slot->depth;
  if (depth > 0) {
    do {
      depth--;
      node = node->next;
    } while (depth != 0);
  }
  SET_VECTOR(position, node->x, node->y, node->z);
}

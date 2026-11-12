/// @file
/// Routines first written as f0d9194.c; none has a twin in The Broken Seal.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

// A slot's chain of nodes built around a point, and Func_080d92a4, which gives
// the position of the node at the slot's depth.



/* VIEW(f0d9194): tag struct ChainNode as struct ChainNode__f0d9194; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5445: structure has no member named `visible'. */
struct ChainNode__f0d9194 {
  struct ChainNode__f0d9194 *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
};

/* VIEW(f0d9194): tag struct ChainSlot as struct ChainSlot__f0d9194; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5435: structure has no member named `segments'. */
struct ChainSlot__f0d9194 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[22];
  struct ChainNode__f0d9194 *head;
};



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
  struct ChainSlot__f0d9194 *slot = &work->slots[index];
  struct ChainNode__f0d9194 **link = &slot->head;
  s32 angle = 0;
  s32 radius = 0x40000;
  s32 i;
  s32 visible = 2; // One constant for its whole life.

  slot->segments = segments;
  for (i = 0; i <= segments; i++) {
    struct ChainNode__f0d9194 *node = Func_080d8d40();
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

// Near miss, differ 62 of 123, register choice and order: as in
// Func_080d9194's draft, the angle and the link pointer trade registers
// (the native keeps the link in r10 and the angle in r7), and the argument
// spills are ordered differently.
// The heap slot 0xa0 work's copy of Func_080d9194. Builds a slot's
// chain of nodes, one for each 6 units of the distance between two points,
// laid out as a flat spiral around the first point, every node kind 2. Unlike
// Func_080d9194 it records no segment count.
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "ram_buffer.h"

s16 ArcTan2(s32 y, s32 x);

struct LineNode {
  struct LineNode *next;
  s32 x;
  s32 y;
  s32 z;
  u8 kind;
  u8 slot;
};

struct LineSlot {
  s16 first;
  s16 second;
  s32 direction;
  s32 amplitude;
  u8 unk0c[4];
  s32 angle;
  s32 active;
  struct LineNode *head;
};

struct LineWork {
  u8 unk00[12];
  struct LineSlot slots[8];
};

struct LineNode *Func_080d8d40(void);

s32 Func_080da22c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct LineWork *work = Ram_HeapSlots->unk84[7];
  s32 result = 0;
  s32 dx = (toX - x) >> 8;
  s32 dy = (toY - y) >> 8;
  s32 dz = (toZ - z) >> 8;
  s32 length = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                               Iwram_MulQ16(dz, dz));
  s32 segments = length / 6 + 1;
  struct LineNode **link = &work->slots[index].head;
  s32 radius = 0x40000;
  s32 angle = 0;
  s32 i;

  for (i = 0; i <= segments; i++) {
    struct LineNode *node = Func_080d8d40();
    u16 step;

    if (node == NULL) {
      result = -1;
      break;
    }
    node->x = x + Iwram_MulQ16(Trig_Cos(angle), radius);
    node->y = y;
    node->z = z + Iwram_MulQ16(Trig_Sin(angle), radius);
    node->kind = 2;
    *link = node;
    step = ArcTan2(0x60000, radius);
    angle += step;
    radius += step * 4;
    link = &node->next;
  }
  *link = NULL;
  return result;
}

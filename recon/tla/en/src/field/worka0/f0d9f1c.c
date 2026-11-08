#include "types.h"
#include "call.h"
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

struct LineNode *Func_080d9e74(void);

s32 Func_080d9f1c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct LineWork *work = Ram_HeapSlots->unk84[7];
  s32 result = 0;
  s32 dx = toX - x;
  s32 sx = dx >> 8;
  s32 dy = toY - y;
  s32 sy = dy >> 8;
  s32 dz = toZ - z;
  s32 sz = dz >> 8;
  s32 segments = ValueLr1((s32 (*)())Iwram_SqrtEntry, (s32)(Iwram_MulQ16(sx, sx) + Iwram_MulQ16(sy, sy) +
                                 Iwram_MulQ16(sz, sz))) /
                     6 +
                 1;
  struct LineNode **link = &work->slots[index].head;
  s32 i;

  work->slots[index].direction = (u16)ArcTan2(sz, sx);
  for (i = 0; i <= segments; i++) {
    struct LineNode *node = Func_080d9e74();

    if (node == NULL) {
      result = -1;
      break;
    }
    node->slot = index + 1;
    node->x = x + i * dx / segments;
    node->y = y + i * dy / segments;
    node->z = z + i * dz / segments;
    if (i == segments)
      node->kind = 2;
    else
      node->kind = 1;
    *link = node;
    link = &node->next;
  }
  *link = NULL;
  work->slots[index].active = 1;
  return result;
}

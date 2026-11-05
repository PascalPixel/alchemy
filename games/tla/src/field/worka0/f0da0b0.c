#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "ram_buffer.h"

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

s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

s32 Func_080da0b0(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                  s32 height) {
  struct LineWork *work = Ram_HeapSlots->unk84[7];
  s32 dx = toX - x;
  s32 sx = dx >> 8;
  s32 dy = toY - y;
  s32 sy = dy >> 8;
  s32 dz = toZ - z;
  s32 sz = dz >> 8;
  s32 segments = Iwram_SqrtEntry(IwramMulQ16(sx, sx) + IwramMulQ16(sy, sy) +
                                 IwramMulQ16(sz, sz)) /
                     6 +
                 1;
  struct LineNode *node = work->slots[index].head;
  s32 i;
  s32 one = 1;

  for (i = 0; i <= segments; i += one) {
    s32 bow =IwramMulQ16(Trig_Sin((i << 15) / segments), height);

    node->x = x + i * dx / segments;
    node->y = y + i * dy / segments + bow;
    node->z = z + i * dz / segments;
    node->kind = one;
    node = node->next;
  }
  return 0;
}

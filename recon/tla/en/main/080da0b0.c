// Draft, differ 82 of 122, register choice and order: this draft keeps the
// IwramMulQ16 address in r11 from the length's squares through the loop,
// where the native loads it into r6 for the squares and again from the pool
// at each call in the loop, keeping the accumulated z delta in r11 instead;
// the deltas and their shifts follow in another order. The work loaded ahead
// of dx measured 82, after it 102.
// Moves the heap slot 0xa0 work's chain of nodes onto the line between
// two points, one segment for each 6 units of its length, bowed upward by a
// half sine of the given height; every node becomes kind 1. Func_080da060
// calls it with two objects' positions. The 0x9c work's Func_080d8ff8 is the
// sideways-bowed sibling.
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

s32 Func_080da0b0(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                  s32 height) {
  struct LineWork *work = Ram_HeapSlots->unk84[7];
  s32 dx = toX - x;
  s32 dy = toY - y;
  s32 dz = toZ - z;
  s32 sx = dx >> 8;
  s32 sy = dy >> 8;
  s32 sz = dz >> 8;
  s32 segments = Iwram_SqrtEntry(Iwram_MulQ16(sx, sx) + Iwram_MulQ16(sy, sy) +
                                 Iwram_MulQ16(sz, sz)) /
                     6 +
                 1;
  struct LineNode *node = work->slots[index].head;
  s32 i;

  for (i = 0; i <= segments; i++) {
    s32 bow = Iwram_MulQ16(Trig_Sin((i << 15) / segments), height);

    node->x = x + i * dx / segments;
    node->y = y + i * dy / segments + bow;
    node->z = z + i * dz / segments;
    node->kind = 1;
    node = node->next;
  }
  return 0;
}

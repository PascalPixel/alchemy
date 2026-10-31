// Near miss, differ 4 of 151 (tla-en, 2026-10-31): only the argument spills'
// order. The native stores r1, r2, r3 and r0 before it builds the work's address
// in r3, where this stores r3 first. In sched2 the store of r3 has priority 197
// and the other three 196: the address is three split insns that anti-depend on
// it, while the stores reach the work's load through one memory dependence; at
// equal priority the native order would follow (fewer dependents put r0 last).
// Unmoved by putting result or the differences before the work's load (worse),
// a u32 index, reading the slot through a cast, the loop counter's placement or
// the angle's mask; two 4-minute permuter runs found nothing better. The gap is
// the cost model: each spill reaches the work's load at cost 2 (a missing load
// after a store, -mthumb-slow-loads), the address adds 3 behind r3's store, and
// every later successor sits below that load, so no reshaping ties them. An
// empty asm opening the body ties the four stores but sorts r0 first and lifts
// the stack arguments' loads (parameter copies, ahead of the body) above the
// work's load: differ 16.
// Builds a slot's chain of nodes in the heap slot 0xa0 work, one node
// for each 6 units of the distance between two points, spread evenly along
// the line; the last node is kind 2, the others kind 1, and each records its
// slot as index + 1. Stores the line's direction and marks the slot active.
// The 0x9c work's Func_080d9194 is the spiral sibling.
#include "types.h"
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
  s32 segments = Iwram_SqrtEntry(Iwram_MulQ16(sx, sx) + Iwram_MulQ16(sy, sy) +
                                 Iwram_MulQ16(sz, sz)) /
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

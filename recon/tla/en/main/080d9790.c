// Draft, differ 6 of 225, skeleton gap 4 (tla-en, 2026-11-01): grows a slot's
// chain of nodes (heap slot 0x9c work) out from (x, y, z) a node a frame. It
// clears the slot's active halfword and its flag, copies the node list into a
// 0x200-byte buffer, puts both ends in mode 3, waits 4 frames, then each frame
// lays the nodes up to the frame's count on a flat spiral around the point
// (Func_080d9194's) and pulls each later node to within 6 units of the one
// before it. Allocation and frame match; two sched2 orders are left. The entry
// stores run y, z, x before the 192 load here z, the load, y, x (sched2 ranks z
// 14 and y 13; the native needs y above z, and no statement order, declaration
// order or loop spelling changed that). In the first loop the native copies the
// counter for its compare ahead of stmia; here after it (all do-while and
// separate-cursor spellings measured the same). Facts that fixed the rest: the
// distance shares radius's variable (r5 is overwritten on the path back to the
// spiral branch); lastX and lastY are read back from the node after its store;
// a 0 local set before the active test is the stores' zero; the stack order
// fixes the declaration order, segments to nodes. Work9c's header has no name
// for these slot fields, so the structs stay here.
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "actor.h"
#include "ram_buffer.h"

s32 ArcTan2(s32 y, s32 x);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);

struct ChainNode {
  struct ChainNode *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
  u8 unk13;
};

struct ChainSlot {
  u8 unk00[4];
  s16 segments;
  u8 unk06[2];
  s16 flag;
  s16 active;
  u8 unk0c[12];
  s32 unk18;
  struct ChainNode *head;
};

struct ChainWork {
  struct Actor *from;
  struct Actor *to;
  u8 unk08[4];
  struct ChainSlot slots[8];
};

s32 Func_080d9790(s32 index, s32 x, s32 y, s32 z) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  s32 zero = 0;
  struct ChainSlot *slot = &work->slots[index];
  struct ChainNode *node = slot->head;
  struct ChainNode **cursor;
  s32 i;
  s32 angle;
  s32 radius;
  s32 lastZ;
  s32 segments;
  s32 frame;
  s32 lastX;
  s32 lastY;
  struct ChainNode **nodes;

  if (slot->active == 0)
    return;
  slot->active = zero;
  segments = slot->segments;
  cursor = nodes = Heap_Alloc(0x200);
  slot->unk18 = zero;
  if (slot->flag != 0)
    GameFlag_ClearBit(slot->flag);
  for (i = 0; i <= segments; i++) {
    *cursor++ = node;
    node = node->next;
  }
  Actor_SetMode(work->from, 3);
  Actor_SetMode(work->to, 3);
  wait(4);
  for (frame = 0; frame <= segments; frame++) {
    angle = frame << 12;
    radius = 0x40000;
    for (i = 0; i <= segments; i++) {
      node = nodes[i];
      node->unk13 = 0;
      if (i <= frame) {
        u16 step;

        node->x = x + Iwram_MulQ16(Trig_Cos(angle), radius);
        lastX = node->x;
        node->y = y;
        lastY = node->y;
        node->z = z + Iwram_MulQ16(Trig_Sin(angle), radius);
        lastZ = node->z;
        node->visible = 2;
        step = ArcTan2(0x60000, radius);
        angle += step;
        radius += step * 4;
      } else {
        s32 dx = (lastX - node->x) >> 8;
        s32 dy = (lastY - node->y) >> 8;
        s32 dz = (lastZ - node->z) >> 8;

        radius = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                                 Iwram_MulQ16(dz, dz));

        if (radius <= 6)
          break;
        node->x = lastX + (node->x - lastX) * 6 / radius;
        lastX = node->x;
        node->y = lastY + (node->y - lastY) * 6 / radius;
        lastY = node->y;
        node->z = lastZ + (node->z - lastZ) * 6 / radius;
        node->visible = 2;
        lastZ = node->z;
      }
    }
    wait(1);
  }
  Heap_Release(nodes);
  return 0;
}

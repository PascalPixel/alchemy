// Draft, differ 227 of 225 (2026-10-24): grows a slot's chain of nodes (heap
// slot 0x9c work) out from (x, y, z) a node a frame. It clears the slot's
// active halfword and its flag, copies the node list into a 0x200-byte
// buffer, puts both ends in mode 3, waits 4 frames, then each frame lays the
// nodes up to the frame's count on a flat spiral around the point
// (Func_080d9194's) and pulls each later node to within 6 units of the one
// before it. Left: register choice. The native keeps IwramMulQ16's address in
// r11 (loop.c hoists it here too, but global.c leaves its pseudo unallocated
// and reload loads the pool at each call), the angle in r10, the radius in r5
// and the last node's x and y on the frame (sp 12, 8); here the last
// position takes three callee-saved registers and the angle and radius go
// to the frame. An array for the last x and y measured 192 to 198.
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "mem.h"
#include "object_runtime.h"
#include "ram_buffer.h"

s32 ArcTan2(s32 y, s32 x);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);

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
  struct ObjectRuntime *from;
  struct ObjectRuntime *to;
  u8 unk08[4];
  struct ChainSlot slots[8];
};

s32 Func_080d9790(s32 index, s32 x, s32 y, s32 z) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  struct ChainSlot *slot = &work->slots[index];
  struct ChainNode *node = slot->head;
  struct ChainNode **nodes;
  struct ChainNode **cursor;
  s32 segments;
  s32 frame;
  s32 i;
  s32 angle;
  s32 radius;
  s32 lastX;
  s32 lastY;
  s32 lastZ;

  if (slot->active == 0)
    return;
  slot->active = 0;
  segments = slot->segments;
  cursor = nodes = MemAlloc(0x200);
  slot->unk18 = 0;
  if (slot->flag != 0)
    GameFlag_ClearBit(slot->flag);
  for (i = 0; i <= segments; i++) {
    *cursor++ = node;
    node = node->next;
  }
  Object_SetMode(work->from, 3);
  Object_SetMode(work->to, 3);
  WaitFrames(4);
  for (frame = 0; frame <= segments; frame++) {
    angle = frame << 12;
    radius = 0x40000;
    for (i = 0; i <= segments; i++) {
      node = nodes[i];
      node->unk13 = 0;
      if (i <= frame) {
        u16 step;

        node->x = lastX = x + Iwram_MulQ16(Trig_Cos(angle), radius);
        node->y = lastY = y;
        node->z = lastZ = z + Iwram_MulQ16(Trig_Sin(angle), radius);
        node->visible = 2;
        step = ArcTan2(0x60000, radius);
        angle += step;
        radius += step * 4;
      } else {
        s32 dx = (lastX - node->x) >> 8;
        s32 dy = (lastY - node->y) >> 8;
        s32 dz = (lastZ - node->z) >> 8;
        s32 distance =
            Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                            Iwram_MulQ16(dz, dz));

        if (distance <= 6)
          break;
        node->x = lastX = lastX + (node->x - lastX) * 6 / distance;
        node->y = lastY = lastY + (node->y - lastY) * 6 / distance;
        node->z = lastZ = lastZ + (node->z - lastZ) * 6 / distance;
        node->visible = 2;
      }
    }
    WaitFrames(1);
  }
  MemFree(nodes);
  return 0;
}

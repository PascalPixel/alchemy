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
  if (segments >= 0) {
    i = segments + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter, which copies it
       * for the compare ahead of the store; as the plain counted loop, 2 of 225
       * instructions differ in tla-en. */
      asm("" : : "l"(i));
      *cursor++ = node;
      node = node->next;
    } while (i != 0);
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

void Func_080d9974(void) {}

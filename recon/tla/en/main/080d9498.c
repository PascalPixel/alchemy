// Draft, differ 291 (341 before statements reordered 2026-10-26) of 297 (2026-10-24): swings a slot's chain (heap slot
// 0x9c work) between (x, y, z) and the target point for twice its segment
// count of frames: the last node rides the line between the ends, raised by
// a sine wave across the line's heading (ArcTan2 of the distance), and each
// node before it is pulled to within 6 units of the next. It marks the slot
// active (1), and at the end sets its fields 0x0c, 0x14 and 0x18 and the
// slot's flag. The draft follows the listing's control flow (dz * frame is
// what loop.c strength-reduces into the frame's sp 8 to 16 sums); left is
// register and frame allocation, as in Func_080d9790: the native keeps
// IwramMulQ16 in r9, the last node's y and z in r11 and r10.
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "mem.h"
#include "ram_buffer.h"

s32 ArcTan2(s32 y, s32 x);
void GameFlag_SetBit(s32 flag);
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
  s32 unk0c;
  u8 unk10[4];
  s32 unk14;
  s32 unk18;
  struct ChainNode *head;
};

struct ChainWork {
  u8 unk00[12];
  struct ChainSlot slots[8];
};

s32 Func_080d9498(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  s32 dx = toX - x;
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  s32 dy = toY - y;
  s32 frame;
  s32 sine;
  s32 segments;
  s32 shiftedX = dx >> 8;
  s32 dz = toZ - z;
  s32 shiftedY = dy >> 8;
  struct ChainSlot *slot = &work->slots[index];
  struct ChainNode *node = slot->head;
  struct ChainNode **cursor;
  s32 i;
  u16 angle;
  struct ChainNode **nodes;
  s32 cosine;
  s32 lastX, lastY, lastZ;

  if (slot->active == 1)
    return;
  slot->active = 1;
  segments = slot->segments;
  cursor = nodes = MemAlloc(0x200);
  for (i = 0; i <= segments; i++) {
    *cursor++ = node;
    node = node->next;
  }
  angle = ArcTan2(shiftedX, shiftedY);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      node->unk13 = index + 1;
      if (i == segments) {
        s32 wave = Iwram_MulQ16(Trig_Sin((frame << 14) / i), 0x80000);
        s32 offset = Iwram_MulQ16(wave, cosine);

        node->x = lastX = x + dx * frame / 2 / i + offset;
        offset = Iwram_MulQ16(wave, sine);
        node->y = lastY = y + dy * frame / 2 / i + offset;
        node->z = lastZ = z + dz * frame / 2 / i;
      } else {
        s32 ez = (lastZ - node->z) >> 8;
        s32 ex = (lastX - node->x) >> 8;
        s32 ey = (lastY - node->y) >> 8;
        s32 distance =
            Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                            Iwram_MulQ16(ez, ez));

        if (distance > 6) {
          node->x = lastX = lastX + (node->x - lastX) * 6 / distance;
          node->y = lastY = lastY + (node->y - lastY) * 6 / distance;
          node->z = lastZ = lastZ + (node->z - lastZ) * 6 / distance;
          node->visible = 1;
        }
      }
    }
    WaitFrames(1);
  }
  slot = &work->slots[index];
  slot->unk18 = 1;
  slot->unk0c = 0x40000;
  slot->unk14 = 0x4000;
  if (slot->flag != 0)
    GameFlag_SetBit(slot->flag);
  MemFree(nodes);
  return 0;
}

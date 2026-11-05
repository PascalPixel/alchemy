#include "types.h"
#include "fixed_math.h"
#include "flag_call.h"
#include "iwram_call.h"
#include "ram_buffer.h"


struct RopeNode {
  struct RopeNode *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  u8 kind;
  u8 slot;
};

struct RopeSlot {
  s16 id;
  s16 value;
  s16 count;
  u8 unk06[2];
  s16 flag;
  s16 state;
  s32 amplitude;
  u8 unk10[4];
  s32 phase;
  s32 active;
  struct RopeNode *head;
};

struct RopeWork {
  u8 unk000[12];
  struct RopeSlot slots[8];
};

struct RopeNode *Func_080d8d40(void);
s32 ArcTan2(s32 x, s32 y);

s32 Func_080d8e08(s32 index, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2) {
  struct RopeWork *work = Ram_HeapSlots->unk84[6];
  s32 result = 0;
  s32 dx = x2 - x1;
  s32 sx = dx >> 8;
  s32 dy = y2 - y1;
  s32 sy = dy >> 8;
  s32 dz = z2 - z1;
  s32 sz = dz >> 8;
  s32 count;
  struct RopeSlot *slot;
  struct RopeNode **link;
  struct RopeNode *node;
  u16 angle;
  s32 sin;
  s32 cos;
  s32 i;

  count = Iwram_Sqrt(Iwram_MulQ16(sx, sx) + Iwram_MulQ16(sy, sy) +
                     Iwram_MulQ16(sz, sz)) /
              6 +
          1;
  slot = &work->slots[index];
  link = &slot->head;
  angle = ArcTan2(sx, sy);
  sin = Trig_Sin(angle);
  cos = Trig_Cos(angle);
  slot->count = count;
  for (i = 0; i <= count; i++) {
    s32 bow;

    node = Func_080d8d40();
    if (node == NULL) {
      result = -1;
      break;
    }
    bow = Iwram_MulQ16(Trig_Sin((i << 16) / count), 0x10000);
    node->x = x1 + i * dx / count + Iwram_MulQ16(bow, cos);
    node->y = y1 + i * dy / count + Iwram_MulQ16(bow, sin);
    node->z = z1 + i * dz / count;
    node->slot = index + 1;
    if (i == count)
      node->kind = 2;
    else
      node->kind = 1;
    *link = node;
    link = &node->next;
  }
  *link = NULL;
  slot->state = 1;
  slot->active = 1;
  slot->amplitude = 0x10000;
  slot->phase = 0x4000;
  if (slot->flag != 0)
    GameFlag_SetBit(slot->flag);
  return result;
}

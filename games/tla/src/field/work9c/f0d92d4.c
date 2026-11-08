#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "actor.h"
#include "ram_buffer.h"

struct ChainSlot {
  u8 unk00[4];
  s16 segments;
  u8 unk06[18];
  s32 unk18;
  void *head;
};

struct ChainWork {
  u8 unk00[12];
  struct ChainSlot slots[8];
};

typedef void (*TargetHook)(struct Actor *, s32 *, s32 *);

extern s32 gData_080f393c;

void wait(s32 frames);
void Func_080d9498(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                   s32 wobble);
void Func_080d9790(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                   s32 wobble);

void Func_080d92d4(s32 index, s32 fromId, s32 toId) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  struct Actor *from = ObjectTable_Get(fromId);
  struct Actor *to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    s32 toX = to->x;
    s32 toY = to->y;
    s32 toZ = to->z;
    struct ChainSlot *slot = &work->slots[index];
    s32 x, y = 0, z; // Default: y.
    s32 xx, yy, length, segments, count;
    s32 (*mul)(s32, s32);

    if (((u8 *)to)[99] == 99)
      (*(TargetHook *)((u8 *)to + 104))(to, &toX, &toZ);
    x = (toX - from->x) >> 8;
    y = (toY - from->y) >> 8;
    z = (toZ - from->z) >> 8;
    mul = Iwram_MulQ16;
    xx = mul(x, x);
    yy = mul(y, y);
    length = xx + yy + mul(z, z);
    segments = Iwram_SqrtEntry(length) / 6 + 1;
    count = slot->segments;
    if (segments > count) {
      count++;
      x = from->x + (toX - from->x) * count / segments;
      y = toY;
      z = from->z + (toZ - from->z) * count / segments;

      Func_080d9498(index, from->x, from->y + 0x80000, from->z, x, y + 0x80000,
                    z, Trig_Sin(gData_080f393c) << 2);
      slot->unk18 = 0;
      wait(1);
      Func_080d9790(index, from->x, from->y + 0x80000, from->z, x, y + 0x80000,
                    z, Trig_Sin(gData_080f393c) << 2);
    } else {
      Func_080d9498(index, from->x, from->y + 0x80000, from->z, to->x,
                    to->y + 0x80000, to->z, Trig_Sin(gData_080f393c) << 2);
      gData_080f393c += 0x1000;
      wait(6);
      Actor_SetMode(from, 2);
      Actor_SetMode(to, 2);
    }
  }
}

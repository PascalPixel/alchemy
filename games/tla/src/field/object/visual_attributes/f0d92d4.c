/// @file
/// One routine of the chain work, not yet understood.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "module/field/field.h"

/* VIEW(f0d92d4): tag struct ChainSlot as struct ChainSlot__f0d92d4; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5515: structure has no member named `segments'. */
struct ChainSlot__f0d92d4 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[18];
  s32 unk18;
  void *head;
};

/* VIEW(f0d92d4): tag struct ChainWork as struct ChainWork__f0d92d4; visual_attributes.c has another definition. */
struct ChainWork__f0d92d4 {
  u8 unk00[12];
  struct ChainSlot__f0d92d4 slots[8];
};

extern s32 gData_080f393c;

/* VIEW(f0d92d4): routine Func_080d9498 as void Func_080d9498(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ, s32 wobble); the definition has s32 Func_080d9498(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ). */
extern void Func_080d9498__f0d92d4(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                   s32 wobble) __asm__("Func_080d9498");
/* VIEW(f0d92d4): routine Func_080d9790 as void Func_080d9790(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ, s32 wobble); the definition has s32 Func_080d9790(s32 index, s32 x, s32 y, s32 z). */
extern void Func_080d9790__f0d92d4(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                   s32 wobble) __asm__("Func_080d9790");

void Func_080d92d4(s32 index, s32 fromId, s32 toId) {
  struct ChainWork__f0d92d4 *work = Ram_HeapSlots->unk84[6];
  struct Actor *from = ObjectTable_Get(fromId);
  struct Actor *to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    s32 toX = to->x;
    s32 toY = to->y;
    s32 toZ = to->z;
    struct ChainSlot__f0d92d4 *slot = &work->slots[index];
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

      Func_080d9498__f0d92d4(index, from->x, from->y + 0x80000, from->z, x, y + 0x80000,
                    z, Trig_Sin(gData_080f393c) << 2);
      slot->unk18 = 0;
      wait(1);
      Func_080d9790__f0d92d4(index, from->x, from->y + 0x80000, from->z, x, y + 0x80000,
                    z, Trig_Sin(gData_080f393c) << 2);
    } else {
      Func_080d9498__f0d92d4(index, from->x, from->y + 0x80000, from->z, to->x,
                    to->y + 0x80000, to->z, Trig_Sin(gData_080f393c) << 2);
      gData_080f393c += 0x1000;
      wait(6);
      Actor_SetMode(from, 2);
      Actor_SetMode(to, 2);
    }
  }
}

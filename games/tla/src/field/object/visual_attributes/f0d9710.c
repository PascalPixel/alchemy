/// @file
/// Routines first written as f0d9710.c; none has a twin in The Broken Seal.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

/* VIEW(f0d9710): tag struct ChainWork as struct ChainWork__f0d9710; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5671: structure has no member named `from'. */
struct ChainWork__f0d9710 {
  struct Actor *from;
  struct Actor *to;
};

extern s32 gData_080f3940;

/* VIEW(f0d9710): routine Func_080d9790 as void Func_080d9790(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ, s32 wobble); the definition has s32 Func_080d9790(s32 index, s32 x, s32 y, s32 z). */
extern void Func_080d9790__f0d9710(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                   s32 wobble) __asm__("Func_080d9790");

void Func_080d9710(s32 index, s32 fromId, s32 toId) {
  struct ChainWork__f0d9710 *work;
  struct Actor *from;
  struct Actor *to;

  work = Ram_HeapSlots->unk84[6];
  from = ObjectTable_Get(fromId);
  to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    work->from = from;
    work->to = to;
    Func_080d9790__f0d9710(index, from->x, from->y + 0x80000, from->z, to->x,
                  to->y + 0x80000, to->z, Trig_Sin(gData_080f3940) * 4);
    gData_080f3940 += 0x1000;
  }
}

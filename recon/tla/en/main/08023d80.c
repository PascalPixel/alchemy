// Draft, differ 14 of 71: draws a field actor's sprite (kind 1, unless
// flag 0x10 hides it) or its four sprites (kind 2) through Func_0802254c at
// the actor's depth plus the given offset, with the fixed scale
// gData_0802eb90 while game flag 363 is set. No ☀️ twin found. Left: the
// native's switch tests kind < 1 before kind == 2 (cmp, ble; the draft
// has no such test, also as an s8 or a 4-bit field: 14 and 22), and in kind
// 1 it sets up the sprite and position arguments before the hidden test;
// both other than that are order.
#include "types.h"

struct FieldActor {
  u8 unk00[6];
  u16 depth;
  s32 position[4];
  s32 scale[2];
  u8 unk20[2];
  u8 palette;
  u8 flags;
  u8 unk24[44];
  void *sprite;
  u8 kind;
};

extern const s32 gData_0802eb90[2];

s32 GameFlag_Test(s32 flag);
void Func_0802254c(void *sprite, s32 *position, const s32 *scale, s32 depth,
                   s32 palette);

void Func_08023d80(struct FieldActor *actor, s32 depth) {
  const s32 *scale = actor->scale;
  void **sprites;
  s32 i;

  if (GameFlag_Test(363))
    scale = gData_0802eb90;
  switch (actor->kind & 15) {
  case 1:
    if (actor->flags & 0x10)
      break;
    Func_0802254c(actor->sprite, actor->position, scale, actor->depth + depth,
                  actor->palette);
    break;
  case 2:
    sprites = actor->sprite;
    for (i = 3; i >= 0; i--) {
      void *sprite = *sprites++;

      if (sprite != 0)
        Func_0802254c(sprite, actor->position, scale, actor->depth + depth,
                      actor->palette);
    }
    break;
  }
}

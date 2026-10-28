#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "field_sprite.h"
#include "io_reg.h"

struct Actor *Actor_GetById(s32 id);
struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z,
                                                  s32 type);
void OverlayObject_WaitUntilIdle(struct Actor *object);
void Func_02003310(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003398(void);
void Func_020033a0(void);
void Func_020032f8(struct Actor *object);
void Func_020033e0(s32 id, s32 x, s32 z);
void Func_02003488(s32 a);
void Func_020034b8(s32 cue);

struct SpriteView {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 unk4 : 4;
  u8 unk06[14];
  s32 unk14;
};

void Func_020019a8(void) {
  struct Actor *object;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 position[3];

  if (gSequenceParentWork.entrance == 1)
    object = Actor_GetById(8);
  else if (gSequenceParentWork.entrance == 3)
    object = Actor_GetById(9);
  else
    object = Actor_GetById(10);
  position[0] = (object->x & 0xfff00000) + 0x80000;
  position[2] = (object->z & 0xfff00000) + 0x80000;
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Func_02003398();
  Func_02003488(0);
  Func_02003310(object, position[0], 0, position[2]);
  effect =
      OverlayObject_PrepareObject(object->x, 0, position[2] + 0x3b0000, 286);
  effect->scaleX = 0x18000;
  effect->scaleY = 0x18000;
  /* FAKEMATCH(barrier): the sprite pointer loads after the two scale stores,
   * as in the native; as plain statements, 6 of 98 instructions differ in
   * tla-en. */
  asm("");
  sprite = effect->animation;
  ((struct SpriteView *)sprite)->mode = 2;
  /* FAKEMATCH(one-pass): REG_WINOUT is stored before REG_DISPCNT's address is
   * built; as a plain store, 2 of 98 instructions differ in tla-en. */
  do {
    REG_WINOUT = 0x2f3f;
  } while (0);
  REG_DISPCNT |= 0x8000;
  Func_020034b8(204);
  object->motionFlags = 3;
  OverlayObject_WaitUntilIdle(object);
  object->x = 0;
  object->y = 0;
  object->z = 0;
  Func_020033e0(8, 0, 0);
  Func_020032f8(effect);
  Func_020033a0();
}

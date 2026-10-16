// Near miss, differ 16 (2026-10-15): written from the listing; scheduling is
// left. The listing loads the sprite pointer (effect->animation) after the two
// scale stores, where this source loads it before them; it stores REG_WINOUT
// before building REG_DISPCNT's address, where this source stores it after; and
// it places its mid-function literal pool (with the branch around it) before
// the Func_020033e0 call, where this source places it after.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_sprite.h"
#include "io_reg.h"

struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *OverlayObject_PrepareObject(s32 x, s32 y, s32 z,
                                                  s32 type);
void OverlayObject_WaitUntilIdle(struct ObjectRuntime *object);
void Func_02003310(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02003398(void);
void Func_020033a0(void);
void Func_020032f8(struct ObjectRuntime *object);
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
  struct ObjectRuntime *object;
  struct ObjectRuntime *effect;
  struct FieldSprite *sprite;
  s32 position[3];

  if (gPartyState.entrance == 1)
    object = Object_GetById(8);
  else if (gPartyState.entrance == 3)
    object = Object_GetById(9);
  else
    object = Object_GetById(10);
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
  sprite = effect->animation;
  ((struct SpriteView *)sprite)->mode = 2;
  REG_WINOUT = 0x2f3f;
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

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "io_reg.h"
#include "event_inline.h"

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z,
                                                  s32 type);
void OverlayObject_WaitUntilIdle(struct Actor *object);
void Func_02003398(void);
void Func_020033a0(void);
void Func_020032f8(struct Actor *object);

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

  if (gMainRoutineWork.entrance == 1)
    object = Actor_Get(8);
  else if (gMainRoutineWork.entrance == 3)
    object = Actor_Get(9);
  else
    object = Actor_Get(10);
  position[0] = (object->x & 0xfff00000) + 0x80000;
  position[2] = (object->z & 0xfff00000) + 0x80000;
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetMoveTarget(object, position[0], 0, position[2]);
  effect =
      OverlayObject_PrepareObject(object->x, 0, position[2] + 0x3b0000, 286);
  effect->scaleX = 0x18000;
  effect->scaleY = 0x18000;
  sprite = effect->animation;
  ((struct SpriteView *)sprite)->mode = 2;
  REG_WINOUT = WINOUT_WIN01_ALL | WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 |
               WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3 | WINOUT_WINOBJ_CLR;
  REG_DISPCNT |= DISPCNT_OBJWIN_ON;
  Event_PlaySound(204);
  object->motionFlags = 3;
  OverlayObject_WaitUntilIdle(object);
  object->x = 0;
  object->y = 0;
  object->z = 0;
  Actor_SetPosition(8, 0, 0);
  Func_020032f8(effect);
  Func_020033a0();
}

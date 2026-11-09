#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern char SceneId_Ov676Room1;

struct TileSprite {
  u8 unk00[18];
  u16 tile;
};

void Func_020023c8(void);

void Func_020022f8(void) {
  s32 step;
  s32 i;
  struct Actor *object = Actor_Get(9);
  struct TileSprite *sprite = object->animation;

  Event_SetFlag(0x300);
  object->acceleration = 0x18000;
  object->speedLimit = 0x80000;
  Event_PlaySound(227);
  FieldActor_SetMoveTarget(object, 0xb00000, 0x280000, 0xb80000);
  step = 0;
  for (i = 15; i >= 0; i--) {
    sprite->tile -= step;
    System_Wait(1);
    step += 40;
  }
  FieldActor_SetMoveTarget(object, 0xa80000, 0x100000, 0xb80000);
  step = 400;
  for (i = 25; i >= 0; i--) {
    sprite->tile -= step;
    System_Wait(1);
    step += 40;
  }
  Actor_WaitForMove(object);
  FieldEvent_Wait(2);
  Event_PlaySound(240);
  i = 0;
  sprite->tile = i;
  object->x = 0xa80000;
  object->z = 0xc00000;
  object->y = i;
  object->velocityY = i;
  object->velocityX = i;
  MapGrid_CopyCellAttributes(8, 12, 1, 1, 10, 12);
}

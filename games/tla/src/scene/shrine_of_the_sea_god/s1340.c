#include "../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"

extern s32 gData_02004a18[3];

void Func_02001340(struct ShrineOfTheSeaGodObject *object,
                   struct ShrineOfTheSeaGodObject *other) {
  struct ShrineOfTheSeaGodSprite *sprite = object->sprite;
  u32 n;
  u32 i;

  Func_080d489c(gMainRoutineWork.currentOwner, 258);
  Sound_Play(210);
  object->motionFlags = 0;
  for (n = 0; n <= 1; n++) {
    other->y -= 0x10000;
    for (i = 0; i <= 3; i++) {
      sprite->rotation += 128;
      wait(1);
    }
    other->y += 0x10000;
    for (i = 0; i <= 7; i++) {
      sprite->rotation -= 128;
      wait(1);
    }
    other->y -= 0x10000;
    for (i = 0; i <= 3; i++) {
      sprite->rotation += 128;
      wait(1);
    }
  }
  other->y += 0x10000;
  wait(20);
  gData_02004a18[0] = other->x - object->x;
  gData_02004a18[1] = other->y - object->y;
  gData_02004a18[2] = other->z - object->z;
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
}

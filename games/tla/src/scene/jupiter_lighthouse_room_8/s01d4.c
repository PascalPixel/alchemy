#include "types.h"
#include "field_event.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Scene_SetArrivalFlags(s32 value);
struct FieldActor *Object_GetById(s32 id);
void Func_020021e0(s32 x, s32 z, s32 a, s32 b);
void Func_02002228(s32 id, s32 x, s32 z);
void Func_020022c0(s32 x, s32 z, s32 a, s32 b);

static __inline__ void Func_02002228_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02002228(id, x, z);
}

s32 Func_020001d4(void) {
  Scene_SetArrivalFlags(0);
  if (GameFlag_Test(0x109) == 0)
    GameFlag_ClearBit(0xa18);
  if (GameFlag_Test(0xa18)) {
    struct FieldActor *object = Object_GetById(10);

    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    Func_02002228_(10, 0x680000, 0xe80000);
    Func_020021e0(object->x, object->z, 0, 0);
    Func_020022c0(object->x, object->z, 0, 128);
  }
  return 0;
}

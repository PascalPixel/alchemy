#include "types.h"
#include "field_event.h"
#include "scene_tables.h"
#include "event_inline.h"

void Scene_SetArrivalFlags(s32 value);
void Func_020021e0(s32 x, s32 z, s32 a, s32 b);
void Func_020022c0(s32 x, s32 z, s32 a, s32 b);

s32 Func_020001d4(void) {
  Scene_SetArrivalFlags(0);
  if (GameFlag_Test(0x109) == 0)
    GameFlag_ClearBit(0xa18);
  if (GameFlag_Test(0xa18)) {
    struct FieldActor *object = ((struct FieldActor *)Actor_GetById(10));

    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    Actor_SetPosition(10, PIXELS(104), PIXELS(232));
    Func_020021e0(object->x, object->z, 0, 0);
    Func_020022c0(object->x, object->z, 0, 128);
  }
  return 0;
}

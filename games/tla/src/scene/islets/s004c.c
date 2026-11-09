#include "islets.h"
#include "scene_tables.h"
#include "event_inline.h"

u8 *Func_0200004c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov674Room1)
    return gData_0200476c;
  if (scene == (s32)&SceneId_Ov674Room2)
    return gData_020047e4;
  if (scene == (s32)&SceneId_Ov674Room3)
    return gData_0200494c;
  if (scene == (s32)&SceneId_Ov674Room4)
    return gData_020049c4;
  if (scene == (s32)&SceneId_Ov674Room5) {
    if (Event_GetFlag(0xaa5))
      gData_02004a84[46] = 1;
    return gData_02004a84;
  }
  return gData_02004754;
}

void Func_020000d8(void) {
  struct IsletsTarget target;
  struct Actor *object = Actor_Get(10);

  if (Func_020037d8(&target)) {
    Func_02003a5c(target);
    System_Wait(5);
    object->gravity = 0x8000;
    Actor_SetAnimation(10, 3);
    Actor_SetDestinationOffset(10, 0, 8);
    FieldEvent_Wait(22);
    Event_PlaySound(240);
    Actor_SetAnimation(10, 8);
    object->priorityFlags |= 2;
    object->motionFlags = 0;
    object->velocityY = 0;
    object->y = PIXELS(-8);
    object->terrainHeight = PIXELS(-8);
    MapGrid_CopyCellAttributes(9, 1, 4, 1, 9, 10);
    MapGrid_CopyCellAttributes(9, 8, 4, 1, 9, 9);
    MapGrid_CopyCellAttributes(9, 64, 4, 1, 9, 74);
    Event_SetFlag(0xa2c);
  }
}

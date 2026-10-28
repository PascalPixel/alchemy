#include "izumo_ruins.h"

s32 Func_020012dc(void) {
  struct IzumoRuinsEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 height;

  work->field1ac = 0x204;
  Actor_GetById(gAdventure.currentOwner)->priorityFlags |= 0x20;
  Func_020014e0(0);
  if (gAdventure.scene == (s32)&SceneId_Ov67FRoom1) {
    Func_02001510(gData_02001654);
    Func_0200100c((u16 *)gData_02001674);
    Func_020014f8(gData_02001696);
  } else {
    Actor_GetById(10)->priorityFlags |= 0x20;
    Actor_GetById(11)->priorityFlags |= 0x20;
    Func_02001510(gData_02001658);
    Func_0200100c((u16 *)gData_02001688);
    if (GameFlag_Test(0x941)) {
      Call6((void (*)())Func_02001460, 7, 2, 8, 7, 7, 12);
      Call6((void (*)())Func_02001450, 7, 66, 7, 76, 8, 9);
      Func_02001448();
      wait(1);
      if (GameFlag_Test(0x109)) {
        object = Actor_GetById(gAdventure.currentOwner);
        height = Map_GetTerrainHeight(object->terrainId, object->x,
                                      object->z - object->y);
        object->terrainHeight = height;
        object->y = height;
        Camera_FollowActor(gAdventure.currentOwner, 0);
      }
    }
  }
  return 0;
}

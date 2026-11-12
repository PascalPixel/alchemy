/// @file
/// Scene code of overlay 0x661, in ROM order, from s1bec's offset up to the next object the overlay links.

#include "scene.h"

void Func_02001bec(void) {
  struct FieldActor *object;

  Func_02004040();
  Func_020041b8(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  object = Func_02001b70();
  PartyInventory_GiveItem(209, 0);
  if (object != 0) {
    Actor_SetPositionAndResetMotion((struct Actor *)object, 0, 0, 0);
    wait(1);
    Func_02003fc8(object);
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  GameFlag_SetBit(0x88c);
  Func_02004048();
}

void Func_02001c5c(void) {
  struct FieldActor *object;
  s16 scene;

  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->unk16a = gData_02004808;
  if (gData_0200480c != 0) {
    scene = gMainRoutineWork.scene;
    if (scene == (s32)&SceneId_Ov661Room4)
      object = Actor_GetById(16);
    else if (scene == (s32)&SceneId_Ov661Room5)
      object = Actor_GetById(21);
    else
      return;
    gData_0200480c->x = object->x;
    gData_0200480c->z = object->z;
    gData_0200480c->facing = object->facing;
  }
}

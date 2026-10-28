#include "scene.h"

void Func_02001c5c(void) {
  struct FieldActor *object;
  s16 scene;

  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->unk16a = gData_02004808;
  if (gData_0200480c != 0) {
    scene = gAdventure.scene;
    if (scene == (s32)&SceneId_Ov661Room4)
      object = Object_GetById(16);
    else if (scene == (s32)&SceneId_Ov661Room5)
      object = Object_GetById(21);
    else
      return;
    gData_0200480c->x = object->x;
    gData_0200480c->z = object->z;
    gData_0200480c->facing = object->facing;
  }
}

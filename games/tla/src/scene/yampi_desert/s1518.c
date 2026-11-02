#include "scene.h"
#include "scene_tables.h"

void Func_02001518(void) {
  struct FieldActor *object = Actor_GetById(64);
  struct FieldActor *leader;

  if (object != 0) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    Scene_Call4090(64, PIXELS(288), PIXELS(240));
    if ((leader->x + 0x80000) >> 20 == (object->x + 0x80000) >> 20 &&
        (leader->z + 0x80000) >> 20 == (object->z + 0x80000) >> 20)
      Func_02004090(gMainRoutineWork.currentOwner, PIXELS(288), PIXELS(256));
  }
}

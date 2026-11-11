#include "scene.h"

void Func_02002010(void) {
  struct FieldActor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x;
  s32 y;
  s32 z;

  y = leader->y;
  if (y > 0x100000)
    gMainRoutineWork.unk2c3 = 1;
  else
    gMainRoutineWork.unk2c3 = 0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room3) {
    Actor_SetSpritePriority(18, leader->sprite->priority);
    x = leader->x;
    if (x > 0x1d80000) {
      z = leader->z;
      if (z <= 0x1e80000)
        return;
      if (leader->y > (s32)0xfff00000)
        gMainRoutineWork.unk2c3 = 1;
      else
        gMainRoutineWork.unk2c3 = 0;
    }
  }
}

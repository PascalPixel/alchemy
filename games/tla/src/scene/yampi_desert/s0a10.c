#include "scene.h"

void Func_02000a10(void) {
  struct Actor *leader =
      (struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner);
  s16 scene;

  leader->terrainId = 0;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov661Room1)
    Func_0200095c((u16 *)gData_02004768);
  else if (scene == (s32)&SceneId_Ov661Room3)
    Func_0200095c((u16 *)gData_0200478e);
  else if (scene == (s32)&SceneId_Ov661Room4)
    Func_0200095c((u16 *)gData_020047d2);
  else if (scene == (s32)&SceneId_Ov661Room5)
    Func_0200095c((u16 *)gData_020047ec);
}

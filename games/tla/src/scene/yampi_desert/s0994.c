#include "scene.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Func_02000994(void) {
  s16 scene;

  BYTE_AT(Actor_GetById(gMainRoutineWork.currentOwner), 34) = 2;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov661Room1)
    Func_02000918((u16 *)gData_02004768);
  else if (scene == (s32)&SceneId_Ov661Room3)
    Func_02000918((u16 *)gData_0200478e);
  else if (scene == (s32)&SceneId_Ov661Room4)
    Func_02000918((u16 *)gData_020047d2);
  else if (scene == (s32)&SceneId_Ov661Room5)
    Func_02000918((u16 *)gData_020047ec);
}

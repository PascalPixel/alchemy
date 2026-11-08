#include "scene.h"
#include "scene_ids.h"

extern s32 gData_02003d20[];
extern s32 gData_02003e7c[];
extern s32 gData_02003ffc[];
extern s32 gData_02004104[];

void Func_02000464(s32 a) { Func_02003474(a, 326, 0x978); }

s32 *Func_02000478(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov685Room1)
    return gData_02003d20;
  if (scene == (s32)&SceneId_Ov685Room2)
    return gData_02003e7c;
  if (scene == (s32)&SceneId_Ov685Room3)
    return gData_02003ffc;
  if (scene == (s32)&SceneId_Ov685Room4)
    return gData_02004104;
  return gData_02003d20;
}

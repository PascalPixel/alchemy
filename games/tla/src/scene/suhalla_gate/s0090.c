#include "types.h"
#include "main_routine_work.h"

extern char SceneId_SuhallaGate1;
extern char SceneId_SuhallaGate2;
extern char SceneId_SuhallaGate3;

extern u8 gData_02000f10[];
extern u8 gData_02000f58[];
extern u8 gData_02001048[];
extern u8 gData_02001078[];
extern u8 gData_020010e4[];
extern u8 gData_02001180[];

u8 *Func_02000090(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_SuhallaGate2)
    return gData_02000f58;
  if (scene == (s32)&SceneId_SuhallaGate1)
    return gData_02001048;
  return gData_02000f10;
}

u8 *Func_020000d0(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_SuhallaGate2)
    return gData_020010e4;
  if (scene == (s32)&SceneId_SuhallaGate3)
    return gData_02001180;
  return gData_02001078;
}

#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"

extern const u8 gData_02006e9c[];
extern const u8 gData_02006b54[];
extern const u8 gData_02006b6c[];
extern const u8 gData_02006bd8[];
extern const u8 gData_02006c80[];
extern const u8 gData_02006d1c[];
extern const u8 gData_02006db8[];
extern const u8 gData_02006e78[];

const u8 *Func_02001124(void) {
  s16 scene = gMainRoutineWork.scene;
  const u8 *table = gData_02006e9c;

  if (scene == (s32)&SceneId_Ov671Room1)
    table = gData_02006b54;
  else if (scene == (s32)&SceneId_Ov671Room2)
    table = gData_02006b6c;
  else if (scene == (s32)&SceneId_Ov671Room3)
    table = gData_02006bd8;
  else if (scene == (s32)&SceneId_Ov671Room4)
    table = gData_02006c80;
  else if (scene == (s32)&SceneId_Ov671Room5)
    table = gData_02006d1c;
  else if (scene == (s32)&SceneId_Ov671Room6)
    table = gData_02006db8;
  else if (scene == (s32)&SceneId_Ov671Room7)
    table = gData_02006e78;
  return table;
}

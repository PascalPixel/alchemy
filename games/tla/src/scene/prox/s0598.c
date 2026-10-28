#include "types.h"
#include "sequence_parent_work.h"
#include "scene_ids.h"

extern const u8 gData_0200554c[];
extern const u8 gData_02005558[];
extern const u8 gData_02005588[];
extern const u8 gData_020057f8[];
extern const u8 gData_02005a20[];
extern const u8 gData_02005da4[];
extern const u8 gData_02005eac[];

const u8 *Func_02000598(void) {
  s32 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Ov6a8Room1)
    return gData_02005558;
  if (scene == (s32)&SceneId_Ov6a8Room2)
    return gData_02005588;
  if (scene == (s32)&SceneId_Ov6a8Room3)
    return gData_020057f8;
  if (scene == (s32)&SceneId_Ov6a8Room4)
    return gData_02005a20;
  if (scene == (s32)&SceneId_Ov6a8Room5)
    return gData_02005da4;
  if (scene == (s32)&SceneId_Ov6a8Room6)
    return gData_02005eac;
  return gData_0200554c;
}

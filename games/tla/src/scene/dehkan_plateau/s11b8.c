#include "types.h"
#include "adventure.h"
#include "scene_ids.h"

extern const u8 gData_020045e0[];
extern const u8 gData_020045f8[];
extern const u8 gData_020047d8[];
extern const u8 gData_02004ac0[];
extern const u8 gData_02004d78[];
extern const u8 gData_02004dd8[];
extern const u8 gData_02004e20[];
extern const u8 gData_02004f10[];
extern const u8 gData_02004f1c[];
extern const u8 gData_02005078[];
extern const u8 gData_020051bc[];
extern const u8 gData_020053b4[];
extern const u8 gData_02005450[];
extern const u8 gData_020055b8[];

const u8 *Func_020011b8(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    return gData_020045f8;
  if (scene == (s32)&SceneId_Ov655Room2)
    return gData_020047d8;
  if (scene == (s32)&SceneId_Ov655Room3)
    return gData_02004ac0;
  if (scene == (s32)&SceneId_Ov655Room4)
    return gData_02004d78;
  if (scene == (s32)&SceneId_Ov655Room5)
    return gData_02004dd8;
  if (scene == (s32)&SceneId_Ov655Room6)
    return gData_02004e20;
  return gData_020045e0;
}

const u8 *Func_02001240(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    return gData_02004f1c;
  if (scene == (s32)&SceneId_Ov655Room2)
    return gData_02005078;
  if (scene == (s32)&SceneId_Ov655Room3)
    return gData_020051bc;
  if (scene == (s32)&SceneId_Ov655Room4)
    return gData_020053b4;
  if (scene == (s32)&SceneId_Ov655Room5)
    return gData_02005450;
  if (scene == (s32)&SceneId_Ov655Room6)
    return gData_020055b8;
  return gData_02004f10;
}

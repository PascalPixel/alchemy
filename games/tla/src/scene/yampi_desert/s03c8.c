#include "scene.h"

extern const u8 gData_0200518c[];
extern const u8 gData_02005198[];
extern const u8 gData_0200521c[];
extern const u8 gData_020053fc[];
extern const u8 gData_020054e0[];
extern const u8 gData_02005594[];
extern const u8 gData_02005630[];
extern const u8 gData_02005678[];
extern const u8 gData_020056b4[];
extern const u8 gData_02005750[];
extern const u8 gData_0200578c[];
extern const u8 gData_020057bc[];
extern const u8 gData_020057e0[];

/// Returns the current room's table.
// As ☀️'s Scene_GetEntrances picks one.
const u8 *Func_020003c8(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov661Room1)
    return gData_02005198;
  if (scene == (s32)&SceneId_Ov661Room2)
    return gData_0200521c;
  if (scene == (s32)&SceneId_Ov661Room3)
    return gData_020053fc;
  if (scene == (s32)&SceneId_Ov661Room4)
    return gData_020054e0;
  if (scene == (s32)&SceneId_Ov661Room5)
    return gData_02005594;
  if (scene == (s32)&SceneId_Ov661Room6)
    return gData_02005630;
  if (scene == (s32)&SceneId_Ov661Room7)
    return gData_02005678;
  if (scene == (s32)&SceneId_Ov661Room8)
    return gData_020056b4;
  if (scene == (s32)&SceneId_Ov661Room9)
    return gData_02005750;
  if (scene == (s32)&SceneId_Ov661Room10)
    return gData_0200578c;
  if (scene == (s32)&SceneId_Ov661Room11)
    return gData_020057bc;
  if (scene == (s32)&SceneId_Ov661Room12)
    return gData_020057e0;
  return gData_0200518c;
}

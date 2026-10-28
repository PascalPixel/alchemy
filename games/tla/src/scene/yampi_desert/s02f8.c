#include "scene.h"

extern const u8 gData_020049c4[];
extern const u8 gData_020049dc[];
extern const u8 gData_02004a84[];
extern const u8 gData_02004b44[];
extern const u8 gData_02004c04[];
extern const u8 gData_02004d3c[];
extern const u8 gData_02004e44[];
extern const u8 gData_02004fdc[];
extern const u8 gData_0200506c[];
extern const u8 gData_020050b4[];

const u8 *Func_020002f8(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov661Room1)
    return gData_020049dc;
  if (scene == (s32)&SceneId_Ov661Room2) {
    if (GameFlag_Test(0x8f0))
      return gData_02004b44;
    return gData_02004a84;
  }
  if (scene == (s32)&SceneId_Ov661Room3)
    return gData_02004c04;
  if (scene == (s32)&SceneId_Ov661Room4)
    return gData_02004d3c;
  if (scene == (s32)&SceneId_Ov661Room5)
    return gData_02004e44;
  if (scene == (s32)&SceneId_Ov661Room8)
    return gData_02004fdc;
  if (scene == (s32)&SceneId_Ov661Room10)
    return gData_0200506c;
  if (scene == (s32)&SceneId_Ov661Room12)
    return gData_020050b4;
  return gData_020049c4;
}

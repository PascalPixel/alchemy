#include "local.h"

extern u32 gData_020057a8[];
extern u32 gData_020057c0[];
extern u32 gData_020058b0[];
extern u32 gData_02005958[];
extern u32 gData_02005aa8[];
extern struct ActorPlacement gData_02005b20[];
extern struct ActorPlacement gData_02005be0[];

void *Func_020003d0(void) {
  s32 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Ov653Room1)
    return gData_020057c0;
  if (scene == (s32)&SceneId_Ov653Room3)
    return gData_020058b0;
  if (scene == (s32)&SceneId_Ov653Room4)
    return gData_02005958;
  if (scene == (s32)&SceneId_Ov653Room7)
    return gData_02005aa8;
  if (scene == (s32)&SceneId_Ov653Room8) {
    if (GameFlag_Test(0x849)) {
      gData_02005b20[3].talkFacing = 0;
      gData_02005b20[4].talkFacing = 0;
      gData_02005b20[5].talkFacing = 0;
    }
    return gData_02005b20;
  }
  if (scene == (s32)&SceneId_Ov653Room9) {
    if (GameFlag_Test(0x849)) {
      gData_02005be0[5].talkFacing = 3;
      gData_02005be0[6].x = 0x1700000;
      gData_02005be0[6].z = 0x2b60000;
      gData_02005be0[7].talkFacing = 1;
    }
    return gData_02005be0;
  }
  return gData_020057a8;
}

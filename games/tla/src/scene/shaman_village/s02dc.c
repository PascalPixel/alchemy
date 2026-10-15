#include "shaman_village.h"

extern u32 gData_02005410[];
extern u32 gData_02005428[];
extern s16 gData_02005590[];
struct ShamanVillageSceneBlock {
  u8 unk00[22];
  u8 field16;
};

extern struct ShamanVillageSceneBlock gData_020056b0;
extern u32 gData_02005860[];
extern u32 gData_02005998[];
extern u32 gData_02005a40[];

void *Func_020002dc(void) {
  s32 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_Shaman1)
    return gData_02005428;
  if (scene == (s32)&SceneId_Shaman3)
    return gData_02005590;
  if (scene == (s32)&SceneId_Shaman2) {
    if (GameFlag_Test(0x9fe))
      gData_020056b0.field16 = 1;
    return &gData_020056b0;
  }
  if (scene == (s32)&SceneId_Shaman4)
    return gData_02005860;
  if (scene == (s32)&SceneId_Shaman5)
    return gData_02005998;
  if (scene == (s32)&SceneId_Shaman6)
    return gData_02005a40;
  return gData_02005410;
}

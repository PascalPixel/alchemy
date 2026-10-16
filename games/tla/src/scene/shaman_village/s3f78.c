#include "shaman_village.h"

void Func_0200482c(void);
void Func_02004060(void);
void Func_020041b8(void);
void Func_020043f8(void);
void Func_02004534(void);
void Func_020046ec(void);

void Func_02003f78(void) {
  Func_020048c4();
  Func_02004a24(0);
  Func_0200498c((s32)MsgShamanMoapaAdvisesLeaving);
  Object_GetById(15)->facing = 0x8000;
  Object_GetById(16)->facing = 0x8000;
  Object_GetById(17)->facing = 0x8000;
  Camera_MoveWithinMapBounds(0xe00000, -1, 0x1180000, 0);
  WaitFrames(1);
  Func_0200482c();
  WaitFrames(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_WaitFrames(20);
  Func_020049a4(0x200f, 0);
  Func_0200289c();
  GameFlag_ClearBit(0x12f);
  Func_020048cc();
}

s32 Func_02003ff8(void) {
  s32 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_ShamanVillage1)
    Func_02004060();
  else if (scene == (s32)&SceneId_ShamanVillage3)
    Func_020041b8();
  else if (scene == (s32)&SceneId_ShamanVillage2)
    Func_020043f8();
  else if (scene == (s32)&SceneId_ShamanVillage4)
    Func_02004534();
  else if (scene == (s32)&SceneId_ShamanVillage6)
    Func_020046ec();
  return 0;
}

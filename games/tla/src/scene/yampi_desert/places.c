#include "scene.h"

extern const u8 gData_02004810[];
extern const u8 gData_02004840[];
extern const u8 gData_020048b8[];
extern const u8 gData_020048d8[];

void Func_02000280(void) { Func_02004148(19, 68); }

const u8 *Func_0200028c(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov661Room11)
    return gData_02004840;
  return gData_02004810;
}

const u8 *Func_020002b8(void) {
  s16 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_Ov661Room10)
    return gData_020048b8;
  if (scene == (s32)&SceneId_Ov661Room11)
    return gData_020048d8;
  return 0;
}

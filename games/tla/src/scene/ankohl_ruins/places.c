#include "ankohl_ruins.h"

extern const u8 gData_0200307c[];
extern const u8 gData_0200309c[];

const struct ScenePlacement *Func_02000344(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov686Room5)
    return (const struct ScenePlacement *)gData_0200307c;
  if (gPartyState.scene == (s32)&SceneId_Ov686Room8)
    return (const struct ScenePlacement *)gData_0200309c;
  return 0;
}

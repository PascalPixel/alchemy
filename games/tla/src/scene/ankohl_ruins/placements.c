#include "ankohl_ruins.h"

extern const u8 gData_0200307c[];
extern const u8 gData_0200309c[];

const struct ActorPlacement *Func_02000344(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5)
    return (const struct ActorPlacement *)gData_0200307c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room8)
    return (const struct ActorPlacement *)gData_0200309c;
  return 0;
}

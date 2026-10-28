#include "local.h"

extern const u8 gData_02001378[];
extern const u8 gData_02001390[];
extern const u8 gData_020013a8[];
extern const u8 gData_020013d8[];
extern const u8 gData_02001498[];
extern const u8 gData_02001510[];
extern const u8 gData_02001540[];
extern const u8 gData_0200154c[];
extern const u8 gData_02001594[];
extern const u8 gData_020015c4[];
extern const u8 gData_020016cc[];
extern const u8 gData_02001714[];

const struct ActorPlacement *Func_02000de4(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room1)
    return (const struct ActorPlacement *)gData_02001390;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room2)
    return (const struct ActorPlacement *)gData_020013a8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room3)
    return (const struct ActorPlacement *)gData_020013d8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room4)
    return (const struct ActorPlacement *)gData_02001498;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room5)
    return (const struct ActorPlacement *)gData_02001510;
  return (const struct ActorPlacement *)gData_02001378;
}

const struct Trigger *Func_02000e58(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room1)
    return (const struct Trigger *)gData_0200154c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room2)
    return (const struct Trigger *)gData_02001594;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room3)
    return (const struct Trigger *)gData_020015c4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room4)
    return (const struct Trigger *)gData_020016cc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room5)
    return (const struct Trigger *)gData_02001714;
  return (const struct Trigger *)gData_02001540;
}

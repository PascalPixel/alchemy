#include "ankohl_ruins.h"

s32 Func_020005c0(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  ((struct FieldSprite *)object->animation)->priority = 2;
  object->priorityFlags &= 0xfe;
  object->motionFlags = 0;
  object->y -= 0x40000;
  return 0;
}

extern const u8 gData_020032a0[];
extern const u8 gData_020032b8[];
extern const u8 gData_020034f8[];
extern const u8 gData_02003570[];
extern const u8 gData_020035d0[];
extern const u8 gData_02003708[];
extern const u8 gData_02003750[];
extern const u8 gData_02003768[];

const u8 *Func_02000600(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1)
    return gData_020032b8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room2)
    return gData_020034f8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room3)
    return gData_02003570;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4)
    return gData_020035d0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5)
    return gData_02003708;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room6)
    return gData_02003750;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room8)
    return gData_02003768;
  return gData_020032a0;
}

void Func_02000698(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 2;
}

void Func_020006b4(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 0;
}

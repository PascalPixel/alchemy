#include "ankohl_ruins.h"

s32 Func_020005c0(struct ObjectRuntime *object) {
  ObjectDispatch_SetSingleChildFlags(object, 0);
  ((struct FieldSprite *)object->animation)->priority = 2;
  object->priorityFlags &= 0xfe;
  object->motionFlags = 0;
  {
    s32 step = -0x40000;

    /* FAKEMATCH: the object passes through an asm that reads the step, so the
     * step's pool load comes before the y load; as plain C, the y load comes
     * first. */
    __asm__("" : "+l"(object) : "l"(step));
    object->y += step;
  }
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
  if (gPartyState.scene == (s32)&SceneId_Ov686Room1)
    return gData_020032b8;
  if (gPartyState.scene == (s32)&SceneId_Ov686Room2)
    return gData_020034f8;
  if (gPartyState.scene == (s32)&SceneId_Ov686Room3)
    return gData_02003570;
  if (gPartyState.scene == (s32)&SceneId_Ov686Room4)
    return gData_020035d0;
  if (gPartyState.scene == (s32)&SceneId_Ov686Room5)
    return gData_02003708;
  if (gPartyState.scene == (s32)&SceneId_Ov686Room6)
    return gData_02003750;
  if (gPartyState.scene == (s32)&SceneId_Ov686Room8)
    return gData_02003768;
  return gData_020032a0;
}

void Func_02000698(void) {
  Object_GetById(gPartyState.currentOwner)->terrainId = 2;
}

void Func_020006b4(void) {
  Object_GetById(gPartyState.currentOwner)->terrainId = 0;
}

#include "caverns.h"

const u8 *Func_02000ad0(void) {
  s32 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Ov6a9Room1)
    return gData_0200149c;
  if (scene == (s32)&SceneId_Ov6a9Room3)
    return gData_020014d8;
  if (scene == (s32)&SceneId_Ov6a9Room4)
    return gData_020014fc;
  return gData_02001430;
}

s32 Func_02000b20(void) {
  s16 *scene;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov6a9Room1)
    Func_02000ba8();
  /* FAKEMATCH: a pointer to the scene field keeps the second test a plain ldrsh
   * (plain gSequenceParentWork.scene or a party pointer: differ 16). */
  scene = &gSequenceParentWork.scene;
  if (*scene == (s32)&SceneId_Ov6a9Room4)
    Func_02000f98();
  return 0;
}

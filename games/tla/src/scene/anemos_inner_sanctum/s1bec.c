#include "types.h"
#include "sequence_parent_work.h"

extern char SceneId_Ov6B0Room1;
extern char SceneId_Ov6B0Room2;
extern char SceneId_Ov6B0Room3;
extern char SceneId_Ov6B0Room4;
extern char SceneId_Ov6B0Room5;
extern const u8 gData_02003dc0[];
extern const u8 gData_02003dcc[];
extern const u8 gData_02003e38[];
extern const u8 gData_02003fac[];
extern const u8 gData_02004150[];
extern const u8 gData_020042c4[];

const u8 *Func_02001bec(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov6B0Room1)
    return gData_02003dcc;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov6B0Room2)
    return gData_02003e38;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov6B0Room3)
    return gData_02003fac;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov6B0Room4)
    return gData_02004150;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov6B0Room5)
    return gData_020042c4;
  return gData_02003dc0;
}

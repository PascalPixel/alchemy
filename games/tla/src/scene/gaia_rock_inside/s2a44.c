#include "types.h"
#include "sequence_parent_work.h"

extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room3;
extern char SceneId_Ov681Room4;
extern char SceneId_Ov681Room5;
extern const u8 gData_02005ee8[];
extern const u8 gData_02005f30[];
extern const u8 gData_02005fcc[];
extern const u8 gData_02006038[];
extern const u8 gData_020060f8[];
extern const u8 gData_02006164[];
extern const u8 gData_020061a0[];

/// Returns the current room's table.
const u8 *Func_02002a44(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov681Room2)
    return gData_02006038;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov681Room3)
    return gData_020060f8;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov681Room4)
    return gData_02006164;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov681Room5)
    return gData_020061a0;
  if (gSequenceParentWork.entrance == 9)
    return gData_02005f30;
  if (gSequenceParentWork.entrance > 9)
    return gData_02005fcc;
  return gData_02005ee8;
}

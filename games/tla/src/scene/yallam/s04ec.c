#include "types.h"
#include "sequence_parent_work.h"

extern char SceneId_Ov676Room1;
extern char SceneId_Ov676Room2;
extern const u8 gData_02002d24[];
extern const u8 gData_02002eec[];
extern const u8 gData_02002f10[];

const u8 *Func_020004ec(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov676Room1 &&
      gSequenceParentWork.entrance == 15)
    return gData_02002f10;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov676Room2)
    return gData_02002eec;
  return gData_02002d24;
}

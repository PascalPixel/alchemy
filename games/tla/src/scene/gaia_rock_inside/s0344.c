#include "types.h"
#include "sequence_parent_work.h"

extern char SceneId_Ov681Room5;
extern const u8 gData_02005458[];

const u8 *Func_02000344(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov681Room5)
    return gData_02005458;
  return 0;
}

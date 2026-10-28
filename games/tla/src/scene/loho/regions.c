#include "types.h"
#include "sequence_parent_work.h"

extern char SceneId_Ov6A6Room2;
extern const u8 gData_0200183c[];

const u8 *Func_02000050(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov6A6Room2)
    return gData_0200183c;
  return 0;
}

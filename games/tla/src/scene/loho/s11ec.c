#include "types.h"
#include "sequence_parent_work.h"

extern char SceneId_Ov6A6Room1;

s32 GameFlag_Test(s32 flag);
void Func_020000ac(void);

s32 Func_020011ec(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov6A6Room1 && GameFlag_Test(0xa5f))
    Func_020000ac();
  return 0;
}

#include "types.h"
#include "sequence_parent_work.h"
#include "flag_call.h"

void Func_02000064(void);

s32 Func_02000680(void) {
  if (gSequenceParentWork.entrance == 3)
    Func_02000064();
  if (GameFlag_Test(0x90e))
    Func_02000064();
  return 0;
}

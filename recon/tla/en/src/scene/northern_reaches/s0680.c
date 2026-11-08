#include "types.h"
#include "main_routine_work.h"
#include "flag_call.h"

void Func_02000064(void);

s32 Func_02000680(void) {
  if (gMainRoutineWork.entrance == 3)
    Func_02000064();
  if (GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x900, 14))))
    Func_02000064();
  return 0;
}

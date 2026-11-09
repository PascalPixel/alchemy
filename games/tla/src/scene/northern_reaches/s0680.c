#include "types.h"
#include "main_routine_work.h"
#include "flag_call.h"
#include "event_inline.h"

void Func_02000064(void);

s32 Func_02000680(void) {
  if (gMainRoutineWork.entrance == 3)
    Func_02000064();
  if (Event_GetFlag(0x90e))
    Func_02000064();
  return 0;
}

#include "types.h"
#include "task.h"
#include "event_inline.h"

void Func_02000ab4(void);

void Func_02000dd0(void) {
  Task_AddFrame(Func_02000ab4, TASK_PASS_VBLANK);
}

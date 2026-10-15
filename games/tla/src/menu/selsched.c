#include "types.h"
#include "callback_scheduler.h"

void Menu_RunSelection(void);

void Func_0804dabc(void) {
  Scheduler_AddOrUpdateCallback(Menu_RunSelection, 0x480);
}

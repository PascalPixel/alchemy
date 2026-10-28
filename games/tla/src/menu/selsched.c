#include "types.h"
#include "task.h"

void Menu_RunSelection(void);

void Func_0804dabc(void) {
  Task_Add(Menu_RunSelection, TASK_PASS_VBLANK);
}

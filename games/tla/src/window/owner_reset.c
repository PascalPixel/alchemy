#include "types.h"
#include "task.h"

void SelectionMenu_Draw(void);

void Func_0803df00(void) {
  Task_Add(SelectionMenu_Draw, TASK_PASS_VBLANK);
}

void Resource_ScheduleOwnerReset(void) {
  Task_Remove(SelectionMenu_Draw);
}

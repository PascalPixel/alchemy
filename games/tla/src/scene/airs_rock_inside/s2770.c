#include "airs_rock_inside.h"

void Task_Add(void (*func)(void), s32 priority);
void Func_020026c0(void);

void Func_02002770(void) {
  s32 value = -1;

  gData_02006c0c = value;
  {
    /* FAKEMATCH(temporary): the priority in a local, which keeps the constant
     * ahead of the task function's load; as a literal argument. */
    s32 priority = 0x480;
    Task_Add(Func_020026c0, priority);
  }
}

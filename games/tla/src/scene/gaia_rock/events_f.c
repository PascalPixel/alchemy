#include "types.h"

s32 Task_Add(void (*callback)(void), s32 priority);
void Func_02000ab4(void);

void Func_02000dd0(void) {
  Task_Add(Func_02000ab4, 0x480);
}

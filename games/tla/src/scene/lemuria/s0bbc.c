#include "types.h"
#include "event_inline.h"

void Func_02000b28(void);
void Func_02000ad8(void);

void Func_02000bbc(void) {
  s32 priority;

  priority = 0x480;
  Task_Add(Func_02000b28, priority);
  priority = 0xc80;
  Task_Add(Func_02000ad8, priority);
}

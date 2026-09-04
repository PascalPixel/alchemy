#include "types.h"

#define Runtime_RemoveIrqHandlerSlot2 Func_080c1a24

typedef void (*InterruptHandler)(void);

void Func_0800307c(s32, s32, InterruptHandler);
void Runtime_RemoveIrqHandlerSlot2(void)
{
  int no;
  unsigned long long handler;
  handler = 2;
  no = handler;
  handler = 0;
  Func_0800307c(no, 0, (InterruptHandler)handler);
}

#include "types.h"
#include "scene.h"

typedef void (*InterruptHandler)(void);

void Runtime_RemoveIrqHandlerSlot2(void)
{
  int no;
  unsigned long long handler;
  handler = 2;
  no = handler;
  handler = 0;
  Sys_Place(no, 0, (InterruptHandler)handler);
}

#include "types.h"
#include "scene.h"
#include "abi/runtime/callbacks/remove_irq_handler_slot2.h"

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

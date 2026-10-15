#include "types.h"
#include "mem.h"
#include "callback_scheduler.h"

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);

void Func_0803fd14(void) {
  Scheduler_RemoveCallback(GraphicsPalette_LoadSelectionResourcesAndAdvance);
  Runtime_ReleaseHeapBlock(208);
}

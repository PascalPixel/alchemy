#include "types.h"
#include "mem.h"
#include "task.h"

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);

void Func_0803fd14(void) {
  Task_Remove(GraphicsPalette_LoadSelectionResourcesAndAdvance);
  HeapSlot_Release(208);
}

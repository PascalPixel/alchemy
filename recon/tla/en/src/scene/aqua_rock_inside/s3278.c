#include "../../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_02003278(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWords(&gData_0200536c, SPLIT_CONSTANT(0x600, 4));
  buffer = MemAlloc_(0xc80);
  Func_02003c7c(gData_0200414c, buffer);
  gData_0200536c.vramEntry = Resource_FindFreeEntry();
  /* FAKEMATCH: the native stores the entry before it starts the size; without
   * the barrier the size's first half comes first (differ 2). */
  __asm__("");
  VramCache_Load(gData_0200536c.vramEntry, 0xc80, buffer);
  Heap_Release(buffer);
  priority = SPLIT_CONSTANT(0x400, 28);
  Task_Add(Func_020030f4, priority);
}

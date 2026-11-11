#include "local.h"

void Func_02003628(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWords(&gData_02005970, 0x3a4);
  buffer = MemAlloc_(0x400);
  Func_02003c7c(gData_02004351, buffer);
  gData_02005970.vramEntry = Resource_FindFreeEntry();
  Vram_Load(gData_02005970.vramEntry, 0x400, buffer);
  Heap_Release(buffer);
  priority = 0x41c;
  FieldTask_AddFrame(Func_020034fc, priority);
  Event_PlaySound(220);
}

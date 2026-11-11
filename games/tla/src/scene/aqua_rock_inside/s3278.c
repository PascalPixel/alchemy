#include "local.h"

void Func_02003278(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWords(&gData_0200536c, 0x604);
  buffer = MemAlloc_(0xc80);
  Func_02003c7c(gData_0200414c, buffer);
  gData_0200536c.vramEntry = Resource_FindFreeEntry();
  /* FAKEMATCH(barrier): the empty asm keeps the entry's store ahead of the
   * size, as the ROM does; as no asm, the size's first half comes first and 2
   * instructions differ. */
  __asm__("");
  Vram_Load(gData_0200536c.vramEntry, 0xc80, buffer);
  Heap_Release(buffer);
  priority = 0x41c;
  FieldTask_AddFrame(Func_020030f4, priority);
}

#include "local.h"

void Func_02002550(void) {
  struct AquaRockWork *work = &gData_020034f0;
  void *buffer;

  Iwram_ClearWords(work, 0x70c);
  buffer = MemAlloc_(0x100);
  Func_020028b8(gData_02002c74, buffer);
  work->vramEntry = Resource_FindFreeEntry();
  /* FAKEMATCH: keeps the entry store before the size is built (differ 2
   * without) */
  __asm__("");
  VramBlock_LoadCached_(work->vramEntry, 0x100, buffer);
  MemFree(buffer);
  // Use a value-returning call; a void declaration changes argument order
  // (differ 2).
  ((s32 (*)(void *, s32))Scheduler_AddOrUpdateCallback)(Func_02002534, 0x480);
}

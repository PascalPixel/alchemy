/// @file
/// Scene code of overlay 0x67b, in ROM order, from events_i's offset up to the next object the overlay links.

#include "local.h"

void Func_02002534(void) {
  if (gData_020034f0.direction > 0)
    Func_02002250();
  else
    Func_020023c0();
}

void Func_02002550(void) {
  struct AquaRockWork *work = &gData_020034f0;
  void *buffer;

  Iwram_ClearWords(work, 0x70c);
  buffer = MemAlloc_(0x100);
  Func_020028b8(gData_02002c74, buffer);
  work->vramEntry = Resource_FindFreeEntry();
  VramCache_Load(work->vramEntry, 0x100, buffer);
  Heap_Release(buffer);
  // Use a value-returning call; a void declaration changes argument order
  // (differ 2).
  ((s32 (*)(void *, s32))Task_Add)(Func_02002534, 0x480);
}

void Func_020025a8(s32 id) {
  struct AquaRockWork *work = &gData_020034f0;

  if (id == 0) {
    work->object = 0;
  } else {
    work->object = Actor_GetById(id);
    if (id == 8)
      work->direction = -1;
    else
      work->direction = 1;
  }
}

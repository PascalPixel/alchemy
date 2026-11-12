/// @file
/// Scene code of overlay 0x67c, in ROM order, from s3628's offset up to the next object the overlay links.

#include "local.h"
#include "macros.h"

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

s32 Func_02003688(s32 x, s32 y, s32 z, s32 kind) {
  struct BeamWork *work = &gData_02005970;
  struct BeamEntry *entry = work->entries;

  if (entry->x != 0)
    entry++;
  SET_POSITION(entry, (x << 20) + 0x80000, y << 20, (z << 20) + 0x80000);
  entry->kind = kind;
}

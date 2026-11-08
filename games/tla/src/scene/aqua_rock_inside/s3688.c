#include "local.h"
#include "macros.h"

s32 Func_02003688(s32 x, s32 y, s32 z, s32 kind) {
  struct BeamWork *work = &gData_02005970;
  struct BeamEntry *entry = work->entries;

  if (entry->x != 0)
    entry++;
  SET_POSITION(entry, (x << 20) + 0x80000, y << 20, (z << 20) + 0x80000);
  entry->kind = kind;
}

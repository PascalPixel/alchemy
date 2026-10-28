#include "types.h"
#include "heap_release.h"

s32 Func_080381c0(s32 unit, void *buffer, s32 mode);

s32 Battle_BuildUnitActions(s32 unit) {
  void *buffer = Heap_AllocEwram(17);
  s32 count = Func_080381c0(unit, buffer, 0);
  s32 result = -1;

  if (count >= 0)
    result = count;
  Heap_Release(buffer);
  return result;
}

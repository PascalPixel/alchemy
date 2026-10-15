#include "types.h"

#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

void Func_02001950(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count;) {
    u8 *flags = &Object_GetById(first + i)->priorityFlags;
    u32 next = i + 1;

    if (show)
      *flags &= ~0x10;
    else
      /* FAKEMATCH: the native ties the flag or to the constant's register. */
      *flags = *flags + 0x10 - (*flags & 0x10);
    i = next;
  }
}

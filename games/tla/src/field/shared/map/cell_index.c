#include "types.h"
#include "object_runtime.h"

s32 Func_080eab70(struct ObjectRuntime *object) {
  return object->x / 0x100000 + (object->z / 0x100000) * 128;
}

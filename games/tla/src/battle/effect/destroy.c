#include "types.h"
#include "object_runtime.h"

void Object_Destroy(struct ObjectRuntime *object);

void Object_DestroyIfPresent(struct ObjectRuntime *object) {
  if (object != 0)
    Object_Destroy(object);
}

/// Nothing in the image calls it.
void Func_080cf34c(void) {}

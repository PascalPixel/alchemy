#include "types.h"
#include "object_runtime.h"

void Object_Destroy(struct ObjectRuntime *object);

void Func_080cf340(struct ObjectRuntime *object) {
  if (object != 0)
    Object_Destroy(object);
}

void Func_080cf34c(void) {}

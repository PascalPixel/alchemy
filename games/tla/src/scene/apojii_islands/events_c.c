#include "local.h"

void Func_02000770(struct ObjectRuntime *object) {
  Animation_ApplyChildValues(object, 0);
  object->update = 0;
}

void Func_02000780(struct ObjectRuntime *object) {
  if (gFrameCount & 2)
    Animation_ApplyChildValues(object, 7);
  else
    Animation_ApplyChildValues(object, 0);
}

#include "local.h"

void Func_02000b10(struct Actor *object) {
  if (gFrameCount & 2)
    Animation_ApplyChildValues(object, 7);
  else
    Animation_ApplyChildValues(object, 0);
}

void Func_02000b34(struct Actor *object) {
  if (gFrameCount & 2)
    Animation_ApplyChildValues(object, 6);
  else
    Animation_ApplyChildValues(object, 0);
}

void Func_02000b58(struct Actor *object) {
  Animation_ApplyChildValues(object, 0);
  object->update = 0;
}

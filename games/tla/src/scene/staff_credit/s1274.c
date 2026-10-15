#include "types.h"
#include "object_runtime.h"

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_020020b4(s32 cue);
void Func_020011f4(struct ObjectRuntime *object);
void Func_0200122c(struct ObjectRuntime *object);

void Func_02001274(struct ObjectRuntime *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Animation_ApplyChildValues(object, 0);
    Func_020020b4(390);
  } else if (mode == 1) {
    object->update = (void (*)(void))Func_020011f4;
  } else {
    object->update = (void (*)(void))Func_0200122c;
    Func_020020b4(291);
  }
}

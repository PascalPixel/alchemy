#include "types.h"
#include "actor.h"

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02003b38(struct Actor *object);
void Func_02003b70(struct Actor *object);
void Func_02004e30(s32 value);

void Func_02003bb8(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Animation_ApplyChildValues(object, 0);
    Func_02004e30(390);
  } else if (mode == 1) {
    object->update = (void (*)(void))Func_02003b38;
  } else {
    object->update = (void (*)(void))Func_02003b70;
    Func_02004e30(291);
  }
}

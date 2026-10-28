#include "local.h"

void Func_0200052c(struct ObjectRuntime *object) {
  if ((u32)Random16Far() < 0x1000) {
    struct ObjectRuntime *child;
    s32 spread = Random16Far();
    s32 x = object->x;
    s32 bias;

    /* FAKEMATCH: keeps the x load before the spread is scaled (differ 4
     * without) */
    __asm__("" : : "r"(x));
    x += spread * 48;
    bias = -0x180000;
    /* FAKEMATCH: keeps the bias load before the kind is built (differ 4
     * without) */
    __asm__("" : : "r"(bias));
    child = Import_ActorCreate(0x2a2, x + bias, object->y, object->z);
    if (child != 0) {
      ObjectDispatch_SetSingleChildFlags(child, 0);
      child->update = (void (*)(void))Func_020004e4;
      child->motionFlags = 0;
    }
  }
}

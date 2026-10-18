#include "ride.h"
#include "macros.h"

void Func_02002fc4(struct ObjectRuntime *object);

s32 Func_020031dc(struct ObjectRuntime *object) {
  Func_02002fc4(object);
  SET_POSITION(object, 0, 0, 0);
  SET_VELOCITY(object, 0, 0, 0);
  return 0;
}

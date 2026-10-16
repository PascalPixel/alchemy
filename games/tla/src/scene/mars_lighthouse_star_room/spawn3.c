#include "types.h"
#include "object_runtime.h"

extern const u8 gData_02004744[];

void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_020041cc(struct ObjectRuntime *object, s32 mode);
void Func_020041dc(struct ObjectRuntime *object, const void *script);

void Func_02001b38(struct ObjectRuntime *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  Func_020041cc(object, 2);
  Func_020041dc(object, gData_02004744);
  Object_SetPartAttribute(object, 10);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

#include "types.h"
#include "actor.h"

extern const u8 gData_02004744[];

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_020041cc(struct Actor *object, s32 mode);
void Func_020041dc(struct Actor *object, const void *script);

void Func_02001b38(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_020041cc(object, 2);
  Func_020041dc(object, gData_02004744);
  Actor_SetPartAttribute(object, 10);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

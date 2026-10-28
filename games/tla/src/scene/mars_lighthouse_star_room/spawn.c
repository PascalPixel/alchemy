#include "types.h"
#include "actor.h"
#include "call.h"

extern const u8 gData_02004744[];

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_020041cc(struct Actor *object, s32 mode);
void Func_020041dc(struct Actor *object, const void *script);
struct Actor *Func_020041e4();
void Func_020015ec(void);

void Func_02001750(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_020041dc(object, gData_02004744);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_02001780(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = (struct Actor *)Value4(
      (s32 (*)())Func_020041e4, 366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    ((void (*)())Func_02001750)(object, 2);
    object->update = Func_020015ec;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}

#include "types.h"
#include "actor.h"
#include "event_inline.h"

struct IsletsVector {
  s32 x;
  s32 y;
  s32 z;
};

extern const u16 gData_020022d0[];

s32 Func_020021b8(struct Actor *object, struct IsletsVector *position);
struct Actor *Func_020022a8(struct IsletsVector *position, s32 mode);

void Func_02001178(struct Actor *object) {
  struct IsletsVector position;
  struct Actor *hit;
  u32 i;
  s32 angle = object->angle >> 12;

  angle = ((angle + 2) & 12) << 12;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, &position);
  hit = Func_020022a8(&position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->animation)->value28 ==
        gData_020022d0[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  if (Func_020021b8(object, &position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}

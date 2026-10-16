// Draft, differ 30 of 61: the native keeps the index in r4 and reloads the
// animation, its value and the table entry (index * 2) on every pass, where
// this loop is strength-reduced to a walking pointer; it also builds the facing
// angle before sub sp. A goto loop hoists the animation load (differ 42).
// permute (1 minute) reached differ 14 by reading the value into the angle
// variable first, which is no source form.
#include "types.h"
#include "object_runtime.h"

extern const u16 gData_020022d0[];

void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x,
                                      s32 y, s32 z);
struct ObjectRuntime *Func_020022a8(s32 *position, s32 mode);
s32 Func_020021b8(struct ObjectRuntime *object, s32 *position);

void Func_02001178(struct ObjectRuntime *object) {
  s32 position[3];
  struct ObjectRuntime *other;
  u32 i;
  s32 angle = (((object->angle >> 12) + 2) & 12) << 12;

  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  other = Func_020022a8(position, 1);
  if (other) {
    i = 0;
    do {
      if (((*((struct BattleAnimationState *)other->animation)->value28 ==
            gData_020022d0[i])))
        return;
      i++;
    } while (i <= 5);
    Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (Func_020021b8(object, position) > 0)
    Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}

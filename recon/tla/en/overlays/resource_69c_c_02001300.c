// Draft of overlay 69C Func_02001300 (first written 2026-10-12): differ 42
// of 61. The native rereads other->animation->value28 and reloads the table
// address on every pass with the index in r4, and builds the angle through two
// registers before it copies the position; the draft hoists all three in a for
// loop, a goto loop and with a one-pass block around the test.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_3/local.h"

extern const u16 gData_02002478[];

void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
struct ObjectRuntime *Func_02002438(s32 *position, s32 mode);
s32 Func_02002338(struct ObjectRuntime *object, s32 *position);

void Func_02001300(struct ObjectRuntime *object) {
  s32 position[3];
  struct ObjectRuntime *other;
  u32 i;
  s32 angle;
  s32 dir;

  dir = object->angle >> 12;
  angle = ((dir + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  other = Func_02002438(position, 1);
  if (other != NULL) {
    i = 0;
  next:
    if (*((struct BattleAnimationState *)other->animation)->value28 ==
        gData_02002478[i])
      return;
    i++;
    if (i < 6)
      goto next;
    Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (Func_02002338(object, position) > 0)
    Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}

// Matches: differ 0 (tla-en, 2026-11-01)
// One forced form to judge at adoption: the local heading holds the angle for
// the first call only. With the plain (s16)*angle argument, differ 2 of 89 and
// skeleton gap 1: the radius load moves ahead of the movs and ldrsh of the
// angle. Unchanged by a block s32 radius local, an s32 cast, an s16 field or
// parameter, or heading = *angle inside the argument list; an unprototyped call
// gives differ 4. It carries no FAKEMATCH tag because this round adds none, and
// it may count as temporary under S2 when adopted.
// position is set inside the first call's argument list: assigned before the
// call, differ 15.
#include "types.h"

struct Orbiter {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x38];
  s32 radius;
  u8 unk50[0x12];
  u8 timer;
  u8 unk63;
  u16 angle;
  u16 speed;
  struct Orbiter *parent;
};

void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);
void Func_02002a14(struct Orbiter *object);

s32 Func_02002a6c(struct Orbiter *object) {
  u16 *angle = &object->angle;
  u16 *speed = &object->speed;
  struct Orbiter *parent = object->parent;
  s32 *position;
  s16 heading;

  object->angle += (s16)*speed >> 1;
  object->x = parent->x;
  object->y = parent->y + 0x80000;
  object->z = parent->z;
  heading = *angle;
  Vector_AddPolarOffsetFar(object->radius, heading, position = &object->x);
  if (--object->timer == 0)
    return 0;
  if (object->radius == 0x20000)
    return 1;
  Func_02002a14(object);
  object->radius -= 0x40000;
  object->angle += (s16)*speed >> 1;
  *position = parent->x;
  object->y = parent->y + 0x80000;
  object->z = parent->z;
  Vector_AddPolarOffsetFar(object->radius, (s16)*angle, position);
  Func_02002a14(object);
  object->radius -= 0x40000;
  return 1;
}

#include "types.h"
#include "actor.h"
#include "call.h"

struct Orbiter {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x38];
  s32 radius;
  u8 unk50[0x12];
  u8 countdown;
  u8 unk63;
  s16 angle;
  u16 speed;
  struct Actor *anchor;
};

void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);
void Func_02003e7c(struct Orbiter *orbiter);

s32 Func_02003ed4(struct Orbiter *orbiter) {
  struct Actor *anchor = orbiter->anchor;

  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x;
  orbiter->y = anchor->y + 0x80000;
  orbiter->z = anchor->z;
  Call3((void (*)())Vector_AddPolarOffsetFar, orbiter->radius, orbiter->angle,
        (s32)&orbiter->x);
  if (--orbiter->countdown == 0)
    return 0;
  if (orbiter->radius == 0x20000)
    return 1;
  Func_02003e7c(orbiter);
  orbiter->radius -= 0x40000;
  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x;
  orbiter->y = anchor->y + 0x80000;
  orbiter->z = anchor->z;
  Vector_AddPolarOffsetFar(orbiter->radius, orbiter->angle, &orbiter->x);
  Func_02003e7c(orbiter);
  orbiter->radius -= 0x40000;
  return 1;
}

// Draft, differ 2 of 89: steps an orbiting object around its anchor: turns it
// by half its speed, puts it on the anchor (raised 8 pixels) and pushes it out
// by its radius; when its countdown runs out returns 0, else, unless its radius
// is down to 2 pixels, takes a second, shrinking step. Returns 1 while it runs.
// The same listing as overlay 67C's Func_020013bc. The first push goes
// through Call3 (directly, its offset pointer is set after the angle's load).
// Left: the known store-then-load row before the second push: the native sets
// r2 from r8 between the z store and the angle's load. A pinned r2 behind an
// empty asm breaks r8's reuse (84).
#include "types.h"
#include "object_runtime.h"
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
  struct ObjectRuntime *anchor;
};

void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);
void Func_02001314(struct Orbiter *orbiter);

s32 Func_0200136c(struct Orbiter *orbiter) {
  struct ObjectRuntime *anchor = orbiter->anchor;

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
  Func_02001314(orbiter);
  orbiter->radius -= 0x40000;
  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x;
  orbiter->y = anchor->y + 0x80000;
  orbiter->z = anchor->z;
  Vector_AddPolarOffsetFar(orbiter->radius, orbiter->angle, &orbiter->x);
  Func_02001314(orbiter);
  orbiter->radius -= 0x40000;
  return 1;
}

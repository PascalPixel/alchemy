#include "types.h"
#include "call.h"
#include "actor.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

extern u32 gInput;
extern s16 gData_020068b0;
extern const s16 gData_02005798[];
s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

void Func_02003284(void);
void Func_02004bc0(struct Actor *actor, s32 mode);
void Func_02004bf8(struct Actor *actor, s32 x, s32 y, s32 z);
void Func_02004c00(struct Actor *actor);
s32 Func_02004c10(u8 layer, s32 x, s32 z);
s32 Func_02004c70(struct Actor *actor);
s32 Func_02004c78(s32 *a, s32 radiusA, struct Position *b, s32 radiusB);
s32 Map_GetTerrainHeight(u8 layer, s32 x, s32 z);
void Actor_SetAnimationSpeed(struct Actor *actor, s32 speed);
void Vector_AddPolarOffsetFar(s32 length, s32 angle, struct Position *position);
void wait(s32 frames);
s32 ArcTan2(s32 z, s32 x);
void Func_020031f8(void);

void Func_02003564(void) {
  struct Position probe;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 startX;
  s32 startZ;
  u8 *layer;
  s32 radius;
  struct Actor *actor;
  struct Position *next;
  s32 nextBehavior;
  struct Actor *other;
  struct Position *position;
  u8 *flags;
  s32 i;
  s32 frame;
  u16 heading;
  s32 dx;
  s32 dz;
  s32 distance;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_020068b0 = 0;

top:
  Func_02003284();
  actor->speedLimit = PIXELS(1);
  actor->acceleration = PIXELS(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  actor->velocityX = 0;
  actor->velocityZ = 0;

  position = &probe;
  position->x = startX = (actor->x & 0xfff00000) + PIXELS(8);
  position->y = actor->y;
  position->z = startZ = (actor->z & 0xfff00000) + PIXELS(8);
  x = startX;
  z = startZ;
  layer = &actor->terrainId;
  startBehavior = Func_02004c10(*layer, x, z);
  dx = x - actor->x;
  dz = z - actor->z;
  distance = IwramMulQ16(dx, dx);
  distance += IwramMulQ16(dz, dz);
  if (distance < PIXELS(4)) {
    heading = gData_02005798[(gInput >> 4) & 15];
    if (heading == 0xffff) {
      goto done;
    }
  } else {
    heading = ArcTan2(z - actor->z, x - actor->x);
    heading = (heading + 0x2000) & -0x4000;
  }
  Vector_AddPolarOffsetFar(PIXELS(16), heading, &probe);
  nextBehavior = Func_02004c10(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y > PIXELS(8)) {
    actor->angle = heading;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x = startX;
    actor->z = startZ;
    goto done;
  }
  position->x = startX;
  position->z = startZ;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  actor->action = 0;
  Func_02004bf8(actor, startX, actor->y, startZ);
  Func_02004bc0(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  frame = 0;
  goto wait1;
step1:
  if (gData_020068b0 != 0) {
    goto done;
  }
  wait(1);
  frame++;
wait1:
  if (frame <= 179 && Func_02004c70(actor) == 0) {
    goto step1;
  }
  actor->update = Func_020031f8;
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y > PIXELS(8)) {
    goto stop;
  }
  if (gData_020068b0 != 0) {
    goto done;
  }
  radius = actor->radius - 2;
  other = Ram_HeapSlots->scriptObjects;
  flags = &other->collisionFlags;
  for (i = 0; i < 64; i++, flags += 128, other++) {
    if (other->script != NULL && (*flags & 1) && other != actor) {
      if (Func_02004c78(&other->x, radius, &probe, other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  Func_02004bf8(actor, next->x, next->y, next->z);
  frame = 0;
  goto wait2;
step2:
  wait(1);
  frame++;
wait2:
  if (frame <= 179 && Func_02004c70(actor) == 0 && gData_020068b0 == 0) {
    goto step2;
  }
  if (gData_020068b0 != 0) {
    goto done;
  }
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  Vector_AddPolarOffsetFar(PIXELS(16), heading, &probe);
  next = &probe;
  nextBehavior = Func_02004c10(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1);
  Func_02004bf8(actor, x, actor->y, z);
  Func_02004c00(actor);
  wait(2);
  asm("" : "=r"(actor) : "0"(actor) : "sfp");
  goto top;

blocked:
  wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = PIXELS(1) / 4;
  Func_02004bc0(actor, 1);
}

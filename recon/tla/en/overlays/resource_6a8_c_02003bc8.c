// Draft, differ 257 of 347, skeleton gap 47/46 (tla-en, 2026-11-01)
// The leader slides along the ice, a relative of Imil_RunIceSlide in The Broken
// Seal and of this overlay's Func_02003ee8: the same probe of the next cell,
// cell behaviour 255 as the wall and height test of 8 pixels. Here the heading
// comes from the held keys near the cell's middle and from ArcTan2 otherwise,
// and gData_020062a4 cuts every wait short. Rewritten from the translation
// (differ 357, gap 75/96) with no devices; overlay 6aa's Func_02003564 is the
// same function.
// Left, biggest first:
// 1. PRE hoists sfp - 12 and actor + 34 above top, 6 instructions the native
//    rebuilds in the loop, and the long-lived pointer then shifts the high
//    registers: flags r8, count r10, next r6 and heading r9 all move, count
//    takes the caller-save r4, and startZ and radius lose their stack slots.
//    Func_02003ee8 stops this only with asm sfp clobbers, a new device, so none
//    is used here. goto, for (;;), one or two pointers and every order of the
//    pointer's sets all leave the hoist in place.
// 2. The native tests gData_020062a4 as ldrh; lsls #16; cmp #0 at 4 sites.
//    extern volatile u16 gives that at all four; a plain u16, an s32 local, a
//    shift or cast, and a u8[] cast give no lsls, and s16 or a 16-bit field give
//    movs; ldrsh. Volatile in one function is a device under S2, so it is left
//    out.
// Tried without gain: a mul local, one sum expression, the counter's
// declaration order, a do-while search, a next pointer of its own (next r6 in
// the native, but it adds 6 to the gap while the hoist stands). position->z =
// z = startZ gives gap 46/44 but differ 278.
#include "types.h"
#include "actor.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

extern u32 gInput;
extern s16 gData_02004780[];
extern u16 gData_020062a4;

void Func_020038e8(void);
void Func_020043c4(struct Actor *actor, s32 mode);
void Func_020043f4(struct Actor *actor, s32 x, s32 y, s32 z);
void Func_020043fc(struct Actor *actor);
s32 Func_02004424(u8 layer, s32 x, s32 z);
s32 Func_0200444c(struct Actor *actor);
s32 Func_02004454(s32 *a, s32 radiusA, struct Position *b, s32 radiusB);
s32 ArcTan2(s32 z, s32 x);
s32 Map_GetTerrainHeight(u8 layer, s32 x, s32 z);
void Actor_SetAnimationSpeed(struct Actor *actor, s32 speed);
void Vector_AddPolarOffsetFar(s32 length, s32 angle, struct Position *position);
void wait(s32 frames);
void Func_0200385c(void);

void Func_02003bc8(void) {
  struct Position probe;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 startX;
  s32 startZ;
  u8 *layer;
  s32 radius;
  struct Actor *actor;
  s32 heading;
  s32 nextBehavior;
  struct Position *position;
  struct Actor *other;
  u8 *flags;
  s32 i;
  s32 count;
  s32 dx;
  s32 dz;
  s32 distance;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_020062a4 = 0;

top:
  Func_020038e8();
  actor->speedLimit = PIXELS(1);
  actor->acceleration = PIXELS(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  position = &probe;
  startX = (actor->x & 0xfff00000) + PIXELS(8);
  position->x = startX;
  position->y = actor->y;
  startZ = (actor->z & 0xfff00000) + PIXELS(8);
  position->z = startZ;
  z = startZ;
  x = startX;
  layer = &actor->terrainId;
  startBehavior = Func_02004424(*layer, x, z);
  dx = x - actor->x;
  dz = z - actor->z;
  distance = Iwram_MulQ16(dx, dx);
  distance += Iwram_MulQ16(dz, dz);
  if (distance < PIXELS(4)) {
    heading = gData_02004780[(gInput >> 4) & 15];
    if ((heading << 16) == -0x10000) {
      goto done;
    }
  } else {
    heading = (s16)ArcTan2(z - actor->z, x - actor->x);
    heading = (s16)((heading + 0x2000) & -0x4000);
  }
  Vector_AddPolarOffsetFar(PIXELS(16), heading, &probe);
  nextBehavior = Func_02004424(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y >
          PIXELS(8)) {
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
  Func_020043f4(actor, startX, actor->y, startZ);
  Func_020043c4(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  for (i = 0; i < 180 && Func_0200444c(actor) == 0; i++) {
    if ((gData_020062a4 << 16) != 0) {
      goto done;
    }
    wait(1);
  }
  actor->update = Func_0200385c;
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y > PIXELS(8)) {
    goto stop;
  }
  if ((gData_020062a4 << 16) != 0) {
    goto done;
  }
  radius = actor->radius - 2;
  other = Ram_HeapSlots->scriptObjects;
  flags = &other->collisionFlags;
  for (count = 0; count < 64; count++, flags += 128, other++) {
    if (other->script != NULL && (*flags & 1) && other != actor) {
      if (Func_02004454(&other->x, radius, &probe, other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = position->x;
  z = position->z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  i = 0;
  Func_020043f4(actor, position->x, position->y, position->z);
  goto slideTest;
slideWait:
  wait(1);
  i++;
slideTest:
  if (i <= 179 && Func_0200444c(actor) == 0 && (gData_020062a4 << 16) == 0) {
    goto slideWait;
  }
  if ((gData_020062a4 << 16) != 0) {
    goto done;
  }
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  Vector_AddPolarOffsetFar(PIXELS(16), heading, &probe);
  position = &probe;
  nextBehavior = Func_02004424(*layer, position->x, position->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1);
  Func_020043f4(actor, x, actor->y, z);
  Func_020043fc(actor);
  wait(2);
  goto top;

blocked:
  wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = PIXELS(1) / 4;
  Func_020043c4(actor, 1);
}

// Draft, differ 278 of 246, skeleton gap 38/42 (tla-en, 2026-11-01)
// The Lost Age's copy of the ice slide. Imil_RunIceSlide in The Broken Seal is
// its twin: the same probe a cell ahead, the 255 wall test, the height test and
// the 0x1999 acceleration. This copy takes its heading from the actor's angle
// rather than the keys, and adds a scan of the script objects. Its nearest
// relative is Func_02003ee8, games/tla/src/scene/prox/s3ee8.c, which this draft
// follows statement for statement, so block layout is the listing's: wall arm
// before the loop body, loop test at the bottom.
// Left: in the listing actor + 34 and probe's frame address are rebuilt inside
// the outer label loop; here they sit before it, which reads as GCSE's PRE
// (PRE hoists out of goto loops). Likely but unproven: the extra spill, low
// registers for i and flags (native r9, r8) and the argument order follow.
// s3ee8.c's full form gives differ 0 here. It uses sfp barriers, tied asms, a
// dead store of step and a reused i, all devices barred this round, so none is
// kept, and they were removed together, not one at a time.
// Measured without devices: goto outer loop 272, 278 or 290, for outer loop 288, 294 or 300. Layer as a set, inline or
// direct local, a zero or mask local, block scopes, the wall as two ifs and
// taking x and z from probe compile the same; a PIXELS(8) local moves it (281,
// 290). One position local through both halves (kept) has the smallest gap.
// Plain for loops with break reach differ 194, gap 59/55, and a while with a
// comma test 256, gap 44/54, both without the listing's layout.
// Next: a plain form that keeps both addresses in the loop. If none exists this
// joins the landing-burst hoisting question, a compiler matter for Pascal.
#include "types.h"
#include "actor.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"

struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Func_02003284(void);
void Func_02004bc0(struct Actor *actor, s32 mode);
void Func_02004bf8(struct Actor *actor, s32 x, s32 y, s32 z);
void Func_02004c00(struct Actor *actor);
s32 Func_02004c10(u8 layer, s32 x, s32 z);
s32 Func_02004c78(s32 *a, s32 radiusA, struct Position *b, s32 radiusB);
s32 Map_GetTerrainHeight(u8 layer, s32 x, s32 z);
void Actor_SetAnimationSpeed(struct Actor *actor, s32 speed);
void Vector_AddPolarOffsetFar(s32 length, s32 angle, struct Position *position);
void wait(s32 frames);
void Func_020031f8(void);

void Func_02003884(void) {
  struct Position probe;
  s32 heading;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 radius;
  struct Actor *actor;
  s32 nextBehavior;
  u8 *layer;
  struct Actor *other;
  struct Position *position;
  u8 *flags;
  s32 i;
  s32 startX;
  s32 startZ;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);
  heading = (s16)((actor->angle + 0x2000) & -0x4000);

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
  startBehavior = Func_02004c10(*layer, startX, startZ);
  Vector_AddPolarOffsetFar(PIXELS(16), heading, position);
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
  position->x = x;
  position->z = z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  actor->action = 0;
  Func_02004bc0(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  actor->update = Func_020031f8;
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y > PIXELS(8)) {
    goto stop;
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
  x = position->x;
  z = position->z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  Func_02004bf8(actor, position->x, position->y, position->z);
  Func_02004c00(actor);
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  Vector_AddPolarOffsetFar(PIXELS(16), heading, &probe);
  position = &probe;
  nextBehavior = Func_02004c10(*layer, position->x, position->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1);
  Func_02004bf8(actor, x, actor->y, z);
  Func_02004c00(actor);
  wait(2);
  goto top;

blocked:
  wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = PIXELS(1) / 4;
  Func_02004bc0(actor, 1);
}

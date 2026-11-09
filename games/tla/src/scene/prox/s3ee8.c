#include "types.h"
#include "actor.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "event_inline.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Func_020038e8(void);
void Func_020043c4(struct Actor *actor, s32 mode);
s32 Func_02004454(s32 *a, s32 radiusA, struct Position *b, s32 radiusB);
s32 Map_GetTerrainHeight(u8 layer, s32 x, s32 z);
void Actor_SetAnimationSpeed(struct Actor *actor, s32 speed);
void Func_0200385c(void);

void Func_02003ee8(void) {
  struct Position probe;
  s32 heading;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 radius;
  struct Actor *actor;
  struct Position *next;
  s32 nextBehavior;
  u8 *layer;
  struct Actor *other;
  struct Position *position;
  u8 *flags;
  s32 i;
  s32 cell;
  s32 angle;
  s32 zero;
  struct Position *step;
  s32 startX;
  s32 startZ;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);
  heading = (s16)((actor->angle + 0x2000) & -0x4000);

top:
  Func_020038e8();
  actor->speedLimit = PIXELS(1);
  actor->acceleration = PIXELS(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  // Default: one zero, spilled and reloaded at each use.
  zero = 0;
  actor->velocityX = zero;
  actor->velocityZ = zero;
  /* FAKEMATCH(temporary): cell holds the masked x; as one expression, 2 of 246
   * instructions differ in tla-en. */
  cell = actor->x & 0xfff00000;
  /* FAKEMATCH(temporary): i holds half a cell here and the search's count
   * later; as a separate PIXELS(8), 18 of 246 instructions differ in tla-en. */
  i = PIXELS(8);
  position = &probe;
  position->x = startX = cell + i;
  position->y = actor->y;
  position->z = startZ = (actor->z & 0xfff00000) + i;
  x = startX;
  z = startZ;
  startBehavior =
      GetMapCellCollision(*(layer = &actor->terrainId), startX, startZ);
  Vector_AddPolarOffsetFar(PIXELS(16), heading, position);
  nextBehavior = GetMapCellCollision(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y > i) {
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
  actor->action = zero;
  Func_020043c4(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  actor->update = Func_0200385c;
  /* FAKEMATCH(barrier): the empty asm clobbering sfp and reading nextBehavior
   * twice; as no asm, 18 of 246 instructions differ in tla-en. */
  asm("" : : "r"(nextBehavior), "r"(nextBehavior) : "sfp");
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y > PIXELS(8)) {
    goto stop;
  }
  radius = actor->radius - 2;
  other = Ram_HeapSlots->scriptObjects;
  flags = &other->collisionFlags;
  for (i = 0; i < 64; i++, flags += 128, other++) {
    if (other->script != NULL && (*flags & 1) && other != actor) {
      /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading
       * next and actor; as no asm, 170 of 246 instructions differ in tla-en. */
      asm(""
          :
          : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),
            "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)
          : "sfp", "r4");
      if (Func_02004454(&other->x, radius, &probe, other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  Actor_SetMoveTarget(actor, next->x, next->y, next->z);
  Script_WaitForEventTimeout(actor);
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  /* FAKEMATCH(barrier): the asm tied to heading, with an r4 clobber so PRE
   * leaves it, loads the angle ahead of the address; as heading in the call, 44
   * of 246 instructions differ in tla-en. */
  asm("" : "=r"(angle) : "0"(heading) : "r4");
  step = &probe;
  Vector_AddPolarOffsetFar(PIXELS(16), angle, step);
  /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the address, so
   * next rebuilds it; as &probe in the call, 97 of 246 instructions differ in
   * tla-en. */
  step = NULL;
  next = &probe;
  nextBehavior = GetMapCellCollision(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1);
  Actor_SetMoveTarget(actor, x, actor->y, z);
  Script_WaitForEventTimeout(actor);
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

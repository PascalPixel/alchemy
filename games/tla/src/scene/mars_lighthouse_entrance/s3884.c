#include "types.h"
#include "actor.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "event_inline.h"
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/scene.h"
#include "module/tla.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Func_020031f8(void);

void Func_02003884(void) {
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
  heading = (s16)((actor->facing + 0x2000) & -0x4000);

top:
  Func_02003284();
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
  startBehavior = MapGrid_GetCellBehaviorAt(*(layer = &actor->terrainId), startX, startZ);
  FieldVector_AddPolarOffset(PIXELS(16), heading, position);
  nextBehavior = MapGrid_GetCellBehaviorAt(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y > i) {
    actor->facing = heading;
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
  actor->timer = zero;
  Func_02004bc0(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  actor->update = Func_020031f8;
  /* FAKEMATCH(barrier): the empty asm clobbering sfp and reading nextBehavior
   * twice; as no asm, 18 of 246 instructions differ in tla-en. */
  asm("" : : "r"(nextBehavior), "r"(nextBehavior) : SFP_CLOBBER);
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
          : SFP_CLOBBER, "r4");
      if (Func_02004c78(&other->x, radius, &probe, other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  FieldActor_SetMoveTarget(actor, next->x, next->y, next->z);
  Actor_WaitForMove(actor);
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  /* FAKEMATCH(barrier): the asm tied to heading, with an r4 clobber so PRE
   * leaves it, loads the angle ahead of the address; as heading in the call, 44
   * of 246 instructions differ in tla-en. */
  asm("" : "=r"(angle) : "0"(heading) : "r4");
  step = &probe;
  FieldVector_AddPolarOffset(PIXELS(16), angle, step);
  /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the address, so
   * next rebuilds it; as &probe in the call, 97 of 246 instructions differ in
   * tla-en. */
  step = NULL;
  next = &probe;
  nextBehavior = MapGrid_GetCellBehaviorAt(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1);
  FieldActor_SetMoveTarget(actor, x, actor->y, z);
  Actor_WaitForMove(actor);
  System_Wait(2);
  goto top;

blocked:
  System_Wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = PIXELS(1) / 4;
  Func_02004bc0(actor, 1);
}

#include "../../field/object/set_sprite_priority_b.c"

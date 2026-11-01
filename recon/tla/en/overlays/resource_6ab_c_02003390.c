// Matches: differ 0 (tla-en, 2026-11-01)
// The same function as overlay 6a8's Func_02003ee8, which closed first, and
// overlay 6aa's Func_02003884: only the callee addresses differ, so this is
// that draft's source with these names. The owner slides from the middle of
// its cell along its angle, snapped to a quarter turn, stopping at cell value
// 255, a rise over 8 pixels or a solid object. Twin of Imil_RunIceSlide in The
// Broken Seal, reworked: heading from the actor's own angle and not the keys,
// plus the search over the other objects.
// What the devices do is written up in 6a8's header: plain C leaves differ 276
// and skeleton gap 46, because PRE hoists sfp - 12 and actor + 34 above the
// retry label, and the sfp clobbers kill availability so the native's rebuilds
// stay. The device set is heavy; review it under S2 with 6a8's before adoption.
// Each tag's N was measured here by undoing that one device. Only tla-en was
// measured.
#include "types.h"
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

void Func_02002d90(void);
void Func_020041cc(struct Actor *actor, s32 mode);
void Func_02004204(struct Actor *actor, s32 x, s32 y, s32 z);
void Func_0200420c(struct Actor *actor);
s32 Func_02004224(u8 layer, s32 x, s32 z);
s32 Func_0200428c(s32 *a, s32 radiusA, struct Position *b, s32 radiusB);
s32 Map_GetTerrainHeight(u8 layer, s32 x, s32 z);
void Actor_SetAnimationSpeed(struct Actor *actor, s32 speed);
void Vector_AddPolarOffsetFar(s32 length, s32 angle, struct Position *position);
void wait(s32 frames);
void Func_02002d04(void);

void Func_02003390(void) {
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
  Func_02002d90();
  actor->speedLimit = PIXELS(1);
  actor->acceleration = PIXELS(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  // Default: one zero, spilled and reloaded at each use.
  zero = 0;
  actor->velocityX = zero;
  actor->velocityZ = zero;
  /* FAKEMATCH(temporary): cell holds the masked x; as one expression, 93 of 246 instructions differ in tla-en. */
  cell = actor->x & 0xfff00000;
  /* FAKEMATCH(temporary): i holds half a cell here and the search's count later; as a separate PIXELS(8), 18 of 246 instructions differ in tla-en. */
  i = PIXELS(8);
  position = &probe;
  position->x = startX = cell + i;
  position->y = actor->y;
  position->z = startZ = (actor->z & 0xfff00000) + i;
  x = startX;
  z = startZ;
  /* FAKEMATCH(barrier): the empty asm keeps the stores ahead of the layer's load; with no asm, 4 of 246 instructions differ in tla-en. */
  asm("");
  startBehavior = Func_02004224(*(layer = &actor->terrainId), startX, startZ);
  Vector_AddPolarOffsetFar(PIXELS(16), heading, position);
  nextBehavior = Func_02004224(*layer, position->x, position->z);
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
  Func_020041cc(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  actor->update = Func_02002d04;
  /* FAKEMATCH(barrier): the empty asm clobbering sfp and reading layer and nextBehavior; with no asm, 70 of 246 instructions differ in tla-en. */
  asm("" : : "r"(layer), "r"(nextBehavior) : "sfp");
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
      /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading next and actor; with no asm, 209 of 246 instructions differ in tla-en. */
      asm("" : : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),
          "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor) : "sfp",
          "r4");
      if (Func_0200428c(&other->x, radius, &probe, other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  Func_02004204(actor, next->x, next->y, next->z);
  Func_0200420c(actor);
  /* FAKEMATCH(barrier): the empty asm clobbering sfp; with no asm, 149 of 246 instructions differ in tla-en. */
  asm("" : : : "sfp");
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  /* FAKEMATCH(barrier): the asm tied to heading, with an r4 clobber so PRE leaves it, loads the angle ahead of the address; as heading in the call, 2 of 246 instructions differ in tla-en. */
  asm("" : "=r"(angle) : "0"(heading) : "r4");
  step = &probe;
  Vector_AddPolarOffsetFar(PIXELS(16), angle, step);
  /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the address, so next rebuilds it; as &probe in the call, 114 of 246 instructions differ in tla-en. */
  step = NULL;
  next = &probe;
  nextBehavior = Func_02004224(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1);
  Func_02004204(actor, x, actor->y, z);
  Func_0200420c(actor);
  wait(2);
  /* FAKEMATCH(barrier): the asm tied to actor and clobbering sfp kills both addresses on the back edge; with no asm, 190 of 246 instructions differ in tla-en. */
  asm("" : "=r"(actor) : "0"(actor) : "sfp");
  goto top;

blocked:
  wait(10);
done:
  actor->update = NULL;
  /* FAKEMATCH(barrier): the empty asm keeps the store ahead of the flags' load; with no asm, 2 of 246 instructions differ in tla-en. */
  asm("");
  actor->actionFlags |= 1;
  actor->acceleration = PIXELS(1) / 4;
  Func_020041cc(actor, 1);
}

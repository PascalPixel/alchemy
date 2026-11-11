#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Func_020026f4(void);
void VenusLighthouseEntrance_StopSparks(void);
void VenusLighthouseEntrance_UpdateSparkTrail(void);

/// The spark trail while the actor steps 96 out and 96 back.
void Func_020000c8(s32 actor) {
  struct Actor *object = Actor_Get(actor);

  Func_020026f4();
  object->update = VenusLighthouseEntrance_UpdateSparkTrail;
  object->priorityFlags = 0;
  {
    s32 a = actor;
    /* FAKEMATCH(pin): the 96 pinned to r1; as
     * Actor_CommitPositionAndActivate(a, 96, dz), the pin below kept, 2 of 35
     * instructions differ in tla-en. */
    register s32 dx asm("r1") = 96;
    /* FAKEMATCH(pin): the -96 pinned to r2, the same call's second; as
     * Actor_CommitPositionAndActivate(actor, 96, -96) with neither pin nor the
     * copy, 23 of 35 instructions differ in tla-en. */
    register s32 dz asm("r2") = -96;

    Actor_CommitPositionAndActivate(a, dx, dz);
  }
  {
    /* FAKEMATCH(pin): the arguments pinned in order, so the -96 is negated
     * after the actor is copied; as Actor_CommitPositionAndActivate(
     * actor, -96, 96), 2 of 35 instructions differ in tla-en. */
    register s32 a asm("r0") = actor;
    s32 dx = -96;

    Actor_CommitPositionAndActivate(a, dx, 96);
  }
  object->update = 0;
  object->priorityFlags = 1;
  FieldEvent_Wait(30);
  VenusLighthouseEntrance_StopSparks();
}

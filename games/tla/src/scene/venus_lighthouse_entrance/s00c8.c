#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020026f4(void);
void ObjectMotion_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void FieldEvent_WaitTicks(s32 ticks);
void VenusLighthouseEntrance_StopSparks(void);
void VenusLighthouseEntrance_UpdateSparkTrail(void);

/// The spark trail while the actor steps 96 out and 96 back.
void Func_020000c8(s32 actor) {
  struct ObjectRuntime *object = Object_GetById(actor);

  Func_020026f4();
  object->update = VenusLighthouseEntrance_UpdateSparkTrail;
  object->priorityFlags = 0;
  {
    /* FAKEMATCH(pin): the arguments pinned in order, so the -96 is built
     * afresh and negated last; as ObjectMotion_CommitPositionAndActivate(actor,
     * 96, -96), 23 of 35 instructions differ in tla-en. */
    register s32 a asm("r0") = actor;
    register s32 dx asm("r1") = 96;
    register s32 dz asm("r2") = -96;

    ObjectMotion_CommitPositionAndActivate(a, dx, dz);
  }
  {
    /* FAKEMATCH(pin): the arguments pinned in order, so the -96 is negated
     * after the actor is copied; as ObjectMotion_CommitPositionAndActivate(
     * actor, -96, 96), 2 of 35 instructions differ in tla-en. */
    register s32 a asm("r0") = actor;
    register s32 dx asm("r1") = -96;

    ObjectMotion_CommitPositionAndActivate(a, dx, 96);
  }
  object->update = 0;
  object->priorityFlags = 1;
  FieldEvent_WaitTicks(30);
  VenusLighthouseEntrance_StopSparks();
}

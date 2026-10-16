// Draft of overlay 64d's Func_020000c8, differ 4 of 35 (was 23): the spark
// trail while the actor steps out and back. As plain calls the two -96
// arguments share one pseudo kept in r8 across the first call (expand_call puts
// a constant costlier than one insn into a pseudo, and CSE reuses it); the
// native builds -96 afresh in r2 and then in r1. A register pin on the first
// one stops the sharing. Left: in each call the native negates after the other
// arguments are set (movs r1, #96; negs r2 and adds r0, r5; negs r1), where the
// pinned or precomputed -96 is negated first. Tried: the scene layer's
// forwarding inline (23), one-pass blocks round each call (6, the first call's
// setup then cannot start before the flag store), a block between the calls
// (23), s16 and unprototyped declarations (23), the pin on the second call (42)
// or both (4).
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020026f4(void);
void ObjectMotion_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void FieldEvent_WaitFrames(s32 frames);
void VenusLighthouseEntrance_StopSparks(void);
void VenusLighthouseEntrance_UpdateSparkTrail(void);

void Func_020000c8(s32 actor) {
  struct ObjectRuntime *object = Object_GetById(actor);

  Func_020026f4();
  object->update = VenusLighthouseEntrance_UpdateSparkTrail;
  object->priorityFlags = 0;
  {
    /* FAKEMATCH: the pin keeps this -96 out of a pseudo the second call would
     * share (differ 23 without it). */
    register s32 dz asm("r2") = -96;

    ObjectMotion_CommitPositionAndActivate(actor, 96, dz);
  }
  ObjectMotion_CommitPositionAndActivate(actor, -96, 96);
  object->update = 0;
  object->priorityFlags = 1;
  FieldEvent_WaitFrames(30);
  VenusLighthouseEntrance_StopSparks();
}

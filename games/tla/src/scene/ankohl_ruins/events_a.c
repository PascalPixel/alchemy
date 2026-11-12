/// @file
/// Scene code of overlay 0x686, in ROM order, from events_a's offset up to the next object the overlay links.

#include "ankohl_ruins.h"
#include "version.h"

void Func_02002e7c(s32 id, s32 a, s32 b);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_Start(s32 id, s32 a, s32 b) {
  Func_02002e7c(id, a, b);
}

void Func_02000384(void) {
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_Start(gMainRoutineWork.currentOwner, 6, 0x1000);
  Event_PlaySound(0x136);
  Event_SetFlag(0x122);
  FieldEvent_SetExitRequest(13);
}

// Includes the shared Effect_AdvanceMotion in this scene.
// Neither it nor its caller Func_02000418 exists in the Japanese scene.
#if TLA_PATCH_1
#include "../../field/field_effect/advance_motion.c"
#endif

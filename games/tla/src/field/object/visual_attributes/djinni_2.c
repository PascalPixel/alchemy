/// @file
/// Routines whose twins are in The Broken Seal's field/event_commands/djinni.c.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

void Actor_MoveHalfwayTowardTargetCopy(struct Actor *object) {
  struct Actor *target = object->watchedObject;

  object->x += (target->x - object->x) / 2;
  object->y += (target->y - object->y) / 2;
  object->z += (target->z - object->z) / 2;
}
#undef Actor_SetChildValueUnlessFifteenFar
#undef Actor_ReleaseFar
#undef gBattleEffect_PulseScales
#undef gEffectWork

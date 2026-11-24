/// @file
/// A field effect's motion each frame, drawn at the leader's priority.

#include "types.h"
#include "system/field_effect.h"
#include "system/main_routine_state.h"
#include "field/event.h"
#include "field/sprite.h"

/// FieldEffect_AdvanceMotion, then gives the effect's sprite the leader's
/// sprite priority, so the effect is drawn at the leader's depth.
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX = effect->velocity.x;
  s32 velocityZ;
  struct MainRoutineState *party;
  struct FieldSprite *sprite;
  struct Actor *leader;
  struct PlacedSprite *to;
  struct PlacedSprite *from;

  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;
  effect->velocity.x = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
  effect->velocity.z = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;
  effect->scaleX += effect->scaleRateX;
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;

  party = &gMainRoutineState;

  leader = Actor_GetById(party->leaderActorId);
  to = (struct PlacedSprite *)effect->sprite;
  from = (struct PlacedSprite *)leader->sprite;
  to->priority = from->priority;
}

/// @file
/// A field effect's motion each frame, drawn at the leader's priority.

#include "types.h"
#include "system/field_effect.h"
#include "system/main_routine_work.h"
#include "field/event.h"
#include "field/sprite.h"

/// FieldEffect_AdvanceMotion, then gives the effect's sprite the leader's
/// sprite priority, so the effect is drawn at the leader's depth.
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX = effect->velocity.x;
  s32 velocityZ;
  struct MainRoutineWork *party;
  struct Sprite *sprite;
  struct Actor *leader;
  struct PlacedSprite *to;
  struct PlacedSprite *from;

  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;
  effect->velocity.x = velocityX - velocityX / 18;
  effect->velocity.z = velocityZ - velocityZ / 16;
  effect->scaleX += effect->scaleRateX;
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;

  party = &gMainRoutineWork;

  leader = Actor_GetById(party->leaderActorId);
  to = (struct PlacedSprite *)effect->sprite;
  from = (struct PlacedSprite *)leader->sprite;
  to->priority = from->priority;
}

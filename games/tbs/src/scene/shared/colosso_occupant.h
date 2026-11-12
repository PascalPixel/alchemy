#include "types.h"
#include "actor.h"
#include "scene.h"
#include "main_routine_work.h"

// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3).

/// The leader pushes the actor standing in the next cell ahead one cell on
/// (a rock, a weight or a stone pillar on the stages), when nothing solid
/// stands beyond it or on top of it and the ground lets it move, with the
/// pushing pose and SE_PUSH_STEP.
static void Colosso_PushActorAhead(void) {
  struct Actor *leader;
  struct Actor *pushed;
  struct Actor *blocker;
  u32 step;
  u32 direction;
  struct FieldPosition position;
  s32 zero;
  s32 leaderId;

  leaderId = gMainRoutineWork.leaderActorId;
  leader = Actor_Get(leaderId);

  direction = leader->facing >> 12;

  step = gPushedActor_DirectionSteps[direction];
  position.x = leader->x.fixed + (step & 0xffff0000);
  position.y = leader->y.fixed;
  step <<= 16;
  position.z = leader->z.fixed + step;

  pushed = Actor_FindAtPosition(&position, leader);
  if (pushed == 0) {
    return;
  }

  step = gPushedActor_DirectionSteps[direction];
  position.x = pushed->x.fixed + (step & 0xffff0000);
  position.y = pushed->y.fixed;
  step <<= 16;
  position.z = pushed->z.fixed + step;

  blocker = Actor_FindAtPosition(&position, pushed);
  if (blocker != 0 && (blocker->collisionFlags & ACTOR_SOLID) != 0) {
    return;
  }

  position.x = pushed->x.fixed;
  position.y = pushed->y.fixed + PIXELS(16);
  position.z = pushed->z.fixed;

  blocker = Actor_FindAtPosition(&position, pushed);
  if (blocker != 0 && (blocker->collisionFlags & ACTOR_SOLID) != 0) {
    return;
  }

  pushed->layer = 2;
  zero = 0;

  step = gPushedActor_DirectionSteps[direction];
  position.x = pushed->x.fixed + (step & 0xffff0000);
  position.y = pushed->y.fixed;
  step <<= 16;
  position.z = pushed->z.fixed + step;

  if (FieldActor_CheckMovementCollision(pushed, &position) > 0) {
    return;
  }

  Actor_SelectAnimation(leader, 8);
  wait(15);

  pushed->speedLimit = PIXELS(1) / 5;
  pushed->acceleration = PIXELS(1) / 5;
  FieldActor_SetMoveTarget(pushed, position.x, position.y, position.z);

  leader->speedLimit = PIXELS(1) / 5;
  leader->acceleration = PIXELS(1) / 5;
  FieldActor_SetMoveTarget(leader, position.x, position.y, position.z);

  Sound_Play(SE_PUSH_STEP);
  Actor_WaitForMove(pushed);
  Sound_Play(SE_STOP_ACTION_SE);

  pushed->x.fixed = position.x;
  pushed->z.fixed = position.z;
  pushed->velocityX = zero;
  pushed->velocityZ = zero;

  Actor_SelectAnimation(leader, 1);
}

/// Returns the placed actor standing one cell, or else two cells, ahead
/// of the actor the player moves (straight ahead in the nearest of the
/// four directions), or 0 when there is none.
void *Colosso_FindActorAhead(void) {
  struct Actor *record;
  s32 facing;
  struct FieldPosition position;
  struct Actor *occupant;

  record = Actor_GetById(gMainRoutineWork.leaderActorId);

  facing = (record->facing + TRIG_TURN / 8) & 0xc000;

  position.x = (record->x.fixed & 0xfff00000) + PIXELS(8);
  position.y = record->y.fixed;
  position.z = (record->z.fixed & 0xfff00000) + PIXELS(8);
  Vector_AddPolarOffset(PIXELS(16), facing, &position);

  occupant = Actor_FindAtPosition(&position, record);
  if (occupant == 0) {
    position.x = (record->x.fixed & 0xfff00000) + PIXELS(8);
    position.y = record->y.fixed;
    position.z = (record->z.fixed & 0xfff00000) + PIXELS(8);
    Vector_AddPolarOffset(PIXELS(32), facing, &position);

    occupant = Actor_FindAtPosition(&position, record);
  }

  return occupant;
}

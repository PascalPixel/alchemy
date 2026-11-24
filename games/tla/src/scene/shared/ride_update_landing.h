/// Flame_UpdateForward for a flame flying east or west: it goes along x, and a
/// leader it reaches is knocked back along x.
void Flame_UpdateLanding(struct Actor *actor) {
  struct FlameHitState *state = RIDE_STATE;
  struct Actor *other = Actor_Get(gMainRoutineState.leaderActorId);
  struct FieldState *event = Ram_HeapSlots->fieldState;
  FIXED dx = actor->bounce;
  FIXED dz;
  s32 i;
  struct RideDirection *direction;

  actor->z.fixed += dx;
  actor->y.fixed += actor->gravity;
  dz = actor->sceneWord;
  actor->x.fixed += dz;
  actor->bounce = dx - dx / 18;
  actor->sceneWord = dz - dz / 16;
  actor->scaleX += actor->speedLimit;
  actor->scaleY += actor->acceleration;
  ((struct FieldSprite *)actor->sprite)->rotation += actor->timer;
  if (other->y.fixed == other->groundHeight && Actor_IsTouching(actor, other) &&
      gMainRoutineState.movementMode == LEADER_MOVEMENT_WALK) {
    if (actor->sceneWord != 0) {
      state->direction.z = 0;
      direction = &state->direction;
      if (actor->sceneWord > 0)
        direction->x = 1;
      else
        direction->x = -1;
    }
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor->z.fixed -= actor->bounce;
    actor->gravity += actor->bounce >> 2;
    actor->sceneWord = 0;
    state->active = 1;
    event->stepTrigger = state->hitTrigger;
  }
  for (i = 0; i < 4; i++) {
    s32 id = state->slots[i].id;

    if (id == 0)
      break;
    other = Actor_Get(id);
    if (other->y.fixed == other->groundHeight && Actor_IsBeside(actor, other)) {
      if (state->slots[i].blocked == 0) {
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        actor->z.fixed -= actor->sceneWord;
        actor->gravity += actor->sceneWord >> 2;
        actor->sceneWord = 0;
      } else {
        state->blocked = 1;
      }
    }
  }
}

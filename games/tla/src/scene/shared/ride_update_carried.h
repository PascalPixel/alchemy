/// A flame's update as it flies along z, slowing, growing and turning as it
/// goes: on reaching the leader where he stands it sets the way he is knocked
/// back, passes under his feet and rises away, and raises the hit trigger; on
/// reaching a slot's actor it rises away the same, unless the slot is blocked,
/// which marks the state blocked.
void Flame_UpdateForward(struct Actor *flame) {
  struct FlameHitState *state = RIDE_STATE;
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  struct FieldState *event = Ram_HeapSlots->fieldState;
  FIXED dx = flame->bounce;
  FIXED dz;
  s32 i;
  s32 id;

  flame->x.fixed += dx;
  flame->y.fixed += flame->gravity;
  dz = flame->riseBaseY;
  flame->z.fixed += dz;
  flame->bounce = dx - dx / 18;
  flame->riseBaseY = dz - dz / 16;
  flame->scaleX += flame->speedLimit;
  flame->scaleY += flame->acceleration;
  ((struct FieldSprite *)flame->sprite)->rotation += flame->timer;
  if (actor->y.fixed == actor->groundHeight && Actor_IsTouching(flame, actor) &&
      gMainRoutineState.movementMode == LEADER_MOVEMENT_WALK) {
    if (flame->riseBaseY != 0) {
      state->direction.x = 0;
      if (flame->riseBaseY > 0)
        state->direction.z = 1;
      else
        state->direction.z = -1;
    }
    flame->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    {
      s32 speed = flame->riseBaseY;

      flame->riseBaseY = 0;
      flame->z.fixed -= speed;
      flame->gravity += speed >> 2;
    }
    state->active = 1;
    event->stepTrigger = state->hitTrigger;
  }
  for (i = 0; i < 4 && (id = state->slots[i].id) != 0; i++) {
    actor = Actor_Get(id);

    if (actor->y.fixed == actor->groundHeight && Actor_IsBeside(flame, actor)) {
      if (state->slots[i].blocked == 0) {
        FIXED speed;

        flame->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        speed = flame->riseBaseY;
        flame->riseBaseY = 0;
        flame->z.fixed -= speed;
        flame->gravity += speed >> 2;
      } else {
        state->blocked = 1;
      }
    }
  }
}

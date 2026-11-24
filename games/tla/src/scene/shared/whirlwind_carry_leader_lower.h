/// The leader spins in the whirlwind that caught it, then drops beside it and
/// the camera follows again: the lighthouse's lower rooms' copy of
/// Whirlwind_CarryLeader.
void Whirlwind_CarryLeaderLower(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  struct Actor *linked = actor->watchedActor;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  state->glowHeld = 0;
  actor->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteFlags(actor, 0);
  while (*(u8 *)&linked->riseEnabled != 0) {
    actor->x.fixed = linked->x.fixed;
    actor->y.fixed = linked->y.fixed + i;
    actor->z.fixed = linked->z.fixed;
    if (i <= toFIXED(4) - 1)
      i += toFIXED(0.1);
    actor->facing = (u8)gFrameCount * FACING_STEP;
    actor->priorityFlags ^= ACTOR_PRIORITY_UNDERFOOT << (gFrameCount & 1);
    System_Wait(1);
  }
  state->glow = 1;
  state->glowHeld = 1;
  facing = linked->facing;
  actor->x.fixed =
      FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(linked->x.fixed) & ~1) +
                             FACING_EIGHTH_INDEX(facing) - 1);
  actor->z.fixed =
      FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(linked->z.fixed) & ~1) -
                             (FACING_EIGHTH_INDEX(facing) & 2) + 1);
  actor->velocityY = toFIXED(6);
  actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpriteFlags(actor, 1);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  shadow = EventCamera_GetActor();
  shadow->speedLimit = toFIXED(8);
  shadow->acceleration = toFIXED(1);
  shadow->motionFlags = 0;
  FieldActor_SetMoveTargetFX(
      shadow, actor->x.fixed,
      Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed), actor->z.fixed);
  i = 0;
  while (i <= 59 && actor->y.fixed > actor->groundHeight + toFIXED(16)) {
    i++;
    System_Wait(1);
  }
  Event_PlaySound(SE_LANDING_THUD);
  i = 0;
  while (i <= 59 && actor->velocityY != 0) {
    i++;
    System_Wait(1);
  }
  FieldActor_WaitForMove(shadow);
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  state->glowHeld = 1;
  state->stepTrigger = 0;
  Event_End();
}

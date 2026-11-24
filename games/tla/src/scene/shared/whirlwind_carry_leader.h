/// A whirlwind blown out of a wall catches Felix: he spins inside it, lifted a
/// little, as it carries him along, and when it dies away he drops to the
/// ground beside where it stopped with a thud, and the camera follows him
/// again.
void Whirlwind_CarryLeader(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  struct Actor *linked = actor->watchedActor;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 rise;
  s32 frame;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  rise = 0;
  state->glowHeld = rise;

  actor->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteFlags(actor, 0);

  while (*(u8 *)&linked->riseEnabled != 0) {
    actor->x.fixed = linked->x.fixed;
    actor->y.fixed = linked->y.fixed + rise;
    actor->z.fixed = linked->z.fixed;
    if (rise < toFIXED(4))
      rise += toFIXED(0.1);
    actor->facing = (u8)gFrameCount * FACING_STEP;
    // As he spins he is drawn now under the wind, now over it.
    p = &actor->priorityFlags;
    *p ^= ACTOR_PRIORITY_UNDERFOOT << (gFrameCount & 1);
    wait(1);
  }
  state->glow = 1;
  state->glowHeld = 1;
  angle = linked->facing;
  actor->x.fixed = FIELD_HALF_CELL_ORIGIN(
      (FIELD_HALF_CELL(linked->x.fixed) & ~1) + FACING_EIGHTH_INDEX(angle) - 1);
  actor->z.fixed =
      FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(linked->z.fixed) & ~1) -
                             (FACING_EIGHTH_INDEX(angle) & 2) + 1);
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
  frame = 0;
  if (actor->y.fixed > actor->groundHeight + toFIXED(16)) {
    do {
      wait(1);
      frame++;
      if (frame >= 60)
        break;
    } while (actor->y.fixed > actor->groundHeight + toFIXED(16));
  }
  Event_PlaySound(SE_LANDING_THUD);
  frame = 0;
  if (actor->velocityY != 0) {
    do {
      wait(1);
      frame++;
      if (frame >= 60)
        break;
    } while (actor->velocityY != 0);
  }
  FieldActor_WaitForMove(shadow);
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  state->glowHeld = 1;
  state->stepTrigger = 0;
  Event_End();
}

/// The leader spins in the whirlwind that caught it, then drops beside it and
/// the camera follows again.
void Whirlwind_CarryLeader(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *linked = actor->watchedActor;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 rise;
  s32 frame;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  rise = 0;
  work->glowHeld = rise;

  actor->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteFlags(actor, 0);

  while (*(u8 *)&linked->riseEnabled != 0) {
    actor->x.fixed = linked->x.fixed;
    actor->y.fixed = linked->y.fixed + rise;
    actor->z.fixed = linked->z.fixed;
    if (rise < toFIXED(4))
      rise += toFIXED(0.1);
    actor->facing = (u8)gFrameCount * FACING_STEP;
    p = &actor->priorityFlags;
    *p ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  work->glow = 1;
  work->glowHeld = 1;
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
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  work->glowHeld = 1;
  work->stepTrigger = 0;
  Event_End();
}

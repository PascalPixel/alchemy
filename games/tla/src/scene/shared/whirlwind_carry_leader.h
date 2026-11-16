/// The leader spins in the whirlwind that caught it, then drops beside it and
/// the camera follows again.
void Whirlwind_CarryLeader(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *linked = actor->watchedActor;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->glowHeld = i;

  actor->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteFlags(actor, 0);

  while (*(u8 *)&linked->riseEnabled != 0) {
    actor->x.fixed = linked->x.fixed;
    actor->y.fixed = linked->y.fixed + i;
    actor->z.fixed = linked->z.fixed;
    if (i <= toFIXED(4) - 1)
      i += toFIXED(0.1);
    actor->facing = (u8)gFrameCount << 12;
    p = &actor->priorityFlags;
    *p ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->glow = 1;
  work->glowHeld = 1;
  angle = linked->facing;
  actor->x.fixed = (((linked->x.fixed >> 19) & ~1) + (angle >> 13) - 1) << 19;
  actor->z.fixed = (((linked->z.fixed >> 19) & ~1) - ((angle >> 13) & 2) + 1)
                   << 19;
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
  if (actor->y.fixed > actor->groundHeight + toFIXED(16)) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->y.fixed > actor->groundHeight + toFIXED(16));
  }
  Event_PlaySound(SE_LANDING_THUD);
  i = 0;
  if (actor->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
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

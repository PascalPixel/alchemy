/// Whirlwind_CarryLeader as the lower floors' copies compile it.
void Whirlwind_CarryLeaderLower(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *linked = actor->watchedActor;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->glowHeld = 0;
  actor->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteFlags(actor, 0);
  while (*((u8 *)linked + 0x63) != 0) {
    actor->x.fixed = linked->x.fixed;
    actor->y.fixed = linked->y.fixed + i;
    actor->z.fixed = linked->z.fixed;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->facing = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->glow = 1;
  work->glowHeld = 1;
  facing = linked->facing;
  actor->x.fixed = (((linked->x.fixed >> 19) & ~1) + (facing >> 13) - 1) << 19;
  actor->z.fixed = (((linked->z.fixed >> 19) & ~1) - ((facing >> 13) & 2) + 1)
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
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  work->glowHeld = 1;
  work->stepTrigger = 0;
  Event_End();
}

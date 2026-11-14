/// The leader spins in the whirlwind that caught it, then drops beside it and
/// the camera follows again.
void Whirlwind_CarryLeader(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->glowHeld = i;

  actor->motionFlags = 4;
  Actor_SetSpriteFlags(actor, 0);

  while (*((u8 *)linked + 0x63) != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->facing = (u8)gFrameCount << 12;
    p = &actor->priorityFlags;
    *p ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->glow = 1;
  work->glowHeld = 1;
  angle = linked->facing;
  actor->x = (((linked->x >> 19) & ~1) + (angle >> 13) - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - ((angle >> 13) & 2) + 1) << 19;
  actor->velocityY = px(6);
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSpriteFlags(actor, 1);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  shadow = EventCamera_GetActor();
  shadow->speedLimit = px(8);
  shadow->acceleration = px(1);
  shadow->motionFlags = 0;
  FieldActor_SetMoveTargetFixed(
      shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z), actor->z);
  i = 0;
  if (actor->y > actor->terrainHeight + px(16)) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->y > actor->terrainHeight + px(16));
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
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->glowHeld = 1;
  work->stepTrigger = 0;
  Event_End();
}

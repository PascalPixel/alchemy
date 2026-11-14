/// The leader spins up and away to `entrance`.
void Leader_SpinAway(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  leader->motionFlags = 0;
  leader->priorityFlags |= 0x20;
  Event_PlaySound(SE_SPLASH_SMALL);
  Actor_SelectAnimation(leader, 18);
  Event_PlaySound(SE_612);
  for (i = 0; i < 60; i++) {
    s32 angle;

    if (i == 30)
      Event_FinishScreenTransition();
    leader->y -= 0x4000;
    angle = leader->facing;
    leader->facing = angle + 0x1000;
    if ((i & 7) == 0)
      ApojiiIslands_SpawnSplash(Actor_Get(15));
    System_Wait(1);
  }
  Event_End();
  FieldEvent_SetExitRequest(entrance);
}

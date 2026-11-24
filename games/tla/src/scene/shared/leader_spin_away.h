/// Unused: the leader spins round, sinking, with splashes at the place of
/// Aqua Rock's Vial chest room's third puddle, and leaves by `entrance`.
UNUSED void Leader_SpinAway(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineState.leaderActorId, 1);
  leader->motionFlags = 0;
  leader->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Event_PlaySound(SE_SPLASH_SMALL);
  Actor_SelectAnimation(leader, ANIM_LEADER_TUMBLE_IN_WATER);
  Event_PlaySound(SE_SPIN_AWAY);
  for (i = 0; i < 60; i++) {
    s32 angle;

    if (i == 30)
      Event_FinishScreenTransition();
    leader->y.fixed -= toFIXED(1) / 4;
    angle = leader->facing;
    leader->facing = angle + TRIG_TURN / 16;
    if ((i & 7) == 0)
      Actor_SpawnWadingSplash(Actor_Get(ACTOR_VIAL_CHEST_PUDDLE_C));
    System_Wait(1);
  }
  Event_End();
  FieldEvent_SetExitRequest(entrance);
}

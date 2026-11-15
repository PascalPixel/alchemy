/// The leader sinks away to `entrance`.
void Leader_SinkAway(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 *owner = &gMainRoutineWork.leaderActorId;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_SPARKLING_PASSAGE);
  leader->update = (void (*)(void))Actor_EmitMist;
  leader->motionFlags = 0;
  leader->speedLimit = toFIXED(1) / 5;
  Actor_SetAnimation(*owner, 2);
  Actor_SetDestinationOffset(*owner, 0, -8);
  FieldActor_WaitForMoveById(*owner);
  FieldActor_SetSpriteColor(Actor_Get(*owner), 9);
  Actor_SetSpriteFlags(Actor_Get(*owner), 0);
  leader->update = 0;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(entrance);
  Event_End();
}

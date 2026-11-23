/// Felix walks into a door: it opens, he walks through it, lined up with it,
/// and the screen closes as he leaves by exit. Eastern Alhafra's ship cabin
/// and Lemuria's palace doors open so.
void Leader_OpenDoorAndEnter(const struct MapGridCellAnimation *door, s32 exit,
                             s32 unused) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(door->steps, door->x, door->y);
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(exit);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

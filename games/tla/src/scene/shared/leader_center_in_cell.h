/// Stops the leader in the middle of its cell and raises the camera.
void Leader_CenterInCell(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = actor->x.fixed;
  s32 z = actor->z.fixed;

  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 4);

  x >>= 20;
  z >>= 20;
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, (x << 4) + 8,
                          (z << 4) + 8);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(actor, (x << 20) + toFIXED(8),
                                       actor->y.fixed, (z << 20) + toFIXED(8));
  FieldEvent_Wait(4);
  EventCamera_GetActor()->y.fixed -= toFIXED(8);
}

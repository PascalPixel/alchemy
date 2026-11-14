/// Stops the leader in the middle of its cell and raises the camera.
void Leader_CenterInCell(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = object->x.fixed;
  s32 z = object->z.fixed;

  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 4);

  x >>= 20;
  z >>= 20;
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, (x << 4) + 8,
                          (z << 4) + 8);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(object, (x << 20) + px(8),
                                       object->y.fixed, (z << 20) + px(8));
  FieldEvent_Wait(4);
  EventCamera_GetActor()->y.fixed -= px(8);
}

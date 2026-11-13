/// Stops the leader in the middle of its cell and raises the camera.
void Leader_CenterInCell(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x;
  s32 z = object->z;

  Actor_SetSpeed(gMainRoutineWork.currentOwner, 4);

  x >>= 20;
  z >>= 20;
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, (x << 4) + 8,
                          (z << 4) + 8);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(object, (x << 20) + PIXELS(8), object->y,
                                       (z << 20) + PIXELS(8));
  FieldEvent_Wait(4);
  EventCamera_GetActor()->y -= 0x80000;
}

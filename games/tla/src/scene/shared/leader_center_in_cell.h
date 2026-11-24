/// Walks the leader to the middle of the cell he stands on, as he does before
/// dropping through a cracked tile, sets him there and lowers the camera's
/// point 8 pixels.
void Leader_CenterInCell(void) {
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  s32 x = actor->x.fixed;
  s32 z = actor->z.fixed;

  Actor_SetSpeed(gMainRoutineState.leaderActorId, 4);

  x = FIELD_CELL(x);
  z = FIELD_CELL(z);
  Actor_MoveToAndWaitById(gMainRoutineState.leaderActorId,
                          x * MAPGRID_CELL_PIXELS + MAPGRID_CELL_PIXELS / 2,
                          z * MAPGRID_CELL_PIXELS + MAPGRID_CELL_PIXELS / 2);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(actor, FIELD_CELL_CENTER(x),
                                       actor->y.fixed, FIELD_CELL_CENTER(z));
  FieldEvent_Wait(4);
  EventCamera_GetActor()->y.fixed -= toFIXED(8);
}

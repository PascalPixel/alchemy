/// Drops the leader through a cracked tile: it gives way and the leader spins
/// down into it while the screen fades.
void CrackedTile_Fall(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = (FIELD_CELL(leader->x.fixed)) | (s16)(table->x & 64);
  s32 z = (FIELD_CELL(leader->z.fixed)) | (s16)(table->z & 64);

  Leader_CenterInCell();
  Event_PlaySound(SE_SPLASH);
  MapGrid_CopyCells(table->x, table->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  leader->facing = FACING_SOUTH;
  leader->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  Io_Write16(0xf1f, &REG_WINOUT);
  REG_DISPCNT |= 0x8000;
  table->shown = 1;
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 28);
  FieldEvent_Wait(16);
  x = 0;
  do {
    if (x == 5)
      Event_PlaySound(SE_DUST_PUFF);
    leader->scaleX += -(FIXED_ONE / 64);
    leader->scaleY += -(FIXED_ONE * 3 / 256);
    leader->y.fixed += -(toFIXED(3) / 5);
    System_Wait(1);
    x++;
  } while (x <= 39);
  Actor_Get(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  table->shown = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

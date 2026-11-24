/// Cracks the tile under the leader, swapping in its cracked picture.
void CrackedTile_Crack(struct CellTarget *table,
                       struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  s32 x;
  s32 z;

  x = (FIELD_CELL(leader->x.fixed)) | (s16)(table->x & 64);
  z = (FIELD_CELL(leader->z.fixed)) | (s16)(table->z & 64);
  Leader_CenterInCell();
  Event_PlaySound(SE_RATTLE);
  MapGrid_CopyCells(table->x, table->z + 1, 1, 1, x, z);
  FieldEvent_Wait(12);
}

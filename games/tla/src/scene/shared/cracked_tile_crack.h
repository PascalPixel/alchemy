/// Cracks the tile under the leader, swapping in its cracked picture.
void CrackedTile_Crack(struct CellTarget *table,
                       struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x;
  s32 z;

  x = (leader->x >> 20) | (s16)(table->x & 64);
  z = (leader->z >> 20) | (s16)(table->z & 64);
  Leader_CenterInCell();
  Event_PlaySound(SE_RATTLE);
  MapGrid_CopyCells(table->x, table->z + 1, 1, 1, x, z);
  FieldEvent_Wait(12);
}

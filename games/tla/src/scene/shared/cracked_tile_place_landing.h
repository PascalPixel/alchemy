/// On landing below a cracked tile, puts the leader on the cell the flag bytes
/// saved, offset from (x, z), and starts the screen effect.
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 dx = FieldGameFlag_GetByte(gCrackedTiles.flagByte);
  s32 dz = FieldGameFlag_GetByte(gCrackedTiles.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gCrackedTiles.flagByte, 0);
    GameFlag_SetByte(gCrackedTiles.flagByte + 8, 0);
    leader->x.fixed = ((dx + x) << 20) + 0x80000;
    leader->z.fixed = ((dz + z) << 20) + 0x80000;
    marker = work->camera;
    marker->x.fixed = leader->x.fixed;
    marker->z.fixed = leader->z.fixed;
    MapGrid_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    gFieldWork->transition = effect;
  }
}

/// When the leader has fallen through a cracked tile, sets him in the middle of
/// the cell the two flag bytes saved, counted from cell (x, z), clears them,
/// centres the view on him and drops him in with the screen transition effect.
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 dx = FieldGameFlag_GetByte(gCrackedTiles.flagByte);
  s32 dz = FieldGameFlag_GetByte(gCrackedTiles.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gCrackedTiles.flagByte, 0);
    GameFlag_SetByte(gCrackedTiles.flagByte + 8, 0);
    leader->x.fixed = FIELD_CELL_CENTER(dx + x);
    leader->z.fixed = FIELD_CELL_CENTER(dz + z);
    marker = state->camera;
    marker->x.fixed = leader->x.fixed;
    marker->z.fixed = leader->z.fixed;
    MapGrid_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    gFieldState->transition = effect;
  }
}

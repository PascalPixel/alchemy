/// Run each frame: when the leader steps onto a whole tile it cracks,
/// and onto a cracked one other than the one it cracked last, the leader falls
/// through it; 1 when it falls.
s32 CrackedTile_Step(void) {
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  struct CellTarget *state = &gCrackedTiles;
  struct CellTargetEntry *entry = state->entries;
  s32 i;

  for (i = 0; i < state->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        CrackedTile_Crack(state, entry);
        Event_SetFlag(entry->flag);
        state->current = i;
        break;
      }
      if (i == state->current)
        break;
      state->current = i;
      CrackedTile_Fall(state, entry);
      GameFlag_SetByte(state->flagByte, x);
      GameFlag_SetByte(state->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

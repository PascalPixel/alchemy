/// Starts the geysers: clears their state unless the party keeps its place,
/// loads the column's tiles and starts Geyser_Update and Geyser_Draw.
void Geyser_Start(s32 unused, s32 firstTerrain) {
  struct GeyserState *state = GEYSER_STATE;
  void *buffer;

  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWords((s32)state, sizeof(struct GeyserState));
  buffer = (void *)Value1((s32 (*)())Heap_AllocEwram, 2048);
  Decompress_TaggedLz(gField_GeyserTiles, buffer);
  state->entry = VramCache_FindFreeEntry();
  VramCache_Load(state->entry, 2048, buffer);
  Heap_Release(buffer);
  state->block = Heap_AllocEwram(GEYSER_SPRITES * 3 * sizeof(u32));
  state->firstTerrain = firstTerrain;
  Task_Add((s32)Geyser_Update, TASK_PASS_FRAME + 13);
  Task_Add((s32)Geyser_Draw, TASK_PASS_FRAME);
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
}

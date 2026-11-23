/// Starts the geysers: clears their work unless the party keeps its place,
/// loads the column's tiles and starts Geyser_Update and Geyser_Draw.
void Geyser_Start(s32 unused, s32 firstTerrain) {
  struct GeyserWork *work = GEYSER_WORK;
  void *buffer;

  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWords((s32)work, sizeof(struct GeyserWork));
  buffer = (void *)Value1((s32 (*)())Heap_AllocEwram, 2048);
  Decompress_TaggedLz(gField_GeyserTiles, buffer);
  work->entry = VramCache_FindFreeEntry();
  VramCache_Load(work->entry, 2048, buffer);
  Heap_Release(buffer);
  work->block = Heap_AllocEwram(GEYSER_SPRITES * 3 * sizeof(u32));
  work->firstTerrain = firstTerrain;
  Task_Add((s32)Geyser_Update, TASK_PASS_FRAME + 13);
  Task_Add((s32)Geyser_Draw, TASK_PASS_FRAME);
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
}

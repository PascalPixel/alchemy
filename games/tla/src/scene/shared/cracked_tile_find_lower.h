/// CrackedTile_Find under the name the lower floors call it by.
s32 CrackedTile_FindLower(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gCrackedTiles;
  struct CellTargetEntry *entry = table->entries;
  struct MapWork *map;
  struct MapLayer *layer;
  struct MapCell *cells;
  struct MapCell *cell;
  s32 x0, z0, width, height, i, j;
  u32 *buffer;
  u32 event, last;
  s32 cx, cz;
  s32 k;

  Iwram_ClearWordsEntry(table, 4);
  table->kind = kind;
  table->firstFlag = flag;
  table->flagByte = flagByte;
  table->x = x;
  table->z = z;
  table->current = -1;
  table->count = 0;
  map = Ram_HeapSlots->mapGrid;
  layer = &map->layers[0];
  cells = layer->cells;
  x0 = (layer->offsetX + map->minX) >> 20;
  z0 = (layer->offsetY + map->minZ) >> 20;
  width = (map->maxX - map->minX) >> 20;
  height = (map->maxZ - map->minZ) >> 20;
  cells += ((map->minZ >> 20) << 7) + (map->minX >> 20);
  for (j = 0; j < height; j++) {
    cell = cells + (j << 7);
    for (i = 0; i < width; i++, cell++) {
      event = cell->collisionCode;
      if (event != 0 && event >= kind) {
        last = kind + 1;
        if (event <= last) {
          cx = i + x0;
          entry->x = cx;
          cz = j + z0;
          entry->z = cz;
          entry->flag = flag;
          flag++;
          if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
            if (event == last)
              Event_SetFlag(entry->flag);
          } else if (Event_GetFlag(entry->flag)) {
            MapGrid_CopyCells(table->x, table->z + 1, 1, 1,
                              cx | (s16)(table->x & 64),
                              cz | (s16)(table->z & 64));
          }
          entry++;
          table->count++;
        }
      }
    }
  }
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
    s32 leaderX = FIELD_CELL(leader->x.fixed);
    s32 leaderZ = FIELD_CELL(leader->z.fixed);

    entry = table->entries;
    for (k = 0; k < table->count; k++) {
      if (entry->x == leaderX && entry->z == leaderZ)
        table->current = k;
      entry++;
    }
  }
  buffer = Heap_Alloc(256);
  {
    u32 *word = buffer;

    for (k = 0; k < 64; k++)
      *word++ = 0x11111111;
  }
  table->tile = VramCache_FindFreeEntry();
  Vram_Load(table->tile, 256, buffer);
  Heap_Release(buffer);
  Task_AddFrame(CrackedTile_Update, TASK_PASS_VBLANK);
  return table->count;
}

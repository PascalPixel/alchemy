/// Finds the room's cracked tiles: every cell whose event is `kind` (whole) or
/// `kind + 1` (cracked), each given the next flag from `flag`; returns how
/// many.
s32 CrackedTile_Find(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gCrackedTiles;
  struct CellTargetEntry *entry = table->entries;
  struct MapState *map;
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
  x0 = FIELD_CELL(layer->offsetX + map->minX);
  z0 = FIELD_CELL(layer->offsetY + map->minZ);
  width = FIELD_CELL(map->maxX - map->minX);
  height = FIELD_CELL(map->maxZ - map->minZ);
  cells += FIELD_CELL(map->minZ) * MAPGRID_SIZE + FIELD_CELL(map->minX);
  for (j = 0; j < height; j++) {
    cell = cells + j * MAPGRID_SIZE;
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
    struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
    s32 leaderX = FIELD_CELL(leader->x.fixed);
    s32 leaderZ = FIELD_CELL(leader->z.fixed);

    entry = table->entries;
    for (k = 0; k < table->count; k++) {
      if (entry->x == leaderX && entry->z == leaderZ)
        table->current = k;
      entry++;
    }
  }
  // The hole mask CrackedTile_Update draws into the OBJ window: a 16 by 32
  // sprite whose every pixel is colour 1.
  buffer = Heap_Alloc(HOLE_MASK_BYTES);
  {
    u32 *word = buffer;

    for (k = 0; k < HOLE_MASK_BYTES / 4; k++)
      *word++ = PIXELS_4BPP(1);
  }
  table->tile = VramCache_FindFreeEntry();
  Vram_Load(table->tile, HOLE_MASK_BYTES, buffer);
  Heap_Release(buffer);
  Task_AddFrame(CrackedTile_Update, TASK_PASS_FRAME);
  return table->count;
}

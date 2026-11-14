/// Finds the room's cracked tiles: every cell whose event is `kind` (whole) or
/// `kind + 1` (cracked), each given the next flag from `flag`; returns how
/// many.
s32 CrackedTiles_Find(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gCrackedTiles;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *cells;
  struct CellMapCell *rows;
  struct CellMapCell *cell;
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
  map = (struct CellMapWork *)Ram_HeapSlots->mapWork;
  layer = &map->layers[0];
  cells = layer->cells;
  x0 = (layer->originX + map->minX) >> 20;
  z0 = (layer->originZ + map->minZ) >> 20;
  width = (map->maxX - map->minX) >> 20;
  height = (map->maxZ - map->minZ) >> 20;
  cells += ((map->minZ >> 20) << 7) + (map->minX >> 20);
  /* Goto loops: as for loops (either one) loop.c moves the 64 of the cell
   */
  /* copy's masks out of them and 321 of 268 instructions differ. */
  j = 0;
  rows = cells;
  if (j < height) {
  row:
    cell = rows + (j << 7);
    i = 0;
    if (i < width) {
    column:
      event = cell->event;
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
      i++;
      cell++;
      if (i < width)
        goto column;
    }
    j++;
    if (j < height)
      goto row;
  }
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
    s32 leaderX = leader->x >> 20;
    s32 leaderZ = leader->z >> 20;

    entry = table->entries;
    k = 0;
    if (k < table->count) {
    search:
      if (entry->x == leaderX && entry->z == leaderZ)
        table->current = k;
      entry++;
      k++;
      if (k < table->count)
        goto search;
    }
  }
  buffer = Heap_Alloc(0x100);
  {
    u32 *word = buffer;

    k = 63;
  fill:
    *word++ = 0x11111111;
    if (--k >= 0)
      goto fill;
  }
  table->tile = VramCache_FindFreeEntry();
  Vram_Load(table->tile, 0x100, buffer);
  Heap_Release(buffer);
  Task_AddFrame(CrackedTiles_Update, 0x480);
  return table->count;
}

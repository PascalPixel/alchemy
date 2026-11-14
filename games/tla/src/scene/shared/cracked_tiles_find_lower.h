/// CrackedTiles_Find as the lower floors' copies compile it.
s32 CrackedTiles_FindLower(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gCrackedTiles;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *rows;
  struct CellMapCell *cell;
  s32 x0, z0, width, height, i, j;
  u32 *buffer;
  u32 event, last;
  s32 cz, cx;

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
  rows = layer->cells;
  x0 = (layer->originX + map->minX) >> 20;
  z0 = (layer->originZ + map->minZ) >> 20;
  width = (map->maxX - map->minX) >> 20;
  height = (map->maxZ - map->minZ) >> 20;
  rows += ((map->minZ >> 20) << 7) + (map->minX >> 20);
  /* Goto loops: as for loops (either one) loop.c moves the 64 of the cell
   */
  /* copy's masks out of them and 321 of 268 instructions differ. */
  j = 0;
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
            s32 sourceX = table->x;
            s32 sourceZ = table->z;

            cx |= (s16)(table->x & 64);
            cz |= (s16)(table->z & 64);
            MapGrid_CopyCells(sourceX, sourceZ + 1, 1, 1, cx, cz);
          }
          entry++;
          table->count++;
          /* FAKEMATCH(barrier): an empty asm on the stored count. */
          asm("" : "=m"(table->count) : "m"(table->count) : "r3");
          /* FAKEMATCH(barrier): an empty asm on the word over the count. */
          asm("" : "=m"(*(s32 *)&table->z));
        }
      }
    columnNext:
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life. */
      asm("");
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life. */
      asm("");
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life. */
      asm("");
      i++;
      cell++;
      if (i < width)
        goto column;
    }
  rowNext:
    j++;
    if (j < height)
      goto row;
  }
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
    s32 leaderX = leader->x >> 20;
    s32 leaderZ = leader->z >> 20;
    s32 k;

    entry = table->entries;
    k = 0;
    if (k < table->count) {
    leaderRow:
      do {
        if (entry->x == leaderX && entry->z == leaderZ)
          table->current = k;
      leaderNext:
        k++;
        entry++;
        /* A cast read: gcse keeps it in the loop. */
      } while (k < *(s16 *)&table->count);
    }
  }
  buffer = Heap_Alloc(0x100);
  {
    u32 *word = buffer;
    s32 count = 63;

  fill:
    *word++ = 0x11111111;
  fillNext:
    if (--count >= 0)
      goto fill;
  }
  table->tile = VramCache_FindFreeEntry();
  Vram_Load(table->tile, 0x100, buffer);
  Heap_Release(buffer);
  Task_AddFrame(CrackedTiles_Update, 0x480);
  return table->count;
}

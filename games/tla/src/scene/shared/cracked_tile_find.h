/* Finds the room's cracked tiles: every cell whose event is `kind` (whole) or
 */
/* `kind + 1` (cracked), each given the next flag from `flag`; returns how */
/* many. */
s32 scCrackedTileFind(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
	CELL_TARGET *table = &CrackedTiles;
	CELL_TARGET_ENTRY *entry = table->entries;
	MAP_STATE *map;
	MAP_LAYER *layer;
	MAP_CELL *cells;
	MAP_CELL *cell;
	s32 x0, z0, width, height, i, j;
	u32 *buffer;
	u32 event, last;
	s32 cx, cz;
	s32 k;

	syWordsClearEntry(table, 4);
	table->kind = kind;
	table->firstFlg = flag;
	table->flgByte = flagByte;
	table->x = x;
	table->z = z;
	table->cur = -1;
	table->cnt = 0;
	map = Ram_HeapSlots->mapGrid;
	layer = &map->layers[0];
	cells = layer->cells;
	x0 = FIELD_CELL(layer->ofsX + map->minX);
	z0 = FIELD_CELL(layer->ofsY + map->minZ);
	width = FIELD_CELL(map->maxX - map->minX);
	height = FIELD_CELL(map->maxZ - map->minZ);
	cells += FIELD_CELL(map->minZ) * MAPGRID_SIZE + FIELD_CELL(map->minX);
	for(j = 0; j < height; j++) {
		cell = cells + j * MAPGRID_SIZE;
		for(i = 0; i < width; i++, cell++) {
			event = cell->collisionCode;
			if(event != 0 && event >= kind) {
				last = kind + 1;
				if(event <= last) {
					cx = i + x0;
					entry->x = cx;
					cz = j + z0;
					entry->z = cz;
					entry->flg = flag;
					flag++;
					if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
						if(event == last)
							fdEventSetFlag(entry->flg);
					}
					else if(fdEventGetFlag(entry->flg)) {
						fdMapGridCopyCells(table->x, table->z + 1, 1, 1,
										   cx | (s16)(table->x & 64),
										   cz | (s16)(table->z & 64));
					}
					entry++;
					table->cnt++;
				}
			}
		}
	}
	if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		s32 leaderX = FIELD_CELL(leader->x.fixed);
		s32 leaderZ = FIELD_CELL(leader->z.fixed);

		entry = table->entries;
		for(k = 0; k < table->cnt; k++) {
			if(entry->x == leaderX && entry->z == leaderZ)
				table->cur = k;
			entry++;
		}
	}
	/* The hole mask fdCrackedTileUpdate draws into the OBJ window: a 16 by 32
	 */
	/* sprite whose every pixel is colour 1. */
	buffer = syHeapAlloc(HOLE_MASK_BYTES);
	{
		u32 *word = buffer;

		for(k = 0; k < HOLE_MASK_BYTES / 4; k++)
			*word++ = PIXELS_4BPP(1);
	}
	table->tile = syVramCacheFindFreeEntry();
	mnVramLoad(table->tile, HOLE_MASK_BYTES, buffer);
	syHeapRelease(buffer);
	syTaskAddFrame(fdCrackedTileUpdate, TASK_PASS_FRAME);
	return table->cnt;
}

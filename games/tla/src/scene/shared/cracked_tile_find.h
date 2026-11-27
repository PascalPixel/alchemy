/* Finds the room's cracked tiles: every metatile whose script is `kind` (whole)
   or `kind + 1` (cracked), each given the next flag from `flag`; returns how
   many. */
Sint32 scCrackedTileFind(
	Uint32 kind, Sint32 flag, Sint32 flagByte, Sint32 x, Sint32 z)
{
	METATILE_TARGET		  *table = &CrackedTiles;
	METATILE_TARGET_ENTRY *entry = table->entries;
	MAP_STATE			  *map;
	MAP_LAYER			  *layer;
	MAP_METATILE		  *metatiles;
	MAP_METATILE		  *metatile;
	Sint32				   x0, z0, width, height, i, j;
	Uint32				  *buffer;
	Uint32				   event, last;
	Sint32				   cx, cz;
	Sint32				   k;

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
	metatiles = layer->metatiles;
	x0 = FIELD_METATILE(layer->ofsX + map->minX);
	z0 = FIELD_METATILE(layer->ofsY + map->minZ);
	width = FIELD_METATILE(map->maxX - map->minX);
	height = FIELD_METATILE(map->maxZ - map->minZ);
	metatiles +=
		FIELD_METATILE(map->minZ) * MAPGRID_SIZE + FIELD_METATILE(map->minX);
	for(j = 0; j < height; j++) {
		metatile = metatiles + j * MAPGRID_SIZE;
		for(i = 0; i < width; i++, metatile++) {
			event = metatile->collisionCode;
			if(event != 0 && event >= kind) {
				last = kind + 1;
				if(event <= last) {
					cx = i + x0;
					entry->x = cx;
					cz = j + z0;
					entry->z = cz;
					entry->flg = flag;
					flag++;
					if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
						if(event == last)
							fdScriptSetFlag(entry->flg);
					} else if(fdScriptGetFlag(entry->flg)) {
						fdMapGridCopyMetatiles(table->x, table->z + 1, 1, 1,
							cx | (Sint16)(table->x & 64),
							cz | (Sint16)(table->z & 64));
					}
					entry++;
					table->cnt++;
				}
			}
		}
	}
	if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		Sint32 leaderX = FIELD_METATILE(leader->x.fixed);
		Sint32 leaderZ = FIELD_METATILE(leader->z.fixed);

		entry = table->entries;
		for(k = 0; k < table->cnt; k++) {
			if(entry->x == leaderX && entry->z == leaderZ)
				table->cur = k;
			entry++;
		}
	}
	/* The hole mask fdCrackedTileUpdate draws into the OBJ window: a 16 by 32
	   sprite whose every pixel is colour 1. */
	buffer = syHeapAlloc(HOLE_MASK_BYTES);
	{
		Uint32 *word = buffer;

		for(k = 0; k < HOLE_MASK_BYTES / 4; k++)
			*word++ = PIXELS_4BPP(1);
	}
	table->tile = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(table->tile, HOLE_MASK_BYTES, buffer);
	syHeapRelease(buffer);
	sySetFrameEvent(fdCrackedTileUpdate, EVENT_PASS_FRAME);
	return table->cnt;
}

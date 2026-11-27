/* Finds the room's cracked tiles: every metatile whose script is `kind` (whole)
   or `kind + 1` (cracked), each given the next flag from `flg`; returns how
   many. */
Sint32 scCrackedTileFind(
	Uint32 kind, Sint32 flg, Sint32 flgByte, Sint32 x, Sint32 z)
{
	METATILE_TARGET		  *tbl = &CrackedTiles;
	METATILE_TARGET_ENTRY *entry = tbl->entries;
	MAP_STATE			  *map;
	MAP_LAYER			  *layer;
	MAP_METATILE		  *metatiles;
	MAP_METATILE		  *metatile;
	Sint32				   x0, z0, width, height, i, j;
	Uint32				  *buf;
	Uint32				   event, last;
	Sint32				   cx, cz;
	Sint32				   k;

	syWordsClearEntry(tbl, 4);
	tbl->kind = kind;
	tbl->firstFlg = flg;
	tbl->flgByte = flgByte;
	tbl->x = x;
	tbl->z = z;
	tbl->cur = -1;
	tbl->cnt = 0;
	map = RamHeapSlots->mapGrid;
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
					entry->flg = flg;
					flg++;
					if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
						if(event == last)
							fdScriptSetFlag(entry->flg);
					} else if(fdScriptGetFlag(entry->flg)) {
						fdMapGridCopyMetatiles(tbl->x, tbl->z + 1, 1, 1,
							cx | (Sint16)(tbl->x & 64),
							cz | (Sint16)(tbl->z & 64));
					}
					entry++;
					tbl->cnt++;
				}
			}
		}
	}
	if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		Sint32 leaderX = FIELD_METATILE(leader->x.fixed);
		Sint32 leaderZ = FIELD_METATILE(leader->z.fixed);

		entry = tbl->entries;
		for(k = 0; k < tbl->cnt; k++) {
			if(entry->x == leaderX && entry->z == leaderZ)
				tbl->cur = k;
			entry++;
		}
	}
	/* The hole mask fdCrackedTileUpdate draws into the OBJ window: a 16 by 32
	   sprite whose every pixel is colour 1. */
	buf = syHeapAlloc(HOLE_MASK_BYTES);
	{
		Uint32 *word = buf;

		for(k = 0; k < HOLE_MASK_BYTES / 4; k++)
			*word++ = PIXELS_4BPP(1);
	}
	tbl->tile = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(tbl->tile, HOLE_MASK_BYTES, buf);
	syHeapRelease(buf);
	sySetTickEvent(fdCrackedTileUpdate, EVENT_PASS_TICK);
	return tbl->cnt;
}

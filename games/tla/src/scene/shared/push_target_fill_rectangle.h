/* Makes the cells under a pushable actor solid, by its rectangle: its sprite */
/* picks one of the rectangles in PushRectangles, and the cells' own */
/* behaviors are kept in the map's spare area for when it moves. Returns 1, or
 */
/* 0 for a sprite that cannot be pushed. */
s32 scPushTargetFillRectangle(s32 id) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR *actor = fdActorGet(id);
	s16 shape = ((SPR *)actor->spr)->entries[0]->anmId;
	s16 i = 0;
	u32 j;
	s32 width;
	s32 height;
	s32 x;
	s32 z;
	s32 mapX;
	s32 mapZ;
	s32 toX;
	s32 toZ;

	if((u16)shape != PushKinds[(u16)i]) {
		do {
			/* FAKEMATCH(dead): the counter's halfword step written out by hand;
			 * as a u16 counter in for (i = 0; i < PUSH_KINDS; i++), j = i after
			 * it and toX from (u16)mapX, 52 lines off. */
			j = (u16)NEXT_HALFWORD(i);
			i = j;
		} while(j <= PUSH_KINDS - 1 && (u16)shape != PushKinds[j]);
	}
	j = (u16)i;
	if(j == PUSH_KINDS)
		return 0;
	width = (ABS(PushRectangles[j][0]) + ABS(PushRectangles[j][2])) >> 4;
	height = (ABS(PushRectangles[j][1]) + ABS(PushRectangles[j][3])) >> 4;
	x = FIELD_CELL(actor->x.fixed + toFIXED(PushRectangles[j][0]));
	z = FIELD_CELL(actor->z.fixed + toFIXED(PushRectangles[j][1]));
	mapX = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].ofsX);
	i = mapX;
	mapZ = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].ofsY);
	toX = x + (u16)i;
	toZ = z + (u16)mapZ;
	fdMapGridCopyCellAttributes(x, z, width, height, toX, toZ);
	fdMapGridFillCollisionRectangle(MAPGRID_LAYER_GROUND, x, z, width, height,
									CELL_BEHAVIOR_WALL);
	fdMapGridFillCollisionRectangle(MAPGRID_LAYER_OVERHEAD, x, z, width, height,
									CELL_BEHAVIOR_WALL);
	return 1;
}

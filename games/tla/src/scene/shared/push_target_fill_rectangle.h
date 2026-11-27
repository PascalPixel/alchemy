/* Makes the metatiles under a pushable actor solid, by its rectangle: its
   sprite picks one of the rectangles in PushRectangles, and the metatiles' own
   behaviors are kept in the map's spare area for when it moves. Returns 1, or 0
   for a sprite that cannot be pushed. */
Bool scPushTargetFillRectangle(Sint32 id)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	ACTOR	  *actor = fdActorGet(id);
	Sint16	   shape = ((SPR *)actor->spr)->entries[0]->anmId;
	Sint16	   i = 0;
	Uint32	   j;
	Sint32	   width;
	Sint32	   height;
	Sint32	   x;
	Sint32	   z;
	Sint32	   mapX;
	Sint32	   mapZ;
	Sint32	   toX;
	Sint32	   toZ;

	if((Uint16)shape != PushKinds[(Uint16)i]) {
		do {
			/* FAKEMATCH(dead): the counter's halfword step written out by hand;
			 * as a Uint16 counter in for (i = 0; i < PUSH_KINDS; i++), j = i
			 * after it and toX from (Uint16)mapX, 52 lines off. */
			j = (Uint16)NEXT_HALFWORD(i);
			i = j;
		} while(j <= PUSH_KINDS - 1 && (Uint16)shape != PushKinds[j]);
	}
	j = (Uint16)i;
	if(j == PUSH_KINDS)
		return FALSE;
	width = (ABS(PushRectangles[j][0]) + ABS(PushRectangles[j][2])) >> 4;
	height = (ABS(PushRectangles[j][1]) + ABS(PushRectangles[j][3])) >> 4;
	x = FIELD_METATILE(actor->x.fixed + toFIXED(PushRectangles[j][0]));
	z = FIELD_METATILE(actor->z.fixed + toFIXED(PushRectangles[j][1]));
	mapX = FIELD_METATILE(map->layers[MAPGRID_LAYER_MIDDLE].ofsX);
	i = mapX;
	mapZ = FIELD_METATILE(map->layers[MAPGRID_LAYER_MIDDLE].ofsY);
	toX = x + (Uint16)i;
	toZ = z + (Uint16)mapZ;
	fdMapGridCopyMetatileAttributes(x, z, width, height, toX, toZ);
	fdMapGridFillCollisionRectangle(
		MAPGRID_LAYER_GROUND, x, z, width, height, METATILE_BEHAVIOR_WALL);
	fdMapGridFillCollisionRectangle(
		MAPGRID_LAYER_OVERHEAD, x, z, width, height, METATILE_BEHAVIOR_WALL);
	return TRUE;
}

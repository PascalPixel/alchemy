/* Sets the shape of a metatile to the height given. */
Sint32 scMagmaRockInsideSetMetatileShape(
	Sint32 layer, Sint32 x, Sint32 z, Sint32 height)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   shape = fdMapGridGetTerrainKind(layer, x, z);
	Uint8	  *out = Ram_MapShapeGrid +
		(map->layers[layer].metatiles - (MAP_METATILE *)Ram_MapMetatileBuffer);

	out += x + z * MAPGRID_SIZE;
	*out = fdMapShapeFindOrAdd(layer, shape, height);
	return 0;
}

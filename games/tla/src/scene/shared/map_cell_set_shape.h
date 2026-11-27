/* Sets the shape of a cell to the height given. */
s32 scMagmaRockInsideSetCellShape(s32 layer, s32 x, s32 z, s32 height) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 shape = fdMapGridGetTerrainKind(layer, x, z);
	u8 *out = Ram_MapShapeGrid +
			  (map->layers[layer].cells - (MAP_CELL *)Ram_MapCellBuffer);

	out += x + z * MAPGRID_SIZE;
	*out = fdMapShapeFindOrAdd(layer, shape, height);
	return 0;
}

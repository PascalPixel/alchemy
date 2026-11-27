/* @file */
/* The pushed-actor scenes (pushed_actor.c) and Venus Lighthouse's unused copy:
 */
/* a rectangle of cells given one cell behavior. */

/* Sets the cell behavior of a width by height rectangle of cells, from */
/* cell (x, z), to value, on map grid layer 0 to 2, or (3 and up) in */
/* MapGridCellBuffer. */
static s32 scMapGridFillCollisionRectangle(u32 layer, s32 x, s32 z, u32 width,
										   u32 height, s32 value) {
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	MAP_GRID_CELL *cells;
	u32 row;
	u32 column;

	if(grid != NULL) {
		if(layer <= 2) {
			cells = grid->layers[layer].cells;
		}
		else {
			cells = (MAP_GRID_CELL *)MapGridCellBuffer;
		}
		cells += x + z * MAPGRID_SIZE;
		for(row = 0; row < height; row++) {
			MAP_GRID_CELL *cell = cells + row * MAPGRID_SIZE;

			for(column = 0; column < width; column++) {
				cell->behavior = value;
				cell++;
			}
		}
	}
	return 0;
}

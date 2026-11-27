/* The pushed-actor scenes (pushed_actor.c) and Venus Lighthouse's unused copy:
   a rectangle of metatiles given one metatile behavior. */

/* Sets the metatile behavior of a width by height rectangle of metatiles, from
   metatile (x, z), to value, on map grid layer 0 to 2, or (3 and up) in
   MapGridMetatileBuffer. */
static Sint32 scMapGridFillCollisionRectangle(
	Uint32 layer, Sint32 x, Sint32 z, Uint32 width, Uint32 height, Sint32 value)
{
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	MAP_GRID_METATILE	  *metatiles;
	Uint32				   row;
	Uint32				   column;

	if(grid != NULL) {
		if(layer <= 2) {
			metatiles = grid->layers[layer].metatiles;
		} else {
			metatiles = (MAP_GRID_METATILE *)MapGridMetatileBuffer;
		}
		metatiles += x + z * MAPGRID_SIZE;
		for(row = 0; row < height; row++) {
			MAP_GRID_METATILE *metatile = metatiles + row * MAPGRID_SIZE;

			for(column = 0; column < width; column++) {
				metatile->behavior = value;
				metatile++;
			}
		}
	}
	return 0;
}

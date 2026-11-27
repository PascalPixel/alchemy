/* fdMapGridPlayMapEdit, the same in both games. Both games'
   field/field_map_grid.c include it where the function stands. */

/* Plays a map edit (MAP_GRID_METATILE_ANIMATION_STEP entries, ended by a source
   of MAPGRID_ANIMATION_STEPS_END): copies each entry's rectangle of metatile
   picture numbers to (dstX, dstY) and waits its delay in
   ticks. */
void fdMapGridPlayMapEdit(Uint16 *steps, Sint32 dstX, Uint32 dstY)
{
	/* Each entry is read in turn: its source, then its signed y, width, height
	   and delay (MAP_GRID_METATILE_ANIMATION_STEP). */
	while(*steps != MAPGRID_ANIMATION_STEPS_END) {
		Uint16 src = *steps++;
		Sint32 srcY = (Sint16)*steps++;
		Sint32 width = (Sint16)*steps++;
		Sint32 height = (Sint16)*steps++;
		Sint32 delay = (Sint16)*steps++;

		fdMapGridCopyMetatileTilesIndicesRect(
			src, (Uint16)srcY, dstX, dstY, (Uint16)width, (Uint16)height);
		sySynch((Uint16)delay);
	}
}

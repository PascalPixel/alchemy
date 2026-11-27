/* Crossbone Isle Cave (SceneId_CrossboneIsleSteppingStones to
   SceneId_CrossboneIsleDragonStatue) and Kolima Forest
   (SceneId_KolimaForestFloodgate): drawing a stretch of the layout, in one of
   the states kept elsewhere in it (such as the water raised or not), straight
   into the background's tile map. */

#include "system.h"

/* Metatiles across and down a screen block: 32 tiles square, two tiles a
   metatile. */
#define SCREEN_BLOCK_METATILES (BG_SCREEN_WIDTH / 2)

/* Draws the width by height metatiles from metatile (x, y) of the layout into
   screen block 5 at metatile (destinationX, destinationY), wrapping every
   SCREEN_BLOCK_METATILES metatiles. The block is cut into horizontal planes of
   SCREEN_BLOCK_METATILES metatile rows; plane picks which one is drawn into. */
static void scMapGridDrawMetatilePanel(Sint32 x, Sint32 y, Sint32 width,
	Sint32 height, Sint32 plane, Sint32 destinationX, Sint32 destinationY)
{
	Uint32 *metatile =
		(Uint32 *)Ram_MapGridMetatileBuffer + (y * MAPGRID_SIZE + x);
	Sint32 row;
	Sint32 col;

	for(row = destinationY; row < destinationY + height; row++) {
		for(col = destinationX; col < destinationX + width; col++) {
			/* A metatile's tiles are four tile map entries, two rows of two, 8
			   bytes; a metatile row of the screen block is two rows of
			   BG_SCREEN_WIDTH entries, BG_SCREEN_WIDTH words. */
			Uint32 tile = *metatile++ & MAPGRID_METATILE_TILES_ID_MASK;
			Sint32 index = (plane * SCREEN_BLOCK_METATILES +
							   (row & (SCREEN_BLOCK_METATILES - 1))) *
					BG_SCREEN_WIDTH +
				(col & (SCREEN_BLOCK_METATILES - 1));
			Uint32 *from;
			Uint32 *to;
			Uint32	picture;

			to = (Uint32 *)BG_SCREEN_BLOCK(5) + index;
			picture = tile * 2;
			from = (Uint32 *)Ram_MetatileTiles;
			from += picture;
			*to = *from;
			from = (Uint32 *)(Ram_MetatileTiles + 4);
			from += picture;
			to += BG_SCREEN_WIDTH / 2;
			*to = *from;
		}
		metatile += MAPGRID_SIZE - width;
	}
}

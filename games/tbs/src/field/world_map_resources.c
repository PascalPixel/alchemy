/* World map graphics: a palette and five tilesets for each of its two looks,
   the second the snowfield's (fdWorldMapLoadGraphics). */

#include "resource_ids.h"
#include "field/field.h"

/* Each set is a palette and five tilesets: fdWorldMapLoadGraphics decodes the
   palette into BG palettes 0 to 13, the next four into the two halves of char
   blocks 2 and 3, and the last into MapGridAnimTiles, where the tileset
   animations copy their pictures from. */
Uint32 WorldMap_GraphicsResources[2][6] = {
	{
		(Uint32)RESOURCE_WORLD_MAP_PALETTE_A,
		(Uint32)RESOURCE_WORLD_MAP_TILES_A,
		(Uint32)RESOURCE_WORLD_MAP_TILES_B,
		(Uint32)RESOURCE_WORLD_MAP_TILES_C,
		(Uint32)RESOURCE_WORLD_MAP_TILES_D,
		(Uint32)RESOURCE_WORLD_MAP_TILES_E,
	},
	{
		(Uint32)RESOURCE_WORLD_MAP_PALETTE_B,
		(Uint32)RESOURCE_WORLD_MAP_TILES_F,
		(Uint32)RESOURCE_WORLD_MAP_TILES_G,
		(Uint32)RESOURCE_WORLD_MAP_TILES_H,
		(Uint32)RESOURCE_WORLD_MAP_TILES_I,
		(Uint32)RESOURCE_WORLD_MAP_TILES_J,
	},
};

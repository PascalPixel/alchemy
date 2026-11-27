/* @file */
/* The hole mask over the cracked tile the leader stands on, shared by the */
/* Jupiter and Mars Lighthouse rooms and the Anemos Inner Sanctum, each of */
/* which lists the cells its map watches in its own CrackedTiles. While he */
/* drops through the tile (scCrackedTileFall), the mask is drawn into the OBJ */
/* window, where no sprite shows, so that he sinks out of sight through the */
/* hole. The including file names its overlay's import of syOamAddSprAtDepth */
/* (CELL_PUSH_SLOT_ENTRY). */

#include "field/field_sequence_functions.h"
#include "field/actor.h"
#include "field/map_grid.h"
#include "field/cell_target.h"
#include "field/field_sprite.h"

/* The mask, HOLE_MASK_WIDTH by 32, hangs from HOLE_MASK_TOP pixels down its */
/* cell. */
#define HOLE_MASK_WIDTH 16
#define HOLE_MASK_TOP	14

/* The cracked tiles the including map watches, in its own table. */
extern CELL_TARGET CrackedTiles;

s32 fdMapGetTerrainHeight(s32 layer, s32 x, s32 z);
void CELL_PUSH_SLOT_ENTRY(void *entry, s32 priority);

/* Clears the current entry once the leader has left its cell; while the */
/* leader stays on it and the mask is shown, draws the mask over the cell at */
/* the height of the ground there, unless it is off screen. */
void fdCrackedTileUpdate(void) {
	CELL_TARGET *table = &CrackedTiles;
	CELL_TARGET_ENTRY *entry;
	ACTOR *leader;
	MAP_STATE *map;
	s32 viewX, viewZ, viewY, tile;
	s32 screenX, height, z, screenY, depth;

	if(table->cur == -1)
		return;
	entry = &table->entries[table->cur];
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if((FIELD_CELL(leader->x.fixed)) != entry->x ||
	   (FIELD_CELL(leader->z.fixed)) != entry->z)
		table->cur = -1;
	{
		s16 current = table->cur;

		if(current == -1)
			return;
	}
	if(table->shown == 0)
		return;
	map = Ram_HeapSlots->mapGrid;
	viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	viewY = map->origin[1];
	tile = VramCacheEntries[table->tile].ofs >> 5;
	screenX = FIELD_CELL_ORIGIN(entry->x) - viewX;
	height = fdMapGetTerrainHeight(0, FIELD_CELL_ORIGIN(entry->x),
								   FIELD_CELL_ORIGIN(entry->z)) -
			 viewY;
	z = FIELD_CELL_ORIGIN(entry->z) - viewZ - viewY;
	screenY = z - height;
	screenX = FIXEDtoINT(screenX);
	height += z;
	height = FIXEDtoINT(height);
	screenY = FIXEDtoINT(screenY);
	screenY += HOLE_MASK_TOP;
	depth = height + SCENE_SPRITE_DEPTH;
	/* On screen, or within the mask's size of it: x from -15 to 319, y from */
	/* -15 to 239. */
	if((u32)(screenX + HOLE_MASK_WIDTH - 1) > 334 || screenY < -15 ||
	   screenY > 239)
		return;
	screenX &= OBJ_X_MASK;
	screenY &= OBJ_Y_MASK;
	table->oam[0] = 0;
	table->oam[1] =
		screenY | OBJ_X(screenX) | OBJ_SIZE_16X32 | OBJ_MODE(OBJ_MODE_WINDOW);
	table->oam[2] = tile | OBJ_PRIORITY(1);
	CELL_PUSH_SLOT_ENTRY(table->oam, depth);
}

#undef HOLE_MASK_WIDTH
#undef HOLE_MASK_TOP

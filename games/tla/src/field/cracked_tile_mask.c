/* The hole mask over the cracked tile the leader stands on, shared by the
   Jupiter and Mars Lighthouse rooms and the Anemos Inner Sanctum, each of which
   lists the metatiles its map watches in its own CrackedTiles. While he drops
   through the tile (scCrackedTileFall), the mask is drawn into the OBJ window,
   where no sprite shows, so that he sinks out of sight through the hole. The
   including file names its overlay's import of syOAMAddSprAtDepth
   (METATILE_PUSH_SLOT_ENTRY). */

#include "field/field_mode_functions.h"
#include "field/actor.h"
#include "field/map_grid.h"
#include "field/metatile_target.h"
#include "field/field_sprite.h"

/* The mask, HOLE_MASK_WIDTH by 32, hangs from HOLE_MASK_TOP pixels down its
   metatile. */
#define HOLE_MASK_WIDTH 16
#define HOLE_MASK_TOP	14

/* The cracked tiles the including map watches, in its own table. */
extern METATILE_TARGET CrackedTiles;

Sint32 fdMapGetTerrainHeight(Sint32 layer, Sint32 x, Sint32 z);
void   METATILE_PUSH_SLOT_ENTRY(void *entry, Sint32 priority);

/* Clears the current entry once the leader has left its metatile; while the
   leader stays on it and the mask is shown, draws the mask over the metatile at
   the height of the ground there, unless it is off screen. */
void fdCrackedTileUpdate(void)
{
	METATILE_TARGET		  *tbl = &CrackedTiles;
	METATILE_TARGET_ENTRY *entry;
	ACTOR				  *leader;
	MAP_STATE			  *map;
	Sint32				   viewX, viewZ, viewY, tile;
	Sint32				   screenX, height, z, screenY, depth;

	if(tbl->cur == -1)
		return;
	entry = &tbl->entries[tbl->cur];
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if((FIELD_METATILE(leader->x.fixed)) != entry->x ||
		(FIELD_METATILE(leader->z.fixed)) != entry->z)
		tbl->cur = -1;
	{
		Sint16 cur = tbl->cur;

		if(cur == -1)
			return;
	}
	if(tbl->shown == 0)
		return;
	map = Ram_HeapSlots->mapGrid;
	viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	viewY = map->origin[1];
	tile = VramCacheEntries[tbl->tile].ofs >> 5;
	screenX = FIELD_METATILE_ORIGIN(entry->x) - viewX;
	height = fdMapGetTerrainHeight(0, FIELD_METATILE_ORIGIN(entry->x),
				 FIELD_METATILE_ORIGIN(entry->z)) -
		viewY;
	z = FIELD_METATILE_ORIGIN(entry->z) - viewZ - viewY;
	screenY = z - height;
	screenX = FIXEDtoINT(screenX);
	height += z;
	height = FIXEDtoINT(height);
	screenY = FIXEDtoINT(screenY);
	screenY += HOLE_MASK_TOP;
	depth = height + SCENE_SPRITE_DEPTH;
	/* On screen, or within the mask's size of it: x from -15 to 319, y from -15
	   to 239. */
	if((Uint32)(screenX + HOLE_MASK_WIDTH - 1) > 334 || screenY < -15 ||
		screenY > 239)
		return;
	screenX &= OBJ_X_MASK;
	screenY &= OBJ_Y_MASK;
	tbl->oam[0] = 0;
	tbl->oam[1] =
		screenY | OBJ_X(screenX) | OBJ_SIZE_16X32 | OBJ_MODE(OBJ_MODE_WINDOW);
	tbl->oam[2] = tile | OBJ_PRIORITY(1);
	METATILE_PUSH_SLOT_ENTRY(tbl->oam, depth);
}

#undef HOLE_MASK_WIDTH
#undef HOLE_MASK_TOP

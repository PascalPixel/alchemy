/* The ground mask: raised ground in front of an actor hides the part of it
   behind, as a ledge in the Kalay Tunnel's treasure room hides Isaac's legs.
   Part of field_mode.c. */

#include "field_mode_internal.h"

/* A screen row's map metatile, in pixels, in an OAM entry's 8-bit y, as
   MAPGRID_METATILE_PIXEL_MASK takes a whole pixel coordinate's. */
#define METATILE_GRID_ROW_MASK 0xf0

/* Each tick of the ground mask: on each of the two metatiles beside the actor
   (left and right of it), where the ground one or two metatiles south rises
   above the actor's floor, draws a semi-transparent 16-pixel-wide sprite of
   colour 1 at the height of that ground, which hides what of the actor is below
   that ground's edge. */
void fdGroundMaskDrawMarks(void)
{
	GROUND_MASK_STATE	  *state = HEAP_SLOT_BLOCK(HEAP_SLOT_GROUND_MASK);
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	Sint32 cameraX = (Sint16)FIXEDtoINT(syMapGridScrollStateView(grid)[0]);
	Sint32 cameraZ = (Sint16)FIXEDtoINT(syMapGridScrollStateView(grid)[1]);
	ANIMATION_SPR_PART *mark = state->marks;
	ACTOR			   *actor = state->actor;
	FIXED				x;
	FIXED				z;
	Sint32				terrainFloor;
	Sint32				terrainLayer;
	Uint32				tile;
	Sint32				height;
	Sint32				secondHeight;
	Sint32				screenRow;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint16 unused[2];

	if(actor == 0)
		return;
	x = actor->x.fixed;
	z = actor->z.fixed;
	terrainFloor = FIXEDtoINT(actor->groundHeight);
	terrainLayer = actor->layer;
	/* The first tile of the solid block fdEffectStartGroundMask loaded. */
	tile = VramCacheEntries[VRAM_CACHE_ENTRY_GROUND_MASK].ofs / TILE_SIZE_4BPP;
	/* The left metatile, 8 pixels left of the actor. */
	x -= toFIXED(8);
	height = FIXEDtoINT(
		fdMapGridGetTerrainHeightStub(terrainLayer, x, z + toFIXED(16)));
	secondHeight = FIXEDtoINT(fdMapGridGetTerrainHeightStub(
					   terrainLayer, x, z + toFIXED(32))) -
		16;
	if(secondHeight > height)
		height = secondHeight;
	if(height > 0 && height > terrainFloor) {
		/* The sprite mode and priority set next replace what the two words put
		   there, so the left and right marks end up alike although their third
		   words differ. */
		((Uint32 *)mark)[1] = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_WINDOW);
		((Uint32 *)mark)[2] = OBJ_PRIORITY(1);
		mark->pri = 0;
		mark->tile = tile;
		mark->objMode = OBJ_MODE_SEMI_TRANSPARENT;
		/* Snapped to the metatile's grid, then moved up by the ground's
		   height. */
		mark->x =
			((Uint32)FIXEDtoINT(x) & MAPGRID_METATILE_PIXEL_MASK) - cameraX;
		screenRow = FIXEDtoINT(z) & METATILE_GRID_ROW_MASK;
		mark->y = screenRow - cameraZ - height + 16;
		syOAMAddSprAtDepth(mark, 0);
	}
	/* The right metatile, 8 pixels right of the actor, read the same way. */
	x += toFIXED(16);
	height = FIXEDtoINT(
		fdMapGridGetTerrainHeightStub(terrainLayer, x, z + toFIXED(16)));
	mark = &state->marks[1];
	secondHeight = FIXEDtoINT(fdMapGridGetTerrainHeightStub(
					   terrainLayer, x, z + toFIXED(32))) -
		16;
	if(secondHeight > height)
		height = secondHeight;
	if(height > 0 && height > terrainFloor) {
		((Uint32 *)mark)[1] = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_WINDOW);
		((Uint32 *)mark)[2] = 0;
		mark->pri = 0;
		mark->tile = tile;
		mark->objMode = OBJ_MODE_SEMI_TRANSPARENT;
		mark->x =
			((Uint32)FIXEDtoINT(x) & MAPGRID_METATILE_PIXEL_MASK) - cameraX;
		screenRow = FIXEDtoINT(z) & METATILE_GRID_ROW_MASK;
		mark->y = screenRow - cameraZ - height + 16;
		syOAMAddSprAtDepth(mark, 0);
	}
}

/* Starts the ground mask on object (the party leader when NULL): loads its
   solid tiles, sets the blending and draws it each tick. Kalay's tunnel,
   Mercury and Venus Lighthouse, the Fuchin Falls cave, the Tunnel Ruins and
   Crossbone Isle start it. */
void fdEffectStartGroundMask(void *object)
{
	GROUND_MASK_STATE *state;
	Uint8			  *tiles;
	volatile Uint32	   fill;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_GROUND_MASK, sizeof(*state));
	/* Upload the solid sprite tiles before enabling the drawing event. */
	tiles = syHeapSlotAlloc(HEAP_SLOT_TILE_SCRATCH, 1024);
	DMA_FILL32_CELL(fill, PIXELS_4BPP(1), tiles, 512);
	syVRAMCacheLoad(VRAM_CACHE_ENTRY_GROUND_MASK, 512, tiles);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	sySetEvent(fdGroundMaskDrawMarks, EVENT_PASS_TICK);
	IO_WRITE16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
			BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
		&REG_BLDCNT);
	IO_WRITE16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);
	/* 31 is above the register's useful maximum of 16, which it acts as. */
	IO_WRITE16(31, &REG_BLDY);
	DMA_FILL32_CELL(fill, 0, state, sizeof(*state));
	if(object == NULL)
		object = fdActorLookup(MainRoutineStateWork.leaderActorId);
	state->actor = object;
}

/* Stops drawing the ground mask, keeping its state. */
void fdGroundMaskSuspend(void)
{
	sySuspendEvent(fdGroundMaskDrawMarks);
}

/* Draws the ground mask again after fdGroundMaskSuspend. */
void fdGroundMaskResume(void)
{
	syResumeEvent(fdGroundMaskDrawMarks);
}

#undef METATILE_GRID_ROW_MASK

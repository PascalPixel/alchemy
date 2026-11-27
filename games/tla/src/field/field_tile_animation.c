/* Timed map changes (the map animations, the blend script and the palette
   queue), then the ground heights and the collision of metatiles and the world
   map, as The Broken Seal's field_tile_animation.c and field_terrain.c have
   them. Part of field.c. */

#include "graphics/palette_cycles.h"
#include "field/curve.h"

/* A tileset animation script's commands, a halfword each where a source tile
   would stand (fdTilesetAnimUpdate and fdTilesetAnimStartAll). */
#define TILESET_ANIM_LOOP		 0xffff /* back to the start of the script */
#define TILESET_ANIM_OPCODE_MASK 0xff00 /* the high byte: the command */
#define TILESET_ANIM_JUMP		 0xfe00 /* low byte: the halfword pair to jump to */
#define TILESET_ANIM_STOP		 0xff /* TILESET_ANIM_JUMP's low byte to stop */
#define TILESET_ANIM_ARGUMENT_MASK \
	0xff /* the low byte: the command's argument */
/* A layout with wide tiles animates 64-byte (256-colour) tiles in char block 2;
   others animate 16-colour ones in char block 1. */
#define TILESET_ANIM_WIDE_TILE_BYTES	   (TILE_SIZE_4BPP * 2)
/* low four bits: the animation it starts */
#define TILESET_ANIM_MARKER				   0xfd00
#define TILESET_ANIM_MARKER_PAUSED		   0x80 /* a marker's bit: start it paused */
#define TILESET_ANIM_MARKER_ANIMATION_MASK 0xf /* a marker's animation */
/* The first source tile read from MapGridAnimTiles rather than VRAM, for
   32-byte and for 64-byte (wide) tiles: both bases put it at its start. */
#define TILESET_ANIM_RAM_TILE			   1536
#define TILESET_ANIM_RAM_WIDE_TILE		   512

/* The blend script's other commands (fdBlendScriptRun; BLEND_SCRIPT_LOOP and
   BLEND_SCRIPT_JUMP are map_grid.h's): as the tileset animations', and a BLDCNT
   value, told apart by its top four bits. */
#define BLEND_SCRIPT_OPCODE_MASK   0xff00
#define BLEND_SCRIPT_STOP		   0xff
#define BLEND_SCRIPT_ARGUMENT_MASK 0xff
#define BLEND_SCRIPT_BLDCNT_MASK   0xf000
#define BLEND_SCRIPT_BLDCNT		   (BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD)

/* A world map collision tile: 4 by 4 values of 4 bits, each for 4 by 4 pixels,
   in 8 bytes. The collision buffer holds the 256 tiles of the lower layer's,
   then the upper layer's. */
#define WORLD_MAP_COLLISION_TILE_BYTES 8
#define WORLD_MAP_COLLISION_TILES	   256

/* The last collision the flying ship crosses once it is up
   (fdWorldMapIsFlyable): past it, the forest and the ground after it stop it,
   all but 10. */
#define WORLD_MAP_COLLISION_FLYABLE_LAST 8

/* The ticks a world map graphics set takes to reload, one step a tick,
   counted down from this: its palette, animations and shapes, then each tile
   set decoded and copied to VRAM. */
#define WORLD_MAP_RELOAD_STEPS 12

/* Loads the first of the 2x2 world map tiles around the player that is not
   shown yet (at most one a tick), without drawing it. When the region under
   the party picks another graphics set, reloads that set over the next
   WORLD_MAP_RELOAD_STEPS ticks, unless a screen transition is running. */
void fdWorldMapPreloadCurrentTileBlock(void)
{
	WORLD_MAP_VIEW	   *perspective = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;
	Sint32				x = 0;
	Sint32				y = 0;
	Sint32				firstQuarterX;
	Sint32				firstQuarterY;
	Sint32				layer;
	Sint32				row;
	Sint32				col;
	Sint32				tile;
	WORLD_MAP_METATILE *block;
	const Uint32	   *resources;
	Sint32				value;
	Uint8			   *buf;

	if(perspective->origin != NULL) {
		Sint32 *p = perspective->origin;

		x = *p++;
		y = p[1];
	}
	firstQuarterX = WORLD_MAP_TILE(x - toFIXED(256));
	firstQuarterY = WORLD_MAP_TILE(y - toFIXED(320));
	for(layer = 0; layer <= 1; layer++) {
		for(row = 0; row <= 1; row++) {
			for(col = 0; col <= 1; col++) {
				tile = (((firstQuarterY + row) & (WORLD_MAP_TILES_ACROSS - 1))
						   << 5) +
					((firstQuarterX + col) & (WORLD_MAP_TILES_ACROSS - 1));
				tile = perspective->tileIds[tile];
				tile += layer * WORLD_MAP_LAYER_TILES;
				if(fdWorldMapWriteLayerMetatileTile(layer, firstQuarterX + col,
					   firstQuarterY + row, tile, 0) != 0)
					return;
			}
		}
	}
	block = (WORLD_MAP_METATILE *)syRamMapBlocks +
		(((x / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
			 (WORLD_MAP_VIEW_METATILES - 1)) +
			(((y / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
				 (WORLD_MAP_VIEW_METATILES - 1)) *
				WORLD_MAP_VIEW_METATILES));
	fdWorldMapUpdateRegionTint(block->region, 1);
	value = block->region;
	resources = WorldMapGraphicsResources[fdWorldMapGetRegionGroupGraphics(
		value / WORLD_MAP_REGION_GROUP)];
	if(resources != perspective->resources) {
		perspective->resources = resources;
		perspective->reloadSteps = WORLD_MAP_RELOAD_STEPS;
	}
	value = fdScreenTransitionIsScanlineActiveStub();
	if(perspective->reloadSteps == 0)
		return;
	if(value != 0)
		return;
	buf = syHeapAlloc(BG_PLTT_SIZE);
	switch(perspective->reloadSteps) {
	case WORLD_MAP_RELOAD_STEPS: {
		Sint16 backdropColor = *(Sint16 *)BG_PLTT;

		syDecompressByteLZ(
			syResourceGetTableEntry(resources[WORLD_MAP_SET_PALETTE]), buf);
		*(Sint16 *)buf = backdropColor;
		syDMAXCopy(buf, (void *)BG_PLTT,
			DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2), REG_DMA3);
		{
			Uint8 *anim = MAPGRID_ANIMATION_SCRIPT;
			fdPaletteFadeLoadPaletteStub(buf);
			syDecompressTaggedLZ(
				syResourceGetTableEntry(resources[WORLD_MAP_SET_ANIMATION]),
				anim);
			fdTilesetAnimStartAll((const Uint16 *)anim);
		}
		syDecompressTaggedLZ(syResourceGetTableEntry(
								 resources[WORLD_MAP_SET_METATILE_ATTRIBUTES]),
			MapGridShapes);
		break;
	}
	case 11:
		syDecompressTaggedLZ(
			syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_1]),
			BgTileBuffer);
		break;
	case 10:
		syDMAXCopy(BgTileBuffer, (void *)BG_CHAR_BLOCK(2),
			DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
		break;
	case 9:
		syDecompressTaggedLZ(
			syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_2]),
			WORLD_MAP_TILE_SET(1));
		break;
	case 8:
		syDMAXCopy(WORLD_MAP_TILE_SET(1),
			(void *)(BG_CHAR_BLOCK(2) + WORLD_MAP_TILE_SET_BYTES),
			DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
		break;
	case 7:
		syDecompressTaggedLZ(
			syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_3]),
			WORLD_MAP_TILE_SET(2));
		break;
	case 6:
		syDMAXCopy(WORLD_MAP_TILE_SET(2), (void *)BG_CHAR_BLOCK(3),
			DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
		break;
	case 5:
		syDecompressTaggedLZ(
			syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_4]),
			WORLD_MAP_TILE_SET(3));
		break;
	case 4:
		syDMAXCopy(WORLD_MAP_TILE_SET(3),
			(void *)(BG_CHAR_BLOCK(3) + WORLD_MAP_TILE_SET_BYTES),
			DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
		break;
	case 3:
		syDecompressTaggedLZ(
			syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_5]),
			MapGridAnimTiles);
		break;
	case 1:
		syDecompressTaggedLZ(
			syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_6]),
			&WorldMapStateWork);
		break;
	}
	perspective->reloadSteps--;
	syHeapRelease(buf);
}

/* VBlank: shows BG1 and copies the decoded picture's tiles into VRAM. */
void fdWorldMapPresentStillImage(void)
{
	syIOWrite16(BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(8) |
			BGCNT_WRAP | BGCNT_SIZE(2),
		&REG_BG3CNT);
	syIOWrite16(BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(10) |
			BGCNT_WRAP | BGCNT_SIZE(2),
		&REG_BG2CNT);
	syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6),
		&REG_BG1CNT);
	syDMAXCopy(RamMapMetatileBuffer,
		BG_CHAR_BLOCK(0) + STILL_IMAGE_FIRST_TILE * TILE_SIZE_8BPP,
		DMA_COPY_WORDS(STILL_IMAGE_TILES_ACROSS * STILL_IMAGE_TILE_ROWS *
			TILE_SIZE_8BPP / 4),
		REG_DMA3);
}

/* Freezes the world map's perspective view as a still image on BG1: decodes the
   area's tiles again where a field Psynergy used their buffer, saves BG1's
   tiles, draws the current affine frame into pixels and hands the window and
   VBlank to it. */
void fdWorldMapStartStillImage(void)
{
	WORLD_MAP_AFFINE_STATE *affineState = RamHeapSlots->worldMapAffine;
	WORLD_MAP_VIEW *perspective = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;
	const Uint32   *resources;

	perspective->anmActive = 1;
	sySuspendEvent(fdTilesetAnimUpdate);
	perspective->viewHeld = 1;
	resources = perspective->resources;
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_1]),
		BgTileBuffer);
	if(perspective->tileBufUsed != 0)
		syDecompressTaggedLZ(
			syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_2]),
			WORLD_MAP_TILE_SET(1));
	perspective->tileBufUsed = 0;
	syDMAXCopy((void *)BG_CHAR_BLOCK(1), DecodeBuffer,
		DMA_COPY_WORDS(WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
	sySynch(1);
	fdTileMapDrawRowsFromRam(
		affineState->pages[TickCount & 1], RamMapMetatileBuffer);
	perspective->winTop = STILL_IMAGE_WINDOW_TOP;
	perspective->winBottom = STILL_IMAGE_WINDOW_BOTTOM;
	VBlankCallback = fdWorldMapPresentStillImage;
	perspective->stillImageShown = 1;
}

/* VBlank: shows BG1 and copies the area's tiles back into VRAM. */
void fdWorldMapEndStillImage(void)
{
	syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(8) |
			BGCNT_WRAP | BGCNT_SIZE(2),
		&REG_BG3CNT);
	syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(10) |
			BGCNT_WRAP | BGCNT_SIZE(2),
		&REG_BG2CNT);
	syIOWrite16(BGCNT_PRIORITY(1) | BGCNT_SCREEN_BASE(5), &REG_BG1CNT);
	syDMAXCopy(BgTileBuffer, (void *)BG_CHAR_BLOCK(2),
		DMA_COPY_WORDS(4 * WORLD_MAP_TILE_SET_BYTES / 4), REG_DMA3);
}

/* Reloads the world map area's palette and tiles after the still image, puts
   the default metatiles back and returns to the perspective view. */
void fdWorldMapLoadAreaGraphics(void)
{
	WORLD_MAP_VIEW *perspective;
	const Uint32   *resources;
	Uint8		   *buf;
	Sint16			backdropCol;

	perspective = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;
	buf = RamMapMetatileBuffer;
	resources = perspective->resources;
	backdropCol = *(Sint16 *)BG_PLTT;
	syDecompressByteLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_PALETTE]), buf);
	*(Sint16 *)buf = backdropCol;
	syDMAXCopy(buf, (void *)BG_PLTT, DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2),
		REG_DMA3);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_1]),
		BgTileBuffer);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_2]),
		WORLD_MAP_TILE_SET(1));
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_3]),
		WORLD_MAP_TILE_SET(2));
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_4]),
		WORLD_MAP_TILE_SET(3));
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_5]),
		MapGridAnimTiles);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(resources[WORLD_MAP_SET_TILES_6]),
		&WorldMapStateWork);
	/* The VBlank copies the tiles back and the next tick shows the ground;
	   only then is the still image's buffer refilled with the metatiles. */
	VBlankCallback = fdWorldMapEndStillImage;
	perspective->winTop = 0;
	perspective->winBottom = DISPLAY_HEIGHT - 1;
	sySynch(1);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_DEFAULT_MAP_METATILES), buf);
	fdWorldMapUpdateCurrentTileBlock();
	perspective->anmActive = 0;
	syResumeEvent(fdTilesetAnimUpdate);
	perspective->viewHeld = 0;
	perspective->stillImageShown = 0;
	fdWorldMapUpdateRegionTint(perspective->tintRegion, 0);
	sySynch(1);
}

/* Puts the default world map metatiles back after the still image and returns
   to the perspective view, without reloading the area's palette or tiles. */
void fdWorldMapLoadDefaultMetatilesAndUpdateBlock(void)
{
	WORLD_MAP_VIEW *perspective = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;

	VBlankCallback = fdWorldMapEndStillImage;
	perspective->winTop = 0;
	perspective->winBottom = DISPLAY_HEIGHT - 1;
	sySynch(1);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_DEFAULT_MAP_METATILES),
		RamMapMetatileBuffer);
	fdWorldMapUpdateCurrentTileBlock();
	syResumeEvent(fdTilesetAnimUpdate);
	sySynch(1);
}

/* Each tick: runs the 16 tileset animations. */
void fdTilesetAnimUpdate(void)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	Uint32	   channelIdx;

	for(channelIdx = 0; channelIdx < ARRAY_COUNT(map->anim); channelIdx++) {
		const Uint16 *commands;
		Uint32		  srcTileOrOpcode;
		Uint32		  tileCnt;
		Uint32		  dstTile;

		/* A tileset animation with no script, or paused by a script, keeps its
		   place. */
		if((map->anim + channelIdx)->start == NULL ||
			(map->anim + channelIdx)->paused != 0)
			continue;
		/* While the wait lasts the tileset animation only counts down; when it
		   is over, commands run in this same tick until one sets a new wait or
		   stops. */
	next:
		if((map->anim + channelIdx)->tmr == 0) {
			commands = (map->anim + channelIdx)->cursor;
			srcTileOrOpcode = *commands++;
			if(srcTileOrOpcode == TILESET_ANIM_LOOP) {
				(map->anim + channelIdx)->cursor =
					(map->anim + channelIdx)->start;
				goto next;
			}
			/* TILESET_ANIM_STOP leaves the cursor on itself, so the tileset
			   animation stays stopped until it is started again;
			   TILESET_ANIM_JUMP jumps to the halfword pair its low byte
			   names. */
			if((srcTileOrOpcode & TILESET_ANIM_OPCODE_MASK) ==
				TILESET_ANIM_JUMP) {
				if((srcTileOrOpcode & TILESET_ANIM_ARGUMENT_MASK) ==
					TILESET_ANIM_STOP)
					continue;
				(map->anim + channelIdx)->cursor =
					(map->anim + channelIdx)->start +
					(srcTileOrOpcode & TILESET_ANIM_ARGUMENT_MASK) * 2;
				goto next;
			}
			tileCnt = *commands++;
			dstTile = commands[0];
			(map->anim + channelIdx)->tmr = commands[1];
			/* A source tile from TILESET_ANIM_RAM_TILE
			   (TILESET_ANIM_RAM_WIDE_TILE for 64-byte tiles) is read from RAM,
			   at the start of MapGridAnimTiles. */
			if(map->wideTiles == 0) {
				if(srcTileOrOpcode >= TILESET_ANIM_RAM_TILE)
					syDMAXCopy(
						RamDecodeBuffer + srcTileOrOpcode * TILE_SIZE_4BPP,
						BG_CHAR_BLOCK(1) + dstTile * TILE_SIZE_4BPP,
						DMA_COPY_WORDS(tileCnt * (TILE_SIZE_4BPP / 4)),
						REG_DMA3);
				else
					syDMAXCopy(
						BG_CHAR_BLOCK(1) + srcTileOrOpcode * TILE_SIZE_4BPP,
						BG_CHAR_BLOCK(1) + dstTile * TILE_SIZE_4BPP,
						DMA_COPY_WORDS(tileCnt * (TILE_SIZE_4BPP / 4)),
						REG_DMA3);
			} else {
				if(srcTileOrOpcode >= TILESET_ANIM_RAM_WIDE_TILE)
					syDMAXCopy(syRamMapBlocks +
							srcTileOrOpcode * TILESET_ANIM_WIDE_TILE_BYTES,
						BG_CHAR_BLOCK(2) +
							dstTile * TILESET_ANIM_WIDE_TILE_BYTES,
						DMA_COPY_WORDS(
							tileCnt * (TILESET_ANIM_WIDE_TILE_BYTES / 4)),
						REG_DMA3);
				else
					syDMAXCopy(BG_CHAR_BLOCK(2) +
							srcTileOrOpcode * TILESET_ANIM_WIDE_TILE_BYTES,
						BG_CHAR_BLOCK(2) +
							dstTile * TILESET_ANIM_WIDE_TILE_BYTES,
						DMA_COPY_WORDS(
							tileCnt * (TILESET_ANIM_WIDE_TILE_BYTES / 4)),
						REG_DMA3);
			}
			/* A copy with a wait of 0 lets the next command run at once. */
			(map->anim + channelIdx)->cursor += 4;
			goto next;
		} else {
			(map->anim + channelIdx)->tmr--;
		}
	}
}

/* Resumes tileset animation channelIdx. */
void fdTilesetAnimResume(Uint32 channelIdx)
{
	MAP_STATE	  *map = RamHeapSlots->mapGrid;
	MAP_ANIMATION *channel = &map->anim[channelIdx];

	channel->paused = 0;
}

/* Pauses tileset animation channelIdx. */
void fdTilesetAnimPause(Uint32 channelIdx)
{
	MAP_STATE	  *map = RamHeapSlots->mapGrid;
	MAP_ANIMATION *channel = &map->anim[channelIdx];

	channel->paused = 1;
}

/* Starts the tileset animations a map's animation script lists: each
   TILESET_ANIM_MARKER starts the tileset animation its low four bits name there
   (paused with TILESET_ANIM_MARKER_PAUSED). */
void fdTilesetAnimStartAll(const Uint16 *script)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	Sint32	   channelsStarted;
	Uint32	   marker;

	channelsStarted = 0;
	syWordsClearEntry(map->anim, sizeof(map->anim));
	/* Each tileset animation's script starts just after its marker and runs on
	   through the rest of the stream; the scan steps over the halfwords between
	   markers. */
	marker = *script++;
	/* The stream ends at the TILESET_ANIM_LOOP every script after a marker runs
	   on to. */
	while(marker != TILESET_ANIM_LOOP) {
		if((marker & TILESET_ANIM_OPCODE_MASK) == TILESET_ANIM_MARKER) {
			MAP_ANIMATION *channel;
			Uint32 channelIdx = marker & TILESET_ANIM_MARKER_ANIMATION_MASK;
			Uint32 paused = 0;

			if(marker & TILESET_ANIM_MARKER_PAUSED)
				paused = 1;
			channel = &map->anim[channelIdx];
			channel->start = script;
			channel->cursor = script;
			channel->tmr = 0;
			channel->paused = paused;
			channelsStarted++;
		}
		marker = *script++;
	}
	/* A map with no tileset animations never adds the update, nor adds it
	   twice. */
	if(channelsStarted != 0 && syFindEvent(fdTilesetAnimUpdate) == -1)
		sySetEvent(fdTilesetAnimUpdate, EVENT_PASS_TICK);
}

/* Suspends the tileset animations, unless the world map's still image is
   shown. */
void fdTilesetAnimSuspendAll(void)
{
	if(((MAP_STATE *)RamHeapSlots->mapGrid)->anmActive == 0)
		sySuspendEvent(fdTilesetAnimUpdate);
}

/* Resumes the tileset animations, unless the world map's still image is
   shown. */
void fdTilesetAnimResumeAll(void)
{
	if(((MAP_STATE *)RamHeapSlots->mapGrid)->anmActive == 0)
		syResumeEvent(fdTilesetAnimUpdate);
}

/* Each tick: runs the map's blend script. */
void fdBlendScriptRun(void)
{
	MAP_STATE		   *map = RamHeapSlots->mapGrid;
	BLEND_SCRIPT_STATE *blend;
	Uint16			   *commands;
	Uint32				instruction;
	Uint32				delayOrJumpIdx;
	Uint16				delay;

	blend = &map->blend;
	if(blend->script == 0)
		return;
	if(blend->paused != 0)
		return;

	/* As with the tileset animations, commands run in one tick until a level
	   sets a wait; a BLDCNT command and the jumps take no time. */
again:
	delayOrJumpIdx = blend->delay;
	delay = delayOrJumpIdx;
	if(delay == 0) {
		commands = blend->cursor;
		instruction = *commands;
		commands++;
		if(instruction == BLEND_SCRIPT_LOOP) {
			blend->cursor = blend->script;
			goto again;
		}

		if((instruction & BLEND_SCRIPT_OPCODE_MASK) == BLEND_SCRIPT_JUMP) {
			delayOrJumpIdx = instruction & BLEND_SCRIPT_ARGUMENT_MASK;
			if(delayOrJumpIdx == BLEND_SCRIPT_STOP)
				return;
			blend->cursor = blend->script + delayOrJumpIdx * 2;
			goto again;
		}

		/* A BLDCNT value is told apart by its top four bits being 3, its second
		   target's sprite and backdrop bits. */
		if((instruction & BLEND_SCRIPT_BLDCNT_MASK) == BLEND_SCRIPT_BLDCNT) {
			REG_BLDCNT = instruction;
			map->blendCtrl = instruction;
			blend->cursor++;
			goto again;
		}

		/* An alpha blend takes the level as BLDALPHA; a fade to white or black
		   takes it as BLDY. */
		if((map->blendCtrl & BLDCNT_EFFECT_MASK) == BLDCNT_EFFECT_BLEND)
			REG_BLDALPHA = instruction;
		else
			REG_BLDY = instruction;
		blend->delay = commands[0];
		blend->cursor += 2;
		goto again;
	}
	/* Counts the wait down by one. */
	blend->delay = delayOrJumpIdx - 1;
}

/* Starts a map's blend script, unless it is empty. Clears the map state's
   three-word blend state first, then adds fdBlendScriptRun as an event. */
void fdBlendScriptStart(Uint16 *script)
{
	BLEND_SCRIPT_STATE *blend;
	Sint32				hasScript;
	volatile Uint32		zero;

	/* A fixed-source DMA of the zero word clears script, cursor, delay and
	   paused; a script that is only its end marker leaves the blend unset. */
	hasScript = 0;
	zero = 0;
	blend = &((MAP_STATE *)RamHeapSlots->mapGrid)->blend;
	syDMAXCopy((const void *)&zero, blend, DMA_FILL_WORDS(sizeof(*blend) / 4),
		REG_DMA3);
	if(*script != BLEND_SCRIPT_LOOP) {
		blend->script = script;
		blend->cursor = script;
		blend->delay = 0;
		blend->paused = 0;
		hasScript = 1;
	}
	if(hasScript)
		sySetEvent(fdBlendScriptRun, EVENT_PASS_TICK);
}

/* Suspends the blend script; scenes reach it through
   fdBlendScriptSuspendStub. */
void fdBlendScriptSuspend(void)
{
	sySuspendEvent(fdBlendScriptRun);
}

/* Resumes the blend script, through fdBlendScriptResumeStub. */
void fdBlendScriptResume(void)
{
	syResumeEvent(fdBlendScriptRun);
}

#include "palette_cycle_initialize.c"

#include "palette_cycle_add.c"

/* Unused: stops the palette cycles and frees their queue
   (HEAP_SLOT_PALETTE_CYCLES). */
UNUSED void fdPaletteQueueStop(void)
{
	syCloseEvent(fdPaletteCycleStep);
	syHeapSlotRelease(HEAP_SLOT_PALETTE_CYCLES);
}

/* Unused: starts running the palette cycles each tick. */
UNUSED void fdPaletteCycleStart(void)
{
	sySetEvent(fdPaletteCycleStep, EVENT_PASS_TICK);
}

/* Each tick: advances each palette cycle whose timer has run out by one
   colour, writing the rotated colours back to the palette. */
void fdPaletteCycleStep(void)
{
	PALETTE_CYCLE_STATE *paletteQueue = RamHeapSlots->palCycles;
	Uint8				 cycleIdx;
	/* BUG: four queued cycles become zero here, so none advance. */
	Uint16 activeCycles = paletteQueue->cnt & 3;
	Uint16 rotatedCols[PALETTE_COLORS];

	for(cycleIdx = 0; cycleIdx < activeCycles; cycleIdx++) {
		PALETTE_CYCLE *cycle = &paletteQueue->entries[cycleIdx];
		if(cycle->tmr == 0) {
			Uint16	rotationOfs = cycle->pos;
			Uint16	colCnt = cycle->cnt;
			void   *paletteCols = cycle->src;
			Uint16 *savedCols = cycle->cols;
			Uint8	colIdx;

			/* Each saved colour moves rotationOfs places down, the first
			   ones wrapping round to the end. */
			for(colIdx = colCnt - rotationOfs; colIdx < colCnt; colIdx++)
				rotatedCols[colIdx] = *savedCols++;
			for(colIdx = 0; colIdx < colCnt - rotationOfs; colIdx++)
				rotatedCols[colIdx] = *savedCols++;
			syDMAXCopy(
				rotatedCols, paletteCols, DMA_COPY_HALFWORDS(colCnt), REG_DMA3);
			/* The next step rotates one place further, back to none after a
			   full turn, and waits delay ticks. */
			rotationOfs++;
			if(rotationOfs >= colCnt)
				rotationOfs = 0;
			cycle->pos = rotationOfs;
			cycle->tmr = cycle->delay;
		} else {
			cycle->tmr--;
		}
	}
}

/* The encounter regions the world map is tinted in, and their tints in BLDY's
   sixteenths: one brightened, one darkened until the Gabomba ceremony ends. */
#define TINT_BRIGHT_REGION 9
#define TINT_BRIGHT		   12
#define TINT_DARK_REGION   18
#define TINT_DARK		   6

/* Tints the world map for the encounter region under the party, any region but
   TINT_BRIGHT_REGION and TINT_DARK_REGION clearing the tint. Gradually, the
   tint moves one step every 16 ticks; at once, it jumps there. While a menu is
   open the tint stays as it is. */
void fdWorldMapUpdateRegionTint(Sint32 region, Sint32 gradual)
{
	WORLD_MAP_VIEW *perspective = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;

	if(perspective->tintRegion != region) {
		perspective->tintRegion = region;
		if(region == TINT_BRIGHT_REGION)
			perspective->tintTgt = TINT_BRIGHT;
		else if(region == TINT_DARK_REGION &&
			!syGameFlagGet(FLAG_GABOMBA_CEREMONY_ENDED))
			perspective->tintTgt = -TINT_DARK;
		else
			perspective->tintTgt = 0;
	}
	if(syGameFlagGet(FLAG_MENU_OPEN))
		return;
	if(gradual == 0) {
		perspective->tint = perspective->tintTgt;
	} else {
		region = perspective->tintTgt - perspective->tint;
		if(region == 0)
			return;
		if(TickCount & 15)
			return;
		if(region < 0)
			region = -1;
		else
			region = 1;
		perspective->tint += region;
	}
	if(perspective->tint > 0) {
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
			BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
				BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
			IO_WRITE_HALFWORD);
		IO_WRITE_QUEUE_PUSH(
			(Uint32)&REG_BLDY, (Uint16)perspective->tint, IO_WRITE_HALFWORD);
	} else {
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
			BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
				BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_ALL,
			IO_WRITE_HALFWORD);
		IO_WRITE_QUEUE_PUSH(
			(Uint32)&REG_BLDY, (Uint16)-perspective->tint, IO_WRITE_HALFWORD);
	}
}

/* The ground height functions (MapGridTerrainHeightFunctions): each takes a
   metatile's height samples after its kind byte and a pixel (x, y) within the
   16x16 metatile, and returns the ground height there in 16.16 pixels. */
FIXED fdTerrainGetFlatHeight(const Sint8 *samples)
{
	return samples[0] << CURVE_VALUE_SHIFT;
}

/* The height of a slope across x at position (0 to 15 of the metatile): a
   straight line from samples[0] to samples[1]. */
FIXED fdTerrainGetSlopeXHeight(const Sint8 *samples, Sint32 pos)
{
	Sint32 start;
	Sint32 delta;

	start = samples[0] << CURVE_VALUE_SHIFT;
	delta = ((samples[1] << CURVE_VALUE_SHIFT) - start) * pos;
	return start + delta / CURVE_FULL_STEPS;
}

/* A slope along y: from sample 0 at the top to sample 1 at the bottom. */
FIXED fdTerrainGetSlopeYHeight(const Sint8 *samples, Sint32 unused, Sint32 pos)
{
	Sint32 base;
	Sint32 ofs;

	base = samples[0] << CURVE_VALUE_SHIFT;
	ofs = ((samples[1] << CURVE_VALUE_SHIFT) - base) * pos;
	return base + ofs / CURVE_FULL_STEPS;
}

/* A step across the diagonal x + y = 15: sample 0 before it, sample 1 after it
   and the higher of the two on it. */
FIXED fdTerrainGetStepSumDiagonalHeight(
	const Sint8 *samples, Sint32 x, Sint32 y)
{
	Sint32 first;
	Sint32 second;
	Sint32 maximum;
	Uint32 pos;

	first = samples[0] << CURVE_VALUE_SHIFT;
	second = samples[1] << CURVE_VALUE_SHIFT;
	maximum = first;
	if(second > first) {
		maximum = second;
	}
	pos = x + y;
	if(pos == CURVE_FULL_STEPS - 1) {
		return maximum;
	}
	if(pos < CURVE_FULL_STEPS - 1) {
		second = first;
	}
	return second;
}

/* A step across the diagonal x = y: sample 0 where y < x, sample 1 where y > x
   and the higher of the two on it. */
FIXED fdTerrainGetStepDifferenceDiagonalHeight(
	const Sint8 *samples, Sint32 x, Sint32 y)
{
	Sint32 first;
	Sint32 second;
	Sint32 maximum;
	Sint32 difference;
	Uint32 pos = x;

	first = samples[0] << CURVE_VALUE_SHIFT;
	second = samples[1] << CURVE_VALUE_SHIFT;
	maximum = first;
	if(second > first) {
		maximum = second;
	}
	difference = y - pos;
	pos = difference + CURVE_FULL_STEPS - 1;
	if(pos == CURVE_FULL_STEPS - 1) {
		return maximum;
	}
	if(pos < CURVE_FULL_STEPS - 1) {
		second = first;
	}
	return second;
}

/* A slope that bends on the diagonal x + y = 15: from sample 0 at the top left
   corner to sample 1 on the diagonal to sample 2 at the bottom right. */
FIXED fdTerrainGetBendSumDiagonalHeight(
	const Sint8 *samples, Sint32 x, Sint32 y)
{
	Sint32 first;
	Sint32 middle;
	Sint32 last;
	Sint32 result;

	first = *samples++ << CURVE_VALUE_SHIFT;
	middle = *samples << CURVE_VALUE_SHIFT;
	last = samples[1] << CURVE_VALUE_SHIFT;
	/* x now holds x + y, which is 15 on the diagonal. */
	x = x + y;
	if((Uint32)x == CURVE_FULL_STEPS - 1) {
		result = middle;
	} else if((Uint32)x < CURVE_FULL_STEPS - 1) {
		result = first + (middle - first) * x / (CURVE_FULL_STEPS - 1);
	} else {
		x = x - (CURVE_FULL_STEPS - 1);
		result = middle + (last - middle) * x / (CURVE_FULL_STEPS - 1);
	}
	return result;
}

/* A slope that bends on the diagonal x = y: from sample 0 at the top right
   corner to sample 1 on the diagonal to sample 2 at the bottom left. */
FIXED fdTerrainGetBendDifferenceDiagonalHeight(
	const Sint8 *samples, Sint32 x, Sint32 y)
{
	Sint32 first;
	Sint32 second;
	Sint32 third;
	Sint32 difference;
	Sint32 delta;

	first = *samples++ << CURVE_VALUE_SHIFT;
	second = samples[0] << CURVE_VALUE_SHIFT;
	third = samples[1];
	/* x now holds y - x + 15, which is 15 on the diagonal. */
	difference = y - x;
	x = difference + CURVE_FULL_STEPS - 1;
	third <<= CURVE_VALUE_SHIFT;

	if(x == CURVE_FULL_STEPS - 1)
		return second;

	if((Uint32)x < CURVE_FULL_STEPS - 1) {
		delta = second - first;
		difference = delta * x / CURVE_FULL_STEPS;
		return first + difference;
	} else {
		difference *= third - second;
		return second + difference / CURVE_FULL_STEPS;
	}
}

/* A raised or sunken rounded square: sample 1 where TerrainRoundedSquareMask
   is 1, sample 0 in the corners. */
FIXED fdTerrainGetRoundedSquareHeight(
	const Sint8 *samples, Sint32 pos, Sint32 row)
{
	Sint32 start;

	start = samples[0] << CURVE_VALUE_SHIFT;
	/* The weight is 0 or 1, so the result is one sample or the other, never
	   between. */
	return start +
		(((samples[1] << CURVE_VALUE_SHIFT) - start) *
			TerrainRoundedSquareMask[pos + (row * CURVE_FULL_STEPS)]);
}

/* The height of a step across x at position (0 to 15 of the metatile):
   samples[0] on the metatile's first half, samples[1] on its second. */
FIXED fdTerrainGetStepXHeight(const Sint8 *samples, Uint32 pos)
{
	Sint32 value;
	if(pos < CURVE_HALF_STEPS)
		value = samples[0];
	else
		value = samples[1];
	return value << CURVE_VALUE_SHIFT;
}

/* A step along y: sample 0 above the middle, sample 1 below it. */
FIXED fdTerrainGetStepYHeight(const Sint8 *samples, Uint32 unused, Uint32 pos)
{
	Sint32 value;
	if(pos < CURVE_HALF_STEPS)
		value = samples[0];
	else
		value = samples[1];
	return value << CURVE_VALUE_SHIFT;
}

/* As fdTerrainGetSampleTableHeight with x and y swapped. */
FIXED fdTerrainGetSampleTableTransposedHeight(
	const Sint8 *samples, Uint32 pixelX, Uint32 pixelY)
{
	return samples[TerrainSampleIndexTable[pixelY + (pixelX << 4)]]
		<< CURVE_VALUE_SHIFT;
}

/* As fdTerrainGetSampleTableMirroredHeight with x and y swapped. */
FIXED fdTerrainGetSampleTableTransposedMirroredHeight(
	const Sint8 *samples, Uint32 row, Uint32 pos)
{
	const Uint8 *tbl = TerrainSampleIndexTable;
	Uint32		 idx;

	idx = (row << 4) - pos;
	idx += CURVE_FULL_STEPS - 1;
	return samples[tbl[idx]] << CURVE_VALUE_SHIFT;
}

/* The sample TerrainSampleIndexTable picks for (x, y): a byte per pixel of the
   metatile, row by row. */
FIXED fdTerrainGetSampleTableHeight(
	const Sint8 *samples, Uint32 pos, Uint32 row)
{
	return samples[TerrainSampleIndexTable[pos + (row << 4)]]
		<< CURVE_VALUE_SHIFT;
}

/* The same, with the table mirrored left to right. */
FIXED fdTerrainGetSampleTableMirroredHeight(
	const Sint8 *samples, Uint32 pos, Uint32 row)
{
	const Uint8 *tbl = TerrainSampleIndexTable;
	Uint32		 idx;

	idx = (row << 4) - pos;
	idx += CURVE_FULL_STEPS - 1;
	return samples[tbl[idx]] << CURVE_VALUE_SHIFT;
}

/* Above y 8 a step along x from the first sample to the second; below, the
   third. */
FIXED fdTerrainGetSplitTopStepHeight(
	const Sint8 *samples, Uint32 column, Uint32 row)
{
	Sint32 value;

	if(row < CURVE_HALF_STEPS) {
		if(column < CURVE_HALF_STEPS)
			value = samples[0];
		else
			value = samples[1];
	} else {
		value = samples[2];
	}
	return value << CURVE_VALUE_SHIFT;
}

/* Above y 8 the first sample; below, a step along x from the second to the
   third. */
FIXED fdTerrainGetSplitBottomStepHeight(
	const Sint8 *samples, Uint32 column, Uint32 row)
{
	Sint32 value;

	if(row < CURVE_HALF_STEPS)
		value = samples[0];
	else if(column < CURVE_HALF_STEPS)
		value = samples[1];
	else
		value = samples[2];
	return value << CURVE_VALUE_SHIFT;
}

typedef Sint32 (*TERRAIN_HEIGHT_FN)(Uint8 *heights, Sint32 x, Sint32 z);

extern TERRAIN_HEIGHT_FN MapGridTerrainHeightFunctions[16];

/* The ground height the shape at code gives at pixel (x, z) of its metatile, in
   16.16 pixels: its terrain kind's height function on its samples. */
/* UB: the kind byte is read, and the samples after it passed, in one
   unsequenced expression. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
#define fdMapGridGetShapeHeight(code, x, z) \
	MapGridTerrainHeightFunctions[(code)[0] & MAPGRID_SHAPE_KIND_MASK]( \
		(code) + 1, (x) & (MAPGRID_METATILE_PIXELS - 1), \
		(z) & (MAPGRID_METATILE_PIXELS - 1))
#else
#define fdMapGridGetShapeHeight(code, x, z) \
	MapGridTerrainHeightFunctions[*(code)++ & MAPGRID_SHAPE_KIND_MASK]((code), \
		(x) & (MAPGRID_METATILE_PIXELS - 1), \
		(z) & (MAPGRID_METATILE_PIXELS - 1))
#endif

/* Returns the ground height of a layer at (x, z) in 16.16 pixels, as The Broken
   Seal's: the height function of the metatile's terrain kind applied to its
   samples. */
FIXED fdMapGridGetTerrainHeight(Sint32 layer, FIXED x, FIXED z)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	Uint8	  *grid;
	Uint8	  *code;

	x >>= 16;
	z >>= 16;
	if(map != NULL)
		grid = map->layers[layer & 3].terrainIds;
	else
		grid = MapShapeGrid;
	/* A row of the grid is MAPGRID_SIZE metatiles, 1 << 7; a shape is 4
	   bytes. */
	grid += x / MAPGRID_METATILE_PIXELS + (z / MAPGRID_METATILE_PIXELS << 7);
	code = RamMapCollision + *grid * 4;
	return fdMapGridGetShapeHeight(code, x, z);
}

/* Returns the terrain kind of a layer's metatile at (x, z) in 16.16 pixels, as
   The Broken Seal's. */
Sint32 fdMapGridGetTerrainKindAt(Sint32 layer, FIXED x, FIXED z)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	Uint8	  *grid;

	x >>= 16;
	z >>= 16;
	if(map != NULL)
		grid = map->layers[layer & 3].terrainIds;
	else
		grid = MapShapeGrid;
	return RamMapCollision[grid[x / MAPGRID_METATILE_PIXELS +
							   (z / MAPGRID_METATILE_PIXELS << 7)] *
			   4] &
		MAPGRID_SHAPE_KIND_MASK;
}

/* As fdMapGridGetTerrainKindAt, but a rounded square whose two samples differ
   reads as TERRAIN_BUMP. */
Sint32 fdMapGridGetTerrainKindOrBumpAt(Sint32 layer, Sint32 x, Sint32 z)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	Uint8	  *grid;
	Uint8	  *code;
	Sint32	   kind;

	x >>= 16;
	z >>= 16;
	if(map != NULL)
		grid = map->layers[layer & 3].terrainIds;
	else
		grid = MapShapeGrid;
	code = &RamMapCollision[grid[x / MAPGRID_METATILE_PIXELS +
								(z / MAPGRID_METATILE_PIXELS << 7)] *
		4];
	kind = code[0];
	if(kind == TERRAIN_ROUNDED_SQUARE && code[1] != code[2])
		kind = TERRAIN_BUMP;
	return kind;
}

/* The highest ground at (x, z) and at the four points `reach` away from it
   along x and z, all in 16.16 pixels. */
FIXED fdMapGridGetHighestTerrainHeightAround(
	Sint32 layer, FIXED x, FIXED z, FIXED reach)
{
	FIXED best;
	FIXED height;

	best = fdMapGridGetTerrainHeight(layer, x, z);
	height = fdMapGridGetTerrainHeight(layer, x - reach, z);
	if(best < height)
		best = height;
	height = fdMapGridGetTerrainHeight(layer, x + reach, z);
	if(best < height)
		best = height;
	height = fdMapGridGetTerrainHeight(layer, x, z - reach);
	if(best < height)
		best = height;
	height = fdMapGridGetTerrainHeight(layer, x, z + reach);
	if(best < height)
		best = height;
	return best;
}

/* Gives the metatile at (x, z) of layer the ground height `height`, in 8-pixel
   steps, by tagging its shape with terrain kind 0 at that height. */
void fdMapGridSetMetatileHeight(Sint32 x, Sint32 z, Sint32 layer, Sint32 height)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	Uint8	  *shape;

	shape = MapShapeGrid +
		(map->layers[layer & 3].metatiles -
			(MAP_METATILE *)RamMapMetatileBuffer);
	shape += (FIELD_METATILE(x)) + FIELD_METATILE(z) * MAPGRID_SIZE;
	*shape = fdMapShapeFindOrAddStub(layer, 0, height);
}

/* The ground height at (x, z) of layer, in 8-pixel steps. */
Sint32 fdMapGridGetMetatileHeight(FIXED x, FIXED z, Sint32 layer)
{
	return fdMapGridGetTerrainHeight(layer, x, z) >> CURVE_VALUE_SHIFT;
}

/* As fdMapGridGetTerrainKindAt: the ROM keeps two functions with this body,
   each with callers of its own (the scenes' overlays import only this one). */
Sint32 fdMapGridGetTerrainKind(Sint32 layer, Sint32 x, Sint32 z)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	Uint8	  *grid;

	x >>= 16;
	z >>= 16;
	if(map != NULL)
		grid = map->layers[layer & 3].terrainIds;
	else
		grid = MapShapeGrid;
	return RamMapCollision[grid[x / MAPGRID_METATILE_PIXELS +
							   (z / MAPGRID_METATILE_PIXELS << 7)] *
			   4] &
		MAPGRID_SHAPE_KIND_MASK;
}

/* Returns the metatile behavior of a layer's metatile at (x, y) in 16.16
   pixels. */
Uint8 fdMapGridGetMetatileBehaviorAt(Sint32 layer, FIXED x, FIXED y)
{
	MAP_STATE	 *state;
	MAP_METATILE *metatiles;

	state = RamHeapSlots->mapGrid;
	x = FIELD_METATILE(x);
	y = FIELD_METATILE(y);
	metatiles = (MAP_METATILE *)MapGridMetatileBuffer;
	if(state != NULL) {
		metatiles = state->layers[layer & 3].metatiles;
	}
	metatiles += x + y * MAPGRID_SIZE;
	return metatiles->collisionCode;
}

/* Sets the metatile behavior of a layer's metatile at (x, y) in 16.16 pixels.
   Writes the metatile in the map's grid; does nothing before the map is set
   up. */
void fdMapGridSetMetatileBehaviorAt(
	Uint32 layer, FIXED x, FIXED y, Uint32 collisionCode)
{
	MAP_STATE *state = RamHeapSlots->mapGrid;

	x = FIELD_METATILE(x);
	y = FIELD_METATILE(y);
	if(state != NULL) {
		MAP_METATILE *metatiles = state->layers[layer & 3].metatiles;

		metatiles += x + y * MAPGRID_SIZE;
		metatiles->collisionCode = collisionCode;
	}
}

/* The height of the ground at (px, pz) (16.16 pixels) on the given layer, less
   base; a layer past 2 reads the shape grid. With no map, or on the World Map,
   it is 0. */
FIXED fdMapGridGetTerrainHeightAbove(
	FIXED px, FIXED pz, Sint32 layer, FIXED base)
{
	Sint32		 x = FIXEDtoINT(px);
	Sint32		 z = FIXEDtoINT(pz);
	MAP_STATE	*map = RamHeapSlots->mapGrid;
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Uint8		*shapes;
	Uint8		*code;

	if(map == NULL)
		return 0;
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP)
		return 0;
	if(layer <= 2)
		shapes = map->layers[layer].terrainIds;
	else
		shapes = MapShapeGrid;
	code = RamMapCollision +
		shapes[x / MAPGRID_METATILE_PIXELS +
			(z / MAPGRID_METATILE_PIXELS << 7)] *
			4;
	return fdMapGridGetShapeHeight(code, x, z) - base;
}

/* How the ground at (px, pz) (16.16 pixels) on the given layer stands to
   height: 2 on a wall, 1 when the ground rises more than 8 pixels above it, -1
   when it falls more than 12 below, else 0. A layer past 2 reads the metatile
   buffer and shape grid. With no map, or on the World Map, every point is level
   (0). */
Sint32 fdMapGridCheckCollisionAt(
	FIXED px, FIXED pz, Sint32 layer, Sint32 height)
{
	Sint32		 x = FIXEDtoINT(px);
	Sint32		 z = FIXEDtoINT(pz);
	MAP_STATE	*map = RamHeapSlots->mapGrid;
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Uint8		*metatiles;
	Uint8		*shapes;
	Uint8		*code;
	Sint32		 idx;
	Sint32		 rise;

	if(map == NULL)
		return 0;
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP)
		return 0;
	if(layer <= 2) {
		metatiles = (Uint8 *)map->layers[layer].metatiles;
		shapes = map->layers[layer].terrainIds;
	} else {
		metatiles = MapGridMetatileBuffer;
		shapes = MapShapeGrid;
	}
	idx = x / MAPGRID_METATILE_PIXELS + (z / MAPGRID_METATILE_PIXELS << 7);
	metatiles += idx * 4;
	shapes += idx;
	if(metatiles[2] == METATILE_BEHAVIOR_WALL)
		return 2;
	code = RamMapCollision + *shapes * 4;
	rise = fdMapGridGetShapeHeight(code, x, z) - height;
	if(rise > toFIXED(8))
		return 1;
	if(rise < -toFIXED(12))
		return -1;
	return 0;
}

/* As fdMapGridCheckCollisionAt for the point position (16.16 pixels, read as
   its whole-pixel halves) on the actor's layer, against its ground height. */
Sint32 fdMapGridCheckCollision(ACTOR *actor, POS *pos)
{
	Sint32		 x = ((FIELD_COORDINATE *)&pos->x)->part[0].integer;
	Sint32		 z = ((FIELD_COORDINATE *)&pos->z)->part[0].integer;
	MAP_STATE	*map = RamHeapSlots->mapGrid;
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Uint8		*metatiles;
	Uint8		*shapes;
	Uint8		*code;
	Sint32		 idx;
	Sint32		 rise;

	if(map == NULL)
		return 0;
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP)
		return 0;
	if(actor->layer <= 2) {
		metatiles = (Uint8 *)map->layers[actor->layer].metatiles;
		shapes = map->layers[actor->layer].terrainIds;
	} else {
		metatiles = MapGridMetatileBuffer;
		shapes = MapShapeGrid;
	}
	idx = x / MAPGRID_METATILE_PIXELS + (z / MAPGRID_METATILE_PIXELS << 7);
	metatiles += idx * 4;
	shapes += idx;
	if(metatiles[2] == METATILE_BEHAVIOR_WALL)
		return 2;
	code = RamMapCollision + *shapes * 4;
	rise = fdMapGridGetShapeHeight(code, x, z) - actor->groundHeight;
	if(rise > toFIXED(8))
		return 1;
	if(rise < -toFIXED(12))
		return -1;
	return 0;
}

/* As fdMapGridCheckCollision, against the actor's height and with no -1: 2 on a
   wall, 1 when the ground rises more than 8 pixels above it, else 0. */
Sint32 fdMapGridCheckCollisionAbove(ACTOR *actor, POS *pos)
{
	Sint32		 x = ((FIELD_COORDINATE *)&pos->x)->part[0].integer;
	Sint32		 z = ((FIELD_COORDINATE *)&pos->z)->part[0].integer;
	MAP_STATE	*map = RamHeapSlots->mapGrid;
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Uint8		*metatiles;
	Uint8		*shapes;
	Uint8		*code;
	Sint32		 idx;

	if(map == NULL)
		return 0;
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP)
		return 0;
	if(actor->layer <= 2) {
		metatiles = (Uint8 *)map->layers[actor->layer].metatiles;
		shapes = map->layers[actor->layer].terrainIds;
	} else {
		metatiles = MapGridMetatileBuffer;
		shapes = MapShapeGrid;
	}
	idx = x / MAPGRID_METATILE_PIXELS + (z / MAPGRID_METATILE_PIXELS << 7);
	metatiles += idx * 4;
	shapes += idx;
	if(metatiles[2] == METATILE_BEHAVIOR_WALL)
		return 2;
	code = RamMapCollision + *shapes * 4;
	if(fdMapGridGetShapeHeight(code, x, z) - actor->y.fixed > toFIXED(8))
		return 1;
	return 0;
}

/* Returns -1 when the metatile under position (its x, and z less its height) in
   the overhead layer is a wall, else 0; also 0 before the map state exists. */
Sint32 fdMapGridIsWallAt(Sint32 *pos)
{
	Sint32 x = pos[0] / toFIXED(1);
	Sint32 z = (Uint32)((pos[2] & FIELD_METATILE_MASK) - pos[1]) / toFIXED(1);
	MAP_STATE	 *map = RamHeapSlots->mapGrid;
	MAP_METATILE *metatile;

	if(map == NULL)
		return 0;
	metatile = map->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
	metatile += x / MAPGRID_METATILE_PIXELS +
		(z / MAPGRID_METATILE_PIXELS) * MAPGRID_SIZE;
	return (metatile->collisionCode != METATILE_BEHAVIOR_WALL) - 1;
}

/* The metatile behavior of the world map block under position (blocks 32 pixels
   square on a 32 by 32 grid), which the World Map's step cues answer
   (world_map.c: STEP_CUE_AREA(AREA_DAILA) enters Daila,
   METATILE_BEHAVIOR_WORLD_MAP_IRON meets Iron), or METATILE_BEHAVIOR_WALL where
   the map has no block. */
Sint32 fdWorldMapGetBlockMetatileBehavior(WORLD_POS *pos)
{
	Sint32 z = pos->y;
	Sint32 x = pos->x;
	Uint32 idx;

	idx = ((x / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
			  (WORLD_MAP_VIEW_METATILES - 1)) +
		((z / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
			(WORLD_MAP_VIEW_METATILES - 1)) *
			WORLD_MAP_VIEW_METATILES;
	return ((WORLD_MAP_BLOCKS *)MetatileTiles)->blocks[idx].place;
}

/* Returns the world map's collision value (1-15) at position: from the upper
   collision layer where it has one, else the lower, else
   WORLD_MAP_COLLISION_DEFAULT. */
Sint32 fdWorldMapGetCollision(WORLD_POS *pos)
{
	Sint32 halfPixelX = FIXEDtoINT(pos->x) >> 1;
	Sint32 y = pos->y;
	Sint32 halfPixelY = FIXEDtoINT(y) >> 1;
	Uint32 collisionMetatile;
	Uint32 pixelOfs;
	Uint8  collisionTile;
	Uint8  packedCollisions;
	Uint32 result;

	/* Coordinates in 2-pixel units, a tile map's tile 8 of them across. */
	collisionMetatile = ((halfPixelY / 8) & (WORLD_MAP_TILE_MAP_WIDTH - 1)) *
			WORLD_MAP_TILE_MAP_WIDTH +
		((halfPixelX / 8) & (WORLD_MAP_TILE_MAP_WIDTH - 1));
	pixelOfs = (((halfPixelY / 2) & 3) << 1) + ((halfPixelX / 4) & 1);

	/* The upper layer's collision tiles follow the lower layer's. */
	collisionTile = WORLD_MAP_UPPER_TILE_MAP[collisionMetatile];
	packedCollisions = *(RamMapCollision +
		WORLD_MAP_COLLISION_TILES * WORLD_MAP_COLLISION_TILE_BYTES +
		collisionTile * WORLD_MAP_COLLISION_TILE_BYTES + pixelOfs);
	if(packedCollisions != 0) {
		if(halfPixelX & 2)
			result = packedCollisions >> 4;
		else
			result = packedCollisions & 0xf;
		if(result != 0)
			return result;
	}

	/* The lower layer's, at the start of the buffer. */
	collisionTile = WORLD_MAP_LOWER_TILE_MAP[collisionMetatile];
	packedCollisions = *(RamMapCollision +
		collisionTile * WORLD_MAP_COLLISION_TILE_BYTES + pixelOfs);
	if(packedCollisions != 0) {
		if(halfPixelX & 2)
			result = packedCollisions >> 4;
		else
			result = packedCollisions & 0xf;
		if(result != 0)
			return result;
	}

	return WORLD_MAP_COLLISION_DEFAULT;
}

/* Returns 0 when the world map at position can be walked on (a place there, and
   collision WORLD_MAP_COLLISION_WALKABLE_FIRST to
   WORLD_MAP_COLLISION_WALKABLE_LAST), else -1. */
Sint32 fdWorldMapIsWalkable(void *mover, WORLD_POS *pos)
{
	if(fdWorldMapGetBlockMetatileBehavior(pos) != METATILE_BEHAVIOR_WALL) {
		if((Uint32)(fdWorldMapGetCollision(pos) -
			   WORLD_MAP_COLLISION_WALKABLE_FIRST) <=
			WORLD_MAP_COLLISION_WALKABLE_LAST -
				WORLD_MAP_COLLISION_WALKABLE_FIRST)
			return 0;
	}
	return -1;
}

/* Returns 0 where the ship can sail (the sea or shallow water), else -1. */
Sint32 fdWorldMapIsSailable(void *mover, WORLD_POS *pos)
{
	if(fdWorldMapGetBlockMetatileBehavior(pos) != METATILE_BEHAVIOR_WALL) {
		Sint32		 collision = fdWorldMapGetCollision(pos);
		FIELD_STATE *state = RamHeapSlots->fieldState;
		Sint32		 mode;

		/* Whichever water the ship was last over, it sails the same water; last
		   over ground (SHIP_TERRAIN_GROUND), it sails nowhere. */
		mode = state->terrainKind;
		if(mode == SHIP_TERRAIN_SEA) {
			if((Uint32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) <=
					WORLD_MAP_COLLISION_SEA_LAST -
						WORLD_MAP_COLLISION_SEA_FIRST ||
				collision == WORLD_MAP_COLLISION_SHALLOW_WATER)
				return 0;
		} else if(mode == SHIP_TERRAIN_SHALLOWS) {
			if((Uint32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) <=
					WORLD_MAP_COLLISION_SEA_LAST -
						WORLD_MAP_COLLISION_SEA_FIRST ||
				collision == WORLD_MAP_COLLISION_SHALLOW_WATER)
				return 0;
		} else if(mode == SHIP_TERRAIN_SEA_LAST) {
			if((Uint32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) <=
					WORLD_MAP_COLLISION_SEA_LAST -
						WORLD_MAP_COLLISION_SEA_FIRST ||
				collision == WORLD_MAP_COLLISION_SHALLOW_WATER)
				return 0;
		}
	}
	return -1;
}

/* Returns 0 where the flying ship can go, else -1: low down, where it can sail;
   higher, more of the ground by the kind it last flew over. */
Sint32 fdWorldMapIsFlyable(ACTOR *mover, WORLD_POS *pos)
{
	if(fdWorldMapGetBlockMetatileBehavior(pos) != METATILE_BEHAVIOR_WALL) {
		Sint32		 collision = fdWorldMapGetCollision(pos);
		FIELD_STATE *state = RamHeapSlots->fieldState;

		Sint32 height = mover->y.fixed;

		if(height < toFIXED(4)) {
			if((Uint32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) >
					WORLD_MAP_COLLISION_SEA_LAST -
						WORLD_MAP_COLLISION_SEA_FIRST &&
				collision != WORLD_MAP_COLLISION_SHALLOW_WATER)
				return -1;
			return 0;
		} else {
			Sint32 mode = state->terrainKind;

			if(mode == SHIP_TERRAIN_SEA) {
				if((Uint32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) <=
					WORLD_MAP_COLLISION_SHALLOW_WATER -
						WORLD_MAP_COLLISION_SEA_FIRST)
					return 0;
			} else if(mode == SHIP_TERRAIN_SHALLOWS) {
				if((Uint32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) <=
					WORLD_MAP_COLLISION_FLYABLE_LAST -
						WORLD_MAP_COLLISION_SEA_FIRST)
					return 0;
			} else {
				if(mode == SHIP_TERRAIN_SEA_LAST) {
					if((Uint32)(collision - WORLD_MAP_COLLISION_SEA_FIRST) <=
						WORLD_MAP_COLLISION_FLYABLE_LAST -
							WORLD_MAP_COLLISION_SEA_FIRST)
						return 0;
				} else {
					if((Uint32)(collision - WORLD_MAP_COLLISION_SEA_LAST) <=
						WORLD_MAP_COLLISION_FLYABLE_LAST -
							WORLD_MAP_COLLISION_SEA_LAST)
						return 0;
				}
				/* XXX (no evidence): what ground collision 10 is, which a
				   battle counts as grassland as it does 7, 8, 11 and 12;
				   nothing in reach shows the World Map's collision under a
				   point, and no text names it. */
				if(collision == 10)
					return 0;
			}
		}
	}
	return -1;
}

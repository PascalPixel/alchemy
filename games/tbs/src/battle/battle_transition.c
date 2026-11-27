/* The opening transition: the curtain, or the Venus Lighthouse storm. Part of
   battle.c. */

#include "battle_internal.h"

/* The curtain's first tile, CURTAIN_TILE(0), as BG1 numbers it from character
   block 1. */
#define TRANSITION_CURTAIN_FIRST_TILE 128
/* The curtain's map entries: its tile n, in WINDOW_PALETTE. */
#define TRANSITION_CURTAIN_ENTRY(n) \
	WINDOW_TILE(TRANSITION_CURTAIN_FIRST_TILE + (n))
/* The closed curtain's colour, which fills its first tile. */
#define TRANSITION_CURTAIN_COLOR	  3
/* The screen block of the curtain's map, and its middle row, from which it
   opens a row up and a row down every eight frames for
   TRANSITION_OPEN_FRAMES. */
#define TRANSITION_CURTAIN_SCREEN	  12
#define TRANSITION_CURTAIN_MIDDLE_ROW 13
#define TRANSITION_CURTAIN_LAST_ROW	  20
#define TRANSITION_OPEN_FRAMES		  80
/* Eight variants of the curtain's first tile, a new one every four frames
   (btTransitionUploadTileVariant). */
#define TRANSITION_CURTAIN_VARIANTS	  8
#define TRANSITION_VARIANT_FRAMES	  4
/* BG0's top 32 lines, which roll down into place as the curtain opens: BG0
   starts scrolled up by as many, and the VCount interrupt at the band's bottom
   clears the scroll for the lines below. */
#define TRANSITION_BG0_BAND			  32
/* The camera turns 360 of TRIG_TURN's 65,536 a frame (about 2 degrees) as the
   curtain opens, ending at DEGtoANG(45) as the zoom ends. */
#define TRANSITION_TURN_SPEED		  360
#define TRANSITION_START_YAW \
	(TRIG_TURN + DEGtoANG(45) - TRANSITION_OPEN_FRAMES * TRANSITION_TURN_SPEED)

/* REG_MOSAIC's largest block size, 16 pixels: each of its sizes is a four-bit
   field holding the size less one. */
#define MOSAIC_SIZE_MAX 15

/* The words that hold a background map row's 32 entries two at a time. */
#define MAP_ROW_WORDS (MAP_ROW_BYTES / 4)
/* Four affine map entries side by side, a byte each, written a word at a
   time. */
#define AFFINE_MAP_ENTRIES(first, second, third, fourth) \
	((fourth) << 24 | (third) << 16 | (second) << 8 | (first))
/* The battle background's image: 32 by 15 tiles of 256 colours, 256 by 120
   pixels. The first 256 tiles fill character block 2 and the rest block 3. */
#define BACKGROUND_TILE_ROWS		15
#define BACKGROUND_TILES			(BG_SCREEN_WIDTH * BACKGROUND_TILE_ROWS)
#define BACKGROUND_UPPER_TILES		(BG_CHAR_SIZE / TILE_SIZE_8BPP)
#define BACKGROUND_LOWER_TILES		(BACKGROUND_TILES - BACKGROUND_UPPER_TILES)
/* The zoomed background's affine map: 32 by 32 tiles, a byte each. */
#define BACKGROUND_AFFINE_MAP_BYTES (32 * 32)
/* The picture's first tile on BG1, numbered from BG1's character base 0:
   character block 2. */
#define BACKGROUND_BG1_FIRST_TILE	(2 * BG_CHAR_SIZE / TILE_SIZE_8BPP)
/* The last tile, which BG1's flat map shows in the rows around the picture. */
#define BG1_EDGE_TILE				TILE_NUMBER_MASK
/* BG1's horizontal scroll at rest, which centres the 256-pixel picture on the
   screen (battle_background.c's BACKGROUND_PAN_X, in whole pixels). */
#define BACKGROUND_SCROLL_X			((BG_SCREEN_WIDTH * 8 - DISPLAY_WIDTH) / 2)

/* Fades the screen to black over sixteen frames as every background's mosaic
   blocks grow wider, then shows no layer. */
Sint32 btTransitionRunMosaicFadeOut(void)
{
	Uint16			 bg0cnt;
	Uint16			 bg1cnt;
	Uint16			 bg2cnt;
	Uint16			 bg3cnt;
	Sint32			 mosaicSize;
	volatile Uint16 *registerCursor;

	syIORead16(&REG_BG0CNT, &bg0cnt);
	REG_BG0CNT = bg0cnt | BGCNT_MOSAIC;
	registerCursor = &REG_BG0CNT;
	syIORead16(++registerCursor, &bg1cnt);
	*registerCursor = bg1cnt | BGCNT_MOSAIC;
	syIORead16(++registerCursor, &bg2cnt);
	*registerCursor = bg2cnt | BGCNT_MOSAIC;
	syIORead16(++registerCursor, &bg3cnt);
	*registerCursor = bg3cnt | BGCNT_MOSAIC;
	registerCursor += &REG_BLDCNT - &REG_BG3CNT;
	*registerCursor = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
		BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_BG1 |
		BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD;

	syScreenFadeToBlack(16);
	for(mosaicSize = 0; mosaicSize <= MOSAIC_SIZE_MAX; mosaicSize++) {
		/* Four random numbers a frame, drawn only to move the generator on. */
		syRandom();
		syRandom();
		syRandom();
		syRandom();
		/* The backgrounds' and sprites' blocks grow wider, staying a line
		   tall. */
		REG_MOSAIC = (mosaicSize << 8) | mosaicSize;
		sySynch(1);
	}

	REG_DISPCNT = DISPCNT_MODE_1;
	sySynch(4);
	REG_BG0CNT = bg0cnt;
	REG_BG1CNT = bg1cnt;
	REG_BG2CNT = bg2cnt;
	REG_BG3CNT = bg3cnt;
	return 0;
}

/* Builds the zoomed background's 32 by 32 affine map, in which the background
   picture's two halves come from character blocks 2 and 3. */
void btTransitionBuildAffineTilemap(void *tilemap)
{
	Uint32 *words = tilemap;
	Uint32	tiles;
	Uint32	i;

	tiles = AFFINE_MAP_ENTRIES(0, 1, 2, 3);
	for(i = 0; i < BACKGROUND_UPPER_TILES / 4; i++) {
		*words++ = tiles;
		tiles += AFFINE_MAP_ENTRIES(4, 4, 4, 4);
	}
	tiles = AFFINE_MAP_ENTRIES(0, 1, 2, 3);
	for(i = 0; i < BACKGROUND_LOWER_TILES / 4; i++) {
		*words++ = tiles;
		tiles += AFFINE_MAP_ENTRIES(4, 4, 4, 4);
	}
	syWordsFill(words, BACKGROUND_AFFINE_MAP_BYTES - BACKGROUND_TILES, -1);
}

/* Builds BG1's flat map of the same picture: its tiles in rows 6 to 20, from
   screen line 16 at BATTLE_BACKGROUND_SCROLL_Y, and the last tile in the rows
   around them. */
void btTransitionBuildFlatTilemap(void *tilemap)
{
	Uint32 *pairs = tilemap;
	Uint32	pair;
	Uint32	i;

	syWordsFill(pairs, 4 * MAP_ROW_BYTES, -1);
	pairs += 4 * MAP_ROW_WORDS;
	syWordsFill(
		pairs, 2 * MAP_ROW_BYTES, MAP_ENTRY_PAIR(BG1_EDGE_TILE, BG1_EDGE_TILE));
	pairs += 2 * MAP_ROW_WORDS;
	pair = MAP_ENTRY_PAIR(
		BACKGROUND_BG1_FIRST_TILE, BACKGROUND_BG1_FIRST_TILE + 1);
	for(i = 0; i < BACKGROUND_TILE_ROWS * MAP_ROW_WORDS; i++) {
		*pairs++ = pair;
		pair += MAP_ENTRY_PAIR(2, 2);
	}
	syWordsFill(pairs, 10 * MAP_ROW_BYTES,
		MAP_ENTRY_PAIR(BG1_EDGE_TILE, BG1_EDGE_TILE));
}

/* Shows the zoomed background's finished page each VBlank. */
void btBackgroundShowZoomedPage(void)
{
	BATTLE_BACKGROUND_STATE *view =
		HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
	BATTLE_AFFINE_HDMA *scanlines;
	Uint16			   *line;

	if(view->mode != BATTLE_BACKGROUND_ZOOMED)
		return;
	scanlines = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SCANLINES);
	line = scanlines->lines[scanlines->page];
	IO_WRITE16(line[0], &REG_BG2CNT);
	syDMAXCopy(
		line + 1, &REG_BG2CNT, DMA_HBLANK_HALFWORD_PER_LINE, &REG_DMA0SAD);
	syDMAXCopy(&scanlines->affine, &REG_BG2PA, DMA_COPY_WORDS(4), REG_DMA3);
}

/* Unused: steps the curtain's solid tile through eight variants, a new one
   every four frames. XXX (no evidence): which transition used it; nothing in
   either game calls it, and The Lost Age keeps an unused copy too. */
void btTransitionUploadTileVariant(void)
{
	Uint32 frame = *BattleEntryTimer - 1;
	if(frame < TRANSITION_CURTAIN_VARIANTS * TRANSITION_VARIANT_FRAMES)
		syDMAXCopy(BattleTransition_CurtainVariants +
				frame / TRANSITION_VARIANT_FRAMES * TILE_SIZE_4BPP,
			CURTAIN_TILE(0), DMA_COPY_WORDS(8), REG_DMA3);
}

/* Advances the curtain opening by a frame: BG0 slides into place as the camera
   turns round to the party and zooms out. */
void btTransitionAdvanceOpening(void)
{
	void		 **slots = &HeapSlots[HEAP_SLOT_BATTLE_ENTRY_TIMER];
	Uint32		  *timer = slots[0];
	Uint32		   frame = *timer;
	BATTLE_CAMERA *camera =
		HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_ENTRY_TIMER, HEAP_SLOT_CAMERA);
	/* BG0's top band rolls down from frame 20 to frame 52. */
	Sint32 scroll = 52 - frame;

	if(scroll > TRANSITION_BG0_BAND)
		scroll = TRANSITION_BG0_BAND;
	if(scroll < 0)
		scroll = 0;
	BgScrollWork[0].y = scroll;
	if(frame <= TRANSITION_OPEN_FRAMES)
		camera->yaw = frame * TRANSITION_TURN_SPEED + TRANSITION_START_YAW;
	frame = ++*timer;
	if(frame <= TRANSITION_OPEN_FRAMES)
		btCameraSetView(
			0, 0, 0, BATTLE_ZOOM_NORMAL + TRANSITION_OPEN_FRAMES - frame);
	else
		btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
}

/* Draws the curtain opening from the middle of the screen, one row up and one
   down every eight frames. */
void btTransitionDrawRows(void)
{
	Uint32	column;
	Sint32	transitionFrame;
	Sint32	tileEntry;
	Uint32	tileRow;
	Uint16 *tilemap;

	transitionFrame = *BattleEntryTimer;
	if(transitionFrame >= 0 && transitionFrame < TRANSITION_OPEN_FRAMES) {
		/* Over its eight frames a row steps through the seven edge tiles to the
		   clear one. */
		tileEntry = (7 & transitionFrame) + TRANSITION_CURTAIN_ENTRY(1);
		tileRow = TRANSITION_CURTAIN_MIDDLE_ROW - transitionFrame / 8;
		tilemap = (Uint16 *)BG_SCREEN_BLOCK(TRANSITION_CURTAIN_SCREEN) +
			tileRow * BG_SCREEN_WIDTH;
		for(column = 0; column != BG_SCREEN_WIDTH; column++)
			tilemap[column] = tileEntry;

		/* The lower half: the same tile flipped vertically. */
		tileEntry |= TILE_VFLIP;
		tileRow = transitionFrame / 8 + TRANSITION_CURTAIN_MIDDLE_ROW;
		if(tileRow <= TRANSITION_CURTAIN_LAST_ROW) {
			tilemap = (Uint16 *)BG_SCREEN_BLOCK(TRANSITION_CURTAIN_SCREEN) +
				tileRow * BG_SCREEN_WIDTH;
			for(column = 0; column != BG_SCREEN_WIDTH; column++)
				tilemap[column] = tileEntry;
		}
	}
}

/* Clears BG0's scroll from the line the interrupt fires on. */
void btTransitionClearBg0Vofs(void)
{
	IO_WRITE16(0, &REG_BG0VOFS);
}

/* Opens a battle: the curtain opening, or for the Fusion Dragon the storm on
   the Venus Lighthouse aerie; then announces the enemies. */
void btTransitionRun(Sint32 enemyCount, Sint32 encounterId)
{
	volatile Uint32			 fill;
	BATTLE_STAGING_STATE	 state;
	Uint16					 unitIds[UNIT_MAX];
	Uint16					 visibleUnitIds[UNIT_MAX];
	BATTLE_BACKGROUND_STATE *view;
	Sint32					*timer;
	Uint16					*tilemap;
	Sint32					 tile;
	Uint32					 index;
	Uint32					 column;
	Sint32					 count;
	Sint32					 id;
	BATTLE_OBJECT			*battleObject;
	BATTLE_STATE			*battle;
	Sint32					 visibleCount;

	view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
	timer = syHeapSlotAlloc(HEAP_SLOT_BATTLE_ENTRY_TIMER, sizeof(*timer));
	if(encounterId != FORMATION_FUSION_DRAGON) {
		/* The curtain's seven edge tiles. */
		syDMAXCopy(BattleTransition_CurtainTiles, CURTAIN_TILE(1),
			DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransition_CurtainTiles + 1 * TILE_SIZE_4BPP,
			CURTAIN_TILE(2), DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransition_CurtainTiles + 2 * TILE_SIZE_4BPP,
			CURTAIN_TILE(3), DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransition_CurtainTiles + 3 * TILE_SIZE_4BPP,
			CURTAIN_TILE(4), DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransition_CurtainTiles + 4 * TILE_SIZE_4BPP,
			CURTAIN_TILE(5), DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransition_CurtainTiles + 5 * TILE_SIZE_4BPP,
			CURTAIN_TILE(6), DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransition_CurtainTiles + 6 * TILE_SIZE_4BPP,
			CURTAIN_TILE(7), DMA_COPY_WORDS(8), REG_DMA3);
		view->secondMode = BATTLE_BACKGROUND_FIXED;
		view->mode = BATTLE_BACKGROUND_FIXED;
		view->busy = 0;
		/* The curtain on BG1, behind BG0. */
		REG_DISPCNT = DISPCNT_MODE_1;
		DMA_FILL32_CELL(fill, PIXELS_4BPP(TRANSITION_CURTAIN_COLOR),
			CURTAIN_TILE(0), TILE_SIZE_4BPP);
		DMA_FILL32_CELL(fill, 0, CURTAIN_TILE(8), TILE_SIZE_4BPP);
		REG_BG1CNT =
			BGCNT_CHAR_BASE(1) | BGCNT_SCREEN_BASE(TRANSITION_CURTAIN_SCREEN);
		REG_BG0CNT |= BGCNT_PRIORITY(2);
		view->mode = BATTLE_BACKGROUND_ZOOMED;
		/* A closed curtain: solid rows 0 to TRANSITION_CURTAIN_LAST_ROW, clear
		   below. */
		tilemap = (Uint16 *)BG_SCREEN_BLOCK(TRANSITION_CURTAIN_SCREEN);
		for(index = 0; index < BG_SCREEN_WIDTH; index++) {
			if(index <= TRANSITION_CURTAIN_LAST_ROW)
				tile = TRANSITION_CURTAIN_ENTRY(0);
			else
				tile = TRANSITION_CURTAIN_ENTRY(8);
			for(column = 0; column < BG_SCREEN_WIDTH; column++)
				*tilemap++ = tile;
		}
		{
			Sint32 start = 0;

			BgScrollWork[0].y = TRANSITION_BG0_BAND;
			BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
			BgScrollWork[1].x = BACKGROUND_SCROLL_X;
			sySynch(1);
			REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
			REG_WIN0V = WIN_RANGE(0, 136);
			REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
			REG_WIN1V = WIN_RANGE(0, 136);
			REG_WININ = BATTLE_WININ;
			REG_WINOUT = BATTLE_WINOUT;
			syIOWriteQueueStore16((Uint32)&REG_DISPCNT,
				DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
					DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON |
					DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
			/* The opening runs from the events while the status window opens
			   and the encounter is announced. */
			btCameraSetView(
				0, 0, 0, BATTLE_ZOOM_NORMAL + TRANSITION_OPEN_FRAMES);
			*timer = start;
			sySetEvent(btTransitionAdvanceOpening, EVENT_PASS_FRAME);
			sySetEvent(btTransitionDrawRows, EVENT_PASS_VBLANK);
			syIntFunction(
				IRQ_VCOUNT, TRANSITION_BG0_BAND, btTransitionClearBg0Vofs);
			BgScrollWork[0].y = TRANSITION_BG0_BAND;
			sySynch(1);
			mnPartyStatCreateWindowStub(BattleStateWork->partyStat);
			sySynch(20);
			syIOWriteQueueClearBits16((Uint32)&REG_BG0CNT, BGCNT_PRIORITY(2));
			syIOWriteQueueSetBits16((Uint32)&REG_BG0CNT, BGCNT_PRIORITY(0));
			btIntroAnnounceEncounter(enemyCount);
			syCloseEvent(btTransitionAdvanceOpening);
			syCloseEvent(btTransitionDrawRows);
			BgScrollWork[0].y = start;
			syIntFunction(IRQ_VCOUNT, 0, 0);
		}
	} else {
		/* The storm: everyone but the Fusion Dragon drawn smaller. */
		battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
		view->secondMode = BATTLE_BACKGROUND_FIXED;
		view->busy = 0;
		count = btUnitListIds(BATTLE_SIDE_MASK_BOTH, (Uint16 *)unitIds);
		for(index = 0; index != count; index++) {
			/* BUG: unitIds is never read; the index is taken as a unit id, and
			   a gap among the party's ids makes the scale store go near address
			   0. */
			id = index + FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
			if((Sint32)index <= ALLY_MAX)
				id = index;
			battleObject = btObjectGet(id);
			if(gmUnitGetStub(id)->id != ENEMY_FUSION_DRAGON)
				battleObject->scale = toFIXED(7) / 10;
		}
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
			DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_WIN0_ON |
				DISPCNT_WIN1_ON,
			IO_WRITE_HALFWORD);
		sySynch(1);
		{
			Sint32 start = 0;

			/* The enemies alone take part in the shattering opening. */
			battle->bg = RESOURCE_VENUS_LIGHTHOUSE_AERIE_STORM_BACKDROP;
			state.eff.tgtCnt =
				btUnitListIds(BATTLE_SIDE_MASK_ENEMIES, state.eff.tgts);
			state.eff.tgts[state.eff.tgtCnt] = BATTLE_UNIT_LIST_END;
			btObjectSpawnObjectsForList(state.eff.tgts, 0);
			btEffectRunShatteringOpeningStub(&state.eff);
			btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
			*timer = start;
		}
		syIntFunction(
			IRQ_VCOUNT, TRANSITION_BG0_BAND, btTransitionClearBg0Vofs);
		sySynch(1);
		sySynch(20);
		mnPartyStatCreateWindowStub(BattleStateWork->partyStat);
		syIOWriteQueueClearBits16((Uint32)&REG_BG0CNT, BGCNT_PRIORITY(2));
		syIOWriteQueueSetBits16((Uint32)&REG_BG0CNT, BGCNT_PRIORITY(0));
		/* Everyone appears, the party fading in. */
		REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
		visibleCount =
			btUnitListIds(BATTLE_SIDE_MASK_BOTH, (Uint16 *)visibleUnitIds);
		visibleUnitIds[visibleCount] = BATTLE_UNIT_LIST_END;
		btObjectSpawnObjectsForList(visibleUnitIds, 0);
		visibleCount =
			btUnitListIds(BATTLE_SIDE_MASK_PARTY, (Uint16 *)visibleUnitIds);
		for(index = 0; index != visibleCount; index++)
			btHighlightSetSprMode(
				visibleUnitIds[index], SPRITE_SEMI_TRANSPARENT);
		for(index = 0; index != BLEND_WEIGHT_MAX; index++) {
			REG_BLDALPHA = BLEND_ALPHA(index, BLEND_WEIGHT_MAX);
			sySynch(1);
		}
		for(index = 0; index != visibleCount; index++)
			btHighlightSetSprMode(visibleUnitIds[index], SPRITE_OPAQUE);
		btIntroAnnounceEncounter(enemyCount);
		BgScrollWork[0].y = 0;
		sySynch(1);
		syIntFunction(IRQ_VCOUNT, 0, 0);
	}
	syIntFunction(IRQ_VCOUNT, 0, 0);
	REG_BG1CNT = BATTLE_BG1CNT;
	sySynch(1);
	REG_BG1CNT = BATTLE_BG1CNT;
	REG_BG0CNT &= ~BGCNT_PRIORITY(2);
	BgScrollWork[1].x = BACKGROUND_SCROLL_X;
	REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
		DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	syHeapSlotRelease(HEAP_SLOT_BATTLE_ENTRY_TIMER);
}

#undef TRANSITION_CURTAIN_FIRST_TILE
#undef TRANSITION_CURTAIN_ENTRY
#undef TRANSITION_CURTAIN_COLOR
#undef TRANSITION_CURTAIN_SCREEN
#undef TRANSITION_CURTAIN_MIDDLE_ROW
#undef TRANSITION_CURTAIN_LAST_ROW
#undef TRANSITION_OPEN_FRAMES
#undef TRANSITION_CURTAIN_VARIANTS
#undef TRANSITION_VARIANT_FRAMES
#undef TRANSITION_BG0_BAND
#undef TRANSITION_TURN_SPEED
#undef TRANSITION_START_YAW
#undef MOSAIC_SIZE_MAX
#undef MAP_ROW_WORDS
#undef AFFINE_MAP_ENTRIES
#undef BACKGROUND_TILE_ROWS
#undef BACKGROUND_TILES
#undef BACKGROUND_UPPER_TILES
#undef BACKGROUND_LOWER_TILES
#undef BACKGROUND_AFFINE_MAP_BYTES
#undef BACKGROUND_BG1_FIRST_TILE
#undef BG1_EDGE_TILE
#undef BACKGROUND_SCROLL_X

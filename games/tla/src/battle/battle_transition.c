/* The battle's screen transitions: the mosaic fade to black, and the rows of
   tiles that close in, with the HBlank scroll they run on. Part of battle.c. */

/* The curtain's first tile in BG character block 1. */
#define TRANSITION_CURTAIN_FIRST_TILE 128
/* The curtain's map entries: its tile n, WINDOW_PALETTE. */
#define TRANSITION_CURTAIN_ENTRY(n) \
	(WINDOW_TILE(TRANSITION_CURTAIN_FIRST_TILE) + (n))
/* The screen block of the curtain's map, and its middle row, from which it
   opens a row up and a row down every eight ticks for
   TRANSITION_OPEN_TICKS. */
#define TRANSITION_CURTAIN_SCREEN	  12
#define TRANSITION_CURTAIN_MIDDLE_ROW 13
#define TRANSITION_CURTAIN_LAST_ROW	  20
#define TRANSITION_OPEN_TICKS		  80
/* The curtain's tile n in BG character block 1. */
#define CURTAIN_TILE(n) \
	(BG_CHAR_BLOCK(1) + (TRANSITION_CURTAIN_FIRST_TILE + (n)) * TILE_SIZE_4BPP)
/* The closed curtain's colour, which fills its first tile. */
#define TRANSITION_CURTAIN_COLOR	3
/* Eight variants of the curtain's first tile, a new one every four ticks
   (btTransitionUploadTileVariant). */
#define TRANSITION_CURTAIN_VARIANTS 8
#define TRANSITION_VARIANT_TICKS	4
/* BG0's top 32 lines, which roll down into place as the curtain opens: BG0
   starts scrolled up by as many, and the VCount interrupt at the band's bottom
   clears the scroll for the lines below. */
#define TRANSITION_BG0_BAND			32
/* The camera turns 360 of TRIG_TURN's 65,536 a tick (about 2 degrees) as the
   curtain opens, ending at DEGtoANG(45) as the zoom ends. */
#define TRANSITION_TURN_SPEED		360
#define TRANSITION_START_YAW \
	(TRIG_TURN + DEGtoANG(45) - TRANSITION_OPEN_TICKS * TRANSITION_TURN_SPEED)

/* REG_MOSAIC's largest block size, 16 pixels: each of its sizes is a four-bit
   field holding the size less one. */
#define MOSAIC_SIZE_MAX 15

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
/* The picture's first tile on BG1 (BATTLE_BG1CNT), numbered from BG1's
   character base 0: character block 2. */
#define BACKGROUND_BG1_FIRST_TILE	(2 * BG_CHAR_SIZE / TILE_SIZE_8BPP)
/* The last tile, which BG1's flat map shows in the rows around the picture: it
   lies in the last row of the map's own screen block, which btBackgroundLoad
   leaves clear. */
#define BG1_EDGE_TILE				TILE_NUMBER_MASK

#include "battle/transition_mosaic_fade.c"

/* Builds BG1's flat map of the same picture, in screen block 31: four rows of
   blank entries, two of BG1_EDGE_TILE, the picture's tiles in order on rows 6
   to 20, then BG1_EDGE_TILE down to the last row, which btBackgroundLoad
   clears. */
void btTransitionBuildFlatTilemap(Sint32 *dst)
{
	Sint32 entry;
	Uint32 idx;

	syWordsFill(dst, 4 * MAP_ROW_BYTES, -1);
	dst += 4 * MAP_ROW_WORDS;
	syWordsFill(dst, 2 * MAP_ROW_BYTES, WINDOW_TILE_PAIR(BG1_EDGE_TILE));
	entry = TILE_PAIR(BACKGROUND_BG1_FIRST_TILE);
	dst += 2 * MAP_ROW_WORDS;
	idx = 0;
	do {
		idx++;
		*dst++ = entry;
		entry += TILE_PAIR_STEP;
	} while(idx < BACKGROUND_TILE_ROWS * MAP_ROW_WORDS);
	syWordsFill(dst, 10 * MAP_ROW_BYTES, WINDOW_TILE_PAIR(BG1_EDGE_TILE));
}

/* Copies this tick's BG2CNT word and starts the HBlank DMA of the BG2 lines
   and the affine block, while the background view is in mode 2. */
void btBackgroundShowZoomedPage(void)
{
	BATTLE_AFFINE_HDMA *records;
	Uint16			   *record;

	if((RamHeapSlots->battleBg)->mode == BATTLE_BACKGROUND_ZOOMED) {
		records = RamHeapSlots->battleScanlines;
		record = records->lines[records->page];
		syIOWrite16(record[0], &REG_BG2CNT);
		syDMAXCopy(record + 1, (void *)&REG_BG2CNT,
			DMA_HBLANK_HALFWORD_PER_LINE, &REG_DMA0SAD);
		syDMAXCopy(
			&records->affine, (void *)&REG_BG2PA, DMA_COPY_WORDS(4), REG_DMA3);
	}
}

/* Unused: steps the curtain's solid tile through its variants
   (BattleTransitionCurtainVariants) while the transition timer runs from 1. */
void btTransitionUploadTileVariant(void)
{
	Uint32 variant = *(Sint32 *)RamHeapSlots->battleEntryTmr - 1;

	if(variant < TRANSITION_CURTAIN_VARIANTS * TRANSITION_VARIANT_TICKS)
		syDMAXCopy(BattleTransitionCurtainVariants +
				variant / TRANSITION_VARIANT_TICKS * TILE_SIZE_4BPP,
			CURTAIN_TILE(0), DMA_COPY_WORDS(TILE_SIZE_4BPP / 4), REG_DMA3);
}

/* Advances the curtain opening by a tick: BG0 slides into place as the camera
   turns round to the party and zooms out. */
void btTransitionAdvanceOpening(void)
{
	Sint32		   scroll;
	BATTLE_CAMERA *camera;
	Uint32		  *tmr;
	Uint32		   frm;
	Uint32		   next;

	tmr = (Uint32 *)RamHeapSlots->battleEntryTmr;
	frm = *tmr;
	camera = (BATTLE_CAMERA *)RamHeapSlots->cameraState;
	/* BG0's top band rolls down from tick 20 to tick 52. */
	scroll = 52 - frm;
	if(scroll > TRANSITION_BG0_BAND)
		scroll = TRANSITION_BG0_BAND;
	if(scroll < 0)
		scroll = 0;
	BgScrollWork[0].y = scroll;
	if(frm <= TRANSITION_OPEN_TICKS)
		camera->yaw = frm * TRANSITION_TURN_SPEED + TRANSITION_START_YAW;
	next = (*tmr = *tmr + 1);
	if(next <= TRANSITION_OPEN_TICKS) {
		btCameraSetView(
			0, 0, 0, BATTLE_ZOOM_NORMAL + TRANSITION_OPEN_TICKS - next);
		return;
	}
	btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
}

/* Draws the curtain opening from the middle of the screen, one row up and one
   down every eight ticks. */
void btTransitionDrawRows(void)
{
	Uint32	column;
	Sint32	transitionFrm;
	Sint32	tileEntry;
	Uint32	tileRow;
	Uint16 *tilemap;

	transitionFrm = *(Sint32 *)RamHeapSlots->battleEntryTmr;
	if(transitionFrm >= 0 && transitionFrm < TRANSITION_OPEN_TICKS) {
		tileEntry = (7 & transitionFrm) + TRANSITION_CURTAIN_ENTRY(1);
		tileRow = TRANSITION_CURTAIN_MIDDLE_ROW - transitionFrm / 8;
		tilemap = (Uint16 *)BG_SCREEN_BLOCK(TRANSITION_CURTAIN_SCREEN) +
			tileRow * BG_SCREEN_WIDTH;
		for(column = 0; column != BG_SCREEN_WIDTH; column++)
			tilemap[column] = tileEntry;

		/* The lower half: the same tile flipped vertically. */
		tileEntry |= TILE_VFLIP;
		tileRow = transitionFrm / 8 + TRANSITION_CURTAIN_MIDDLE_ROW;
		if(tileRow <= TRANSITION_CURTAIN_LAST_ROW) {
			tilemap = (Uint16 *)BG_SCREEN_BLOCK(TRANSITION_CURTAIN_SCREEN) +
				tileRow * BG_SCREEN_WIDTH;
			for(column = 0; column != BG_SCREEN_WIDTH; column++)
				tilemap[column] = tileEntry;
		}
	}
}

/* Clears BG0's scroll from the line the VCount interrupt fires on. */
void btTransitionResetBg0Scroll(void)
{
	syIOWrite16(0, &REG_BG0VOFS);
}

/* Opens the battle against formation kind with enemyCnt enemies: the curtain
   opening, then the enemies announced (Karst and Agatio's battles wait a second
   instead); for the Doom Dragon's the shattering opening over the Serpent's
   stars. */
void btTransitionRun(Sint32 enemyCnt, Sint32 kind)
{
	BATTLE_PRESENTATION_STATE state;
	Uint16					  owners[UNIT_MAX];
	Uint16					  shown[UNIT_MAX];
	Sint32					 *tmr;
	BATTLE_STATE			 *battle;
	BATTLE_BACKGROUND_STATE	 *view;
	HEAP_SLOTS				 *heap;
	Sint32					  ownerCnt;
	Sint32					  cnt;
	Uint32					  i;

	heap = RamHeapSlots;
	view = heap->battleBg;
	tmr = syHeapSlotAlloc(HEAP_SLOT_BATTLE_ENTRY_TIMER, 4);
	if((Uint32)(kind - FORMATION_DOOM_DRAGON_A) >
		FORMATION_DOOM_DRAGON_C - FORMATION_DOOM_DRAGON_A) {
		Sint32 y;

		syDMAXCopy(BattleTransitionCurtainTiles, CURTAIN_TILE(1),
			DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransitionCurtainTiles + 1 * TILE_SIZE_4BPP,
			CURTAIN_TILE(2), DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransitionCurtainTiles + 2 * TILE_SIZE_4BPP,
			CURTAIN_TILE(3), DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransitionCurtainTiles + 3 * TILE_SIZE_4BPP,
			CURTAIN_TILE(4), DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransitionCurtainTiles + 4 * TILE_SIZE_4BPP,
			CURTAIN_TILE(5), DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransitionCurtainTiles + 5 * TILE_SIZE_4BPP,
			CURTAIN_TILE(6), DMA_COPY_WORDS(8), REG_DMA3);
		syDMAXCopy(BattleTransitionCurtainTiles + 6 * TILE_SIZE_4BPP,
			CURTAIN_TILE(7), DMA_COPY_WORDS(8), REG_DMA3);
		view->bg1ScrollMode = BATTLE_BACKGROUND_FIXED;
		view->busy = 0;
		view->mode = BATTLE_BACKGROUND_FIXED;
		REG_DISPCNT = DISPCNT_MODE_1;
		IWRAM_FILL_WORDS(CURTAIN_TILE(0), TILE_SIZE_4BPP,
			PIXELS_4BPP(TRANSITION_CURTAIN_COLOR));
		IWRAM_FILL_WORDS(CURTAIN_TILE(8), TILE_SIZE_4BPP, 0);
		REG_BG1CNT =
			BGCNT_CHAR_BASE(1) | BGCNT_SCREEN_BASE(TRANSITION_CURTAIN_SCREEN);
		REG_BG0CNT |= BGCNT_PRIORITY(2);
		view->mode = BATTLE_BACKGROUND_ZOOMED;
		/* A closed curtain: solid rows 0 to TRANSITION_CURTAIN_LAST_ROW, clear
		   below. */
		{
			Uint16 *map = (Uint16 *)BG_SCREEN_BLOCK(TRANSITION_CURTAIN_SCREEN);

			for(i = 0; i < BG_SCREEN_WIDTH; i++) {
				Uint32 tile;
				Uint32 column;

				if(i <= TRANSITION_CURTAIN_LAST_ROW)
					tile = TRANSITION_CURTAIN_ENTRY(0);
				else
					tile = TRANSITION_CURTAIN_ENTRY(8);
				for(column = 0; column < BG_SCREEN_WIDTH; column++)
					*map++ = tile;
			}
		}
		/* BG0 starts scrolled up by its band, and BG1 at the same 32 lines,
		   BATTLE_BACKGROUND_SCROLL_Y. */
		y = TRANSITION_BG0_BAND;
		BgScrollWork[1].x = 8;
		BgScrollWork[0].y = y;
		BgScrollWork[1].y = y;
		sySynch(1);
		REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
		REG_WIN0V = WIN_RANGE(0, 136);
		REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
		REG_WIN1V = WIN_RANGE(0, 136);
		REG_WININ = WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 |
			WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
			WININ_WIN1_OBJ | WININ_WIN1_CLR;
		REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;
		syIOWriteQueueStore16((Uint32)&REG_DISPCNT,
			DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
				DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON |
				DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
		btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL + TRANSITION_OPEN_TICKS);
		*tmr = 0;
		sySetEvent(btTransitionAdvanceOpening, EVENT_PASS_TICK);
		sySetEvent(btTransitionDrawRows, EVENT_PASS_VBLANK);
		syIntFunction(
			IRQ_VCOUNT, TRANSITION_BG0_BAND, btTransitionResetBg0Scroll);
		BgScrollWork[0].y = y;
		sySynch(1);
		mnPartyStatCreateWindowStub(RamHeapSlots->battleState->partyStat);
		sySynch(20);
		syIOWriteQueueClearBits16((Uint32)&REG_BG0CNT, BGCNT_PRIORITY(2));
		syIOWriteQueueSetBits16((Uint32)&REG_BG0CNT, BGCNT_PRIORITY(0));
		/* Karst and Agatio's battles open with no announcement. */
		if((Uint32)(kind - FORMATION_KARST_AND_AGATIO_A) <=
				FORMATION_KARST_AND_AGATIO_C - FORMATION_KARST_AND_AGATIO_A ||
			kind == FORMATION_KARST_AND_AGATIO_VS_ISAAC)
			sySynch(60);
		else
			btIntroAnnounceEncounter(enemyCnt);
		syCloseEvent(btTransitionAdvanceOpening);
		syCloseEvent(btTransitionDrawRows);
		BgScrollWork[0].y = 0;
		syIntFunction(IRQ_VCOUNT, 0, 0);
	} else {
		battle = heap->battleState;
		view->bg1ScrollMode = BATTLE_BACKGROUND_FIXED;
		view->busy = 0;
		/* Against the Doom Dragon the party, reserves too, is drawn at seven
		   tenths of its size for the rest of the battle; nothing sets it
		   back. */
		ownerCnt = btPartyPrepareMembers(owners);
		ownerCnt += btPartyPrepareReserveOwners(owners + ownerCnt);
		for(i = 0; i != ownerCnt; i++) {
			Sint32		   id = owners[i];
			BATTLE_OBJECT *battleObject = btObjectGet(id);
			Uint16		   enemyId = syUnitGet(id)->enemyId;

			if(enemyId != ENEMY_FUSION_DRAGON &&
				enemyId != ENEMY_DOOM_DRAGON_A &&
				enemyId != ENEMY_DOOM_DRAGON_E &&
				enemyId != ENEMY_DOOM_DRAGON_H)
				battleObject->scale = toFIXED(7) / 10;
		}
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
			DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_WIN0_ON |
				DISPCNT_WIN1_ON,
			IO_WRITE_HALFWORD);
		sySynch(1);
		{
			Sint32	background = RESOURCE_SERPENT_STARS_BACKDROP;
			Uint16 *slot = &battle->bg;
			Sint32	start = 0;

			*slot = background;
			/* The enemies alone take part in the shattering opening. */
			state.cnt =
				btUnitListIds(BATTLE_SIDE_MASK_ENEMIES, (Uint16 *)state.actors);
			state.actors[state.cnt] = BATTLE_UNIT_LIST_END;
			btActorSpawnObjectsForList(state.actors, 0);
			btEffectRunShatteringOpeningStub(&state);
			btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
			*tmr = start;
		}
		syIntFunction(
			IRQ_VCOUNT, TRANSITION_BG0_BAND, btTransitionResetBg0Scroll);
		sySynch(1);
		sySynch(20);
		mnPartyStatCreateWindowStub(RamHeapSlots->battleState->partyStat);
		syIOWriteQueueClearBits16((Uint32)&REG_BG0CNT, BGCNT_PRIORITY(2));
		syIOWriteQueueSetBits16((Uint32)&REG_BG0CNT, BGCNT_PRIORITY(0));
		REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
		cnt = btUnitListIds(BATTLE_SIDE_MASK_BOTH, shown);
		shown[cnt] = BATTLE_UNIT_LIST_END;
		btActorSpawnObjectsForList((Sint16 *)shown, 0);
		cnt = btUnitListIds(BATTLE_SIDE_MASK_PARTY, shown);
		for(i = 0; i != cnt; i++)
			btHighlightSetSprMode(shown[i], SPRITE_SEMI_TRANSPARENT);
		for(i = 0; i != BLEND_WEIGHT_MAX; i++) {
			REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
			sySynch(1);
		}
		for(i = 0; i != cnt; i++)
			btHighlightSetSprMode(shown[i], SPRITE_OPAQUE);
		btIntroAnnounceEncounter(enemyCnt);
		BgScrollWork[0].y = 0;
		sySynch(1);
		syIntFunction(IRQ_VCOUNT, 0, 0);
	}
	syIntFunction(IRQ_VCOUNT, 0, 0);
	REG_BG1CNT = BATTLE_BG1CNT;
	sySynch(1);
	REG_BG1CNT = BATTLE_BG1CNT;
	REG_BG0CNT &= ~BGCNT_PRIORITY(2);
	BgScrollWork[1].x = 8;
	REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
		DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	syHeapSlotRelease(HEAP_SLOT_BATTLE_ENTRY_TIMER);
}

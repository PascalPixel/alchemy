/* The Title (SceneId_Title): the screens between scenes that are not on a map,
   chosen by entrance.

   - TITLE_AFTER_RUN, at power-on: the publisher's and Camelot's logos and the
	 title, looping until a key is pressed, then the File Screen.
   - TITLE_FROM_VALE_PLAZA, at the prologue's end: the title held while the
	 years pass (MUS_THE_PASSAGE_OF_TIME), then Vale, the party grown up.
   - TITLE_FROM_LEMURIAN_SHIP, once the party sails away: the staff credits,
	 then the File Screen.
   - TITLE_FROM_WORLD_MAP, after the ending on Jenna and Kraden's island: the
	 Golden Sun logo with "To Be Continued" written in under it from the left,
	 held a minute or until a key, then back to the logos. */

#include "scene/title.h"
#include "resource_ids.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "menu/title_intro.h"
#include "sound/sound_driver_symbols.h"

extern Uint32	   TitleExits[];
extern ACTOR_PLACE TitleActorPlacements[];
extern CUE		   TitleCues[];
extern Sint16	   TitleVramBlock;

static void scTitleRevealScreen(Sint32 unused);
static void scTitleLoadBackground(void);

/* The scene tables fdModeRun reads; the title has no exits, actors or cues. */
const SCENE_ENTRANCE *scGetEntrances(void)
{
	return Screen_Entrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return TitleExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return TitleActorPlacements;
}

const CUE *scGetCues(void)
{
	return TitleCues;
}

/* Plays the screens the entrance asks for, then sets the scene that follows. */
Sint32 scInit(void)
{
	Sint32 waitFrames;

	if(MainRoutineStateWork.entrance == TITLE_FROM_WORLD_MAP) {
		/* "To Be Continued": after 120 frames the logo waits up to 3600 frames
		   (a minute) for a key press. */
		fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
		sySndPlay(MUS_TO_BE_CONTINUED);
		scTitleRevealScreen(0);
		sySynch(120);
		for(waitFrames = 0; waitFrames < 3600 && PerPush == 0; waitFrames++)
			sySynch(1);
		fdScriptSetDestination(SCENE_TITLE, TITLE_AFTER_RUN);
		return 0;
	}
	if(MainRoutineStateWork.entrance == TITLE_FROM_LEMURIAN_SHIP) {
		sySndPlay(MUS_THE_BROKEN_SEAL_ENDING);
		/* The staff credits (mnCreditsRun). */
		syImportCreditsRun(0);
		sySndPlay(SOUND_FADE_OUT_BGM);
		syScreenFadeToBlack(60);
		syScreenFadeWait();
		fdScriptWait(240);
		sySndPlay(MUS_SILENCE);
		fdScriptSetDestination(SCENE_FILE_SCREEN, FILE_SCREEN_FROM_TITLE);
		return 0;
	}
	/* The hook is handed the module whose stub table the logo and intro calls
	   below go through, and does nothing with it (system/system_start_up.c).
	   XXX (no evidence): what the call once did; the stub is empty in every
	   edition, so only an earlier build of the game would show. */
	syDummyNoOperationHook(RESOURCE_TITLE_STUBS);
	if(MainRoutineStateWork.entrance == TITLE_AFTER_RUN) {
		for(;;) {
			sySndPlay(MUS_SILENCE);
			/* The publisher's logo, then the Camelot logo
			   (mnTitleShowCamelotLogo). */
			mnTitleShowPublisherLogo(0);
			syImportTitleShowCamelotLogo(0);
			/* With no save file (or no backup memory) there is no title
			   intro. */
			if(mnSaveScanFileSummaries() > 0) {
				sySndPlay(MUS_MAIN_THEME_TITLE);
				/* mnTitleShowTitleScreen with its prompt returns 0 when a
				   minute passes without A or Start: fade out and start again
				   from the logos. */
				if(mnTitleShowTitleScreen(1) == 0) {
					sySndPlay(SOUND_FADE_OUT_BGM);
					syScreenFadeToBlack(30);
					syScreenFadeWait();
					waitFrames = 0;
					if(PerData == 0) {
						do {
							sySynch(1);
							if(++waitFrames > 119)
								break;
						} while(PerData == 0);
					}
					continue;
				}
			}
			break;
		}
		fdScriptSetDestination(SCENE_FILE_SCREEN, FILE_SCREEN_FROM_SANCTUM);
	} else {
		/* TITLE_FROM_VALE_PLAZA: the intro without its prompt holds the title
		   five seconds. */
		sySndPlay(MUS_THE_PASSAGE_OF_TIME);
		mnTitleShowTitleScreen(0);
		gmPartyEndChildhood();
		fdScriptSetDestination(SCENE_VALE, VALE_FROM_TITLE);
		sySndPlay(SOUND_FADE_OUT_BGM);
	}
	sySndPlay(SOUND_FADE_OUT_BGM);
	syScreenFadeToBlack(30);
	syScreenFadeWait();
	fdScriptWait(60);
	sySndPlay(MUS_SILENCE);
	return 0;
}

/* The lettering's sprite palette. */
enum { LETTERING_PALETTE = 15 };

/* In the German, Spanish, French and Italian pictures, the lettering's rows at
   the bottom of the screen. */
enum { LETTERING_ROWS = 32 };

/* Loads the "To Be Continued" lettering's sprites and their palette. */
static void scTitleLoadSprs(Sint32 unused)
{
	Uint8			*decodedSprites;
	volatile Uint32 *dma3;
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
	Uint8 *encodedGraphics;

	decodedSprites = syHeapAllocEWRAM(DISPLAY_WIDTH * DISPLAY_HEIGHT);
	encodedGraphics =
		syResourceGetTableEntry(RESOURCE_TO_BE_CONTINUED_LETTERING);
	if(TitleVramBlock == -1)
		TitleVramBlock = syVRAMCacheFindFreeEntry();
	/* The resource opens with its colours, before its compressed data. */
	syDMAXCopy(encodedGraphics, (void *)OBJ_PLTT,
		DMA_COPY_WORDS(LOGO_PALETTE_COLORS / 2), REG_DMA3);
	encodedGraphics += LOGO_PALETTE_COLORS * 2;
	syFieldDecompressTaggedLZ(encodedGraphics, decodedSprites);
	/* The picture is a whole screen in 256 colours (to_be_continued_de.png and
	   the others). The lettering is its bottom LETTERING_ROWS rows: 120 sprites
	   of one 64-byte tile. The tiles of one colour above it are not loaded. */
	mnVRAMLoad(TitleVramBlock, DISPLAY_WIDTH * LETTERING_ROWS,
		decodedSprites + DISPLAY_WIDTH * (DISPLAY_HEIGHT - LETTERING_ROWS));
#else
	/* The decoded data opens with its 16-colour palette, then 40 4-bit tiles of
	   lettering. */
	decodedSprites = syHeapAllocEWRAM(PALETTE_COLORS * 2 + 40 * TILE_SIZE_4BPP);
	if(TitleVramBlock == -1)
		TitleVramBlock = syVRAMCacheFindFreeEntry();
	syFieldDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_TO_BE_CONTINUED_LETTERING),
		decodedSprites);
	syDMAXCopy(decodedSprites, (void *)&OBJ_PLTT_COLOR(LETTERING_PALETTE, 0),
		DMA_COPY_WORDS(PALETTE_COLORS / 2), REG_DMA3);
	mnVRAMLoad(TitleVramBlock, 40 * TILE_SIZE_4BPP,
		decodedSprites + PALETTE_COLORS * 2);
#endif
	dma3 = REG_DMA3;
	while(dma3[2] & DMA_BUSY)
		;
	syHeapRelease(decodedSprites);
}

/* The event that queues the "To Be Continued" lettering each frame, writing it
   in from the left a column every two frames, each new column flickering for
   its first few. */
static void scTitleUpdateSprReveal(void)
{
	Uint32	  *attributes;
	TITLE_SPR *sprite;
	Sint32	   tileIndex;
	Sint32	   column;
	Sint32	   revealAge;
	Sint32	   screenY;
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
	Sint32 screenX;
#endif
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
	Sint32 row;

	/* Four rows of 30 8-by-8 sprites across the screen at y 128 to 152, in 256
	   colours and semi-transparent. A 256-colour tile is two of the 32-byte
	   tile numbers the cache offset counts in. */
	sprite = TitleSprites;
	attributes = TitleSprites[0].attr;
	tileIndex = VramCacheEntries[TitleVramBlock].ofs >> 5;
	row = 0;
	screenY = 128;
	for(; row <= 3; row++) {
		for(column = 0; column <= 29; column++) {
#if FIX_POINTER_OUTSIDE_OBJECT
			/* Stay inside each sprite's three-word array instead of walking
			   into the next struct. */
			attributes = sprite->attr;
#endif
			*attributes++ = 0;
			*attributes++ = OBJ_X(column * 8) | screenY | OBJ_256_COLORS |
				OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
			*attributes++ = tileIndex;
			revealAge = TitleRevealFrame / 2 - column;
			if(revealAge < 0)
				revealAge = 0;
			if(revealAge <= 2 && (FrameCount & 1))
				revealAge = 0;
			if(revealAge != 0)
				syOAMAddSprAtDepth(sprite, OAM_DEPTH_FRONT);
			sprite++;
			tileIndex += 2;
		}
		screenY += 8;
	}
#else

	/* One row of 18 semi-transparent sprites of 8 by 16 pixels at y 136 and x
	   88 to 224. */
	sprite = TitleSprites;
	attributes = TitleSprites[0].attr;
	tileIndex = VramCacheEntries[TitleVramBlock].ofs >> 5;
	column = 0;
	screenY = 136;
	for(; column <= 17; column++) {
		screenX = 232 - (18 - column) * 8;
#if FIX_POINTER_OUTSIDE_OBJECT
		attributes = TitleSprites[column].attr;
#endif
		*attributes++ = 0;
		*attributes++ = OBJ_X(screenX) | screenY | OBJ_SIZE_8X16 |
			OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		*attributes++ = OBJ_PALETTE(LETTERING_PALETTE) | tileIndex;
		revealAge = TitleRevealFrame / 2 - column;
		if(revealAge < 0)
			revealAge = 0;
		if(revealAge <= 2 && (FrameCount & 1))
			revealAge = 0;
		if(revealAge != 0)
			syOAMAddSprAtDepth(sprite++, OAM_DEPTH_FRONT);
		tileIndex += 2;
	}
#endif
	TitleRevealFrame++;
}

/* Shows the Golden Sun logo for TITLE_FROM_WORLD_MAP: the logo fades up from
   black over the lettering being written in, then the screen opens. */
static void scTitleRevealScreen(Sint32 unused)
{
	Sint32 brightnessStep;

	scTitleLoadBackground();
	fdScriptWait(30);
	TitleRevealFrame = 0;
	scTitleLoadSprs(0);
	sySetFrameEvent(scTitleUpdateSprReveal, EVENT_PASS_FRAME);
	/* Show BG0, BG2 and the sprites. BG1 to BG3 start fully darkened, so the
	   logo on BG2 is black; the semi-transparent lettering is blended onto the
	   layers under it at full weight. */
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
		IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
		BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
			BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 |
			BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_BD,
		IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDY, BLEND_FADE_MAX, IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
		BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX), IO_WRITE_HALFWORD);
	fdScriptWait(120);
	for(brightnessStep = 0; brightnessStep <= BLEND_FADE_MAX;
		brightnessStep++) {
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDY, BLEND_FADE_MAX - brightnessStep,
			IO_WRITE_HALFWORD);
		sySynch(3);
	}
	FieldStateWork->transition = 0;
	FieldStateWork->transitionFrm = 1;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	FieldStateWork->transitionFrm = 60;
}

/* Loads the Golden Sun logo onto BG2 (title.h's LOGO_ map) with the screen
   faded to black. */
static void scTitleLoadBackground(void)
{
	Uint8		*encodedGraphics;
	Sint32		 resourceId;
	Uint16		*tilemap;
	Uint32		 screenX;
	Uint32		 screenY;
	Uint16		 tileIndex;
	SCROLL_PAIR *bgScroll;
	Sint32		 offscreenTile = OFFSCREEN_TILE;

	resourceId = RESOURCE_GOLDEN_SUN_LOGO;
	syScreenFadeToBlack(0);
	REG_BG2CNT = LOGO_BGCNT;
	BgScrollWork[2].y = 0;
	encodedGraphics = syResourceGetTableEntry(resourceId);
	syDMAXCopy(encodedGraphics, (void *)BG_PLTT,
		DMA_COPY_WORDS(LOGO_PALETTE_COLORS / 2), REG_DMA3);
	encodedGraphics += LOGO_PALETTE_COLORS * 2;
	syFieldDecompressTaggedLZ(encodedGraphics, Ram_MapGridMetatileBuffer);
	syDMAXCopy((void *)Ram_MapGridMetatileBuffer, LOGO_TILES,
		DMA_COPY_WORDS(9600), REG_DMA3);
	tilemap = LOGO_MAP;
	tileIndex = LOGO_FIRST_TILE;
	for(screenY = 0; screenY <= 19; screenY++) {
		for(screenX = 0; screenX <= 29; screenX++)
			*tilemap++ = tileIndex++;
		*tilemap++ = offscreenTile;
		*tilemap++ = offscreenTile;
	}
	bgScroll = (SCROLL_PAIR *)BgScrollWork;
	for(screenY = 0; screenY <= 3; screenY++) {
		bgScroll->y = 0;
		bgScroll->x = 0;
		bgScroll++;
	}
	syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
	((MAP_GRID_SCROLL_STATE *)MapGridState[0])->flg =
		DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
}

#include "shared/screen_entrances.c"

Uint32 TitleExits[] = {
	SCENE_EXITS_END,
};

ACTOR_PLACE TitleActorPlacements[] = {
	{SCENE_TABLE_END},
};

CUE TitleCues[] = {
	{CUE_TABLE_END},
};

/* The VRAM cache entry holding the lettering's tiles, which is -1 until
   scTitleLoadSprs takes one. */
Sint16 TitleVramBlock = -1;

/* @file */
/* The title: the publisher's and Camelot's logos, the intro, and the starts it
 */
/* leads to, the prologue with Jenna or the game with Felix. TITLE_STORY_CRAWL
 */
/* plays the story crawl a new game opens with, over Vale in the rain, and */
/* TITLE_TO_BE_CONTINUED, which nothing reaches, keeps The Broken Seal's Golden
 */
/* Sun logo with "To Be Continued" revealed under it. */

/* This unit declares its own form of mnTitleShowCamelotLogo, below. */
#define OWN_Title_ShowCamelotLogo

#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "game/game.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "scene/title.h"

extern s16 TitleVramBlock;

static void scTitleRevealScr(s32 unused);
static void scTitleLoadBackground(void);

/* One sprite of the logo's lettering, as syOamAddSprAtDepthClamped queues */
/* it. scTitleUpdateSprReveal rewrites every entry each frame. */
typedef struct TITLE_SPR {
	/* [0] the queue link the depth queue writes; [1] OAM attribute 0 (y, mode
	 */
	/* and shape) in the low half and attribute 1 (x) in the high half; [2] */
	/* attribute 2 (the tile, and the palette). */
	u32 attr[3];
} TITLE_SPR;

/* Frames since the lettering's reveal began; a column appears every two. */
extern s16 TitleRevealFrame;
/* The lettering's 18 sprites, in the overlay's BSS (scene/title_bss.s). */
extern TITLE_SPR TitleSprites[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* The ROM's caller here is compiled against this form, which takes an argument
 */
/* and returns nothing where the definition takes none and returns an s32. */
void mnTitleShowCamelotLogo(s32 unused);
/* The overlay's initialize entry, which plays what the entrance asks for. */
s32 scInit(void) {
	s32 waitFrames;

	if(MainRoutineStateWork.entrance == TITLE_TO_BE_CONTINUED) {
		fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
		fdEventPlaySnd(MUS_TITLE_AFTER_PROLOGUE);
		scTitleRevealScr(0);
		/* After 120 frames, the logo waits up to 3600 frames (a minute) for a
		 * key */
		/* press. */
		sySynch(120);
		for(waitFrames = 0; waitFrames < 3600; waitFrames++) {
			sySynch(1);
			if(InputWork.pressed != 0)
				break;
		}
		fdEventSetDestination(SCENE_TITLE, TITLE_PROLOGUE);
		return 0;
	}
	if(MainRoutineStateWork.entrance == TITLE_STORY_CRAWL) {
		fdEventPlaySnd(MUS_THE_BROKEN_SEAL);
		/* "Ages ago, or so the stories / tell, the power of Alchemy..." */
		mnCreditsRun(CREDITS_MODE_STORY_CRAWL);
		fdEventPlaySnd(SOUND_FADE_OUT_BGM);
		syScrFadeToBlack(60);
		syScrFadeWait();
		fdEventWait(240);
		fdEventPlaySnd(MUS_SILENCE);
		fdEventSetDestination(SCENE_TITLE, TITLE_PROLOGUE);
		return 0;
	}
	syResourceReservedNoOp(RESOURCE_TITLE_STUBS);
	if(MainRoutineStateWork.entrance == TITLE_START) {
		for(;;) {
			fdEventPlaySnd(MUS_SILENCE);
			mnTitleShowPublisherLogo(0);
			mnTitleShowCamelotLogo(0);
			/* With no save file the intro is skipped. Left alone, the title
			 * fades */
			/* out and the logos play again after two seconds, or a key press.
			 */
			if(mnSaveScanFileSummaries() > 0) {
				fdEventPlaySnd(MUS_MAIN_THEME_TITLE);
				mnTitleIntroRun(1);
				if(mnTitleShowTitleScr(1) == 0) {
					fdEventPlaySnd(SOUND_FADE_OUT_BGM);
					syScrFadeToBlack(30);
					syScrFadeWait();
					for(waitFrames = 0; waitFrames <= 119; waitFrames++) {
						sySynch(1);
						if(InputWork.pressed != 0)
							break;
					}
					continue;
				}
			}
			break;
		}
		fdEventSetDestination(SCENE_CLEAR, CLEAR_FROM_TITLE);
	}
	else {
		if(MainRoutineStateWork.entrance == TITLE_PROLOGUE) {
			fdEventPlaySnd(MUS_MAIN_THEME_TITLE);
			mnTitleIntroRun(0);
			syFieldWindowColApply(MainRoutineStateWork.winHue,
								  MainRoutineStateWork.winBrightness);
			gmPartySetLeaderJenna();
			fdEventSetDestination(SCENE_VENUS_LIGHTHOUSE,
								  VENUS_LIGHTHOUSE_FROM_TITLE);
		}
		if(MainRoutineStateWork.entrance == TITLE_FROM_IDEJIMA) {
			fdEventPlaySnd(MUS_TITLE_AFTER_PROLOGUE);
			mnTitleShowTitleScr(0);
			syFieldWindowColApply(MainRoutineStateWork.winHue,
								  MainRoutineStateWork.winBrightness);
			gmPartySetLeaderFelix();
			fdEventSetDestination(SCENE_IDEJIMA, IDEJIMA_RIDE_THE_TIDAL_WAVE);
		}
	}
	fdEventPlaySnd(SOUND_FADE_OUT_BGM);
	syScrFadeToBlack(30);
	syScrFadeWait();
	fdEventWait(60);
	fdEventPlaySnd(MUS_SILENCE);
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* The sprite palette of the "To Be Continued" lettering. */
enum { TO_BE_CONTINUED_PALETTE = 15 };

/* Loads the logo's lettering sprites and their palette. */
void scTitleLoadSprs(s32 unused) {
	u8 *decodedSprites;
	volatile s32 *dma3;

	/* The decoded data opens with its 16-colour palette, then 40 4-bit tiles of
	 */
	/* lettering. */
	decodedSprites = syHeapAllocEwram(PALETTE_COLORS * 2 + 40 * TILE_SIZE_4BPP);
	if(TitleVramBlock == -1)
		TitleVramBlock = syVramCacheFindFreeEntry();
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_INTRO_TILES_B),
						 decodedSprites);
	syDMAXCopy(decodedSprites,
			   (void *)&OBJ_PLTT_COLOR(TO_BE_CONTINUED_PALETTE, 0),
			   DMA_COPY_WORDS(PALETTE_COLORS / 2), REG_DMA3);
	syVramCacheLoad(TitleVramBlock, 40 * TILE_SIZE_4BPP,
					decodedSprites + PALETTE_COLORS * 2);
	dma3 = (volatile s32 *)REG_DMA3;
	while(dma3[2] & DMA_BUSY)
		;
	syHeapRelease(decodedSprites);
}

/* The task that queues the logo's lettering sprites each frame, revealing */
/* them from the left: those the reveal has reached are queued, the two newest
 */
/* only on even frames. */
void scTitleUpdateSprReveal(void) {
	u32 *attributes;
	TITLE_SPR *sprite;
	s32 tileIndex;
	s32 column;
	s32 revealAge;
	s32 screenX;

	/* One row of 18 semi-transparent sprites of 8 by 16 pixels at y 136 and x
	 * 88 */
	/* to 224. */
	sprite = TitleSprites;
	attributes = TitleSprites[0].attr;
	tileIndex = VramCacheEntries[TitleVramBlock].ofs >> 5;
	for(column = 0; column <= 17; column++) {
		s32 screenY = 136;

		screenX = 232 - (18 - column) * 8;
#ifdef UBFIX
		/* Stay inside each sprite's three-word array instead of walking into
		 * the */
		/* next struct. */
		attributes = TitleSprites[column].attr;
#endif
		*attributes++ = 0;
		*attributes++ = OBJ_X(screenX) | screenY | OBJ_SIZE_8X16 |
						OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		*attributes++ = OBJ_PALETTE(TO_BE_CONTINUED_PALETTE) | tileIndex;
		revealAge = TitleRevealFrame / 2 - column;
		if(revealAge < 0)
			revealAge = 0;
		if(revealAge <= 2 && (FrameCount & 1))
			revealAge = 0;
		if(revealAge != 0)
			syOamAddSprAtDepthClamped(sprite++, OAM_DEPTH_FRONT);
		tileIndex += 2;
	}
	TitleRevealFrame++;
}

/* Shows the Golden Sun logo for TITLE_TO_BE_CONTINUED: its "To Be Continued" */
/* lettering */
/* is revealed from the left while the logo brightens out of black. */
static void scTitleRevealScr(s32 unused) {
	s32 brightnessStep;

	scTitleLoadBackground();
	fdEventWait(30);
	TitleRevealFrame = 0;
	scTitleLoadSprs(0);
	sySetEvent(scTitleUpdateSprReveal, TASK_PASS_FRAME);
	/* Show BG0, BG2 and the sprites. BG1 to BG3 start fully darkened, so the */
	/* logo on BG2 is black; the semi-transparent lettering is blended onto the
	 */
	/* layers under it at full weight. */
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
						DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG0_ON |
							DISPCNT_OBJ_1D_MAP,
						IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
						BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
							BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_BG0 |
							BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 |
							BLDCNT_TGT2_BG3 | BLDCNT_TGT2_BD,
						IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, BLEND_FADE_MAX, IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
						BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX),
						IO_WRITE_HALFWORD);
	fdEventWait(120);
	for(brightnessStep = 0; brightnessStep <= BLEND_FADE_MAX;
		brightnessStep++) {
		IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, BLEND_FADE_MAX - brightnessStep,
							IO_WRITE_HALFWORD);
		sySynch(3);
	}
	FieldStateWork->transition = 0;
	FieldStateWork->transitionFrm = 1;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	FieldStateWork->transitionFrm = 60;
}

/* The title logo: its palette, its tiles decoded and copied to VRAM, a 30 by */
/* 20 tilemap of consecutive tiles with two blank cells closing each row, and */
/* the four scroll pairs cleared. */
static void scTitleLoadBackground(void) {
	u8 *encodedGraphics;
	s32 resourceId;
	u16 *tilemap;
	u32 screenX;
	u32 screenY;
	u16 tileIndex;
	BG_SCROLL *bgScroll;
	s32 blankTile;

	resourceId = RESOURCE_LOGO_ON_BLACK;
	syScrFadeToBlack(0);
	REG_BG2CNT = LOGO_BGCNT;
	BgScrollWork[2].y = 0;
	blankTile = LOGO_BLANK_TILE;
	encodedGraphics = syResourceGetTableEntry(resourceId);
	syDMAXCopy(encodedGraphics, (void *)BG_PLTT,
			   DMA_COPY_WORDS(LOGO_PALETTE_COLORS / 2), REG_DMA3);
	encodedGraphics += LOGO_PALETTE_COLORS * 2;
	syFieldDecompressTaggedLz(encodedGraphics, Ram_MapCellBuffer);
	syDMAXCopy((void *)Ram_MapCellBuffer, LOGO_TILES, DMA_COPY_WORDS(9600),
			   REG_DMA3);
	tilemap = LOGO_MAP;
	tileIndex = LOGO_FIRST_TILE;
	for(screenY = 0; screenY <= 19; screenY++) {
		for(screenX = 0; screenX <= 29; screenX++)
			*tilemap++ = tileIndex++;
		*tilemap++ = blankTile;
		*tilemap++ = blankTile;
	}
	bgScroll = BgScrollWork;
	for(screenY = 0; screenY <= 3; screenY++) {
		bgScroll->y = 0;
		bgScroll->x = 0;
		bgScroll++;
	}
	syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
	((MAP_STATE *)Ram_HeapSlots->mapGrid)->dispcnt =
		DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
}

#include "shared/scene_entrances_mapless.c"

u32 SceneExits[] = {
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	{SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
	{TRIGGER_TABLE_END},
};

/* The VRAM cache entry the logo's lettering takes; -1 for none yet. */
s16 TitleVramBlock = -1;

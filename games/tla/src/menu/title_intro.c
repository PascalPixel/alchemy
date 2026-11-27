/* @file */
/* The title intro, the publisher's and Camelot's logos, the title screen and */
/* the palette fade they use. The intro opens on drifting clouds, shows the */
/* cards `Camelot presents` and `Golden Sun` `Book Two` between them, drops */
/* through the clouds into white, and scrolls down the lighthouse in its valley
 */
/* while a gull flies off towards it and `The Lost Age` comes into focus. */
/* */
/* The gull's frames and path are logo.c; the prompt's levels and the fade's */
/* channel curves are title_intro_tables.c. */

#include "sounds.h"
#include "version.h"
#include "macros.h"
#include "window/window.h"
#include "graphics/resource.h"
#include "graphics/palette_buffers.h"
#include "field/actor.h"
#include "resource_ids.h"
#include "scene/title.h"

/* The Title's "Press Start" prompt, byte-LZ compressed, in each */
/* language. */
extern const u8 Title_PromptTiles[];

/* The screen blocks of the intro's maps: the near clouds on BG2 and the far */
/* ones on BG1, which the lighthouse later takes; the cards' text on BG3 and */
/* the line blended in over it on BG0. */
#define INTRO_FRONT_SCREEN		30
#define INTRO_BACK_SCREEN		31
#define INTRO_CARD_SCREEN		29
#define INTRO_CARD_BLEND_SCREEN 28

/* One step of the clouds' scroll (mnTitleRunScrollStep, TITLE_STEP). */
typedef struct TITLE_SCROLL_STEP {
	s32 frames; /* how long the step lasts */
	/* Pixels a frame: BG1's y and x, then BG2's y and x. */
	s32 speed[4];
	/* For each speed, the frame count bits that must be clear for it to move;
	 */
	/* 0 moves it every frame. */
	u32 mask[4];
} TITLE_SCROLL_STEP;

/* The scroll down the lighthouse picture, or back up it: where it is and where
 */
/* it stops, the pixels a step and the frames each step waits, and the */
/* picture's rows of tiles copied into VRAM as they come into view. */
typedef struct TITLE_SCROLL {
	s16 pos;	 /* pixels down the picture */
	s16 limit;	 /* the position this scroll stops at */
	s16 step;	 /* pixels a step, negative going back up */
	u8 *src;	 /* the next row of the picture to copy */
	u8 *dst;	 /* where in VRAM that row goes */
	u16 *cursor; /* the map entry of the row's first tile */
	u16 tile;	 /* that tile's number */
	s16 scroll;	 /* pixels scrolled since the last row was copied */
	/* Going down, the VRAM row the next copy fills; going back up, the rows */
	/* left to pass before the next copy. */
	u16 rows;
	u32 frames; /* frames each step waits */
} TITLE_SCROLL;

/* An OAM entry's attributes 0 to 2, as the title's logo pieces are written. */
typedef struct OAM_ENTRY {
	u32 y : 8;
	u32 affine : 2; /* 1 drawn through its matrix, 3 also at double size */
	u32 mode : 2;	/* an SPR_MODE */
	u32 mosaic : 1;
	u32 fullCol : 1;
	u32 shape : 2;
	u32 x : 9;
	u32 matrix : 5;
	u32 size : 2;
	u16 tile : 10;
	u16 pri : 2;
	u16 pal : 4;
	u16 unused;
} OAM_ENTRY;

/* OAM_ENTRY.size for a 64 by 64 OBJ_SHAPE_SQUARE sprite. */
#define OAM_SIZE_64 3

/* TITLE_LOGO_STATE.phase: what mnTitleIntroUpdateLogo draws. */
typedef enum TITLE_LOGO_PHASE {
	/* A gull flies off towards the lighthouse along TitleIntro_GullPath. */
	TITLE_LOGO_GULL = 0,
	/* `The Lost Age` closes in, its three pieces scaling down and fading up. */
	TITLE_LOGO_ZOOM = 1
} TITLE_LOGO_PHASE;

/* The title intro's state while the lighthouse shows (HEAP_SLOT_TITLE_INTRO):
 */
/* the gull or the logo, and the countdowns to the fade to white and back that
 */
/* the scroll down starts. */
typedef struct TITLE_LOGO_STATE {
	s16 phase; /* TITLE_LOGO_PHASE */
	/* Where the gull's frame starts in TitleIntro_GullFrames; in the zoom, the
	 */
	/* logo's blend weight. */
	s16 frm;
	/* Frames left of the gull's frame; in the zoom, frames since it began. */
	s16 tmr;
	/* Where the gull's step starts in TitleIntro_GullPath; in the zoom, the */
	/* logo's scale. */
	s16 step;
	s16 delay;	   /* frames left of the gull's step */
	s16 toWhite;   /* scroll steps until the fade to white; 0 when none */
	s16 fromWhite; /* scroll steps until the fade back; 0 when none */
} TITLE_LOGO_STATE;

/* A sprite of the "Press Start" prompt, linked into a render slot each frame:
 */
/* OAM attributes 0 to 2 with the position split out. */
typedef struct INTRO_SPR {
	u32 next; /* the render slot link */
	u8 y;
	u8 attr0High; /* shape and mode bits of attribute 0 */
	u16 x : 9;
	u16 attr1High : 7; /* flip and size bits of attribute 1 */
	s32 tile;
} INTRO_SPR;

/* The title screen's state (HEAP_SLOT_TITLE_INTRO): the prompt's sprites. */
typedef struct TITLE_SCR_STATE {
	u8 unused[16]; /* no code names it */
	INTRO_SPR sprs[8];
} TITLE_SCR_STATE;

void mnTitlePaletteFadeSetCurrent(s32 tint, s32 selectedPalettes);
extern const u8 Title_PromptBlendLevels[];
extern u16 TitlePaletteFade_ChannelCurve[];
extern u16 TitlePaletteFade_ChannelCurveLight[];
extern u16 TitlePaletteFade_ChannelCurveContrast[];
s32 mnTitlePaletteFadeClampChannel(s32 value);
s32 mnTitlePaletteFadeClampShiftedChannel(s32 value);

/* The intro's tiles, in 16 colours: the far clouds in character block 0, the */
/* near clouds in block 1, each 256 by 128 pixels, and the cards' text in block
 */
/* 2. The text is `Camelot presents`, three rows in these editions and two in */
/* the others, then `Golden Sun` on three and `Book Two` on two. */
#define CLOUD_ROWS 16
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
	LANGUAGE == LANGUAGE_ITALIAN
#define PRESENTS_ROWS 3
#else
#define PRESENTS_ROWS 2
#endif
#define GOLDEN_SUN_ROWS 3
#define BOOK_TWO_ROWS	2
#define CARD_TEXT_ROWS	(PRESENTS_ROWS + GOLDEN_SUN_ROWS + BOOK_TWO_ROWS)
#define GOLDEN_SUN_TILE (PRESENTS_ROWS * BG_SCREEN_WIDTH)
#define BOOK_TWO_TILE	(GOLDEN_SUN_TILE + GOLDEN_SUN_ROWS * BG_SCREEN_WIDTH)
#define INTRO_TILE_BYTES                                                       \
	((2 * CLOUD_ROWS + CARD_TEXT_ROWS) * BG_SCREEN_WIDTH * TILE_SIZE_4BPP)

/* Blanks the display, loads the intro's clouds and cards: their colours into */
/* the BG palette with colour 0 black, and their tiles, unpacked through the */
/* map cell buffer, from BG character block 0. */
void mnTitleIntroLoadPictureTiles(void) {
	u8 *source;
	u8 *cells;

	REG_DISPCNT = 0;
	source = syResourceGetTableEntry(RESOURCE_INTRO_CLOUDS);
	syDMAXCopy(source, (void *)BG_PLTT, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
			   REG_DMA3);
	BG_PLTT[0] = 0;
	cells = Ram_MapCellBuffer;
	syDecompressByteLz(source + BG_PLTT_SIZE, cells);
	syDMAXCopy(cells, (void *)BG_CHAR_BLOCK(0),
			   DMA_COPY_WORDS(INTRO_TILE_BYTES / 4), REG_DMA3);
}

/* Waits frames frames, stirring the random seed each frame; returns 1 at once
 */
/* when a key is pressed, 0 when the wait ends. */
s32 mnTitleWaitOrPress(u32 frames) {
	u32 i;

	for(i = 0; i < frames; i++) {
		sySynch(1);
		syRandom();
		if(InputWork.pressed != 0)
			return 1;
	}
	return 0;
}

/* Runs one step of the clouds' scroll, moving BG1 and BG2 each frame by the */
/* step's speeds on the frames its masks allow; returns 0 when a key cuts it */
/* short, 1 when it ends. */
s32 mnTitleRunScrollStep(TITLE_SCROLL_STEP *step) {
	s32 i;

	for(i = 0; i < step->frames; i++) {
		if(step->mask[0] == 0 || (FrameCount & step->mask[0]) == 0)
			BgScrollWork[1].y += step->speed[0];
		if(step->mask[1] == 0 || (FrameCount & step->mask[1]) == 0)
			BgScrollWork[1].x += step->speed[1];
		if(step->mask[2] == 0 || (FrameCount & step->mask[2]) == 0)
			BgScrollWork[2].y += step->speed[2];
		if(step->mask[3] == 0 || (FrameCount & step->mask[3]) == 0)
			BgScrollWork[2].x += step->speed[3];
		if(mnTitleWaitOrPress(1))
			return 0;
	}
	return 1;
}

/* One step of the clouds' scroll: for `duration` frames, BG1 and BG2 move by */
/* their dy and dx each frame whose count has none of its mask's bits. */
#define TITLE_STEP(duration, bg1Dy, bg1Dx, bg2Dy, bg2Dx, bg1DyMask, bg1DxMask, \
				   bg2DyMask, bg2DxMask)                                       \
	(step.frames = (duration), step.speed[0] = (bg1Dy),                        \
	 step.speed[1] = (bg1Dx), step.speed[2] = (bg2Dy),                         \
	 step.speed[3] = (bg2Dx), step.mask[0] = (bg1DyMask),                      \
	 step.mask[1] = (bg1DxMask), step.mask[2] = (bg2DyMask),                   \
	 step.mask[3] = (bg2DxMask), mnTitleRunScrollStep(&step))

/* The near clouds' and the far clouds' blend weights over each other; the */
/* plunge raises both from the near clouds' to full. */
#define CLOUD_FRONT_WEIGHT 11
#define CLOUD_BACK_WEIGHT  10

/* The intro's three passes over the clouds. */
typedef enum CLOUDS_PASS {
	CLOUD_BEFORE_CARDS = 0,
	CLOUD_BETWEEN_CARDS = 1,
	/* After the last card, ending in a plunge through them into white. */
	CLOUD_AFTER_CARDS = 2
} CLOUDS_PASS;

/* Shows the two layers of clouds and runs one pass's scroll steps over them */
/* (an CLOUDS_PASS), fading in from dark red, at once on the first pass; */
/* returns 0 when a key cuts it short. */
s32 mnTitleIntroScrollClouds(s32 pass) {
	TITLE_SCROLL_STEP step;
	u16 *map;
	u16 tile;
	s32 i;
	s32 j;

	REG_DISPCNT = 0;
	map = (u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN);
	tile = 0;
	for(j = 0; j < CLOUD_ROWS; j++) {
		for(i = 0; i < BG_SCREEN_WIDTH; i++) {
			*map++ = tile++;
		}
	}
	tile = 0;
	for(; j < BG_SCREEN_WIDTH; j++) {
		for(i = 0; i < BG_SCREEN_WIDTH; i++) {
			*map++ = tile++;
		}
	}
	map = (u16 *)BG_SCREEN_BLOCK(INTRO_FRONT_SCREEN);
	tile = 0;
	for(j = 0; j < CLOUD_ROWS; j++) {
		for(i = 0; i < BG_SCREEN_WIDTH; i++) {
			*map++ = tile++ | BG_TILE_PALETTE(1);
		}
	}
	tile = 0;
	for(; j < BG_SCREEN_WIDTH; j++) {
		for(i = 0; i < BG_SCREEN_WIDTH; i++) {
			*map++ = tile++ | BG_TILE_PALETTE(1);
		}
	}
	REG_BG1CNT = BGCNT_PRIORITY(2) | BGCNT_SCREEN_BASE(INTRO_BACK_SCREEN);
	REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) |
				 BGCNT_SCREEN_BASE(INTRO_FRONT_SCREEN);
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1;
	REG_BLDALPHA = BLEND_ALPHA(CLOUD_FRONT_WEIGHT, CLOUD_BACK_WEIGHT);
	for(i = 0; i < 4; i++) {
		BgScrollWork[i].y = 0;
		BgScrollWork[i].x = 0;
	}
	if(pass == CLOUD_BEFORE_CARDS) {
		mnTitlePaletteFadeInit();
	}
	mnTitlePaletteFadeSetCurrent(RGB(2, 0, 0), PALETTES_ALL);
	mnTitlePaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	REG_DISPCNT = DISPCNT_BG1_ON | DISPCNT_BG2_ON;
	if(pass == CLOUD_BEFORE_CARDS) {
		mnTitlePaletteFadeStart(1);
	}
	else {
		mnTitlePaletteFadeStart(10);
	}

	/* A positive dy carries a layer up the screen, a positive dx to the left.
	 */
	if(pass == CLOUD_BEFORE_CARDS) {
		/* The far clouds rise and the near ones sink past them, slowing almost
		 * to */
		/* a stop over 80 frames... */
		if(!TITLE_STEP(30, 2, 0, -3, 0, 0, 0, 0, 0))
			return 0;
		if(!TITLE_STEP(10, 2, 0, -2, 0, 0, 0, 0, 0))
			return 0;
		if(!TITLE_STEP(10, 1, 0, -2, 0, 0, 0, 0, 0))
			return 0;
		if(!TITLE_STEP(10, 1, 0, -1, 0, 0, 0, 0, 0))
			return 0;
		if(!TITLE_STEP(10, 1, 0, -1, 0, 1, 0, 0, 0))
			return 0;
		if(!TITLE_STEP(10, 0, 0, -1, 0, 0, 0, 1, 0))
			return 0;
		/* ...then drift slowly back the other way... */
		if(!TITLE_STEP(20, -1, 0, 0, 0, 1, 0, 0, 0))
			return 0;
		if(!TITLE_STEP(20, -1, 0, 1, 0, 3, 0, 1, 0))
			return 0;
		if(!TITLE_STEP(20, -1, 0, 1, 0, 3, 0, 1, 0))
			return 0;
		/* ...and darken to red as they go, before "Camelot presents". */
		mnTitlePaletteFadeSetTarget(RGB(2, 0, 0), PALETTES_ALL);
		mnTitlePaletteFadeStart(60);
		if(!TITLE_STEP(60, -1, 0, 1, 0, 3, 0, 1, 0))
			return 0;
	}
	else if(pass == CLOUD_BETWEEN_CARDS) {
		/* The layers slide apart on a slant, the far clouds up and left,
		 * slowing */
		/* over 80 frames... */
		if(!TITLE_STEP(40, 1, 1, -1, -1, 1, 0, 1, 0))
			return 0;
		if(!TITLE_STEP(20, 1, 1, -1, -1, 3, 0, 1, 0))
			return 0;
		if(!TITLE_STEP(10, 1, 1, -1, -1, 7, 3, 1, 0))
			return 0;
		if(!TITLE_STEP(10, 0, 1, 0, -1, 0, 3, 0, 0))
			return 0;
		/* ...then turn, the far clouds sinking and the near ones going right...
		 */
		if(!TITLE_STEP(30, -1, 0, 0, -1, 15, 1, 0, 0))
			return 0;
		if(!TITLE_STEP(20, -1, -1, 0, -1, 15, 1, 0, 0))
			return 0;
		if(!TITLE_STEP(60, -1, -1, 0, -2, 15, 1, 0, 0))
			return 0;
		/* ...and darken to red, before "Golden Sun Book Two". */
		mnTitlePaletteFadeSetTarget(RGB(2, 0, 0), PALETTES_ALL);
		mnTitlePaletteFadeStart(60);
		if(!TITLE_STEP(60, -1, -1, 0, -2, 15, 1, 0, 0))
			return 0;
	}
	else {
		/* The layers drift crosswise for a second, then rush apart for 16
		 * frames, */
		/* opening a gap of white sky. */
		if(!TITLE_STEP(60, 0, -1, -1, 1, 0, 0, 0, 0))
			return 0;
		if(!TITLE_STEP(16, 8, -8, 8, 8, 0, 0, 0, 0))
			return 0;
		/* The plunge: both layers speed up and brighten, two frames a weight.
		 */
		j = 2;
		for(i = CLOUD_FRONT_WEIGHT; i < BLEND_WEIGHT_MAX; i++) {
			REG_BLDALPHA = BLEND_ALPHA(i, i);
			BgScrollWork[1].y += j + 16;
			BgScrollWork[2].y += j + 8;
			j++;
			if(mnTitleWaitOrPress(1))
				return 0;
			BgScrollWork[1].y += j + 16;
			BgScrollWork[2].y += j + 8;
			j++;
			if(mnTitleWaitOrPress(1))
				return 0;
		}
		mnTitlePaletteFadeSetTarget(RGB_WHITE, PALETTES_ALL);
		mnTitlePaletteFadeStart(1);
		if(mnTitleWaitOrPress(2)) {
			syScrFadeWait();
			return 0;
		}
	}
	return 1;
}

/* The screen rows each card's text stands on. */
#define PRESENTS_TOP_ROW   9
#define GOLDEN_SUN_TOP_ROW 7
#define BOOK_TWO_TOP_ROW   (GOLDEN_SUN_TOP_ROW + GOLDEN_SUN_ROWS)
/* A tile every edition's card text leaves blank, the left end of its third */
/* row: the `Golden Sun` card fills the screen around its text with it and with
 */
/* the blank tile below it. */
#define CARD_BLANK_TILE (2 * BG_SCREEN_WIDTH)

/* Fades in the card `Camelot presents` on BG3, holds it 150 frames and fades */
/* it out to dark red; returns 0 when a key cuts it short. */
s32 mnTitleIntroShowPresentsCard(void) {
	u16 *map;
	u16 tile;
	s32 i;
	s32 row;

	REG_DISPCNT = 0;
	map = (u16 *)BG_SCREEN_BLOCK(INTRO_CARD_SCREEN);
	tile = 0;
	for(row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
		if(row >= PRESENTS_TOP_ROW && row < PRESENTS_TOP_ROW + PRESENTS_ROWS) {
			for(i = 0; i < BG_SCREEN_WIDTH; i++) {
				*map++ = tile++ | BG_TILE_PALETTE(2);
			}
		}
		else {
			for(i = 0; i < BG_SCREEN_WIDTH; i++)
				*map++ = BG_TILE_PALETTE(2);
		}
	}
	REG_BG3CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) |
				 BGCNT_SCREEN_BASE(INTRO_CARD_SCREEN);
	REG_BLDCNT = 0;
	REG_BLDALPHA = 0;
	for(i = 0; i < 4; i++)
		BgScrollWork[i].x = BgScrollWork[i].y = 0;

	mnTitlePaletteFadeSetCurrent(RGB(0, 0, 0), PALETTES_ALL);
	mnTitlePaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	REG_DISPCNT = DISPCNT_BG3_ON;
	mnTitlePaletteFadeStart(40);
	if(mnTitleWaitOrPress(150))
		return 0;
	mnTitlePaletteFadeSetTarget(RGB(2, 0, 0), PALETTES_ALL);
	mnTitlePaletteFadeStart(40);
	if(mnTitleWaitOrPress(60))
		return 0;
	return 1;
}

/* Fades in the card `Golden Sun` on BG3, blends `Book Two` in below it from */
/* BG0 over 16 steps of 3 frames, holds the card 150 frames and fades it out to
 */
/* dark red; returns 0 when a key cuts it short. */
s32 mnTitleIntroShowBookTwoCard(void) {
	u16 *map;
	u16 tile;
	s32 i;
	s32 row;

	REG_DISPCNT = 0;
	map = (u16 *)BG_SCREEN_BLOCK(INTRO_CARD_SCREEN);
	tile = GOLDEN_SUN_TILE;
	for(row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
		if(row >= GOLDEN_SUN_TOP_ROW && row < BOOK_TWO_TOP_ROW) {
			for(i = 0; i < BG_SCREEN_WIDTH; i++) {
				*map++ = tile++ | BG_TILE_PALETTE(3);
			}
		}
		else if(row >= BOOK_TWO_TOP_ROW &&
				row < BOOK_TWO_TOP_ROW + BOOK_TWO_ROWS) {
			for(i = 0; i < BG_SCREEN_WIDTH; i++)
				*map++ =
					BG_TILE_PALETTE(3) | (CARD_BLANK_TILE + BG_SCREEN_WIDTH);
		}
		else {
			for(i = 0; i < BG_SCREEN_WIDTH; i++)
				*map++ = BG_TILE_PALETTE(3) | CARD_BLANK_TILE;
		}
	}
	map = (u16 *)BG_SCREEN_BLOCK(INTRO_CARD_BLEND_SCREEN);
	tile = BOOK_TWO_TILE;
	for(row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
		if(row >= BOOK_TWO_TOP_ROW && row < BOOK_TWO_TOP_ROW + BOOK_TWO_ROWS) {
			for(i = 0; i < BG_SCREEN_WIDTH; i++) {
				*map++ = tile++ | BG_TILE_PALETTE(3);
			}
		}
		else {
			for(i = 0; i < BG_SCREEN_WIDTH; i++)
				*map++ = BG_TILE_PALETTE(3) | CARD_BLANK_TILE;
		}
	}
	REG_BG3CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) |
				 BGCNT_SCREEN_BASE(INTRO_CARD_SCREEN);
	REG_BG0CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(2) |
				 BGCNT_SCREEN_BASE(INTRO_CARD_BLEND_SCREEN);
	REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3;
	REG_BLDALPHA = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
	for(i = 0; i < 4; i++)
		BgScrollWork[i].x = BgScrollWork[i].y = 0;

	mnTitlePaletteFadeSetCurrent(RGB(0, 0, 0), PALETTES_ALL);
	mnTitlePaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	REG_DISPCNT = DISPCNT_BG0_ON | DISPCNT_BG3_ON;
	mnTitlePaletteFadeStart(40);
	if(mnTitleWaitOrPress(60))
		return 0;
	for(i = 1; i <= BLEND_WEIGHT_MAX; i++) {
		REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
		if(mnTitleWaitOrPress(3))
			return 0;
	}
	if(mnTitleWaitOrPress(150))
		return 0;
	mnTitlePaletteFadeSetTarget(RGB(2, 0, 0), PALETTES_ALL);
	mnTitlePaletteFadeStart(40);
	if(mnTitleWaitOrPress(60))
		return 0;
	return 1;
}

/* The lighthouse picture, 240 by 384 pixels in 256 colours: its first screen */
/* is loaded before it scrolls and the rest of its rows of tiles are copied in
 */
/* as they come into view, wrapping round the map's 32 rows. */
#define LIGHTHOUSE_HEIGHT	 384
#define LIGHTHOUSE_ROW_BYTES (DISPLAY_TILE_WIDTH * TILE_SIZE_8BPP)
/* The scroll that shows the picture's foot. */
#define LIGHTHOUSE_BOTTOM (LIGHTHOUSE_HEIGHT - DISPLAY_HEIGHT)
/* A screen of tiles in 256 colours: the lighthouse picture's first screen, and
 */
/* the publisher's logo and the title screen. */
#define SCREEN_TILE_BYTES (DISPLAY_TILE_HEIGHT * LIGHTHOUSE_ROW_BYTES)
/* The map entry of the two columns past the screen's right edge, which never */
/* show. */
#define OFFSCREEN_TILE 511

/* Scrolls the lighthouse picture from one position to another: each step moves
 */
/* it by the scroll's step and waits its frames, copies in the next row of the
 */
/* picture as one comes into view, and starts the fade to white or back when */
/* its countdown reaches 1. Returns 1 when a key cuts the scroll short. */
s32 mnTitleIntroScrollLighthouse(TITLE_SCROLL *s) {
	TITLE_LOGO_STATE *timers = Ram_HeapSlots->titleIntro;
	s32 i;
	s32 j;

	if(s->step > 0) {
		for(i = s->pos; i < s->limit; i += s->step) {
			if(mnTitleWaitOrPress(s->frames))
				goto fail;
			if(s->scroll > 0) {
				syDMAXCopy(s->src, s->dst,
						   DMA_COPY_WORDS(LIGHTHOUSE_ROW_BYTES / 4), REG_DMA3);
				s->src += LIGHTHOUSE_ROW_BYTES;
				s->dst += LIGHTHOUSE_ROW_BYTES;
				for(j = 0; j < DISPLAY_TILE_WIDTH; j++)
					*s->cursor++ = s->tile++;
				*s->cursor++ = OFFSCREEN_TILE;
				*s->cursor++ = OFFSCREEN_TILE;
				if(++s->rows == BG_SCREEN_WIDTH) {
					s->dst = (u8 *)BG_CHAR_BLOCK(0);
					s->rows = 0;
					s->tile = 0;
					s->cursor = (u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN);
				}
				s->scroll -= 8;
			}
			s->scroll += s->step;
			BgScrollWork[1].y += s->step;
			if(timers->toWhite != 0 && --timers->toWhite == 1) {
				mnTitlePaletteFadeSetTarget(RGB_WHITE, PALETTES_ALL);
				mnTitlePaletteFadeStart(10);
			}
			if(timers->fromWhite != 0 && --timers->fromWhite == 1) {
				mnTitlePaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
				mnTitlePaletteFadeStart(10);
			}
		}
		goto done;
	fail:
		return 1;
	}
	else {
		for(i = s->pos; i > s->limit; i += s->step) {
			BgScrollWork[1].y += s->step;
			s->scroll -= s->step;
			if(s->scroll > 8) {
				s->rows--;
				if(s->rows == 0) {
					s->rows = 1;
					syDMAXCopy(s->src, s->dst,
							   DMA_COPY_WORDS(LIGHTHOUSE_ROW_BYTES / 4),
							   REG_DMA3);
					s->src -= LIGHTHOUSE_ROW_BYTES;
					s->dst -= LIGHTHOUSE_ROW_BYTES;
					for(j = 0; j < DISPLAY_TILE_WIDTH; j++)
						*s->cursor++ = s->tile++;
					s->cursor -= DISPLAY_TILE_WIDTH;
					s->tile -= DISPLAY_TILE_WIDTH;
				}
				s->scroll -= 8;
			}
			if(mnTitleWaitOrPress(s->frames))
				goto fail;
		}
	}
done:
	s->pos = i;
	return 0;
}

/* A piece of `The Lost Age`: 64 by 64 pixels in 256 colours, numbered in the */
/* sprites' tiles of 32 bytes. */
#define LOGO_PIECE_BYTES (64 * 64)
#define LOGO_PIECE_TILES (LOGO_PIECE_BYTES / TILE_SIZE_4BPP)

/* Writes one piece of `The Lost Age` straight into OAM, in the entry its tile
 */
/* picks: affine at double size, so that its top left is at (x, y) unscaled, */
/* semi-transparent and in 256 colours. */
void mnTitlePlaceLogoPiece(s32 tile, s32 x, u32 y) {
	OAM_ENTRY oam;

	btDmaFill32(0, &oam, sizeof(oam));
	oam.shape = OBJ_SHAPE_SQUARE;
	oam.size = OAM_SIZE_64;
	oam.fullCol = 1;
	oam.mode = OBJ_MODE_SEMI_TRANSPARENT;
	oam.affine = OAM_AFFINE_DOUBLE_SIZE;
	oam.tile = tile;
	oam.x = x - 32;
	oam.y = y - 32;
	syDMAXCopy(&oam, (void *)((u32)OAM + ((u32)tile >> 4)), DMA_COPY_WORDS(2),
			   REG_DMA3);
}

/* Where the left, middle and right pieces of `The Lost Age` come to rest, and
 */
/* how far down. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
	LANGUAGE == LANGUAGE_ITALIAN
#define LOGO_LEFT_X	  24
#define LOGO_MIDDLE_X 88
#define LOGO_RIGHT_X  152
#elif LANGUAGE == LANGUAGE_FRENCH
#define LOGO_LEFT_X	  19
#define LOGO_MIDDLE_X 83
#define LOGO_RIGHT_X  147
#else
#define LOGO_LEFT_X	  30
#define LOGO_MIDDLE_X 83
#define LOGO_RIGHT_X  140
#endif
#define LOGO_Y 50

/* The logo pieces' affine scale, in 8.8: they start drawn 1.6 times their size
 */
/* and shrink to it. */
#define LOGO_MATRIX_START Q_8_8(0.625)
#define LOGO_MATRIX_REST  Q_8_8(1)

/* The fields of a gull frame in TitleIntro_GullFrames: its first tile, its */
/* rows of GULL_COLUMNS 8x8 sprites, the frames it shows for and its offset. */
typedef enum GULL_FRAME_FIELD {
	GULL_FRAME_TILE,
	GULL_FRAME_ROWS,
	GULL_FRAME_DURATION,
	GULL_FRAME_OFFSET_X,
	GULL_FRAME_OFFSET_Y,
	GULL_FRAME_FIELDS
} GULL_FRAME_FIELD;
/* The fields of a step of TitleIntro_GullPath: where the gull is and the */
/* frames it stays; an x of -1 ends the path. */
typedef enum GULL_STEP_FIELD {
	GULL_STEP_X,
	GULL_STEP_Y,
	GULL_STEP_DURATION,
	GULL_STEP_FIELDS
} GULL_STEP_FIELD;
/* The gull's frames are 64 pixels wide, eight 8x8 sprites to a row, and the */
/* path places the middle of its first frame, 28 pixels in. */
#define GULL_COLUMNS  8
#define GULL_CENTER_X 28
/* The tiles the gull's frames draw from: 108 rows of eight. Its sheet's 109th
 */
/* row is blank and is not copied. */
#define GULL_TILES (108 * GULL_COLUMNS)

extern s16 TitleIntro_GullFrames[];
extern s16 TitleIntro_GullPath[];

/* Unpacks a picture of `The Lost Age`, its three pieces, into sprite tiles. */
#define LOAD_LOGO_TILES(id)                                                    \
	{                                                                          \
		u8 *buffer = TitleIntro_LogoTileBuffer;                                \
		syDecompressByteLz(syResourceGetTableEntry(id), buffer);               \
		syDMAXCopy(buffer, (void *)OBJ_VRAM0,                                  \
				   DMA_COPY_WORDS(3 * LOGO_PIECE_BYTES / 4), REG_DMA3);        \
	}

/* The task that draws over the lighthouse each frame: the gull flying off */
/* towards it along its path, then `The Lost Age` closing in, its pictures */
/* coming into focus as it fades up. */
void mnTitleIntroUpdateLogo(void) {
	TITLE_LOGO_STATE *logo;
	OAM_ENTRY sprite;
	volatile u32 fill;
	s32 x;
	s32 tile;
	s32 left;
	s32 rows;
	s32 top;
	s32 offsetX;
	s32 offsetY;
	s32 row;
	s32 column;
	s32 y;
	s32 pieceX;
	volatile u16 *matrix;

	logo = Ram_HeapSlots->titleIntro;
	btDmaFill32Cell(fill, 0, (void *)OAM, OAM_SIZE);
	if(logo->phase == TITLE_LOGO_GULL) {
		if(logo->tmr == 0) {
			if(TitleIntro_GullFrames[logo->frm + GULL_FRAME_FIELDS] != -1)
				logo->frm += GULL_FRAME_FIELDS;
			logo->tmr = TitleIntro_GullFrames[logo->frm + GULL_FRAME_DURATION];
		}
		else {
			logo->tmr--;
		}
		if(logo->delay == 0) {
			logo->step += GULL_STEP_FIELDS;
			logo->delay = TitleIntro_GullPath[logo->step + GULL_STEP_DURATION];
		}
		else {
			logo->delay--;
		}
		x = TitleIntro_GullPath[logo->step + GULL_STEP_X];
		if(x == -1) {
			syDMAXCopy(syResourceGetTableEntry(RESOURCE_SUBTITLE_PALETTE),
					   (void *)OBJ_PLTT, DMA_COPY_WORDS(OBJ_PLTT_SIZE / 4),
					   REG_DMA3);
			LOAD_LOGO_TILES(RESOURCE_SUBTITLE_A);
			logo->phase = TITLE_LOGO_ZOOM;
			logo->frm = 1;
			logo->tmr = 0;
			logo->step = LOGO_MATRIX_START;
			logo->delay = 0;
			REG_BLDCNT =
				BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1;
			REG_BLDALPHA = BLEND_ALPHA(1, BLEND_WEIGHT_MAX);
			return;
		}
		tile = TitleIntro_GullFrames[logo->frm + GULL_FRAME_TILE];
		rows = TitleIntro_GullFrames[logo->frm + GULL_FRAME_ROWS];
		offsetX = TitleIntro_GullFrames[logo->frm + GULL_FRAME_OFFSET_X];
		offsetY = TitleIntro_GullFrames[logo->frm + GULL_FRAME_OFFSET_Y];
		left = x + offsetX - GULL_CENTER_X;
		top = TitleIntro_GullPath[logo->step + GULL_STEP_Y] + offsetY;
		for(row = 0; row < rows; row++) {
			for(column = 0; column < GULL_COLUMNS; column++) {
				btDmaFill32Cell(fill, 0, &sprite, sizeof(sprite));
				sprite.tile = tile + column + row * GULL_COLUMNS;
				sprite.x = left + column * 8;
				sprite.y = top + row * 8;
				syDMAXCopy(&sprite,
						   (u8 *)OAM + (row * GULL_COLUMNS + column) * 8,
						   DMA_COPY_WORDS(2), REG_DMA3);
			}
		}
	}
	else if(logo->phase == TITLE_LOGO_ZOOM) {
		/* The side pieces close in from 24 pixels out as the scale reaches
		 * rest. */
		pieceX = (logo->step - LOGO_MATRIX_REST) / 4 + LOGO_LEFT_X;
		y = (logo->step - LOGO_MATRIX_REST) / 32 + LOGO_Y;
		mnTitlePlaceLogoPiece(0, pieceX, y);
		mnTitlePlaceLogoPiece(LOGO_PIECE_TILES, LOGO_MIDDLE_X, y);
		pieceX = LOGO_RIGHT_X - (logo->step - LOGO_MATRIX_REST) / 4;
		mnTitlePlaceLogoPiece(2 * LOGO_PIECE_TILES, pieceX, y);
		/* Affine matrix 0, in the fourth halfwords of the first four entries.
		 */
		matrix = (volatile u16 *)((u8 *)OAM + 6);
		*matrix = logo->step;
		matrix += 4;
		*matrix = 0;
		matrix += 4;
		*matrix = 0;
		matrix[4] = logo->step;
		if(logo->step < LOGO_MATRIX_REST) {
			logo->step += 2;
			if(logo->step > LOGO_MATRIX_REST)
				logo->step = LOGO_MATRIX_REST;
		}
		if(logo->frm < BLEND_WEIGHT_MAX) {
			logo->tmr++;
			if(logo->tmr % 3 == 0) {
				logo->frm++;
				REG_BLDALPHA = BLEND_ALPHA(logo->frm, BLEND_WEIGHT_MAX);
			}
			if(logo->tmr == 8)
				LOAD_LOGO_TILES(RESOURCE_SUBTITLE_B)
			else if(logo->tmr == 16)
				LOAD_LOGO_TILES(RESOURCE_SUBTITLE_C)
			else if(logo->tmr == 24)
				LOAD_LOGO_TILES(RESOURCE_SUBTITLE_D)
		}
	}
}

/* The gull's resource begins with its 16 colours. */
#define GULL_PALETTE_BYTES (16 * 2)

/* Shows the lighthouse picture from white and scrolls down it to its foot, */
/* flashing white once on the way. There a gull flies off towards the */
/* lighthouse and `The Lost Age` closes in as the picture drifts back up, and */
/* it ends back at the top under a fade to white. A key cuts it short with a */
/* fade to black. Returns 1. */
s32 mnTitleIntroShowLighthouse(void) {
	TITLE_SCROLL scroll;
	volatile u32 fill;
	TITLE_LOGO_STATE *logo;
	u8 *source;
	u16 *map;
	u16 tile;
	s32 row;
	s32 i;
	u8 *buffer;

	/* The slot is given the title screen's size, though the logo's state needs
	 */
	/* far less. */
	logo = syHeapSlotAlloc(HEAP_SLOT_TITLE_INTRO, sizeof(TITLE_SCR_STATE));
	REG_DISPCNT = 0;
	btDmaFill32Cell(fill, 0, (void *)OAM, OAM_SIZE);
	source = syResourceGetTableEntry(RESOURCE_INTRO_GULL_SPRITES);
	buffer = Ram_MapCellBuffer;
	source += GULL_PALETTE_BYTES;
	syDecompressByteLz(source, buffer);
	syDMAXCopy(buffer, (void *)OBJ_VRAM0,
			   DMA_COPY_WORDS(GULL_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
	sySynch(1);
	source = syResourceGetTableEntry(RESOURCE_INTRO_LIGHTHOUSE_PICTURE);
	syDMAXCopy(source, (void *)BG_PLTT, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
			   REG_DMA3);
	*(u16 *)BG_PLTT = RGB_WHITE;
	sySynch(1);
	source += BG_PLTT_SIZE;
	syDecompressByteLz(source, buffer);
	syDMAXCopy(buffer, (void *)BG_CHAR_BLOCK(0),
			   DMA_COPY_WORDS(SCREEN_TILE_BYTES / 4), REG_DMA3);
	sySynch(1);

	map = (u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN);
	tile = 0;
	for(row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
		for(i = 0; i < DISPLAY_TILE_WIDTH; i++) {
			*map++ = tile++;
		}
		*map++ = OFFSCREEN_TILE;
		*map++ = OFFSCREEN_TILE;
	}
	REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS |
				 BGCNT_SCREEN_BASE(INTRO_BACK_SCREEN);
	REG_BLDCNT = 0;
	REG_BLDALPHA = 0;
	for(i = 0; i < 4; i++) {
		BgScrollWork[i].y = 0;
		BgScrollWork[i].x = 0;
	}
	syDMAXCopy((void *)BG_PLTT, Ram_HeapSlots->palFade,
			   DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
	sySynch(1);
	mnTitlePaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG1_ON | DISPCNT_OBJ_ON;
	mnTitlePaletteFadeStart(10);

	/* Down the picture, slowing, with a flash of white near the top. */
	scroll.pos = 0;
	scroll.limit = 176;
	scroll.step = 4;
	scroll.src = Ram_MapCellBuffer + SCREEN_TILE_BYTES;
	scroll.dst = (u8 *)BG_CHAR_BLOCK(0) + SCREEN_TILE_BYTES;
	scroll.cursor = map;
	scroll.tile = tile;
	scroll.scroll = 0;
	scroll.rows = row;
	scroll.frames = 1;
	logo->toWhite = 10;
	logo->fromWhite = 25;
	if(mnTitleIntroScrollLighthouse(&scroll))
		goto stopped;
	scroll.limit = 200;
	scroll.step = 3;
	if(mnTitleIntroScrollLighthouse(&scroll))
		goto stopped;
	scroll.limit = 216;
	scroll.step = 2;
	if(mnTitleIntroScrollLighthouse(&scroll))
		goto stopped;
	source = syResourceGetTableEntry(RESOURCE_INTRO_GULL_SPRITES);
	syDMAXCopy(source, (void *)OBJ_PLTT, DMA_COPY_WORDS(GULL_PALETTE_BYTES / 4),
			   REG_DMA3);
	logo->phase = TITLE_LOGO_GULL;
	logo->frm = -GULL_FRAME_FIELDS;
	logo->tmr = 0;
	logo->step = -GULL_STEP_FIELDS;
	logo->delay = 0;
	scroll.step = 1;
	scroll.limit = LIGHTHOUSE_BOTTOM;
	if(mnTitleIntroScrollLighthouse(&scroll))
		goto stopped;
	sySetEvent(mnTitleIntroUpdateLogo, TASK_PASS_FRAME);
	/* Back up, a pixel every eight frames, then fast under the fade to white,
	 */
	/* copying the picture's rows back from its row 15, the last the scroll down
	 */
	/* wrote over, once eleven rows have passed. */
	scroll.limit = 160;
	scroll.step = -1;
	scroll.scroll = 0;
#ifdef BUGFIX
	/* From row 16, still in place, so that each row lands in its own place and
	 */
	/* the last copy is row 0. */
	scroll.src = Ram_MapCellBuffer + 16 * LIGHTHOUSE_ROW_BYTES;
	scroll.dst = (u8 *)BG_CHAR_BLOCK(0) + 16 * LIGHTHOUSE_ROW_BYTES;
	scroll.cursor =
		(u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN) + 16 * BG_SCREEN_WIDTH;
	scroll.tile = 16 * DISPLAY_TILE_WIDTH;
#else
	/* BUG: each row lands one row low, from VRAM row 16, the map row the cursor
	 */
	/* rewrites is row 7, one entry in, and the last copy reads the row above
	 * the */
	/* picture; unseen, as the rows are sky under the fade to white. */
	scroll.src = Ram_MapCellBuffer + 15 * LIGHTHOUSE_ROW_BYTES;
	scroll.dst = (u8 *)BG_CHAR_BLOCK(0) + 16 * LIGHTHOUSE_ROW_BYTES;
	scroll.cursor =
		(u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN) + 7 * BG_SCREEN_WIDTH + 1;
	scroll.tile = 15 * DISPLAY_TILE_WIDTH;
#endif
	scroll.rows = 11;
	scroll.frames = 8;
	if(mnTitleIntroScrollLighthouse(&scroll))
		goto stopped;
	mnTitlePaletteFadeSetTarget(RGB_WHITE, PALETTES_ALL);
	mnTitlePaletteFadeStart(20);
	scroll.limit = 0;
	scroll.step = -5;
	scroll.frames = 1;
	if(!mnTitleIntroScrollLighthouse(&scroll))
		mnTitleWaitOrPress(120);
	goto done;
stopped:
	mnTitlePaletteFadeSetTarget(RGB(0, 0, 0), PALETTES_ALL);
	mnTitlePaletteFadeStart(10);
	sySynch(10);
done:
	btDmaFill32Cell(fill, 0, (void *)OAM, OAM_SIZE);
	syCloseEvent(mnTitleIntroUpdateLogo);
	syHeapSlotRelease(HEAP_SLOT_TITLE_INTRO);
	sySynch(1);
	return 1;
}

/* Plays the title intro: the clouds, `Camelot presents`, the clouds again, */
/* `Golden Sun` `Book Two`, the plunge through the clouds into white, then the
 */
/* lighthouse in its valley. A key fades it out early, skipping the lighthouse.
 */
/* Then it clears the screen for what follows and puts back the CPU-load */
/* display it turned off. `unused`, 1 from the title and 0 before the prologue,
 */
/* is never read. */
s32 mnTitleIntroRun(s32 unused) {
	u8 cpuLoad;
	s32 result;

	syIoWrite8(CpuLoadDisplayEnabled, &cpuLoad);
	syDebugTextClear();
	syVramCacheReset();
	sySynch(1);
	syInitEvent();
	OamCopyEnabled = 0;
	CpuLoadDisplayEnabled = 0;
	mnTitleIntroLoadPictureTiles();
	if((result = mnTitleIntroScrollClouds(CLOUD_BEFORE_CARDS)) == 0 ||
	   (result = mnTitleIntroShowPresentsCard()) == 0 ||
	   (result = mnTitleIntroScrollClouds(CLOUD_BETWEEN_CARDS)) == 0 ||
	   (result = mnTitleIntroShowBookTwoCard()) == 0 ||
	   (result = mnTitleIntroScrollClouds(CLOUD_AFTER_CARDS)) == 0) {
		mnTitlePaletteFadeSetTarget(RGB(0, 0, 0), PALETTES_ALL);
		mnTitlePaletteFadeStart(10);
		sySynch(10);
	}
	if(result)
		mnTitleIntroShowLighthouse();
	OamCopyEnabled = 1;
	sySynch(1);
	/* result is reused as the counter. */
	for(result = 0; result < 4; result++)
		BgScrollWork[result].x = BgScrollWork[result].y = 0;
	REG_DISPCNT = 0;
	syWindowLoadGraphics();
	syDebugTextClear();
	mnTitlePaletteFadeInit();
	CpuLoadDisplayEnabled = cpuLoad;
	return 1;
}

/* The title screen's "Press Start" prompt: five sprites further left in the */
/* French and Japanese editions; two rows of three, from eight tile blocks, in
 */
/* German; one row of three elsewhere. */
#if (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_JAPANESE)
#define PROMPT_SPRITES 5
#define PROMPT_X	   56
#define PROMPT_Y	   126
#elif LANGUAGE == LANGUAGE_GERMAN
#define PROMPT_SPRITES	  3
#define PROMPT_ROWS		  2
#define PROMPT_ROW_HEIGHT 8
#define PROMPT_X		  80
#define PROMPT_Y		  116
#else
#define PROMPT_SPRITES 3
#define PROMPT_X	   80
#define PROMPT_Y	   124
#endif
#if LANGUAGE == LANGUAGE_GERMAN
#define PROMPT_TILES 8
#else
#define PROMPT_TILES 5
#endif

/* The title screen holds this long with its prompt, and this long without. */
#define TITLE_PROMPT_HOLD_FRAMES (90 * FRAMES_PER_SECOND)
#define TITLE_HOLD_FRAMES		 (11 * FRAMES_PER_SECOND)
/* The prompt pulses once a second, from two seconds in. */
#define PROMPT_PULSE_FRAMES FRAMES_PER_SECOND
#define PROMPT_DELAY_FRAMES (2 * FRAMES_PER_SECOND)

/* The publisher's and Camelot's logos are shown as the Golden Sun logo is */
/* (scene/title.h), on BG2, each resource beginning with its 224 colours. */
#define LOGO_PALETTE_BYTES (LOGO_PALETTE_COLORS * 2)

/* Shows the title screen from white: the Golden Sun logo over the valley and */
/* its lighthouse, with, when `prompt` is set, the "Press Start" prompt */
/* pulsing. It holds 90 seconds with the prompt and 11 without, and A or Start
 */
/* ends it early, returning 1 (else 0); except in the Japanese edition, the */
/* prompt and the keys wait two seconds. */
s32 mnTitleShowTitleScr(s32 prompt) {
	s32 blankTile = OFFSCREEN_TILE;
	s32 result = 0;
	TITLE_SCR_STATE *state =
		syHeapSlotAllocEwram(HEAP_SLOT_TITLE_INTRO, sizeof(TITLE_SCR_STATE));
	u8 *data;
	u16 *tilemap;
	u16 tile;
	u32 i;
	u32 j;
	u32 holdFrames;

	syInitEvent();
	syScrFadeToWhite(1);
	syDebugTextClear();
	syVramCacheReset();
	sySynch(1);
	REG_BG2CNT = LOGO_BGCNT;
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	BgScrollWork[2].y = result;
	data = syResourceGetTableEntry(RESOURCE_TITLE_SCREEN_PICTURE);
	syDMAXCopy(data, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
			   REG_DMA3);
	syDecompressTaggedLz(data + LOGO_PALETTE_BYTES, Ram_MapCellBuffer);
	syDMAXCopy(Ram_MapCellBuffer, (void *)LOGO_TILES,
			   DMA_COPY_WORDS(SCREEN_TILE_BYTES / 4), REG_DMA3);
	tile = LOGO_FIRST_TILE;
	tilemap = LOGO_MAP;
	for(i = 0; i < DISPLAY_TILE_HEIGHT; i++) {
		for(j = 0; j < DISPLAY_TILE_WIDTH; j++)
			*tilemap++ = tile++;
		*tilemap++ = blankTile;
		*tilemap++ = blankTile;
	}
	for(i = 0; i < 4; i++)
		BgScrollWork[i].x = BgScrollWork[i].y = 0;
	syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
	syWindowLoadGraphics();
	syDebugTextClear();
	if(prompt != 0) {
		u8 *buffer = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
		INTRO_SPR *sprite = state->sprs;

		syDecompressByteLz(Title_PromptTiles, buffer);
		for(i = 0; i < PROMPT_TILES; i++) {
			s32 vramBlock = syVramCacheLoad(syVramCacheFindFreeEntry(), 128,
											buffer + (i << 8) / 2);
			u32 *spriteWords = (u32 *)sprite;

			*spriteWords++ = 0;
			sprite++;
			*spriteWords++ = OBJ_SIZE_32X8;
			*spriteWords = vramBlock;
		}
		syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	}
	syScrFadeFromWhite(30);
	syScrFadeWait();
	REG_DISPCNT =
		DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	if(prompt != 0)
		holdFrames = TITLE_PROMPT_HOLD_FRAMES;
	else
		holdFrames = TITLE_HOLD_FRAMES;
	for(i = 0; i < holdFrames; i++) {
		if(!TLA_PATCH_1 || i > PROMPT_DELAY_FRAMES) {
			if(prompt != 0) {
				INTRO_SPR *sprite = state->sprs;
				s32 x;
				u32 level;
#ifdef PROMPT_ROWS
				u32 row;
				s32 y;

				for(row = 0, y = PROMPT_Y; row < PROMPT_ROWS;
					y += PROMPT_ROW_HEIGHT, row++) {
					for(j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
						sprite->x = x;
						sprite->y = y;
						syOamAddSprAtDepthClamped(sprite, 0);
						x += 32;
						sprite++;
					}
				}
#else
				for(j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
					sprite->x = x;
					sprite->y = PROMPT_Y;
					syOamAddSprAtDepthClamped(sprite, 0);
					x += 32;
					sprite++;
				}
#endif
				/* The prompt pulses once a second: its blend weight over the
				 * picture. */
				level = Title_PromptBlendLevels[i % PROMPT_PULSE_FRAMES];
				REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND |
							 BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 |
							 BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_BD;
				REG_BLDALPHA =
					(BLEND_WEIGHT_MAX - level) * BLEND_ALPHA(0, 1) + level;
			}
			if(InputWork.pressed & (KEY_A | KEY_START)) {
				result = 1;
				break;
			}
		}
		syRandom();
		sySynch(1);
	}
	syHeapSlotRelease(HEAP_SLOT_TITLE_INTRO);
	REG_BLDCNT = 0;
	REG_BLDALPHA = 0;
	sySynch(1);
	return result;
}

/* Unused: an empty entry the stub table keeps. */
void mnTitleBeforeLogosNoOp(void) {}

/* The publisher logo's first tile: character block 1's, as BG2 numbers its */
/* 256-colour tiles from block 0. */
#define PUBLISHER_LOGO_TILE (BG_CHAR_SIZE / TILE_SIZE_8BPP)

/* Shows the publisher's logo with its chime. With mode 0, which is all the */
/* title passes, it appears at once and holds two seconds; any other mode, */
/* unused, waits a second, fades in, holds three seconds and fades out, faster
 */
/* once skipped. Returns -1 when A or Start skipped it. */
s32 mnTitleShowPublisherLogo(s32 mode) {
	s32 result;
	u8 *data;
	u16 *map;
	u32 x;
	u32 y;
	u16 tile;
	s32 blank = OFFSCREEN_TILE;
	s32 resource;

	sySndPlayStub(SE_CHIME);
	OamCopyEnabled = 1;
	resource = RESOURCE_PUBLISHER_LOGO;
	syInitEvent();
	syScrFadeToBlack(1);
	syDebugTextClear();
	sySynch(1);
	REG_BG2CNT = LOGO_BGCNT;
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	result = 0;
	BgScrollWork[2].y = result;
	data = syResourceGetTableEntry(resource);
	syDMAXCopy(data, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
			   REG_DMA3);
	syDecompressTaggedLz(data + LOGO_PALETTE_BYTES, Ram_MapCellBuffer);
	syDMAXCopy(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(1),
			   DMA_COPY_WORDS(SCREEN_TILE_BYTES / 4), REG_DMA3);
	/* Pad the 30-tile picture to the hardware map's 32-tile row width. */
	tile = PUBLISHER_LOGO_TILE;
	map = LOGO_MAP;
	for(y = 0; y < DISPLAY_TILE_HEIGHT; y++) {
		for(x = 0; x < DISPLAY_TILE_WIDTH; x++)
			*map++ = tile++;
		*map++ = blank;
		*map++ = blank;
	}
	for(y = 0; y < 4; y++) {
		BgScrollWork[y].y = 0;
		BgScrollWork[y].x = 0;
	}
	syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
	syWindowLoadGraphics();
	syDebugTextClear();
	REG_DISPCNT =
		DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	if(mode == 0) {
		syScrFadeFromBlack(1);
		syScrFadeWait();
		for(y = 0; y < 2 * FRAMES_PER_SECOND; y++) {
			if(InputWork.pressed & (KEY_A | KEY_START)) {
				result = -1;
				break;
			}
			sySynch(1);
		}
		return result;
	}
	for(y = 0; y < FRAMES_PER_SECOND; y++) {
		if(InputWork.pressed & (KEY_A | KEY_START)) {
			result = -1;
			break;
		}
		sySynch(1);
	}
	if(result != 0)
		syScrFadeFromBlack(8);
	else
		syScrFadeFromBlack(FRAMES_PER_SECOND);
	syScrFadeWait();
	if(result == 0) {
		for(y = 0; y < 3 * FRAMES_PER_SECOND; y++) {
			if(InputWork.pressed & (KEY_A | KEY_START)) {
				result = -1;
				break;
			}
			sySynch(1);
		}
	}
	if(result != 0)
		syScrFadeToBlack(8);
	else
		syScrFadeToBlack(FRAMES_PER_SECOND);
	syScrFadeWait();
	return result;
}

/* The Camelot logo's flag: four pictures of 16 tiles after the logo's own, */
/* each shown 8 frames, whose first 13 tiles are copied over the logo's from */
/* its fifth tile. */
#define CAMELOT_FLAG_TILE		   4
#define CAMELOT_FLAG_PICTURE_BYTES (16 * TILE_SIZE_8BPP)
#define CAMELOT_FLAG_BYTES		   (13 * TILE_SIZE_8BPP)

/* Shows the Camelot logo for two seconds, cycling its four frames of tiles. A
 */
/* or Start ends it early. */
s32 mnTitleShowCamelotLogo(void) {
	s32 result;
	u32 i;
	u8 *buffer;
	s32 resource;

	OamCopyEnabled = 1;
	result = 0;
	resource = RESOURCE_CAMELOT_LOGO;
	syInitEvent();
	syScrFadeToBlack(1);
	syDebugTextClear();
	sySynch(1);
	REG_BG2CNT = LOGO_BGCNT | BGCNT_CHAR_BASE(1);
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	BgScrollWork[2].y = result;
	buffer = syResourceGetTableEntry(resource);
	syDecompressTaggedLz(buffer, Ram_MapCellBuffer);
	/* The unpacked resource: 224 colours, the map, the still tiles, then the */
	/* flag's four pictures. */
	buffer = Ram_MapCellBuffer;
	syDMAXCopy(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
			   REG_DMA3);
	buffer += LOGO_PALETTE_BYTES;
	syDMAXCopy(buffer, LOGO_MAP, DMA_COPY_WORDS(BG_SCREEN_SIZE / 4), REG_DMA3);
	buffer += BG_SCREEN_SIZE;
	syDMAXCopy(buffer, BG_CHAR_BLOCK(1), DMA_COPY_WORDS(BG_CHAR_SIZE / 4),
			   REG_DMA3);
	buffer += BG_CHAR_SIZE;
	for(i = 0; i < 4; i++) {
		BgScrollWork[i].y = 0;
		BgScrollWork[i].x = 0;
	}
	syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
	syWindowLoadGraphics();
	syDebugTextClear();
	syScrFadeFromWhite(1);
	syScrFadeWait();
	REG_DISPCNT =
		DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	for(i = 0; i < 2 * FRAMES_PER_SECOND; i++) {
		syDMAXCopy(((FrameCount >> 3) & 3) * CAMELOT_FLAG_PICTURE_BYTES +
					   buffer,
				   BG_CHAR_BLOCK(1) + CAMELOT_FLAG_TILE * TILE_SIZE_8BPP,
				   DMA_COPY_WORDS(CAMELOT_FLAG_BYTES / 4), REG_DMA3);
		if(InputWork.pressed & (KEY_A | KEY_START))
			break;
		sySynch(1);
	}
	return result;
}

/* Unused: an empty entry the stub table keeps. */
void mnTitleAfterIntroNoOp(void) {}

/* The palette fade (TITLE_PALETTE_FADE) keeps the saved palettes, the */
/* colour components shown now, the target components and each one's step. */

/* A component is a colour's five-bit channel shifted into the high bits of a */
/* halfword, with ten bits of fraction below it. */
#define COMPONENT_SHIFT 10

/* Computes the change per frame of each of the TITLE_PALETTE_FADE_CHANNELS */
/* colour components, from the current components to the target (nothing when */
/* `frames` is not positive). */
void mnTitlePaletteFadeCalcSteps(s16 *current, s16 *target, s16 *step,
								 s32 frames) {
	s32 n;
	s32 from;
	s32 to;

	if(frames > 0) {
		for(n = 0; n < TITLE_PALETTE_FADE_CHANNELS; n++) {
			from = *current;
			to = *target;
			*step = syIntegerDivide(to - from, frames);
			current++;
			target++;
			step++;
		}
	}
}

/* A task added by mnTitlePaletteFadeInit: steps the palette fade each */
/* frame, adding the step to every colour component until the fade is done, */
/* packs the components into the spare RGB555 copy of the palettes and queues */
/* both palettes for VBlank. */
void mnTitlePaletteFadeUpdate(void) {
	TITLE_PALETTE_FADE *state = Ram_HeapSlots->palFade;
	u16 *delta = state->delta;
	u16 *current;
	u16 *packed;
	s32 i;
	u16 *bank;
	s32 blueMask;

	if(state->duration == 0)
		return;
	if((s8)++state->step < state->duration) {
		current = state->cur;
		for(i = 0; i <= TITLE_PALETTE_FADE_CHANNELS - 1; i++)
			*current++ += *delta++;
	}
	else {
		syDMAXCopy(state->tgt, state->cur,
				   DMA_COPY_WORDS(sizeof(state->cur) / 4), REG_DMA3);
		state->duration = 0;
	}
	/* The queued DMA keeps a pointer until VBlank. */
	packed = state->packed[state->page ^ 1];
	blueMask = BLUE_BITS;
	i = PLTT_COLORS;
	current = state->cur;
	for(; i != 0; i--) {
		*packed++ = (current[0] & blueMask) |
					(((s16)current[1] >> (COMPONENT_SHIFT - RGB_GREEN_SHIFT)) &
					 GREEN_BITS) |
					(((s16)current[2] >> COMPONENT_SHIFT) & RED_BITS);
		current += 3;
	}
	state->page ^= 1;
	bank = state->packed[state->page];
	IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)bank,
						DMA_COPY_WORDS(BG_PLTT_SIZE / 4));
	IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(bank + BG_PLTT_COLORS),
						DMA_COPY_WORDS(BG_PLTT_SIZE / 4));
}

/* Tints 512 source colours into destination components (three per colour, each
 */
/* in the high bits of a halfword). An PALETTE_TINT picks the tint: a */
/* colour below PALETTE_TINT_FROM_SOURCE fills every colour with it; */
/* PALETTE_TINT_FROM_SOURCE takes that colour from source[0]; PALETTE_TINT_NONE
 */
/* to PALETTE_TINT_WARM_DARK are the whole-palette filters; the CUSTOM, BLEND */
/* and SPLIT flags carry a colour in their low bits; anything else is the */
/* address of components to copy. `selectedPalettes` is an enum */
/* PALETTE_SELECTION. */
void mnTitlePaletteFadeBuildCols(u32 tint, u16 *source, u16 *destination,
								 s32 selectedPalettes) {
	u32 colorIndex;
	u32 colorCount = PLTT_COLORS;
	s32 red;
	s32 blue;
	u32 color;
	s32 green;
	s32 tintedRed;

	if(tint == PALETTE_TINT_FROM_SOURCE)
		tint = *source;
	if(selectedPalettes == PALETTES_BG) {
		colorCount = BG_PLTT_COLORS;
	}
	else if(selectedPalettes == PALETTES_OBJECTS) {
		destination += 3 * BG_PLTT_COLORS;
		source += BG_PLTT_COLORS;
		colorCount = BG_PLTT_COLORS;
	}

	if(tint < PALETTE_TINT_FROM_SOURCE) {
		*destination++ = tint & BLUE_BITS;
		*destination++ = (tint & GREEN_BITS)
						 << (COMPONENT_SHIFT - RGB_GREEN_SHIFT);
		*destination++ = (tint & RED_BITS) << COMPONENT_SHIFT;
		syDMAXCopy(destination - 3, destination,
				   DMA_COPY_HALFWORDS(((colorCount - 1) * 6) >> 1), REG_DMA3);
	}
	else if(tint < PALETTE_TINT_FLAGS) {
		switch(tint) {
		case PALETTE_TINT_GRAYSCALE:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				color = *source++;
				red = ((color << BRIGHTNESS_RED_SHIFT) &
					   (RED_BITS << BRIGHTNESS_RED_SHIFT));
				green = ((color << BRIGHTNESS_GREEN_SHIFT) &
						 (GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
				blue = (color & BLUE_BITS);
				color = syIntegerDivide(red + green + blue, 2 + 4 + 1);
				*destination++ = color;
				*destination++ = color;
				*destination++ = color;
			}
			break;
		case PALETTE_TINT_SEPIA:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				color = *source++;
				red = RGB_RED(color);
				green = RGB_GREEN(color);
				blue = RGB_BLUE(color);
				color =
					syIntegerDivide(red + green + blue, SEPIA_SHADE_DIVISOR);
				red = color * 4 + 5;
				blue = color * 3 + 5;
				green = color * 3 + 5;
				if(red < SEPIA_LEVEL_MIN)
					red = SEPIA_LEVEL_MIN;
				if(green < SEPIA_LEVEL_MIN)
					green = SEPIA_LEVEL_MIN;
				if(blue < SEPIA_LEVEL_MIN)
					blue = SEPIA_LEVEL_MIN;
				if(red > SEPIA_LEVEL_MAX)
					red = SEPIA_LEVEL_MAX;
				if(green > SEPIA_LEVEL_MAX)
					green = SEPIA_LEVEL_MAX;
				if(blue > SEPIA_LEVEL_MAX)
					blue = SEPIA_LEVEL_MAX;
				*destination++ = TitlePaletteFade_ChannelCurveLight[blue];
				*destination++ = TitlePaletteFade_ChannelCurveLight[green];
				*destination++ = TitlePaletteFade_ChannelCurveLight[red];
			}
			break;
		/* The same as PALETTE_TINT_WARM_DARK below, where the field's fade
		 * warms */
		/* more. */
		case PALETTE_TINT_WARM:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				color = *source++;
				red = RGB_RED(color);
				green = RGB_GREEN(color);
				blue = RGB_BLUE(color);
				red -= (u32)red >> 1;
				green -= green / 3;
				red += 6;
				green += 4;
				blue -= 6;
				red = mnTitlePaletteFadeClampChannel(red);
				green = mnTitlePaletteFadeClampChannel(green);
				blue = mnTitlePaletteFadeClampChannel(blue);
				SET_VECTOR(destination,
						   TitlePaletteFade_ChannelCurveContrast[blue],
						   TitlePaletteFade_ChannelCurveLight[green],
						   TitlePaletteFade_ChannelCurve[red]);
				destination += 3;
			}
			break;
		case PALETTE_TINT_MUTED:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				color = *source++;
				red = RGB_RED(color);
				green = RGB_GREEN(color);
				blue = RGB_BLUE(color);
				if(red < MUTED_RED_MIN)
					red = MUTED_RED_MIN;
				if(green < MUTED_GREEN_MIN)
					green = MUTED_GREEN_MIN;
				if(blue < MUTED_BLUE_MIN)
					blue = MUTED_BLUE_MIN;
				if(red > MUTED_RED_MAX)
					red = MUTED_RED_MAX;
				if(green > MUTED_GREEN_MAX)
					green = MUTED_GREEN_MAX;
				if(blue > MUTED_BLUE_MAX)
					blue = MUTED_BLUE_MAX;
				green += 2;
				blue += 2;
				red = mnTitlePaletteFadeClampChannel(red);
				green = mnTitlePaletteFadeClampChannel(green);
				blue = mnTitlePaletteFadeClampChannel(blue);
				*destination++ = TitlePaletteFade_ChannelCurve[blue];
				*destination++ = TitlePaletteFade_ChannelCurve[green];
				*destination++ = TitlePaletteFade_ChannelCurve[red];
			}
			break;
		case PALETTE_TINT_PALE:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				s32 mean;

				color = *source++;
				red = RGB_RED(color);
				green = RGB_GREEN(color);
				blue = RGB_BLUE(color);
				mean = mnTitlePaletteFadeClampChannel((red + green + blue) / 3);
				red = (red >> 1) + mean;
				green = (green >> 1) + mean;
				blue = (blue >> 1) + mean;
				red = mnTitlePaletteFadeClampChannel(red);
				green = mnTitlePaletteFadeClampChannel(green);
				blue = mnTitlePaletteFadeClampChannel(blue);
				SET_VECTOR(destination,
						   TitlePaletteFade_ChannelCurveContrast[blue],
						   TitlePaletteFade_ChannelCurveContrast[green],
						   TitlePaletteFade_ChannelCurveContrast[red]);
				destination += 3;
			}
			break;
		case PALETTE_TINT_RED:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				color = *source++;
				green = RGB_GREEN(color);
				red = RGB_RED(color);
				blue = RGB_BLUE(color);
				red += ((green >> 3) + (blue >> 3));
				red = mnTitlePaletteFadeClampChannel(red);
				green -= green / 3;
				blue -= blue / 3;
				SET_VECTOR(destination, TitlePaletteFade_ChannelCurve[blue],
						   TitlePaletteFade_ChannelCurve[green],
						   TitlePaletteFade_ChannelCurveLight[red]);
				destination += 3;
			}
			break;
		case PALETTE_TINT_WARM_DARK:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				color = *source++;
				red = RGB_RED(color);
				green = RGB_GREEN(color);
				blue = RGB_BLUE(color);
				red -= (u32)red >> 1;
				green -= green / 3;
				red += 6;
				green += 4;
				blue -= 6;
				red = mnTitlePaletteFadeClampChannel(red);
				green = mnTitlePaletteFadeClampChannel(green);
				blue = mnTitlePaletteFadeClampChannel(blue);
				SET_VECTOR(destination,
						   TitlePaletteFade_ChannelCurveContrast[blue],
						   TitlePaletteFade_ChannelCurveLight[green],
						   TitlePaletteFade_ChannelCurve[red]);
				destination += 3;
			}
			break;
		default:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				color = *source++;
				destination[colorIndex * 3 + 0] = color & BLUE_BITS;
				destination[colorIndex * 3 + 1] =
					(color & GREEN_BITS) << (COMPONENT_SHIFT - RGB_GREEN_SHIFT);
				destination[colorIndex * 3 + 2] = (color & RED_BITS)
												  << COMPONENT_SHIFT;
			}
			break;
		}
	}
	else if(tint & PALETTE_TINT_CUSTOM) {
		u32 tintRed;
		u32 tintGreen;
		u32 tintBlue;

		tintRed = RGB_RED(tint);
		tintGreen = RGB_GREEN(tint);
		tintBlue = RGB_BLUE(tint);
		for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
			color = *source++;
			red = ((color << BRIGHTNESS_RED_SHIFT) &
				   (RED_BITS << BRIGHTNESS_RED_SHIFT));
			green = ((color << BRIGHTNESS_GREEN_SHIFT) &
					 (GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
			blue = (color & BLUE_BITS);
			color = syIntegerDivide(red + green + blue, CUSTOM_TINT_DIVISOR);
			tintedRed = color * tintRed;
			green = color * tintGreen;
			blue = color * tintBlue;
			tintedRed = mnTitlePaletteFadeClampShiftedChannel(tintedRed);
			green = mnTitlePaletteFadeClampShiftedChannel(green);
			blue = mnTitlePaletteFadeClampShiftedChannel(blue);
			destination[colorIndex * 3 + 0] = blue;
			destination[colorIndex * 3 + 1] = green;
			destination[colorIndex * 3 + 2] = tintedRed;
		}
	}
	else if(tint & PALETTE_TINT_BLEND) {
		u32 tintRed;
		u32 tintGreen;
		u32 tintBlue;
		s32 scaledGreen;
		s32 scaledBlue;
		s32 clampedRed;
		s32 clampedGreen;
		s32 clampedBlue;

		tintRed = RGB_RED(tint);
		tintGreen = RGB_GREEN(tint);
		tintBlue = RGB_BLUE(tint);
		for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
			color = *source++;
			red = RGB_RED(color);
			green = RGB_GREEN(color);
			blue = RGB_BLUE(color);
			color = syIntegerDivide((red + green + blue) << 4,
									tintRed + tintGreen + tintBlue);
			tintedRed = syMulFX(INTtoFIXED((color * tintRed) >> 4),
								(s32)INTtoFIXED(tintRed) >> 4);
			scaledGreen = syMulFX(INTtoFIXED((color * tintGreen) >> 4),
								  (s32)INTtoFIXED(tintGreen) >> 4);
			scaledBlue = syMulFX(INTtoFIXED((color * tintBlue) >> 4),
								 (s32)INTtoFIXED(tintBlue) >> 4);
			tintedRed = FIXEDtoINT((u32)tintedRed);
			clampedRed = mnTitlePaletteFadeClampChannel(tintedRed);
			scaledGreen = FIXEDtoINT((u32)scaledGreen);
			clampedGreen = mnTitlePaletteFadeClampChannel(scaledGreen);
			scaledBlue = FIXEDtoINT((u32)scaledBlue);
			clampedBlue = mnTitlePaletteFadeClampChannel(scaledBlue);
			destination[colorIndex * 3 + 0] =
				TitlePaletteFade_ChannelCurve[clampedBlue];
			destination[colorIndex * 3 + 1] =
				TitlePaletteFade_ChannelCurve[clampedGreen];
			destination[colorIndex * 3 + 2] =
				TitlePaletteFade_ChannelCurve[clampedRed];
		}
	}
	else if(tint & PALETTE_TINT_SPLIT) {
		for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
			color = *source++;
			destination[colorIndex * 3 + 0] = color & BLUE_BITS;
			destination[colorIndex * 3 + 1] =
				(color & GREEN_BITS) << (COMPONENT_SHIFT - RGB_GREEN_SHIFT);
			destination[colorIndex * 3 + 2] = (color & RED_BITS)
											  << COMPONENT_SHIFT;
		}
	}
	else {
		if(selectedPalettes == PALETTES_OBJECTS)
			tint +=
				3 * BG_PLTT_SIZE; /* past the background colours' components */
		syDMAXCopy((const void *)tint, destination,
				   DMA_COPY_WORDS((colorCount * 6) >> 2), REG_DMA3);
	}
}

/* Allocates the palette fade, saves both hardware palettes in it as the */
/* starting colours and adds the task mnTitlePaletteFadeUpdate. */
void mnTitlePaletteFadeInit(void) {
	TITLE_PALETTE_FADE *buffer;

	buffer = syHeapSlotAllocEwram(HEAP_SLOT_PALETTE_FADE, sizeof(*buffer));
	btDmaFill32(0, buffer, sizeof(*buffer));
	syDMAXCopy((void *)BG_PLTT, buffer, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
			   REG_DMA3);
	syDMAXCopy((void *)OBJ_PLTT, &buffer->pal[BG_PLTT_COLORS],
			   DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
	mnTitlePaletteFadeBuildCols(PALETTE_TINT_NONE, buffer->pal, buffer->tgt,
								PALETTES_ALL);
	sySetEvent(mnTitlePaletteFadeUpdate, TASK_PASS_FRAME);
}

/* Unused: removes the palette fade's task and frees its buffers. */
UNUSED void mnTitlePaletteFadeRelease(void) {
	syCloseEvent(&mnTitlePaletteFadeUpdate);
	syHeapSlotRelease(HEAP_SLOT_PALETTE_FADE);
}

/* Sets the fade target by tinting the saved background and/or sprite palettes
 */
/* with the requested tint. */
void mnTitlePaletteFadeSetTarget(s32 tint, s32 selectedPalettes) {
	TITLE_PALETTE_FADE *fade = Ram_HeapSlots->palFade;

	if(fade != NULL)
		mnTitlePaletteFadeBuildCols(tint, fade->pal, fade->tgt,
									selectedPalettes);
}

/* Sets the colours shown now by tinting the saved palettes. */
void mnTitlePaletteFadeSetCurrent(s32 tint, s32 selectedPalettes) {
	TITLE_PALETTE_FADE *fade = Ram_HeapSlots->palFade;

	if(fade != NULL)
		mnTitlePaletteFadeBuildCols(tint, fade->pal, fade->cur,
									selectedPalettes);
}

/* Unused: replaces the first saved colour, the backdrop. */
UNUSED void mnTitlePaletteFadeSetBackdrop(s32 color) {
	TITLE_PALETTE_FADE *fade = Ram_HeapSlots->palFade;

	if(fade != NULL)
		fade->pal[0] = color;
}

/* Starts fading the shown colours to the target over the requested frames: */
/* each of the components (three for each of the 512 colours) steps by */
/* its distance divided by the frames, the remainder kept in the ten bits below
 */
/* it, so every colour arrives together. */
void mnTitlePaletteFadeStart(s32 frames) {
	TITLE_PALETTE_FADE *fade = Ram_HeapSlots->palFade;

	if(fade != NULL) {
		fade->duration = frames;
		fade->step = 0;
		mnTitlePaletteFadeCalcSteps((s16 *)fade->cur, (s16 *)fade->tgt,
									(s16 *)fade->delta, frames);
	}
}

/* Holds a colour channel's level to 0 to 31. */
s32 mnTitlePaletteFadeClampChannel(s32 value) {
	if(value > RGB_LEVEL_MAX)
		return RGB_LEVEL_MAX;
	if(value < 0)
		value = 0;
	return value;
}

/* Caps a colour component at a channel's brightest level shifted into the high
 */
/* bits. There is no lower bound. */
s32 mnTitlePaletteFadeClampShiftedChannel(s32 value) {
	if(value > RGB_LEVEL_MAX << COMPONENT_SHIFT)
		value = RGB_LEVEL_MAX << COMPONENT_SHIFT;
	return value;
}

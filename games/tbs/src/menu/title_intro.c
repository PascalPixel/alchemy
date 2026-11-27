/* The title intro, the view rising up a peak towards the sun and its lens flare
   until the flash into the title picture and its "Press Start" prompt; the
   publisher's and Camelot's logos; and the palette fade the intro uses. */

#include "menu/title_intro.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "field/map_grid_scroll.h"
#include "game/game.h"
#include "graphics/graphics.h"

/* A sprite of the "Press Start" prompt, linked into a render slot each tick:
   OAM attributes 0 to 2 with the position split out. */
typedef struct INTRO_SPR {
	Uint32 next; /* the render slot link */
	Uint8  y;
	Uint8  attr0High; /* shape and mode bits of attribute 0 */
	Uint16 x : 9;
	Uint16 attr1High : 7; /* flip and size bits of attribute 1 */
	Sint32 tile;
} INTRO_SPR;

/* The intro's ticks: A or START skips to the approach only after the first
   INTRO_SKIP_AFTER_TICK, and the screen flashes white into the title at
   INTRO_FLASH_TICK. */
#define INTRO_SKIP_AFTER_TICK 20
#define INTRO_APPROACH_TICK	  239
#define INTRO_FLASH_TICK	  280

/* INTRO_STATE.state, which the intro writes and nothing reads. */
typedef enum INTRO_PHASE {
	INTRO_STATE_SCROLLING = 0, /* the pictures scroll in */
	/* From tick 239, or a skip with A or START: the approach to the flash. */
	INTRO_STATE_APPROACH = 1,
	/* Set from tick 281, which the loop never reaches: it ends at 280. */
	INTRO_STATE_AFTER_FLASH = 2
} INTRO_PHASE;

/* OAM entries the intro builds each VBlank. */
#define INTRO_OBJECT_COUNT 120

/* The most OAM entries the lens flare takes in a VBlank; the entry after them
   is always hidden, and the sun is built there. */
#define FLARE_OBJECT_COUNT 12

/* The title intro's state, in HEAP_SLOT_TITLE_INTRO. */
typedef struct INTRO_STATE {
	Sint32 backRows;  /* rows of the clouds copied in as they scroll */
	Sint32 frontRows; /* rows of the peak copied in */
	/* The intro's tick, which a skip moves on to INTRO_APPROACH_TICK */
	Sint32 frm;
	/* Frames counted by mnTitleUpdateIntroSprs, which pace the scroll and the
	   flare's rise */
	Sint32 frmCnt;
	Sint32 state; /* an INTRO_PHASE */
	Sint32 rise;  /* how far the lens flare's lower end has risen */

	union {
		OAM_WORDS objs[INTRO_OBJECT_COUNT];

		struct {
			/* The lens flare's entries and the sun's, still the intro's. */
			OAM_WORDS flareObjs[FLARE_OBJECT_COUNT + 1];
			INTRO_SPR sprs[8];
		} prompt;
	} area;
} INTRO_STATE;

/* The intro's two pictures share character block 0: the clouds (BG1) in
   16-colour tiles, the peak (BG2) in 256-colour tiles after them. */
#define INTRO_BACK_TILE(n)	(BG_CHAR_BLOCK(0) + (n) * TILE_SIZE_4BPP)
#define INTRO_FRONT_TILE(n) (BG_CHAR_BLOCK(0) + (n) * 2 * TILE_SIZE_4BPP)

/* The screen blocks of the two pictures' maps */
#define INTRO_FRONT_SCREEN 30
#define INTRO_BACK_SCREEN  31
/* Both maps are 32 rows of tiles. */
#define INTRO_MAP_ROWS	   32
/* The screen block of the map of the title picture and of the logos. */
#define PICTURE_SCREEN	   6
/* Both pictures start scrolled this far down and scroll up into place. */
#define INTRO_SCROLL_START 96

/* The intro's pictures unpack one after the other: the clouds, 33 rows of tiles
   in 16 colours, then the peak, 28 rows in 256 colours, each row a screen
   wide. */
#define CLOUD_ROWS		33
#define CLOUD_ROW_BYTES (DISPLAY_TILE_WIDTH * TILE_SIZE_4BPP)
#define PEAK_ROW_BYTES	(DISPLAY_TILE_WIDTH * TILE_SIZE_8BPP)
#define CLOUD_ROW(row)	(Ram_MapGridMetatileBuffer + (row) * CLOUD_ROW_BYTES)
#define PEAK_ROW(row)	(CLOUD_ROW(CLOUD_ROWS) + (row) * PEAK_ROW_BYTES)

/* Each picture keeps 21 rows of tiles in VRAM, the screen's 20 and the one
   coming into view, and its map's 32 rows wrap round them: map row
   INTRO_FIRST_MAP_ROW, the one above the screen at the start, shows the
   first. */
#define INTRO_VRAM_ROWS		(DISPLAY_TILE_HEIGHT + 1)
#define INTRO_FIRST_MAP_ROW (INTRO_SCROLL_START / 8 - 1)
/* The peak's first tile, after the clouds' in character block 0; it is
   blank. */
#define PEAK_FIRST_TILE		(INTRO_VRAM_ROWS * CLOUD_ROW_BYTES / TILE_SIZE_8BPP)
#define CLOUD_VRAM_ROW(row) INTRO_BACK_TILE((row) * DISPLAY_TILE_WIDTH)
#define PEAK_VRAM_ROW(row) \
	INTRO_FRONT_TILE(PEAK_FIRST_TILE + (row) * DISPLAY_TILE_WIDTH)
/* The peak's top stands this many rows down the map, below the clouds' top. */
#define PEAK_MAP_ROW	6
/* The peak's rows above those in VRAM at the start, copied in as they come into
   view; its top row is blank and never copied. */
#define PEAK_ROWS_ABOVE 4

/* The x of each of the lens flare's six pieces */
const Uint8 TitleFlareColumns[] = {96, 136, 140, 160, 168, 184};

/* The prompt pulses once a second. */
#define PROMPT_PULSE_TICKS TICKS_PER_SECOND

/* The prompt's blend level over its pulse */
/* clang-format off */
const Uint8 TitlePromptBlendLevels[PROMPT_PULSE_TICKS] = {
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
};
/* clang-format on */

#define INTRO_OBJ(i) (state->area.objs[i])

/* Brings `v` into a sprite's y, 0 to 255, by whole turns */
#define WRAP_OBJ_Y(v) \
	while((v) > OBJ_Y_RANGE - 1) \
		(v) -= OBJ_Y_RANGE; \
	while((v) < 0) \
	(v) += OBJ_Y_RANGE

/* The lens flare's pieces lie on a line from lineLeftY at this x to lineRightY
   at FLARE_LINE_RIGHT. */
#define FLARE_LINE_LEFT	 104
#define FLARE_LINE_RIGHT 184

/* The lens flare's pictures in the sprite tiles, after the sun's: an orange
   disc whole, and quarters, mirrored into the rest, of a rainbow ring, a
   yellow-green ring, a green disc with a cyan heart and a white-hot disc. */
#define SUN_TILE				0
#define FLARE_ORANGE_DISC_TILE	128
#define FLARE_RAINBOW_RING_TILE 160
#define FLARE_GREEN_RING_TILE	192
#define FLARE_GREEN_DISC_TILE	224
#define FLARE_WHITE_DISC_TILE	232
/* The sprite tiles end with the white disc's eight. */
#define INTRO_SPRITE_TILES		(FLARE_WHITE_DISC_TILE + 8)
/* A disc is 32 pixels across, a ring 64. */
#define FLARE_DISC_RADIUS		16
#define FLARE_RING_RADIUS		32

/* Sets x and y to the top left of a piece of `radius` centred on column
   `column`, on the flare's line. */
#define PLACE_ON_FLARE_LINE(column, radius) \
	x = TitleFlareColumns[column] - (radius); \
	y = (lineRightY - lineLeftY) * ((x + (radius)) - FLARE_LINE_LEFT) / \
			(FLARE_LINE_RIGHT - FLARE_LINE_LEFT) + \
		lineLeftY - (radius); \
	WRAP_OBJ_Y(y)

/* Writes OAM entries i to i + 3: four sprites of `size` pixels, mirrored into
   the four quarters of one picture at (x, y) */
#define MIRRORED_QUAD(i, size, shape, tile) \
	INTRO_OBJ(i).attr01 = OBJ_X(x + 4) | y | (shape) | \
		OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) | OBJ_256_COLORS; \
	INTRO_OBJ((i) + 1).attr01 = OBJ_X(x + 4 + (size)) | y | (shape) | \
		OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) | OBJ_256_COLORS | OBJ_HFLIP; \
	INTRO_OBJ((i) + 2).attr01 = OBJ_X(x + 4) | (Uint8)(y + (size)) | (shape) | \
		OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) | OBJ_256_COLORS | OBJ_VFLIP; \
	INTRO_OBJ((i) + 3).attr01 = OBJ_X(x + 4 + (size)) | (Uint8)(y + (size)) | \
		(shape) | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) | OBJ_256_COLORS | \
		OBJ_HFLIP | OBJ_VFLIP; \
	INTRO_OBJ(i).attr2 = (tile); \
	INTRO_OBJ((i) + 1).attr2 = (tile); \
	INTRO_OBJ((i) + 2).attr2 = (tile); \
	INTRO_OBJ((i) + 3).attr2 = (tile)

/* Where the sun stands over the clouds, which it scrolls with. */
#define SUN_X 92
#define SUN_Y 32

/* A VBlank event of the title intro, added by mnTitleShowTitleScreen: places
   the lens flare's pieces along a line down from the sun, which swings up as
   the intro goes on, alternating two sets of three every other VBlank so all
   six show, hides the rest, adds the sun and copies the list to OAM. */
void mnTitleUpdateIntroSprs(void)
{
	INTRO_STATE *state;
	Uint32		 sprCnt;
	Sint32		 lineLeftY;
	Sint32		 lineRightY;
	Sint32		 x;
	Sint32		 y;
	Sint32		 sunY;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_TITLE_INTRO);
	sprCnt = 0;
	if(DebugPaused == 0) {
		if((++state->frmCnt & 3) == 0)
			state->rise++;
	}
	lineLeftY = 48 - BgScrollWork[1].y;
	lineRightY = 144 - state->rise;
	if(state->frm < INTRO_FLASH_TICK) {
		if((state->frmCnt & 1) == 0) {
			PLACE_ON_FLARE_LINE(0, FLARE_DISC_RADIUS);
			MIRRORED_QUAD(sprCnt, FLARE_DISC_RADIUS, OBJ_SIZE_16X16,
				FLARE_WHITE_DISC_TILE);
			sprCnt += 4;
			PLACE_ON_FLARE_LINE(2, FLARE_DISC_RADIUS);
			INTRO_OBJ(sprCnt).attr01 = OBJ_X(x + 4) | y | OBJ_SIZE_32X32 |
				OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) | OBJ_256_COLORS;
			INTRO_OBJ(sprCnt).attr2 = FLARE_ORANGE_DISC_TILE;
			sprCnt++;
			PLACE_ON_FLARE_LINE(4, FLARE_RING_RADIUS);
			MIRRORED_QUAD(sprCnt, FLARE_RING_RADIUS, OBJ_SIZE_32X32,
				FLARE_GREEN_RING_TILE);
			sprCnt += 4;
		} else {
			PLACE_ON_FLARE_LINE(1, FLARE_DISC_RADIUS);
			MIRRORED_QUAD(sprCnt, FLARE_DISC_RADIUS, OBJ_SIZE_16X16,
				FLARE_WHITE_DISC_TILE);
			sprCnt += 4;
			PLACE_ON_FLARE_LINE(3, FLARE_DISC_RADIUS);
			MIRRORED_QUAD(sprCnt, FLARE_DISC_RADIUS, OBJ_SIZE_16X16,
				FLARE_GREEN_DISC_TILE);
			sprCnt += 4;
			PLACE_ON_FLARE_LINE(5, FLARE_RING_RADIUS);
			MIRRORED_QUAD(sprCnt, FLARE_RING_RADIUS, OBJ_SIZE_32X32,
				FLARE_RAINBOW_RING_TILE);
			sprCnt += 4;
		}
	}
	for(; sprCnt < INTRO_OBJECT_COUNT; sprCnt++)
		INTRO_OBJ(sprCnt).attr01 =
			DISPLAY_HEIGHT | OBJ_SIZE_16X16 | OBJ_256_COLORS;
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDALPHA = BLEND_ALPHA(14, 14);
	syDMAXCopy(&INTRO_OBJ(0), OAM, DMA_COPY_WORDS((sprCnt * 8) >> 2), REG_DMA3);
	/* The sun goes in the OAM entry after the list. */
	sunY = SUN_Y - BgScrollWork[1].y;
	WRAP_OBJ_Y(sunY);
	INTRO_OBJ(FLARE_OBJECT_COUNT).attr01 =
		sunY | OBJ_X(SUN_X) | OBJ_SIZE_64X64 | OBJ_256_COLORS;
	INTRO_OBJ(FLARE_OBJECT_COUNT).attr2 = OBJ_PRIORITY(2) | SUN_TILE;
	syDMAXCopy(&INTRO_OBJ(FLARE_OBJECT_COUNT), (Uint8 *)OAM + sprCnt * 8,
		DMA_COPY_WORDS(2), REG_DMA3);
	/* DEAD: the first four entries copied to OAM again, unchanged; the ROM
	   keeps the copy. */
	syDMAXCopy(&INTRO_OBJ(0), OAM, DMA_COPY_WORDS(8), REG_DMA3);
}

/* Writes map rows `first` to `last` at tilemap: each row the screen's width of
   tiles numbered on from tileIndex, then two columns past the edge that show
   offscreenTile. */
#define FILL_INTRO_MAP_ROWS(first, last) \
	for(row = (first); row <= (last); row++) { \
		for(column = 0; column < DISPLAY_TILE_WIDTH; column++) \
			*tilemap++ = tileIndex++; \
		*tilemap++ = offscreenTile; \
		*tilemap++ = offscreenTile; \
	}

/* Loads the title intro's two scrolling images, the clouds and the peak, and
   the sun's and lens flare's sprite tiles, builds the images' maps and sets the
   blending and windows. */
void mnTitleLoadIntroBackgrounds(void)
{
	INTRO_STATE *introState;
	Uint8		*resourceData;
	Uint16		*tilemap;
	Sint32		 column;
	Sint32		 row;
	Uint16		 tileIndex;

	introState = HEAP_SLOT_BLOCK(HEAP_SLOT_TITLE_INTRO);
	REG_DISPCNT = 0;
	resourceData = syResourceGetTableEntry(RESOURCE_INTRO_SUN);
	syDMAXCopy(resourceData, (void *)OBJ_PLTT, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
		REG_DMA3);
	OBJ_PLTT[0] = 0;
	resourceData += BG_PLTT_SIZE;
	syDecompressByteLZ(resourceData, Ram_MapGridMetatileBuffer);
	syDMAXCopy(Ram_MapGridMetatileBuffer, (void *)OBJ_VRAM0,
		DMA_COPY_HALFWORDS(INTRO_SPRITE_TILES * TILE_SIZE_4BPP / 2), REG_DMA3);
	resourceData = syResourceGetTableEntry(RESOURCE_INTRO_PEAK);
	syDMAXCopy(resourceData, (void *)BG_PLTT, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
		REG_DMA3);
	BG_PLTT[0] = 0;
	resourceData += BG_PLTT_SIZE;
	syDecompressByteLZ(resourceData, Ram_MapGridMetatileBuffer);
	syDMAXCopy(CLOUD_ROW(INTRO_FIRST_MAP_ROW), CLOUD_VRAM_ROW(0),
		DMA_COPY_HALFWORDS(INTRO_VRAM_ROWS * CLOUD_ROW_BYTES / 2), REG_DMA3);
	syDMAXCopy(PEAK_ROW(INTRO_FIRST_MAP_ROW - PEAK_MAP_ROW), PEAK_VRAM_ROW(0),
		DMA_COPY_HALFWORDS(INTRO_VRAM_ROWS * PEAK_ROW_BYTES / 2), REG_DMA3);

	{
		Sint32 offscreenTile = OFFSCREEN_TILE;

		/* Map rows from INTRO_FIRST_MAP_ROW show the VRAM rows from the first;
		   the rows above it show the last ones. */
		tilemap = (Uint16 *)BG_SCREEN_BLOCK(INTRO_FRONT_SCREEN);
		tileIndex = PEAK_FIRST_TILE +
			(INTRO_VRAM_ROWS - INTRO_FIRST_MAP_ROW) * DISPLAY_TILE_WIDTH;
		FILL_INTRO_MAP_ROWS(0, INTRO_FIRST_MAP_ROW - 1);
		tileIndex = PEAK_FIRST_TILE;
		FILL_INTRO_MAP_ROWS(INTRO_FIRST_MAP_ROW, INTRO_MAP_ROWS - 1);
		tilemap = (Uint16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN);
		tileIndex =
			(INTRO_VRAM_ROWS - INTRO_FIRST_MAP_ROW) * DISPLAY_TILE_WIDTH;
		FILL_INTRO_MAP_ROWS(0, INTRO_FIRST_MAP_ROW - 1);
		tileIndex = 0;
		FILL_INTRO_MAP_ROWS(INTRO_FIRST_MAP_ROW, INTRO_MAP_ROWS - 1);

		REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_MOSAIC |
			BGCNT_SCREEN_BASE(INTRO_BACK_SCREEN);
		REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(INTRO_FRONT_SCREEN);
		REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
		REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT - 1);
		REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
		REG_WIN1V = WIN_RANGE(0, DISPLAY_HEIGHT - 1);
		REG_WININ = WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ |
			WININ_WIN1_BG1 | WININ_WIN1_BG2 | WININ_WIN1_OBJ;
		for(row = 0; row < 4; row++) {
			BgScrollWork[row].y = 0;
			BgScrollWork[row].x = 0;
		}
		BgScrollWork[1].y = INTRO_SCROLL_START;
		BgScrollWork[2].y = INTRO_SCROLL_START;
		introState->frm = 0;
		introState->backRows = 0;
		introState->frontRows = 0;
		introState->frmCnt = 0;
		introState->rise = 0;
		introState->state = INTRO_STATE_SCROLLING;
		syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
		/* DEAD: the first BLDCNT, a lighten effect, at once replaced by the
		   blend; the ROM keeps the write. */
		REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL;
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX);
		REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	}
}

/* How long the title waits for A or START: a minute with the prompt shown, five
   seconds without. */
#define TITLE_PROMPT_HOLD_TICKS (60 * TICKS_PER_SECOND)
#define TITLE_HOLD_TICKS		(5 * TICKS_PER_SECOND)

/* The "Press Start" prompt, by edition: the 32-by-8 pictures it loads
   (PROMPT_TILES), its rows and the sprites in each, and where it stands. */
#define PROMPT_TILES GERMAN_OR(8, 5)
#if LANGUAGE == LANGUAGE_GERMAN
#define PROMPT_ROWS		  2
#define PROMPT_ROW_HEIGHT 8
#define PROMPT_SPRITES	  3
#define PROMPT_X		  80
#define PROMPT_Y		  116
#elif LANGUAGE == LANGUAGE_JAPANESE
#define PROMPT_SPRITES 5
#define PROMPT_X	   56
#define PROMPT_Y	   126
#elif LANGUAGE == LANGUAGE_FRENCH
#define PROMPT_SPRITES 5
#define PROMPT_X	   56
#define PROMPT_Y	   124
#else
#define PROMPT_SPRITES 3
#define PROMPT_X	   80
#define PROMPT_Y	   124
#endif

/* The title picture's first tile: character block 1's, as BG2 numbers its
   256-colour tiles from block 0. */
#define TITLE_PICTURE_TILE	 (BG_CHAR_SIZE / TILE_SIZE_8BPP)
/* The title picture's colours, palettes 0 to 14; the last is the window's. */
#define TITLE_PICTURE_COLORS (15 * PALETTE_COLORS)
/* A prompt sprite's picture: 32 by 8 pixels in 16 colours. */
#define PROMPT_SPRITE_BYTES	 (4 * TILE_SIZE_4BPP)

/* Plays the title intro: the view rises up the peak towards the sun, the clouds
   slower behind it, while the sun's lens flare swings up, until just before
   INTRO_FLASH_TICK (A or Start skips ahead); then the screen flashes white
   into the title picture, the Golden Sun logo over the peak. That holds a
   minute with the "Press Start" prompt pulsing when `prompt` is set, five
   seconds without, and returns 1 when A or Start ends it, else 0. */
Sint32 mnTitleShowTitleScreen(Sint32 prompt)
{
	INTRO_STATE *introState;
	Bool		 result;
	Uint8		 savedCpuLoadDisplay;
	Uint16		 clearValue;
	Uint8		*resourceData;
	Uint16		*tilemap;
	Uint32		 i;
	Uint16		 tile;
	Uint32		 j;
	Uint32		 holdTicks;
	Sint32		 offscreenTile = OFFSCREEN_TILE;

	result = FALSE;
	savedCpuLoadDisplay = CpuLoadDisplayEnabled;
	/* BUG: the block is sized for the prompt's sprites, but the intro's OAM
	   list runs on past its end. */
#if FIX_TITLE_INTRO_STATE_ALLOCATION
	introState = syHeapSlotAlloc(HEAP_SLOT_TITLE_INTRO, sizeof(INTRO_STATE));
#else
	introState = syHeapSlotAlloc(HEAP_SLOT_TITLE_INTRO,
		offsetof(INTRO_STATE, area) + sizeof(introState->area.prompt));
#endif
	syPrintClear();
	syVRAMCacheReset();
	sySynch(1);
	syInitEvent();
	/* The intro writes OAM itself in VBlank. */
	OamCopyEnabled = 0;
	CpuLoadDisplayEnabled = 0;
	mnTitleLoadIntroBackgrounds();
	/* The pictures fade up from a near-black red over a second. */
	mnTitlePaletteFadeInit();
	mnTitlePaletteFadeSetCurrent(RGB(2, 0, 0), PALETTES_ALL);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON |
			DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON |
			DISPCNT_OBJWIN_ON,
		IO_WRITE_HALFWORD);
	mnTitlePaletteFadeStart(60);
	sySetEvent(mnTitleUpdateIntroSprs, EVENT_PASS_VBLANK);

	for(;;) {
		Sint32 frm;
		Sint32 nextFrm;
		Sint32 frmCnt;

		/* A or Start skips to the approach, which still plays out to the
		   flash. */
		frm = introState->frm;
		if(frm > INTRO_SKIP_AFTER_TICK && frm < INTRO_APPROACH_TICK &&
			(PerPush & (KEY_A | KEY_START)) != 0) {
			introState->state = INTRO_STATE_APPROACH;
			introState->frm = INTRO_APPROACH_TICK;
			frm = INTRO_APPROACH_TICK;
		}
		nextFrm = frm + 1;
		introState->frm = nextFrm;
		if(nextFrm <= INTRO_FLASH_TICK - 2) {
			/* The clouds (BG1) scroll a pixel every third tick and the peak
			   (BG2) every other one; as a new row of tiles comes into view, its
			   row of the picture is copied over the VRAM row leaving at the
			   bottom. */
			frmCnt = introState->frmCnt;
			if(frmCnt % 3 == 0) {
				Sint32 y;

				BgScrollWork[1].y--;
				y = BgScrollWork[1].y;
				if((y & 7) == 0) {
					syDMAXCopy(CLOUD_ROW(INTRO_FIRST_MAP_ROW - 1) -
							introState->backRows * CLOUD_ROW_BYTES,
						CLOUD_VRAM_ROW(INTRO_VRAM_ROWS - 1) -
							introState->backRows * CLOUD_ROW_BYTES,
						DMA_COPY_HALFWORDS(CLOUD_ROW_BYTES / 2), REG_DMA3);
					introState->backRows++;
					frmCnt = introState->frmCnt;
				}
			}
			if((frmCnt & 1) == 0) {
				Sint32 y;

				BgScrollWork[2].y--;
				y = BgScrollWork[2].y;
				if((y & 7) == 0) {
					if(introState->frontRows * 8 <= (PEAK_ROWS_ABOVE - 1) * 8) {
						syDMAXCopy(
							PEAK_ROW(INTRO_FIRST_MAP_ROW - PEAK_MAP_ROW - 1) -
								introState->frontRows * PEAK_ROW_BYTES,
							PEAK_VRAM_ROW(INTRO_VRAM_ROWS - 1) -
								introState->frontRows * PEAK_ROW_BYTES,
							DMA_COPY_HALFWORDS(PEAK_ROW_BYTES / 2), REG_DMA3);
					} else {
						/* Above the peak: each new row is cleared, and the
						   map's bottom five rows, whose VRAM rows held the
						   peak's top and which the scroll brings round above
						   the screen, show its blank first tile. */
						clearValue = 0;
						syDMAXCopy(&clearValue,
							INTRO_FRONT_TILE(PEAK_FIRST_TILE) +
								(DISPLAY_HEIGHT - introState->frontRows * 8) %
									DISPLAY_HEIGHT * DISPLAY_WIDTH,
							DMA_FILL_HALFWORDS(PEAK_ROW_BYTES / 2), REG_DMA3);
						tilemap =
							(Uint16 *)BG_SCREEN_BLOCK(INTRO_FRONT_SCREEN) +
							(INTRO_MAP_ROWS - 5) * BG_SCREEN_WIDTH;
						for(i = 0; i < 5; i++) {
							for(j = 0; j < DISPLAY_TILE_WIDTH; j++)
								*tilemap++ = PEAK_FIRST_TILE;
							*tilemap++ = PEAK_FIRST_TILE;
							*tilemap++ = PEAK_FIRST_TILE;
						}
					}
					introState->frontRows++;
				}
			}
			if(introState->frm == INTRO_APPROACH_TICK)
				introState->state = INTRO_STATE_APPROACH;
		} else if(nextFrm == INTRO_FLASH_TICK + 1) {
			/* Unused: the loop ends at the flash, before these ticks. */
			introState->state = INTRO_STATE_AFTER_FLASH;
		} else if(nextFrm == INTRO_FLASH_TICK + 9) {
			introState->state = INTRO_STATE_SCROLLING;
		} else if(nextFrm == INTRO_FLASH_TICK) {
			/* The flash: behind the white the intro's sprites stop, OAM goes
			   back to the tick's own sprite list and the title picture is put
			   up. */
			syScreenFadeToWhite(1);
			syCloseEvent(mnTitleUpdateIntroSprs);
			OamCopyEnabled = 1;
			sySynch(1);
			REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS |
				BGCNT_SCREEN_BASE(PICTURE_SCREEN);
			REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
			BgScrollWork[2].y = 0;
			resourceData = syResourceGetTableEntry(RESOURCE_TITLE_SCREEN);
			syDMAXCopy(resourceData, (void *)BG_PLTT,
				DMA_COPY_WORDS(TITLE_PICTURE_COLORS * 2 / 4), REG_DMA3);
			BG_PLTT[0] = 0;
			resourceData += BG_PLTT_SIZE;
			syDecompressByteLZ(resourceData, Ram_MapGridMetatileBuffer);
			syDMAXCopy(Ram_MapGridMetatileBuffer, BG_CHAR_BLOCK(1),
				DMA_COPY_HALFWORDS(SCREEN_TILE_BYTES / 2), REG_DMA3);
			tilemap = (Uint16 *)BG_SCREEN_BLOCK(PICTURE_SCREEN);
			tile = TITLE_PICTURE_TILE;
			i = 0;
			break;
		}
		sySynch(1);
	}
	/* The title picture's map, each row's two columns past the screen's edge,
	   which never show. */
	for(; i < DISPLAY_TILE_HEIGHT; i++) {
		for(j = 0; j < DISPLAY_TILE_WIDTH; j++)
			*tilemap++ = tile++;
		*tilemap++ = offscreenTile;
		*tilemap++ = offscreenTile;
	}

	syWindowLoadGraphics();
	syPrintClear();
	if(prompt != 0) {
		Uint8	  *buf = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
		INTRO_SPR *spr = introState->area.prompt.sprs;

		syDecompressByteLZ(Title_PromptTiles, buf);
		/* The pictures are packed one after another, each half the size of a
		   32-by-8 picture of 256 colours. */
		for(i = 0; i < PROMPT_TILES; i++) {
			Sint32	vramBlock = syVRAMCacheLoad(syVRAMCacheFindFreeEntry(),
				 PROMPT_SPRITE_BYTES, buf + i * (4 * TILE_SIZE_8BPP) / 2);
			Uint32 *sprWords = (Uint32 *)spr;

			/* Unlinked, 32 by 8, and its tiles. */
			*sprWords++ = 0;
			spr++;
			*sprWords++ = OBJ_SIZE_32X8;
			*sprWords = vramBlock;
		}
		syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	}
	syScreenFadeFromWhite(30);
	syScreenFadeWait();
	REG_DISPCNT =
		DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	if(prompt != 0)
		holdTicks = TITLE_PROMPT_HOLD_TICKS;
	else
		holdTicks = TITLE_HOLD_TICKS;
	for(i = 0; i < holdTicks; i++) {
		if(prompt != 0) {
			INTRO_SPR *spr = introState->area.prompt.sprs;
			Uint32	   level;
			Sint32	   x;
#if LANGUAGE == LANGUAGE_GERMAN
			Uint32 row;
			Sint32 y;

			for(row = 0, y = PROMPT_Y; row < PROMPT_ROWS;
				y += PROMPT_ROW_HEIGHT, row++) {
				for(j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
					spr->x = x;
					spr->y = y;
					syOAMAddSprAtDepth(spr, 0);
					x += 32;
					spr++;
				}
			}
#else

			for(j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
				spr->x = x;
				spr->y = PROMPT_Y;
				syOAMAddSprAtDepth(spr, 0);
				x += 32;
				spr++;
			}
#endif
			/* The prompt pulses once a second: its blend weight over the
			   picture. */
			level = TitlePromptBlendLevels[i % PROMPT_PULSE_TICKS];
			REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND |
				BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 |
				BLDCNT_TGT2_BG3 | BLDCNT_TGT2_BD;
			REG_BLDALPHA =
				(BLEND_WEIGHT_MAX - level) * BLEND_ALPHA(0, 1) + level;
		}
		if((PerPush & (KEY_A | KEY_START)) != 0) {
			result = TRUE;
			break;
		}
		sySynch(1);
	}
	CpuLoadDisplayEnabled = savedCpuLoadDisplay;
	syHeapSlotRelease(HEAP_SLOT_TITLE_INTRO);
	REG_BLDCNT = 0;
	REG_BLDALPHA = 0;
	sySynch(1);
	return result;
}

/* Unused: an empty entry the stub table keeps. */
UNUSED void mnTitleBeforeLogosNoOperation(void)
{
}

#define LOGO_PALETTE_BYTES (LOGO_PALETTE_COLORS * 2)

/* The publisher logo's first tile: character block 1's, as BG2 numbers its
   256-colour tiles from block 0. */
#define PUBLISHER_LOGO_TILE (BG_CHAR_SIZE / TILE_SIZE_8BPP)

/* Shows the publisher's logo with its chime. Without `fadeOut`, as the title
   shows it, the logo appears at once and holds two seconds; with it, unused,
   the logo fades in after a second of black, holds three seconds and fades out.
   Returns -1 when A or Start skipped it. */
Sint32 mnTitleShowPublisherLogo(Sint32 fadeOut)
{
	Sint32	result;
	Uint8  *resourceData;
	Uint16 *tilemap;
	Uint32	column;
	Uint32	i;
	Uint16	tileIdx;
	Sint32	offscreenTile = OFFSCREEN_TILE;
	Sint32	resourceId;

	sySndPlayStub(SE_CHIME);
	OamCopyEnabled = 1;
	resourceId = RESOURCE_PUBLISHER_LOGO;
	syInitEvent();
	syScreenFadeToBlack(1);
	syPrintClear();
	sySynch(1);
	REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(PICTURE_SCREEN);
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	result = 0;
	BgScrollWork[2].y = result;
	/* The packed tiles follow the colours. */
	resourceData = syResourceGetTableEntry(resourceId);
	syDMAXCopy(resourceData, (void *)BG_PLTT,
		DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4), REG_DMA3);
	syDecompressTaggedLZ(
		resourceData + LOGO_PALETTE_BYTES, Ram_MapGridMetatileBuffer);
	syDMAXCopy(Ram_MapGridMetatileBuffer, BG_CHAR_BLOCK(1),
		DMA_COPY_WORDS(SCREEN_TILE_BYTES / 4), REG_DMA3);
	/* Pad the 30-tile picture to the hardware map's 32-tile row width. */
	tileIdx = PUBLISHER_LOGO_TILE;
	tilemap = (Uint16 *)BG_SCREEN_BLOCK(PICTURE_SCREEN);
	for(i = 0; i < DISPLAY_TILE_HEIGHT; i++) {
		for(column = 0; column < DISPLAY_TILE_WIDTH; column++)
			*tilemap++ = tileIdx++;
		*tilemap++ = offscreenTile;
		*tilemap++ = offscreenTile;
	}
	for(i = 0; i < 4; i++) {
		BgScrollWork[i].y = 0;
		BgScrollWork[i].x = 0;
	}
	syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
	syWindowLoadGraphics();
	syPrintClear();
	REG_DISPCNT =
		DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	if(fadeOut == 0) {
		syScreenFadeFromBlack(1);
		syScreenFadeWait();
		for(i = 0; i < 2 * TICKS_PER_SECOND; i++) {
			if(PerPush & (KEY_A | KEY_START)) {
				result = -1;
				break;
			}
			sySynch(1);
		}
		return result;
	}
	/* A second of black, a second's fade in, three seconds held and a second's
	   fade out; a skip shortens each fade to eight ticks. */
	for(i = 0; i < TICKS_PER_SECOND; i++) {
		if(PerPush & (KEY_A | KEY_START)) {
			result = -1;
			break;
		}
		sySynch(1);
	}
	if(result != 0)
		syScreenFadeFromBlack(8);
	else
		syScreenFadeFromBlack(TICKS_PER_SECOND);
	syScreenFadeWait();
	if(result == 0) {
		for(i = 0; i < 3 * TICKS_PER_SECOND; i++) {
			if(PerPush & (KEY_A | KEY_START)) {
				result = -1;
				break;
			}
			sySynch(1);
		}
	}
	if(result != 0)
		syScreenFadeToBlack(8);
	else
		syScreenFadeToBlack(TICKS_PER_SECOND);
	syScreenFadeWait();
	return result;
}

/* The Camelot logo's flag: four pictures of 16 tiles after the logo's own, each
   shown 8 ticks, whose first 13 tiles are copied over the logo's from its
   fifth tile. */
#define CAMELOT_FLAG_TILE		   4
#define CAMELOT_FLAG_PICTURE_BYTES (16 * TILE_SIZE_8BPP)
#define CAMELOT_FLAG_BYTES		   (13 * TILE_SIZE_8BPP)

/* Shows the Camelot logo for two seconds, its flag waving. A or Start ends it
   early; it returns 0 either way. */
Sint32 mnTitleShowCamelotLogo(void)
{
	Uint32 i;
	Uint8 *resourceData;
	Sint32 resourceId;
	Sint32 result;

	OamCopyEnabled = 1;
	result = 0;
	resourceId = RESOURCE_CAMELOT_LOGO;
	syInitEvent();
	syScreenFadeToBlack(1);
	syPrintClear();
	sySynch(1);
	REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(PICTURE_SCREEN);
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	BgScrollWork[2].y = result;
	resourceData = syResourceGetTableEntry(resourceId);
	syDecompressTaggedLZ(resourceData, Ram_MapGridMetatileBuffer);
	/* The unpacked resource: the colours, the map, the still tiles, then the
	   flag's four pictures. */
	resourceData = Ram_MapGridMetatileBuffer;
	syDMAXCopy(resourceData, (void *)BG_PLTT,
		DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4), REG_DMA3);
	resourceData += LOGO_PALETTE_BYTES;
	syDMAXCopy(resourceData, BG_SCREEN_BLOCK(PICTURE_SCREEN),
		DMA_COPY_WORDS(BG_SCREEN_SIZE / 4), REG_DMA3);
	resourceData += BG_SCREEN_SIZE;
	syDMAXCopy(resourceData, BG_CHAR_BLOCK(1), DMA_COPY_WORDS(BG_CHAR_SIZE / 4),
		REG_DMA3);
	resourceData += BG_CHAR_SIZE;
	for(i = 0; i < 4; i++) {
		BgScrollWork[i].y = 0;
		BgScrollWork[i].x = 0;
	}
	syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
	syWindowLoadGraphics();
	syPrintClear();
	syScreenFadeFromWhite(1);
	syScreenFadeWait();
	REG_DISPCNT =
		DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	for(i = 0; i < 2 * TICKS_PER_SECOND; i++) {
		syDMAXCopy(
			((TickCount >> 3) & 3) * CAMELOT_FLAG_PICTURE_BYTES + resourceData,
			BG_CHAR_BLOCK(1) + CAMELOT_FLAG_TILE * TILE_SIZE_8BPP,
			DMA_COPY_WORDS(CAMELOT_FLAG_BYTES / 4), REG_DMA3);
		if(PerPush & (KEY_A | KEY_START))
			break;
		sySynch(1);
	}
	return result;
}

/* Unused: an empty entry the stub table keeps. */
UNUSED void mnTitleAfterIntroNoOperation(void)
{
}

/* The intro's palette fade (TITLE_PALETTE_FADE) keeps the saved palettes, and
   the colours shown now, their target and their steps as components: each
   five-bit channel moved up to blue's place (bits 14 to 10) with a fraction in
   the ten bits below. */

/* Computes each of the 1536 colour components' change per tick from the
   current components to the target (nothing when `ticks` is not positive). */
void mnTitlePaletteFadeCalcSteps(Sint16 *srcComponents, Sint16 *tgtComponents,
	Sint16 *componentSteps, Sint32 ticks)
{
	Sint32 i;
	Sint32 srcValue;
	Sint32 tgtValue;

	if(ticks > 0) {
		for(i = 0; i < TITLE_PALETTE_FADE_CHANNELS; i++) {
			srcValue = *srcComponents;
			tgtValue = *tgtComponents;
			*componentSteps = syIntegerDivide(tgtValue - srcValue, ticks);
			srcComponents++;
			tgtComponents++;
			componentSteps++;
		}
	}
}

/* An event added by mnTitlePaletteFadeInit: steps the palette fade each tick,
   adding the step to every colour component until the fade is done, packs the
   components into the spare RGB555 copy of the palettes and queues both
   palettes for VBlank. */
void mnTitlePaletteFadeUpdate(void)
{
	TITLE_PALETTE_FADE *state = (TITLE_PALETTE_FADE *)PaletteFadeState;
	Uint16			   *componentSteps = state->delta;
	Uint16			   *curComponents;
	Uint16			   *packedCols;
	Sint32				i;
	Uint16			   *pagePalettes;

	if(state->duration == 0)
		return;
	if((Sint8)++state->step < state->duration) {
		curComponents = state->cur;
		for(i = 0; i <= TITLE_PALETTE_FADE_CHANNELS - 1; i++)
			*curComponents++ += *componentSteps++;
	} else {
		syDMAXCopy(state->tgt, state->cur,
			DMA_COPY_WORDS(sizeof(state->cur) / 4), REG_DMA3);
		state->duration = 0;
	}
	/* Pack into the page not queued last tick, which VBlank may still read. */
	packedCols = state->packed[state->page ^ 1];
	{
		Sint32 blueMask = BLUE_BITS;

		i = PLTT_COLORS;
		curComponents = state->cur;
		for(; i != 0; i--) {
			*packedCols++ = (curComponents[0] & blueMask) |
				(((Sint16)curComponents[1] >> (BLUE_SHIFT - GREEN_SHIFT)) &
					GREEN_BITS) |
				(((Sint16)curComponents[2] >> BLUE_SHIFT) & RED_BITS);
			curComponents += 3;
		}
		state->page ^= 1;
		pagePalettes = state->packed[state->page];
		{
			Uint16 *backgroundCols = pagePalettes;

			IO_WRITE_QUEUE_PUSH((Uint32)BG_PLTT, (Uint32)backgroundCols,
				DMA_COPY_WORDS(BG_PLTT_SIZE / 4));
		}
		IO_WRITE_QUEUE_PUSH((Uint32)OBJ_PLTT,
			(Uint32)(pagePalettes + BG_PLTT_COLORS),
			DMA_COPY_WORDS(BG_PLTT_SIZE / 4));
	}
}

/* PALETTE_TINT_DIMMER: red halved, green cut by a third, then red and green
   raised and blue lowered, each channel through its own curve. */
#define BUILD_DIMMER_COLORS() \
	for(colorIndex = 0; colorIndex < colorCount; colorIndex++) { \
		color = *source++; \
		red = RGB_RED(color); \
		green = RGB_GREEN(color); \
		blue = RGB_BLUE(color); \
		red -= (Uint32)red >> 1; \
		green -= green / 3; \
		red += 6; \
		green += 4; \
		blue -= 6; \
		red = mnTitlePaletteFadeClampChannel(red); \
		green = mnTitlePaletteFadeClampChannel(green); \
		blue = mnTitlePaletteFadeClampChannel(blue); \
		SET_VECTOR(destination, TitlePaletteFadeChannelCurveContrast[blue], \
			TitlePaletteFadeChannelCurveLight[green], \
			TitlePaletteFadeChannelCurve[red]); \
		destination += 3; \
	}

/* Tints 512 source colours into destination components (three per colour, each
   in the high bits of a halfword). `mode` picks the tint: a colour below
   PALETTE_TINT_FROM_SOURCE fills every colour with it; PALETTE_TINT_FROM_SOURCE
   takes that colour from source[0]; PALETTE_TINT_NONE to PALETTE_TINT_DIMMER
   are the whole-palette filters; the CUSTOM, BLEND and SPLIT flags carry a
   colour in their low bits; anything else is the address of components to copy.
   `selectedPalettes` is an enum PALETTE_SELECTION. */
void mnTitlePaletteFadeBuildCols(
	Uint32 mode, Uint16 *source, Uint16 *destination, Sint32 selectedPalettes)
{
	Uint32 colorIndex;
	Uint32 colorCount = PLTT_COLORS;
	Sint32 red;
	Sint32 blue;
	Uint32 color;
	Sint32 green;

	if(mode == PALETTE_TINT_FROM_SOURCE)
		mode = *source;
	if(selectedPalettes == PALETTES_BG) {
		colorCount = BG_PLTT_COLORS;
	} else if(selectedPalettes == PALETTES_SPRITES) {
		destination += 3 * BG_PLTT_COLORS;
		source += BG_PLTT_COLORS;
		colorCount = BG_PLTT_COLORS;
	}

	if(mode < PALETTE_TINT_FROM_SOURCE) {
		*destination++ = mode & BLUE_BITS;
		*destination++ = (mode & GREEN_BITS) << (BLUE_SHIFT - GREEN_SHIFT);
		*destination++ = (mode & RED_BITS) << BLUE_SHIFT;
		syDMAXCopy(destination - 3, destination,
			DMA_COPY_HALFWORDS(((colorCount - 1) * 6) >> 1), REG_DMA3);
	} else if(mode < PALETTE_TINT_FLAGS) {
		switch(mode) {
		case PALETTE_TINT_GRAYSCALE:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				color = *source++;
				red = ((color << BRIGHTNESS_RED_SHIFT) &
					(RED_BITS << BRIGHTNESS_RED_SHIFT));
				green = ((color << BRIGHTNESS_GREEN_SHIFT) &
					(GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
				blue = (color & BLUE_BITS);
				color = syIntegerDivide(red + green + blue, 7);
				*destination++ = color;
				*destination++ = color;
				*destination++ = color;
			}
			break;
		case PALETTE_TINT_SEPIA:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				color = *source++;
				red = RGB_RED(color);
				color =
					syIntegerDivide(red + RGB_GREEN(color) + RGB_BLUE(color),
						SEPIA_SHADE_DIVISOR);
				red = color * 4 + 5;
				blue = color * 3 + 5;
				green = blue;
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
				*destination++ = TitlePaletteFadeChannelCurveLight[blue];
				*destination++ = TitlePaletteFadeChannelCurveLight[green];
				*destination++ = TitlePaletteFadeChannelCurveLight[red];
			}
			break;
		case PALETTE_TINT_DIM:
			/* Unlike the field's fade, the intro's builds this tint as
			   PALETTE_TINT_DIMMER. */
			BUILD_DIMMER_COLORS();
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
				*destination++ = TitlePaletteFadeChannelCurve[blue];
				*destination++ = TitlePaletteFadeChannelCurve[green];
				*destination++ = TitlePaletteFadeChannelCurve[red];
			}
			break;
		case PALETTE_TINT_PALE:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				Sint32 mean;

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
					TitlePaletteFadeChannelCurveContrast[blue],
					TitlePaletteFadeChannelCurveContrast[green],
					TitlePaletteFadeChannelCurveContrast[red]);
				destination += 3;
			}
			break;
		case PALETTE_TINT_RED:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				color = *source++;
				red = RGB_RED(color);
				green = RGB_GREEN(color);
				blue = RGB_BLUE(color);
				red += ((green >> 3) + (blue >> 3));
				red = mnTitlePaletteFadeClampChannel(red);
				green -= green / 3;
				blue -= blue / 3;
				SET_VECTOR(destination, TitlePaletteFadeChannelCurve[blue],
					TitlePaletteFadeChannelCurve[green],
					TitlePaletteFadeChannelCurveLight[red]);
				destination += 3;
			}
			break;
		case PALETTE_TINT_DIMMER:
			BUILD_DIMMER_COLORS();
			break;
		default:
			for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
				color = *source++;
				destination[colorIndex * 3 + 0] = color & BLUE_BITS;
				destination[colorIndex * 3 + 1] = (color & GREEN_BITS)
					<< (BLUE_SHIFT - GREEN_SHIFT);
				destination[colorIndex * 3 + 2] = (color & RED_BITS)
					<< BLUE_SHIFT;
			}
			break;
		}
	} else if(mode & PALETTE_TINT_CUSTOM) {
		Uint32 tintRed;
		Uint32 tintGreen;
		Uint32 tintBlue;
		Sint32 red;
		Sint32 green;

		tintRed = RGB_RED(mode);
		tintGreen = RGB_GREEN(mode);
		tintBlue = RGB_BLUE(mode);
		for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
			color = *source++;
			red = ((color << BRIGHTNESS_RED_SHIFT) &
				(RED_BITS << BRIGHTNESS_RED_SHIFT));
			green = ((color << BRIGHTNESS_GREEN_SHIFT) &
				(GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
			blue = (color & BLUE_BITS);
			color = syIntegerDivide(red + green + blue, CUSTOM_TINT_DIVISOR);
			red = color * tintRed;
			green = color * tintGreen;
			blue = color * tintBlue;
			red = mnTitlePaletteFadeClampShiftedChannel(red);
			green = mnTitlePaletteFadeClampShiftedChannel(green);
			blue = mnTitlePaletteFadeClampShiftedChannel(blue);
			*destination++ = blue;
			*destination++ = green;
			*destination++ = red;
		}
	} else if(mode & PALETTE_TINT_BLEND) {
		Uint32 tintRed;
		Uint32 tintGreen;
		Uint32 tintBlue;
		FIXED  blendedRed;
		FIXED  blendedGreen;
		FIXED  blendedBlue;

		tintRed = RGB_RED(mode);
		tintGreen = RGB_GREEN(mode);
		tintBlue = RGB_BLUE(mode);
		for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
			color = *source++;
			red = RGB_RED(color);
			color =
				syIntegerDivide((red + RGB_GREEN(color) + RGB_BLUE(color)) << 4,
					tintRed + tintGreen + tintBlue);
			blendedRed = syMulFX(
				toFIXED((color * tintRed) >> 4), (Sint32)toFIXED(tintRed) >> 4);
			blendedGreen = syMulFX(toFIXED((color * tintGreen) >> 4),
				(Sint32)toFIXED(tintGreen) >> 4);
			blendedBlue = syMulFX(toFIXED((color * tintBlue) >> 4),
				(Sint32)toFIXED(tintBlue) >> 4);
			blendedRed =
				mnTitlePaletteFadeClampChannel(FIXEDtoINT((Uint32)blendedRed));
			blendedGreen = mnTitlePaletteFadeClampChannel(
				FIXEDtoINT((Uint32)blendedGreen));
			blendedBlue =
				mnTitlePaletteFadeClampChannel(FIXEDtoINT((Uint32)blendedBlue));
			*destination++ = TitlePaletteFadeChannelCurve[blendedBlue];
			*destination++ = TitlePaletteFadeChannelCurve[blendedGreen];
			*destination++ = TitlePaletteFadeChannelCurve[blendedRed];
		}
	} else if(mode & PALETTE_TINT_SPLIT) {
		for(colorIndex = 0; colorIndex < colorCount; colorIndex++) {
			color = *source++;
			destination[colorIndex * 3 + 0] = color & BLUE_BITS;
			destination[colorIndex * 3 + 1] = (color & GREEN_BITS)
				<< (BLUE_SHIFT - GREEN_SHIFT);
			destination[colorIndex * 3 + 2] = (color & RED_BITS) << BLUE_SHIFT;
		}
	} else {
		if(selectedPalettes == PALETTES_SPRITES)
			mode += 3 * BG_PLTT_SIZE;
		syDMAXCopy((const void *)mode, destination,
			DMA_COPY_WORDS((colorCount * 6) >> 2), REG_DMA3);
	}
}

/* Allocates the palette fade, saves both hardware palettes in it as the
   starting colours and adds the event mnTitlePaletteFadeUpdate. */
void mnTitlePaletteFadeInit(void)
{
	TITLE_PALETTE_FADE *buf;

	buf = syHeapSlotAllocEWRAM(HEAP_SLOT_PALETTE_FADE, sizeof(*buf));
	DMA_FILL32(0, buf, sizeof(*buf));
	syDMAXCopy(
		(void *)BG_PLTT, buf, DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
	syDMAXCopy((void *)OBJ_PLTT, &buf->pal[BG_PLTT_COLORS],
		DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
	mnTitlePaletteFadeBuildCols(
		PALETTE_TINT_NONE, buf->pal, buf->tgt, PALETTES_ALL);
	sySetEvent(mnTitlePaletteFadeUpdate, EVENT_PASS_TICK);
}

/* Unused: removes the palette fade's event and frees its buffers. */
UNUSED void mnTitlePaletteFadeRelease(void)
{
	syCloseEvent(&mnTitlePaletteFadeUpdate);
	syHeapSlotRelease(HEAP_SLOT_PALETTE_FADE);
}

/* Unused: sets the fade target by tinting the saved background and/or sprite
   palettes with the requested tint. */
UNUSED void mnTitlePaletteFadeSetTarget(Sint32 mode, Sint32 selectedPalettes)
{
	TITLE_PALETTE_FADE *fade = (TITLE_PALETTE_FADE *)PaletteFadeState;

	if(fade != NULL)
		mnTitlePaletteFadeBuildCols(
			mode, fade->pal, fade->tgt, selectedPalettes);
}

/* Sets the colours shown now by tinting the saved palettes. */
void mnTitlePaletteFadeSetCurrent(Sint32 mode, Sint32 selectedPalettes)
{
	TITLE_PALETTE_FADE *fade = (TITLE_PALETTE_FADE *)PaletteFadeState;

	if(fade != NULL)
		mnTitlePaletteFadeBuildCols(
			mode, fade->pal, fade->cur, selectedPalettes);
}

/* Unused: replaces the first saved colour, the backdrop. */
UNUSED void mnTitlePaletteFadeSetBackdrop(Sint32 col)
{
	TITLE_PALETTE_FADE *fade = (TITLE_PALETTE_FADE *)PaletteFadeState;

	if(fade != NULL)
		fade->pal[0] = col;
}

/* Starts fading the shown colours to the target over the requested ticks: each
   of the 1,536 components (three for each of the 512 colours) steps by its
   distance divided by the ticks, the remainder kept in the ten bits below it,
   so every colour arrives together. */
void mnTitlePaletteFadeStart(Sint32 ticks)
{
	TITLE_PALETTE_FADE *fade = (TITLE_PALETTE_FADE *)PaletteFadeState;

	if(fade != NULL) {
		fade->duration = ticks;
		fade->step = 0;
		mnTitlePaletteFadeCalcSteps((Sint16 *)fade->cur, (Sint16 *)fade->tgt,
			(Sint16 *)fade->delta, ticks);
	}
}

/* Clamps a five-bit colour channel to 0..31. */
Sint32 mnTitlePaletteFadeClampChannel(Sint32 value)
{
	if(value > RGB_LEVEL_MAX)
		return RGB_LEVEL_MAX;
	if(value < 0)
		value = 0;
	return value;
}

/* Caps a colour component at a channel's brightest level moved up to the
   components' place. There is no lower bound. */
Sint32 mnTitlePaletteFadeClampShiftedChannel(Sint32 value)
{
	if(value > BLUE_BITS)
		value = BLUE_BITS;
	return value;
}

/* The response curves of mnTitlePaletteFadeBuildCols's filters. */

/* Maps a channel (0..31) to a component (0..31 << 10). */
/* clang-format off */
Uint16 TitlePaletteFadeChannelCurve[32] = {
	0, 524, 1071, 1643, 2240, 2863, 3513, 4191,
	4898, 5635, 6403, 7202, 8035, 8901, 9803, 10740,
	11715, 12729, 13783, 14877, 16014, 17195, 18422, 19694,
	21015, 22386, 23808, 25283, 26812, 28397, 30041, 31744
};

/* A brighter curve */
Uint16 TitlePaletteFadeChannelCurveLight[32] = {
	0, 613, 1247, 1902, 2580, 3281, 4005, 4753,
	5526, 6324, 7148, 7999, 8876, 9782, 10716, 11680,
	12674, 13699, 14755, 15843, 16965, 18121, 19312, 20538,
	21801, 23101, 24440, 25818, 27237, 28697, 30199, 31744
};

/* A high-contrast curve (25 and above are full) */
Uint16 TitlePaletteFadeChannelCurveContrast[32] = {
	0, 360, 759, 1201, 1689, 2226, 2817, 3466,
	4178, 4957, 5808, 6738, 7752, 8856, 10058, 11365,
	12784, 14325, 15996, 17806, 19767, 21888, 24183, 26662,
	29340, 31744, 31744, 31744, 31744, 31744, 31744, 31744
};
/* clang-format on */

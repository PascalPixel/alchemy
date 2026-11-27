/* The staff credits: the battle backdrops behind them, the lines that scroll
   past, and the scrolling images with their border strips.

   One object, as The Broken Seal's credits.c is, with its slides and lines at
   its end; the background decoders linked before it are assembly
   (credits_background_decode.s and credits_background_expand.s, then the ending
   images' ending_image_decode.s and ending_image_expand.s). */

#include "field/field.h"
#include "game/game.h"
#include "menu/menu.h"
#include "edition.h"
#include "text/msg_ids.h"
#include "macros.h"

struct QUEUED_SPR;

typedef Sint32 (*PACKED_DECODER)(
	const Uint8 *source, void *destination, Uint32 fill);

/* One eight-byte OAM entry in the credits' sprite buffer. */
typedef struct DISPLAY_SCROLL_OBJECT {
	Uint32 attr01;
	Uint32 attr2;
} DISPLAY_SCROLL_OBJECT;

extern const Uint32 DisplayScroll_SlideResources[];
/* Draws one line of the credits' scrolling text into object tiles from slot
   on. */
Sint32 mnDispScrollDrawMessageLine(Sint32 message, Sint32 slot, Sint32 align);
extern const void *DisplayScroll_LineTable[];

void   mnCreditsInitCrawlSprs(void);
void   mnCreditsInitTextSprsAtPage(Sint32 credits);
void   mnCreditsStopTextSprs(void);
Sint32 mnCreditsDrawLine(const Uint8 *text, Sint32 firstTile, Sint32 alignment);
/* mnCreditsDecodeBackground, the ARM decoder RAM_CODE_LOAD copies into a heap
   block to run (credits_background_decode.s). */
extern const Uint8 mnCreditsDecodeBackgroundCode[];
/* mnCreditsDecodeBackgroundCode's bytes, as its address. */
extern Uint8	   Credits_DecodeBackgroundCodeSize[];
extern const Uint8 Credits_GlyphWidths[];
/* The credits' font, the glyphs mnCreditsDrawLine draws a line's tiles from. */
extern const Uint8 DisplayScroll_Font[];

void mnEndingImageInitStrips(void);
/* EndingImage_Decode, the ARM decoder RAM_CODE_LOAD copies into a heap block to
   run (ending_image_decode.s). */
extern const Uint8 EndingImage_DecodeCode[];
/* EndingImage_DecodeCode's bytes, as its address. */
extern Uint8 EndingImage_DecodeCodeSize[];

/* Credits text uses six 32-pixel sprites across each 192-pixel line, drawn into
   a ring of 32 lines of sprite tiles. */
#define CREDITS_LINE_PIXELS		  192
#define CREDITS_VISIBLE_ROWS	  16
#define CREDITS_COLUMNS			  6
#define CREDITS_RING_ROWS		  32
#define CREDITS_LINE_TILES		  24 /* A line's 4bpp tiles, 192 / 8. */
#define CREDITS_RING_TILES		  (CREDITS_RING_ROWS * CREDITS_LINE_TILES)
#define CREDITS_RING_BYTES		  (CREDITS_RING_TILES * TILE_SIZE_4BPP)
/* A line's raster: nine rows of 256 one-byte pixels, the ninth for the shadow
   below the eighth. */
#define CREDITS_BUFFER_BYTES	  2304
#define CREDITS_RASTER_STRIDE	  256
/* OAM entries 0 to 23: the opaque strips above and below the text. */
#define CREDITS_BORDER_SPRITES	  24
/* The whole OAM: the borders, 96 text sprites and 8 parked off screen. */
#define CREDITS_OAM_ENTRIES		  128
/* The border sprites' one 32x16 picture, solid in colour 1, after the ring. */
#define CREDITS_BORDER_TILE_BYTES (8 * TILE_SIZE_4BPP)
/* One ring line's tiles: the top or bottom half of a drawn line. */
#define CREDITS_LINE_BYTES		  (CREDITS_LINE_TILES * TILE_SIZE_4BPP)
/* The line table's page mark, which ends a stretch of the credits: the scroll
   stops on it, and a page's start is found by counting these marks. Then how
   far mnCreditsInitTextSprsAtPage counts for one, and the frames the credits
   run on after a mark. */
#define CREDITS_PAGE_MARK		  ((const void *)255)
#define CREDITS_MAX_LINES		  500
#define CREDITS_FRAMES_AFTER_END  375
/* The ring line a scroll line is drawn into, sixteen rows below the top. */
#define CREDITS_RING_LINE(line) \
	((((line) + CREDITS_VISIBLE_ROWS) & (CREDITS_RING_ROWS - 1)) * \
		CREDITS_LINE_TILES)
/* The scene flag that says a credits line was drawn before, so the next one
   starts from its shadow row. */
#define FLAG_CREDITS_LINE_DRAWN		 FLAG_SCENE_TEMP_00
/* The text sprites' rows start below the top strip, and their 192 pixels stand
   in the middle of the screen. */
#define CREDITS_TEXT_TOP			 16
#define CREDITS_TEXT_LEFT			 ((DISPLAY_WIDTH - CREDITS_LINE_PIXELS) / 2)
/* The x and the y at which the eight spare sprites wait, (192, 192), below the
   screen. */
#define CREDITS_PARKED				 192
/* A line's letters are drawn in colour 15, each with its shadow a pixel below
   and to the right in colour 1, the strips' black. */
#define CREDITS_TEXT_COLOR			 15
#define CREDITS_SHADOW_COLOR		 1
/* A glyph of the credits' font is eight rows of a byte each, its leftmost pixel
   the top bit. */
#define CREDITS_GLYPH_LEFT_PIXEL_BIT 0x80
/* A message of the story crawl takes two scroll lines of 8 pixels. */
#define CRAWL_MESSAGE_PIXELS		 (2 * 8)

/* DisplayScroll_Busy while the story crawl runs: what its last line stopped it
   on. A crawl line of dashes draws nothing and stops the crawl, the dashes past
   the first saying how (mnTextDrawMessageToCanvas). */
typedef enum CRAWL_STOP {
	CRAWL_SCROLLING = 0,
	/* "--": the crawl waits for the next slide, which A or B releases. */
	CRAWL_STOP_FOR_SLIDE = 1,
	CRAWL_STOP_END = 2, /* "---", the crawl's last line: the crawl ends */
	CRAWL_STOP_LONG = 3 /* "----", which no crawl line has */
} CRAWL_STOP;

/* Where mnCreditsDrawLine places a line in its 192 pixels. */
typedef enum CREDITS_ALIGNMENT {
	CREDITS_ALIGN_LEFT = 0,
	CREDITS_ALIGN_CENTER = 1,
	CREDITS_ALIGN_RIGHT = 2
} CREDITS_ALIGNMENT;

/* The ending's scrolling images: a map buffer of 15 columns, each a strip of 4
   KB copied into its own 2 KB screen block, 31 rows of 64 bytes at a time. */
#define ENDING_IMAGE_COLUMNS	  15
#define ENDING_IMAGE_STRIP_BYTES  4096
#define ENDING_IMAGE_SCREEN_BYTES BG_SCREEN_SIZE
#define ENDING_IMAGE_ROWS		  31
#define ENDING_IMAGE_ROW_BYTES	  64
/* A row's offset wrapped into its screen block. */
#define ENDING_IMAGE_WRAP(offset) ((offset) & (ENDING_IMAGE_SCREEN_BYTES - 1))
/* The fade-in's frames, four to each blend step. */
#define ENDING_IMAGE_FADE_FRAMES  (BLEND_WEIGHT_MAX * 4)
/* The map offset of the row of eight pixels a 16.16 scroll stands at. */
#define ENDING_IMAGE_ROW_OFFSET(scroll) \
	((FIXEDtoINT(scroll) / 8) * ENDING_IMAGE_ROW_BYTES)
/* The map buffer: every column's strip. */
#define ENDING_IMAGE_MAP_BYTES		(ENDING_IMAGE_COLUMNS * ENDING_IMAGE_STRIP_BYTES)
/* mnEndingImageWaitForScroll gives up after five seconds. */
#define ENDING_IMAGE_SCROLL_TIMEOUT (5 * FRAMES_PER_SECOND)
/* The image's border strips: eight sprites across at y 120, 136 and 152. */
#define ENDING_IMAGE_STRIP_SPRITES	24

/* A backdrop or image resource begins with its 128 colours, the palette half it
   takes; its packed tiles follow. */
#define BACKDROP_COLORS			128
#define BACKDROP_PALETTE_BYTES	(BACKDROP_COLORS * 2)
/* A battle backdrop's 480 tiles of 256 colours, 256 by 120 pixels. */
#define BACKDROP_TILES			480
#define BACKDROP_TILE_BYTES		(BACKDROP_TILES * 64)
/* The credits grey a backdrop's colours: a channel becomes the colour's
   brightness (2 red + 4 green + blue) and BACKDROP_CHANNEL_WEIGHT parts of
   itself, over BACKDROP_GREY_PARTS. The weights add to 12, so a grey keeps
   three quarters of its level. */
#define BACKDROP_CHANNEL_WEIGHT 5
#define BACKDROP_GREY_PARTS		16

/* The two images' screen blocks, each holding the same tilemap: BG3's, over
   character block 0, and BG2's, over character block 2. */
#define CREDITS_BG3_SCREEN_BLOCK  15
#define CREDITS_BG2_SCREEN_BLOCK  31
/* A tilemap word: two map entries side by side, `left` in its low half. */
#define TILEMAP_PAIR(left, right) (((right) << 16) | (left))
/* A tilemap row's words. */
#define TILEMAP_ROW_WORDS		  (BG_SCREEN_WIDTH / 2)
/* The tile the margins show: the character block's last, which lies in the
   screen block's own zeroed tail and so is clear. */
#define CREDITS_BLANK_TILE		  511

/* Fills a backdrop's 256-colour tiles with its half's first colour, an opaque
   black for the next crossfade, and clears the half's 128 palette colours.
   alternate is 0 for the first half (character block 0, colours 0 to 127), 1
   for the second (character block 2, colours 128 to 255). */
void mnCreditsClearBuffer(Sint32 alternate)
{
	Uint32			fillColor;
	Uint32			tileDestination;
	Uint32			paletteDestination;
	volatile Uint32 dmaFill;

	if(alternate == 0) {
		fillColor = PIXELS_8BPP(1);
		tileDestination = (Uint32)BG_CHAR_BLOCK(0);
		paletteDestination = (Uint32)BG_PLTT;
	} else {
		fillColor = PIXELS_8BPP(129);
		tileDestination = (Uint32)BG_CHAR_BLOCK(2);
		paletteDestination = (Uint32)&BG_PLTT_COLOR(8, 0);
	}
	DMA_FILL32_CELL(
		dmaFill, fillColor, (void *)tileDestination, BACKDROP_TILE_BYTES);
	DMA_FILL32_CELL(
		dmaFill, 0, (void *)paletteDestination, BACKDROP_PALETTE_BYTES);
}

/* Loads a backdrop resource (its 128 colours, then its packed tiles) into one
   half of BG VRAM and queues its palette for the next VBlank, as The Broken
   Seal does, but greyed by BACKDROP_CHANNEL_WEIGHT and BACKDROP_GREY_PARTS. A
   zero resource clears that half instead. The tiles are unpacked by
   mnCreditsDecodeBackground, run from a heap block, and the grey colours are
   built in Credits_GreyColors: the credits have no buffer of their own for
   them, and borrow the cells of the SDK's flash library, which IdentifyFlash
   sets again before the flash code reads them. */
void mnCreditsLoadBackground(Uint32 resourceId, Sint32 alternate)
{
	const Uint8	  *resourceData;
	Uint32		   paletteDestination;
	Uint32		   paletteOffset;
	void		  *tileDestination;
	PACKED_DECODER decoder;
	Sint32		   i;

	if(resourceId == 0) {
		mnCreditsClearBuffer(alternate);
		return;
	}
	resourceData = syResourceGetTableEntry(resourceId);
	if(alternate == 0) {
		paletteOffset = 0;
		tileDestination = (void *)BG_CHAR_BLOCK(0);
		paletteDestination = (Uint32)BG_PLTT;
	} else {
		paletteOffset = PIXELS_8BPP(128);
		tileDestination = (void *)BG_CHAR_BLOCK(2);
		paletteDestination = (Uint32)&BG_PLTT_COLOR(8, 0);
	}
	RAM_CODE_LOAD(decoder, mnCreditsDecodeBackgroundCode,
		Credits_DecodeBackgroundCodeSize);
	decoder(
		resourceData + BACKDROP_PALETTE_BYTES, tileDestination, paletteOffset);
	syHeapRelease(decoder);

	for(i = 0; i < BACKDROP_COLORS; i++) {
		Uint16 color = *(const Sint16 *)resourceData;
		Sint32 blue = RGB_BLUE(color);
		Sint32 green = RGB_GREEN(color);
		Sint32 red = RGB_RED(color);
		Sint32 sum = (green * 2 + red) * 2 + blue;

		resourceData += 2;
		blue = (sum + blue * BACKDROP_CHANNEL_WEIGHT) / BACKDROP_GREY_PARTS;
		green = (sum + green * BACKDROP_CHANNEL_WEIGHT) / BACKDROP_GREY_PARTS;
		red = (sum + red * BACKDROP_CHANNEL_WEIGHT) / BACKDROP_GREY_PARTS;
		Credits_GreyColors[i] = RGB(red, green, blue);
	}
	IO_WRITE_QUEUE_PUSH(paletteDestination, (Uint32)Credits_GreyColors,
		DMA_COPY_WORDS(BACKDROP_PALETTE_BYTES / 4));
}

/* Builds a backdrop's tilemap, as The Broken Seal does: two blank rows, the
   backdrop's 480 tiles in order over fifteen rows, three blank rows, then a
   zeroed tail. */
void mnCreditsBuildBackgroundTilemap(Uint32 *destination)
{
	Sint32 i;
	Uint32 blankTiles;
	Uint32 tilePair;

	blankTiles = TILEMAP_PAIR(CREDITS_BLANK_TILE, CREDITS_BLANK_TILE);
	tilePair = TILEMAP_PAIR(0, 1);
	/* Pad above and below the image so vertical wrapping stays blank. */
	for(i = 0; i < 2 * TILEMAP_ROW_WORDS; i++)
		*destination++ = blankTiles;
	for(i = 0; i < BACKDROP_TILES / 2; i++) {
		*destination++ = tilePair;
		tilePair += TILEMAP_PAIR(2, 2);
	}
	for(i = 0; i < 3 * TILEMAP_ROW_WORDS; i++)
		*destination++ = blankTiles;
	for(i = 0; i < 12 * TILEMAP_ROW_WORDS; i++)
		*destination++ = 0;
}

/* A VBlank event of the credits: drifts both images together, keeping their
   horizontal offsets aligned throughout the crossfade. */
void mnCreditsScrollBackgrounds(void)
{
	if((VBlankCount & 3) == 0) {
		BgScrollWork[2].x--;
		BgScrollWork[3].x--;
	}
}

/* Runs the title's story crawl or the staff credits over the greyed battle
   backdrops. The story crawl (CREDITS_MODE_STORY_CRAWL) cross-fades a new
   backdrop each time the crawl stops for one, until Start or the crawl's end;
   any other mode scrolls the staff credits from page mode -
   CREDITS_MODE_FIRST_PAGE and returns once they have run out. */
Sint32 mnCreditsRun(Sint32 mode)
{
	Sint32 slide;
	Sint32 step;
	Sint32 frame;

	OamCopyEnabled = 0;
	CpuLoadDisplayEnabled = 0;
	DebugTextEnabled = 0;
	Credits_PendingSlides = 0;
	IdleSleepEnabled = 0;
	syInitEvent();
	sySetEvent(mnCreditsScrollBackgrounds, EVENT_PASS_VBLANK);
	syIOWrite16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
	mnCreditsBuildBackgroundTilemap(
		(Uint32 *)BG_SCREEN_BLOCK(CREDITS_BG3_SCREEN_BLOCK));
	mnCreditsBuildBackgroundTilemap(
		(Uint32 *)BG_SCREEN_BLOCK(CREDITS_BG2_SCREEN_BLOCK));
	mnCreditsClearBuffer(0);
	mnCreditsClearBuffer(1);
	syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(CREDITS_BG2_SCREEN_BLOCK),
		&REG_BG2CNT);
	syIOWrite16(BGCNT_PRIORITY(3) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(CREDITS_BG3_SCREEN_BLOCK),
		&REG_BG3CNT);
	syIOWrite16(
		DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON,
		&REG_DISPCNT);
	syIOWrite16(BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3 |
			BLDCNT_TGT2_BD,
		&REG_BLDCNT);

	if(mode == CREDITS_MODE_STORY_CRAWL) {
		mnCreditsInitCrawlSprs();
		sySynch(60);
		for(slide = 0;; slide++) {
			Sint16 *count = &Credits_PendingSlides;

			mnCreditsLoadBackground(
				DisplayScroll_SlideResources[slide], (slide & 1) ^ 1);
			for(step = 1; step <= BLEND_WEIGHT_MAX; step++) {
				/* Fade the new half in: an even slide went to BG2, the first
				   target (BLDALPHA bits 4..0), an odd one to BG3, the second
				   (bits 12..8). */
				if(slide & 1)
					syIOWrite16(
						(step << 8) | (BLEND_WEIGHT_MAX - step), &REG_BLDALPHA);
				else
					syIOWrite16(
						((BLEND_WEIGHT_MAX - step) << 8) | step, &REG_BLDALPHA);
				for(frame = 0; frame <= 3; frame++) {
					if(PerWork.data & KEY_START)
						goto done;
					sySynch(1);
				}
			}
			if(*count == 0) {
				do {
					/* DEAD: a read of the pressed keys nothing uses; the ROM
					   keeps its load. */
					PerWork.push;
					if(PerWork.data & KEY_START)
						goto done;
					if(DisplayScroll_Busy == CRAWL_STOP_END)
						goto done;
					sySynch(1);
				} while(Credits_PendingSlides == 0);
			}
			Credits_PendingSlides--;
		}
	} else {
		mnCreditsInitTextSprsAtPage(mode - CREDITS_MODE_FIRST_PAGE);
		while(DisplayScroll_Busy <= CREDITS_FRAMES_AFTER_END)
			sySynch(1);
		mnCreditsStopTextSprs();
	}

done:
	syIOWrite16(0, &REG_BLDCNT);
	syIOWrite16(DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON, &REG_DISPCNT);
	syWindowLoadGraphics();
	syPrintClear();
	OamCopyEnabled = 1;
	return 0;
}

/* A frame-pass event added by mnCreditsInitCrawlSprs, as The Broken Seal's
   mnCreditsUpdateTextSprs: maps the visible sixteen rows of the story crawl
   onto the tile ring and uploads their sprites. While a slide waits, A or B
   releases the next one (Credits_PendingSlides); the crawl advances a pixel
   every fourth frame, and every frame but those while B is held. */
void mnCreditsUpdateCrawlSprs(void)
{
	Uint32 fine = DisplayScroll_Position & 7;
	Uint32 tile =
		(((Sint16)DisplayScroll_Position / 8) & (CREDITS_RING_ROWS - 1)) *
		CREDITS_LINE_TILES;
	Uint32 *words = (Uint32 *)(DisplayScroll_Objects + CREDITS_BORDER_SPRITES);
	Sint32	row;

	for(row = 0; row < CREDITS_VISIBLE_ROWS; row++) {
		Sint32 column;

		for(column = 0; column < CREDITS_COLUMNS; column++) {
			*words++ = (row * 8 + CREDITS_TEXT_TOP - fine) |
				OBJ_X(column * 32 + CREDITS_TEXT_LEFT) | OBJ_SIZE_32X8;
			*words++ = tile;
			tile += 4;
			if(tile == CREDITS_RING_TILES)
				tile = 0;
		}
	}
	syDMAXCopy(DisplayScroll_Objects, (void *)OAM, DMA_COPY_WORDS(OAM_SIZE / 4),
		REG_DMA3);

	if(DisplayScroll_Busy == CRAWL_STOP_FOR_SLIDE &&
		(PerWork.data & (KEY_A | KEY_B))) {
		DisplayScroll_Busy = CRAWL_SCROLLING;
		Credits_PendingSlides++;
	}
#if CRAWL_PAUSE_WITHOUT_SLIDE
	/* In every edition but the Japanese A or B also goes on past a "----" line,
	   without a slide. */
	if(DisplayScroll_Busy == CRAWL_STOP_LONG &&
		(PerWork.data & (KEY_A | KEY_B)))
		DisplayScroll_Busy = CRAWL_SCROLLING;
#endif
	if(DisplayScroll_Busy == CRAWL_SCROLLING) {
		if((VBlankCount & 3) == 0)
			DisplayScroll_Position++;
		if((PerWork.data & KEY_B) && (VBlankCount & 3))
			DisplayScroll_Position++;
	}
}

/* A VBlank event added by mnCreditsInitCrawlSprs: draws each line of the story
   crawl (MsgIntroCrawl) as it enters, two scroll lines a message. */
void mnCreditsDrawEnteringCrawlLine(void)
{
	Sint32 first = (Sint32)MsgIntroCrawl;
	Uint32 current;
	Sint32 line;

	if(DisplayScroll_Busy == CRAWL_SCROLLING) {
		current = DisplayScroll_Position;
		if((Sint16)DisplayScroll_Position / CRAWL_MESSAGE_PIXELS !=
			DisplayScroll_LastPosition / CRAWL_MESSAGE_PIXELS) {
			DisplayScroll_LastPosition = current;
			line = (Sint16)((Sint16)DisplayScroll_Position / 8);
			DisplayScroll_Busy =
				mnDispScrollDrawMessageLine(line / 2 + first - 1,
					CREDITS_RING_LINE(line), CREDITS_ALIGN_CENTER);
		}
	}
}

/* Allocates the story crawl's sprites, builds its border and text rows, and
   adds mnCreditsUpdateCrawlSprs and mnCreditsDrawEnteringCrawlLine, as The
   Broken Seal's Credits_InitTextSprites starts its credits. */
void mnCreditsInitCrawlSprs(void)
{
	volatile Uint32 fill;
	Uint32		   *entry;
	Uint32			i;
	Uint32			j;

	void *objects =
		syHeapAllocEWRAM(CREDITS_OAM_ENTRIES * sizeof(DISPLAY_SCROLL_OBJECT));
	void **cell = (void **)&DisplayScroll_Objects;
	*cell = objects;
	fill = 0;
	syDMAXCopy((void *)&fill, (void *)OBJ_VRAM0,
		DMA_FILL_WORDS(CREDITS_RING_BYTES / 4), REG_DMA3);
	fill = PIXELS_4BPP(1);
	syDMAXCopy((void *)&fill, (void *)(OBJ_VRAM0 + CREDITS_RING_BYTES),
		DMA_FILL_WORDS(CREDITS_BORDER_TILE_BYTES / 4), REG_DMA3);

	entry = *(Uint32 **)cell;
	{
		Uint32 attr = OBJ_SIZE_32X16;
		for(i = 0; i < 8; i++) {
			Uint32 *q = entry;

			*q++ = OBJ_X(i * 32) | attr;
			*q = CREDITS_RING_TILES;
			entry += 2;
		}
	}
	{
		Uint32 attr = 136 | OBJ_SIZE_32X16;
		for(i = 0; i < 8; i++) {
			Uint32 *q = entry;

			*q++ = OBJ_X(i * 32) | attr;
			*q = CREDITS_RING_TILES;
			entry += 2;
		}
	}
	{
		Uint32 attr = 152 | OBJ_SIZE_32X8;
		for(i = 0; i < 8; i++) {
			Uint32 *q = entry;

			*q++ = OBJ_X(i * 32) | attr;
			*q = CREDITS_RING_TILES;
			entry += 2;
		}
	}
	for(i = 0; i < CREDITS_VISIBLE_ROWS; i++) {
		for(j = 0; j < CREDITS_COLUMNS; j++) {
			Uint32 *q = entry;
			Uint32	tile = (i * 3) * 8 + j * 4;

			*q++ = (i * 8 + CREDITS_TEXT_TOP) |
				OBJ_X(j * 32 + CREDITS_TEXT_LEFT) | OBJ_SIZE_32X8;
			*q = tile;
			entry += 2;
		}
	}
	for(i = 0; i < 8; i++) {
		Uint32 *q = entry;

		*q++ = OBJ_X(CREDITS_PARKED) | CREDITS_PARKED;
		*q = CREDITS_RING_TILES;
		entry += 2;
	}
	DisplayScroll_Position = 0;
	DisplayScroll_LastPosition = 0;
	DisplayScroll_Busy = 0;
	sySetEvent(mnCreditsUpdateCrawlSprs, EVENT_PASS_VBLANK);
	sySetEvent(mnCreditsDrawEnteringCrawlLine, EVENT_PASS_FRAME);
	for(i = 0; i < CREDITS_RING_ROWS; i++)
		mnCreditsDrawLine(DisplayScroll_LineTable[0], i * CREDITS_LINE_TILES,
			CREDITS_ALIGN_CENTER);
}

/* Draws one line of the credits' scrolling text: the message is drawn into a
   canvas of one byte a pixel, packed to two pixels a byte, and copied into
   object tiles from slot on, its top half then its bottom half. */
Sint32 mnDispScrollDrawMessageLine(Sint32 message, Sint32 slot, Sint32 align)
{
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	Uint32	unused[8];
	Uint8  *canvas;
	Uint8  *src;
	Uint8  *dst;
	Uint32 *tiles;
	Uint32 *top;
	Sint32	busy;
	Sint32	i;
	Sint32	j;
	Sint32	row;
	Sint32	offset;

	canvas = syHeapAllocEWRAM(CREDITS_BUFFER_BYTES);
	/* BUG: the clear, and the packing loop's reads below, cover 3,072 bytes of
	   a 2,304-byte allocation. */
	syWordsClear(canvas, 3072);
	busy = mnTextDrawMessageToCanvasStub(message, canvas, align);
	dst = canvas;
	src = canvas;
	for(j = 0; j < 2 * CREDITS_LINE_BYTES; j++) {
		Uint8 low = *src++;
		*dst++ = low | (*src++ << 4);
	}
	for(i = 0; i < CREDITS_LINE_TILES; i++) {
		Uint32 *column = (Uint32 *)canvas + i;

		tiles = (Uint32 *)((slot + i) * TILE_SIZE_4BPP + OBJ_VRAM0);
		for(row = 0; row < 8; row++) {
			*tiles++ = column[row * CREDITS_LINE_TILES];
		}
	}
	for(offset = slot * TILE_SIZE_4BPP,
	top = (Uint32 *)(canvas + CREDITS_LINE_BYTES), j = 0;
		j < CREDITS_LINE_TILES; j++, offset += TILE_SIZE_4BPP, top++) {
		tiles = (Uint32 *)(offset + OBJ_VRAM0 + CREDITS_LINE_BYTES);
		for(row = 0; row < 8; row++) {
			*tiles++ = top[row * CREDITS_LINE_TILES];
		}
	}
	syHeapRelease(canvas);
	return busy;
}

/* A VBlank event added by Credits_InitTextSprites: maps the visible sixteen
   rows onto the tile ring and uploads their sprites. */
void mnCreditsUpdateTextSprs(void)
{
	Uint32 frame = DisplayScroll_Position;
	Uint32 fine = frame & 7;
	Sint32 tile =
		(((Sint16)frame / 8) & (CREDITS_RING_ROWS - 1)) * CREDITS_LINE_TILES;
	Uint32 *entry = (Uint32 *)(DisplayScroll_Objects + CREDITS_BORDER_SPRITES);
	Sint32	row;

	for(row = 0; row < CREDITS_VISIBLE_ROWS; row++) {
		Sint32 col;

		for(col = 0; col < CREDITS_COLUMNS; col++) {
			Uint32 *q = entry;

			*q++ = (row * 8 + CREDITS_TEXT_TOP - fine) |
				OBJ_X(col * 32 + CREDITS_TEXT_LEFT) | OBJ_SIZE_32X8;
			entry += 2;
			*q = tile;
			tile += 4;
			if(tile == CREDITS_RING_TILES)
				tile = 0;
		}
	}
	syDMAXCopy(
		DisplayScroll_Objects, OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
	if(VBlankCount % 3 == 0)
		DisplayScroll_Position++;
}

/* An event added by mnCreditsInitTextSprsAtPage: draws each line entering the
   credits sixteen rows ahead of the visible top. A page mark stops the scroll,
   and blank rows follow it while DisplayScroll_Busy counts the frames since. */
void mnCreditsDrawEnteringLine(void)
{
	Sint32		 position;
	Uint32		 current;
	Sint32		 line;
	const Uint8 *text;

	position = (Sint16)DisplayScroll_Position;
	current = DisplayScroll_Position;
	line = position / 8;
	if(DisplayScroll_Busy != 0) {
		DisplayScroll_Busy++;
		mnCreditsDrawLine(DisplayScroll_LineTable[0],
			CREDITS_RING_LINE(position / 8), CREDITS_ALIGN_CENTER);
	} else if(position / 8 != DisplayScroll_LastPosition / 8) {
		DisplayScroll_LastPosition = current;
		text = DisplayScroll_LineTable[line];
		if(text == CREDITS_PAGE_MARK) {
			DisplayScroll_Busy = 1;
			mnCreditsDrawLine(DisplayScroll_LineTable[0],
				CREDITS_RING_LINE(line), CREDITS_ALIGN_CENTER);
		} else {
			DisplayScroll_Busy = mnCreditsDrawLine(
				text, CREDITS_RING_LINE(line), CREDITS_ALIGN_CENTER);
		}
	}
}

/* Starts the scrolling credits at a page, as mnCreditsInitCrawlSprs starts them
   at the top: counts the line table's 255 page marks up to the page asked for
   (at most CREDITS_MAX_LINES lines, stopping at the end) and sets the scroll
   there, then builds the sprite table and its tiles, adds
   mnCreditsUpdateTextSprs and mnCreditsDrawEnteringLine to the event table and
   draws the first 32 lines blank. */
void mnCreditsInitTextSprsAtPage(Sint32 page)
{
	volatile Uint32 fill;
	Sint32			marks;
	Uint32		   *entry;
	Uint32			i;
	Uint32			j;

	marks = 0;
	i = 0;
	if(page != 0 && DisplayScroll_LineTable[0] != 0) {
		do {
			if(DisplayScroll_LineTable[i] == CREDITS_PAGE_MARK)
				marks++;
			i++;
			if(i == CREDITS_MAX_LINES || marks == page)
				break;
		} while(DisplayScroll_LineTable[i] != 0);
	}
	DisplayScroll_Position = i * 8;

	{
		void *objects = syHeapAllocEWRAM(
			CREDITS_OAM_ENTRIES * sizeof(DISPLAY_SCROLL_OBJECT));
		void **cell = (void **)&DisplayScroll_Objects;

		*cell = objects;
		fill = 0;
		syDMAXCopy((void *)&fill, (void *)OBJ_VRAM0,
			DMA_FILL_WORDS(CREDITS_RING_BYTES / 4), REG_DMA3);
		fill = PIXELS_4BPP(1);
		syDMAXCopy((void *)&fill, (void *)(OBJ_VRAM0 + CREDITS_RING_BYTES),
			DMA_FILL_WORDS(CREDITS_BORDER_TILE_BYTES / 4), REG_DMA3);
		entry = *(Uint32 **)cell;
	}
	{
		Uint32 attr = OBJ_SIZE_32X16;

		for(i = 0; i < 8; i++) {
			Uint32 *q = entry;

			*q++ = OBJ_X(i * 32) | attr;
			*q = CREDITS_RING_TILES;
			entry += 2;
		}
	}
	{
		Uint32 attr = 136 | OBJ_SIZE_32X16;

		for(i = 0; i < 8; i++) {
			Uint32 *q = entry;

			*q++ = OBJ_X(i * 32) | attr;
			*q = CREDITS_RING_TILES;
			entry += 2;
		}
	}
	{
		Uint32 attr = 152 | OBJ_SIZE_32X8;

		for(i = 0; i < 8; i++) {
			Uint32 *q = entry;

			*q++ = OBJ_X(i * 32) | attr;
			*q = CREDITS_RING_TILES;
			entry += 2;
		}
	}
	for(i = 0; i < CREDITS_VISIBLE_ROWS; i++) {
		for(j = 0; j < CREDITS_COLUMNS; j++) {
			Uint32 *q = entry;
			Uint32	tile = (i * 3) * 8 + j * 4;

			*q++ = (i * 8 + CREDITS_TEXT_TOP) |
				OBJ_X(j * 32 + CREDITS_TEXT_LEFT) | OBJ_SIZE_32X8;
			*q = tile;
			entry += 2;
		}
	}
	for(i = 0; i < 8; i++) {
		Uint32 *q = entry;

		*q++ = OBJ_X(CREDITS_PARKED) | CREDITS_PARKED;
		*q = CREDITS_RING_TILES;
		entry += 2;
	}
	DisplayScroll_LastPosition = 0;
	DisplayScroll_Busy = 0;
	sySetEvent(mnCreditsUpdateTextSprs, EVENT_PASS_VBLANK);
	sySetEvent(mnCreditsDrawEnteringLine, EVENT_PASS_FRAME);
	for(i = 0; i < CREDITS_RING_ROWS; i++)
		mnCreditsDrawLine(DisplayScroll_LineTable[0], i * CREDITS_LINE_TILES,
			CREDITS_ALIGN_CENTER);
}

/* Ends the scrolling credits: blanks the tile ring, removes their two events
   and clears the sprites. */
void mnCreditsStopTextSprs(void)
{
	Sint32	i;
	Uint32 *objects;

	for(i = 0; i < CREDITS_RING_ROWS; i++)
		mnCreditsDrawLine(DisplayScroll_LineTable[0], i * CREDITS_LINE_TILES,
			CREDITS_ALIGN_CENTER);
	syCloseEvent(mnCreditsUpdateTextSprs);
	syCloseEvent(mnCreditsDrawEnteringLine);
	objects = (Uint32 *)DisplayScroll_Objects;
	for(i = 0; i != CREDITS_OAM_ENTRIES * 2; i++)
		*objects++ = 0;
	syDMAXCopy(
		DisplayScroll_Objects, OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
}

/* A credits tile's row of 4 bytes in object VRAM. */
#define OBJ_TILE_ROW(tile, row) \
	(*(Uint32 *)(OBJ_VRAM0 + (row) * 4 + (tile) * TILE_SIZE_4BPP))

/* Centers, right-aligns or left-aligns one credits line in a 192-pixel row,
   with its shadow, and writes it as tiles from `firstTile`. Returns -1 at the
   end of the text. */
Sint32 mnCreditsDrawLine(const Uint8 *text, Sint32 firstTile, Sint32 alignment)
{
	Uint8		*linePixels = (Uint8 *)syHeapAllocEWRAM(CREDITS_BUFFER_BYTES);
	Sint32		 startX = 0;
	Sint32		 lineWidth = CREDITS_LINE_PIXELS;
	const Uint8 *font = DisplayScroll_Font;
	Sint32		 tileCount;
	Sint32		 textAdvance;
	const Uint8 *sourceBytes;
	Uint8		*destination;
	Uint32		 sourceValue;
	Sint32		 glyphIndex;
	Sint32		 glyphWidth;
	Sint32		 row;
	Sint32		 columnIndex;
	Uint32		 glyphBits;
	Uint32		 glyphMask;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Uint32 unused[8];

	if(text == NULL) {
		/* BUG: the end marker is checked after allocation, leaking one line
		   buffer. syHeapRelease rewinds this bump pool to the allocation's
		   start. */
#if FIX_CREDITS_LINE_LEAK
		if(linePixels != NULL)
			syHeapRelease(linePixels);
#endif
		return -1;
	}
	/* The ninth raster row holds the shadow below this line. */
	if(!syGameFlagGet(FLAG_CREDITS_LINE_DRAWN)) {
		syWordsFillEntry(linePixels, CREDITS_BUFFER_BYTES, 0);
		syGameFlagSet(FLAG_CREDITS_LINE_DRAWN);
	} else {
		syWordsCopy(linePixels, linePixels + 8 * CREDITS_RASTER_STRIDE,
			CREDITS_RASTER_STRIDE);
		syWordsFillEntry(
			linePixels + CREDITS_RASTER_STRIDE, 8 * CREDITS_RASTER_STRIDE, 0);
	}

	/* Measure the line in the credits font, then choose where it starts.
	   sourceBytes walks the text here, and each glyph's rows below. */
	sourceBytes = text;
	textAdvance = 0;
	while((sourceValue = *sourceBytes++) != 0) {
		if(sourceValue >= ' ')
			textAdvance += Credits_GlyphWidths[sourceValue - ' '];
	}
	if(alignment == CREDITS_ALIGN_RIGHT)
		startX = lineWidth - textAdvance;
	else if(alignment == CREDITS_ALIGN_CENTER)
		startX = (lineWidth - textAdvance) / 2;

	/* Rasterize each glyph and its one-pixel shadow at the aligned start. */
	/* FAKEMATCH(dead): sourceBytes = text, overwritten before it is read; this
	 * pass walks text itself; as no store. */
	sourceBytes = text;
	textAdvance = 0;
	while((sourceValue = *text++) != 0) {
		if(sourceValue >= ' ') {
			glyphIndex = sourceValue - ' ';
			sourceBytes = font + glyphIndex * 8;
			destination = linePixels + startX + textAdvance;
			for(row = 0; row < 8; row++) {
				glyphBits = *sourceBytes++;
				glyphMask = CREDITS_GLYPH_LEFT_PIXEL_BIT;
				for(columnIndex = 0; columnIndex < 8; columnIndex++) {
					if(glyphBits & glyphMask) {
						destination[CREDITS_RASTER_STRIDE + 1] =
							CREDITS_SHADOW_COLOR;
						destination[0] = CREDITS_TEXT_COLOR;
					}
					destination++;
					glyphMask >>= 1;
				}
				destination += CREDITS_RASTER_STRIDE - 8;
			}
			glyphWidth = 1;
			if(sourceValue >= ' ')
				glyphWidth = Credits_GlyphWidths[glyphIndex];
			textAdvance += glyphWidth;
		}
	}

	/* Pack pairs of pixel indices into bytes, keeping the 256-pixel stride. */
	tileCount = lineWidth / 8;
	sourceBytes = linePixels;
	destination = linePixels;
	for(row = 0; row < 8; row++) {
		for(columnIndex = 0; columnIndex < tileCount * 4; columnIndex++) {
			sourceValue = *sourceBytes++;
			sourceValue |= *sourceBytes++ << 4;
			*destination++ = sourceValue;
		}
		destination += CREDITS_RASTER_STRIDE - tileCount * 4;
		sourceBytes += CREDITS_RASTER_STRIDE - tileCount * 8;
	}

	/* Transpose raster rows into the tile ring used by the scrolling
	   sprites. */
	for(row = 0; row < tileCount; row++) {
		Sint32	tileIndex = firstTile + row;
		Uint32 *words = (Uint32 *)linePixels + row;

		/* A tile's eight rows of 4 bytes, each a 256-pixel raster row apart. */
		OBJ_TILE_ROW(tileIndex, 0) = words[0];
		OBJ_TILE_ROW(tileIndex, 1) = words[1 * (CREDITS_RASTER_STRIDE / 4)];
		OBJ_TILE_ROW(tileIndex, 2) = words[2 * (CREDITS_RASTER_STRIDE / 4)];
		OBJ_TILE_ROW(tileIndex, 3) = words[3 * (CREDITS_RASTER_STRIDE / 4)];
		OBJ_TILE_ROW(tileIndex, 4) = words[4 * (CREDITS_RASTER_STRIDE / 4)];
		OBJ_TILE_ROW(tileIndex, 5) = words[5 * (CREDITS_RASTER_STRIDE / 4)];
		OBJ_TILE_ROW(tileIndex, 6) = words[6 * (CREDITS_RASTER_STRIDE / 4)];
		OBJ_TILE_ROW(tileIndex, 7) = words[7 * (CREDITS_RASTER_STRIDE / 4)];
	}
	syHeapRelease(linePixels);
	return 0;
}

/* Unused: an empty function nothing calls. */
UNUSED void mnCreditsReservedNoOperation(void)
{
}

/* Sets the row the ending's scrolling image moves to, in pixels from its top,
   at most EndingImage_Speed a frame. */
void mnEndingImageSetTargetRow(Uint16 row)
{
	EndingImage_TargetRow = row;
}

/* Copies 15 columns of a scrolling image's map into a wrapping 2 KB tilemap by
   DMA, 31 rows of 64 bytes each, from the row the position selects, and records
   the position in the three scroll words. The scrolling image scene's callback
   (mnEndingImageScroll) and its image loader (mnEndingImageShow, through its
   inline copy) call it. */
void mnEndingImageCopyMap(
	const Uint8 *source, Uint8 *destination, Sint32 position)
{
	Uint32 start;
	Sint32 column;
	Sint32 row;
	Uint32 offset;
	Uint32 from;

	start = ENDING_IMAGE_ROW_OFFSET(EndingImage_MapScroll);
	EndingImage_TargetRow = EndingImage_Row = position;
	EndingImage_Scroll = toFIXED(position);
	for(column = 0; column < ENDING_IMAGE_COLUMNS; column++) {
		from = (position / 8) * ENDING_IMAGE_ROW_BYTES;
		offset = ENDING_IMAGE_WRAP(start);
		for(row = 0; row < ENDING_IMAGE_ROWS; row++) {
			syDMAXCopy(source + from, destination + offset,
				DMA_COPY_WORDS(ENDING_IMAGE_ROW_BYTES / 4), REG_DMA3);
			from += ENDING_IMAGE_ROW_BYTES;
			offset = ENDING_IMAGE_WRAP(offset + ENDING_IMAGE_ROW_BYTES);
		}
		source += ENDING_IMAGE_STRIP_BYTES;
		destination += ENDING_IMAGE_SCREEN_BYTES;
	}
}

/* The ending image's VBlank event: fades the new image in over 64 frames, moves
   the row toward EndingImage_TargetRow by at most EndingImage_Speed, eases the
   16.16 scroll after it, and copies the map strip that scrolls into view. */
void mnEndingImageScroll(void)
{
	Sint32 old = EndingImage_Scroll;
	Sint32 d;
	Sint32 scroll;
	Uint8 *source;
	Uint8 *destination;
	Uint32 start;
	Uint32 from;
	Uint32 offset;
	Sint32 i;

	if(EndingImage_FadeFrames < ENDING_IMAGE_FADE_FRAMES) {
		Sint32 t = ++EndingImage_FadeFrames / 4;
		Sint32 alpha;

		if(EndingImage_Block & 1) {
			alpha = ((BLEND_WEIGHT_MAX - t) << 8) | t;
			REG_BLDALPHA = alpha;
		} else {
			alpha = (t << 8) | (BLEND_WEIGHT_MAX - t);
			REG_BLDALPHA = alpha;
		}
	}
	if(EndingImage_TargetRow != EndingImage_Row) {
		d = EndingImage_TargetRow - EndingImage_Row;
		if(ABS(d) > EndingImage_Speed) {
			if(d > 0)
				d = EndingImage_Speed;
			else
				d = -EndingImage_Speed;
		}
		EndingImage_Row += d;
	}
	{
		/* FAKEMATCH(temporary): the scroll read through a pointer to it. */
		volatile Sint32 *word = &EndingImage_Scroll;
		FIXED			 target = toFIXED(EndingImage_Row);

		scroll = *word;
		if(scroll != target) {
			d = target - scroll;
			if(ABS(d) > toFIXED(EndingImage_Speed) / 4)
				d /= 8;
			if(ABS(d) > toFIXED(EndingImage_Speed)) {
				if(d > 0)
					d = toFIXED(EndingImage_Speed);
				else
					d = -toFIXED(EndingImage_Speed);
			}
			EndingImage_Scroll += d;
			EndingImage_MapScroll += d;
		}
	}
	if((EndingImage_Scroll ^ old) & toFIXED(8)) {
		source = EndingImage_Map;
		destination = (Uint8 *)BG_CHAR_BLOCK(0);
		start = ENDING_IMAGE_ROW_OFFSET(EndingImage_MapScroll);
		if(EndingImage_Block != 0)
			destination = (Uint8 *)BG_CHAR_BLOCK(2);
		scroll = EndingImage_Scroll;
		if(scroll > old) {
			from = ENDING_IMAGE_ROW_OFFSET((Uint32)scroll) +
				(ENDING_IMAGE_ROWS - 1) * ENDING_IMAGE_ROW_BYTES;
			offset = ENDING_IMAGE_WRAP(
				start + (ENDING_IMAGE_ROWS - 1) * ENDING_IMAGE_ROW_BYTES);
			for(i = 0; i < ENDING_IMAGE_COLUMNS; i++) {
				syDMAXCopy(source + from, destination + offset,
					DMA_COPY_WORDS(ENDING_IMAGE_ROW_BYTES / 4), REG_DMA3);
				source += ENDING_IMAGE_STRIP_BYTES;
				destination += ENDING_IMAGE_SCREEN_BYTES;
			}
		} else {
			from = ENDING_IMAGE_ROW_OFFSET((Uint32)EndingImage_Scroll);
			offset = ENDING_IMAGE_WRAP(start);
			for(i = 0; i < ENDING_IMAGE_COLUMNS; i++) {
				syDMAXCopy(source + from, destination + offset,
					DMA_COPY_WORDS(ENDING_IMAGE_ROW_BYTES / 4), REG_DMA3);
				source += ENDING_IMAGE_STRIP_BYTES;
				destination += ENDING_IMAGE_SCREEN_BYTES;
			}
		}
	}
	/* d, done with the step, takes the map's scroll in pixels. */
	d = FIXEDtoINT(EndingImage_MapScroll);
	BgScrollWork[2].x = d;
	BgScrollWork[3].x = d;
}

/* Starts the ending's images (the epilogue scene calls it first): resets the
   event table and the scroll, adds mnEndingImageScroll, sets up BG2, BG3 and
   blending, and builds the border strips. */
void mnEndingImageStart(void)
{
	CpuLoadDisplayEnabled = 0;
	DebugTextEnabled = 0;
	IdleSleepEnabled = 0;
	syInitEvent();
	EndingImage_MapScroll = 0;
	EndingImage_TargetRow = EndingImage_Row = EndingImage_Scroll = 0;
	EndingImage_Map = NULL;
	EndingImage_Speed = 1;
	EndingImage_Block = 0;
	sySetEvent(mnEndingImageScroll, EVENT_PASS_FRAME);
	syIOWrite16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
	mnCreditsBuildBackgroundTilemap(
		(Uint32 *)BG_SCREEN_BLOCK(CREDITS_BG3_SCREEN_BLOCK));
	mnCreditsBuildBackgroundTilemap(
		(Uint32 *)BG_SCREEN_BLOCK(CREDITS_BG2_SCREEN_BLOCK));
	mnCreditsClearBuffer(0);
	mnCreditsClearBuffer(1);
	syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(CREDITS_BG2_SCREEN_BLOCK),
		&REG_BG2CNT);
	syIOWrite16(BGCNT_PRIORITY(3) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(CREDITS_BG3_SCREEN_BLOCK),
		&REG_BG3CNT);
	syIOWrite16(
		DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON,
		&REG_DISPCNT);
	syIOWrite16(BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3 |
			BLDCNT_TGT2_BD,
		&REG_BLDCNT);
	syIOWrite16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);
	mnEndingImageInitStrips();
}

/* mnEndingImageCopyMap's body as mnEndingImageShow inlines it: the same copy,
   written with each column's strip and tilemap found from its number. */
static inline void CopyScrollMap(
	const Uint8 *source, Uint8 *destination, Sint32 position)
{
	Uint32 start;
	Sint32 column;
	Sint32 row;
	Uint32 offset;
	Uint32 from;

	start = ENDING_IMAGE_ROW_OFFSET(EndingImage_MapScroll);
	EndingImage_TargetRow = EndingImage_Row = position;
	EndingImage_Scroll = toFIXED(position);
	for(column = 0; column < ENDING_IMAGE_COLUMNS; column++) {
		Uint8		*to = destination + column * ENDING_IMAGE_SCREEN_BYTES;
		const Uint8 *strip = source + column * ENDING_IMAGE_STRIP_BYTES;
		from = (position / 8) * ENDING_IMAGE_ROW_BYTES;
		offset = ENDING_IMAGE_WRAP(start);
		for(row = 0; row < ENDING_IMAGE_ROWS; row++) {
			syDMAXCopy(strip + from, to + offset,
				DMA_COPY_WORDS(ENDING_IMAGE_ROW_BYTES / 4), REG_DMA3);
			from += ENDING_IMAGE_ROW_BYTES;
			offset = ENDING_IMAGE_WRAP(offset + ENDING_IMAGE_ROW_BYTES);
		}
	}
}

/* Shows a scrolling image on the hidden one of the two BG character blocks.
   With no resource it fills the map buffer with a plain tile word and clears
   the palette half; otherwise it clears the block's first 2 KB, expands
   resource's map into the buffer with EndingImage_DecodeCode copied to the heap
   (the fill word marks the second block's tiles), and queues the resource's
   128-colour palette. Then it copies the map at position into the tilemap
   (CopyScrollMap), flips the block and restarts the fade-in counter. */
void mnEndingImageShow(Sint32 resource, Uint8 *map, Sint32 position)
{
	volatile Uint32 cell;
	Uint32			word;
	Uint32			fill = 0;
	Uint8		   *tiles;
	Uint16		   *palette;
	Uint8		   *data;
	Sint32 (*code)(Uint8 *, Uint8 *, Uint32);

	if(resource == 0) {
		if(EndingImage_Block != 0) {
			word = PIXELS_8BPP(1);
			tiles = (Uint8 *)BG_CHAR_BLOCK(0);
			palette = (Uint16 *)BG_PLTT;
		} else {
			word = PIXELS_8BPP(129);
			tiles = (Uint8 *)BG_CHAR_BLOCK(2);
			palette = (Uint16 *)&BG_PLTT_COLOR(8, 0);
		}
		DMA_FILL32_CELL(cell, word, map, ENDING_IMAGE_MAP_BYTES);
		DMA_FILL32_CELL(cell, 0, palette, BACKDROP_PALETTE_BYTES);
	} else {
		if(EndingImage_Block != 0) {
			tiles = (Uint8 *)BG_CHAR_BLOCK(0);
			palette = (Uint16 *)BG_PLTT;
		} else {
			fill = PIXELS_8BPP(128);
			tiles = (Uint8 *)BG_CHAR_BLOCK(2);
			palette = (Uint16 *)&BG_PLTT_COLOR(8, 0);
		}
		DMA_FILL32_CELL(cell, 0, tiles, ENDING_IMAGE_SCREEN_BYTES);
		data = syResourceGetTableEntry(resource);
		RAM_CODE_LOAD(code, EndingImage_DecodeCode, EndingImage_DecodeCodeSize);
		code(data + BACKDROP_PALETTE_BYTES, map, fill);
		syHeapRelease(code);
		IO_WRITE_QUEUE_PUSH((Uint32)palette, (Uint32)data,
			DMA_COPY_WORDS(BACKDROP_PALETTE_BYTES / 4));
	}
	EndingImage_Map = map;
	CopyScrollMap(map, tiles, position);
	EndingImage_Block ^= 1;
	EndingImage_FadeFrames = 0;
}

/* Waits, for at most 300 frames, until the scroll reaches its row. */
void mnEndingImageWaitForScroll(void)
{
	Sint32 frames = 0;

	if(EndingImage_Scroll != toFIXED(EndingImage_Row)) {
		do {
			sySynch(1);
			frames++;
			if(frames >= ENDING_IMAGE_SCROLL_TIMEOUT)
				break;
		} while(EndingImage_Scroll != toFIXED(EndingImage_Row));
	}
}

/* A VBlank event added by mnEndingImageInitStrips: queues the 24 border strip
   sprites. */
void mnEndingImageDrawStrips(void)
{
	QUEUED_SPR *entry = EndingImage_Strips;
	Sint32		i;

	for(i = 0; i < ENDING_IMAGE_STRIP_SPRITES; i++)
		syOAMAddSprAtDepthClamped(entry++, 0);
}

/* Allocates the 24 sprite entries of the scrolling image's border strips, fills
   their tiles with colour 1 and adds mnEndingImageDrawStrips to draw them; the
   entries are written as words, as The Broken Seal's mnCreditsInitCrawlSprs
   writes its strips. */
void mnEndingImageInitStrips(void)
{
	Sint32	slot = syVRAMCacheFindFreeEntry();
	Uint32 *words;
	Sint32	tile;
	Uint32	i;

	EndingImage_Strips =
		syHeapAllocEWRAM(ENDING_IMAGE_STRIP_SPRITES * sizeof(QUEUED_SPR));
	tile = syVRAMCacheLoad(slot, 512, 0);
	DMA_FILL32(
		PIXELS_4BPP(1), (void *)(OBJ_VRAM0 + tile * TILE_SIZE_4BPP), 512);
	words = (Uint32 *)EndingImage_Strips;
	for(i = 0; i < 8; i++) {
		*words++ = 0;
		*words++ = OBJ_X(i * 32) | OBJ_SIZE_32X16 | 120;
		*words++ = tile;
	}
	for(i = 0; i < 8; i++) {
		*words++ = 0;
		*words++ = OBJ_X(i * 32) | OBJ_SIZE_32X16 | 136;
		*words++ = tile | OBJ_PRIORITY(2);
	}
	for(i = 0; i < 8; i++) {
		*words++ = 0;
		*words++ = OBJ_X(i * 32) | OBJ_SIZE_32X8 | 152;
		*words++ = tile | OBJ_PRIORITY(2);
	}
	sySetEvent(mnEndingImageDrawStrips, EVENT_PASS_FRAME);
}

/* Moves the top strip's eight sprites to y 120 when enable is set, and to y 0
   otherwise. */
void mnEndingImageLowerTopStrip(Sint32 enable)
{
	QUEUED_SPR *entry = EndingImage_Strips;
	Uint32		i;

	if(!enable) {
		for(i = 0; i < 8; i++) {
			entry->oam.y = 0;
			entry++;
		}
	} else {
		for(i = 0; i < 8; i++) {
			entry->oam.y = 120;
			entry++;
		}
	}
}

/* clang-format off */
/* Unused: the slideshow backdrops, ending in 0. */
const Uint32 DisplayScroll_UnusedSlideResources[] = {
	RESOURCE_VALE_STORM_BACKDROP,
	RESOURCE_SOL_SANCTUM_BACKDROP,
	RESOURCE_GRASSLAND_BACKDROP,
	RESOURCE_ROOMS_BACKDROP,
	RESOURCE_FOREST_BACKDROP,
	RESOURCE_KOLIMA_FOREST_BACKDROP,
	RESOURCE_TRET_TREE_ROOMS_BACKDROP,
	RESOURCE_TRET_TREE_BRANCH_BACKDROP,
	RESOURCE_SNOWFIELD_BACKDROP,
	RESOURCE_MERCURY_LIGHTHOUSE_BACKDROP,
	RESOURCE_MERCURY_LIGHTHOUSE_AERIE_BACKDROP,
	RESOURCE_IMIL_FUCHIN_FALLS_BACKDROP,
	RESOURCE_MOGALL_FOREST_BACKDROP,
	RESOURCE_ALTIN_PEAK_BACKDROP,
	RESOURCE_LAMAKAN_DESERT_BACKDROP,
	RESOURCE_TOLBI_SHIP_DECK_BACKDROP,
	RESOURCE_TOLBI_SHIP_ROOMS_BACKDROP,
	RESOURCE_BEACH_BACKDROP,
	RESOURCE_COLOSSO_BACKDROP,
	RESOURCE_ALTMILLER_CAVE_BACKDROP,
	RESOURCE_SUHALLA_DESERT_BACKDROP,
	RESOURCE_CROSSBONE_ISLE_CAVE_BACKDROP,
	RESOURCE_PIRATE_SHIP_BACKDROP,
	RESOURCE_SUHALLA_GATE_BACKDROP,
	RESOURCE_TUNNEL_RUINS_BACKDROP,
	RESOURCE_BABI_LIGHTHOUSE_BACKDROP,
	RESOURCE_VENUS_LIGHTHOUSE_BACKDROP,
	RESOURCE_VENUS_LIGHTHOUSE_AERIE_BACKDROP,
	RESOURCE_VENUS_LIGHTHOUSE_AERIE_STORM_BACKDROP,
	RESOURCE_DUSK_CLOUDS_BACKDROP,
	RESOURCE_VIOLET_SKY_BACKDROP,
	0
};

/* The backdrops the slideshow fades between, in turn; 0 ends the list, */
/* and the table has room for 25. */
const Uint32 DisplayScroll_SlideResources[25] = {
	RESOURCE_VALE_STORM_BACKDROP,
	RESOURCE_SOL_SANCTUM_BACKDROP,
	RESOURCE_ROOMS_BACKDROP,
	RESOURCE_KOLIMA_FOREST_BACKDROP,
	RESOURCE_TRET_TREE_BRANCH_BACKDROP,
	RESOURCE_SNOWFIELD_BACKDROP,
	RESOURCE_MERCURY_LIGHTHOUSE_BACKDROP,
	RESOURCE_IMIL_FUCHIN_FALLS_BACKDROP,
	RESOURCE_MOGALL_FOREST_BACKDROP,
	RESOURCE_ALTIN_PEAK_BACKDROP,
	RESOURCE_LAMAKAN_DESERT_BACKDROP,
	RESOURCE_TOLBI_SHIP_DECK_BACKDROP,
	RESOURCE_TOLBI_SHIP_ROOMS_BACKDROP,
	RESOURCE_BEACH_BACKDROP,
	RESOURCE_COLOSSO_BACKDROP,
	RESOURCE_SUHALLA_DESERT_BACKDROP,
	RESOURCE_TUNNEL_RUINS_BACKDROP,
	RESOURCE_BABI_LIGHTHOUSE_BACKDROP,
	RESOURCE_VENUS_LIGHTHOUSE_BACKDROP,
	RESOURCE_VENUS_LIGHTHOUSE_AERIE_BACKDROP,
	RESOURCE_DUSK_CLOUDS_BACKDROP,
	0
};

/* An empty credits line, between the names. */
static const Uint8 sBlankCreditLine[] = "";

#if (LANGUAGE == LANGUAGE_GERMAN)
#define CREDITS_SUBTITLE "DIE VERGESSENE EPOCHE"
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define CREDITS_SUBTITLE "LA EDAD PERDIDA"
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define CREDITS_SUBTITLE "L'\177GE PERDU" /* L'ÂGE PERDU: the French font draws Â at 0x7f */
#elif (LANGUAGE == LANGUAGE_ITALIAN)
#define CREDITS_SUBTITLE "L'ERA PERDUTA"
#endif

/* The Spanish, French and Italian fonts draw í at '`'. */
#if (LANGUAGE == LANGUAGE_GERMAN)
#define RODRIGUEZ "Rodriguez"
#else
#define RODRIGUEZ "Rodr`guez"
#endif

/* The staff credits, one line each, ending in 0. */
const void *DisplayScroll_LineTable[] = {
	sBlankCreditLine,
#if (LANGUAGE == LANGUAGE_JAPANESE)
	"OUGON NO TAIYO STAFF",
	sBlankCreditLine,
#elif (LANGUAGE == LANGUAGE_ENGLISH)
	"GOLDEN SUN: THE LOST AGE",
	"STAFF",
#else
	"GOLDEN SUN",
	CREDITS_SUBTITLE,
	"STAFF",
#endif
	sBlankCreditLine,
	"Game Planning",
	sBlankCreditLine,
	"Hiroyuki Takahashi",
	"Shugo Takahashi",
	sBlankCreditLine,
	sBlankCreditLine,
	"Scenario",
	sBlankCreditLine,
	"Hiroyuki Takahashi",
	sBlankCreditLine,
	sBlankCreditLine,
	"Director",
	sBlankCreditLine,
	"Shugo Takahashi",
	sBlankCreditLine,
	sBlankCreditLine,
	"Assistant Director",
	sBlankCreditLine,
	"Yusuke Sugimoto",
	sBlankCreditLine,
	sBlankCreditLine,
	"Game Design",
	sBlankCreditLine,
	"Yusuke Sugimoto",
	"Shuji Shimizu",
	"Ayumu Shindo",
	"Kentaro Sakou",
	"Norisumi Osawa",
	"Masato Kawamura",
	CREDITS_PAGE_MARK,
	"Programming",
	sBlankCreditLine,
	sBlankCreditLine,
	"System ",
	sBlankCreditLine,
	"Yasuhiro Taguchi",
	sBlankCreditLine,
	sBlankCreditLine,
	"Battle",
	sBlankCreditLine,
	"Haruki Kodera",
	"Toru Takamatsu",
	sBlankCreditLine,
	sBlankCreditLine,
	"Field",
	sBlankCreditLine,
	"Kaoru Shimada",
	"Hiroyuki Ichikawa",
	"Akiko Sato",
	"Kenji Numaya",
#if CREDITS_REVISED_FOR_LOCALIZATION
	"P.E. Jareth Hein",
#else
	"P.E.Jareth Hein",
#endif
	sBlankCreditLine,
	sBlankCreditLine,
	"User Interface",
	sBlankCreditLine,
	"Yutaka Yamamoto",
	CREDITS_PAGE_MARK,
#if CREDITS_REVISED_FOR_LOCALIZATION
	"Graphic Art",
#else
	"Graphic Arts",
#endif
	sBlankCreditLine,
	sBlankCreditLine,
	"Battle Characters",
	sBlankCreditLine,
	"Junko Nakamura",
	"Kaori Matsunaga",
	sBlankCreditLine,
	sBlankCreditLine,
	"Battle Effects",
	"& Faces",
	sBlankCreditLine,
	"Junichi Ochiai",
	sBlankCreditLine,
	sBlankCreditLine,
	"Battle Backgrounds",
	sBlankCreditLine,
	"Hiroto Nakashima",
	sBlankCreditLine,
	sBlankCreditLine,
	"Maps",
	sBlankCreditLine,
	"Masayuki Hashimoto",
	"Mitsumasa Muraishi",
	"Mitsuru Fukumoto",
	"Akihito Kimura",
	"Shinichiro Sugimoto",
	"Natsuko Takeuchi",
	sBlankCreditLine,
	sBlankCreditLine,
	"Map Characters",
	sBlankCreditLine,
	"Mieko Koguchi",
	"Kanako Horiguchi",
	sBlankCreditLine,
	sBlankCreditLine,
	"Graphics Direction",
	sBlankCreditLine,
	"Fumihide Aoki",
	CREDITS_PAGE_MARK,
	"Character Design",
	sBlankCreditLine,
	"Shin Yamanouchi",
	sBlankCreditLine,
	sBlankCreditLine,
	"Monster Design",
	sBlankCreditLine,
	"Hiroshi Kajiyama",
	sBlankCreditLine,
	sBlankCreditLine,
	"Music",
	sBlankCreditLine,
	"Motoi Sakuraba",
	sBlankCreditLine,
	sBlankCreditLine,
	"Sound Effects",
	sBlankCreditLine,
	"Hiroyuki Hamada",
	sBlankCreditLine,
	sBlankCreditLine,
	"Sound Direction",
	sBlankCreditLine,
	"Masaaki Uno",
	sBlankCreditLine,
	sBlankCreditLine,
	"Staff Coordination",
	sBlankCreditLine,
	"Akiko Sato",
	"Kenji Numaya",
	CREDITS_PAGE_MARK,
	"Illustration",
	sBlankCreditLine,
	"Shin Yamanouchi",
	"Hiroshi Kajiyama",
	"Masanori Sato",
	sBlankCreditLine,
	sBlankCreditLine,
	"Graphic Support",
	sBlankCreditLine,
	"Yoichi Kotabe",
	"Masanori Sato",
	sBlankCreditLine,
	sBlankCreditLine,
#if !CREDITS_REVISED_FOR_LOCALIZATION
	"Artwork Design",
	sBlankCreditLine,
	"Nanako Kinoshita",
	"Takashi Maeda",
	"Naoko Okamoto",
	"Shinji Yamamoto",
	sBlankCreditLine,
	sBlankCreditLine,
	"Public Relations",
	sBlankCreditLine,
	"Mitsuaki Hagishima",
	"Asako Haruhana",
	sBlankCreditLine,
	sBlankCreditLine,
#endif
	"Coordination",
	sBlankCreditLine,
	"Masaaki Uno",
	"Toshiharu Izuno",
	"Kenichi Nishida",
	sBlankCreditLine,
	sBlankCreditLine,
#if !CREDITS_REVISED_FOR_LOCALIZATION
	"Special Thanks to",
	"Super Mario Club Staff",
	sBlankCreditLine,
	"Yoshihiro Maruyama",
	"Nobuaki Maeda",
	"Tadasumi Ueno",
	"Tomoharu Hizakae",
	"Yoshifumi Ogawa",
	"Naotaka Matsumoto",
	"Kei Nadabe",
	"Masami Kato",
	"Hideki Katsui",
	"Hiroyuki Murata",
	"Junko Komaki",
	"Kayo Komaki",
	"Hisashi Takizaki",
	"Akiko Matsumoto",
	"Norihide Okamura",
	"Satoru Uno",
	"Masaki Mori",
	"Tomoko Yuasa",
	"Takeshi Morishima",
	"Yoshikazu Tanaka",
	"Haruka Kato",
	"Norihiko Morikawa",
	"Katsuhiro Nii",
	"Katsunori Masutani",
	"Yukio Homma",
	"Ayumi Hidaka",
	"Kuniharu Naitou",
	"Shunsuke Kobori",
	"Masaharu Shima",
	"Masaru Ikeshita",
	"Naoki Tsuta",
	"Satoshi Mutou",
	"Takayuki Murayama",
	"Tsuyoshi Miyamae",
	"Eri Hashimoto",
#else
	"North American",
	" Localization",
	sBlankCreditLine,
	"Translation",
	sBlankCreditLine,
	"William Barnes",
	"Reiko Ninomiya",
	"Tim O'Leary",
	"Scot Ritchey",
	"Bill Trinen",
	sBlankCreditLine,
	"Writing",
	" & Editing",
	sBlankCreditLine,
	"Richard Amtower",
	"Shawn Seavers",
	sBlankCreditLine,
	"Localization",
	" Management",
	sBlankCreditLine,
	"Leslie Swan",
	"Jeff Miller",
	sBlankCreditLine,
	sBlankCreditLine,
	"NOA Product Testing",
	"Michael Kelbaugh",
	"Eric Bush",
	"Todd Buechele",
	"Tim Casey",
	"Jeffrey Storbo",
	sBlankCreditLine,
#if (LANGUAGE != LANGUAGE_ENGLISH)
	sBlankCreditLine,
	"European Localization",
	sBlankCreditLine,
	"Product Localization",
	"Team",
	sBlankCreditLine,
	"Coordination",
	"Andy Fey",
	sBlankCreditLine,
	"Translation",
	sBlankCreditLine,
	/* Each edition names its own translator first. */
#if (LANGUAGE == LANGUAGE_GERMAN)
	"German",
	"Micky Auer",
	sBlankCreditLine,
	"Spanish",
	"Carlos Montilla",
	RODRIGUEZ,
	sBlankCreditLine,
	"French",
	"Zadia Messerli",
	sBlankCreditLine,
	"Italian",
	"Christian Massi",
	sBlankCreditLine,
#elif (LANGUAGE == LANGUAGE_SPANISH)
	"Spanish",
	"Carlos Montilla",
	RODRIGUEZ,
	sBlankCreditLine,
	"German",
	"Micky Auer",
	sBlankCreditLine,
	"French",
	"Zadia Messerli",
	sBlankCreditLine,
	"Italian",
	"Christian Massi",
	sBlankCreditLine,
#elif (LANGUAGE == LANGUAGE_FRENCH)
	"French",
	"Zadia Messerli",
	sBlankCreditLine,
	"German",
	"Micky Auer",
	sBlankCreditLine,
	"Spanish",
	"Carlos Montilla",
	RODRIGUEZ,
	sBlankCreditLine,
	"Italian",
	"Christian Massi",
	sBlankCreditLine,
#else
	"Italian",
	"Christian Massi",
	sBlankCreditLine,
	"German",
	"Micky Auer",
	sBlankCreditLine,
	"French",
	"Zadia Messerli",
	sBlankCreditLine,
	"Spanish",
	"Carlos Montilla",
	RODRIGUEZ,
	sBlankCreditLine,
#endif
	"Validation",
	"& Testing Team",
#endif
#endif
	CREDITS_PAGE_MARK,
	"Produced by",
	sBlankCreditLine,
	"Shinji Hatano",
	"Hiroyuki Takahashi",
	"Shugo Takahashi",
	sBlankCreditLine,
	sBlankCreditLine,
	"Executive Producer",
	sBlankCreditLine,
	"Hiroshi Yamauchi",
	sBlankCreditLine,
	sBlankCreditLine,
#if CREDITS_REVISED_FOR_LOCALIZATION
	"Copyrights of Game, Scenario,",
#else
	"Copyrights of Game, Scenario",
#endif
	" and Program reserved by",
	sBlankCreditLine,
	"NINTENDO",
	"and",
	"CAMELOT",
	sBlankCreditLine,
	"Copyrights of All Characters",
	"and Music reserved by",
	sBlankCreditLine,
	"NINTENDO",
	sBlankCreditLine,
	CREDITS_PAGE_MARK,
	0
};
/* clang-format on */

#include "../graphics/credits/glyph_widths.h"

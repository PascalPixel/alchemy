// The staff credits: the battle backdrops behind them, the lines that scroll past, and the
// scrolling images with their border strips.
//
// One object, as The Broken Seal's credits.c is, with its slides and lines at its end; the
// background decoders linked before it are assembly (credits_background_decode.s and
// credits_background_expand.s, then the ending images' ending_image_decode.s and
// ending_image_expand.s).

#include "field/field.h"
#include "game/game.h"
#include "menu/menu.h"
#include "edition.h"
#include "text/msg_ids.h"
#include "macros.h"

struct QUEUED_SPR;

typedef Sint32 (*PACKED_DECODER)(const Uint8 *source, void *destination, Uint32 fill);

// One eight-byte OAM entry in the credits' sprite buffer.
typedef struct DISPLAY_SCROLL_OBJECT
{
    Uint32 attr01;
    Uint32 attr2;
} DISPLAY_SCROLL_OBJECT;

extern const Uint32 DisplayScrollSlideResources[];
// Draws one line of the credits' scrolling text into object tiles from slot on.
Sint32              mnDisplayScrollDrawMessageLine(Sint32 msg, Sint32 slot, Sint32 align);
extern const void  *DisplayScrollLineTable[];

void                mnCreditsInitCrawlSprites(void);
void                mnCreditsInitTextSpritesAtPage(Sint32 credits);
void                mnCreditsStopTextSprites(void);
Sint32              mnCreditsDrawLine(const Uint8 *text, Sint32 firstTile, Sint32 alignment);
// mnCreditsDecodeBackground, the ARM decoder RAM_CODE_LOAD copies into a heap block to run
// (credits_background_decode.s).
extern const Uint8  mnCreditsDecodeBackgroundCode[];
// mnCreditsDecodeBackgroundCode's bytes, as its address.
extern Uint8        CreditsDecodeBackgroundCodeSize[];
extern const Uint8  CreditsGlyphWidths[];
// The credits' font, the glyphs mnCreditsDrawLine draws a line's tiles from.
extern const Uint8  DisplayScrollFont[];

void                mnEndingImageInitStrips(void);
// EndingImageDecode, the ARM decoder RAM_CODE_LOAD copies into a heap block to run
// (ending_image_decode.s).
extern const Uint8  EndingImageDecodeCode[];
// EndingImageDecodeCode's bytes, as its address.
extern Uint8        EndingImageDecodeCodeSize[];

// Credits text uses six 32-pixel sprites across each 192-pixel line, drawn into a ring of 32 lines
// of sprite tiles.
#define CREDITS_LINE_PIXELS       192
#define CREDITS_VISIBLE_ROWS      16
#define CREDITS_COLUMNS           6
#define CREDITS_RING_ROWS         32
#define CREDITS_LINE_TILES        24 // A line's 4bpp tiles, 192 / 8.
#define CREDITS_RING_TILES        (CREDITS_RING_ROWS * CREDITS_LINE_TILES)
#define CREDITS_RING_BYTES        (CREDITS_RING_TILES * TILE_SIZE_4BPP)
// A line's raster: nine rows of 256 one-byte pixels, the ninth for the shadow below the eighth.
#define CREDITS_BUFFER_BYTES      2304
#define CREDITS_RASTER_STRIDE     256
// OAM entries 0 to 23: the opaque strips above and below the text.
#define CREDITS_BORDER_SPRITES    24
// The whole OAM: the borders, 96 text sprites and 8 parked off screen.
#define CREDITS_OAM_ENTRIES       128
// The border sprites' one 32x16 picture, solid in colour 1, after the ring.
#define CREDITS_BORDER_TILE_BYTES (8 * TILE_SIZE_4BPP)
// One ring line's tiles: the top or bottom half of a drawn line.
#define CREDITS_LINE_BYTES        (CREDITS_LINE_TILES * TILE_SIZE_4BPP)
// The line table's page mark, which ends a stretch of the credits: the scroll stops on it, and a
// page's start is found by counting these marks. Then how far mnCreditsInitTextSpritesAtPage counts
// for one, and the ticks the credits run on after a mark.
#define CREDITS_PAGE_MARK         ((const void *)255)
#define CREDITS_MAX_LINES         500
#define CREDITS_TICKS_AFTER_END   375
// The ring line a scroll line is drawn into, sixteen rows below the top.
#define CREDITS_RING_LINE(line)                                                                    \
    ((((line) + CREDITS_VISIBLE_ROWS) & (CREDITS_RING_ROWS - 1)) * CREDITS_LINE_TILES)
// The scene flag that says a credits line was drawn before, so the next one starts from its shadow
// row.
#define FLAG_CREDITS_LINE_DRAWN      FLAG_SCENE_TEMP_00
// The text sprites' rows start below the top strip, and their 192 pixels stand in the middle of the
// screen.
#define CREDITS_TEXT_TOP             16
#define CREDITS_TEXT_LEFT            ((DISPLAY_WIDTH - CREDITS_LINE_PIXELS) / 2)
// The x and the y at which the eight spare sprites wait, (192, 192), below the screen.
#define CREDITS_PARKED               192
// A line's letters are drawn in colour 15, each with its shadow a pixel below and to the right in
// colour 1, the strips' black.
#define CREDITS_TEXT_COLOR           15
#define CREDITS_SHADOW_COLOR         1
// A glyph of the credits' font is eight rows of a byte each, its leftmost pixel the top bit.
#define CREDITS_GLYPH_LEFT_PIXEL_BIT 0x80
// A message of the story crawl takes two scroll lines of 8 pixels.
#define CRAWL_MESSAGE_PIXELS         (2 * 8)

// DisplayScrollBusy while the story crawl runs: what its last line stopped it on. A crawl line of
// dashes draws nothing and stops the crawl, the dashes past the first saying how
// (mnTextDrawMessageToCanvas).
typedef enum CRAWL_STOP
{
    CRAWL_SCROLLING      = 0,
    // "--": the crawl waits for the next slide, which A or B releases.
    CRAWL_STOP_FOR_SLIDE = 1,
    CRAWL_STOP_END       = 2, // "---", the crawl's last line: the crawl ends
    CRAWL_STOP_LONG      = 3  // "----", which no crawl line has
} CRAWL_STOP;

// Where mnCreditsDrawLine places a line in its 192 pixels.
typedef enum CREDITS_ALIGNMENT
{
    CREDITS_ALIGN_LEFT   = 0,
    CREDITS_ALIGN_CENTER = 1,
    CREDITS_ALIGN_RIGHT  = 2
} CREDITS_ALIGNMENT;

// The ending's scrolling images: a map buffer of 15 columns, each a strip of 4 KB copied into its
// own 2 KB screen block, 31 rows of 64 bytes at a time.
#define ENDING_IMAGE_COLUMNS            15
#define ENDING_IMAGE_STRIP_BYTES        4096
#define ENDING_IMAGE_SCREEN_BYTES       BG_SCREEN_SIZE
#define ENDING_IMAGE_ROWS               31
#define ENDING_IMAGE_ROW_BYTES          64
// A row's offset wrapped into its screen block.
#define ENDING_IMAGE_WRAP(offset)       ((offset) & (ENDING_IMAGE_SCREEN_BYTES - 1))
// The fade-in's frames, four to each blend step.
#define ENDING_IMAGE_FADE_FRAMES        (BLEND_WEIGHT_MAX * 4)
// The map offset of the row of eight pixels a 16.16 scroll stands at.
#define ENDING_IMAGE_ROW_OFFSET(scroll) ((FIXEDtoINT(scroll) / 8) * ENDING_IMAGE_ROW_BYTES)
// The map buffer: every column's strip.
#define ENDING_IMAGE_MAP_BYTES          (ENDING_IMAGE_COLUMNS * ENDING_IMAGE_STRIP_BYTES)
// mnEndingImageWaitForScroll gives up after five seconds.
#define ENDING_IMAGE_SCROLL_TIMEOUT     (5 * TICKS_PER_SECOND)
// The image's border strips: eight sprites across at y 120, 136 and 152.
#define ENDING_IMAGE_STRIP_SPRITES      24

// A backdrop or image resource begins with its 128 colours, the palette half it takes; its packed
// tiles follow.
#define BACKDROP_COLORS                 128
#define BACKDROP_PALETTE_BYTES          (BACKDROP_COLORS * 2)
// A battle backdrop's 480 tiles of 256 colours, 256 by 120 pixels.
#define BACKDROP_TILES                  480
#define BACKDROP_TILE_BYTES             (BACKDROP_TILES * 64)
// The credits grey a backdrop's colours: a channel becomes the colour's brightness (2 red + 4 green
// + blue) and BACKDROP_CHANNEL_WEIGHT parts of itself, over BACKDROP_GREY_PARTS. The weights add to
// 12, so a grey keeps three quarters of its level.
#define BACKDROP_CHANNEL_WEIGHT         5
#define BACKDROP_GREY_PARTS             16

// The two images' screen blocks, each holding the same tilemap: BG3's, over character block 0, and
// BG2's, over character block 2.
#define CREDITS_BG3_SCREEN_BLOCK        15
#define CREDITS_BG2_SCREEN_BLOCK        31
// A tilemap word: two map entries side by side, `left` in its low half.
#define TILEMAP_PAIR(left, right)       (((right) << 16) | (left))
// A tilemap row's words.
#define TILEMAP_ROW_WORDS               (BG_SCREEN_WIDTH / 2)
// The tile the margins show: the character block's last, which lies in the screen block's own
// zeroed tail and so is clear.
#define CREDITS_BLANK_TILE              511

// Fills a backdrop's 256-colour tiles with its half's first colour, an opaque black for the next
// crossfade, and clears the half's 128 palette colours. alternate is 0 for the first half
// (character block 0, colours 0 to 127), 1 for the second (character block 2, colours 128 to 255).
void mnCreditsClearBuffer(Sint32 alternate)
{
    Uint32          fillColor;
    Uint32          tileDest;
    Uint32          paletteDest;
    volatile Uint32 dmaFill;

    if (alternate == 0)
    {
        fillColor   = PIXELS_8BPP(1);
        tileDest    = (Uint32)BG_CHAR_BLOCK(0);
        paletteDest = (Uint32)BG_PLTT;
    }
    else
    {
        fillColor   = PIXELS_8BPP(129);
        tileDest    = (Uint32)BG_CHAR_BLOCK(2);
        paletteDest = (Uint32)&BG_PLTT_COLOR(8, 0);
    }
    DMA_FILL32_CELL(dmaFill, fillColor, (void *)tileDest, BACKDROP_TILE_BYTES);
    DMA_FILL32_CELL(dmaFill, 0, (void *)paletteDest, BACKDROP_PALETTE_BYTES);
}

// Loads a backdrop resource (its 128 colours, then its packed tiles) into one half of BG VRAM and
// queues its palette for the next VBlank, as The Broken Seal does, but greyed by
// BACKDROP_CHANNEL_WEIGHT and BACKDROP_GREY_PARTS. A zero resource clears that half instead. The
// tiles are unpacked by mnCreditsDecodeBackground, run from a heap block, and the grey colours are
// built in CreditsGreyColors: the credits have no buffer of their own for them, and borrow the
// cells of the SDK's flash library, which IdentifyFlash sets again before the flash code reads
// them.
void mnCreditsLoadBackground(Uint32 resourceId, Sint32 alternate)
{
    const Uint8   *resourceData;
    Uint32         paletteDest;
    Uint32         paletteOffset;
    void          *tileDest;
    PACKED_DECODER decoder;
    Sint32         i;

    if (resourceId == 0)
    {
        mnCreditsClearBuffer(alternate);
        return;
    }
    resourceData = syResourceGetTableEntry(resourceId);
    if (alternate == 0)
    {
        paletteOffset = 0;
        tileDest      = (void *)BG_CHAR_BLOCK(0);
        paletteDest   = (Uint32)BG_PLTT;
    }
    else
    {
        paletteOffset = PIXELS_8BPP(128);
        tileDest      = (void *)BG_CHAR_BLOCK(2);
        paletteDest   = (Uint32)&BG_PLTT_COLOR(8, 0);
    }
    RAM_CODE_LOAD(decoder, mnCreditsDecodeBackgroundCode, CreditsDecodeBackgroundCodeSize);
    decoder(resourceData + BACKDROP_PALETTE_BYTES, tileDest, paletteOffset);
    syHeapRelease(decoder);

    for (i = 0; i < BACKDROP_COLORS; i++)
    {
        Uint16 color          = *(const Sint16 *)resourceData;
        Sint32 blue           = RGB_BLUE(color);
        Sint32 green          = RGB_GREEN(color);
        Sint32 red            = RGB_RED(color);
        Sint32 sum            = (green * 2 + red) * 2 + blue;

        resourceData         += 2;
        blue                  = (sum + blue * BACKDROP_CHANNEL_WEIGHT) / BACKDROP_GREY_PARTS;
        green                 = (sum + green * BACKDROP_CHANNEL_WEIGHT) / BACKDROP_GREY_PARTS;
        red                   = (sum + red * BACKDROP_CHANNEL_WEIGHT) / BACKDROP_GREY_PARTS;
        CreditsGreyColors[i]  = RGB(red, green, blue);
    }
    IO_WRITE_QUEUE_PUSH(
        paletteDest, (Uint32)CreditsGreyColors, DMA_COPY_WORDS(BACKDROP_PALETTE_BYTES / 4));
}

// Builds a backdrop's tilemap, as The Broken Seal does: two blank rows, the backdrop's 480 tiles in
// order over fifteen rows, three blank rows, then a zeroed tail.
void mnCreditsBuildBackgroundTilemap(Uint32 *dest)
{
    Sint32 i;
    Uint32 blankTiles;
    Uint32 tilePair;

    blankTiles = TILEMAP_PAIR(CREDITS_BLANK_TILE, CREDITS_BLANK_TILE);
    tilePair   = TILEMAP_PAIR(0, 1);
    // Pad above and below the image so vertical wrapping stays blank.
    for (i = 0; i < 2 * TILEMAP_ROW_WORDS; i++)
        *dest++ = blankTiles;
    for (i = 0; i < BACKDROP_TILES / 2; i++)
    {
        *dest++   = tilePair;
        tilePair += TILEMAP_PAIR(2, 2);
    }
    for (i = 0; i < 3 * TILEMAP_ROW_WORDS; i++)
        *dest++ = blankTiles;
    for (i = 0; i < 12 * TILEMAP_ROW_WORDS; i++)
        *dest++ = 0;
}

// A VBlank event of the credits: drifts both images together, keeping their horizontal offsets
// aligned throughout the crossfade.
void mnCreditsScrollBackgrounds(void)
{
    if ((FrameCount & 3) == 0)
    {
        BgScrollWork[2].x--;
        BgScrollWork[3].x--;
    }
}

// Runs the title's story crawl or the staff credits over the greyed battle backdrops. The story
// crawl (CREDITS_MODE_STORY_CRAWL) cross-fades a new backdrop each time the crawl stops for one,
// until Start or the crawl's end; any other mode scrolls the staff credits from page mode -
// CREDITS_MODE_FIRST_PAGE and returns once they have run out.
Sint32 mnCreditsRun(Sint32 mode)
{
    Sint32 slide;
    Sint32 step;
    Sint32 frame;

    OamCopyEnabled        = 0;
    CpuLoadDisplayEnabled = 0;
    DebugTextEnabled      = 0;
    CreditsPendingSlides  = 0;
    IdleSleepEnabled      = 0;
    syInitEvent();
    sySetEvent(mnCreditsScrollBackgrounds, EVENT_PASS_VBLANK);
    syIOWrite16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
    mnCreditsBuildBackgroundTilemap((Uint32 *)BG_SCREEN_BLOCK(CREDITS_BG3_SCREEN_BLOCK));
    mnCreditsBuildBackgroundTilemap((Uint32 *)BG_SCREEN_BLOCK(CREDITS_BG2_SCREEN_BLOCK));
    mnCreditsClearBuffer(0);
    mnCreditsClearBuffer(1);
    syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_256_COLORS
                    | BGCNT_SCREEN_BASE(CREDITS_BG2_SCREEN_BLOCK),
                &REG_BG2CNT);
    syIOWrite16(BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(CREDITS_BG3_SCREEN_BLOCK),
                &REG_BG3CNT);
    syIOWrite16(DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON,
                &REG_DISPCNT);
    syIOWrite16(BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_BD,
                &REG_BLDCNT);

    if (mode == CREDITS_MODE_STORY_CRAWL)
    {
        mnCreditsInitCrawlSprites();
        sySynch(60);
        for (slide = 0;; slide++)
        {
            Sint16 *count = &CreditsPendingSlides;

            mnCreditsLoadBackground(DisplayScrollSlideResources[slide], (slide & 1) ^ 1);
            for (step = 1; step <= BLEND_WEIGHT_MAX; step++)
            {
                // Fade the new half in: an even slide went to BG2, the first target (BLDALPHA
                // bits 4..0), an odd one to BG3, the second (bits 12..8).
                if (slide & 1)
                    syIOWrite16((step << 8) | (BLEND_WEIGHT_MAX - step), &REG_BLDALPHA);
                else
                    syIOWrite16(((BLEND_WEIGHT_MAX - step) << 8) | step, &REG_BLDALPHA);
                for (frame = 0; frame <= 3; frame++)
                {
                    if (PerWork.data & KEY_START)
                        goto done;
                    sySynch(1);
                }
            }
            if (*count == 0)
            {
                do
                {
                    // DEAD: a read of the pressed keys nothing uses; the ROM keeps its load.
                    PerWork.push;
                    if (PerWork.data & KEY_START)
                        goto done;
                    if (DisplayScrollBusy == CRAWL_STOP_END)
                        goto done;
                    sySynch(1);
                } while (CreditsPendingSlides == 0);
            }
            CreditsPendingSlides--;
        }
    }
    else
    {
        mnCreditsInitTextSpritesAtPage(mode - CREDITS_MODE_FIRST_PAGE);
        while (DisplayScrollBusy <= CREDITS_TICKS_AFTER_END)
            sySynch(1);
        mnCreditsStopTextSprites();
    }

done:
    syIOWrite16(0, &REG_BLDCNT);
    syIOWrite16(DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON, &REG_DISPCNT);
    syWindowLoadGraphics();
    syPrintClear();
    OamCopyEnabled = 1;
    return 0;
}

// A tick-pass event added by mnCreditsInitCrawlSprites, as The Broken Seal's
// mnCreditsUpdateTextSprites: maps the visible sixteen rows of the story crawl onto the tile ring
// and uploads their sprites. While a slide waits, A or B releases the next one
// (CreditsPendingSlides); the crawl advances a pixel every fourth tick, and every tick but those
// while B is held.
void mnCreditsUpdateCrawlSprites(void)
{
    Uint32 fine = DisplayScrollPosition & 7;
    Uint32 tile =
        (((Sint16)DisplayScrollPosition / 8) & (CREDITS_RING_ROWS - 1)) * CREDITS_LINE_TILES;
    Uint32 *words = (Uint32 *)(DisplayScrollObjects + CREDITS_BORDER_SPRITES);
    Sint32  row;

    for (row = 0; row < CREDITS_VISIBLE_ROWS; row++)
    {
        Sint32 column;

        for (column = 0; column < CREDITS_COLUMNS; column++)
        {
            *words++ = (row * 8 + CREDITS_TEXT_TOP - fine) | OBJ_X(column * 32 + CREDITS_TEXT_LEFT)
                     | OBJ_SIZE_32X8;
            *words++  = tile;
            tile     += 4;
            if (tile == CREDITS_RING_TILES)
                tile = 0;
        }
    }
    syDMAXCopy(DisplayScrollObjects, (void *)OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);

    if (DisplayScrollBusy == CRAWL_STOP_FOR_SLIDE && (PerWork.data & (KEY_A | KEY_B)))
    {
        DisplayScrollBusy = CRAWL_SCROLLING;
        CreditsPendingSlides++;
    }
#if CRAWL_PAUSE_WITHOUT_SLIDE
    // In every edition but the Japanese A or B also goes on past a "----" line, without a slide.
    if (DisplayScrollBusy == CRAWL_STOP_LONG && (PerWork.data & (KEY_A | KEY_B)))
        DisplayScrollBusy = CRAWL_SCROLLING;
#endif
    if (DisplayScrollBusy == CRAWL_SCROLLING)
    {
        if ((FrameCount & 3) == 0)
            DisplayScrollPosition++;
        if ((PerWork.data & KEY_B) && (FrameCount & 3))
            DisplayScrollPosition++;
    }
}

// A VBlank event added by mnCreditsInitCrawlSprites: draws each line of the story crawl
// (MsgIntroCrawl) as it enters, two scroll lines a message.
void mnCreditsDrawEnteringCrawlLine(void)
{
    Sint32 first = (Sint32)MsgIntroCrawl;
    Uint32 current;
    Sint32 line;

    if (DisplayScrollBusy == CRAWL_SCROLLING)
    {
        current = DisplayScrollPosition;
        if ((Sint16)DisplayScrollPosition / CRAWL_MESSAGE_PIXELS
            != DisplayScrollLastPosition / CRAWL_MESSAGE_PIXELS)
        {
            DisplayScrollLastPosition = current;
            line                      = (Sint16)((Sint16)DisplayScrollPosition / 8);
            DisplayScrollBusy         = mnDisplayScrollDrawMessageLine(
                line / 2 + first - 1, CREDITS_RING_LINE(line), CREDITS_ALIGN_CENTER);
        }
    }
}

// Allocates the story crawl's sprites, builds its border and text rows, and adds
// mnCreditsUpdateCrawlSprites and mnCreditsDrawEnteringCrawlLine, as The Broken Seal's
// CreditsInitTextSprites starts its credits.
void mnCreditsInitCrawlSprites(void)
{
    volatile Uint32 fill;
    Uint32         *entry;
    Uint32          i;
    Uint32          j;

    void  *objects = syHeapAllocateEWRAM(CREDITS_OAM_ENTRIES * sizeof(DISPLAY_SCROLL_OBJECT));
    void **cell    = (void **)&DisplayScrollObjects;
    *cell          = objects;
    fill           = 0;
    syDMAXCopy((void *)&fill, (void *)OBJ_VRAM0, DMA_FILL_WORDS(CREDITS_RING_BYTES / 4), REG_DMA3);
    fill = PIXELS_4BPP(1);
    syDMAXCopy((void *)&fill,
               (void *)(OBJ_VRAM0 + CREDITS_RING_BYTES),
               DMA_FILL_WORDS(CREDITS_BORDER_TILE_BYTES / 4),
               REG_DMA3);

    entry = *(Uint32 **)cell;
    {
        Uint32 attribute = OBJ_SIZE_32X16;
        for (i = 0; i < 8; i++)
        {
            Uint32 *q  = entry;

            *q++       = OBJ_X(i * 32) | attribute;
            *q         = CREDITS_RING_TILES;
            entry     += 2;
        }
    }
    {
        Uint32 attribute = 136 | OBJ_SIZE_32X16;
        for (i = 0; i < 8; i++)
        {
            Uint32 *q  = entry;

            *q++       = OBJ_X(i * 32) | attribute;
            *q         = CREDITS_RING_TILES;
            entry     += 2;
        }
    }
    {
        Uint32 attribute = 152 | OBJ_SIZE_32X8;
        for (i = 0; i < 8; i++)
        {
            Uint32 *q  = entry;

            *q++       = OBJ_X(i * 32) | attribute;
            *q         = CREDITS_RING_TILES;
            entry     += 2;
        }
    }
    for (i = 0; i < CREDITS_VISIBLE_ROWS; i++)
    {
        for (j = 0; j < CREDITS_COLUMNS; j++)
        {
            Uint32 *q    = entry;
            Uint32  tile = (i * 3) * 8 + j * 4;

            *q++   = (i * 8 + CREDITS_TEXT_TOP) | OBJ_X(j * 32 + CREDITS_TEXT_LEFT) | OBJ_SIZE_32X8;
            *q     = tile;
            entry += 2;
        }
    }
    for (i = 0; i < 8; i++)
    {
        Uint32 *q  = entry;

        *q++       = OBJ_X(CREDITS_PARKED) | CREDITS_PARKED;
        *q         = CREDITS_RING_TILES;
        entry     += 2;
    }
    DisplayScrollPosition     = 0;
    DisplayScrollLastPosition = 0;
    DisplayScrollBusy         = 0;
    sySetEvent(mnCreditsUpdateCrawlSprites, EVENT_PASS_VBLANK);
    sySetEvent(mnCreditsDrawEnteringCrawlLine, EVENT_PASS_TICK);
    for (i = 0; i < CREDITS_RING_ROWS; i++)
        mnCreditsDrawLine(DisplayScrollLineTable[0], i * CREDITS_LINE_TILES, CREDITS_ALIGN_CENTER);
}

// Draws one line of the credits' scrolling text: the message is drawn into a canvas of one byte a
// pixel, packed to two pixels a byte, and copied into object tiles from slot on, its top half then
// its bottom half.
Sint32 mnDisplayScrollDrawMessageLine(Sint32 msg, Sint32 slot, Sint32 align)
{
    // LEFTOVER(frame): an unused local the ROM's frame reserves and never
    // * touches.
    Uint32  unused[8];
    Uint8  *canvas;
    Uint8  *src;
    Uint8  *dest;
    Uint32 *tiles;
    Uint32 *top;
    Sint32  busy;
    Sint32  i;
    Sint32  j;
    Sint32  row;
    Sint32  offset;

    canvas = syHeapAllocateEWRAM(CREDITS_BUFFER_BYTES);
    // BUG: the clear, and the packing loop's reads below, cover 3,072 bytes of a 2,304-byte
    // allocation.
    syWordsClear(canvas, 3072);
    busy = mnTextDrawMessageToCanvasStub(msg, canvas, align);
    dest = canvas;
    src  = canvas;
    for (j = 0; j < 2 * CREDITS_LINE_BYTES; j++)
    {
        Uint8 low = *src++;
        *dest++   = low | (*src++ << 4);
    }
    for (i = 0; i < CREDITS_LINE_TILES; i++)
    {
        Uint32 *column = (Uint32 *)canvas + i;

        tiles          = (Uint32 *)((slot + i) * TILE_SIZE_4BPP + OBJ_VRAM0);
        for (row = 0; row < 8; row++)
        {
            *tiles++ = column[row * CREDITS_LINE_TILES];
        }
    }
    for (offset = slot * TILE_SIZE_4BPP, top = (Uint32 *)(canvas + CREDITS_LINE_BYTES), j = 0;
         j < CREDITS_LINE_TILES;
         j++, offset += TILE_SIZE_4BPP, top++)
    {
        tiles = (Uint32 *)(offset + OBJ_VRAM0 + CREDITS_LINE_BYTES);
        for (row = 0; row < 8; row++)
        {
            *tiles++ = top[row * CREDITS_LINE_TILES];
        }
    }
    syHeapRelease(canvas);
    return busy;
}

// A VBlank event added by CreditsInitTextSprites: maps the visible sixteen rows onto the tile ring
// and uploads their sprites.
void mnCreditsUpdateTextSprites(void)
{
    Uint32  frame = DisplayScrollPosition;
    Uint32  fine  = frame & 7;
    Sint32  tile  = (((Sint16)frame / 8) & (CREDITS_RING_ROWS - 1)) * CREDITS_LINE_TILES;
    Uint32 *entry = (Uint32 *)(DisplayScrollObjects + CREDITS_BORDER_SPRITES);
    Sint32  row;

    for (row = 0; row < CREDITS_VISIBLE_ROWS; row++)
    {
        Sint32 color;

        for (color = 0; color < CREDITS_COLUMNS; color++)
        {
            Uint32 *q = entry;

            *q++      = (row * 8 + CREDITS_TEXT_TOP - fine) | OBJ_X(color * 32 + CREDITS_TEXT_LEFT)
                 | OBJ_SIZE_32X8;
            entry += 2;
            *q     = tile;
            tile  += 4;
            if (tile == CREDITS_RING_TILES)
                tile = 0;
        }
    }
    syDMAXCopy(DisplayScrollObjects, OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
    if (FrameCount % 3 == 0)
        DisplayScrollPosition++;
}

// An event added by mnCreditsInitTextSpritesAtPage: draws each line entering the credits sixteen
// rows ahead of the visible top. A page mark stops the scroll, and blank rows follow it while
// DisplayScrollBusy counts the ticks since.
void mnCreditsDrawEnteringLine(void)
{
    Sint32       pos;
    Uint32       current;
    Sint32       line;
    const Uint8 *text;

    pos     = (Sint16)DisplayScrollPosition;
    current = DisplayScrollPosition;
    line    = pos / 8;
    if (DisplayScrollBusy != 0)
    {
        DisplayScrollBusy++;
        mnCreditsDrawLine(
            DisplayScrollLineTable[0], CREDITS_RING_LINE(pos / 8), CREDITS_ALIGN_CENTER);
    }
    else if (pos / 8 != DisplayScrollLastPosition / 8)
    {
        DisplayScrollLastPosition = current;
        text                      = DisplayScrollLineTable[line];
        if (text == CREDITS_PAGE_MARK)
        {
            DisplayScrollBusy = 1;
            mnCreditsDrawLine(
                DisplayScrollLineTable[0], CREDITS_RING_LINE(line), CREDITS_ALIGN_CENTER);
        }
        else
        {
            DisplayScrollBusy =
                mnCreditsDrawLine(text, CREDITS_RING_LINE(line), CREDITS_ALIGN_CENTER);
        }
    }
}

// Starts the scrolling credits at a page, as mnCreditsInitCrawlSprites starts them at the top:
// counts the line table's 255 page marks up to the page asked for (at most CREDITS_MAX_LINES lines,
// stopping at the end) and sets the scroll there, then builds the sprite table and its tiles, adds
// mnCreditsUpdateTextSprites and mnCreditsDrawEnteringLine to the event table and draws the first
// 32 lines blank.
void mnCreditsInitTextSpritesAtPage(Sint32 page)
{
    volatile Uint32 fill;
    Sint32          marks;
    Uint32         *entry;
    Uint32          i;
    Uint32          j;

    marks = 0;
    i     = 0;
    if (page != 0 && DisplayScrollLineTable[0] != 0)
    {
        do
        {
            if (DisplayScrollLineTable[i] == CREDITS_PAGE_MARK)
                marks++;
            i++;
            if (i == CREDITS_MAX_LINES || marks == page)
                break;
        } while (DisplayScrollLineTable[i] != 0);
    }
    DisplayScrollPosition = i * 8;

    {
        void  *objects = syHeapAllocateEWRAM(CREDITS_OAM_ENTRIES * sizeof(DISPLAY_SCROLL_OBJECT));
        void **cell    = (void **)&DisplayScrollObjects;

        *cell          = objects;
        fill           = 0;
        syDMAXCopy(
            (void *)&fill, (void *)OBJ_VRAM0, DMA_FILL_WORDS(CREDITS_RING_BYTES / 4), REG_DMA3);
        fill = PIXELS_4BPP(1);
        syDMAXCopy((void *)&fill,
                   (void *)(OBJ_VRAM0 + CREDITS_RING_BYTES),
                   DMA_FILL_WORDS(CREDITS_BORDER_TILE_BYTES / 4),
                   REG_DMA3);
        entry = *(Uint32 **)cell;
    }
    {
        Uint32 attribute = OBJ_SIZE_32X16;

        for (i = 0; i < 8; i++)
        {
            Uint32 *q  = entry;

            *q++       = OBJ_X(i * 32) | attribute;
            *q         = CREDITS_RING_TILES;
            entry     += 2;
        }
    }
    {
        Uint32 attribute = 136 | OBJ_SIZE_32X16;

        for (i = 0; i < 8; i++)
        {
            Uint32 *q  = entry;

            *q++       = OBJ_X(i * 32) | attribute;
            *q         = CREDITS_RING_TILES;
            entry     += 2;
        }
    }
    {
        Uint32 attribute = 152 | OBJ_SIZE_32X8;

        for (i = 0; i < 8; i++)
        {
            Uint32 *q  = entry;

            *q++       = OBJ_X(i * 32) | attribute;
            *q         = CREDITS_RING_TILES;
            entry     += 2;
        }
    }
    for (i = 0; i < CREDITS_VISIBLE_ROWS; i++)
    {
        for (j = 0; j < CREDITS_COLUMNS; j++)
        {
            Uint32 *q    = entry;
            Uint32  tile = (i * 3) * 8 + j * 4;

            *q++   = (i * 8 + CREDITS_TEXT_TOP) | OBJ_X(j * 32 + CREDITS_TEXT_LEFT) | OBJ_SIZE_32X8;
            *q     = tile;
            entry += 2;
        }
    }
    for (i = 0; i < 8; i++)
    {
        Uint32 *q  = entry;

        *q++       = OBJ_X(CREDITS_PARKED) | CREDITS_PARKED;
        *q         = CREDITS_RING_TILES;
        entry     += 2;
    }
    DisplayScrollLastPosition = 0;
    DisplayScrollBusy         = 0;
    sySetEvent(mnCreditsUpdateTextSprites, EVENT_PASS_VBLANK);
    sySetEvent(mnCreditsDrawEnteringLine, EVENT_PASS_TICK);
    for (i = 0; i < CREDITS_RING_ROWS; i++)
        mnCreditsDrawLine(DisplayScrollLineTable[0], i * CREDITS_LINE_TILES, CREDITS_ALIGN_CENTER);
}

// Ends the scrolling credits: blanks the tile ring, removes their two events and clears the
// sprites.
void mnCreditsStopTextSprites(void)
{
    Sint32  i;
    Uint32 *objects;

    for (i = 0; i < CREDITS_RING_ROWS; i++)
        mnCreditsDrawLine(DisplayScrollLineTable[0], i * CREDITS_LINE_TILES, CREDITS_ALIGN_CENTER);
    syCloseEvent(mnCreditsUpdateTextSprites);
    syCloseEvent(mnCreditsDrawEnteringLine);
    objects = (Uint32 *)DisplayScrollObjects;
    for (i = 0; i != CREDITS_OAM_ENTRIES * 2; i++)
        *objects++ = 0;
    syDMAXCopy(DisplayScrollObjects, OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
}

// A credits tile's row of 4 bytes in object VRAM.
#define OBJ_TILE_ROW(tile, row) (*(Uint32 *)(OBJ_VRAM0 + (row) * 4 + (tile) * TILE_SIZE_4BPP))

// Centers, right-aligns or left-aligns one credits line in a 192-pixel row, with its shadow, and
// writes it as tiles from `firstTile`. Returns -1 at the end of the text.
Sint32 mnCreditsDrawLine(const Uint8 *text, Sint32 firstTile, Sint32 alignment)
{
    Uint8        *linePixels = (Uint8 *)syHeapAllocateEWRAM(CREDITS_BUFFER_BYTES);
    Sint32        startX     = 0;
    Sint32        lineWidth  = CREDITS_LINE_PIXELS;
    const Uint8  *font       = DisplayScrollFont;
    Sint32        tileCount;
    Sint32        textAdvance;
    const Uint8  *srcBytes;
    Uint8        *dest;
    Uint32        srcValue;
    Sint32        glyphIndex;
    Sint32        glyphWidth;
    Sint32        row;
    Sint32        columnIndex;
    Uint32        glyphBits;
    Uint32        glyphMask;
    // LEFTOVER(frame): an unused local the ROM's frame reserves and never
    // * touches.
    UNUSED Uint32 unused[8];

    if (text == NULL)
    {
        // BUG: the end marker is checked after allocation, leaking one line buffer. syHeapRelease
        // rewinds this bump pool to the allocation's start.
#if FIX_CREDITS_LINE_LEAK
        if (linePixels != NULL)
            syHeapRelease(linePixels);
#endif
        return -1;
    }
    // The ninth raster row holds the shadow below this line.
    if (!syGameFlagGet(FLAG_CREDITS_LINE_DRAWN))
    {
        syWordsFillEntry(linePixels, CREDITS_BUFFER_BYTES, 0);
        syGameFlagSet(FLAG_CREDITS_LINE_DRAWN);
    }
    else
    {
        syWordsCopy(linePixels, linePixels + 8 * CREDITS_RASTER_STRIDE, CREDITS_RASTER_STRIDE);
        syWordsFillEntry(linePixels + CREDITS_RASTER_STRIDE, 8 * CREDITS_RASTER_STRIDE, 0);
    }

    // Measure the line in the credits font, then choose where it starts. srcBytes walks the text
    // here, and each glyph's rows below.
    srcBytes    = text;
    textAdvance = 0;
    while ((srcValue = *srcBytes++) != 0)
    {
        if (srcValue >= ' ')
            textAdvance += CreditsGlyphWidths[srcValue - ' '];
    }
    if (alignment == CREDITS_ALIGN_RIGHT)
        startX = lineWidth - textAdvance;
    else if (alignment == CREDITS_ALIGN_CENTER)
        startX = (lineWidth - textAdvance) / 2;

    // Rasterize each glyph and its one-pixel shadow at the aligned start. FAKEMATCH(dead): srcBytes
    // = text, overwritten before it is read; this
    // * pass walks text itself; as no store.
    srcBytes    = text;
    textAdvance = 0;
    while ((srcValue = *text++) != 0)
    {
        if (srcValue >= ' ')
        {
            glyphIndex = srcValue - ' ';
            srcBytes   = font + glyphIndex * 8;
            dest       = linePixels + startX + textAdvance;
            for (row = 0; row < 8; row++)
            {
                glyphBits = *srcBytes++;
                glyphMask = CREDITS_GLYPH_LEFT_PIXEL_BIT;
                for (columnIndex = 0; columnIndex < 8; columnIndex++)
                {
                    if (glyphBits & glyphMask)
                    {
                        dest[CREDITS_RASTER_STRIDE + 1] = CREDITS_SHADOW_COLOR;
                        dest[0]                         = CREDITS_TEXT_COLOR;
                    }
                    dest++;
                    glyphMask >>= 1;
                }
                dest += CREDITS_RASTER_STRIDE - 8;
            }
            glyphWidth = 1;
            if (srcValue >= ' ')
                glyphWidth = CreditsGlyphWidths[glyphIndex];
            textAdvance += glyphWidth;
        }
    }

    // Pack pairs of pixel indices into bytes, keeping the 256-pixel stride.
    tileCount = lineWidth / 8;
    srcBytes  = linePixels;
    dest      = linePixels;
    for (row = 0; row < 8; row++)
    {
        for (columnIndex = 0; columnIndex < tileCount * 4; columnIndex++)
        {
            srcValue  = *srcBytes++;
            srcValue |= *srcBytes++ << 4;
            *dest++   = srcValue;
        }
        dest     += CREDITS_RASTER_STRIDE - tileCount * 4;
        srcBytes += CREDITS_RASTER_STRIDE - tileCount * 8;
    }

    // Transpose raster rows into the tile ring used by the scrolling sprites.
    for (row = 0; row < tileCount; row++)
    {
        Sint32  tileIndex          = firstTile + row;
        Uint32 *words              = (Uint32 *)linePixels + row;

        // A tile's eight rows of 4 bytes, each a 256-pixel raster row apart.
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

// Unused: an empty function nothing calls.
UNUSED void mnCreditsReservedNoOperation(void)
{
}

// Sets the row the ending's scrolling image moves to, in pixels from its top, at most
// EndingImageSpeed a tick.
void mnEndingImageSetTargetRow(Uint16 row)
{
    EndingImageTargetRow = row;
}

// Copies 15 columns of a scrolling image's map into a wrapping 2 KB tilemap by DMA, 31 rows of 64
// bytes each, from the row the position selects, and records the position in the three scroll
// words. The scrolling image scene's callback (mnEndingImageScroll) and its image loader
// (mnEndingImageShow, through its inline copy) call it.
void mnEndingImageCopyMap(const Uint8 *src, Uint8 *dest, Sint32 pos)
{
    Uint32 start;
    Sint32 column;
    Sint32 row;
    Uint32 offset;
    Uint32 from;

    start                = ENDING_IMAGE_ROW_OFFSET(EndingImageMapScroll);
    EndingImageTargetRow = EndingImageRow = pos;
    EndingImageScroll                     = toFIXED(pos);
    for (column = 0; column < ENDING_IMAGE_COLUMNS; column++)
    {
        from   = (pos / 8) * ENDING_IMAGE_ROW_BYTES;
        offset = ENDING_IMAGE_WRAP(start);
        for (row = 0; row < ENDING_IMAGE_ROWS; row++)
        {
            syDMAXCopy(
                src + from, dest + offset, DMA_COPY_WORDS(ENDING_IMAGE_ROW_BYTES / 4), REG_DMA3);
            from   += ENDING_IMAGE_ROW_BYTES;
            offset  = ENDING_IMAGE_WRAP(offset + ENDING_IMAGE_ROW_BYTES);
        }
        src  += ENDING_IMAGE_STRIP_BYTES;
        dest += ENDING_IMAGE_SCREEN_BYTES;
    }
}

// The ending image's VBlank event: fades the new image in over 64 frames, moves the row toward
// EndingImageTargetRow by at most EndingImageSpeed, eases the 16.16 scroll after it, and copies the
// map strip that scrolls into view.
void mnEndingImageScroll(void)
{
    Sint32 old = EndingImageScroll;
    Sint32 d;
    Sint32 scroll;
    Uint8 *src;
    Uint8 *dest;
    Uint32 start;
    Uint32 from;
    Uint32 offset;
    Sint32 i;

    if (EndingImageFadeFrames < ENDING_IMAGE_FADE_FRAMES)
    {
        Sint32 t = ++EndingImageFadeFrames / 4;
        Sint32 alpha;

        if (EndingImageBlock & 1)
        {
            alpha        = ((BLEND_WEIGHT_MAX - t) << 8) | t;
            REG_BLDALPHA = alpha;
        }
        else
        {
            alpha        = (t << 8) | (BLEND_WEIGHT_MAX - t);
            REG_BLDALPHA = alpha;
        }
    }
    if (EndingImageTargetRow != EndingImageRow)
    {
        d = EndingImageTargetRow - EndingImageRow;
        if (ABS(d) > EndingImageSpeed)
        {
            if (d > 0)
                d = EndingImageSpeed;
            else
                d = -EndingImageSpeed;
        }
        EndingImageRow += d;
    }
    {
        // FAKEMATCH(temporary): the scroll read through a pointer to it.
        volatile Sint32 *word   = &EndingImageScroll;
        FIXED            target = toFIXED(EndingImageRow);

        scroll                  = *word;
        if (scroll != target)
        {
            d = target - scroll;
            if (ABS(d) > toFIXED(EndingImageSpeed) / 4)
                d /= 8;
            if (ABS(d) > toFIXED(EndingImageSpeed))
            {
                if (d > 0)
                    d = toFIXED(EndingImageSpeed);
                else
                    d = -toFIXED(EndingImageSpeed);
            }
            EndingImageScroll    += d;
            EndingImageMapScroll += d;
        }
    }
    if ((EndingImageScroll ^ old) & toFIXED(8))
    {
        src   = EndingImageMap;
        dest  = (Uint8 *)BG_CHAR_BLOCK(0);
        start = ENDING_IMAGE_ROW_OFFSET(EndingImageMapScroll);
        if (EndingImageBlock != 0)
            dest = (Uint8 *)BG_CHAR_BLOCK(2);
        scroll = EndingImageScroll;
        if (scroll > old)
        {
            from = ENDING_IMAGE_ROW_OFFSET((Uint32)scroll)
                 + (ENDING_IMAGE_ROWS - 1) * ENDING_IMAGE_ROW_BYTES;
            offset = ENDING_IMAGE_WRAP(start + (ENDING_IMAGE_ROWS - 1) * ENDING_IMAGE_ROW_BYTES);
            for (i = 0; i < ENDING_IMAGE_COLUMNS; i++)
            {
                syDMAXCopy(src + from,
                           dest + offset,
                           DMA_COPY_WORDS(ENDING_IMAGE_ROW_BYTES / 4),
                           REG_DMA3);
                src  += ENDING_IMAGE_STRIP_BYTES;
                dest += ENDING_IMAGE_SCREEN_BYTES;
            }
        }
        else
        {
            from   = ENDING_IMAGE_ROW_OFFSET((Uint32)EndingImageScroll);
            offset = ENDING_IMAGE_WRAP(start);
            for (i = 0; i < ENDING_IMAGE_COLUMNS; i++)
            {
                syDMAXCopy(src + from,
                           dest + offset,
                           DMA_COPY_WORDS(ENDING_IMAGE_ROW_BYTES / 4),
                           REG_DMA3);
                src  += ENDING_IMAGE_STRIP_BYTES;
                dest += ENDING_IMAGE_SCREEN_BYTES;
            }
        }
    }
    // d, done with the step, takes the map's scroll in pixels.
    d                 = FIXEDtoINT(EndingImageMapScroll);
    BgScrollWork[2].x = d;
    BgScrollWork[3].x = d;
}

// Starts the ending's images (the epilogue scene calls it first): resets the event table and the
// scroll, adds mnEndingImageScroll, sets up BG2, BG3 and blending, and builds the border strips.
void mnEndingImageStart(void)
{
    CpuLoadDisplayEnabled = 0;
    DebugTextEnabled      = 0;
    IdleSleepEnabled      = 0;
    syInitEvent();
    EndingImageMapScroll = 0;
    EndingImageTargetRow = EndingImageRow = EndingImageScroll = 0;
    EndingImageMap                                            = NULL;
    EndingImageSpeed                                          = 1;
    EndingImageBlock                                          = 0;
    sySetEvent(mnEndingImageScroll, EVENT_PASS_TICK);
    syIOWrite16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
    mnCreditsBuildBackgroundTilemap((Uint32 *)BG_SCREEN_BLOCK(CREDITS_BG3_SCREEN_BLOCK));
    mnCreditsBuildBackgroundTilemap((Uint32 *)BG_SCREEN_BLOCK(CREDITS_BG2_SCREEN_BLOCK));
    mnCreditsClearBuffer(0);
    mnCreditsClearBuffer(1);
    syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_256_COLORS
                    | BGCNT_SCREEN_BASE(CREDITS_BG2_SCREEN_BLOCK),
                &REG_BG2CNT);
    syIOWrite16(BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(CREDITS_BG3_SCREEN_BLOCK),
                &REG_BG3CNT);
    syIOWrite16(DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON,
                &REG_DISPCNT);
    syIOWrite16(BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_BD,
                &REG_BLDCNT);
    syIOWrite16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);
    mnEndingImageInitStrips();
}

// mnEndingImageCopyMap's body as mnEndingImageShow inlines it: the same copy, written with each
// column's strip and tilemap found from its number.
static inline void CopyScrollMap(const Uint8 *src, Uint8 *dest, Sint32 pos)
{
    Uint32 start;
    Sint32 column;
    Sint32 row;
    Uint32 offset;
    Uint32 from;

    start                = ENDING_IMAGE_ROW_OFFSET(EndingImageMapScroll);
    EndingImageTargetRow = EndingImageRow = pos;
    EndingImageScroll                     = toFIXED(pos);
    for (column = 0; column < ENDING_IMAGE_COLUMNS; column++)
    {
        Uint8       *to    = dest + column * ENDING_IMAGE_SCREEN_BYTES;
        const Uint8 *strip = src + column * ENDING_IMAGE_STRIP_BYTES;
        from               = (pos / 8) * ENDING_IMAGE_ROW_BYTES;
        offset             = ENDING_IMAGE_WRAP(start);
        for (row = 0; row < ENDING_IMAGE_ROWS; row++)
        {
            syDMAXCopy(
                strip + from, to + offset, DMA_COPY_WORDS(ENDING_IMAGE_ROW_BYTES / 4), REG_DMA3);
            from   += ENDING_IMAGE_ROW_BYTES;
            offset  = ENDING_IMAGE_WRAP(offset + ENDING_IMAGE_ROW_BYTES);
        }
    }
}

// Shows a scrolling image on the hidden one of the two BG character blocks. With no resource it
// fills the map buffer with a plain tile word and clears the palette half; otherwise it clears the
// block's first 2 KB, expands resource's map into the buffer with EndingImageDecodeCode copied to
// the heap (the fill word marks the second block's tiles), and queues the resource's 128-colour
// palette. Then it copies the map at position into the tilemap (CopyScrollMap), flips the block and
// restarts the fade-in counter.
void mnEndingImageShow(Sint32 resource, Uint8 *map, Sint32 pos)
{
    volatile Uint32 cell;
    Uint32          word;
    Uint32          fill = 0;
    Uint8          *tiles;
    Uint16         *palette;
    Uint8          *data;
    Sint32          (*code)(Uint8 *, Uint8 *, Uint32);

    if (resource == 0)
    {
        if (EndingImageBlock != 0)
        {
            word    = PIXELS_8BPP(1);
            tiles   = (Uint8 *)BG_CHAR_BLOCK(0);
            palette = (Uint16 *)BG_PLTT;
        }
        else
        {
            word    = PIXELS_8BPP(129);
            tiles   = (Uint8 *)BG_CHAR_BLOCK(2);
            palette = (Uint16 *)&BG_PLTT_COLOR(8, 0);
        }
        DMA_FILL32_CELL(cell, word, map, ENDING_IMAGE_MAP_BYTES);
        DMA_FILL32_CELL(cell, 0, palette, BACKDROP_PALETTE_BYTES);
    }
    else
    {
        if (EndingImageBlock != 0)
        {
            tiles   = (Uint8 *)BG_CHAR_BLOCK(0);
            palette = (Uint16 *)BG_PLTT;
        }
        else
        {
            fill    = PIXELS_8BPP(128);
            tiles   = (Uint8 *)BG_CHAR_BLOCK(2);
            palette = (Uint16 *)&BG_PLTT_COLOR(8, 0);
        }
        DMA_FILL32_CELL(cell, 0, tiles, ENDING_IMAGE_SCREEN_BYTES);
        data = syResourceGetTableEntry(resource);
        RAM_CODE_LOAD(code, EndingImageDecodeCode, EndingImageDecodeCodeSize);
        code(data + BACKDROP_PALETTE_BYTES, map, fill);
        syHeapRelease(code);
        IO_WRITE_QUEUE_PUSH(
            (Uint32)palette, (Uint32)data, DMA_COPY_WORDS(BACKDROP_PALETTE_BYTES / 4));
    }
    EndingImageMap = map;
    CopyScrollMap(map, tiles, pos);
    EndingImageBlock      ^= 1;
    EndingImageFadeFrames  = 0;
}

// Waits, for at most 300 ticks, until the scroll reaches its row.
void mnEndingImageWaitForScroll(void)
{
    Sint32 ticks = 0;

    if (EndingImageScroll != toFIXED(EndingImageRow))
    {
        do
        {
            sySynch(1);
            ticks++;
            if (ticks >= ENDING_IMAGE_SCROLL_TIMEOUT)
                break;
        } while (EndingImageScroll != toFIXED(EndingImageRow));
    }
}

// A VBlank event added by mnEndingImageInitStrips: queues the 24 border strip sprites.
void mnEndingImageDrawStrips(void)
{
    QUEUED_SPR *entry = EndingImageStrips;
    Sint32      i;

    for (i = 0; i < ENDING_IMAGE_STRIP_SPRITES; i++)
        syOAMAddSpriteAtDepthClamped(entry++, 0);
}

// Allocates the 24 sprite entries of the scrolling image's border strips, fills their tiles with
// colour 1 and adds mnEndingImageDrawStrips to draw them; the entries are written as words, as The
// Broken Seal's mnCreditsInitCrawlSprites writes its strips.
void mnEndingImageInitStrips(void)
{
    Sint32  slot = syVRAMCacheFindFreeEntry();
    Uint32 *words;
    Sint32  tile;
    Uint32  i;

    EndingImageStrips = syHeapAllocateEWRAM(ENDING_IMAGE_STRIP_SPRITES * sizeof(QUEUED_SPR));
    tile              = syVRAMCacheLoad(slot, 512, 0);
    DMA_FILL32(PIXELS_4BPP(1), (void *)(OBJ_VRAM0 + tile * TILE_SIZE_4BPP), 512);
    words = (Uint32 *)EndingImageStrips;
    for (i = 0; i < 8; i++)
    {
        *words++ = 0;
        *words++ = OBJ_X(i * 32) | OBJ_SIZE_32X16 | 120;
        *words++ = tile;
    }
    for (i = 0; i < 8; i++)
    {
        *words++ = 0;
        *words++ = OBJ_X(i * 32) | OBJ_SIZE_32X16 | 136;
        *words++ = tile | OBJ_PRIORITY(2);
    }
    for (i = 0; i < 8; i++)
    {
        *words++ = 0;
        *words++ = OBJ_X(i * 32) | OBJ_SIZE_32X8 | 152;
        *words++ = tile | OBJ_PRIORITY(2);
    }
    sySetEvent(mnEndingImageDrawStrips, EVENT_PASS_TICK);
}

// Moves the top strip's eight sprites to y 120 when enable is set, and to y 0 otherwise.
void mnEndingImageLowerTopStrip(Sint32 enable)
{
    QUEUED_SPR *entry = EndingImageStrips;
    Uint32      i;

    if (!enable)
    {
        for (i = 0; i < 8; i++)
        {
            entry->oam.y = 0;
            entry++;
        }
    }
    else
    {
        for (i = 0; i < 8; i++)
        {
            entry->oam.y = 120;
            entry++;
        }
    }
}

// clang-format off
// Unused: the slideshow backdrops, ending in 0.
const Uint32 DisplayScrollUnusedSlideResources[] = {
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

// The backdrops the slideshow fades between, in turn; 0 ends the list, and the table has room for 25.
const Uint32 DisplayScrollSlideResources[25] = {
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

// An empty credits line, between the names.
static const Uint8 BlankCreditLine[] = "";

#if (LANGUAGE == LANGUAGE_GERMAN)
#define CREDITS_SUBTITLE "DIE VERGESSENE EPOCHE"
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define CREDITS_SUBTITLE "LA EDAD PERDIDA"
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define CREDITS_SUBTITLE "L'\177GE PERDU" // L'ÂGE PERDU: the French font draws Â at 0x7f
#elif (LANGUAGE == LANGUAGE_ITALIAN)
#define CREDITS_SUBTITLE "L'ERA PERDUTA"
#endif

// The Spanish, French and Italian fonts draw í at '`'.
#if (LANGUAGE == LANGUAGE_GERMAN)
#define RODRIGUEZ "Rodriguez"
#else
#define RODRIGUEZ "Rodr`guez"
#endif

// The staff credits, one line each, ending in 0.
const void *DisplayScrollLineTable[] = {
    BlankCreditLine,
#if (LANGUAGE == LANGUAGE_JAPANESE)
    "OUGON NO TAIYO STAFF",
    BlankCreditLine,
#elif (LANGUAGE == LANGUAGE_ENGLISH)
    "GOLDEN SUN: THE LOST AGE",
    "STAFF",
#else
    "GOLDEN SUN",
    CREDITS_SUBTITLE,
    "STAFF",
#endif
    BlankCreditLine,
    "Game Planning",
    BlankCreditLine,
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    BlankCreditLine,
    BlankCreditLine,
    "Scenario",
    BlankCreditLine,
    "Hiroyuki Takahashi",
    BlankCreditLine,
    BlankCreditLine,
    "Director",
    BlankCreditLine,
    "Shugo Takahashi",
    BlankCreditLine,
    BlankCreditLine,
    "Assistant Director",
    BlankCreditLine,
    "Yusuke Sugimoto",
    BlankCreditLine,
    BlankCreditLine,
    "Game Design",
    BlankCreditLine,
    "Yusuke Sugimoto",
    "Shuji Shimizu",
    "Ayumu Shindo",
    "Kentaro Sakou",
    "Norisumi Osawa",
    "Masato Kawamura",
    CREDITS_PAGE_MARK,
    "Programming",
    BlankCreditLine,
    BlankCreditLine,
    "System ",
    BlankCreditLine,
    "Yasuhiro Taguchi",
    BlankCreditLine,
    BlankCreditLine,
    "Battle",
    BlankCreditLine,
    "Haruki Kodera",
    "Toru Takamatsu",
    BlankCreditLine,
    BlankCreditLine,
    "Field",
    BlankCreditLine,
    "Kaoru Shimada",
    "Hiroyuki Ichikawa",
    "Akiko Sato",
    "Kenji Numaya",
#if CREDITS_REVISED_FOR_LOCALIZATION
    "P.E. Jareth Hein",
#else
    "P.E.Jareth Hein",
#endif
    BlankCreditLine,
    BlankCreditLine,
    "User Interface",
    BlankCreditLine,
    "Yutaka Yamamoto",
    CREDITS_PAGE_MARK,
#if CREDITS_REVISED_FOR_LOCALIZATION
    "Graphic Art",
#else
    "Graphic Arts",
#endif
    BlankCreditLine,
    BlankCreditLine,
    "Battle Characters",
    BlankCreditLine,
    "Junko Nakamura",
    "Kaori Matsunaga",
    BlankCreditLine,
    BlankCreditLine,
    "Battle Effects",
    "& Faces",
    BlankCreditLine,
    "Junichi Ochiai",
    BlankCreditLine,
    BlankCreditLine,
    "Battle Backgrounds",
    BlankCreditLine,
    "Hiroto Nakashima",
    BlankCreditLine,
    BlankCreditLine,
    "Maps",
    BlankCreditLine,
    "Masayuki Hashimoto",
    "Mitsumasa Muraishi",
    "Mitsuru Fukumoto",
    "Akihito Kimura",
    "Shinichiro Sugimoto",
    "Natsuko Takeuchi",
    BlankCreditLine,
    BlankCreditLine,
    "Map Characters",
    BlankCreditLine,
    "Mieko Koguchi",
    "Kanako Horiguchi",
    BlankCreditLine,
    BlankCreditLine,
    "Graphics Direction",
    BlankCreditLine,
    "Fumihide Aoki",
    CREDITS_PAGE_MARK,
    "Character Design",
    BlankCreditLine,
    "Shin Yamanouchi",
    BlankCreditLine,
    BlankCreditLine,
    "Monster Design",
    BlankCreditLine,
    "Hiroshi Kajiyama",
    BlankCreditLine,
    BlankCreditLine,
    "Music",
    BlankCreditLine,
    "Motoi Sakuraba",
    BlankCreditLine,
    BlankCreditLine,
    "Sound Effects",
    BlankCreditLine,
    "Hiroyuki Hamada",
    BlankCreditLine,
    BlankCreditLine,
    "Sound Direction",
    BlankCreditLine,
    "Masaaki Uno",
    BlankCreditLine,
    BlankCreditLine,
    "Staff Coordination",
    BlankCreditLine,
    "Akiko Sato",
    "Kenji Numaya",
    CREDITS_PAGE_MARK,
    "Illustration",
    BlankCreditLine,
    "Shin Yamanouchi",
    "Hiroshi Kajiyama",
    "Masanori Sato",
    BlankCreditLine,
    BlankCreditLine,
    "Graphic Support",
    BlankCreditLine,
    "Yoichi Kotabe",
    "Masanori Sato",
    BlankCreditLine,
    BlankCreditLine,
#if !CREDITS_REVISED_FOR_LOCALIZATION
    "Artwork Design",
    BlankCreditLine,
    "Nanako Kinoshita",
    "Takashi Maeda",
    "Naoko Okamoto",
    "Shinji Yamamoto",
    BlankCreditLine,
    BlankCreditLine,
    "Public Relations",
    BlankCreditLine,
    "Mitsuaki Hagishima",
    "Asako Haruhana",
    BlankCreditLine,
    BlankCreditLine,
#endif
    "Coordination",
    BlankCreditLine,
    "Masaaki Uno",
    "Toshiharu Izuno",
    "Kenichi Nishida",
    BlankCreditLine,
    BlankCreditLine,
#if !CREDITS_REVISED_FOR_LOCALIZATION
    "Special Thanks to",
    "Super Mario Club Staff",
    BlankCreditLine,
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
    BlankCreditLine,
    "Translation",
    BlankCreditLine,
    "William Barnes",
    "Reiko Ninomiya",
    "Tim O'Leary",
    "Scot Ritchey",
    "Bill Trinen",
    BlankCreditLine,
    "Writing",
    " & Editing",
    BlankCreditLine,
    "Richard Amtower",
    "Shawn Seavers",
    BlankCreditLine,
    "Localization",
    " Management",
    BlankCreditLine,
    "Leslie Swan",
    "Jeff Miller",
    BlankCreditLine,
    BlankCreditLine,
    "NOA Product Testing",
    "Michael Kelbaugh",
    "Eric Bush",
    "Todd Buechele",
    "Tim Casey",
    "Jeffrey Storbo",
    BlankCreditLine,
#if (LANGUAGE != LANGUAGE_ENGLISH)
    BlankCreditLine,
    "European Localization",
    BlankCreditLine,
    "Product Localization",
    "Team",
    BlankCreditLine,
    "Coordination",
    "Andy Fey",
    BlankCreditLine,
    "Translation",
    BlankCreditLine,
    // Each edition names its own translator first.
#if (LANGUAGE == LANGUAGE_GERMAN)
    "German",
    "Micky Auer",
    BlankCreditLine,
    "Spanish",
    "Carlos Montilla",
    RODRIGUEZ,
    BlankCreditLine,
    "French",
    "Zadia Messerli",
    BlankCreditLine,
    "Italian",
    "Christian Massi",
    BlankCreditLine,
#elif (LANGUAGE == LANGUAGE_SPANISH)
    "Spanish",
    "Carlos Montilla",
    RODRIGUEZ,
    BlankCreditLine,
    "German",
    "Micky Auer",
    BlankCreditLine,
    "French",
    "Zadia Messerli",
    BlankCreditLine,
    "Italian",
    "Christian Massi",
    BlankCreditLine,
#elif (LANGUAGE == LANGUAGE_FRENCH)
    "French",
    "Zadia Messerli",
    BlankCreditLine,
    "German",
    "Micky Auer",
    BlankCreditLine,
    "Spanish",
    "Carlos Montilla",
    RODRIGUEZ,
    BlankCreditLine,
    "Italian",
    "Christian Massi",
    BlankCreditLine,
#else
    "Italian",
    "Christian Massi",
    BlankCreditLine,
    "German",
    "Micky Auer",
    BlankCreditLine,
    "French",
    "Zadia Messerli",
    BlankCreditLine,
    "Spanish",
    "Carlos Montilla",
    RODRIGUEZ,
    BlankCreditLine,
#endif
    "Validation",
    "& Testing Team",
#endif
#endif
    CREDITS_PAGE_MARK,
    "Produced by",
    BlankCreditLine,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    BlankCreditLine,
    BlankCreditLine,
    "Executive Producer",
    BlankCreditLine,
    "Hiroshi Yamauchi",
    BlankCreditLine,
    BlankCreditLine,
#if CREDITS_REVISED_FOR_LOCALIZATION
    "Copyrights of Game, Scenario,",
#else
    "Copyrights of Game, Scenario",
#endif
    " and Program reserved by",
    BlankCreditLine,
    "NINTENDO",
    "and",
    "CAMELOT",
    BlankCreditLine,
    "Copyrights of All Characters",
    "and Music reserved by",
    BlankCreditLine,
    "NINTENDO",
    BlankCreditLine,
    CREDITS_PAGE_MARK,
    0
};
// clang-format on

#include "../graphics/credits/glyph_widths.h"

/// @file
/// The staff credits: the battle backdrops behind them, the lines that scroll
/// past, and the scrolling pictures with their border strips.
///
/// One object, as The Broken Seal's credits.c is, with its slides and lines at
/// its end; the background decoders linked before it are assembly
/// (credits/credits_background_*.s).

#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "edition.h"
#include "version.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/ram_code.h"
#include "system/heap_release.h"
#include "graphics/resource.h"
#include "system/frame.h"
#include "system/input.h"
#include "system/task.h"
#include "system/bg_scroll.h"
#include "menu/display_scroll.h"
#include "graphics/render_input.h"
#include "text/msg_ids.h"

struct QueuedSprite;

typedef s32 (*PackedDecoder)(const u8 *source, void *destination, u32 fill);

/// One eight-byte OAM entry in the credits' sprite buffer.
struct DisplayScrollObject {
  u32 attr01;
  u32 attr2;
};

extern s16 gCredits_PendingSlides;
extern struct DisplayScrollObject *gDisplayScroll_Objects;
extern const u32 gDisplayScroll_SlideResources[];
extern const void *gDisplayScroll_LineTable[];

/// The scrolling picture's state. EndingPicture_Scroll, a VBlank task, moves
/// the scroll toward its target while EndingPicture_WaitForScroll waits for it
/// to arrive.
///@{
extern s16 gEndingPicture_Block; ///< which character block the picture is in
extern s16 gEndingPicture_FadeFrames; ///< frames of the fade-in, to 64
extern u32 gEndingPicture_MapScroll;  ///< the tilemap's scroll, 16.16 pixels
extern volatile s32 gEndingPicture_Scroll; ///< the scroll, 16.16 pixels
extern s16 gEndingPicture_Row;   ///< the row the scroll is heading for
extern s16 gEndingPicture_Speed; ///< the most rows a frame moves
extern u8 *gEndingPicture_Map;   ///< the picture's map
/// The 24 sprites of the picture's border strips.
extern struct QueuedSprite *gEndingPicture_Strips;
///@}

void Credits_InitCrawlSprites(void);
void Credits_InitTextSpritesAtPage(s32 credits);
void Credits_StopTextSprites(void);
s32 Credits_DrawLine(const u8 *text, s32 firstTile, s32 alignment);
extern const u8 BattleEffect_ExpandCode[];
extern u8 BattleEffect_ExpandCodeSize[];
extern const u8 gCredits_GlyphWidths[];
extern const u8 gDisplayScroll_Font[];

void EndingPicture_InitStrips(void);
extern const u8 BattleEffect_Expand2Code[];
extern u8 BattleEffect_Expand2CodeSize[];

#include "gamepak/far_call/window.h"
#include "system/ram_buffer.h"
#include "fixed_math.h"
#include "window/window.h"
#include "flags.h"
#include "resource_ids.h"
#include "field/flag_call.h"

extern u8 gDebugTextEnabled;

// Credits text uses six 32-pixel sprites across each 192-pixel line, drawn into
// a ring of 32 lines of sprite tiles.
#define CREDITS_LINE_PIXELS 192
#define CREDITS_VISIBLE_ROWS 16
#define CREDITS_COLUMNS 6
#define CREDITS_RING_ROWS 32
#define CREDITS_LINE_TILES 24 ///< A line's 4bpp tiles, 192 / 8.
#define CREDITS_RING_TILES (CREDITS_RING_ROWS * CREDITS_LINE_TILES)
#define CREDITS_RING_BYTES (CREDITS_RING_TILES * TILE_SIZE_4BPP)
/// A line's raster: nine rows of 256 one-byte pixels, the ninth for the
/// shadow below the eighth.
#define CREDITS_BUFFER_BYTES 2304
#define CREDITS_RASTER_STRIDE 256
/// OAM entries 0 to 23: the opaque strips above and below the text.
#define CREDITS_BORDER_SPRITES 24
/// The whole OAM: the borders, 96 text sprites and 8 parked off screen.
#define CREDITS_OAM_ENTRIES 128
/// The border sprites' one 32x16 picture, solid in colour 1, after the ring.
#define CREDITS_BORDER_TILE_BYTES (8 * TILE_SIZE_4BPP)
/// One ring line's tiles: the top or bottom half of a drawn line.
#define CREDITS_LINE_BYTES (CREDITS_LINE_TILES * TILE_SIZE_4BPP)
/// The line table's page mark, how far Credits_InitTextSpritesAtPage counts
/// for one, and the frames the credits run on after their end mark.
#define CREDITS_PAGE_MARK ((const u8 *)255)
#define CREDITS_MAX_LINES 500
#define CREDITS_FRAMES_AFTER_END 375
/// The ring line a scroll line is drawn into, sixteen rows below the top.
#define CREDITS_RING_LINE(line)                                                \
  ((((line) + CREDITS_VISIBLE_ROWS) & (CREDITS_RING_ROWS - 1)) *               \
   CREDITS_LINE_TILES)
/// The scene flag that says a credits line was drawn before, so the next one
/// starts from its shadow row.
#define FLAG_CREDITS_LINE_DRAWN FLAG_SCENE_TEMP_00

// The ending's scrolling pictures: a map buffer of 15 columns, each a strip of
// 4 KB copied into its own 2 KB screen block, 31 rows of 64 bytes at a time.
#define ENDING_PICTURE_COLUMNS 15
#define ENDING_PICTURE_STRIP_BYTES 4096
#define ENDING_PICTURE_SCREEN_BYTES BG_SCREEN_SIZE
#define ENDING_PICTURE_ROWS 31
#define ENDING_PICTURE_ROW_BYTES 64
/// A row's offset wrapped into its screen block.
#define ENDING_PICTURE_WRAP(offset)                                            \
  ((offset) & (ENDING_PICTURE_SCREEN_BYTES - 1))
/// The fade-in's frames, four to each blend step.
#define ENDING_PICTURE_FADE_FRAMES (BLEND_WEIGHT_MAX * 4)
/// The map offset of the row of eight pixels a 16.16 scroll stands at.
#define ENDING_PICTURE_ROW_OFFSET(scroll)                                      \
  (((scroll) >> 19) * ENDING_PICTURE_ROW_BYTES)
/// The map buffer: every column's strip.
#define ENDING_PICTURE_MAP_BYTES                                               \
  (ENDING_PICTURE_COLUMNS * ENDING_PICTURE_STRIP_BYTES)
/// EndingPicture_WaitForScroll gives up after five seconds.
#define ENDING_PICTURE_SCROLL_TIMEOUT (5 * FRAMES_PER_SECOND)
/// The picture's border strips: eight sprites across at y 120, 136 and 152.
#define ENDING_PICTURE_STRIP_SPRITES 24

/// A backdrop or picture resource begins with its 128 colours, the palette
/// half it takes; its packed tiles follow.
#define BACKDROP_COLORS 128
#define BACKDROP_PALETTE_BYTES (BACKDROP_COLORS * 2)
/// A battle backdrop's 480 tiles of 256 colours, 256 by 120 pixels.
#define BACKDROP_TILE_BYTES (480 * 64)

/// Fills 30K bytes of 256-colour characters with colour 1 and clears 128
/// palette colours.
/// @param alternate 0 selects the first half (VRAM 0x06000000, palette colours
/// 0-127);
///      1 selects the second (VRAM 0x06008000, colours 128-255, fill colour
///      129).
void Credits_ClearBuffer(s32 alternate) {
  u32 fillColor;
  u32 tileDestination;
  u32 paletteDestination;
  volatile u32 dmaFill;

  // Use a solid pixel in the palette being cleared, producing an opaque
  // black layer for the next crossfade.
  if (alternate == 0) {
    fillColor = PIXELS_8BPP(1);
    tileDestination = (u32)BG_CHAR_BLOCK(0);
    paletteDestination = (u32)BG_PLTT;
  } else {
    fillColor = PIXELS_8BPP(129);
    tileDestination = (u32)BG_CHAR_BLOCK(2);
    paletteDestination = (u32)&BG_PLTT_COLOR(8, 0);
  }
  Dma_Fill32Cell(dmaFill, fillColor, (void *)tileDestination,
                 BACKDROP_TILE_BYTES);
  Dma_Fill32Cell(dmaFill, 0, (void *)paletteDestination,
                 BACKDROP_PALETTE_BYTES);
}

/* VIEW(credits): variable gWaitForFlashWrite as u16 gWaitForFlashWrite[];
 * lucky_wheels_lzpack.h has struct LzPackWork *gWaitForFlashWrite; tried as the
 * rest of the object declares it, it does not compile in tla-ja:
 * games/tla/src/menu/credits.c:162: `gWaitForFlashWrite' undeclared (first use
 * in this function). */
extern u16 gWaitForFlashWrite__credits[] __asm__("gWaitForFlashWrite");

/// Loads a backdrop resource (its 128 colours, then its packed tiles) into
/// one half of BG VRAM and queues its palette for the next VBlank, as The
/// Broken Seal does, but greyed: each channel
/// becomes (2 red + 4 green + blue + 5 * channel) / 16. A zero resource
/// clears that half instead. The tiles are unpacked by the
/// BattleEffect_Expand kernel, copied by DMA into a heap block for the call;
/// the grey colours are built in the EWRAM buffer at gWaitForFlashWrite.
void Credits_LoadBackground(u32 resourceId, s32 alternate) {
  const u8 *resourceData;
  u32 paletteDestination;
  u32 paletteOffset;
  void *tileDestination;
  PackedDecoder decoder;
  s32 i;

  if (resourceId == 0) {
    Credits_ClearBuffer(alternate);
    return;
  }
  resourceData = Resource_GetTableEntry(resourceId);
  if (alternate == 0) {
    paletteOffset = 0;
    tileDestination = (void *)BG_CHAR_BLOCK(0);
    paletteDestination = (u32)BG_PLTT;
  } else {
    paletteOffset = PIXELS_8BPP(128);
    tileDestination = (void *)BG_CHAR_BLOCK(2);
    paletteDestination = (u32)&BG_PLTT_COLOR(8, 0);
  }
  RamCode_Load(decoder, BattleEffect_ExpandCode, BattleEffect_ExpandCodeSize);
  decoder(resourceData + BACKDROP_PALETTE_BYTES, tileDestination,
          paletteOffset);
  Heap_Release(decoder);

  for (i = 0; i < BACKDROP_COLORS; i++) {
    u16 color = *(const s16 *)resourceData;
    s32 blue = RGB_BLUE(color);
    s32 green = RGB_GREEN(color);
    s32 red = RGB_RED(color);
    s32 sum = (green * 2 + red) * 2 + blue;

    resourceData += 2;
    blue = (sum + blue * 5) / 16;
    green = (sum + green * 5) / 16;
    red = (sum + red * 5) / 16;
    gWaitForFlashWrite__credits[i] = blue << 10 | green << 5 | red;
  }
  IO_WRITE_QUEUE_PUSH(paletteDestination, (u32)gWaitForFlashWrite__credits,
                      DMA_COPY_WORDS(BACKDROP_PALETTE_BYTES / 4));
}

/// Builds the 512 tilemap words surrounding a sliding credits picture, as The
/// Broken Seal does: blank margins, 240 consecutive tile pairs, then a
/// zero-filled tail.
void Credits_BuildBackgroundTilemap(u32 *destination) {
  s32 i;
  u32 blankTiles;
  u32 tilePair;

  blankTiles = 0x01FF01FF;
  tilePair = 0x10000;
  // Pad above and below the picture so vertical wrapping stays blank.
  for (i = 0; i < 32; i++)
    *destination++ = blankTiles;
  for (i = 0; i < 240; i++) {
    *destination++ = tilePair;
    tilePair += 0x20002;
  }
  for (i = 0; i < 48; i++)
    *destination++ = blankTiles;
  for (i = 0; i < 192; i++)
    *destination++ = 0;
}

/// A VBlank task of the credits: drifts both pictures together, keeping their
/// horizontal offsets aligned throughout the crossfade.
void Credits_ScrollBackgrounds(void) {
  if ((gVBlankCount & 3) == 0) {
    u32 decrement = 0xffff;

    gBgScroll[2].x += decrement;
    gBgScroll[3].x += decrement;
  }
}

/// The credits and slide show driver. It resets the task table, starts the
/// scroll task and sets up BG2, BG3 and blending; mode 1 cross-fades the slides
/// until START or the scroll ends, other modes start the scrolling credits
/// (mode - 2) and wait for their end.
s32 Credits_Run(s32 mode) {
  s32 slide;
  s32 step;
  s32 frame;

  gOamCopyEnabled = 0;
  gCpuLoadDisplayEnabled = 0;
  gDebugTextEnabled = 0;
  gCredits_PendingSlides = 0;
  gIdleSleepEnabled = 0;
  Task_ResetAll();
  Task_Add(Credits_ScrollBackgrounds, TASK_PASS_FRAME);
  Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
  Credits_BuildBackgroundTilemap((u32 *)BG_SCREEN_BLOCK(15));
  Credits_BuildBackgroundTilemap((u32 *)BG_SCREEN_BLOCK(31));
  Credits_ClearBuffer(0);
  Credits_ClearBuffer(1);
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(31),
             &REG_BG2CNT);
  Io_Write16(BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(15),
             &REG_BG3CNT);
  Io_Write16(DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                 DISPCNT_OBJ_ON,
             &REG_DISPCNT);
  Io_Write16(BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3 |
                 BLDCNT_TGT2_BD,
             &REG_BLDCNT);

  if (mode == 1) {

    Credits_InitCrawlSprites();
    wait(60);
    for (slide = 0;; slide++) {
      s16 *count = &gCredits_PendingSlides;

      Credits_LoadBackground(gDisplayScroll_SlideResources[slide],
                             (slide & 1) ^ 1);
      for (step = 1; step <= BLEND_WEIGHT_MAX; step++) {
        if (slide & 1) {
          /* FAKEMATCH(temporary): the weights kept in locals; as one
           * expression they load in another order. */
          s32 t = BLEND_WEIGHT_MAX;
          s32 hi = step << 8;
          s32 r = t - step;
          REG_BLDALPHA = hi | r;
        } else
          REG_BLDALPHA = ((BLEND_WEIGHT_MAX - step) << 8) | step;
        for (frame = 0; frame <= 3; frame++) {
          if (gInput.held & KEY_START)
            goto done;
          wait(1);
        }
      }
      if (*count == 0) {
        do {
          gInput.pressed;
          if (gInput.held & KEY_START)
            goto done;
          if (gDisplayScroll_Busy == 2)
            goto done;
          wait(1);
        } while (gCredits_PendingSlides == 0);
      }
      gCredits_PendingSlides--;
    }
  } else {
    Credits_InitTextSpritesAtPage(mode - 2);
    while (gDisplayScroll_Busy <= CREDITS_FRAMES_AFTER_END)
      wait(1);
    Credits_StopTextSprites();
  }

done:
  Io_Write16(0, &REG_BLDCNT);
  Io_Write16(DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON, &REG_DISPCNT);
  Window_LoadGraphics();
  DebugText_Clear();
  gOamCopyEnabled = 1;
  return 0;
}

/// A frame-pass task added by Credits_InitCrawlSprites, as The Broken Seal's
/// Credits_UpdateTextSprites: maps the visible sixteen rows of the story crawl
/// onto the tile ring and uploads their sprites. While a slide waits, A or B
/// releases the next one (gCredits_PendingSlides); the crawl advances a pixel
/// every fourth frame, and every frame but those while B is held.
void Credits_UpdateCrawlSprites(void) {
  u32 fine = gDisplayScroll_Position & 7;
  u32 tile = (((s16)gDisplayScroll_Position / 8) & (CREDITS_RING_ROWS - 1)) *
             CREDITS_LINE_TILES;
  u32 *words = (u32 *)(gDisplayScroll_Objects + CREDITS_BORDER_SPRITES);
  s32 row;

  for (row = 0; row < CREDITS_VISIBLE_ROWS; row++) {
    s32 column;

    for (column = 0; column < CREDITS_COLUMNS; column++) {
      *words++ =
          (row * 8 + 16 - fine) | OBJ_X(column * 32 + 24) | OBJ_SIZE_32X8;
      *words++ = tile;
      tile += 4;
      if (tile == CREDITS_RING_TILES)
        tile = 0;
    }
  }
  Dma_Set(gDisplayScroll_Objects, (void *)OAM, DMA_COPY_WORDS(OAM_SIZE / 4),
          REG_DMA3);

  if (gDisplayScroll_Busy == 1 && (gInput.held & (KEY_A | KEY_B))) {
    gDisplayScroll_Busy = 0;
    gCredits_PendingSlides++;
  }
#if TLA_PATCH_1
  // From patch 1 a held button also releases state 3; the Japanese release
  // checks only state 1.
  if (gDisplayScroll_Busy == 3 && (gInput.held & (KEY_A | KEY_B)))
    gDisplayScroll_Busy = 0;
#endif
  if (gDisplayScroll_Busy == 0) {
    if ((gVBlankCount & 3) == 0)
      gDisplayScroll_Position++;
    if ((gInput.held & KEY_B) && (gVBlankCount & 3))
      gDisplayScroll_Position++;
  }
}

/// A VBlank task added by Credits_InitCrawlSprites: draws each line of the
/// story crawl (MsgIntroCrawl) as it enters, two scroll lines a message.
void Credits_DrawEnteringCrawlLine(void) {
  s32 first = (s32)MsgIntroCrawl;
  u32 current;
  s32 line;

  if (gDisplayScroll_Busy == 0) {
    current = gDisplayScroll_Position;
    if ((s16)gDisplayScroll_Position / 16 != gDisplayScroll_LastPosition / 16) {
      gDisplayScroll_LastPosition = current;
      line = (s16)((s16)gDisplayScroll_Position / 8);
      gDisplayScroll_Busy = DisplayScroll_DrawMessageLine(
          line / 2 + first - 1, CREDITS_RING_LINE(line), 1);
    }
  }
}

/// Allocates the story crawl's sprites, builds its border and text rows, and
/// adds Credits_UpdateCrawlSprites and Credits_DrawEnteringCrawlLine, as The
/// Broken Seal's Credits_InitTextSprites starts its credits.
void Credits_InitCrawlSprites(void) {
  volatile u32 fill;
  u32 *entry;
  u32 i;
  u32 j;

  void *objects =
      Heap_AllocEwram(CREDITS_OAM_ENTRIES * sizeof(struct DisplayScrollObject));
  void **cell = (void **)&gDisplayScroll_Objects;
  *cell = objects;
  fill = 0;
  Dma_Set((void *)&fill, (void *)OBJ_VRAM0,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
              (CREDITS_RING_BYTES / 4),
          REG_DMA3);
  fill = PIXELS_4BPP(1);
  Dma_Set((void *)&fill, (void *)(OBJ_VRAM0 + CREDITS_RING_BYTES),
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
              (CREDITS_BORDER_TILE_BYTES / 4),
          REG_DMA3);

  entry = *(u32 **)cell;
  {
    u32 attr = OBJ_SIZE_32X16;
    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = CREDITS_RING_TILES;
      entry += 2;
    }
  }
  {
    u32 attr = 136 | OBJ_SIZE_32X16;
    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = CREDITS_RING_TILES;
      entry += 2;
    }
  }
  {
    u32 attr = 152 | OBJ_SIZE_32X8;
    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = CREDITS_RING_TILES;
      entry += 2;
    }
  }
  for (i = 0; i < CREDITS_VISIBLE_ROWS; i++) {
    for (j = 0; j < CREDITS_COLUMNS; j++) {
      u32 *q = entry;
      u32 tile = (i * 3) * 8 + j * 4;

      *q++ = (i * 8 + 16) | OBJ_X(j * 32 + 24) | OBJ_SIZE_32X8;
      *q = tile;
      entry += 2;
    }
  }
  for (i = 0; i < 8; i++) {
    u32 *q = entry;

    *q++ = OBJ_X(192) | 192;
    *q = CREDITS_RING_TILES;
    entry += 2;
  }
  gDisplayScroll_Position = 0;
  gDisplayScroll_LastPosition = 0;
  gDisplayScroll_Busy = 0;
  Task_Add(Credits_UpdateCrawlSprites, TASK_PASS_FRAME);
  Task_Add(Credits_DrawEnteringCrawlLine, TASK_PASS_VBLANK);
  for (i = 0; i < CREDITS_RING_ROWS; i++)
    Credits_DrawLine(gDisplayScroll_LineTable[0], i * CREDITS_LINE_TILES, 1);
}

/// Draws one line of the credits' scrolling text: the message is drawn into a
/// canvas of one byte a pixel, packed to two pixels a byte, and copied into
/// object tiles from slot on, its top half then its bottom half.
s32 DisplayScroll_DrawMessageLine(s32 message, s32 slot, s32 align) {
  // Unused: gives the stack frame its 32 bytes.
  u32 unused[8];
  u8 *canvas;
  u8 *src;
  u8 *dst;
  u32 *tiles;
  u32 *top;
  s32 busy;
  s32 i;
  s32 j;
  s32 row;
  s32 offset;

  canvas = Heap_AllocEwram(CREDITS_BUFFER_BYTES);
  // BUG: the clear, and the packing loop's reads below, cover 0xc00 bytes of
  // a 0x900-byte allocation.
  Iwram_ClearWords(canvas, 0xc00);
  busy = Text_DrawMessageToCanvasFar(message, canvas, align);
  dst = canvas;
  src = canvas;
  for (j = 0; j < 2 * CREDITS_LINE_BYTES; j++) {
    u8 low = *src++;
    *dst++ = low | (*src++ << 4);
  }
  for (i = 0; i < CREDITS_LINE_TILES; i++) {
    u32 *column = (u32 *)canvas + i;

    tiles = (u32 *)((slot + i) * TILE_SIZE_4BPP + OBJ_VRAM0);
    for (row = 0; row < 8; row++) {
      *tiles++ = column[row * CREDITS_LINE_TILES];
    }
  }
  for (offset = slot * TILE_SIZE_4BPP,
      top = (u32 *)(canvas + CREDITS_LINE_BYTES), j = 0;
       j < CREDITS_LINE_TILES; j++, offset += TILE_SIZE_4BPP, top++) {
    tiles = (u32 *)(offset + OBJ_VRAM0 + CREDITS_LINE_BYTES);
    for (row = 0; row < 8; row++) {
      *tiles++ = top[row * CREDITS_LINE_TILES];
    }
  }
  Heap_Release(canvas);
  return busy;
}

/// A VBlank task added by Credits_InitTextSprites: maps the visible sixteen
/// rows onto the tile ring and uploads their sprites.
void Credits_UpdateTextSprites(void) {
  u32 frame = gDisplayScroll_Position;
  u32 fine = frame & 7;
  s32 tile = (((s16)frame / 8) & (CREDITS_RING_ROWS - 1)) * CREDITS_LINE_TILES;
  u32 *entry = (u32 *)(gDisplayScroll_Objects + CREDITS_BORDER_SPRITES);
  s32 row;

  for (row = 0; row < CREDITS_VISIBLE_ROWS; row++) {
    s32 col;

    for (col = 0; col < CREDITS_COLUMNS; col++) {
      u32 *q = entry;

      *q++ = (row * 8 + 16 - fine) | OBJ_X(col * 32 + 24) | OBJ_SIZE_32X8;
      entry += 2;
      *q = tile;
      tile += 4;
      if (tile == CREDITS_RING_TILES)
        tile = 0;
    }
  }
  Dma_Set(gDisplayScroll_Objects, OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
  if (gVBlankCount % 3 == 0)
    gDisplayScroll_Position++;
}

/// A task added by Credits_InitTextSpritesAtPage: draws each line entering the
/// credits sixteen rows ahead of the visible top. A line table value of 255
/// ends the text, and blank rows follow it.
void Credits_DrawEnteringLine(void) {
  s32 position;
  u32 current;
  s32 line;
  const u8 *text;

  position = (s16)gDisplayScroll_Position;
  current = gDisplayScroll_Position;
  line = position / 8;
  if (gDisplayScroll_Busy != 0) {
    gDisplayScroll_Busy++;
    Credits_DrawLine(gDisplayScroll_LineTable[0],
                     CREDITS_RING_LINE(position / 8), 1);
  } else if (position / 8 != gDisplayScroll_LastPosition / 8) {
    gDisplayScroll_LastPosition = current;
    text = gDisplayScroll_LineTable[line];
    if (text == CREDITS_PAGE_MARK) {
      gDisplayScroll_Busy = 1;
      Credits_DrawLine(gDisplayScroll_LineTable[0], CREDITS_RING_LINE(line), 1);
    } else {
      gDisplayScroll_Busy = Credits_DrawLine(text, CREDITS_RING_LINE(line), 1);
    }
  }
}

/// Starts the scrolling credits at a page, as Credits_InitCrawlSprites starts
/// them at the top: counts the line table's 255 page marks up to the page asked
/// for (at most 500 lines, stopping at the end mark 0) and sets the scroll
/// there, then builds the sprite table and its tiles, adds
/// Credits_UpdateTextSprites and Credits_DrawEnteringLine to the task table and
/// draws the first 32 lines blank.
void Credits_InitTextSpritesAtPage(s32 page) {
  volatile u32 fill;
  s32 marks;
  u32 *entry;
  u32 i;
  u32 j;

  marks = 0;
  i = 0;
  if (page != 0 && gDisplayScroll_LineTable[0] != 0) {
    do {
      if (gDisplayScroll_LineTable[i] == CREDITS_PAGE_MARK)
        marks++;
      i++;
      if (i == CREDITS_MAX_LINES || marks == page)
        break;
    } while (gDisplayScroll_LineTable[i] != 0);
  }
  gDisplayScroll_Position = i * 8;

  {
    void *objects = Heap_AllocEwram(CREDITS_OAM_ENTRIES *
                                    sizeof(struct DisplayScrollObject));
    void **cell = (void **)&gDisplayScroll_Objects;

    *cell = objects;
    fill = 0;
    Dma_Set((void *)&fill, (void *)OBJ_VRAM0,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
                (CREDITS_RING_BYTES / 4),
            REG_DMA3);
    fill = PIXELS_4BPP(1);
    Dma_Set((void *)&fill, (void *)(OBJ_VRAM0 + CREDITS_RING_BYTES),
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
                (CREDITS_BORDER_TILE_BYTES / 4),
            REG_DMA3);
    entry = *(u32 **)cell;
  }
  {
    u32 attr = OBJ_SIZE_32X16;

    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = CREDITS_RING_TILES;
      entry += 2;
    }
  }
  {
    u32 attr = 136 | OBJ_SIZE_32X16;

    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = CREDITS_RING_TILES;
      entry += 2;
    }
  }
  {
    u32 attr = 152 | OBJ_SIZE_32X8;

    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = CREDITS_RING_TILES;
      entry += 2;
    }
  }
  for (i = 0; i < CREDITS_VISIBLE_ROWS; i++) {
    for (j = 0; j < CREDITS_COLUMNS; j++) {
      u32 *q = entry;
      u32 tile = (i * 3) * 8 + j * 4;

      *q++ = (i * 8 + 16) | OBJ_X(j * 32 + 24) | OBJ_SIZE_32X8;
      *q = tile;
      entry += 2;
    }
  }
  for (i = 0; i < 8; i++) {
    u32 *q = entry;

    *q++ = OBJ_X(192) | 192;
    *q = CREDITS_RING_TILES;
    entry += 2;
  }
  gDisplayScroll_LastPosition = 0;
  gDisplayScroll_Busy = 0;
  Task_Add(Credits_UpdateTextSprites, TASK_PASS_FRAME);
  Task_Add(Credits_DrawEnteringLine, TASK_PASS_VBLANK);
  for (i = 0; i < CREDITS_RING_ROWS; i++)
    Credits_DrawLine(gDisplayScroll_LineTable[0], i * CREDITS_LINE_TILES, 1);
}

/// Ends the scrolling credits: blanks the tile ring, removes their two tasks
/// and clears the sprites.
void Credits_StopTextSprites(void) {
  s32 i;
  u32 *objects;

  for (i = 0; i < CREDITS_RING_ROWS; i++)
    Credits_DrawLine(gDisplayScroll_LineTable[0], i * CREDITS_LINE_TILES, 1);
  Task_Remove(Credits_UpdateTextSprites);
  Task_Remove(Credits_DrawEnteringLine);
  objects = (u32 *)gDisplayScroll_Objects;
  for (i = 0; i != CREDITS_OAM_ENTRIES * 2; i++)
    *objects++ = 0;
  Dma_Set(gDisplayScroll_Objects, OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
}

/// A credits tile's row of 4 bytes in object VRAM.
#define OBJ_TILE_ROW(tile, row)                                                \
  (*(u32 *)(OBJ_VRAM0 + (row) * 4 + (tile) * TILE_SIZE_4BPP))

/// Centers, right-aligns or left-aligns one credits line in a 192-pixel row,
/// with its shadow, and writes it as tiles from `firstTile`. Returns -1 at the
/// end of the text.
s32 Credits_DrawLine(const u8 *text, s32 firstTile, s32 alignment) {
  u8 *linePixels = (u8 *)Heap_AllocEwram(CREDITS_BUFFER_BYTES);
  s32 startX = 0;
  s32 lineWidth = CREDITS_LINE_PIXELS;
  const u8 *font = gDisplayScroll_Font;
  s32 tileCount;
  s32 textAdvance;
  const u8 *sourceBytes;
  u8 *destination;
  u32 sourceValue;
  s32 glyphIndex;
  s32 glyphWidth;
  s32 row;
  s32 columnIndex;
  u32 glyphBits;
  u32 glyphMask;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED u32 unused[8];

  // BUG: the end marker is checked after allocation, leaking one line buffer.
  if (text == NULL)
    return -1;
  // The ninth raster row holds the shadow below this line.
  if (!GameFlag_Get(FLAG_CREDITS_LINE_DRAWN)) {
    Iwram_FillWordsEntry(linePixels, CREDITS_BUFFER_BYTES, 0);
    GameFlag_Set(FLAG_CREDITS_LINE_DRAWN);
  } else {
    Iwram_CopyWords(linePixels, linePixels + 8 * CREDITS_RASTER_STRIDE,
                    CREDITS_RASTER_STRIDE);
    Iwram_FillWordsEntry(linePixels + CREDITS_RASTER_STRIDE,
                         8 * CREDITS_RASTER_STRIDE, 0);
  }

  // Measure the line in the credits font, then choose where it starts.
  // sourceBytes walks the text here, and each glyph's rows below.
  sourceBytes = text;
  textAdvance = 0;
  while ((sourceValue = *sourceBytes++) != 0) {
    if (sourceValue >= ' ')
      textAdvance += gCredits_GlyphWidths[sourceValue - ' '];
  }
  if (alignment == 2)
    startX = lineWidth - textAdvance;
  else if (alignment == 1)
    startX = (lineWidth - textAdvance) / 2;

  // Rasterize each glyph and its one-pixel shadow at the aligned start.
  sourceBytes = text;
  textAdvance = 0;
  while ((sourceValue = *text++) != 0) {
    if (sourceValue >= ' ') {
      glyphIndex = sourceValue - ' ';
      sourceBytes = font + glyphIndex * 8;
      destination = linePixels + startX + textAdvance;
      for (row = 0; row < 8; row++) {
        glyphBits = *sourceBytes++;
        glyphMask = 0x80;
        for (columnIndex = 7; columnIndex >= 0; columnIndex--) {
          if (glyphBits & glyphMask) {
            destination[CREDITS_RASTER_STRIDE + 1] = 1;
            destination[0] = 15;
          }
          destination++;
          glyphMask >>= 1;
        }
        destination += CREDITS_RASTER_STRIDE - 8;
      }
      glyphWidth = 1;
      if (sourceValue >= ' ')
        glyphWidth = gCredits_GlyphWidths[glyphIndex];
      textAdvance += glyphWidth;
    }
  }

  // Pack pairs of pixel indices into bytes, keeping the 256-pixel stride.
  tileCount = lineWidth / 8;
  sourceBytes = linePixels;
  destination = linePixels;
  for (row = 0; row < 8; row++) {
    for (columnIndex = 0; columnIndex < tileCount * 4; columnIndex++) {
      sourceValue = *sourceBytes++;
      sourceValue |= *sourceBytes++ << 4;
      *destination++ = sourceValue;
    }
    destination += CREDITS_RASTER_STRIDE - tileCount * 4;
    sourceBytes += CREDITS_RASTER_STRIDE - tileCount * 8;
  }

  // Transpose raster rows into the tile ring used by the scrolling sprites.
  for (row = 0; row < tileCount; row++) {
    s32 tileIndex = firstTile + row;
    u32 *words = (u32 *)linePixels + row;

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
  Heap_Release(linePixels);
  return 0;
}

/// Unused: an empty function nothing calls, after the pool.
UNUSED void Credits_ReservedNoOp(void) {}

/// Sets the position the credits' scroll moves to.
void SceneState_SetScrollTarget(u16 value) { gScrollTarget = value; }

/// Copies 15 columns of a scrolling picture's map into a wrapping 2 KB
/// tilemap by DMA, 31 rows of 64 bytes each, from the row the position
/// selects, and records the position in the three scroll words. The
/// scrolling picture scene's callback (EndingPicture_Scroll) and its picture
/// loader (EndingPicture_Show, through its inline copy) call it.
void EndingPicture_CopyMap(const u8 *source, u8 *destination, s32 position) {
  u32 start;
  s32 column;
  s32 row;
  u32 offset;
  u32 from;

  start = ENDING_PICTURE_ROW_OFFSET(gEndingPicture_MapScroll);
  gScrollTarget = gEndingPicture_Row = position;
  {
    volatile s32 *word = &gEndingPicture_Scroll;

    *word = position << 16;
  }
  for (column = 0; column < ENDING_PICTURE_COLUMNS; column++) {
    from = (position / 8) * ENDING_PICTURE_ROW_BYTES;
    offset = ENDING_PICTURE_WRAP(start);
    for (row = 0; row < ENDING_PICTURE_ROWS; row++) {
      Dma_Set(source + from, destination + offset,
              DMA_COPY_WORDS(ENDING_PICTURE_ROW_BYTES / 4), REG_DMA3);
      from += ENDING_PICTURE_ROW_BYTES;
      offset = ENDING_PICTURE_WRAP(offset + ENDING_PICTURE_ROW_BYTES);
    }
    source += ENDING_PICTURE_STRIP_BYTES;
    destination += ENDING_PICTURE_SCREEN_BYTES;
  }
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

/// The ending picture's VBlank task: fades the new picture in over 64
/// frames, moves the row toward gScrollTarget by at most gEndingPicture_Speed,
/// eases the 16.16 scroll after it, and copies the map strip that scrolls into
/// view.
void EndingPicture_Scroll(void) {
  s32 old = gEndingPicture_Scroll;
  s32 d;
  s32 scroll;
  u8 *source;
  u8 *destination;
  u32 start;
  u32 from;
  u32 offset;
  s32 i;

  if (gEndingPicture_FadeFrames < ENDING_PICTURE_FADE_FRAMES) {
    s32 t = (s16)++gEndingPicture_FadeFrames / 4;
    s32 alpha;

    if (gEndingPicture_Block & 1) {
      alpha = ((BLEND_WEIGHT_MAX - t) << 8) | t;
      REG_BLDALPHA = alpha;
    } else {
      alpha = (t << 8) | (BLEND_WEIGHT_MAX - t);
      REG_BLDALPHA = alpha;
    }
  }
  if (gScrollTarget != gEndingPicture_Row) {
    d = gScrollTarget - gEndingPicture_Row;
    if (ABS(d) > gEndingPicture_Speed) {
      if (d > 0)
        d = gEndingPicture_Speed;
      else
        d = -gEndingPicture_Speed;
    }
    gEndingPicture_Row += d;
  }
  {
    /* FAKEMATCH(temporary): the scroll read through a word pointer. */
    s32 *word = (s32 *)&gEndingPicture_Scroll;
    s32 target = gEndingPicture_Row << 16;

    scroll = *word;
    if (scroll != target) {
      d = target - scroll;
      if (ABS(d) > gEndingPicture_Speed << 14)
        d /= 8;
      if (ABS(d) > gEndingPicture_Speed << 16) {
        if (d > 0)
          d = gEndingPicture_Speed << 16;
        else
          d = -(gEndingPicture_Speed << 16);
      }
      gEndingPicture_Scroll += d;
      gEndingPicture_MapScroll += d;
    }
  }
  if ((gEndingPicture_Scroll ^ old) & px(8)) {
    source = gEndingPicture_Map;
    destination = (u8 *)BG_CHAR_BLOCK(0);
    start = ENDING_PICTURE_ROW_OFFSET(gEndingPicture_MapScroll);
    if (gEndingPicture_Block != 0)
      destination = (u8 *)BG_CHAR_BLOCK(2);
    scroll = gEndingPicture_Scroll;
    if (scroll > old) {
      from = ENDING_PICTURE_ROW_OFFSET((u32)scroll) +
             (ENDING_PICTURE_ROWS - 1) * ENDING_PICTURE_ROW_BYTES;
      offset = ENDING_PICTURE_WRAP(start + (ENDING_PICTURE_ROWS - 1) *
                                               ENDING_PICTURE_ROW_BYTES);
      for (i = 0; i < ENDING_PICTURE_COLUMNS; i++) {
        Dma_Set(source + from, destination + offset,
                DMA_COPY_WORDS(ENDING_PICTURE_ROW_BYTES / 4), REG_DMA3);
        source += ENDING_PICTURE_STRIP_BYTES;
        destination += ENDING_PICTURE_SCREEN_BYTES;
      }
    } else {
      from = ENDING_PICTURE_ROW_OFFSET((u32)gEndingPicture_Scroll);
      offset = ENDING_PICTURE_WRAP(start);
      for (i = 0; i < ENDING_PICTURE_COLUMNS; i++) {
        Dma_Set(source + from, destination + offset,
                DMA_COPY_WORDS(ENDING_PICTURE_ROW_BYTES / 4), REG_DMA3);
        source += ENDING_PICTURE_STRIP_BYTES;
        destination += ENDING_PICTURE_SCREEN_BYTES;
      }
    }
  }
  d = gEndingPicture_MapScroll >> 16;
  gBgScroll[2].x = d;
  gBgScroll[3].x = d;
}

/// Starts the ending's pictures (the epilogue scene calls it first): resets
/// the task table and the scroll, adds EndingPicture_Scroll, sets up BG2, BG3
/// and blending, and builds the border strips.
void EndingPicture_Start(void) {
  gCpuLoadDisplayEnabled = 0;
  gDebugTextEnabled = 0;
  gIdleSleepEnabled = 0;
  Task_ResetAll();
  gEndingPicture_MapScroll = 0;
  gEndingPicture_Scroll = 0;
  // One chained assignment: the three addresses are loaded nested, the map
  // pointer's innermost.
  gScrollTarget = gEndingPicture_Row = (s32)(gEndingPicture_Map = NULL);
  gEndingPicture_Speed = 1;
  gEndingPicture_Block = 0;
  Task_Add(EndingPicture_Scroll, TASK_PASS_VBLANK);
  Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
  Credits_BuildBackgroundTilemap((u32 *)BG_SCREEN_BLOCK(15));
  Credits_BuildBackgroundTilemap((u32 *)BG_SCREEN_BLOCK(31));
  Credits_ClearBuffer(0);
  Credits_ClearBuffer(1);
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(31),
             &REG_BG2CNT);
  Io_Write16(BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(15),
             &REG_BG3CNT);
  Io_Write16(DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                 DISPCNT_OBJ_ON,
             &REG_DISPCNT);
  Io_Write16(BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3 |
                 BLDCNT_TGT2_BD,
             &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);
  EndingPicture_InitStrips();
}

/// EndingPicture_CopyMap's body as EndingPicture_Show inlines it: the same
/// copy, written with each column's strip and tilemap found from its number.
static inline void CopyScrollMap(const u8 *source, u8 *destination,
                                 s32 position) {
  u32 start;
  s32 column;
  s32 row;
  u32 offset;
  u32 from;

  start = ENDING_PICTURE_ROW_OFFSET(gEndingPicture_MapScroll);
  gScrollTarget = gEndingPicture_Row = position;
  gEndingPicture_Scroll = position << 16;
  for (column = 0; column < ENDING_PICTURE_COLUMNS; column++) {
    u8 *to = destination + column * ENDING_PICTURE_SCREEN_BYTES;
    const u8 *strip = source + column * ENDING_PICTURE_STRIP_BYTES;
    from = (position / 8) * ENDING_PICTURE_ROW_BYTES;
    offset = ENDING_PICTURE_WRAP(start);
    for (row = 0; row < ENDING_PICTURE_ROWS; row++) {
      Dma_Set(strip + from, to + offset,
              DMA_COPY_WORDS(ENDING_PICTURE_ROW_BYTES / 4), REG_DMA3);
      from += ENDING_PICTURE_ROW_BYTES;
      offset = ENDING_PICTURE_WRAP(offset + ENDING_PICTURE_ROW_BYTES);
    }
  }
}

/// Shows a scrolling picture on the hidden one of the two BG character blocks.
/// With no resource it fills the map buffer with a plain tile word and clears
/// the palette half; otherwise it clears the block's first 2 KB, expands
/// resource's map into the buffer with BattleEffect_Expand2Code copied to the
/// heap (the fill word marks the second block's tiles), and queues the
/// resource's 128-colour palette. Then it copies the map at position into the
/// tilemap (CopyScrollMap), flips the block and restarts the fade-in counter.
void EndingPicture_Show(s32 resource, u8 *map, s32 position) {
  volatile u32 cell;
  u32 word;
  u32 fill = 0;
  u8 *tiles;
  u16 *palette;
  u8 *data;
  s32 (*code)(u8 *, u8 *, u32);

  if (resource == 0) {
    if (gEndingPicture_Block != 0) {
      word = PIXELS_8BPP(1);
      tiles = (u8 *)BG_CHAR_BLOCK(0);
      palette = (u16 *)BG_PLTT;
    } else {
      word = PIXELS_8BPP(129);
      tiles = (u8 *)BG_CHAR_BLOCK(2);
      palette = (u16 *)&BG_PLTT_COLOR(8, 0);
    }
    Dma_Fill32Cell(cell, word, map, ENDING_PICTURE_MAP_BYTES);
    Dma_Fill32Cell(cell, 0, palette, BACKDROP_PALETTE_BYTES);
  } else {
    if (gEndingPicture_Block != 0) {
      tiles = (u8 *)BG_CHAR_BLOCK(0);
      palette = (u16 *)BG_PLTT;
    } else {
      fill = PIXELS_8BPP(128);
      tiles = (u8 *)BG_CHAR_BLOCK(2);
      palette = (u16 *)&BG_PLTT_COLOR(8, 0);
    }
    Dma_Fill32Cell(cell, 0, tiles, ENDING_PICTURE_SCREEN_BYTES);
    data = Resource_GetTableEntry(resource);
    RamCode_Load(code, BattleEffect_Expand2Code, BattleEffect_Expand2CodeSize);
    code(data + BACKDROP_PALETTE_BYTES, map, fill);
    Heap_Release(code);
    IO_WRITE_QUEUE_PUSH((u32)palette, (u32)data,
                        DMA_COPY_WORDS(BACKDROP_PALETTE_BYTES / 4));
  }
  gEndingPicture_Map = map;
  CopyScrollMap(map, tiles, position);
  gEndingPicture_Block ^= 1;
  gEndingPicture_FadeFrames = 0;
}

/// Waits, for at most 300 frames, until the scroll reaches its row.
void EndingPicture_WaitForScroll(void) {
  s32 frames = 0;

  if (gEndingPicture_Scroll != gEndingPicture_Row << 16) {
    do {
      wait(1);
      frames++;
      if (frames >= ENDING_PICTURE_SCROLL_TIMEOUT)
        break;
    } while (gEndingPicture_Scroll != gEndingPicture_Row << 16);
  }
}

/// A VBlank task added by EndingPicture_InitStrips: queues the 24 border
/// strip sprites.
void EndingPicture_DrawStrips(void) {
  struct QueuedSprite *entry = gEndingPicture_Strips;
  s32 i;

  for (i = 0; i < ENDING_PICTURE_STRIP_SPRITES; i++)
    Oam_AddSpriteAtDepthClamped(entry++, 0);
}

/// Allocates the 24 sprite entries of the scrolling picture's border strips,
/// fills their tiles with colour 1 and adds EndingPicture_DrawStrips to draw
/// them; the entries are written as words, as The Broken Seal's
/// Credits_InitCrawlSprites writes its strips.
void EndingPicture_InitStrips(void) {
  s32 slot = VramCache_FindFreeEntry();
  struct QueuedSprite *entries;
  u32 *words;
  s32 tile;
  u32 i;

  /* FAKEMATCH(temporary): the allocation kept in entries. */
  entries = Heap_AllocEwram(ENDING_PICTURE_STRIP_SPRITES *
                            sizeof(struct QueuedSprite));
  gEndingPicture_Strips = entries;
  tile = VramCache_Load(slot, 0x200, 0);
  Dma_Fill32(PIXELS_4BPP(1), (void *)(OBJ_VRAM0 + tile * TILE_SIZE_4BPP),
             0x200);
  words = (u32 *)gEndingPicture_Strips;
  for (i = 0; i < 8; i++) {
    *words++ = 0;
    *words++ = (i << 21) | OBJ_SIZE_32X16 | 120;
    *words++ = tile;
  }
  for (i = 0; i < 8; i++) {
    *words++ = 0;
    *words++ = (i << 21) | OBJ_SIZE_32X16 | 136;
    *words++ = tile | OBJ_PRIORITY(2);
  }
  for (i = 0; i < 8; i++) {
    *words++ = 0;
    *words++ = (i << 21) | OBJ_SIZE_32X8 | 152;
    *words++ = tile | OBJ_PRIORITY(2);
  }
  Task_Add(EndingPicture_DrawStrips, TASK_PASS_VBLANK);
}

/// Moves the top strip's eight sprites to y 120 when enable is set, and to
/// y 0 otherwise.
void EndingPicture_LowerTopStrip(s32 enable) {
  struct QueuedSprite *entry = gEndingPicture_Strips;
  u32 i;

  if (!enable) {
    for (i = 0; i < 8; i++) {
      entry->oam.y = 0;
      entry++;
    }
  } else {
    for (i = 0; i < 8; i++) {
      entry->oam.y = 120;
      entry++;
    }
  }
}

/// The slideshow backdrops, ending in 0. Nothing reads this list.
// clang-format off
const u32 gDisplayScroll_UnusedSlideResources[] = {
    (u32)&ResourceId_ValeStormBackdrop,
    (u32)&ResourceId_SolSanctumBackdrop,
    (u32)&ResourceId_GrasslandBackdrop,
    (u32)&ResourceId_InteriorBackdrop,
    (u32)&ResourceId_ForestBackdrop,
    (u32)&ResourceId_KolimaForestBackdrop,
    (u32)&ResourceId_TretTreeInteriorsBackdrop,
    (u32)&ResourceId_TretTreeBranchBackdrop,
    (u32)&ResourceId_SnowfieldBackdrop,
    (u32)&ResourceId_MercuryLighthouseBackdrop,
    (u32)&ResourceId_MercuryLighthouseAerieBackdrop,
    (u32)&ResourceId_ImilFuchinFallsBackdrop,
    (u32)&ResourceId_MogallForestBackdrop,
    (u32)&ResourceId_AltinPeakBackdrop,
    (u32)&ResourceId_LamakanDesertBackdrop,
    (u32)&ResourceId_TolbiShipDeckBackdrop,
    (u32)&ResourceId_TolbiShipInteriorsBackdrop,
    (u32)&ResourceId_BeachBackdrop,
    (u32)&ResourceId_ColossoBackdrop,
    (u32)&ResourceId_AltmillerCaveBackdrop,
    (u32)&ResourceId_SuhallaDesertBackdrop,
    (u32)&ResourceId_CrossboneIsleCaveBackdrop,
    (u32)&ResourceId_PirateShipBackdrop,
    (u32)&ResourceId_SuhallaGateBackdrop,
    (u32)&ResourceId_TunnelRuinsBackdrop,
    (u32)&ResourceId_BabiLighthouseBackdrop,
    (u32)&ResourceId_VenusLighthouseBackdrop,
    (u32)&ResourceId_VenusLighthouseAerieBackdrop,
    (u32)&ResourceId_VenusLighthouseAerieStormBackdrop,
    (u32)&ResourceId_DuskCloudsBackdrop,
    (u32)&ResourceId_VioletSkyBackdrop,
    0
};

/// The backdrops the slideshow fades between, in turn; 0 ends the list,
/// and the table has room for 25.
const u32 gDisplayScroll_SlideResources[25] = {
    (u32)&ResourceId_ValeStormBackdrop,
    (u32)&ResourceId_SolSanctumBackdrop,
    (u32)&ResourceId_InteriorBackdrop,
    (u32)&ResourceId_KolimaForestBackdrop,
    (u32)&ResourceId_TretTreeBranchBackdrop,
    (u32)&ResourceId_SnowfieldBackdrop,
    (u32)&ResourceId_MercuryLighthouseBackdrop,
    (u32)&ResourceId_ImilFuchinFallsBackdrop,
    (u32)&ResourceId_MogallForestBackdrop,
    (u32)&ResourceId_AltinPeakBackdrop,
    (u32)&ResourceId_LamakanDesertBackdrop,
    (u32)&ResourceId_TolbiShipDeckBackdrop,
    (u32)&ResourceId_TolbiShipInteriorsBackdrop,
    (u32)&ResourceId_BeachBackdrop,
    (u32)&ResourceId_ColossoBackdrop,
    (u32)&ResourceId_SuhallaDesertBackdrop,
    (u32)&ResourceId_TunnelRuinsBackdrop,
    (u32)&ResourceId_BabiLighthouseBackdrop,
    (u32)&ResourceId_VenusLighthouseBackdrop,
    (u32)&ResourceId_VenusLighthouseAerieBackdrop,
    (u32)&ResourceId_DuskCloudsBackdrop,
    0
};

static const u8 sBlankCreditLine[] = "";

/// Ends a section of the credits: the scroll stops on it, and a section's
/// start is found by counting these marks.
#define CREDITS_SECTION_END ((const void *)255)

#if (LANGUAGE == LANGUAGE_GERMAN)
#define CREDITS_SUBTITLE "DIE VERGESSENE EPOCHE"
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define CREDITS_SUBTITLE "LA EDAD PERDIDA"
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define CREDITS_SUBTITLE "L'\177GE PERDU" ///< L'ÂGE PERDU: the French font draws Â at 0x7f
#elif (LANGUAGE == LANGUAGE_ITALIAN)
#define CREDITS_SUBTITLE "L'ERA PERDUTA"
#endif

// The Spanish, French and Italian fonts draw í at '`'.
#if (LANGUAGE == LANGUAGE_GERMAN)
#define RODRIGUEZ "Rodriguez"
#else
#define RODRIGUEZ "Rodr`guez"
#endif

/// The staff credits, one line each, ending in 0.
const void *gDisplayScroll_LineTable[] = {
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
    CREDITS_SECTION_END,
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
#if (LANGUAGE != LANGUAGE_JAPANESE)
    "P.E. Jareth Hein",
#else
    "P.E.Jareth Hein",
#endif
    sBlankCreditLine,
    sBlankCreditLine,
    "User Interface",
    sBlankCreditLine,
    "Yutaka Yamamoto",
    CREDITS_SECTION_END,
#if (LANGUAGE != LANGUAGE_JAPANESE)
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
    CREDITS_SECTION_END,
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
    CREDITS_SECTION_END,
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
#if (LANGUAGE == LANGUAGE_JAPANESE)
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
#if (LANGUAGE == LANGUAGE_JAPANESE)
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
    // Each edition names its own translator first.
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
    CREDITS_SECTION_END,
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
#if (LANGUAGE != LANGUAGE_JAPANESE)
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
    CREDITS_SECTION_END,
    0
};
// clang-format on

#include "../graphics/credits/glyph_widths.h"

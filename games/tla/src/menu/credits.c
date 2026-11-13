/// @file
/// The staff credits: the battle backdrops behind them, the lines that scroll
/// past, and the scrolling pictures with their border strips.
///
/// One object, as The Broken Seal's credits.c is, with its slides and lines at
/// its end; the background decoders linked before it are assembly
/// (credits/credits_background_*.s).

#include "types.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "version.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "ram_code.h"
#include "heap_release.h"
#include "resource.h"
#include "frame.h"
#include "input.h"
#include "task.h"
#include "bg_scroll.h"
#include "display_scroll.h"
#include "render_input.h"
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
s32 Credits_DrawLine(const u8 *text, s32 slot, s32 align);
extern const u8 BattleEffect_ExpandCode[];
extern u8 BattleEffect_ExpandCodeSize[];
extern const u8 gCredits_GlyphWidths[];
extern const u8 gDisplayScroll_Font[];

void EndingPicture_InitStrips(void);
extern const u8 BattleEffect_Expand2Code[];
extern u8 BattleEffect_Expand2CodeSize[];

#include "gamepak/far_call/window.h"
#include "module/system/system.h"
#include "ram_buffer.h"
#include "resource_ids.h"
#include "flag_call.h"

extern u8 gDebugTextEnabled;

/// Fills 30K bytes of 256-colour characters with colour 1 and clears 128
/// palette colours.
/// alt: 0 selects the first half (VRAM 0x06000000, palette colours 0-127);
///      1 selects the second (VRAM 0x06008000, colours 128-255, fill colour
///      129).
// The twin of The Broken Seal's Credits_ClearBuffer.
void Graphics_ClearCharacterBlockAndPalette(s32 alt) {
  u32 v;
  u32 vram;
  u32 pal;
  volatile u32 fill;

  if (alt == 0) {
    v = 0x01010101;
    vram = 0x06000000;
    pal = 0x05000000;
  } else {
    v = 0x81818181;
    vram = 0x06008000;
    pal = 0x05000100;
  }
  fill = v;
  Dma_Set((const void *)&fill, (void *)vram,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 7680, REG_DMA3);
  fill = 0;
  Dma_Set((const void *)&fill, (void *)pal,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 64, REG_DMA3);
}

/* VIEW(credits): variable gFlash_Handler3 as u16 gFlash_Handler3[];
 * sound_driver_symbols.h has struct LzPackWork *gFlash_Handler3; tried as the 3
 * declarations' extern FlashWaitProc gFlash_Handler3, it does not compile in
 * tla-ja: games/tla/src/menu/credits.c:72: parse error before
 * `gFlash_Handler3'. */
extern u16 gFlash_Handler3__credits[] __asm__("gFlash_Handler3");

/// Loads a backdrop resource (its 128 colours, then its packed tiles) into
/// one half of BG VRAM and queues its palette for the next VBlank, as The
/// Broken Seal does, but greyed: each channel
/// becomes (2 red + 4 green + blue + 5 * channel) / 16. A zero resource
/// clears that half instead. The tiles are unpacked by the
/// BattleEffect_Expand kernel, copied by DMA into a heap block for the call;
/// the grey colours are built in the EWRAM buffer at gFlash_Handler3.
void Credits_LoadBackground(u32 resourceId, s32 alternate) {
  const u8 *resourceData;
  u32 paletteDestination;
  u32 paletteOffset;
  void *tileDestination;
  PackedDecoder decoder;
  u32 size;
  s32 i;

  if (resourceId == 0) {
    Graphics_ClearCharacterBlockAndPalette(alternate);
    return;
  }
  resourceData = Resource_GetTableEntry(resourceId);
  if (alternate == 0) {
    paletteOffset = 0;
    tileDestination = (void *)0x06000000;
    paletteDestination = 0x05000000;
  } else {
    paletteOffset = 0x80808080;
    tileDestination = (void *)0x06008000;
    paletteDestination = 0x05000100;
  }
  size = (u32)BattleEffect_ExpandCodeSize;
  decoder = (PackedDecoder)Heap_Alloc(size);
  Dma_Set(BattleEffect_ExpandCode, decoder, DMA_COPY_WORDS(size >> 2),
          REG_DMA3);
  decoder(resourceData + 256, tileDestination, paletteOffset);
  Heap_Release(decoder);

  for (i = 0; i < 128; i++) {
    u16 color = *(const s16 *)resourceData;
    s32 blue = (color >> 10) & 0x1f;
    s32 green = (color >> 5) & 0x1f;
    s32 red = color & 0x1f;
    s32 sum = (green * 2 + red) * 2 + blue;

    resourceData += 2;
    blue = (sum + blue * 5) / 16;
    green = (sum + green * 5) / 16;
    red = (sum + red * 5) / 16;
    gFlash_Handler3__credits[i] = blue << 10 | green << 5 | red;
  }
  IO_WRITE_QUEUE_PUSH(paletteDestination, (u32)gFlash_Handler3__credits,
                      DMA_COPY_WORDS(64));
}

/// Fills the HBlank word table: 32 words of 0x01ff01ff, 240 rising steps,
/// 48 more of 0x01ff01ff and 192 zeros.
void Credits_BuildBackgroundTilemap(u32 *table) {
  s32 i;
  u32 value;
  u32 step;

  value = 0x01ff01ff;
  step = 0x10000;
  for (i = 0; i < 32; i++)
    *table++ = value;
  for (i = 0; i < 240; i++) {
    *table++ = step;
    step += 0x20002;
  }
  for (i = 0; i < 48; i++)
    *table++ = value;
  for (i = 0; i < 192; i++)
    *table++ = 0;
}

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

  gRenderOamEnabled = 0;
  gCpuLoadDisplayEnabled = 0;
  gDebugTextEnabled = 0;
  gCredits_PendingSlides = 0;
  gAutoSleepEnabled = 0;
  Task_ResetAll();
  Task_Add(Credits_ScrollBackgrounds, TASK_PASS_FRAME);
  Io_Write16(0x40, &REG_DISPCNT);
  Credits_BuildBackgroundTilemap((u32 *)0x06007800);
  Credits_BuildBackgroundTilemap((u32 *)0x0600f800);
  Graphics_ClearCharacterBlockAndPalette(0);
  Graphics_ClearCharacterBlockAndPalette(1);
  Io_Write16(0x1f8a, &REG_BG2CNT);
  Io_Write16(0x0f83, &REG_BG3CNT);
  Io_Write16(0x1c40, &REG_DISPCNT);
  Io_Write16(0x2844, &REG_BLDCNT);

  if (mode == 1) {

    Credits_InitCrawlSprites();
    wait(60);
    for (slide = 0;; slide++) {
      s16 *count = &gCredits_PendingSlides;

      Credits_LoadBackground(gDisplayScroll_SlideResources[slide],
                             (slide & 1) ^ 1);
      for (step = 1; step <= 16; step++) {
        if (slide & 1) {
          s32 t = 16;
          s32 hi = step << 8;
          s32 r = t - step;
          REG_BLDALPHA = hi | r;
        } else
          REG_BLDALPHA = ((16 - step) << 8) | step;
        for (frame = 0; frame <= 3; frame++) {
          if (gInput.held & 8)
            goto done;
          wait(1);
        }
      }
      if (*count == 0) {
        do {
          gInput.pressed;
          if (gInput.held & 8)
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
    while (gDisplayScroll_Busy <= 375)
      wait(1);
    Credits_StopTextSprites();
  }

done:
  Io_Write16(0, &REG_BLDCNT);
  Io_Write16(0x1040, &REG_DISPCNT);
  Window_LoadGraphics();
  DebugText_Clear();
  gRenderOamEnabled = 1;
  return 0;
}

/// A frame-pass task added by Credits_InitCrawlSprites, as The Broken Seal's
/// Credits_UpdateTextSprites: maps the visible sixteen rows of the story crawl
/// onto the tile ring and uploads their sprites. While a slide waits, A or B
/// releases the next one (gCredits_PendingSlides); the crawl advances a pixel
/// every fourth frame, and every frame but those while B is held.
void Credits_UpdateCrawlSprites(void) {
  u32 fine = gDisplayScroll_Position & 7;
  u32 tile = (((s16)gDisplayScroll_Position / 8) & 31) * 24;
  u32 *words = (u32 *)(gDisplayScroll_Objects + 24);
  s32 row;

  for (row = 0; row < 16; row++) {
    s32 column;

    for (column = 0; column < 6; column++) {
      *words++ =
          (row * 8 + 16 - fine) | ((column * 32 + 24) << 16) | 0x40004000;
      *words++ = tile;
      tile += 4;
      if (tile == 768)
        tile = 0;
    }
  }
  Dma_Set(gDisplayScroll_Objects, (void *)0x07000000, 0x84000100,
          (u32 *)0x040000d4);

  if (gDisplayScroll_Busy == 1 && (gInput.held & 3)) {
    gDisplayScroll_Busy = 0;
    gCredits_PendingSlides++;
  }
#if TLA_PATCH_1
  // From patch 1 a held button also releases state 3; the Japanese release
  // checks only state 1.
  if (gDisplayScroll_Busy == 3 && (gInput.held & 3))
    gDisplayScroll_Busy = 0;
#endif
  if (gDisplayScroll_Busy == 0) {
    if ((gVBlankCount & 3) == 0)
      gDisplayScroll_Position++;
    if ((gInput.held & 2) && (gVBlankCount & 3))
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
          line / 2 + first - 1, ((line + 16) & 31) * 24, 1);
    }
  }
}

/// Allocates the story crawl's sprites, builds its border and text rows, and
/// adds Credits_UpdateCrawlSprites and Credits_DrawEnteringCrawlLine, as The
/// Broken Seal's Credits_InitTextSprites starts its credits.
void Credits_InitCrawlSprites(void) {
  /* FAKEMATCH(volatile): the DMA fill word volatile; as a plain u32, 6 of 152
   * instructions differ in tla-en. */
  volatile u32 fill;
  u32 *entry;
  u32 i;
  u32 j;

  void *objects = Heap_AllocEwram(128 * sizeof(struct DisplayScrollObject));
  void **cell = (void **)&gDisplayScroll_Objects;
  *cell = objects;
  fill = 0;
  Dma_Set((void *)&fill, (void *)OBJ_VRAM0,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (0x6000 / 4),
          REG_DMA3);
  fill = 0x11111111;
  Dma_Set((void *)&fill, (void *)0x06016000,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 64, REG_DMA3);

  entry = *(u32 **)cell;
  {
    u32 attr = 0x80004000;
    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  {
    u32 attr = 0x80004088;
    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  {
    u32 attr = 0x40004098;
    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  for (i = 0; i < 16; i++) {
    for (j = 0; j < 6; j++) {
      u32 *q = entry;
      u32 tile = (i * 3) * 8 + j * 4;

      *q++ = (i * 8 + 16) | ((j * 32 + 24) << 16) | 0x40004000;
      *q = tile;
      entry += 2;
    }
  }
  for (i = 0; i < 8; i++) {
    u32 *q = entry;

    *q++ = 0x00c000c0;
    *q = 0x300;
    entry += 2;
  }
  gDisplayScroll_Position = 0;
  gDisplayScroll_LastPosition = 0;
  gDisplayScroll_Busy = 0;
  Task_Add(Credits_UpdateCrawlSprites, TASK_PASS_FRAME);
  Task_Add(Credits_DrawEnteringCrawlLine, TASK_PASS_VBLANK);
  for (i = 0; i < 32; i++)
    Credits_DrawLine(gDisplayScroll_LineTable[0], i * 24, 1);
}

// The clear goes through an inline wrapper, as the Broken Seal's iwram_call.h
// writes Iwram_ClearWords; through this game's direct entry, the build shifts
// the size before loading the routine's address.

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

  canvas = Heap_AllocEwram(0x900);
  // BUG: the clear, and the packing loop's reads below, cover 0xc00 bytes of
  // a 0x900-byte allocation.
  Iwram_ClearWords(canvas, 0xc00);
  busy = Text_DrawMessageToCanvasFar(message, canvas, align);
  dst = canvas;
  src = canvas;
  for (j = 0; j < 0x600; j++) {
    u8 low = *src++;
    *dst++ = low | (*src++ << 4);
  }
  for (i = 0; i <= 23; i++) {
    u32 *column = (u32 *)canvas + i;

    tiles = (u32 *)((slot + i) * 32 + 0x06010000);
    for (row = 0; row < 8; row++) {
      *tiles++ = column[row * 24];
    }
  }
  for (offset = slot * 32, top = (u32 *)(canvas + 0x300), j = 0; j <= 23;
       j++, offset += 32, top++) {
    tiles = (u32 *)(offset + 0x06010300);
    for (row = 0; row < 8; row++) {
      *tiles++ = top[row * 24];
    }
  }
  Heap_Release(canvas);
  return busy;
}

void Credits_UpdateTextSprites(void) {
  u32 frame = gDisplayScroll_Position;
  u32 fine = frame & 7;
  s32 tile = (((s16)frame / 8) & 0x1f) * 3 * 8;
  u32 *entry = (u32 *)(gDisplayScroll_Objects + 24);
  s32 row;

  for (row = 0; row <= 15; row++) {
    s32 col;

    for (col = 0; col < 6; col++) {
      u32 *q = entry;

      *q++ = (row * 8 + 16 - fine) | ((col * 32 + 24) << 16) | 0x40004000;
      entry += 2;
      *q = tile;
      tile += 4;
      if (tile == 0x300)
        tile = 0;
    }
  }
  Dma_Set(gDisplayScroll_Objects, OAM, DMA_COPY_WORDS(256), REG_DMA3);
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
                     ((position / 8 + 16) & 31) * 24, 1);
  } else if (position / 8 != gDisplayScroll_LastPosition / 8) {
    gDisplayScroll_LastPosition = current;
    text = gDisplayScroll_LineTable[line];
    if (text == (const u8 *)255) {
      gDisplayScroll_Busy = 1;
      Credits_DrawLine(gDisplayScroll_LineTable[0], ((line + 16) & 31) * 24, 1);
    } else {
      gDisplayScroll_Busy = Credits_DrawLine(text, ((line + 16) & 31) * 24, 1);
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
  /* FAKEMATCH(volatile): the DMA fill word volatile; as a plain u32, 6 of 178
   * instructions differ in tla-en. */
  volatile u32 fill;
  s32 marks;
  u32 *entry;
  u32 i;
  u32 j;

  marks = 0;
  i = 0;
  if (page != 0 && gDisplayScroll_LineTable[0] != 0) {
    do {
      if ((u32)gDisplayScroll_LineTable[i] == 255)
        marks++;
      i++;
      if (i == 500 || marks == page)
        break;
    } while (gDisplayScroll_LineTable[i] != 0);
  }
  gDisplayScroll_Position = i * 8;

  {
    void *objects = Heap_AllocEwram(128 * sizeof(struct DisplayScrollObject));
    void **cell = (void **)&gDisplayScroll_Objects;

    *cell = objects;
    fill = 0;
    Dma_Set((void *)&fill, (void *)OBJ_VRAM0,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (0x6000 / 4),
            REG_DMA3);
    fill = 0x11111111;
    Dma_Set((void *)&fill, (void *)0x06016000,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 64, REG_DMA3);
    entry = *(u32 **)cell;
  }
  {
    u32 attr = 0x80004000;

    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  {
    u32 attr = 0x80004088;

    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  {
    u32 attr = 0x40004098;

    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  for (i = 0; i < 16; i++) {
    for (j = 0; j < 6; j++) {
      u32 *q = entry;
      u32 tile = (i * 3) * 8 + j * 4;

      *q++ = (i * 8 + 16) | ((j * 32 + 24) << 16) | 0x40004000;
      *q = tile;
      entry += 2;
    }
  }
  for (i = 0; i < 8; i++) {
    u32 *q = entry;

    *q++ = 0x00c000c0;
    *q = 0x300;
    entry += 2;
  }
  gDisplayScroll_LastPosition = 0;
  gDisplayScroll_Busy = 0;
  Task_Add(Credits_UpdateTextSprites, TASK_PASS_FRAME);
  Task_Add(Credits_DrawEnteringLine, TASK_PASS_VBLANK);
  for (i = 0; i < 32; i++)
    Credits_DrawLine(gDisplayScroll_LineTable[0], i * 24, 1);
}

/// Ends the scrolling credits: blanks the tile ring, removes their two tasks
/// and clears the sprites.
void Credits_StopTextSprites(void) {
  s32 i;
  u32 *objects;

  for (i = 0; i < 32; i++)
    Credits_DrawLine(gDisplayScroll_LineTable[0], i * 24, 1);
  Task_Remove(Credits_UpdateTextSprites);
  Task_Remove(Credits_DrawEnteringLine);
  objects = (u32 *)gDisplayScroll_Objects;
  for (i = 0; i != 0x100; i++)
    *objects++ = 0;
  Dma_Set(gDisplayScroll_Objects, OAM, DMA_COPY_WORDS(256), REG_DMA3);
}

s32 Credits_DrawLine(const u8 *text, s32 slot, s32 align) {
  u8 *buf = (u8 *)Heap_AllocEwram(0x900);
  s32 x = 0;
  s32 width = 192;
  const u8 *font = gDisplayScroll_Font;
  s32 tiles;
  s32 pos;
  const u8 *p;
  u8 *dst;
  u32 c;
  s32 glyph;
  s32 w;
  s32 row;
  s32 bit;
  u32 bits;
  u32 mask;
  /* FAKEMATCH(dead): an unused array for the stack frame's 32 bytes no
   * instruction touches; as no array, 4 of 279 instructions differ in tla-en.
   */
  u32 unused[8];

  if (text == NULL)
    return -1;
  if (!GameFlag_Get(0x200)) {
    Iwram_FillWordsEntry(buf, 0x900, 0);
    GameFlag_Set(0x200);
  } else {
    Iwram_CopyWords(buf, buf + 0x800, 0x100);
    Iwram_FillWordsEntry(buf + 0x100, 0x800, 0);
  }

  p = text;
  pos = 0;
  while ((c = *p++) != 0) {
    if (c > 31)
      pos += gCredits_GlyphWidths[c - 32];
  }
  if (align == 2)
    x = width - pos;
  else if (align == 1)
    x = (width - pos) / 2;

  p = text;
  pos = 0;
  while ((c = *text++) != 0) {
    if (c > 31) {
      glyph = c - 32;
      p = font + glyph * 8;
      dst = buf + x + pos;
      for (row = 0; row < 8; row++) {
        bits = *p++;
        mask = 0x80;
        for (bit = 7; bit >= 0; bit--) {
          if (bits & mask) {
            dst[0x101] = 1;
            dst[0] = 15;
          }
          dst++;
          mask >>= 1;
        }
        dst += 248;
      }
      w = 1;
      if (c > 31)
        w = gCredits_GlyphWidths[glyph];
      pos += w;
    }
  }

  tiles = width / 8;
  p = buf;
  dst = buf;
  for (row = 0; row < 8; row++) {
    for (bit = 0; bit < tiles * 4; bit++) {
      c = *p++;
      c |= *p++ << 4;
      *dst++ = c;
    }
    dst += 256 - tiles * 4;
    p += 256 - tiles * 8;
  }

  for (row = 0; row < tiles; row++) {
    s32 tile = slot + row;
    u32 *words = (u32 *)buf + row;

    *(u32 *)(0x06010000 + tile * 32) = words[0x000];
    *(u32 *)(0x06010004 + tile * 32) = words[0x040];
    *(u32 *)(0x06010008 + tile * 32) = words[0x080];
    *(u32 *)(0x0601000c + tile * 32) = words[0x0c0];
    *(u32 *)(0x06010010 + tile * 32) = words[0x100];
    *(u32 *)(0x06010014 + tile * 32) = words[0x140];
    *(u32 *)(0x06010018 + tile * 32) = words[0x180];
    *(u32 *)(0x0601001c + tile * 32) = words[0x1c0];
  }
  Heap_Release(buf);
  return 0;
}

/// Unused: an empty function nothing calls, after the pool.
UNUSED void Credits_ReservedNoOp(void) {}

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

  start = (gEndingPicture_MapScroll >> 19) << 6;
  gScrollTarget = gEndingPicture_Row = position;
  {
    volatile s32 *word = &gEndingPicture_Scroll;

    *word = position << 16;
  }
  for (column = 0; column < 15; column++) {
    from = (position / 8) << 6;
    offset = start & 0x7ff;
    for (row = 0; row < 31; row++) {
      Dma_Set(source + from, destination + offset, DMA_COPY_WORDS(16),
              REG_DMA3);
      from += 64;
      offset = (offset + 64) & 0x7ff;
    }
    source += 0x1000;
    destination += 0x800;
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

  if (gEndingPicture_FadeFrames < 64) {
    s32 t = (s16)++gEndingPicture_FadeFrames / 4;
    s32 alpha;

    if (gEndingPicture_Block & 1) {
      alpha = ((16 - t) << 8) | t;
      REG_BLDALPHA = alpha;
    } else {
      alpha = (t << 8) | (16 - t);
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
    /* FAKEMATCH(temporary): the form kept; as scroll = gEndingPicture_Scroll, 6
     * of 212 instructions differ in tla-en. */
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
  if ((gEndingPicture_Scroll ^ old) & 0x80000) {
    source = gEndingPicture_Map;
    destination = (u8 *)0x06000000;
    start = (gEndingPicture_MapScroll >> 19) << 6;
    if (gEndingPicture_Block != 0)
      destination = (u8 *)0x06008000;
    scroll = gEndingPicture_Scroll;
    if (scroll > old) {
      from = (((u32)scroll >> 19) << 6) + 0x780;
      offset = (start + 0x780) & 0x7ff;
      for (i = 0; i < 15; i++) {
        Dma_Set(source + from, destination + offset, 0x84000010, REG_DMA3);
        source += 0x1000;
        destination += 0x800;
      }
    } else {
      from = ((u32)gEndingPicture_Scroll >> 19) << 6;
      offset = start & 0x7ff;
      for (i = 0; i < 15; i++) {
        Dma_Set(source + from, destination + offset, 0x84000010, REG_DMA3);
        source += 0x1000;
        destination += 0x800;
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
  gAutoSleepEnabled = 0;
  Task_ResetAll();
  gEndingPicture_MapScroll = 0;
  gEndingPicture_Scroll = 0;
  // One chained assignment: the three addresses are loaded nested, the map
  // pointer's innermost.
  gScrollTarget = gEndingPicture_Row = (s32)(gEndingPicture_Map = NULL);
  gEndingPicture_Speed = 1;
  gEndingPicture_Block = 0;
  Task_Add(EndingPicture_Scroll, TASK_PASS_VBLANK);
  Io_Write16(0x40, &REG_DISPCNT);
  Credits_BuildBackgroundTilemap((u32 *)0x06007800);
  Credits_BuildBackgroundTilemap((u32 *)0x0600f800);
  Graphics_ClearCharacterBlockAndPalette(0);
  Graphics_ClearCharacterBlockAndPalette(1);
  Io_Write16(0x1f8a, &REG_BG2CNT);
  Io_Write16(0x0f83, &REG_BG3CNT);
  Io_Write16(0x1c40, &REG_DISPCNT);
  Io_Write16(0x2844, &REG_BLDCNT);
  Io_Write16(16, &REG_BLDALPHA);
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

  start = (gEndingPicture_MapScroll >> 19) << 6;
  gScrollTarget = gEndingPicture_Row = position;
  gEndingPicture_Scroll = position << 16;
  for (column = 0; column < 15; column++) {
    u8 *to = destination + column * 0x800;
    const u8 *strip = source + column * 0x1000;
    from = (position / 8) << 6;
    offset = start & 0x7ff;
    for (row = 0; row < 31; row++) {
      Dma_Set(strip + from, to + offset, 0x84000010,
              (volatile u32 *)0x040000d4);
      from += 64;
      offset = (offset + 64) & 0x7ff;
    }
  }
}

/// Shows a scrolling picture on the hidden one of the two BG character blocks.
/// With no resource it fills the map buffer with a plain tile word and clears
/// the palette half; otherwise it clears the block's first 2 KB, expands
/// resource's map into the buffer with BattleEffect_Expand2Code copied to the
/// heap (the fill word marks the second block's tiles), and queues the
/// resource's 64-colour palette. Then it copies the map at position into the
/// tilemap (CopyScrollMap), flips the block and restarts the fade-in counter.
/// The size is a linker symbol (the pool holds 0x234 where movs and a shift
/// would do).
void EndingPicture_Show(s32 resource, u8 *map, s32 position) {
  /* FAKEMATCH(volatile): the DMA fill word volatile, so its address stays in a
   * register; as a plain u32, 10 of 209 instructions differ in tla-en. */
  volatile u32 cell;
  u32 word;
  u32 fill = 0;
  u8 *tiles;
  u16 *palette;
  u8 *data;
  s32 (*code)(u8 *, u8 *, u32);

  if (resource == 0) {
    if (gEndingPicture_Block != 0) {
      word = 0x01010101;
      tiles = (u8 *)0x06000000;
      palette = (u16 *)0x05000000;
    } else {
      word = 0x81818181;
      tiles = (u8 *)0x06008000;
      palette = (u16 *)0x05000100;
    }
    Dma_Fill32Cell(cell, word, map, 0xf000);
    Dma_Fill32Cell(cell, 0, palette, 0x100);
  } else {
    if (gEndingPicture_Block != 0) {
      tiles = (u8 *)0x06000000;
      palette = (u16 *)0x05000000;
    } else {
      fill = 0x80808080;
      tiles = (u8 *)0x06008000;
      palette = (u16 *)0x05000100;
    }
    Dma_Fill32Cell(cell, 0, tiles, 0x800);
    data = Resource_GetTableEntry(resource);
    RamCode_Load(code, BattleEffect_Expand2Code, BattleEffect_Expand2CodeSize);
    code(data + 0x100, map, fill);
    Heap_Release(code);
    IO_WRITE_QUEUE_PUSH((u32)palette, (u32)data, 0x84000040);
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
      if (frames > 299)
        break;
    } while (gEndingPicture_Scroll != gEndingPicture_Row << 16);
  }
}

/// A VBlank task added by EndingPicture_InitStrips: queues the 24 border
/// strip sprites.
void EndingPicture_DrawStrips(void) {
  struct QueuedSprite *entry = gEndingPicture_Strips;
  s32 i;

  for (i = 0; i < 24; i++)
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

  /* FAKEMATCH(temporary): the allocation kept in entries; as
   * gEndingPicture_Strips = Heap_AllocEwram(0x120), 1 of 82
   * instructions differ in tla-en. */
  entries = Heap_AllocEwram(0x120);
  gEndingPicture_Strips = entries;
  tile = VramCache_Load(slot, 0x200, 0);
  Dma_Fill32(0x11111111, (void *)(0x06010000 + tile * 32), 0x200);
  words = (u32 *)gEndingPicture_Strips;
  for (i = 0; i < 8; i++) {
    *words++ = 0;
    *words++ = (i << 21) | 0x80004078;
    *words++ = tile;
  }
  for (i = 0; i < 8; i++) {
    *words++ = 0;
    *words++ = (i << 21) | 0x80004088;
    *words++ = tile | 0x800;
  }
  for (i = 0; i < 8; i++) {
    *words++ = 0;
    *words++ = (i << 21) | 0x40004098;
    *words++ = tile | 0x800;
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
// The Broken Seal's list less one.
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

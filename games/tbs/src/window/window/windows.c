/// @file
/// The window sprite pool, opening and closing windows, starting messages.
// Part of window.c.

#include "../window_internal.h"

/// Takes the first window sprite from the free list, or returns NULL when every
/// sprite is in use. The sprite belongs to the caller until it is appended to a
/// window's list, which then frees it when the window is cleared or closed
/// (WindowSprite_ClearList). Glyph sprites, icons and portraits all come from
/// here.
struct WindowSprite *WindowSprite_AcquireFree(void) {
  struct WindowWork *work = WINDOW_WORK;
  struct WindowSprite *out = work->freeSprites.head;

  if (out != NULL) {
    // Taking the last sprite leaves the list empty.
    if (out->next == NULL)
      work->freeSprites.tailLink = &work->freeSprites.head;
    work->freeSprites.head = out->next;
    out->next = NULL;
  }
  return out;
}

/// Returns a window sprite to the end of the free list. A sprite from outside
/// the work block's pool is ignored.
void WindowSprite_ReleaseFree(struct WindowSprite *out) {
  struct WindowWork *work = WINDOW_WORK;

  // freeSprites follows the pool directly, so its address is the pool's end.
  if ((u32)out >= (u32)work->sprites &&
      (u32)out < (u32)&work->freeSprites.head) {
    struct WindowSprite **tail = work->freeSprites.tailLink;

    work->freeSprites.tailLink = &out->next;
    *tail = out;
    out->next = NULL;
  }
}

/// Links all 64 window sprites of the pool into the free list.
void WindowSprite_InitializeFreeList(void) {
  s32 remainingLinks;
  struct WindowWork *work;
  struct WindowSprite *out;
  struct WindowSprite *next;

  work = WINDOW_WORK;
  out = work->sprites;
  work->freeSprites.head = out;
  // Links sprites 0 to 62 to the next; the last, 63, ends the list.
  remainingLinks = WINDOW_SPRITE_COUNT - 2;
  do {
    next = out + 1;
    remainingLinks--;
    out->next = next;
    out = next;
  } while (remainingLinks >= 0);
  next->next = NULL;
  work->freeSprites.tailLink = &next->next;
}

/// Sets up the window system for the field (and the other screens outside
/// battle): allocates and clears the work block, blanks the tilemap, frees
/// every window sprite and starts the two per-frame callbacks, the tilemap
/// upload and the window update. Reached from SceneManager_Run
/// (overworld/overworld/run_scene.c), the debug message viewer
/// (DebugBattle_ViewMessages), Lucky Dice and Lucky Wheels (LuckyWheels_Run).
/// The work block (heap slot HEAP_SLOT_WINDOW) lives until the slot is next
/// allocated; nothing here frees it.
void WindowSystem_InitField(void) {
  struct WindowWork *work;
#if TBS_PATCH_1
  volatile u32 fill;
#endif

  // Clear the work block before linking any of its embedded lists.
  work = AllocHeapSlotEwram(HEAP_SLOT_WINDOW, WINDOW_WORK_SIZE);
#if TBS_PATCH_1
  fill = 0;
  Dma_Set((const void *)&fill, work, DMA_FILL32(WINDOW_WORK_SIZE / 4),
          REG_DMA3);
#else
  Iwram_ClearWords(work, WINDOW_WORK_SIZE);
#endif
  work->dirty = 1;
  work->glyphResource = NO_GLYPH_RESOURCE;
  work->textPalette = WINDOW_PALETTE;
#if TBS_PATCH_1
  fill = WINDOW_TILE_PAIR(WINDOW_TILE_BLANK);
  Dma_Set((const void *)&fill, work, DMA_FILL32(sizeof(work->tilemap) / 4),
          REG_DMA3);
#else
  Iwram_FillWords(work, sizeof(work->tilemap),
                  WINDOW_TILE_PAIR(WINDOW_TILE_BLANK));
#endif
  // Make the sprite and palette pools available, then start the updates. The
  // upload's order, 0x480, is TASK_PASS_VBLANK: it runs in vertical blank.
  WindowSprite_InitializeFreeList();
  Portrait_ClearSlots();
  Task_Add(WindowSystem_UploadTilemap, TASK_PASS_VBLANK);
  WindowSystem_StartFieldUpdate();
}

/// Sets up the window system for battle, as
/// WindowSystem_InitField does for the field, and marks the work
/// block as in battle (WINDOW_BATTLE). Battle_Run's setup
/// (battle/battle/encounter.c) passes loadGlyphTiles set only while
/// FLAG_DJINN_LESSON is set: it reserves the glyph sprites' tiles, which
/// only windows other than text canvases need. Unlike the field setup it does
/// not mark the portrait slots free (999): they stay 0 from the clear.
void WindowSystem_InitBattle(s32 loadGlyphTiles) {
  struct WindowWork *work;
#if TBS_PATCH_1
  volatile u32 fill;
#endif
  s32 tileIndex;

  // Copies pixel rows 4 to 6 of UI tile src to rows 0 to 2 of tile dst and
  // clears the rest of dst. A 4bpp row is 4 bytes: the copy takes rows 4 to 7
  // (16 bytes from byte 16), and the clear then blanks rows 3 to 7.
  s32 WindowSystem_CopyTileLowerRows(u32 sourceTile, u32 destinationTile) {
    sourceTile &= 0x3ff;
    destinationTile &= 0x3ff;
    Dma_Set(BG_CHAR_BLOCK(0) + sourceTile * TILE_SIZE_4BPP + 16,
            BG_CHAR_BLOCK(0) + destinationTile * TILE_SIZE_4BPP, DMA_COPY16(8),
            REG_DMA3);
    return Iwram_ClearWords(
        BG_CHAR_BLOCK(0) + destinationTile * TILE_SIZE_4BPP + 12, 20);
  }

  work = (struct WindowWork *)AllocHeapSlotEwram(HEAP_SLOT_WINDOW,
                                                 WINDOW_WORK_SIZE);
#if TBS_PATCH_1
  fill = 0;
  Dma_Set((const void *)&fill, work, DMA_FILL32(WINDOW_WORK_SIZE / 4),
          REG_DMA3);
#else
  Iwram_ClearWords(work, WINDOW_WORK_SIZE);
#endif
  work->dirty = 1;
  work->glyphResource = NO_GLYPH_RESOURCE;
  work->battleMode = WINDOW_BATTLE;
  work->textPalette = WINDOW_PALETTE;
#if TBS_PATCH_1
  fill = WINDOW_TILE_PAIR(WINDOW_TILE_BLANK);
  Dma_Set((const void *)&fill, work, DMA_FILL32(sizeof(work->tilemap) / 4),
          REG_DMA3);
#else
  Iwram_FillWords(work, sizeof(work->tilemap),
                  WINDOW_TILE_PAIR(WINDOW_TILE_BLANK));
#endif
  WindowSprite_InitializeFreeList();
  Task_Add(WindowSystem_UploadTilemap, TASK_PASS_VBLANK);
  WindowSystem_StartBattleUpdate(loadGlyphTiles);
  // Tiles 128 to 130 take the lower rows of the bottom frame tiles.
  WindowSystem_CopyTileLowerRows(WINDOW_TILE_FRAME_BOTTOM_LEFT, 128);
  WindowSystem_CopyTileLowerRows(WINDOW_TILE_FRAME_BOTTOM, 129);
  WindowSystem_CopyTileLowerRows(WINDOW_TILE_FRAME_BOTTOM_RIGHT, 130);
  // Tiles 128 to 130 are also the first three text tiles. Attribute 4 is
  // neither allocated nor shown, but nonzero, so the text tile search skips
  // them and no clear of the shown bits ever frees them.
  {
    u8 mode = 4;

    for (tileIndex = 2; tileIndex >= 0; tileIndex--)
      work->textTileState[tileIndex] = mode;
  }
}

/// The per-frame tilemap upload: copies each 256-byte band of the tilemap
/// (four tile rows) marked in the dirty bits to VRAM. Bit 0 marks the whole
/// tilemap, bits 1 to 5 one band each. Nothing is copied while
/// holdTilemapUpload holds the upload off.
void WindowSystem_UploadTilemap(void) {
  struct WindowWork *work = WINDOW_WORK;
  u32 bands;
  u8 *src;
  u8 *dst;
  if (!work->holdTilemapUpload) {
    bands = work->dirty;
    if (bands) {
      dst = WINDOW_TILEMAP_VRAM;
      src = (u8 *)work->tilemap;
      // Bit 0 asks for all five bands; after the shift bit n is band n.
      if (bands & 1)
        bands = 63;
      bands &= 63;
      bands >>= 1;
      do {
        if (bands & 1)
          Dma_Set(src, dst, DMA_COPY32(64), REG_DMA3);
        bands >>= 1;
        src += 256;
        dst += 256;
      } while (bands);
      work->dirty = bands;
    }
  }
}

/// Erases a window's rectangle of the tilemap (x, y, width and height in
/// tiles) and frees the text tiles it showed. The height is cut at the
/// screen's 20th row, and width and height are clamped to 2 to 30 tiles. In
/// battle the rows below row 16 take tile 0x7f, the tile the battle command
/// row fills its empty panels with (screens/command_row.c), instead of
/// the blank tile.
void Window_EraseBorderRect(s32 x, s32 y, u32 width, u32 height) {
  struct WindowWork *canvas = WINDOW_WORK;
  u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
  s32 tile;
  u32 bottom;
  u32 row;
  u8 *inBattle;

  // 240 << 8 is WINDOW_TILE_BLANK.
  tile = 240;
  bottom = y + height;
  tile <<= 8;
  if (bottom > 20)
    height = 20 - y;
  if (width <= 1)
    width = 2;
  if (width > 30)
    width = 30;
  if (height <= 1)
    height = 2;
  if (height > 30)
    height = 30;

  // Free the text tiles while the tilemap still names them.
  Window_ClearTextTilesInRect(x, y, width, height);

  row = 0;
  if (row < height) {
    inBattle = &canvas->battleMode;
    do {
      u32 column;

      if (*inBattle != 0) {
        if ((y + row) > 16)
          tile = WINDOW_TILE(0x7f);
        else
          tile = WINDOW_TILE_BLANK;
      }
      column = 0;
      if (column < width) {
        do {
          *cursor++ = tile;
          column++;
        } while (column < width);
      }
      row++;
      cursor += 32 - width;
    } while (row < height);
  }
  canvas->dirty = 1;
}

/// Draws a window at its full size: its frame, and for a text canvas window
/// the canvas, filled with colour 4 under a frame when the window is framed and
/// transparent otherwise.
void Window_DrawOpened(struct Window *window) {
  u32 flags;
  u32 value;
  u32 x;
  u32 y;
  u32 width;
  u32 height;
  struct WindowWork *work;

  work = WINDOW_WORK;
  value = window->height;
  flags = window->flags;
  height = value;
  value = 0;
  window->duration = value;
  x = window->x;
  y = window->y;
  width = window->width;
  // Canvas windows need both pixel storage and their tilemap rectangle.
  if (WINDOW_FLAG_TEXT_CANVAS & flags) {
    if (WINDOW_FLAG_FRAMED_CANVAS & flags) {
      Window_DrawFrame(x, y, width, height);
      Iwram_FillWords(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE,
                      TEXT_CANVAS_BACKGROUND_WORD);
    } else {
      Iwram_FillWords(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE, 0);
    }
    Window_MapTextCanvasTiles(x, y, width, height, 0);
  } else {
    Window_DrawFrame(x, y, width, height);
  }
  work->dirty = 1;
}

/// Opens a window at tile (x, y) of width by height tiles in the first free
/// window slot, and returns it, or NULL when all slots are in use. attrs takes
/// the WINDOW_FLAG_ flags. Unless it is WINDOW_FLAG_INSTANT the window zooms
/// open over eight frames, and this waits until it is fully open.
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs) {
  struct Window *slot;
  struct Window *found;
  s32 i;

  slot = WINDOW_WORK->windows;
  found = 0;
  // A closing slot remains occupied until its animation has finished.
  for (i = 0; i != WINDOW_COUNT; i++, slot++) {
    if ((slot->flags & WINDOW_FLAG_OPEN) == 0 && slot->duration == 0) {
      found = slot;
      break;
    }
  }
  if (found != 0) {
    // Reset the slot before applying the caller's canvas and anchor flags.
    found->y = y;
    found->width = width;
    found->height = height;
    found->x = x;
    found->sprites.head = NULL;
    found->state = 0;
    found->sprites.tailLink = &slot->sprites.head;
    found->opened = 1;
    found->flags = WINDOW_FLAG_OPEN;
    // Every new window starts the shared text style afresh. The function
    // takes no arguments: x is passed and ignored.
    Text_ResetStyle(x);
    if (attrs & WINDOW_FLAG_TEXT_CANVAS) {
      found->flags |= WINDOW_FLAG_TEXT_CANVAS;
    }
    if (attrs & WINDOW_FLAG_FRAMED_CANVAS) {
      found->flags |= WINDOW_FLAG_FRAMED_CANVAS;
    }
    if (attrs & WINDOW_FLAG_OUTLINED_TEXT) {
      found->flags |= WINDOW_FLAG_OUTLINED_TEXT;
    }
    if (attrs & WINDOW_FLAG_ANCHOR_BOTTOM) {
      found->flags |= WINDOW_FLAG_ANCHOR_BOTTOM;
    }
    if (attrs & WINDOW_FLAG_FIXED_X) {
      found->flags |= WINDOW_FLAG_FIXED_X;
    }
    if (attrs & WINDOW_FLAG_INSTANT) {
      found->flags |= WINDOW_FLAG_INSTANT;
      found->frame = 0;
      found->duration = 1;
      Window_DrawOpened(found);
    } else {
      // Zoom open over eight frames, and wait until it is open.
      found->duration = WINDOW_OPEN_FRAMES;
      found->frame = WINDOW_OPEN_FRAMES - 1;
      Window_WaitZoom(found);
      WaitFrames(1);
    }
  }
  return found;
}

/// Waits until a zooming window has finished opening or closing.
void Window_WaitZoom(struct Window *window) {
  if (!(WINDOW_FLAG_INSTANT & window->flags) && window->duration != 0) {
    do {
      WaitFrames(1);
    } while (window->duration != 0);
  }
}

/// Closes a window: clears its contents and remembers its last rectangle.
/// With instant set the window is erased and its slot freed at once; otherwise
/// it zooms closed over four frames first, and its slot stays taken until
/// Window_UpdateZooms frees it (Window_IsClosed says when). It returns
/// without waiting. Unless the window is a text canvas, its window sprites go
/// back to the pool at once; a text printer typing into it lets go of it
/// once it has closed (TextPrinter_RunAll). Does nothing for NULL.
void Window_Close(struct Window *window, s32 instant) {
  u16 zero;

  if (window == 0)
    return;

  WindowSprite_PrepareForRedraw(window);
  // Keep the old rectangle available while the closing animation shrinks it.
  window->previousX = window->x;
  window->previousY = window->y;
  window->previousWidth = window->width;
  zero = 0;
  window->flags = zero;
  window->previousHeight = window->height;

  if (instant != 0) {
    Window_EraseBorderRect(window->x, window->y, window->width, window->height);
    window->sprites.head = NULL;
    window->sprites.tailLink = NULL;
    window->width = zero;
    window->height = zero;
    window->x = zero;
    window->y = zero;
    window->opened = zero;
    window->animation = zero;
    window->state = zero;
    window->flags = zero;
    window->frame = zero;
    window->duration = zero;
    window->previousX = zero;
    window->previousY = zero;
    window->previousWidth = zero;
    window->previousHeight = zero;
  } else {
    // Zoom closed over four frames (Window_UpdateZooms), from frame 0:
    // instant is 0 here.
    window->frame = instant;
    window->duration = WINDOW_CLOSE_FRAMES;
  }
}

/// Clears a window's contents: redraws its empty frame and releases the render
/// window sprites it carries. A text canvas window is left alone.
void WindowSprite_PrepareForRedraw(struct Window *window) {
  if (!(WINDOW_FLAG_TEXT_CANVAS & window->flags)) {
    WindowSprite_RedrawSavedRect(window);
    WindowSprite_ClearList(window);
  }
}

/// Redraws a window's empty frame at its current rectangle.
void WindowSprite_RedrawSavedRect(struct Window *window) {
  Window_DrawFrame(window->x, window->y, window->width, window->height);
}

/// Releases every window sprite in a list and empties it. sprites is the list,
/// or a window (whose sprite list comes first); NULL is ignored.
void WindowSprite_ClearList(void *sprites) {
  struct WindowSpriteList *list = sprites;
  struct WindowSprite *node;
  struct WindowSprite *next;

  next = NULL;
  if (list != NULL) {
    node = list->head;
    // Detach the chain before returning its nodes to the shared free pool.
    list->tailLink = &list->head;
    list->head = next;
    while (node != NULL) {
      next = node->next;
      WindowSprite_Release(node);
      node = next;
    }
  }
}

/// Clears an area of a window's interior to the window fill. left, top, right
/// and bottom are pixels inside the window, and the area covers every tile
/// they touch.
void Window_ClearInteriorTiles(const struct Window *window, u32 left, u32 top,
                               u32 right, u32 bottom) {
  struct WindowWork *work;
  u32 x;
  u32 y;
  u32 width;
  u32 height;
  u16 *tiles;
  u32 row;

  right += 7;
  bottom += 7;
  right >>= 3;
  bottom >>= 3;
  x = left >> 3;
  work = WINDOW_WORK;
  y = top >> 3;
  x += window->x;
  y += window->y;
  right += window->x;
  bottom += window->y;
  left = x + 1;
  top = y + 1;
  width = right - x;
  height = bottom - y;
  Window_ClearTextTilesInRect(left, top, width, height);
  tiles = work->tilemap + top * 32 + left;
  for (row = 0; row < height; row++) {
    for (left = 0; left < width; left++)
      *tiles++ = WINDOW_TILE_WINDOW_FILL;
    tiles += 32 - width;
  }
  work->dirty = 1;
}

/// Points a list's tail link back at its last sprite, after the list has been
/// edited by hand.
UNUSED void WindowSprite_UpdateListTail(struct WindowSpriteList *list) {
  struct WindowSprite *node = list->head;
  struct WindowSprite **tail = &list->head;

  while (node != NULL) {
    tail = &node->next;
    node = node->next;
  }
  list->tailLink = tail;
}

/// Adds a window sprite to the end of a list; NULL lists are ignored.
void WindowSprite_AppendToList(struct WindowSpriteList *list,
                               struct WindowSprite *node) {
  if (list != NULL) {
    *list->tailLink = node;
    // UB: the new tail is stored as node (next is its first member) through
    // a WindowSprite * view of tailLink, which keeps both stores in one alias
    // set and in this order.
#ifdef UBFIX
    // Store the address of next through the tail link's actual pointer type.
    list->tailLink = &node->next;
#else
    *(struct WindowSprite **)&list->tailLink = node;
#endif
  }
}

/// Frees a window sprite: returns it to the free list, releases its VRAM cache
/// entry unless it is a glyph, and for a portrait frees its palette slot too.
/// Its one caller, WindowSprite_ClearList, has detached the list first: the
/// free list reuses the sprite's link.
void WindowSprite_Release(struct WindowSprite *entry) {
  WindowSprite_ReleaseFree(entry);
  if ((u8)entry->kind != WINDOW_SPRITE_GLYPH) {
    VramCache_FreeEntry((u8)entry->index);
    if ((u8)entry->kind == WINDOW_SPRITE_PORTRAIT) {
      u8 *work = gWindowWork[0];
      s32 offset = entry->oam.palette * 2 + WINDOW_WORK_PALETTE_TABLE_OFFSET;
      *(u16 *)(work + offset) = PORTRAIT_SLOT_EMPTY;
    }
  }
  entry->motion = 0;
}

/// Starts a free text printer typing the decoded text that begins at entry
/// into window, with the pen at pixel (x, y). spacing gives each of four lines
/// an extra gap between characters, in 1/256 pixels (or none for NULL), and is
/// copied, so the caller's array need not outlive the call; flags is kept with
/// the printer (nonzero keeps the window's size from page to page). Returns
/// the printer, or NULL when all three are busy. The printer stays taken
/// until its window closes.
struct TextPrinter *TextPrinter_Add(struct Window *window, s32 entry, s32 x,
                                    s32 y, u16 *spacing, s32 flags) {
  struct TextPrinter *printer = WINDOW_WORK->printers;
  struct TextPrinter *found = NULL;
  u32 i;

  for (i = 0; i != TEXT_PRINTER_COUNT; i++, printer++) {
    if (printer->window == NULL) {
      found = printer;
      break;
    }
  }
  if (found != NULL) {
    // The printer stores pen coordinates in 1/256 pixels.
    found->window = window;
    found->startX = x << 8;
    found->x = x << 8;
    found->y = y << 8;
    found->textIndex = entry;
    found->colour = TEXT_DEFAULT_COLOR;
    found->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
    found->countdown = 0;
    found->outline = TEXT_STYLE_PLAIN;
    found->ended = 0;
    found->flags = flags;
    if (spacing != NULL) {
      for (i = 0; i < TEXT_PRINTER_LINE_COUNT; i++)
        found->spacing[i] = *spacing++;
    } else {
      for (i = 0; i < TEXT_PRINTER_LINE_COUNT; i++)
        found->spacing[i] = 0;
    }
    found->line = 0;
  }
  return found;
}

/// Starts a text printer typing the decoded text at entry into the battle
/// message window. A text printer that already serves a finished window
/// continues below its last line (scrolling up once both lines are used) unless
/// keepPosition is set; BattleMessage_Open passes the battle message window's
/// marked flag there. Returns the printer, or NULL when all three are busy.
struct TextPrinter *BattleMessage_Continue(struct Window *window, s32 entry,
                                           s32 keepPosition) {
  struct TextPrinter *printer;
  struct TextPrinter *selected;
  u16 *spacing;
  u16 zero;
  u32 index;

  printer = WINDOW_WORK->printers;
  selected = 0;
  for (index = 0; index != TEXT_PRINTER_COUNT; printer++, index++) {
    if (printer->window == 0 || printer->window->state != 0) {
      selected = printer;
      break;
    }
  }

  if (selected != 0) {
    // Positions are in 1/256 pixel: the first line is 10 pixels down, the
    // second 13 pixels below it, and after the second the text scrolls up.
    if (selected->window == 0) {
      selected->y = 10 << 8;
      selected->window = window;
      selected->x = 3 << 8;
    } else if (keepPosition == 0) {
      if (selected->y == 0) {
        selected->y = 10 << 8;
      } else if (selected->y < 13 << 8) {
        selected->y += 13 << 8;
      } else {
        Text_StartCanvasScroll(selected);
      }
      selected->x = 3 << 8;
    }

    zero = 0;
    selected->startX = 3 << 8;
    selected->window->state = zero;
    // Start the next text run without retaining the previous run's style.
    selected->colour = TEXT_DEFAULT_COLOR;
    selected->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
    selected->textIndex = entry;
    selected->countdown = zero;
    selected->outline = zero;
    selected->line = zero;
    selected->ended = zero;
    index = 0;
    spacing = selected->spacing;
    do {
      index++;
      *spacing++ = zero;
    } while (index < TEXT_PRINTER_LINE_COUNT);
  }
  return selected;
}

/// Clears the text canvas to transparent.
s32 Text_ClearCanvas(void) {
  return Iwram_FillWords(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE, 0);
}

/// Fills the text canvas with the window background colour (4).
s32 Text_FillCanvasBackground(void) {
  return Iwram_FillWords(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE,
                         TEXT_CANVAS_BACKGROUND_WORD);
}

/// Resets the pen and text style of the first idle text printer, clearing
/// the canvas when the printer still has a window. The auto battle
/// (battle/battle/auto_battle.c) calls it after playing each action, so the
/// next battle message starts on a clear canvas at its first line.
void TextPrinter_ResetFree(void) {
  struct TextPrinter *printer = WINDOW_WORK->printers;
  struct TextPrinter *selection = 0;
  s32 i;

  for (i = 0; i != TEXT_PRINTER_COUNT; printer++, i++) {
    if (printer->window == 0 || printer->window->state != 0) {
      selection = printer;
      break;
    }
  }
  if (selection != 0) {
    if (selection->window != 0) {
      Text_ClearCanvas();
      selection->y = 0;
    }
    selection->x = 0;
    selection->countdown = 0;
    selection->colour = TEXT_DEFAULT_COLOR;
    selection->outline = TEXT_STYLE_PLAIN;
    selection->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
  }
}

/// Copies the printer's text colour, outline and line spacing into the shared
/// glyph renderer before that printer draws its next character.
void TextPrinter_ApplyStyle(struct TextPrinter *printer) {
  struct WindowWork *work = WINDOW_WORK;

  work->colour = printer->colour;
  work->outline = printer->outline;
  work->lineSpacing = printer->lineSpacing;
}

/// Starts a text printer scrolling its canvas up a line: 12 pixel rows, 6 a
/// frame over two frames (TextPrinter_Step, Text_ScrollCanvasUp), during which
/// the printer types nothing.
void Text_StartCanvasScroll(struct TextPrinter *printer) {
  printer->transition = 2;
}

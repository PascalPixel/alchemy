/// @file
/// The window system's setup and tilemap upload, the pool of window sprites,
/// opening and closing windows, and starting a message typing into one.
// Part of window.c.

#include "window_internal.h"

void Window_DrawOpened(struct Window *window);
void Window_WaitZoom(void *window);

/// A text tile battle keeps for the bottom border's lower rows: neither taken
/// nor shown, it is never free (WindowState.textTileState).
#define TEXT_TILE_RESERVED 4

/// The tilemap uploads in five bands of four tile rows, each a bit of
/// WindowState.dirty above bit 0, which asks for them all.
#define TILEMAP_BANDS 5
#define TILEMAP_BAND_SIZE (4 * BG_SCREEN_WIDTH * sizeof(u16))
#define TILEMAP_DIRTY_ALL ((1 << (TILEMAP_BANDS + 1)) - 1)

/// Takes the first window sprite from the free list, or returns NULL when every
/// sprite is in use.
struct WindowSprite *WindowSprite_AcquireFree(void) {
  struct WindowState *state = WINDOW_STATE;
  struct WindowSprite *out = state->freeSprites.head;

  if (out != NULL) {
    if (out->next == NULL)
      state->freeSprites.tailLink = &state->freeSprites.head;
    state->freeSprites.head = out->next;
    out->next = NULL;
  }
  return out;
}

/// Returns a window sprite to the end of the free list. A sprite from outside
/// the window system's pool is ignored.
void WindowSprite_ReleaseFree(struct WindowSprite *out) {
  struct WindowState *state = WINDOW_STATE;

  if ((u32)out >= (u32)state->sprites &&
      (u32)out < (u32)&state->freeSprites.head) {
    struct WindowSprite **tail = state->freeSprites.tailLink;

    state->freeSprites.tailLink = &out->next;
    *tail = out;
    out->next = NULL;
  }
}

/// Links all 64 window sprites of the pool into the free list.
void WindowSprite_InitializeFreeList(void) {
  s32 n;
  struct WindowState *state;
  struct WindowSprite *out;
  struct WindowSprite *next;

  state = WINDOW_STATE;
  out = state->sprites;
  state->freeSprites.head = out;
  n = WINDOW_SPRITE_COUNT - 2;
  do {
    next = out + 1;
    n--;
    out->next = next;
    out = next;
  } while (n >= 0);
  next->next = NULL;
  state->freeSprites.tailLink = &next->next;
}

/// Sets up the window system for the field (and the other screens outside
/// battle): allocates and clears its state, blanks the tilemap, frees
/// every window sprite and starts the two per-frame tasks, the tilemap upload
/// and the window update.
void WindowSystem_InitField(void) {
  struct WindowState *state;

  state = (struct WindowState *)HeapSlot_AllocEwram(HEAP_SLOT_WINDOW,
                                                    sizeof(struct WindowState));
  Iwram_ClearWordsEntry(state, sizeof(struct WindowState));
  state->dirty = 1;
  state->glyphResource = NO_GLYPH_RESOURCE;
  state->textPalette = WINDOW_PALETTE;
  Iwram_FillWords(state->tilemap, sizeof(state->tilemap),
                  WINDOW_TILE_PAIR(WINDOW_TILE_BLANK));
  WindowSprite_InitializeFreeList();
  Portrait_ClearSlots();
  Task_Add((s32)WindowSystem_UploadTilemap, TASK_PASS_VBLANK);
  WindowSystem_StartFieldUpdate();
}

/// Sets up the window system for battle, as WindowSystem_InitField does for the
/// field, starting the field's window update when loadGlyphTiles is set
/// (WindowSystem_StartBattleUpdate).
void WindowSystem_InitBattle(s32 loadGlyphTiles) {
  struct WindowState *state;
  s32 i;
  // Copies pixel rows 4 to 6 of tile sourceTile to rows 0 to 2 of
  // destinationTile and clears the rest of it.
  s32 WindowSystem_CopyTileLowerRows(u32 sourceTile, u32 destinationTile) {
    sourceTile &= TILE_NUMBER_MASK;
    destinationTile &= TILE_NUMBER_MASK;
    Dma_Set(BG_CHAR_BLOCK(0) + sourceTile * TILE_SIZE_4BPP +
                4 * TILE_ROW_SIZE_4BPP,
            BG_CHAR_BLOCK(0) + destinationTile * TILE_SIZE_4BPP,
            DMA_COPY_HALFWORDS(4 * TILE_ROW_SIZE_4BPP / 2), REG_DMA3);
    return Iwram_ClearWordsEntry(BG_CHAR_BLOCK(0) +
                                     destinationTile * TILE_SIZE_4BPP +
                                     3 * TILE_ROW_SIZE_4BPP,
                                 5 * TILE_ROW_SIZE_4BPP);
  }

  state = (struct WindowState *)HeapSlot_AllocEwram(HEAP_SLOT_WINDOW,
                                                    sizeof(struct WindowState));
  Iwram_ClearWordsEntry(state, sizeof(struct WindowState));
  state->dirty = 1;
  state->glyphResource = NO_GLYPH_RESOURCE;
  state->battleMode = WINDOW_BATTLE;
  state->textPalette = WINDOW_PALETTE;
  Iwram_FillWordsEntry(state->tilemap, sizeof(state->tilemap),
                       WINDOW_TILE_PAIR(WINDOW_TILE_BLANK));
  WindowSprite_InitializeFreeList();
  Task_Add((s32)WindowSystem_UploadTilemap, TASK_PASS_VBLANK);
  WindowSystem_StartBattleUpdate(loadGlyphTiles);
  // The first three text tiles take the lower rows of the bottom border
  // tiles, and are kept from text.
  WindowSystem_CopyTileLowerRows(WINDOW_TILE_BORDER_BOTTOM_LEFT,
                                 TEXT_TILE_FIRST);
  WindowSystem_CopyTileLowerRows(WINDOW_TILE_BORDER_BOTTOM,
                                 TEXT_TILE_FIRST + 1);
  WindowSystem_CopyTileLowerRows(WINDOW_TILE_BORDER_BOTTOM_RIGHT,
                                 TEXT_TILE_FIRST + 2);
  {
    u8 mode = TEXT_TILE_RESERVED;

    for (i = 0; i < 3; i++)
      state->textTileState[i] = mode;
  }
}

/// The tilemap upload, a VBlank-pass task: copies each band of the tilemap
/// marked in the dirty bits to VRAM.
void WindowSystem_UploadTilemap(void) {
  struct WindowState *state = WINDOW_STATE;
  u32 bands;
  u8 *src;
  u8 *dst;

  bands = state->dirty;
  if (bands != 0) {
    if (!state->holdTilemapUpload) {
      src = (u8 *)state->tilemap;
      dst = (u8 *)BG_SCREEN_BLOCK(4);
      // Bit 0 asks for all five bands; after the shift bit n is band n.
      if (bands & 1)
        bands = TILEMAP_DIRTY_ALL;
      bands &= TILEMAP_DIRTY_ALL;
      bands >>= 1;
      do {
        if (bands & 1)
          Dma_Set(src, dst, DMA_COPY_WORDS(TILEMAP_BAND_SIZE / 4), REG_DMA3);
        bands >>= 1;
        src += TILEMAP_BAND_SIZE;
        dst += TILEMAP_BAND_SIZE;
      } while (bands);
      state->dirty = bands;
    }
  }
}

/// Erases a window's rectangle of the tilemap (x, y, width and height in tiles)
/// and frees the text tiles it showed; in battle the rows of the command window
/// go back to its panel.
void Window_EraseBorderRect(s32 x, s32 y, u32 width, u32 height) {
  struct WindowState *canvas = WINDOW_STATE;
  u16 *cursor = &canvas->tilemap[y][x];
  s32 tile;
  u32 bottom;
  u32 row;

  tile = WINDOW_TILE_BLANK;
  bottom = y + height;
  if (bottom > DISPLAY_TILE_HEIGHT)
    height = DISPLAY_TILE_HEIGHT - y;
  if (width <= 1)
    width = 2;
  if (width > DISPLAY_TILE_WIDTH)
    width = DISPLAY_TILE_WIDTH;
  if (height <= 1)
    height = 2;
  if (height > 30)
    height = 30;

  Window_ClearTextTilesInRect(x, y, width, height);

  row = 0;
  if (row < height) {
    do {
      u32 column;

      if (canvas->battleMode != 0) {
        if ((u32)(y + row) >= COMMAND_ROW_TOP)
          tile = WINDOW_TILE_COMMAND_PANEL;
        else
          tile = WINDOW_TILE_BLANK;
      }
      column = 0;
      if (column < width) {
        for (; column < width; column++) {
          *cursor++ = tile;
        }
      }
      row++;
      cursor += BG_SCREEN_WIDTH - width;
    } while (row < height);
  }
  canvas->dirty = 1;
}

/// Draws a window at its full size: its border, and for a text canvas window
/// the canvas, filled with colour 4 under a border when the window is bordered
/// and transparent otherwise.
void Window_DrawOpened(struct Window *window) {
  u32 flags;
  s32 x;
  s32 y;
  u32 width;
  u32 height;
  struct WindowState *state;

  state = WINDOW_STATE;
  x = (s16)window->x;
  y = (s16)window->y;
  width = window->width;
  height = window->height;
  flags = window->flags;
  window->duration = 0;
  if (WINDOW_FLAG_TEXT_CANVAS & flags) {
    if (WINDOW_FLAG_BORDERED_CANVAS & flags) {
      Window_DrawBorder(x, y, width, height);
      Iwram_FillWords(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE,
                      TEXT_CANVAS_BACKGROUND_WORD);
    } else {
      Iwram_FillWords(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE, 0);
    }
    Window_MapTextCanvasTiles(x, y, width, height, 0);
  } else {
    Window_DrawBorder(x, y, width, height);
  }
  state->dirty = 1;
}

/// Opens a window at tile (x, y) of width by height tiles in the first free
/// window slot, and returns it, or NULL when all slots are in use. attrs takes
/// the WINDOW_FLAG_ flags.
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs) {
  struct Window *slot;
  struct Window *found;
  s32 i;

  slot = WINDOW_STATE->windows;
  found = 0;
  for (i = 0; i != WINDOW_COUNT; i++, slot++) {
    if ((slot->flags & WINDOW_FLAG_OPEN) == 0 && slot->duration == 0) {
      found = slot;
      break;
    }
  }
  if (found != 0) {
    found->x = x;
    found->y = y;
    found->width = width;
    found->height = height;
    found->sprites.head = NULL;
    found->state = 0;
    found->sprites.tailLink = &slot->sprites.head;
    found->opened = 1;
    found->flags = WINDOW_FLAG_OPEN;
    Text_ResetStyle();
    if (attrs & WINDOW_FLAG_TEXT_CANVAS) {
      found->flags |= WINDOW_FLAG_TEXT_CANVAS;
    }
    if (attrs & WINDOW_FLAG_BORDERED_CANVAS) {
      found->flags |= WINDOW_FLAG_BORDERED_CANVAS;
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
      found->zoomStage = 0;
      found->duration = 1;
      Window_DrawOpened(found);
    } else {
      found->duration = WINDOW_OPEN_FRAMES;
      found->zoomStage = WINDOW_OPEN_FRAMES - 1;
      Window_WaitZoom(found);
      wait(1);
    }
  }
  return found;
}

/// Waits until a zooming window has finished opening or closing.
void Window_WaitZoom(void *window) {
  if (!(WINDOW_FLAG_INSTANT & ((struct Window *)window)->flags) &&
      (((struct Window *)window)->duration != 0)) {
    do {
      wait(1);
    } while (((struct Window *)window)->duration != 0);
  }
}

void WindowSprite_PrepareForRedraw(struct Window *window);
void WindowSprite_RedrawSavedRect(struct Window *window);

/// Closes a window, its sprites and contents going at once: with instant set
/// (callers pass WINDOW_FLAG_INSTANT or 1) its rectangle is erased and its slot
/// freed now, else its frame shrinks away over WINDOW_CLOSE_FRAMES.
void Window_Close(struct Window *window, s32 instant) {
  if (window == 0)
    return;

  WindowSprite_PrepareForRedraw(window);
  window->flags = 0;
  window->previousX = window->x;
  window->previousY = window->y;
  window->previousWidth = window->width;
  window->previousHeight = window->height;

  if (instant != 0) {
    Window_EraseBorderRect((s16)window->x, (s16)window->y, window->width,
                           window->height);
    window->sprites.head = NULL;
    window->sprites.tailLink = NULL;
    window->width = 0;
    window->height = 0;
    window->x = 0;
    window->y = 0;
    window->opened = 0;
    window->animation = 0;
    window->state = 0;
    window->flags = 0;
    window->zoomStage = 0;
    window->duration = 0;
    window->previousX = 0;
    window->previousY = 0;
    window->previousWidth = 0;
    window->previousHeight = 0;
  } else {
    window->zoomStage = instant;
    window->duration = WINDOW_CLOSE_FRAMES;
  }
}

/// Clears a window's contents: redraws it empty and releases the
/// window sprites it carries. A text canvas window is left alone.
void WindowSprite_PrepareForRedraw(struct Window *window) {
  if (!(WINDOW_FLAG_TEXT_CANVAS & window->flags)) {
    WindowSprite_RedrawSavedRect(window);
    WindowSprite_ClearList(window);
  }
}

/// Redraws a window empty, its border and fill, at its current rectangle.
void WindowSprite_RedrawSavedRect(struct Window *window) {
  Window_DrawBorder((s16)window->x, (s16)window->y, window->width,
                    window->height);
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
/// and bottom are pixels inside the window, and the area covers every tile they
/// touch.
void Window_ClearInteriorTiles(const struct Window *window, u32 left, u32 top,
                               u32 right, u32 bottom) {
  struct WindowState *state;
  u32 x;
  u32 y;
  u32 width;
  u32 height;
  u16 *tiles;
  u32 row;
  s32 windowX;
  s32 windowY;

  state = WINDOW_STATE;
  x = left >> 3;
  windowX = (s16)window->x;
  y = top >> 3;
  windowY = (s16)window->y;
  right += 7;
  bottom += 7;
  right >>= 3;
  bottom >>= 3;
  y += windowY;
  x += windowX;
  right += windowX;
  bottom += windowY;
  left = x + 1;
  top = y + 1;
  width = right - x;
  height = bottom - y;
  // UB: Window_ClearTextTilesInRect returns nothing; this call goes through a
  // function type that returns s32.
#ifdef UBFIX
  Window_ClearTextTilesInRect(left, top, width, height);
#else
  ((s32 (*)(s32, s32, u32, u32))Window_ClearTextTilesInRect)(left, top, width,
                                                             height);
#endif
  tiles = state->tilemap[0] + top * BG_SCREEN_WIDTH + left;
  for (row = 0; row < height; row++) {
    for (left = 0; left < width; left++)
      *tiles++ = WINDOW_TILE_WINDOW_FILL;
    tiles += BG_SCREEN_WIDTH - width;
  }
  state->dirty = 1;
}

/// Points a list's tail link back at its last sprite, after the list has been
/// edited by hand.
void WindowSprite_UpdateListTail(struct WindowSpriteList *list) {
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
    // a WindowSprite * view of tailLink.
#ifdef UBFIX
    list->tailLink = &node->next;
#else
    *(struct WindowSprite **)&list->tailLink = node;
#endif
  }
}

/// Frees a window sprite: returns it to the free list, releases its VRAM cache
/// entry unless it is a glyph, and for a portrait frees its palette slot too.
void WindowSprite_Release(struct WindowSprite *entry) {
  WindowSprite_ReleaseFree(entry);
  if ((u8)entry->kind != WINDOW_SPRITE_PLAIN) {
    VramCache_FreeEntry((u8)entry->index);
    if ((u8)entry->kind == WINDOW_SPRITE_PORTRAIT) {
      WINDOW_STATE
          ->portraitPictures[entry->sprite.oam.palette - PORTRAIT_PALETTE] =
          PORTRAIT_SLOT_EMPTY;
    }
  }
  entry->motion = 0;
}

/// Starts a free message typing the decoded text that begins at entry into
/// window, with the pen at pixel (x, y). spacing gives each of four lines an
/// extra gap between characters, in 1/256 pixels (or none for NULL), and is
/// copied, so the caller's array need not outlive the call; flags is kept with
/// the message (nonzero keeps the window's size from page to page).
struct Message *Message_Start(struct Window *window, s32 entry, s32 x, s32 y,
                              u16 *spacing, s32 flags) {
  struct WindowState *state = WINDOW_STATE;
  struct Message *render = state->messages;
  struct Message *found = NULL;
  u32 i;

  for (i = 0; i != MESSAGE_COUNT; i++, render++) {
    if (render->window == NULL) {
      found = render;
      break;
    }
  }
  if (found != NULL) {
    found->window = window;
    found->startX = x << 8;
    found->x = x << 8;
    found->y = y << 8;
    found->textIndex = entry;
    found->colour = TEXT_COLOR_WHITE;
    found->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
    found->countdown = 0;
    found->outline = TEXT_STYLE_PLAIN;
    found->ended = 0;
    found->compressLines = 0;
    found->flags = flags;
    if (state->textMode != 0 && spacing != NULL)
      found->compressLines = spacing[3];
    if (spacing != NULL) {
      for (i = 0; i < MESSAGE_LINE_COUNT; i++)
        found->spacing[i] = *spacing++;
    } else {
      for (i = 0; i < MESSAGE_LINE_COUNT; i++)
        found->spacing[i] = 0;
    }
    found->line = 0;
  }
  return found;
}

void Text_StartCanvasScroll(struct Message *message);

/// Starts a message typing the decoded text at entry into the battle message
/// window. A message that already serves a typed-out window continues below
/// its last line (scrolling up once both lines are used) unless keepPosition
/// is set. Returns the message, or NULL when all three are busy.
struct Message *BattleMessage_Continue(struct Window *window, s32 entry,
                                       s32 keepPosition) {
  struct Message *slot;
  struct Message *selected;
  u16 *spacing;
  u16 lineSpacing;
  u32 index;

  slot = WINDOW_STATE->messages;
  selected = 0;
  for (index = 0; index != MESSAGE_COUNT; slot++, index++) {
    if (slot->window == 0 || slot->window->state != 0) {
      selected = slot;
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

    selected->startX = 3 << 8;
    selected->window->state = 0;
    // Start the next text run without retaining the previous run's style.
    selected->colour = TEXT_COLOR_WHITE;
    selected->textIndex = entry;
    lineSpacing = TEXT_DEFAULT_LINE_SPACING;
    selected->countdown = 0;
    selected->outline = 0;
    selected->line = 0;
    selected->ended = 0;
    selected->lineSpacing = lineSpacing;
    index = 0;
    spacing = selected->spacing;
    for (; index < MESSAGE_LINE_COUNT; index++) {
      *spacing++ = 0;
    }
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

/// Resets the pen and text style of the first idle message, clearing the canvas
/// when the message still has a window.
void Message_ResetIdle(void) {
  struct Message *slot = WINDOW_STATE->messages;
  struct Message *selection = 0;
  s32 i;

  for (i = 0; i != MESSAGE_COUNT; slot++, i++) {
    if (slot->window == 0 || slot->window->state != 0) {
      selection = slot;
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
    selection->colour = TEXT_COLOR_WHITE;
    selection->outline = TEXT_STYLE_PLAIN;
    selection->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
  }
}

/// Makes the message's text colour, style and line spacing the ones glyphs are
/// drawn with, before it draws its next character.
void Message_ApplyStyle(struct Message *message) {
  struct WindowState *state = WINDOW_STATE;

  state->colour = message->colour;
  state->outline = message->outline;
  state->lineSpacing = message->lineSpacing;
}

/// Starts a message scrolling its canvas up a line: 12 pixel rows, 6 a
/// frame over two frames (Message_Type, Text_ScrollCanvasUp), during which
/// the message types nothing.
void Text_StartCanvasScroll(struct Message *message) {
  message->scrollFrames = 2;
}

/// @file
/// The window sprite pool, opening and closing windows, starting messages.
// Part of window.c.

#include "../window_internal.h"

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowWork)

void Window_DrawOpened(struct Window *window);
void Window_WaitZoom(void *work);

/// Removes and returns the first entry of the free list, or NULL if it is
/// empty.
struct WindowSprite *WindowSprite_AcquireFree(void) {
  struct WindowWork *work = UI_WORK;
  struct WindowSprite *out = work->freeOutputs.head;

  if (out != NULL) {
    if (out->next == NULL)
      work->freeOutputs.tailLink = &work->freeOutputs.head; // Now empty.
    work->freeOutputs.head = out->next;
    out->next = NULL;
  }
  return out;
}

/// Puts an entry back at the free list's tail; entries outside outputs[] are
/// ignored.
void WindowSprite_ReleaseFree(struct WindowSprite *out) {
  struct WindowWork *work = UI_WORK;

  if ((u32)out >= (u32)work->outputs &&
      (u32)out < (u32)&work->freeOutputs.head) {
    struct WindowSprite **tail = work->freeOutputs.tailLink;

    work->freeOutputs.tailLink = &out->next;
    *tail = out;
    out->next = NULL;
  }
}

/// Link all 64 outputs[] entries into the free list.
void WindowSprite_InitializeFreeList(void) {
  s32 n;
  struct WindowWork *work;
  struct WindowSprite *out;
  struct WindowSprite *next;

  work = UI_WORK;
  out = work->outputs;
  work->freeOutputs.head = out;
  n = UI_OUTPUT_COUNT - 2;
  do {
    next = out + 1;
    n--;
    out->next = next;
    out = next;
  } while (n >= 0);
  next->next = NULL;
  work->freeOutputs.tailLink = &next->next;
}

typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size,
                                u32 value) {
  /* FAKEMATCH(wrapper): the fill called through this inline. */
  return fill(dst, size, value);
}

void WindowSystem_InitField(void) {
  struct WindowWork *work;

  work = (struct WindowWork *)HeapSlot_AllocEwram(HEAP_SLOT_WINDOW,
                                                  sizeof(struct WindowWork));
  Iwram_ClearWordsEntry(work, sizeof(struct WindowWork));
  work->dirty = 1;
  work->glyphResource = 99;
  work->textPalette = 15;
  FillWords(Iwram_FillWordsEntry, work->tilemap, sizeof(work->tilemap),
            WINDOW_TILE_PAIR(WINDOW_TILE_BLANK));
  WindowSprite_InitializeFreeList();
  Portrait_ClearSlots();
  Task_Add((s32)WindowSystem_UploadTilemap, TASK_PASS_FRAME);
  WindowSystem_StartFieldUpdate();
}

void WindowSystem_InitBattle(s32 kind) {
  struct WindowWork *work;
  s32 i;
  s32 WindowSystem_CopyTileLowerRows(u32 src, u32 dst) {
    src &= 0x3ff;
    dst &= 0x3ff;
    Dma_Set((void *)(0x06000010 + src * 32), (void *)(0x06000000 + dst * 32),
            DMA_COPY_HALFWORDS(8), REG_DMA3);
    return Iwram_ClearWordsEntry((void *)(0x0600000c + dst * 32), 20);
  }

  work = (struct WindowWork *)HeapSlot_AllocEwram(HEAP_SLOT_WINDOW,
                                                  sizeof(struct WindowWork));
  Iwram_ClearWordsEntry(work, sizeof(struct WindowWork));
  work->dirty = 1;
  work->glyphResource = 99;
  work->battleMode = 1;
  work->textPalette = 15;
  Iwram_FillWordsEntry(work->tilemap, sizeof(work->tilemap),
                       WINDOW_TILE_PAIR(WINDOW_TILE_BLANK));
  WindowSprite_InitializeFreeList();
  Task_Add((s32)WindowSystem_UploadTilemap, TASK_PASS_FRAME);
  WindowSystem_StartBattleUpdate(kind);
  WindowSystem_CopyTileLowerRows(WINDOW_TILE_BORDER_BOTTOM_LEFT, 128);
  WindowSystem_CopyTileLowerRows(WINDOW_TILE_BORDER_BOTTOM, 129);
  WindowSystem_CopyTileLowerRows(WINDOW_TILE_BORDER_BOTTOM_RIGHT, 130);
  {
    u8 mode = 4;

    for (i = 2; i >= 0; i--)
      work->textTileState[i] = mode;
  }
}

void WindowSystem_UploadTilemap(void) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  u32 flags;
  u8 *src;
  u8 *dst;

  flags = work->dirty;
  if (flags != 0) {
    if (!work->holdTilemapUpload) {
      src = (u8 *)work->tilemap;
      dst = (u8 *)BG_SCREEN_BLOCK(4);
      if (flags & 1)
        flags = 63;
      flags &= 63;
      flags >>= 1;
      do {
        if (flags & 1)
          Dma_Set(src, dst, DMA_COPY_WORDS(64), REG_DMA3);
        flags >>= 1;
        src += 256;
        dst += 256;
      } while (flags);
      work->dirty = flags;
    }
  }
}

void Window_EraseBorderRect(s32 x, s32 y, u32 width, u32 height) {
  struct WindowWork *canvas = (struct WindowWork *)Ram_HeapSlots->windowWork;
  u16 *cursor = &canvas->tilemap[y][x];
  s32 tile;
  u32 bottom;
  u32 row;

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

  Window_ClearTextTilesInRect(x, y, width, height);

  row = 0;
  if (row < height) {
    do {
      u32 column;

      if (canvas->battleMode != 0) {
        if ((u32)(y + row) > 16)
          tile = WINDOW_TILE_COMMAND_PANEL;
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

/// Draws a window at its full size: its border, and for a text canvas window
/// the canvas, filled with colour 4 under a border when the window is bordered
/// and transparent otherwise.
void Window_DrawOpened(struct Window *window) {
  u32 flags;
  s32 x;
  s32 y;
  u32 width;
  u32 height;
  struct WindowWork *work;

  work = UI_WORK;
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
  work->dirty = 1;
}

/// Opens a window at tile (x, y) of width by height tiles in the first free
/// window slot, and returns it, or NULL when all slots are in use. attrs takes
/// the WINDOW_FLAG_ flags.
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs) {
  struct Window *slot;
  struct Window *found;
  s32 i;

  slot = UI_WORK->windows;
  found = 0;
  for (i = 0; i != UI_WINDOW_COUNT; i++, slot++) {
    if ((slot->flags & WINDOW_FLAG_OPEN) == 0 && slot->duration == 0) {
      found = slot;
      break;
    }
  }
  if (found != 0) {
    found->y = y;
    found->width = width;
    found->height = height;
    found->x = x;
    found->sprites.head = NULL;
    found->state = 0;
    found->sprites.tailLink = &slot->sprites.head;
    found->opened = 1;
    found->flags = WINDOW_FLAG_OPEN;
    // Text_ResetStyle takes no arguments: x is passed and ignored.
    Text_ResetStyle(x);
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

void Window_WaitZoom(void *work) {
  if (!(WINDOW_FLAG_INSTANT & ((struct Window *)work)->flags) &&
      (((struct Window *)work)->duration != 0)) {
    do {
      wait(1);
    } while (((struct Window *)work)->duration != 0);
  }
}

void WindowSprite_PrepareForRedraw(struct Window *work);
void WindowSprite_RedrawSavedRect(struct Window *window);

void Window_Close(struct Window *work, s32 release) {
  u16 zero;

  if (work == 0)
    return;

  WindowSprite_PrepareForRedraw(work);
  zero = 0;
  work->flags = zero;
  work->previousX = work->x;
  work->previousY = work->y;
  work->previousWidth = work->width;
  work->previousHeight = work->height;

  if (release != 0) {
    Window_EraseBorderRect((s16)work->x, (s16)work->y, work->width,
                           work->height);
    work->sprites.head = NULL;
    work->sprites.tailLink = NULL;
    work->width = zero;
    work->height = zero;
    work->x = zero;
    work->y = zero;
    work->opened = zero;
    work->animation = zero;
    work->state = zero;
    work->flags = zero;
    work->zoomStage = zero;
    work->duration = zero;
    work->previousX = zero;
    work->previousY = zero;
    work->previousWidth = zero;
    work->previousHeight = zero;
  } else {
    work->zoomStage = release;
    work->duration = WINDOW_CLOSE_FRAMES;
  }
}

void WindowSprite_PrepareForRedraw(struct Window *work) {
  if (!(WINDOW_FLAG_TEXT_CANVAS & work->flags)) {
    WindowSprite_RedrawSavedRect(work);
    WindowSprite_ClearList(work);
  }
}

void WindowSprite_RedrawSavedRect(struct Window *window) {
  Window_DrawBorder((s16)window->x, (s16)window->y, window->width,
                    window->height);
}

void WindowSprite_ClearList(void *work) {
  struct WindowSpriteList *list = work;
  struct WindowSprite *node;
  struct WindowSprite *next;

  next = NULL;
  if (list != NULL) {
    node = list->head;
    list->tailLink = &list->head;
    list->head = next;
    while (node != NULL) {
      next = node->next;
      WindowSprite_Release(node);
      node = next;
    }
  }
}

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowWork)

// Clears an area of a window's interior to the window fill. left, top, right
// and bottom are pixels inside the window, and the area covers every tile
// they touch.
void Window_ClearInteriorTiles(const struct Window *window, u32 left, u32 top,
                               u32 right, u32 bottom) {
  struct WindowWork *work;
  u32 x;
  u32 y;
  u32 width;
  u32 height;
  u16 *tiles;
  u32 row;
  s32 windowX;
  s32 windowY;

  work = UI_WORK;
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
  tiles = work->tilemap[0] + top * 32 + left;
  for (row = 0; row < height; row++) {
    for (left = 0; left < width; left++)
      *tiles++ = WINDOW_TILE_WINDOW_FILL;
    tiles += 32 - width;
  }
  work->dirty = 1;
}

#if (LANGUAGE != LANGUAGE_JAPANESE)
#define UI_PALETTE_OWNER_OFFSET 0x1360
#else
#define UI_PALETTE_OWNER_OFFSET 0x1160
#endif

void WindowSprite_UpdateListTail(struct WindowSpriteList *list) {
  struct WindowSprite *node = list->head;
  struct WindowSprite **tail = &list->head;

  while (node != NULL) {
    tail = &node->next;
    node = node->next;
  }
  list->tailLink = tail;
}

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

void WindowSprite_Release(struct WindowSprite *entry) {
  WindowSprite_ReleaseFree(entry);
  if ((u8)entry->kind != 0) {
    VramCache_FreeEntry((u8)entry->index);
    if ((u8)entry->kind == 2) {
      u8 *dst = (u8 *)Ram_HeapSlots->windowWork;
      s32 idx = entry->sprite.oam.palette * 2 + UI_PALETTE_OWNER_OFFSET;
      *(u16 *)(dst + idx) = 0x3E7;
    }
  }
  entry->motion = 0;
}

struct UiChannelSlot *Message_Start(struct Window *window, s32 entry, s32 x,
                                    s32 y, u16 *spacing, s32 flags) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  struct UiChannelSlot *render = work->channels;
  struct UiChannelSlot *found = NULL;
  u32 i;

  for (i = 0; i != 3; i++, render++) {
    if (render->work == NULL) {
      found = render;
      break;
    }
  }
  if (found != NULL) {
    found->work = window;
    found->startX = x << 8;
    found->x = x << 8;
    found->y = y << 8;
    found->entry = entry;
    found->colour = 15;
    found->lineSpacing = 10;
    found->countdown = 0;
    found->outline = 0;
    found->ended = 0;
    found->unk26 = 0;
    found->flags = flags;
    if (work->textMode != 0 && spacing != NULL)
      found->unk26 = spacing[3];
    if (spacing != NULL) {
      for (i = 0; i < 4; i++)
        found->spacing[i] = *spacing++;
    } else {
      for (i = 0; i < 4; i++)
        found->spacing[i] = 0;
    }
    found->line = 0;
  }
  return found;
}

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowWork)

void Text_StartCanvasScroll(struct UiChannelSlot *channel);

/// Starts a channel typing the decoded text at entry into the battle message
/// window. A channel that already serves a finished window continues below
/// its last line (scrolling up once both lines are used) unless keepPosition
/// is set. Returns the channel, or NULL when all three are busy.
struct UiChannelSlot *BattleMessage_Continue(struct Window *window, s32 entry,
                                             s32 keepPosition) {
  struct UiChannelSlot *slot;
  struct UiChannelSlot *selected;
  u16 *spacing;
  u16 zero;
  u16 lineSpacing;
  u32 index;

  slot = UI_WORK->channels;
  selected = 0;
  for (index = 0; index != 3; slot++, index++) {
    if (slot->work == 0 || slot->work->state != 0) {
      selected = slot;
      break;
    }
  }

  if (selected != 0) {
    // Positions are in 1/256 pixel: the first line is 10 pixels down, the
    // second 13 pixels below it, and after the second the text scrolls up.
    if (selected->work == 0) {
      selected->y = 10 << 8;
      selected->work = window;
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
    selected->work->state = zero;
    // Start the next text run without retaining the previous run's style.
    selected->colour = 15;
    selected->entry = entry;
    lineSpacing = 10;
    selected->countdown = zero;
    selected->outline = zero;
    selected->line = zero;
    selected->ended = zero;
    selected->lineSpacing = lineSpacing;
    index = 0;
    spacing = selected->spacing;
    do {
      index++;
      *spacing++ = zero;
    } while (index < 4);
  }
  return selected;
}

/// Clears the text canvas to transparent.
s32 Text_ClearCanvas(void) {
  s32 (*fill)(void *destination, s32 size, u32 value) = Iwram_FillWordsEntry;

  return fill(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE, 0);
}

/// Fills the text canvas with the window background colour (4).
void Text_FillCanvasBackground(void) {
  s32 (*fill)(void *destination, s32 size, u32 value) = Iwram_FillWordsEntry;

  fill(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE, TEXT_CANVAS_BACKGROUND_WORD);
}

void Message_ResetIdle(void) {
  struct UiChannelSlot *slot =
      ((struct WindowWork *)Ram_HeapSlots->windowWork)->channels;
  struct UiChannelSlot *selection = 0;
  s32 i;

  for (i = 0; i != 3; slot++, i++) {
    if (slot->work == 0 || slot->work->state != 0) {
      selection = slot;
      break;
    }
  }
  if (selection != 0) {
    if (selection->work != 0) {
      Text_ClearCanvas();
      selection->y = 0;
    }
    selection->x = 0;
    selection->countdown = 0;
    selection->colour = 0xF;
    selection->outline = 0;
    selection->lineSpacing = 0xA;
  }
}

// Copies the channel's text colour, outline and line spacing into the shared
// glyph renderer before that channel draws its next character.
/// Copies the message's text colour, outline and line spacing into the shared
/// glyph renderer before that message draws its next character.
void Message_ApplyStyle(struct UiChannelSlot *channel) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;

  work->colour = channel->colour;
  work->outline = channel->outline;
  work->lineSpacing = channel->lineSpacing;
}

/// Starts a message scrolling its canvas up a line: 12 pixel rows, 6 a
/// frame over two frames (Message_Type, Text_ScrollCanvasUp), during which
/// the message types nothing.
void Text_StartCanvasScroll(struct UiChannelSlot *channel) {
  channel->transition = 2;
}

#undef UI_WORK
#undef UI_PALETTE_OWNER_OFFSET

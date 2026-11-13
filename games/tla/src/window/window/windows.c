/// @file
/// The window sprite pool, opening and closing windows, starting messages.
// Part of window.c.

#include "../window_internal.h"

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

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
  /* FAKEMATCH(wrapper): the fill called through this inline, whose entry
   * comes in as a parameter, so the call loads it before the fill value; as a
   * direct call, 2 of 38 instructions of WindowSystem_InitField differ in
   * tla-en. */
  return fill(dst, size, value);
}

void WindowSystem_InitField(void) {
  struct WindowWork *work;

  work = (struct WindowWork *)HeapSlot_AllocEwram(
      60, sizeof(struct WindowWork));
  Iwram_ClearWords(work, sizeof(struct WindowWork));
  work->dirty = 1;
  work->glyphResource = 99;
  work->level = 15;
  FillWords(Iwram_FillWords, work->tilemap, sizeof(work->tilemap), 0xf000f000);
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
    return Iwram_ClearWords((void *)(0x0600000c + dst * 32), 20);
  }

  work = (struct WindowWork *)HeapSlot_AllocEwram(
      60, sizeof(struct WindowWork));
  Iwram_ClearWords(work, sizeof(struct WindowWork));
  work->dirty = 1;
  work->glyphResource = 99;
  work->menuState = 1;
  work->level = 15;
  Iwram_FillWords(work->tilemap, sizeof(work->tilemap), 0xf000f000);
  WindowSprite_InitializeFreeList();
  Task_Add((s32)WindowSystem_UploadTilemap, TASK_PASS_FRAME);
  WindowSystem_StartBattleUpdate(kind);
  WindowSystem_CopyTileLowerRows(0xf013, 128);
  WindowSystem_CopyTileLowerRows(0xf014, 129);
  WindowSystem_CopyTileLowerRows(0xf015, 130);
  {
    u8 mode = 4;

    for (i = 2; i >= 0; i--)
      work->tileAttributes[i] = mode;
  }
}

void WindowSystem_UploadTilemap(void) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 flags;
  u8 *src;
  u8 *dst;

  flags = work->dirty;
  if (flags != 0) {
    if (!work->menuBusy) {
      src = (u8 *)work->tilemap;
      dst = (u8 *)0x06002000;
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
  struct WindowWork *canvas =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
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

      if (canvas->menuState != 0) {
        if ((u32)(y + row) > 16)
          tile = 0xF07F;
        else
          tile = 0xF000;
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

static inline void Window_FillWords(void *destination, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): the call through this inline; as direct calls to the
   * fixed entry, 10 of 68 instructions of Window_DrawOpened differ in tla-en.
   */
  Iwram_FillWordsEntry(destination, size, value);
}

/// Draws a window as its flags ask: with flag 8 its inside is filled (with
/// colour 4 under flag 0x20, which also draws the border, else cleared), and
/// without it only the border is drawn. Clears the window's duration.
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
  if (8 & flags) {
    if (0x20 & flags) {
      Window_DrawBorder(x, y, width, height);
      Window_FillWords((void *)0x06002500, 0xF00, 0x44444444);
    } else {
      Window_FillWords((void *)0x06002500, 0xF00, 0);
    }
    Window_MapTextCanvasTiles(x, y, width, height, 0);
  } else {
    Window_DrawBorder(x, y, width, height);
  }
  work->dirty = 1;
}

/// Opens the first free window at x, y with width and height; attrs bits 8,
/// 32, 64, 128 and 0x100 become window flags, and bit 2 draws it at once
/// instead of waiting for it to open.
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 attrs) {
  struct Window *slot;
  struct Window *found;
  s32 i;

  slot = UI_WORK->windows;
  found = 0;
  for (i = 0; i != UI_WINDOW_COUNT; i++, slot++) {
    if ((slot->flags & 1) == 0 && slot->duration == 0) {
      found = slot;
      break;
    }
  }
  if (found != 0) {
    found->y = y;
    found->width = width;
    found->height = height;
    found->x = x;
    found->output.head = NULL;
    found->state = 0;
    found->output.tailLink = &slot->output.head;
    found->unk10 = 1;
    found->flags = 1;
    // Text_ResetStyle takes no arguments: x is passed and ignored.
    Text_ResetStyle(x);
    if (attrs & 8) {
      found->flags |= 8;
    }
    if (attrs & 32) {
      found->flags |= 32;
    }
    if (attrs & 64) {
      found->flags |= 64;
    }
    if (attrs & 128) {
      found->flags |= 128;
    }
    if (attrs & 0x100) {
      found->flags |= 0x100;
    }
    if (attrs & 2) {
      found->flags |= 2;
      found->frame = 0;
      found->duration = 1;
      Window_DrawOpened(found);
    } else {
      found->duration = 8;
      found->frame = 7;
      Window_WaitZoom(found);
      wait(1);
    }
  }
  return found;
}

void Window_WaitZoom(void *work) {
  if (!(2 & ((struct Window *)work)->flags) &&
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
    work->output.head = NULL;
    work->output.tailLink = NULL;
    work->width = zero;
    work->height = zero;
    work->x = zero;
    work->y = zero;
    work->unk10 = zero;
    work->effect = zero;
    work->state = zero;
    work->flags = zero;
    work->frame = zero;
    work->duration = zero;
    work->previousX = zero;
    work->previousY = zero;
    work->previousWidth = zero;
    work->previousHeight = zero;
  } else {
    work->frame = release;
    work->duration = 4;
  }
}

void WindowSprite_PrepareForRedraw(struct Window *work) {
  if (!(8 & work->flags)) {
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

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

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
  /* FAKEMATCH(signature): Window_ClearTextTilesInRect called as returning
   * s32, though it returns nothing, so the call sets r0 and the scheduler
   * sets r1 and r2 before r0; as void, 2 of 69 instructions differ in
   * tla-en. */
  ((s32(*)(s32, s32, u32, u32))Window_ClearTextTilesInRect)(left, top, width,
                                                            height);
  tiles = work->tilemap[0] + top * 32 + left;
  for (row = 0; row < height; row++) {
    for (left = 0; left < width; left++)
      *tiles++ = 0xf020;
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
    Resource_ResetEntry((u8)entry->index);
    if ((u8)entry->kind == 2) {
      u8 *dst = (u8 *)Ram_HeapSlots->windowTiles;
      s32 idx = entry->sprite.oam.palette * 2 + UI_PALETTE_OWNER_OFFSET;
      *(u16 *)(dst + idx) = 0x3E7;
    }
  }
  entry->active = 0;
}

struct UiChannelSlot *Message_Start(struct Window *window, s32 entry, s32 x,
                                    s32 y, u16 *spacing, s32 flags) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
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

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

void Func_08039754(struct UiChannelSlot *channel);

/// Starts a channel typing the decoded text at entry into the battle message
/// window. A channel that already serves a finished window continues below
/// its last line (scrolling up once both lines are used) unless keepPosition
/// is set. Returns the channel, or NULL when all three are busy.
// As ☀️'s BattleMessage_Continue.
struct UiChannelSlot *BattleMessage_Continue(struct Window *window,
                                                s32 entry, s32 keepPosition) {
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
        Func_08039754(selected);
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

s32 Vram_ClearWindowBlock(void) {
  s32 (*fill)(void *destination, s32 size, u32 value) = Iwram_FillWords;

  return fill((void *)0x06002500, 0xf00, 0);
}

void Vram_FillWindowBlockPattern(void) {
  s32 (*fill)(void *destination, s32 size, u32 value) = Iwram_FillWords;

  fill((void *)0x06002500, 0xf00, 0x44444444);
}

void Message_ResetIdle(void) {
  struct UiChannelSlot *slot =
      ((struct WindowWork *)Ram_HeapSlots->windowTiles)->channels;
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
      Vram_ClearWindowBlock();
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
void Func_0803972c(struct UiChannelSlot *channel) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;

  work->colour = channel->colour;
  work->outline = channel->outline;
  work->lineSpacing = channel->lineSpacing;
}

void Func_08039754(struct UiChannelSlot *channel) {
  channel->transition = 2;
}

#undef UI_WORK
#undef UI_PALETTE_OWNER_OFFSET

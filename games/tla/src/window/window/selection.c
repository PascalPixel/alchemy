/// @file
/// The selection screen's row of icons, and the field menu.
// Part of window.c.

#include "../window_internal.h"

// The part's functions its code calls before their definitions, declared
// once as they are defined.
struct MenuSelection;
struct StepMenu;
struct MenuResourceList;
struct ListNode;
s32 SelectionMenu_FindFreeIcon(s32 kind);
void SelectionMenu_LoadLargeIcon(void);
void SelectionMenu_Draw(void);
void SelectionMenu_DrawArrow(struct MenuSelection *state, s32 index);
void SelectionMenu_SetupArrow(struct SelectionScreen *state, s32 index);
void SelectionMenu_StepRight(struct SelectionScreen *state);
void SelectionMenu_StepLeft(struct StepMenu *state);
void SelectionMenu_RedrawIcon(struct MenuResourceList *state, u32 index);
void SelectionMenu_HighlightIcon(struct MenuResourceList *state, u32 index);
void SelectionMenu_Scroll(struct SelectionScreen *screen, s32 forward);
void SelectionMenu_SetupIcon(u32 kind, s32 base, struct ListNode *node,
                             s32 reuse);
u32 Menu_ConfirmSelection(struct SelectionScreen *screen);
void PartyStatus_OpenAndWait(void);

struct GlyphCursor {
  u16 unused;
  u16 cursor;
};

struct GlyphRow {
  u8 unk00[8];
  struct GlyphCursor index;
  u8 unk0c[6];
  u16 state;
  u8 unk14[32];
};

struct GlyphVisual {
  u8 unk00[5];
  u8 flags05_0 : 2;
  u8 flags05_2 : 2;
  u8 flags05_4 : 1;
  u8 flags05_5 : 1;
  u8 flags05_6 : 2;
  u8 unk06;
  u8 flags07_0 : 1;
  u8 flags07_1 : 5;
  u8 flags07_6 : 2;
  u8 unk08;
  u8 flags09_0 : 2;
  u8 flags09_2 : 2;
  u8 flags09_4 : 4;
  u8 unk0a[2];
};

struct GlyphWork {
  struct GlyphRow rows[14];
  u8 unk2d8[10];
  u16 field2e2;
  u16 slot;
  u16 tile;
  u8 unk2e8[18];
  u16 field2fa;
  u8 unk2fc[4];
  struct GlyphVisual visual;
  u8 unk30c[10];
  u16 field316;
  u8 unk318[48];
  s32 field348;
  s32 field34c;
  s32 field350;
  u8 unk354[64];
  u16 field394;
  u8 unk396[4];
  u16 field39a;
  u16 field39c;
  u16 field39e;
  u16 field3a0;
  u8 unk3a2[22];
  u16 field3b8;
  u8 unk3ba[42];
};

extern const u8 gMenu_AnimatedCursorTiles[];

/// Allocates and resets the selection screen (heap slot 72): the row of icons
/// the player picks from, with arrows at each end and a pointing hand over the
/// chosen target, whose picture takes a VRAM cache entry.
void SelectionMenu_Create(void) {
  struct GlyphWork *work;
  struct GlyphVisual *visual;
  const u8 *table;
  s32 i;
  u16 clear = 0;

  work = HeapSlot_AllocEwram(72, 996);
  work->field348 = clear;
  work->field34c = clear;
  work->field350 = clear;
  work->field39a = clear;
  work->field39c = clear;
  work->field39e = 128;
  work->field3a0 = 32;
  work->field394 = clear;
  work->field3b8 = 999;
  i = 0;
  do {
    work->rows[i + 2].index.cursor = 0;
    work->rows[i + 9].index.cursor = 0;
    i++;
  } while (i != 5);
  {
    struct GlyphCursor *cursor = &work->rows[7].index;

    cursor->cursor = 0;
    cursor = &work->rows[8].index;
    cursor->cursor = 0;
  }
  work->rows[0].index.cursor = 0;
  work->rows[1].index.cursor = 0;
  work->rows[0].state = 0;
  work->rows[1].state = 0;
  table = gMenu_AnimatedCursorTiles;
  work->slot = VramCache_FindFreeEntry();
  work->tile = VramCache_Load(work->slot, 256, table);
  work->field2e2 = 0;
  work->field2fa = 0;
  work->field316 = 0;
  visual = &work->visual;
  visual->flags05_2 = 0;
  visual->flags05_4 = 0;
  visual->flags05_5 = 1;
  visual->flags05_0 = 0;
  visual->flags07_1 = 0;
  visual->flags07_6 = 1;
  visual->flags05_6 = 0;
  visual->flags09_2 = 0;
}

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

/// Empties the selection screen's list of entries.
/// The high cursor bit requests resetting both the list origin and cursor.
void SelectionMenu_Clear(void) {
  struct SelectionScreen *screen;

  screen = Ram_HeapSlots->menuWork;
  screen->head = NULL;
  screen->unk39a = 0;
  if (screen->cursorIndex & 0x80) {
    screen->top = 0;
    screen->cursorIndex = 0;
  }
  screen->verticalScroll = 0;
  screen->count = 0;
}

/// Adds an entry (kind and id) to the selection screen's list, up to 16.
/// The count starts at 0 when the screen is created or cleared. The kinds
/// SelectionMenu_SetupIcon draws (enum IconKind) are ICON_KIND_COMMAND,
/// ICON_KIND_YES_NO, ICON_KIND_ITEM and ICON_KIND_ABILITY; base is the
/// command's, item's or ability's number.
void SelectionMenu_AddEntry(u32 first, u32 second) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  u16 cnt = state->count;

  if (cnt != 16) {
    state->kinds[cnt] = first;
    state->bases[cnt] = second;
    state->count++;
  }
}

/// Creates icons for the first five listed entries in a row along the bottom of
/// the screen, then loads the selected entry's preview. Reached through the
/// far-call table and from Menu_RunSelectionForValue.
// As ☀️'s SelectionMenu_BuildEntries.
/// Creates icons for the first five listed entries in a row along the bottom of
/// the screen.
/// Builds the linked row before placing the visible icons and loading the
/// preview.
void SelectionMenu_BuildEntries(u32 unused) {
  struct SelectionScreen *screen = Ram_HeapSlots->menuWork;
  u32 count = screen->count;
  u32 index = screen->top;
  struct SelectionNode *previous = 0;
  struct SelectionNode *node;
  s32 rowPosition = 0;

  while (index < count) {
    s32 base = screen->bases[index];
    u32 kind = screen->kinds[index];

    node = (struct SelectionNode *)SelectionMenu_FindFreeIcon(0);
    if (node == 0)
      break;
    SelectionMenu_SetupIcon(kind, base, (struct ListNode *)node, 0);
    if (screen->head == 0) {
      screen->head = node;
      node->prev = 0;
    } else {
      previous->next = node;
      node->prev = previous;
    }
    node->next = 0;
    rowPosition++;
    previous = node;
    if (rowPosition == 5)
      break;
    index++;
  }

  // Icons are 16 pixels apart: the row is centred on x 100, at y 140.
  screen->baseX = 100 - rowPosition * 8;
  screen->baseY = 140;
  // The count is reused as a pixel offset while the row is placed.
  for (previous = screen->head, rowPosition = 0; previous != 0;
       previous = previous->next) {
    s32 x = screen->baseX + rowPosition;
    s32 y;

    previous->x = x;
    y = screen->baseY;
    previous->y = y;
    previous->targetX = x;
    previous->targetY = y;
    // While locked is 0 a yes or no row sits at the top of the screen.
    if (previous->kind == 6 && screen->locked == 0) {
      previous->y = 6;
      previous->targetY = 6;
    }
    previous->dx = 0;
    previous->dy = 0;
    rowPosition += 16;
  }
  SelectionMenu_LoadLargeIcon();
}

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

/// An empty function, reached only through the far-call table.
void SelectionMenu_ReservedNoOp(void) {}

/// Returns a free selection icon record of `kind` (enum SelectionIconGroup),
/// or NULL.
s32 SelectionMenu_FindFreeIcon(s32 kind) {
  s32 state;
  s32 off;
  s32 i;
  s32 j;
  u32 ret;
  u16 *q;
  u16 *p;
  u32 v;

  state = (s32)Ram_HeapSlots->menuWork;
  if (kind != 0) {
    i = 0;
    p = &((struct SelectionScreen *)state)->records[9].kind;
    off = 0;
  loop_2:
    if (*p == 0) {
      return state + off + 0x1D4;
    }
    i = i + 1;
    p += 0x1A;
    off = off + 0x34;
    if (i == 5) {
      goto block_10;
    }
    goto loop_2;
  }
  j = 0;
  ret = state + 0x68;
  q = &((struct SelectionScreen *)state)->records[2].kind;
loop_7:
  v = *q;
  q += 0x1A;
  if (v == 0) {
    return ret;
  }
  ret += 0x34;
  j += 1;
  if (j == 7) {
  block_10:
    return 0;
  }
  goto loop_7;
}

/// Starts drawing the selection screen each frame, in the vertical blank pass
/// (TASK_PASS_VBLANK).
void SelectionMenu_StartDrawing(void) {
  Task_Add(SelectionMenu_Draw, TASK_PASS_VBLANK);
}

/// Stops drawing the selection screen.
void SelectionMenu_StopDrawing(void) { Task_Remove(SelectionMenu_Draw); }

/* VIEW(window): tag struct EffectPosition as struct EffectPosition__window;
 * field/effect_step.h has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/include/field/effect_step.h:43: redefinition of `struct
 * EffectPosition'. */
#define EffectPosition EffectPosition__window
struct EffectPosition {
  s32 x;
  s32 y;
  s32 z;
};

extern u32 gVBlankCount;
extern s8 gMenu_CursorBobOffsets[];

struct MenuSelection;

void SelectionMenu_Draw(void) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  struct SelectionSprite *cursorEntry = &state->records[14].sprite;
  struct SelectionNode *cursor = &state->records[14];
  struct SelectionNode *transfer = &state->records[15];
  struct SelectionNode *node;
  struct SelectionSprite *e;
  struct SelectionSprite *te;
  s32 cnt = 0;
  s32 slot;
  s32 tmp;
  struct EffectPosition pos;

  node = state->head;
  while (node != 0) {
    e = &node->sprite;
    e->affine = 0;
    e->param = 0;
    e->x = node->x;
    e->y = node->y;
    slot = 240;
    if (state->verticalScroll != 0) {
      e->y += state->verticalScroll;
    } else if (node->x != node->targetX) {
      if (node->dx > 0) {
        if (node->x + node->dx > node->targetX) {
          node->x = node->targetX;
        } else {
          node->x = node->x + node->dx;
        }
      } else {
        if (node->x + node->dx < node->targetX) {
          node->x = node->targetX;
        } else {
          node->x = node->x + node->dx;
        }
      }
      e->x = node->x;
    } else if (cnt == state->cursorIndex) {
      slot = 241;
      if (cursor->kind != 0) {
        if (BattleMotion_ProjectScaledPositionFar(node->base, (s32 *)&pos) !=
            -1) {
          cursor->targetX = pos.x;
          cursor->targetY = pos.y;
          if (cursor->scale == 0) {
            cursor->x = pos.x;
            cursor->y = pos.y;
            cursor->scale = 1;
          }
        }
      }
    }
    if (node->scale != 0) {
      if (GameFlag_Get(0x103) != 0) {
        if (state->records[14].kind == 1) {
          e->mode = 1;
        } else {
          e->mode = 0;
        }
        if (node->kind == 1) {
          e->mode = 1;
        }
      }
      Oam_AddSpriteAtDepth((s32 *)e, slot);
    }
    node = node->next;
    cnt++;
  }
  if (transfer->kind != 0) {
    node = (struct SelectionNode *)SelectionMenu_GetCursorNode(
        (struct NodeChainState *)state);
    te = &transfer->sprite;
    te->mode = 0;
    te->affine = 0;
    te->param = 0;
    te->mosaic = 0;
    te->colors = 1;
    te->size = 2;
    te->shape = 0;
    te->priority = 0;
    te->tile = transfer->tile;
    te->x = node->x - 4;
    te->y =
        node->y + (gMenu_CursorBobOffsets[(gVBlankCount >> 1) & 15] >> 1) - 4;
    if (transfer->scale != transfer->scaleEnd) {
      state->transform.scaleX = transfer->scale;
      state->transform.scaleY = transfer->scale;
      state->transform.angle = 0;
      te->param = OamMatrix_AllocFromTransform(&state->transform);
      te->affine = 3;
      te->x = te->x + 0xfff0;
      te->y = te->y + 0xf0;
      transfer->scale = transfer->scale + transfer->scaleStep;
    }
    if (GameFlag_Get(0x103) != 0) {
      te->mode = 1;
    }
    Oam_AddSpriteAtDepth((s32 *)te, 248);
  }
  SelectionMenu_DrawArrow((struct MenuSelection *)state, 0);
  SelectionMenu_DrawArrow((struct MenuSelection *)state, 1);
  node = state->path;
  while (node != 0) {
    e = &node->sprite;
    if (node->x != node->targetX) {
      node->x = node->x + node->dx;
    }
    if (node->y != node->targetY) {
      node->y = node->y + node->dy;
    }
    e->x = node->x;
    e->y = node->y;
    if (node->scale != node->scaleEnd) {
      node->scale = node->scale + node->scaleStep;
      state->transform.scaleX = node->scale;
      state->transform.scaleY = node->scale;
      state->transform.angle = 0;
      e->param = OamMatrix_AllocFromTransform(&state->transform);
      e->affine = 3;
      e->x = e->x + 0xfff8;
      e->y = e->y + 0xf8;
    } else {
      e->affine = 0;
      e->param = 0;
    }
    if (GameFlag_Get(0x103) != 0) {
      if (state->records[14].kind == 1) {
        e->mode = 1;
      } else {
        e->mode = 0;
      }
      if (node->kind == 1) {
        e->mode = 1;
      }
    }
    Oam_AddSpriteAtDepth((s32 *)e, 240);
    node = node->next;
  }
  if (cursor->kind != 0) {
    cnt = (s32)gMenu_AnimatedCursorTiles;
    {
      void *src = (void *)(cnt + (((gVBlankCount >> 2) & 15) << 8));

      cursorEntry->tile = VramCache_Load(cursor->slot, 0x100, src);
    }
    if (cursor->targetX != cursor->x) {
      tmp = (cursor->targetX - cursor->x) >> 1;
      if (tmp != 0) {
        cursor->x = cursor->x + tmp;
      } else {
        cursor->x = cursor->targetX;
      }
    }
    if (cursor->targetY != cursor->y) {
      if ((tmp = (cursor->targetY - cursor->y) >> 1) != 0) {
        cursor->y = cursor->y + tmp;
      } else {
        cursor->y = cursor->targetY;
      }
    }
    cursorEntry->y =
        cursor->y + gMenu_CursorBobOffsets[(gVBlankCount >> 2) & 15] - 32;
    cursorEntry->x = cursor->x - 4;
    if (GameFlag_Get(0x103) != 0) {
      if (state->records[14].kind == 1) {
        cursorEntry->mode = 1;
      } else {
        cursorEntry->mode = 0;
      }
    }
    Oam_AddSpriteAtDepth((s32 *)cursorEntry, 248);
  }
  state->status += 1;
}

struct SlotEntry {
  struct SlotEntry *next;
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 mosaic : 1;
  u8 colors : 1;
  u8 shape : 2;
  u16 x : 9;
  u16 param : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 prio : 2;
  u16 pal : 4;
};
struct MenuNode {
  s32 unk00;
  struct MenuNode *next;
  u16 offset;
  u16 active;
  u16 handle;
  u16 tileId;
  s16 x, y, dx, dy, xEnd, yEnd;
  u8 unk1c[6];
  s16 scale, scaleStep, scaleEnd;
  struct SlotEntry entry;
};
struct MenuSelection {
  struct MenuNode nodes[2];
  u8 unk68[0x27a];
  u16 mode;
};
extern u8 gMenu_CursorActorTiles[], gMenu_CursorLeftActorTiles[];

void SelectionMenu_DrawArrow(struct MenuSelection *state, s32 index) {
  u32 frame = (gVBlankCount >> 2) & 7;
  struct SlotEntry *entry;
  u8 *frames;
  u8 *tmp;

  if (state->nodes[index].active == 0)
    return;
  entry = &state->nodes[index].entry;
  entry->x = state->nodes[index].x;
  entry->y = state->nodes[index].y;
  if (index != 0) {
    frames = gMenu_CursorActorTiles;
    if (state->nodes[1].offset != 0)
      entry->x = entry->x + state->nodes[1].offset;
  } else {
    frames = gMenu_CursorLeftActorTiles;
    if (state->nodes[0].offset != 0)
      entry->x = entry->x - state->nodes[0].offset;
  }
  tmp = frames + frame * 128;
  entry->tile = VramCache_Load(state->nodes[index].handle, 128, tmp);
  if (GameFlag_Get(0x103)) {
    if (state->mode == 1)
      entry->mode = 1;
    else
      entry->mode = 0;
  }
  Oam_AddSpriteAtDepth((s32 *)entry, 238);
  if (state->nodes[index].offset != 0)
    state->nodes[index].offset--;
}

struct Window *Window_Create(s32, s32, s32, s32, s32);

void WindowSprite_PrepareForRedraw(struct Window *);

/// Opens or refits the name window above the selection row and writes the name
/// of the icon under the cursor, or that nothing is known (ICON_KIND_ABILITY)
/// or held (ICON_KIND_ITEM). ICON_KIND_YES_NO is a yes or no question, placed
/// at the bottom or top of the screen. count is the name's width in tiles
/// inside the frame; 0 keeps the open window as it is. The window stays open
/// until SelectionMenu_Close closes it.
void SelectionMenu_OpenNameWindow(s32 mode, u32 count) {
  struct SelectionScreen *screen;
  struct SelectionNode *node;
  struct Window **slot;
  struct Window *window;

  screen = Ram_HeapSlots->menuWork;
  node = (struct SelectionNode *)SelectionMenu_GetCursorNode(
      (struct NodeChainState *)screen);
  slot = &screen->window;
  window = *slot;
  if (window == 0) {
    if (mode == 6) {
      if (screen->locked != 0) {
        *slot = Window_Create(17, 17, 5, 3, mode);
      } else {
        *slot = Window_Create(17, 0, 5, 3, mode);
      }
      screen->verticalScroll = 0;
      screen->locked = 999;
    } else {
      *slot = Window_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
    }
    WindowSprite_PrepareForRedraw(screen->window);
  } else {
    if (count != 0 && window->width != count + 2) {
      Window_Close(window, 2);
      *slot = Window_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
    }
    WindowSprite_PrepareForRedraw(screen->window);
  }
  if (screen->count != 0) {
    Text_DrawMessageAsTiles(node->message, (struct RenderInput *)screen->window,
                            0, 0);
  } else {
    switch (mode) {
    case 4:
      Text_DrawMessageAsTiles((s32)MsgAbilityNotKnown,
                              (struct RenderInput *)screen->window, 0, 0);
      break;
    case 2:
      Text_DrawMessageAsTiles((s32)MsgItemNotHeld,
                              (struct RenderInput *)screen->window, 0, 0);
      break;
    }
  }
}

/// Closes the selection screen and frees its icons, tiles and work block (heap
/// slot 18). The chosen icons go too, so the choices are only in the indexes
/// the loops returned.
void SelectionMenu_Close(void) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  struct SelectionNode *node;

  SelectionMenu_StopDrawing();
  Window_Close(state->window, 2);
  wait(1);
  node = state->head;
  while (node != 0) {
    if (node->kind != 0) {
      VramCache_FreeEntry(node->slot);
      node->kind = 0;
    }
    node = node->next;
  }
  node = state->path;
  while (node != 0) {
    if (node->kind != 0) {
      VramCache_FreeEntry(node->slot);
      node->kind = 0;
    }
    node = node->next;
  }
  Resource_ResetPendingTransfer();
  if (state->records[0].y != 0) {
    VramCache_FreeEntry(state->records[0].slot);
    if (state->records[0].y != 0) {
      VramCache_FreeEntry(state->records[1].slot);
    }
  }
  VramCache_FreeEntry(state->records[14].slot);
  HeapSlot_Release(72);
}

/// Moves the row of icons to start at pixel (x, y), 16 pixels apart.
void SelectionMenu_SetPosition(u32 first, u32 second) {
  struct SelectionScreen *state;
  struct SelectionNode *node;

  state = Ram_HeapSlots->menuWork;
  state->baseX = first;
  state->baseY = second;
  node = state->head;
  while (node != 0) {
    node->x = first;
    node->targetX = first;
    node->y = second;
    node->targetY = second;
    node = node->next;
    first += 16;
  }
}

void Menu_SetupSelectionBothSides(void) {
  void *menu = Ram_HeapSlots->menuWork;

  SelectionMenu_SetupArrow(menu, 0);
  SelectionMenu_SetupArrow(menu, 1);
}

extern u8 gMenu_CursorActorTiles[];
extern u8 gMenu_CursorLeftActorTiles[];

/// Sets up the left (side 0) or right (1) arrow of the selection row, shown
/// when there are entries beyond that end. The first call takes the arrow's
/// VRAM cache entry; later calls only move it and decide whether it shows.
void SelectionMenu_SetupArrow(struct SelectionScreen *state, s32 side) {
  struct SelectionSprite *arrow = &state->records[side].sprite;
  u8 *arrowTiles = 0;
  u32 count;

  state->records[side].kind = 0;
  if (side != 0) {
    u8 *right = gMenu_CursorActorTiles;

    count = state->count;
    if (state->top != 0)
      count -= state->top;
    if (count > 5) {
      state->records[side].kind = 1;
      count = 5;
    }
    arrowTiles = right;
    state->records[1].x = state->baseX + 16 * (count - 1) + 17;
  } else {
    arrowTiles = gMenu_CursorLeftActorTiles;
    state->records[0].x = state->baseX - 9;
    if (state->top != 0)
      state->records[0].kind = 1;
  }
  if (state->records[side].y == 0) {
    state->records[side].slot = VramCache_FindFreeEntry();
    state->records[side].tile =
        VramCache_Load(state->records[side].slot, 128, arrowTiles);
    state->records[side].y = state->baseY;
    state->records[side].base = 0;
    arrow->mode = 0;
    arrow->mosaic = 0;
    arrow->colors = 1;
    arrow->affine = 0;
    arrow->param = 0;
    arrow->size = 0;
    arrow->shape = 2;
    arrow->priority = 0;
  }
}

struct NodeChainNode *SelectionMenu_GetCursorNode(struct NodeChainState *st) {
  struct NodeChainNode *node = st->node;
  s32 i;

  for (i = 0; i != st->count; ++i) {
    node = node->next;
  }
  return node;
}

#define SELECTION_FIXED 999

#define NODE_STATUS_SCROLLING 33
#define NODE_STATUS_SHOWN 1

#define NODE_KIND_YES_NO 6

#define SELECTION_ROWS 4
#define ARROW_SCROLL_FRAMES 8

struct SelectionScrollNode {
  u8 pad0[10];
  u16 kind;
};

struct SelectionScrollScreen {
  u8 pad0[8];
  u16 upScroll;
  u16 upArrow;
  u8 pad1[0x3c - 0xc];
  u16 downScroll;
  u16 downArrow;
  u8 pad2[0x348 - 0x40];
  struct SelectionScrollNode *node;
  u8 pad3[0x394 - 0x34c];
  u16 count;
  u8 pad4[0x39c - 0x396];
  u16 top;
  u16 cursor;
  u16 busy;
  u16 status;
};

STATIC_ASSERT(offsetof(struct SelectionScrollScreen, downArrow) == 0x3e);
STATIC_ASSERT(offsetof(struct SelectionScrollScreen, node) == 0x348);
STATIC_ASSERT(offsetof(struct SelectionScrollScreen, status) == 0x3a2);

/// Runs the selection row until the player chooses (the chosen entry's index)
/// or, when mode allows it, cancels with B (-1). SELECTION_FIXED allows no
/// moving or choosing; any mode but 0 allows cancelling. Reached only through
/// a bare entry of the window far-call table.
s32 SelectionMenu_Run(s32 mode) {
  struct SelectionScrollScreen *screen = Ram_HeapSlots->menuWork;

  SelectionMenu_HighlightIcon((struct MenuResourceList *)screen, 0);
  for (;;) {
    wait(1);
    if (screen->busy != 0) {
      continue;
    }
    if (mode != SELECTION_FIXED) {
      if (gInput.repeat & KEY_RIGHT) {
        SelectionMenu_StepRight((struct SelectionScreen *)screen);
      } else if (gInput.repeat & KEY_LEFT) {
        SelectionMenu_StepLeft((struct StepMenu *)screen);
      } else if (gInput.pressed & KEY_A) {
        return Menu_ConfirmSelection((struct SelectionScreen *)screen);
      }
    }
    if (mode != 0 && (gInput.pressed & KEY_B)) {
      return -1;
    }
  }
}

#define SOUND_MENU_CURSOR_MOVE 111
#define SOUND_MENU_CONFIRM 112
#define SOUND_MENU_CANCEL 113

/// As SelectionMenu_Run, with the menu sounds.
u32 SelectionMenu_RunWithSounds(u32 mode) {
  struct SelectionScrollScreen *screen = Ram_HeapSlots->menuWork;
  u32 result;

  for (;;) {
    wait(1);
    if (screen->busy != 0) {
      continue;
    }

    if (mode != SELECTION_FIXED) {
      if (gInput.repeat & KEY_RIGHT) {
        Sound_PlayFar(SOUND_MENU_CURSOR_MOVE);
        SelectionMenu_StepRight((struct SelectionScreen *)screen);
      } else if (gInput.repeat & KEY_LEFT) {
        Sound_PlayFar(SOUND_MENU_CURSOR_MOVE);
        SelectionMenu_StepLeft((struct StepMenu *)screen);
      }

      if (gInput.pressed & KEY_A) {
        result = screen->top + screen->cursor;
        if (screen->node->kind == NODE_KIND_YES_NO) {
          if (result == 0)
            Sound_PlayFar(SOUND_MENU_CONFIRM);
          else
            Sound_PlayFar(SOUND_MENU_CANCEL);
        } else {
          Sound_PlayFar(SOUND_MENU_CONFIRM);
        }
        return result;
      }
    }

    if (mode != 0 && (gInput.pressed & KEY_B)) {
      Sound_PlayFar(SOUND_MENU_CANCEL);
      return -1;
    }
  }
}

/// Moves the cursor one entry right, scrolling the row at its fourth place.
/// This variant stops at the end; SelectionMenu_StepRight also wraps around.
/// Nothing calls it.
UNUSED void Menu_MoveSelectionForward(struct SelectionScrollScreen *screen) {
  u32 nextEntry = screen->top + screen->cursor + 1;

  if (nextEntry == screen->count) {
    return;
  }
  SelectionMenu_RedrawIcon((struct MenuResourceList *)screen, screen->cursor);
  screen->status = NODE_STATUS_SCROLLING;
  wait(1);
  screen->cursor++;
  if (screen->cursor == SELECTION_ROWS && nextEntry + 1 < screen->count) {
    screen->cursor--;
    screen->downScroll = ARROW_SCROLL_FRAMES;
    screen->top++;
    SelectionMenu_Scroll((struct SelectionScreen *)screen, 1);
    if (screen->top + screen->cursor + 2 == screen->count) {
      screen->downArrow = 0;
    }
    screen->upArrow = 1;
  }
  screen->status = NODE_STATUS_SHOWN;
  SelectionMenu_HighlightIcon((struct MenuResourceList *)screen,
                              screen->cursor);
  wait(1);
  SelectionMenu_OpenNameWindow(screen->node->kind, 0);
  wait(1);
}

extern struct SelectionScrollScreen *gResQueueWork;
/* VIEW(window): variable gKeyState as u32 gKeyState; module/window/window.h has
 * s32 gKeyState; not tried: gKeyState's declaration holds a FAKEMATCH tag. */
extern u32 gKeyState__window __asm__("gKeyState");
#define gKeyState gKeyState__window
extern volatile u32 gKeysRepeat;

void Menu_MoveSelectionBackward(struct SelectionScrollScreen *screen) {
  if (screen->top == 0 && screen->cursor == 0) {
    return;
  }
  SelectionMenu_RedrawIcon((struct MenuResourceList *)screen, screen->cursor);
  screen->status = NODE_STATUS_SCROLLING;
  wait(1);
  if (screen->cursor == 1 && screen->top != 0) {
    screen->upScroll = ARROW_SCROLL_FRAMES;
    screen->top--;
    SelectionMenu_Scroll((struct SelectionScreen *)screen, 0);
    if (screen->top == 0) {
      screen->upArrow = 0;
    }
    screen->downArrow = 1;
  } else {
    screen->cursor--;
  }
  screen->status = NODE_STATUS_SHOWN;
  SelectionMenu_HighlightIcon((struct MenuResourceList *)screen,
                              screen->cursor);
  wait(1);
  SelectionMenu_OpenNameWindow(screen->node->kind, 0);
  wait(1);
}

/// Moves the cursor one entry right, scrolling the row at its edge and
/// wrapping from the last entry to the first.
void SelectionMenu_StepRight(struct SelectionScreen *state) {
  struct SelectionNode *node;
  u32 entryIndex;
  s32 y;
  u32 end;

  SelectionMenu_RedrawIcon((struct MenuResourceList *)state,
                           state->cursorIndex);
  state->status = 33;
  wait(1);
  state->cursorIndex++;
  if (state->count > 5) {
    end = state->top + state->cursorIndex;
    if (end == state->count) {
      node = state->head;
      state->records[0].kind = 0;
      while (node->next != NULL) {
        node->targetX = state->baseX;
        node->targetY = state->baseY;
        node->dx = -12;
        node = node->next;
      }
      node->targetX = state->baseX;
      node->targetY = state->baseY;
      node->dx = -12;
      while (node->targetX != node->x)
        wait(1);
      node = state->head;
      if (node != NULL) {
        entryIndex = 0;
        do {
          SelectionMenu_SetupIcon(state->kinds[entryIndex],
                                  state->bases[entryIndex],
                                  (struct ListNode *)node, 1);
          node = node->next;
          entryIndex++;
        } while (node != NULL);
      }
      state->cursorIndex = 0;
      state->top = 0;
      node = state->head;
      y = node->x + 16;
      node = node->next;
      while (node != NULL) {
        node->targetX = y;
        node->dx = 12;
        node = node->next;
        y += 16;
      }
      state->records[1].kind = 1;
    } else if (state->cursorIndex == 4 && end + 1 < state->count) {
      state->cursorIndex--;
      state->records[1].base = 8;
      state->top++;
      SelectionMenu_Scroll(state, 1);
      if (state->top + state->cursorIndex + 2 == state->count)
        state->records[1].kind = 0;
      state->records[0].kind = 1;
    }
  } else if (state->cursorIndex == state->count) {
    state->cursorIndex = 0;
  }
  state->status = 1;
  SelectionMenu_HighlightIcon((struct MenuResourceList *)state,
                              state->cursorIndex);
  wait(1);
  SelectionMenu_OpenNameWindow(state->head->kind, 0);
  wait(1);
}

struct StepNode {
  u8 unk00[4];
  struct StepNode *next;
  u8 unk08[2];
  u16 id;
  u8 unk0c[4];
  s16 y;
  u8 unk12[2];
  u16 speed;
  u8 unk16[2];
  s16 targetY;
  u16 targetZ;
};

struct StepMenu {
  u8 unk000[8];
  u16 scrollUp;
  u16 moreAbove;
  u8 unk00c[0x30];
  u16 scrollDown;
  u16 moreBelow;
  u8 unk040[0x308];
  struct StepNode *nodes;
  u8 unk34c[8];
  u16 entryIds[16];
  u16 entryKinds[16];
  u16 count;
  u16 baseY;
  u16 baseZ;
  u8 unk39a[2];
  u16 top;
  u16 cursor;
  u8 unk3a0[2];
  u16 status;
};

/// Moves the cursor one entry left, scrolling the row at its edge and
/// wrapping from the first entry to the last.
void SelectionMenu_StepLeft(struct StepMenu *state) {
  struct StepNode *node;
  s32 i;
  s32 kind;

  SelectionMenu_RedrawIcon((struct MenuResourceList *)state, state->cursor);
  state->status = 33;
  wait(1);
  if (state->count > 5) {
    if ((state->cursor | state->top) != 0) {
      if (state->cursor == 1 && state->top != 0) {
        state->scrollUp = 8;
        state->top--;
        SelectionMenu_Scroll((struct SelectionScreen *)state, 0);
        if (state->top == 0)
          state->moreAbove = 0;
        state->moreBelow = 1;
      } else {
        state->cursor--;
      }
    } else {
      node = state->nodes;
      i = 64;
      // kind is reused for the 0; as a plain 0, 20 of 206 instructions differ
      // in tla-en.
      kind = 0;
      state->moreBelow = kind;
      while (node->next != NULL) {
        node->targetY = node->y + i;
        node->speed = 12;
        node = node->next;
        i -= 16;
      }
      node = state->nodes;
      while (node->y != node->targetY)
        wait(1);
      i = 0;
      while (i != state->count - 5)
        i++;
      node = state->nodes;
      state->top = i;
      state->cursor = 4;
      while (node != NULL) {
        SelectionMenu_SetupIcon(state->entryIds[i], state->entryKinds[i],
                                (struct ListNode *)node, 1);
        node = node->next;
        i++;
      }
      node = state->nodes;
      i = state->baseY;
      while (node->next != NULL) {
        node->targetY = i;
        node->speed = -12;
        node = node->next;
        i += 16;
      }
      state->moreAbove = 1;
    }
  } else if (state->cursor != 0) {
    state->cursor--;
  } else {
    state->cursor = state->count - 1;
  }
  state->status = 1;
  SelectionMenu_HighlightIcon((struct MenuResourceList *)state, state->cursor);
  wait(1);
  kind = state->nodes->id;
  SelectionMenu_OpenNameWindow(kind, 0);
  wait(1);
}

struct MenuResourceNode {
  u8 unk0[4];
  struct MenuResourceNode *next;
  u8 unk8[2];
  u16 type;
  u16 value;
  u8 unk14[18];
  u16 base;
};

struct MenuResourceList {
  u8 unk0[0x348];
  struct MenuResourceNode *head;
};

void Icon_BuildCommandIconTiles(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3,
                                s32 arg4);

/// Redraws the command icon at row place rowIndex in its plain state, in its
/// own VRAM cache entry. Other kinds of icon are left alone.
void SelectionMenu_RedrawIcon(struct MenuResourceList *state, u32 index) {
  struct MenuResourceNode *node = state->head;
  u32 output;
  u32 value;

  while (index != 0) {
    index--;
    node = node->next;
  }
  if (node->type == 1 || node->type == 6) {
    u32 first = node->base - (u32)&MsgCommandName;

    value = node->value;
    Icon_BuildCommandIconTiles(first, 0, &value, &output, 1);
  }
}

/// Redraws the command icon at row place rowIndex and loads its large icon.
/// The small icon is drawn the same as SelectionMenu_RedrawIcon draws it;
/// the highlight is the large icon.
void SelectionMenu_HighlightIcon(struct MenuResourceList *state, u32 index) {
  struct MenuResourceNode *node = state->head;
  u32 res;
  u32 value;

  while (index != 0) {
    index--;
    node = node->next;
  }
  if (node->type == 1 || node->type == 6) {
    u32 id = node->base - (u32)&MsgCommandName;

    value = node->value;
    Icon_BuildCommandIconTiles(id, 0, &value, &res, 1);
    SelectionMenu_LoadLargeIcon();
  }
}

/// Stops the battle's highlight of units (BattleHighlight_SetModesFar
/// with mode 0); Menu_ConfirmSelection calls it as an entry is chosen. It
/// passes a unit list with one place an icon, of which only the end marker
/// (BATTLE_UNIT_LIST_END) is written; mode 0 never reads the list.
void Menu_ClearBattleHighlight(u8 *arg0) {
  u16 data[6];
  struct NodeChainNode *node = ((struct NodeChainState *)arg0)->node;
  s32 count = 0;

  while (node != 0) {
    node = node->next;
    count++;
  }
  data[count] = 0xff;
  BattleHighlight_SetModesFar(data, 0);
}

/// Scrolls the row one entry: forward brings an icon in from the right as the
/// leftmost shrinks away, and back the reverse; waits until it has moved.
/// Releases the departing cache slot only after its shrinking animation
/// finishes. The icons move 2 pixels a frame and zoom by 32 (in 8.8) a frame,
/// so a scroll takes 8 frames.
void SelectionMenu_Scroll(struct SelectionScreen *screen, s32 forward) {
  struct SelectionNode *p;
  struct SelectionNode *last;
  struct SelectionNode *first;
  s32 base;
  s32 kind;
  u32 index;
  s32 y;
  s32 z;

  if (forward != 0) {
    index = screen->top + SELECTION_ROWS;
    base = screen->bases[index];
    kind = screen->kinds[index];
    p = (struct SelectionNode *)SelectionMenu_FindFreeIcon(0);
    if (p == NULL)
      return;
    SelectionMenu_SetupIcon(kind, base, (struct ListNode *)p, 0);
    y = screen->baseX;
    p->x = y + 80;
    z = screen->baseY;
    p->targetX = y + 64;
    p->y = z;
    p->dx = -2;
    p->targetY = z;
    p->scaleStep = 32;
    p->scale = 32;
    p->scaleEnd = 256;
    last = p;
    p = screen->head;
    p->scaleStep = -32;
    p->scaleEnd = 0;
    for (;;) {
      p->targetX = p->x - 16;
      p->dx = -2;
      if (p->next == NULL)
        break;
      p = p->next;
    }
    p->next = last;
    last->next = NULL;
    last->prev = p;
    p = screen->head;
    do {
      wait(1);
    } while (p->scale != 0);
    screen->head = p->next;
    VramCache_FreeEntry(p->slot);
    p->kind = 0;
    p = p->next;
    p->prev = NULL;
  } else {
    index = screen->top;
    base = screen->bases[index];
    kind = screen->kinds[index];
    p = (struct SelectionNode *)SelectionMenu_FindFreeIcon(0);
    if (p == NULL)
      return;
    SelectionMenu_SetupIcon(kind, base, (struct ListNode *)p, 0);
    p->x = screen->baseX - 16;
    p->y = screen->baseY;
    p->targetY = p->y;
    p->dx = 2;
    p->scale = 32;
    p->scaleStep = 32;
    p->targetX = p->x + 16;
    p->scaleEnd = 256;
    first = p;
    p = screen->head;
    p->prev = first;
    first->next = p;
    first->prev = NULL;
    screen->head = first;
    p = first;
    for (;;) {
      p->targetX = p->x + 16;
      p->dx = 2;
      if (p->next == NULL)
        break;
      p = p->next;
    }
    p->scaleEnd = 0;
    p->scaleStep = -32;
    p = screen->head;
    do {
      wait(1);
    } while (p->scale != 256);
    while (p->next != NULL)
      p = p->next;
    VramCache_FreeEntry(p->slot);
    p->kind = 0;
    p->prev->next = NULL;
  }
}

void Icon_BuildCommandIconTiles(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3,
                                s32 arg4);

s32 Resource_LoadByMode(u32 mode, s32 value) {
  s32 output;
  s32 result = -1;

  switch (mode) {
  case 1:
  case 6:
    Icon_BuildCommandIconTiles(value, 0, &result, &output, 0);
    break;
  case 2:
    result = VramCache_FindFreeEntry();
    if (result == 96)
      return -1;
    Icon_CopyResourceToSlot(value, 0x1a, result);
    break;
  case 4:
    Ability_LoadGlyph(value, 1, &result, &output, 0);
    break;
  }
  return result;
}

s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option) {
  s32 output;
  s32 original = result;

  if (result == -1) {
    result = VramCache_FindFreeEntry();
    if (result == 0x60)
      return original;
  }

  switch (mode) {
  case 1:
  case 6:
    Icon_BuildCommandIconTiles(value, option, &result, &output, 1);
    break;
  case 2:
    Icon_CopyResourceToSlot(value, 58, result);
    break;
  case 7:
    Icon_CopyResourceToSlot(value, 42, result);
    break;
  case 4:
    Ability_LoadGlyph(value, option, &result, &output, 1);
    break;
  case 8:
    Icon_BuildMiscIconTiles(value, 0, result);
    break;
  }

  return result;
}

struct Actor_0801c0dc {
  u8 filler0[5];
  u8 field50 : 2;
  u8 field52 : 2;
  u8 field54 : 1;
  u8 field55 : 1;
  u8 field56 : 2;
  u8 filler6;
  u8 field70 : 1;
  u8 field71 : 5;
  u8 field76 : 2;
  u16 field80 : 10;
  u16 field8a : 2;
  u16 field8c : 4;
};

struct ListNode {
  u8 unk00[8];
  s16 base;
  s16 kind;
  u16 src;
  s16 tile;
  u8 unk10[16];
  s16 end;
  s16 field22;
  u8 unk24[2];
  s16 field26;
  struct Actor_0801c0dc obj;
};

void Icon_BuildCommandIconTiles(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3,
                                s32 arg4);
void Icon_BuildItemIconTiles(u32 glyph, s32 includeBase, s32 *sourceIndex,
                             s32 *result, s32 reuseSource);

/// Sets up a selection icon for an entry: its icon tiles (in its old slot when
/// reuse is set), its name message and its sprite.
/// kind must select a command, item or Psynergy with the matching base ID.
/// Items and Psynergy are drawn over the icon base; the sprite is 16x16 at
/// full size (Q_8_8(1)).
void SelectionMenu_SetupIcon(u32 kind, s32 base, struct ListNode *node,
                             s32 reuse) {
  s32 src;
  s32 tile;
  struct Actor_0801c0dc *obj;

  switch (kind) {
  case 1:
  case 6:
    if (reuse != 0)
      src = node->src;
    Icon_BuildCommandIconTiles(base, 0, &src, &tile, reuse);
    node->end = base + (s32)&MsgCommandName;
    break;
  case 2:
    if (reuse != 0)
      src = node->src;
    Icon_BuildItemIconTiles(base, 1, &src, &tile, reuse);
    node->end = base + (s32)&MsgItemName;
    break;
  case 4:
    if (reuse != 0)
      src = node->src;
    Ability_LoadGlyph(base, 1, (s32 *)&src, &tile, reuse);
    node->end = base + (s32)&MsgAbilityNothing;
    break;
  }

  node->base = base;
  node->src = src;
  node->tile = tile;
  node->kind = kind;
  node->field22 = 0x100;
  node->field26 = 0x100;

  obj = &node->obj;
  obj->field52 = 0;
  obj->field55 = 0;
  obj->field54 = 0;
  obj->field76 = 1;
  obj->field56 = 0;
  obj->field8c = 0;
  obj->field80 = tile;
  obj->field8a = 0;
}

/// The Broken Seal's Menu_ConfirmSelection in this game's form.
u32 Menu_ConfirmSelection(struct SelectionScreen *screen) {
  struct SelectionNode *node;
  struct SelectionNode *other = NULL;
  u32 cnt = 0;
  u32 index;
  struct SelectionSprite *sprite;

  index = screen->top + screen->cursorIndex;
  Menu_ClearBattleHighlight((u8 *)screen);
  SelectionMenu_RedrawIcon((struct MenuResourceList *)screen,
                           screen->cursorIndex);
  screen->status = 33;
  wait(1);
  screen->records[0].kind = 0;
  screen->records[1].kind = 0;
  screen->records[14].kind = 0;
  screen->records[14].scale = 0;
  Resource_ResetPendingTransfer();
  node = screen->head;
  while (node != NULL && screen->cursorIndex != cnt) {
    node = node->next;
    cnt++;
  }
  node->homeX = node->x;
  node->homeY = node->y;
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      other->targetX = node->x;
      other->dx = (node->x - other->x) >> 1;
    }
  }
  wait(2);
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      VramCache_FreeEntry(other->slot);
      other->kind = 0;
    }
  }
  screen->head = node;
  node->prev = NULL;
  node->next = NULL;
  node->targetX = 4;
  for (other = screen->path, cnt = 0; other != NULL; other = other->next) {
    node->targetX += 16;
    cnt++;
  }
  screen->pathTop[cnt] = screen->top;
  screen->pathCursor[cnt] = screen->cursorIndex;
  node->dx = (node->targetX - node->x) >> 1;
  screen->unk39a = 0;
  screen->cursorIndex |= 0x80;
  wait(2);
  other = (struct SelectionNode *)SelectionMenu_FindFreeIcon(1);
  other->kind = node->kind;
  other->message = node->message;
  other->base = node->base;
  other->slot = node->slot;
  other->tile = node->tile;
  other->x = node->x;
  other->y = node->y;
  other->targetX = other->x;
  other->targetY = other->y;
  other->homeX = node->homeX;
  other->homeY = node->homeY;
  other->dx = 0;
  other->dy = 0;
  other->scale = 0x100;
  other->scaleEnd = 0x100;
  sprite = &other->sprite;
  sprite->mode = 0;
  sprite->colors = 0;
  sprite->mosaic = 0;
  sprite->size = 1;
  sprite->shape = 0;
  sprite->palette = 0;
  sprite->tile = other->tile;
  node->kind = 0;
  screen->head = NULL;
  if (screen->path != NULL) {
    node = screen->path;
    while (node->next != NULL)
      node = node->next;
    node->next = other;
    other->prev = node;
    other->next = NULL;
  } else {
    screen->path = other;
    other->prev = NULL;
    other->next = NULL;
  }
  return index;
}

/// Six empty functions; nothing calls the first, the fourth or the fifth. The
/// others are reached only through bare entries of the window far-call table.
/// With no body or caller to give them a role, they are numbered in ROM order.
void MenuArrow_NoOpCallback(void) {}

void MenuArrow_NoOpCallbackBank(void) {}

void MenuArrow_NoOpCallback1(void) {}

void MenuArrow_NoOpCallback2(void) {}

void MenuArrow_NoOpCallback3(void) {}

void MenuArrow_NoOpCallback4(void) {}

/* VIEW(window): tag struct MenuSprite as struct MenuSprite__window;
 * window/menu_list.h has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/include/window/menu_list.h:35: redefinition of `struct MenuSprite'.
 */
#define MenuSprite MenuSprite__window
struct MenuSprite {
  struct MenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

/// Sets up a sprite as the right-pointing arrow, in a free tile slot (returned
/// in *slot).
/// The caller owns the cache slot and releases it through
/// MenuArrow_Release.
void MenuArrow_Create(struct MenuSprite *object, s32 *slot) {
  const void *data = gMenu_CursorActorTiles;
  s32 value = VramCache_FindFreeEntry();

  *slot = value;
  object->oam.f.tile = VramCache_Load(value, 0x80, data);
  object->oam.f.mode = 0;
  object->oam.f.mosaic = 0;
  object->oam.f.colors = 1;
  object->oam.f.affine = 0;
  object->oam.f.affineIndex = 0;
  object->oam.f.size = 0;
  object->oam.f.shape = 2;
  object->oam.f.priority = 0;
}

struct TextResourcePosition {
  u8 padding[4];
  s8 kind;
  u8 padding2;
  u16 index : 9;
  u16 rest : 7;
};

/// Places an arrow sprite at screen pixel (x, y) and queues it for drawing
/// this frame, at depth slot 252; the caller calls it every frame it shows.
void MenuArrow_SetPosition(struct TextResourcePosition *obj, s32 arg1,
                           s32 arg2) {
  obj->index = arg1;
  obj->kind = arg2;
  /* FAKEMATCH(dead): an empty once-only block keeps the index's store ahead of
   * the depth argument (mov r1, #252); without it, 2 of 13 instructions differ
   * in tla-en. */
  do {
  } while (0);
  Oam_AddSpriteAtDepth((s32 *)obj, 0xFC);
}

/// Frees an arrow sprite's tile slot.
void MenuArrow_Release(s32 value) { VramCache_FreeEntry(value); }

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;

/// Loads the large icon of the command under the cursor, zooming in: 32x32 in
/// 256 colours (32 * 32 bytes, record 15), its scale stepping from 40 to 240 by
/// 40 a frame, after which SelectionMenu_Draw shows it at its own size.
/// Other kinds of entry show none. The icon keeps its VRAM cache entry until
/// Resource_ResetPendingTransfer.
void SelectionMenu_LoadLargeIcon(void) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  struct SelectionNode *selection =
      (struct SelectionNode *)SelectionMenu_GetCursorNode(
          (struct NodeChainState *)state);
  struct SelectionNode *transfer;
  GlyphTransfer *buffer;
  u8 *table;
  void *resource;
  s32 no;

  if (selection->kind != 1 && selection->kind != 6)
    return;

  buffer = (GlyphTransfer *)HeapSlot_Alloc(68, sizeof(GlyphTransfer));
  transfer = &state->records[15];
  no = selection->base;
  table = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  {
    u8 **destination = &buffer->encoded;
    resource = table + *(u16 *)(table + selection->base * 2);
    *destination = resource;
  }
  Resource_DecodeByteLzInRam(resource, buffer);

  if (transfer->kind == 0)
    transfer->slot = VramCache_FindFreeEntry();
  transfer->tile = VramCache_Load(transfer->slot, 0x400, buffer);
  transfer->kind = 1;
  transfer->base = no;
  transfer->scale = 40;
  transfer->scaleStep = 40;
  transfer->scaleEnd = 240;
  HeapSlot_Release(68);
}

void Resource_ResetPendingTransfer(void) {
  struct SelectionNode *transfer =
      &((struct SelectionScreen *)Ram_HeapSlots->menuWork)->records[15];

  if (transfer->kind != 0) {
    VramCache_FreeEntry(transfer->slot);
    transfer->kind = 0;
  }
}

void PartyStatus_CreateWindow(s32 flags);
s32 FieldMenu_SelectCommand(s32);
s32 DjinnMenu_RunFar(void);
s32 Actor_GetTriggerCellAheadOfCurrentFar(void);
s32 PsynergyMenu_RunFar(void);
s32 ItemMenu_RunFar(void);
s32 StatusScreen_RunFar(void);

/// Runs the field menu until the player picks something that leaves it, and
/// returns the chosen screen's result. Cancelling the menu returns an
/// undefined value, the missing result of PartyStatus_CloseWithMenu (0 under
/// UBFIX); its one caller, FieldSequence_Run, ignores the result. Its case for
/// command 0, which stores the trigger tile ahead in the field work's
/// examineCell, is unreachable.
s32 FieldMenu_Run(void) {
  s32 ret;
  s32 selection;
  u8 *state;

  state = Ram_HeapSlots->eventWork;
  selection = 0;

loop:
  PartyStatus_OpenAndWait();
  selection = FieldMenu_SelectCommand(selection);
#ifdef UBFIX
  // UB: the original reads the void return of PartyStatus_CloseWithMenu, called
  // with no prototype in scope, as The Broken Seal's FieldMenu_Run does.
  PartyStatus_CloseWithMenu();
  ret = 0;
#else
  ret = PartyStatus_CloseWithMenu();
#endif

  switch (selection) {
  case 0:
    ret = Actor_GetTriggerCellAheadOfCurrentFar();
    if (ret == 0)
      ret = 0xff;
    *(u16 *)(state + 0x162) = ret;
    break;
  case 1:
    ret = PsynergyMenu_RunFar();
    if (ret == -1)
      goto loop;
    break;
  case 2:
    ret = DjinnMenu_RunFar();
    if (ret != 0)
      goto loop;
    break;
  case 3:
    ret = ItemMenu_RunFar();
    if (ret == -1)
      goto loop;
    break;
  case 4:
    ret = StatusScreen_RunFar();
    if (ret == -1)
      goto loop;
    break;
  default:
    break;
  }

  return ret;
}

void PartyStatus_OpenAndWait(void) {
  PartyStatus_CreateWindow(1);
  wait(1);
}

/// Closes the window in heap slot 16 and releases its block.
void PartyStatus_CloseWithMenu(void) { PartyStatus_Close(); }

void SelectionMenu_CreateEmpty(void) {
  SelectionMenu_Create();
  SelectionMenu_Clear();
  wait(1);
}

void SelectionMenu_BuildEntries(u32);

u32 Menu_RunSelectionForValue(u32 value) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  u32 result;

  state->cursorIndex = value;
  state->locked = 1;
  SelectionMenu_BuildEntries(value);
  Menu_SetupSelectionBothSides();
  SelectionMenu_OpenNameWindow(0, 5);
  SelectionMenu_StartDrawing();
  result = SelectionMenu_RunWithSounds(1);
  SelectionMenu_Close();
  return result;
}

#undef SELECTION_FIXED
#undef NODE_STATUS_SCROLLING
#undef NODE_STATUS_SHOWN
#undef NODE_KIND_YES_NO
#undef SELECTION_ROWS
#undef ARROW_SCROLL_FRAMES
#undef SOUND_MENU_CURSOR_MOVE
#undef SOUND_MENU_CONFIRM
#undef SOUND_MENU_CANCEL
#undef Resource_DecodeByteLzInRam

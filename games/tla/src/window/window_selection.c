/// @file
/// The selection screen's row of icons, and the field menu.
// Part of window.c.

#include "window_internal.h"
#include "resource_ids.h"
#include "field/field_work.h"
#include "field/map_grid.h"
#include "flags.h"
#include "sounds.h"
#include "field/particle.h"
#include "window/menu_list.h"

/// The selection row's icons stand 16 pixels apart.
#define SELECTION_ICON_SPACING 16

/// SelectionMenuWork.verticalScroll as the screen is made: the row's icons
/// drawn 32 pixels lower, below the bottom of the screen, and the cursor held
/// still, until SelectionMenu_Clear readies the list.
#define SELECTION_ROW_HIDDEN 32

/// SelectionMenuWork.locked as the screen is made and once a yes or no window
/// has opened: nonzero keeps a yes or no row and its window at the bottom of
/// the screen, as Menu_RunSelectionForValue's 1 does.
#define SELECTION_YES_NO_AT_BOTTOM 999

/// One frame of an end arrow's animation: an 8x16 sprite of 256 colours.
#define ARROW_FRAME_BYTES (2 * TILE_SIZE_8BPP)
/// One frame of the pointing hand's animation: a 16x16 sprite of 256 colours.
#define HAND_FRAME_BYTES (4 * TILE_SIZE_8BPP)

// The part's functions its code calls before their definitions, declared
// once as they are defined.
s32 SelectionMenu_FindFreeIcon(s32 kind);
void SelectionMenu_LoadLargeIcon(void);
void SelectionMenu_Draw(void);
void SelectionMenu_DrawArrow(struct SelectionMenuWork *state, s32 side);
void SelectionMenu_SetupArrow(struct SelectionMenuWork *state, s32 index);
void SelectionMenu_StepRight(struct SelectionMenuWork *state);
void SelectionMenu_StepLeft(struct SelectionMenuWork *state);
void SelectionMenu_RedrawIcon(struct SelectionMenuWork *state, u32 rowIndex);
void SelectionMenu_HighlightIcon(struct SelectionMenuWork *state, u32 rowIndex);
void SelectionMenu_Scroll(struct SelectionMenuWork *screen, s32 forward);
void SelectionMenu_SetupIcon(u32 kind, s32 base, struct SelectionNode *node,
                             s32 reuse);
u32 Menu_ConfirmSelection(struct SelectionMenuWork *screen);
void PartyStatus_OpenAndWait(void);

extern const u8 gMenu_AnimatedCursorTiles[];

/// Allocates and resets the selection screen (heap slot 72): the row of icons
/// the player picks from, with arrows at each end and a pointing hand over the
/// chosen target, whose picture takes a VRAM cache entry.
void SelectionMenu_Create(void) {
  struct SelectionMenuWork *screen;
  struct SelectionSprite *visual;
  const u8 *table;
  s32 i;
  u16 clear = 0;

  screen = HeapSlot_AllocEwram(HEAP_SLOT_SELECTION_MENU,
                               sizeof(struct SelectionMenuWork));
  screen->head = (struct SelectionNode *)(u32)clear;
  screen->path = (struct SelectionNode *)(u32)clear;
  screen->window = (struct Window *)(u32)clear;
  screen->unread = clear;
  screen->top = clear;
  // The first list starts from its first entry.
  screen->cursorIndex = SELECTION_CURSOR_RESET;
  screen->verticalScroll = SELECTION_ROW_HIDDEN;
  screen->count = clear;
  screen->locked = SELECTION_YES_NO_AT_BOTTOM;
  i = 0;
  do {
    screen->records[i + SELECTION_RECORD_ROW].kind = ICON_KIND_NONE;
    screen->records[i + SELECTION_RECORD_CHOSEN].kind = ICON_KIND_NONE;
    i++;
  } while (i != 5);
  // The row has two icons more than the chosen ones; i is 5.
  screen->records[i + SELECTION_RECORD_ROW].kind = ICON_KIND_NONE;
  screen->records[i + SELECTION_RECORD_ROW + 1].kind = ICON_KIND_NONE;
  screen->records[0].kind = FALSE;
  screen->records[1].kind = FALSE;
  screen->records[0].y = 0;
  screen->records[1].y = 0;
  table = gMenu_AnimatedCursorTiles;
  screen->records[SELECTION_RECORD_HAND].slot = VramCache_FindFreeEntry();
  screen->records[SELECTION_RECORD_HAND].tile = VramCache_Load(
      screen->records[SELECTION_RECORD_HAND].slot, HAND_FRAME_BYTES, table);
  screen->records[SELECTION_RECORD_HAND].kind = 0;
  screen->records[SELECTION_RECORD_HAND].scale = 0;
  screen->records[SELECTION_RECORD_LARGE_ICON].kind = ICON_KIND_NONE;
  visual = &screen->records[SELECTION_RECORD_HAND].sprite;
  visual->mode = 0;
  visual->mosaic = 0;
  visual->colors = 1;
  visual->affine = 0;
  visual->matrix = 0;
  visual->size = 1;
  visual->shape = 0;
  visual->priority = 0;
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
  struct SelectionMenuWork *screen;

  screen = Ram_HeapSlots->selectionMenu;
  screen->head = NULL;
  screen->unread = 0;
  if (screen->cursorIndex & SELECTION_CURSOR_RESET) {
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
  struct SelectionMenuWork *state = Ram_HeapSlots->selectionMenu;
  u16 cnt = state->count;

  if (cnt != SELECTION_MENU_ENTRY_LIMIT) {
    state->kinds[cnt] = first;
    state->bases[cnt] = second;
    state->count++;
  }
}

/// Creates icons for the first five listed entries in a row along the bottom of
/// the screen, then loads the selected entry's preview. Reached through the
/// far-call table and from Menu_RunSelectionForValue.
/// Creates icons for the first five listed entries in a row along the bottom of
/// the screen.
/// Builds the linked row before placing the visible icons and loading the
/// preview.
void SelectionMenu_BuildEntries(u32 unused) {
  struct SelectionMenuWork *screen = Ram_HeapSlots->selectionMenu;
  u32 count = screen->count;
  u32 index;
  struct SelectionNode *previous = 0;
  struct SelectionNode *node;
  s32 rowPosition = 0;

  for (index = screen->top; index < count; index++) {
    s32 base = screen->bases[index];
    u32 kind = screen->kinds[index];

    node =
        (struct SelectionNode *)SelectionMenu_FindFreeIcon(SELECTION_ICON_ROW);
    if (node == 0)
      break;
    SelectionMenu_SetupIcon(kind, base, node, 0);
    if (screen->head == 0) {
      screen->head = node;
      node->previous = 0;
    } else {
      previous->next = node;
      node->previous = previous;
    }
    node->next = 0;
    rowPosition++;
    previous = node;
    if (rowPosition == 5)
      break;
  }

  // Icons are 16 pixels apart: the row is centred on x 100, at y 140.
  screen->baseX = 100 - rowPosition * SELECTION_ICON_SPACING / 2;
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
    if (previous->kind == ICON_KIND_YES_NO && screen->locked == 0) {
      previous->y = 6;
      previous->targetY = 6;
    }
    previous->dx = 0;
    previous->dy = 0;
    rowPosition += SELECTION_ICON_SPACING;
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
  struct SelectionMenuWork *state =
      (struct SelectionMenuWork *)Ram_HeapSlots->selectionMenu;
  s32 i;

  if (kind != SELECTION_ICON_ROW) {
    for (i = 0; i != 5; i++) {
      if (state->records[i + SELECTION_RECORD_CHOSEN].kind == ICON_KIND_NONE)
        return (s32)&state->records[i + SELECTION_RECORD_CHOSEN];
    }
  } else {
    for (i = 0; i != 7; i++) {
      if (state->records[i + SELECTION_RECORD_ROW].kind == ICON_KIND_NONE)
        return (s32)&state->records[i + SELECTION_RECORD_ROW];
    }
  }
  return 0;
}

/// Starts drawing the selection screen each frame, in the vertical blank pass
/// (TASK_PASS_FRAME).
void SelectionMenu_StartDrawing(void) {
  Task_Add(SelectionMenu_Draw, TASK_PASS_FRAME);
}

/// Stops drawing the selection screen.
void SelectionMenu_StopDrawing(void) { Task_Remove(SelectionMenu_Draw); }

extern u32 gVBlankCount;
extern const s8 gMenu_CursorBobOffsets[];

/// The selection screen's per-frame task: slides and draws the row of icons,
/// the large icon over the chosen one, the end arrows, the icons already chosen
/// and the pointing hand.
void SelectionMenu_Draw(void) {
  struct SelectionMenuWork *state = Ram_HeapSlots->selectionMenu;
  struct SelectionSprite *cursorEntry =
      &state->records[SELECTION_RECORD_HAND].sprite;
  struct SelectionNode *cursor = &state->records[SELECTION_RECORD_HAND];
  struct SelectionNode *transfer = &state->records[SELECTION_RECORD_LARGE_ICON];
  struct SelectionNode *node;
  struct SelectionSprite *e;
  struct SelectionSprite *te;
  s32 cnt = 0;
  s32 depth;
  s32 tmp;
  struct ScreenPoint pos;

  node = state->head;
  while (node != 0) {
    e = &node->sprite;
    e->affine = 0;
    e->matrix = 0;
    e->x = node->x;
    e->y = node->y;
    depth = OAM_DEPTH_WINDOW;
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
      depth = OAM_DEPTH_WINDOW + 1;
      if (cursor->kind != ICON_KIND_NONE) {
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
      if (GameFlag_Get(FLAG_IN_BATTLE) != 0) {
        if (state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND) {
          e->mode = 1;
        } else {
          e->mode = 0;
        }
        if (node->kind == ICON_KIND_COMMAND) {
          e->mode = 1;
        }
      }
      Oam_AddSpriteAtDepth((s32 *)e, depth);
    }
    node = node->next;
    cnt++;
  }
  if (transfer->kind != ICON_KIND_NONE) {
    node = SelectionMenu_GetCursorNode(state);
    te = &transfer->sprite;
    te->mode = 0;
    te->affine = 0;
    te->matrix = 0;
    te->mosaic = 0;
    te->colors = 1;
    te->size = 2;
    te->shape = 0;
    te->priority = 0;
    te->tile = transfer->tile;
    // A 32x32 sprite over the icon, bobbing by half the hand's bob; while it
    // zooms in, its double-size area moves it 16 pixels up and left.
    te->x = node->x - 4;
    te->y =
        node->y + (gMenu_CursorBobOffsets[(gVBlankCount >> 1) & 15] >> 1) - 4;
    if (transfer->scale != transfer->scaleEnd) {
      state->transform.scaleX = transfer->scale;
      state->transform.scaleY = transfer->scale;
      state->transform.angle = 0;
      te->matrix = OamMatrix_AllocFromTransform(&state->transform);
      te->affine = 3;
      te->x += (u16)-16;
      te->y += (u8)-16;
      transfer->scale = transfer->scale + transfer->scaleStep;
    }
    if (GameFlag_Get(FLAG_IN_BATTLE) != 0) {
      te->mode = 1;
    }
    Oam_AddSpriteAtDepth((s32 *)te, OAM_DEPTH_WINDOW + 8);
  }
  SelectionMenu_DrawArrow(state, 0);
  SelectionMenu_DrawArrow(state, 1);
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
      e->matrix = OamMatrix_AllocFromTransform(&state->transform);
      e->affine = 3;
      e->x += (u16)-8;
      e->y += (u8)-8;
    } else {
      e->affine = 0;
      e->matrix = 0;
    }
    if (GameFlag_Get(FLAG_IN_BATTLE) != 0) {
      if (state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND) {
        e->mode = 1;
      } else {
        e->mode = 0;
      }
      if (node->kind == ICON_KIND_COMMAND) {
        e->mode = 1;
      }
    }
    Oam_AddSpriteAtDepth((s32 *)e, OAM_DEPTH_WINDOW);
    node = node->next;
  }
  if (cursor->kind != ICON_KIND_NONE) {
    cnt = (s32)gMenu_AnimatedCursorTiles;
    {
      void *src = (void *)(cnt + ((gVBlankCount >> 2) & 15) * HAND_FRAME_BYTES);

      cursorEntry->tile = VramCache_Load(cursor->slot, HAND_FRAME_BYTES, src);
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
    if (GameFlag_Get(FLAG_IN_BATTLE) != 0) {
      if (state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND) {
        cursorEntry->mode = 1;
      } else {
        cursorEntry->mode = 0;
      }
    }
    Oam_AddSpriteAtDepth((s32 *)cursorEntry, OAM_DEPTH_WINDOW + 8);
  }
  state->status += 1;
}

/// The pictures of the selection row's right and left arrows
/// (sprite_cursors.s).
extern u8 gMenu_CursorActorTiles[], gMenu_CursorLeftActorTiles[];

/// Draws the selection screen's left (index 0) or right (1) arrow, nudged
/// outwards while it flashes after a scroll: the record's base holds the
/// frames left of the nudge (8 after each scroll) and is the nudge in pixels.
void SelectionMenu_DrawArrow(struct SelectionMenuWork *state, s32 side) {
  u32 animationStep = (gVBlankCount >> 2) & 7;
  struct SelectionSprite *arrow;
  const u8 *arrowTiles;
  const u8 *tmp;

  if (state->records[side].kind == ICON_KIND_NONE)
    return;
  arrow = &state->records[side].sprite;
  arrow->x = state->records[side].x;
  arrow->y = state->records[side].y;
  if (side != 0) {
    arrowTiles = gMenu_CursorActorTiles;
    if (state->records[1].base != 0)
      arrow->x = arrow->x + state->records[1].base;
  } else {
    arrowTiles = gMenu_CursorLeftActorTiles;
    if (state->records[0].base != 0)
      arrow->x = arrow->x - state->records[0].base;
  }
  tmp = arrowTiles + animationStep * ARROW_FRAME_BYTES;
  arrow->tile =
      VramCache_Load(state->records[side].slot, ARROW_FRAME_BYTES, tmp);
  if (GameFlag_Get(FLAG_IN_BATTLE)) {
    if (state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND)
      arrow->mode = 1;
    else
      arrow->mode = 0;
  }
  Oam_AddSpriteAtDepth(arrow, OAM_DEPTH_WINDOW - 2);
  if (state->records[side].base != 0)
    state->records[side].base--;
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
  struct SelectionMenuWork *screen;
  struct SelectionNode *node;
  struct Window **slot;
  struct Window *window;

  screen = Ram_HeapSlots->selectionMenu;
  node = SelectionMenu_GetCursorNode(screen);
  slot = &screen->window;
  window = *slot;
  if (window == 0) {
    if (mode == ICON_KIND_YES_NO) {
      if (screen->locked != 0) {
        *slot = Window_Create(17, 17, 5, 3, mode);
      } else {
        *slot = Window_Create(17, 0, 5, 3, mode);
      }
      screen->verticalScroll = 0;
      screen->locked = SELECTION_YES_NO_AT_BOTTOM;
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
    Text_DrawMessageAsTiles(node->message, (struct Window *)screen->window, 0,
                            0);
  } else {
    switch (mode) {
    case ICON_KIND_ABILITY:
      Text_DrawMessageAsTiles((s32)MsgAbilityNotKnown,
                              (struct Window *)screen->window, 0, 0);
      break;
    case ICON_KIND_ITEM:
      Text_DrawMessageAsTiles((s32)MsgItemNotHeld,
                              (struct Window *)screen->window, 0, 0);
      break;
    }
  }
}

/// Closes the selection screen and frees its icons, tiles and work block (heap
/// slot 18). The chosen icons go too, so the choices are only in the indexes
/// the loops returned.
void SelectionMenu_Close(void) {
  struct SelectionMenuWork *state = Ram_HeapSlots->selectionMenu;
  struct SelectionNode *node;

  SelectionMenu_StopDrawing();
  Window_Close(state->window, 2);
  wait(1);
  node = state->head;
  while (node != 0) {
    if (node->kind != ICON_KIND_NONE) {
      VramCache_FreeEntry(node->slot);
      node->kind = ICON_KIND_NONE;
    }
    node = node->next;
  }
  node = state->path;
  while (node != 0) {
    if (node->kind != ICON_KIND_NONE) {
      VramCache_FreeEntry(node->slot);
      node->kind = ICON_KIND_NONE;
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
  VramCache_FreeEntry(state->records[SELECTION_RECORD_HAND].slot);
  HeapSlot_Release(HEAP_SLOT_SELECTION_MENU);
}

/// Moves the row of icons to start at pixel (x, y), 16 pixels apart.
void SelectionMenu_SetPosition(u32 first, u32 second) {
  struct SelectionMenuWork *state;
  struct SelectionNode *node;

  state = Ram_HeapSlots->selectionMenu;
  state->baseX = first;
  state->baseY = second;
  node = state->head;
  while (node != 0) {
    node->x = first;
    node->targetX = first;
    node->y = second;
    node->targetY = second;
    node = node->next;
    first += SELECTION_ICON_SPACING;
  }
}

/// Sets up both end arrows of the selection row.
void Menu_SetupSelectionBothSides(void) {
  void *menu = Ram_HeapSlots->selectionMenu;

  SelectionMenu_SetupArrow(menu, 0);
  SelectionMenu_SetupArrow(menu, 1);
}

extern u8 gMenu_CursorActorTiles[];
extern u8 gMenu_CursorLeftActorTiles[];

/// Sets up the left (side 0) or right (1) arrow of the selection row, shown
/// when there are entries beyond that end. The first call takes the arrow's
/// VRAM cache entry; later calls only move it and decide whether it shows.
void SelectionMenu_SetupArrow(struct SelectionMenuWork *state, s32 side) {
  struct SelectionSprite *arrow = &state->records[side].sprite;
  const u8 *arrowTiles = 0;
  u32 count;

  state->records[side].kind = 0;
  if (side != 0) {
    const u8 *right = gMenu_CursorActorTiles;

    count = state->count;
    if (state->top != 0)
      count -= state->top;
    if (count > 5) {
      state->records[side].kind = 1;
      count = 5;
    }
    arrowTiles = right;
    // Just past the last of the at most five icons shown.
    state->records[1].x =
        state->baseX + SELECTION_ICON_SPACING * (count - 1) + 17;
  } else {
    arrowTiles = gMenu_CursorLeftActorTiles;
    state->records[0].x = state->baseX - 9;
    if (state->top != 0)
      state->records[0].kind = 1;
  }
  if (state->records[side].y == 0) {
    state->records[side].slot = VramCache_FindFreeEntry();
    state->records[side].tile = VramCache_Load(state->records[side].slot,
                                               ARROW_FRAME_BYTES, arrowTiles);
    state->records[side].y = state->baseY;
    state->records[side].base = 0;
    arrow->mode = 0;
    arrow->mosaic = 0;
    arrow->colors = 1;
    arrow->affine = 0;
    arrow->matrix = 0;
    arrow->size = 0;
    arrow->shape = 2;
    arrow->priority = 0;
  }
}

/// Returns the icon under the cursor: the row's icon at place cursorIndex.
struct SelectionNode *
SelectionMenu_GetCursorNode(struct SelectionMenuWork *screen) {
  struct SelectionNode *node = screen->head;
  s32 i;

  for (i = 0; i != screen->cursorIndex; ++i) {
    node = node->next;
  }
  return node;
}

/// A selection mode that allows neither moving nor choosing, only cancelling
/// with B.
#define SELECTION_FIXED 999

/// The row's last place, the fifth: the cursor stepping onto it with more
/// entries to come scrolls the row instead.
#define SELECTION_LAST_PLACE 4
#define ARROW_SCROLL_FRAMES 8

/// Runs the selection row until the player chooses (the chosen entry's index)
/// or, when mode allows it, cancels with B (-1). SELECTION_FIXED allows no
/// moving or choosing; any mode but 0 allows cancelling. Reached only through
/// a bare entry of the window far-call table.
s32 SelectionMenu_Run(s32 mode) {
  struct SelectionMenuWork *screen = Ram_HeapSlots->selectionMenu;

  SelectionMenu_HighlightIcon(screen, 0);
  for (;;) {
    wait(1);
    if (screen->verticalScroll != 0) {
      continue;
    }
    if (mode != SELECTION_FIXED) {
      if (gInput.repeat & KEY_RIGHT) {
        SelectionMenu_StepRight(screen);
      } else if (gInput.repeat & KEY_LEFT) {
        SelectionMenu_StepLeft(screen);
      } else if (gInput.pressed & KEY_A) {
        return Menu_ConfirmSelection(screen);
      }
    }
    if (mode != 0 && (gInput.pressed & KEY_B)) {
      return -1;
    }
  }
}

/// As SelectionMenu_Run, with the menu sounds.
u32 SelectionMenu_RunWithSounds(u32 mode) {
  struct SelectionMenuWork *screen = Ram_HeapSlots->selectionMenu;
  u32 result;

  for (;;) {
    wait(1);
    if (screen->verticalScroll != 0) {
      continue;
    }

    if (mode != SELECTION_FIXED) {
      if (gInput.repeat & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        SelectionMenu_StepRight(screen);
      } else if (gInput.repeat & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        SelectionMenu_StepLeft(screen);
      }

      if (gInput.pressed & KEY_A) {
        result = screen->top + screen->cursorIndex;
        if (screen->head->kind == ICON_KIND_YES_NO) {
          if (result == 0)
            Sound_PlayFar(SE_MENU_CONFIRM);
          else
            Sound_PlayFar(SE_MENU_CANCEL);
        } else {
          Sound_PlayFar(SE_MENU_CONFIRM);
        }
        return result;
      }
    }

    if (mode != 0 && (gInput.pressed & KEY_B)) {
      Sound_PlayFar(SE_MENU_CANCEL);
      return -1;
    }
  }
}

/// Moves the cursor one entry right, scrolling the row at its fourth place.
/// This variant stops at the end; SelectionMenu_StepRight also wraps around.
/// Nothing calls it.
UNUSED void Menu_MoveSelectionForward(struct SelectionMenuWork *screen) {
  u32 nextEntry = screen->top + screen->cursorIndex + 1;

  if (nextEntry == screen->count) {
    return;
  }
  SelectionMenu_RedrawIcon(screen, screen->cursorIndex);
  screen->status = NODE_STATUS_SCROLLING;
  wait(1);
  screen->cursorIndex++;
  if (screen->cursorIndex == SELECTION_LAST_PLACE &&
      nextEntry + 1 < screen->count) {
    screen->cursorIndex--;
    screen->records[1].base = ARROW_SCROLL_FRAMES;
    screen->top++;
    SelectionMenu_Scroll(screen, TRUE);
    if (screen->top + screen->cursorIndex + 2 == screen->count) {
      screen->records[1].kind = FALSE;
    }
    screen->records[0].kind = TRUE;
  }
  screen->status = NODE_STATUS_SHOWN;
  SelectionMenu_HighlightIcon(screen, screen->cursorIndex);
  wait(1);
  SelectionMenu_OpenNameWindow(screen->head->kind, 0);
  wait(1);
}

/// Moves the cursor one entry left, scrolling the row at its second place. This
/// variant stops at the beginning; SelectionMenu_StepLeft also wraps around.
void Menu_MoveSelectionBackward(struct SelectionMenuWork *screen) {
  if (screen->top == 0 && screen->cursorIndex == 0) {
    return;
  }
  SelectionMenu_RedrawIcon(screen, screen->cursorIndex);
  screen->status = NODE_STATUS_SCROLLING;
  wait(1);
  if (screen->cursorIndex == 1 && screen->top != 0) {
    screen->records[0].base = ARROW_SCROLL_FRAMES;
    screen->top--;
    SelectionMenu_Scroll(screen, FALSE);
    if (screen->top == 0) {
      screen->records[0].kind = FALSE;
    }
    screen->records[1].kind = 1;
  } else {
    screen->cursorIndex--;
  }
  screen->status = NODE_STATUS_SHOWN;
  SelectionMenu_HighlightIcon(screen, screen->cursorIndex);
  wait(1);
  SelectionMenu_OpenNameWindow(screen->head->kind, 0);
  wait(1);
}

/// Moves the cursor one entry right, scrolling the row at its edge and
/// wrapping from the last entry to the first.
void SelectionMenu_StepRight(struct SelectionMenuWork *state) {
  struct SelectionNode *node;
  u32 entryIndex;
  s32 y;
  u32 end;

  SelectionMenu_RedrawIcon(state, state->cursorIndex);
  state->status = NODE_STATUS_SCROLLING;
  wait(1);
  state->cursorIndex++;
  if (state->count > 5) {
    end = state->top + state->cursorIndex;
    if (end == state->count) {
      node = state->head;
      state->records[0].kind = FALSE;
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
                                  state->bases[entryIndex], node, 1);
          node = node->next;
          entryIndex++;
        } while (node != NULL);
      }
      state->cursorIndex = 0;
      state->top = 0;
      node = state->head;
      y = node->x + SELECTION_ICON_SPACING;
      node = node->next;
      while (node != NULL) {
        node->targetX = y;
        node->dx = 12;
        node = node->next;
        y += SELECTION_ICON_SPACING;
      }
      state->records[1].kind = TRUE;
    } else if (state->cursorIndex == SELECTION_LAST_PLACE &&
               end + 1 < state->count) {
      state->cursorIndex--;
      state->records[1].base = ARROW_SCROLL_FRAMES;
      state->top++;
      SelectionMenu_Scroll(state, TRUE);
      if (state->top + state->cursorIndex + 2 == state->count)
        state->records[1].kind = FALSE;
      state->records[0].kind = TRUE;
    }
  } else if (state->cursorIndex == state->count) {
    state->cursorIndex = 0;
  }
  state->status = NODE_STATUS_SHOWN;
  SelectionMenu_HighlightIcon(state, state->cursorIndex);
  wait(1);
  SelectionMenu_OpenNameWindow(state->head->kind, 0);
  wait(1);
}

/// Moves the cursor one entry left, scrolling the row at its edge and
/// wrapping from the first entry to the last.
void SelectionMenu_StepLeft(struct SelectionMenuWork *state) {
  struct SelectionNode *node;
  s32 i;
  s32 kind;

  SelectionMenu_RedrawIcon(state, state->cursorIndex);
  state->status = NODE_STATUS_SCROLLING;
  wait(1);
  if (state->count > 5) {
    if ((state->cursorIndex | state->top) != 0) {
      if (state->cursorIndex == 1 && state->top != 0) {
        state->records[0].base = ARROW_SCROLL_FRAMES;
        state->top--;
        SelectionMenu_Scroll(state, FALSE);
        if (state->top == 0)
          state->records[0].kind = FALSE;
        state->records[1].kind = TRUE;
      } else {
        state->cursorIndex--;
      }
    } else {
      node = state->head;
      i = SELECTION_LAST_PLACE * SELECTION_ICON_SPACING;
      kind = 0;
      state->records[1].kind = kind;
      while (node->next != NULL) {
        node->targetX = node->x + i;
        node->dx = 12;
        node = node->next;
        i -= SELECTION_ICON_SPACING;
      }
      node = state->head;
      while (node->x != node->targetX)
        wait(1);
      i = 0;
      while (i != state->count - 5)
        i++;
      node = state->head;
      state->top = i;
      state->cursorIndex = SELECTION_LAST_PLACE;
      while (node != NULL) {
        SelectionMenu_SetupIcon(state->kinds[i], state->bases[i], node, 1);
        node = node->next;
        i++;
      }
      node = state->head;
      i = state->baseX;
      while (node->next != NULL) {
        node->targetX = i;
        node->dx = -12;
        node = node->next;
        i += SELECTION_ICON_SPACING;
      }
      state->records[0].kind = TRUE;
    }
  } else if (state->cursorIndex != 0) {
    state->cursorIndex--;
  } else {
    state->cursorIndex = state->count - 1;
  }
  state->status = NODE_STATUS_SHOWN;
  SelectionMenu_HighlightIcon(state, state->cursorIndex);
  wait(1);
  kind = state->head->kind;
  SelectionMenu_OpenNameWindow(kind, 0);
  wait(1);
}

void Icon_BuildCommandIconTiles(s32 overlayIcon, s32 baseIcon, s32 *slot,
                                s32 *tileOut, s32 reuse);

/// Redraws the command icon at row place rowIndex in its plain state, in its
/// own VRAM cache entry. Other kinds of icon are left alone.
void SelectionMenu_RedrawIcon(struct SelectionMenuWork *state, u32 rowIndex) {
  struct SelectionNode *node = state->head;
  u32 output;
  u32 value;

  while (rowIndex != 0) {
    rowIndex--;
    node = node->next;
  }
  if (node->kind == ICON_KIND_COMMAND || node->kind == ICON_KIND_YES_NO) {
    u32 first = node->message - (u32)&MsgCommandName;

    value = node->slot;
    Icon_BuildCommandIconTiles(first, 0, &value, &output, 1);
  }
}

/// Redraws the command icon at row place rowIndex and loads its large icon.
/// The small icon is drawn the same as SelectionMenu_RedrawIcon draws it;
/// the highlight is the large icon.
void SelectionMenu_HighlightIcon(struct SelectionMenuWork *state,
                                 u32 rowIndex) {
  struct SelectionNode *node = state->head;
  u32 res;
  u32 value;

  while (rowIndex != 0) {
    rowIndex--;
    node = node->next;
  }
  if (node->kind == ICON_KIND_COMMAND || node->kind == ICON_KIND_YES_NO) {
    u32 id = node->message - (u32)&MsgCommandName;

    value = node->slot;
    Icon_BuildCommandIconTiles(id, 0, &value, &res, 1);
    SelectionMenu_LoadLargeIcon();
  }
}

/// Stops the battle's highlight of units (BattleHighlight_SetModesFar
/// with mode 0); Menu_ConfirmSelection calls it as an entry is chosen. It
/// passes a unit list with one place an icon, of which only the end marker
/// (BATTLE_UNIT_LIST_END) is written; mode 0 never reads the list.
void Menu_ClearBattleHighlight(struct SelectionMenuWork *screen) {
  u16 data[6];
  struct SelectionNode *node = screen->head;
  s32 count = 0;

  while (node != 0) {
    node = node->next;
    count++;
  }
  data[count] = BATTLE_UNIT_LIST_END;
  BattleHighlight_SetModesFar(data, HIGHLIGHT_OFF);
}

/// Scrolls the row one entry: forward brings an icon in from the right as the
/// leftmost shrinks away, and back the reverse; waits until it has moved.
/// Releases the departing cache slot only after its shrinking animation
/// finishes. The icons move 2 pixels a frame and zoom by 32 (in 8.8) a frame,
/// so a scroll takes 8 frames.
void SelectionMenu_Scroll(struct SelectionMenuWork *screen, s32 forward) {
  struct SelectionNode *p;
  struct SelectionNode *last;
  struct SelectionNode *first;
  s32 base;
  s32 kind;
  u32 index;
  s32 y;
  s32 z;

  if (forward != 0) {
    index = screen->top + SELECTION_LAST_PLACE;
    base = screen->bases[index];
    kind = screen->kinds[index];
    p = (struct SelectionNode *)SelectionMenu_FindFreeIcon(SELECTION_ICON_ROW);
    if (p == NULL)
      return;
    SelectionMenu_SetupIcon(kind, base, p, 0);
    y = screen->baseX;
    p->x = y + (SELECTION_LAST_PLACE + 1) * SELECTION_ICON_SPACING;
    z = screen->baseY;
    p->targetX = y + SELECTION_LAST_PLACE * SELECTION_ICON_SPACING;
    p->y = z;
    p->dx = -2;
    p->targetY = z;
    p->scaleStep = Q_8_8(1) / 8;
    p->scale = Q_8_8(1) / 8;
    p->scaleEnd = Q_8_8(1);
    last = p;
    p = screen->head;
    p->scaleStep = -Q_8_8(1) / 8;
    p->scaleEnd = 0;
    for (;;) {
      p->targetX = p->x - SELECTION_ICON_SPACING;
      p->dx = -2;
      if (p->next == NULL)
        break;
      p = p->next;
    }
    p->next = last;
    last->next = NULL;
    last->previous = p;
    p = screen->head;
    do {
      wait(1);
    } while (p->scale != 0);
    screen->head = p->next;
    VramCache_FreeEntry(p->slot);
    p->kind = ICON_KIND_NONE;
    p = p->next;
    p->previous = NULL;
  } else {
    index = screen->top;
    base = screen->bases[index];
    kind = screen->kinds[index];
    p = (struct SelectionNode *)SelectionMenu_FindFreeIcon(SELECTION_ICON_ROW);
    if (p == NULL)
      return;
    SelectionMenu_SetupIcon(kind, base, p, 0);
    p->x = screen->baseX - SELECTION_ICON_SPACING;
    p->y = screen->baseY;
    p->targetY = p->y;
    p->dx = 2;
    p->scale = Q_8_8(1) / 8;
    p->scaleStep = Q_8_8(1) / 8;
    p->targetX = p->x + SELECTION_ICON_SPACING;
    p->scaleEnd = Q_8_8(1);
    first = p;
    p = screen->head;
    p->previous = first;
    first->next = p;
    first->previous = NULL;
    screen->head = first;
    p = first;
    for (;;) {
      p->targetX = p->x + SELECTION_ICON_SPACING;
      p->dx = 2;
      if (p->next == NULL)
        break;
      p = p->next;
    }
    p->scaleEnd = 0;
    p->scaleStep = -Q_8_8(1) / 8;
    p = screen->head;
    do {
      wait(1);
    } while (p->scale != Q_8_8(1));
    while (p->next != NULL)
      p = p->next;
    VramCache_FreeEntry(p->slot);
    p->kind = ICON_KIND_NONE;
    p->previous->next = NULL;
  }
}

void Icon_BuildCommandIconTiles(s32 overlayIcon, s32 baseIcon, s32 *slot,
                                s32 *tileOut, s32 reuse);

/// Loads the icon of an entry of kind mode (enum IconKind: a command, a yes or
/// no, an item or a Psynergy) into a free sprite tile slot and returns the
/// slot, or -1 when none was free or mode is another kind.
s32 Resource_LoadByMode(u32 mode, s32 value) {
  s32 output;
  s32 result = -1;

  switch (mode) {
  case ICON_KIND_COMMAND:
  case ICON_KIND_YES_NO:
    Icon_BuildCommandIconTiles(value, 0, &result, &output, 0);
    break;
  case ICON_KIND_ITEM:
    result = VramCache_FindFreeEntry();
    if (result == VRAM_CACHE_ENTRY_COUNT)
      return -1;
    Icon_CopyResourceToSlot(
        value, ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_BROKEN, result);
    break;
  case ICON_KIND_ABILITY:
    Ability_LoadGlyph(value, 1, &result, &output, 0);
    break;
  }
  return result;
}

/// Loads the icon of an entry of kind mode into sprite tile slot result (a free
/// slot when -1) and returns the slot, or -1 when none was free.
s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option) {
  s32 output;
  s32 original = result;

  if (result == -1) {
    result = VramCache_FindFreeEntry();
    if (result == VRAM_CACHE_ENTRY_COUNT)
      return original;
  }

  switch (mode) {
  case ICON_KIND_COMMAND:
  case ICON_KIND_YES_NO:
    Icon_BuildCommandIconTiles(value, option, &result, &output, 1);
    break;
  case ICON_KIND_ITEM:
    Icon_CopyResourceToSlot(value,
                            ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_BROKEN |
                                ICON_ARTIFACT,
                            result);
    break;
  case ICON_KIND_EQUIPMENT_ITEM:
    Icon_CopyResourceToSlot(
        value, ICON_COUNT_ABOVE_ONE | ICON_BROKEN | ICON_ARTIFACT, result);
    break;
  case ICON_KIND_ABILITY:
    Ability_LoadGlyph(value, option, &result, &output, 1);
    break;
  case ICON_KIND_PATTERN:
    Icon_BuildMiscIconTiles(value, 0, result);
    break;
  }

  return result;
}

void Icon_BuildCommandIconTiles(s32 overlayIcon, s32 baseIcon, s32 *slot,
                                s32 *tileOut, s32 reuse);
void Icon_BuildItemIconTiles(u32 glyph, s32 includeBase, s32 *sourceIndex,
                             s32 *result, s32 reuseSource);

/// Sets up a selection icon for an entry: its icon tiles (in its old slot when
/// reuse is set), its name message and its sprite.
/// kind must select a command, item or Psynergy with the matching base ID.
/// Items and Psynergy are drawn over the icon base; the sprite is 16x16 at
/// full size (Q_8_8(1)).
void SelectionMenu_SetupIcon(u32 kind, s32 base, struct SelectionNode *node,
                             s32 reuse) {
  s32 cacheSlot;
  s32 tile;
  struct SelectionSprite *sprite;

  switch (kind) {
  case ICON_KIND_COMMAND:
  case ICON_KIND_YES_NO:
    if (reuse != 0)
      cacheSlot = node->slot;
    Icon_BuildCommandIconTiles(base, 0, &cacheSlot, &tile, reuse);
    node->message = base + MsgCommandName;
    break;
  case ICON_KIND_ITEM:
    if (reuse != 0)
      cacheSlot = node->slot;
    Icon_BuildItemIconTiles(base, 1, &cacheSlot, &tile, reuse);
    node->message = base + MsgItemName;
    break;
  case ICON_KIND_ABILITY:
    if (reuse != 0)
      cacheSlot = node->slot;
    Ability_LoadGlyph(base, 1, &cacheSlot, &tile, reuse);
    node->message = base + MsgAbilityName;
    break;
  }

  node->base = base;
  node->slot = cacheSlot;
  node->tile = tile;
  node->kind = kind;
  node->scale = Q_8_8(1);
  node->scaleEnd = Q_8_8(1);

  sprite = &node->sprite;
  sprite->mode = 0;
  sprite->colors = 0;
  sprite->mosaic = 0;

  sprite->size = 1;
  sprite->shape = 0;
  sprite->palette = 0;
  sprite->tile = tile;
  sprite->priority = 0;
}

/// Chooses the entry under the cursor: the other icons gather into it and
/// vanish, and it joins the chosen icons at the top left.
u32 Menu_ConfirmSelection(struct SelectionMenuWork *screen) {
  struct SelectionNode *node;
  struct SelectionNode *other = NULL;
  u32 cnt = 0;
  u32 index;
  struct SelectionSprite *sprite;

  index = screen->top + screen->cursorIndex;
  Menu_ClearBattleHighlight(screen);
  SelectionMenu_RedrawIcon(screen, screen->cursorIndex);
  screen->status = NODE_STATUS_SCROLLING;
  wait(1);
  screen->records[0].kind = 0;
  screen->records[1].kind = 0;
  screen->records[SELECTION_RECORD_HAND].kind = 0;
  screen->records[SELECTION_RECORD_HAND].scale = 0;
  Resource_ResetPendingTransfer();
  node = screen->head;
  for (; node != NULL && screen->cursorIndex != cnt; cnt++) {
    node = node->next;
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
      other->kind = ICON_KIND_NONE;
    }
  }
  screen->head = node;
  node->previous = NULL;
  node->next = NULL;
  node->targetX = 4;
  for (other = screen->path, cnt = 0; other != NULL; other = other->next) {
    node->targetX += SELECTION_ICON_SPACING;
    cnt++;
  }
  screen->pathTop[cnt] = screen->top;
  screen->pathCursor[cnt] = screen->cursorIndex;
  node->dx = (node->targetX - node->x) >> 1;
  screen->unread = 0;
  screen->cursorIndex |= SELECTION_CURSOR_RESET;
  wait(2);
  other =
      (struct SelectionNode *)SelectionMenu_FindFreeIcon(SELECTION_ICON_CHOSEN);
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
  other->scale = Q_8_8(1);
  other->scaleEnd = Q_8_8(1);
  sprite = &other->sprite;
  sprite->mode = 0;
  sprite->colors = 0;
  sprite->mosaic = 0;
  sprite->size = 1;
  sprite->shape = 0;
  sprite->palette = 0;
  sprite->tile = other->tile;
  node->kind = ICON_KIND_NONE;
  screen->head = NULL;
  if (screen->path != NULL) {
    node = screen->path;
    while (node->next != NULL)
      node = node->next;
    node->next = other;
    other->previous = node;
    other->next = NULL;
  } else {
    screen->path = other;
    other->previous = NULL;
    other->next = NULL;
  }
  return index;
}

/// Six empty functions; nothing calls the first, the fourth or the fifth. The
/// others are reached only through bare entries of the window far-call table.
/// With no body or caller to give them a role, they are lettered in order.
void MenuArrow_NoOpCallback(void) {}

/// Does nothing.
void MenuArrow_NoOpCallbackBank(void) {}

/// Does nothing.
void MenuArrow_NoOpCallbackA(void) {}

/// Does nothing.
void MenuArrow_NoOpCallbackB(void) {}

/// Does nothing.
void MenuArrow_NoOpCallbackC(void) {}

/// Does nothing.
void MenuArrow_NoOpCallbackD(void) {}

/// Sets up a sprite as the right-pointing arrow, in a free tile slot (returned
/// in *slot).
/// The caller owns the cache slot and releases it through
/// MenuArrow_Release.
void MenuArrow_Create(struct MenuSprite *sprite, s32 *slot) {
  const void *data = gMenu_CursorActorTiles;
  s32 cacheSlot = VramCache_FindFreeEntry();

  *slot = cacheSlot;
  sprite->oam.f.tile = VramCache_Load(cacheSlot, ARROW_FRAME_BYTES, data);
  sprite->oam.f.mode = 0;
  sprite->oam.f.mosaic = 0;
  sprite->oam.f.colors = 1;
  sprite->oam.f.affine = 0;
  sprite->oam.f.affineIndex = 0;
  sprite->oam.f.size = 0;
  sprite->oam.f.shape = 2;
  sprite->oam.f.priority = 0;
}

/// Places an arrow sprite at screen pixel (x, y) and queues it for drawing
/// this frame, at depth OAM_DEPTH_FRONT - 3; the caller calls it every frame it
/// shows.
void MenuArrow_SetPosition(struct MenuSprite *sprite, s32 x, s32 y) {
  sprite->oam.f.x = x;
  sprite->oam.f.y = y;
  Oam_AddSpriteAtDepth(sprite, OAM_DEPTH_FRONT - 3);
}

/// Frees an arrow sprite's tile slot.
void MenuArrow_Release(s32 value) { VramCache_FreeEntry(value); }

/// The large icon's zoom: it opens at this scale (in 8.8) and grows by as much
/// each frame until six steps, when SelectionMenu_Draw shows it at its own
/// size.
#define LARGE_ICON_ZOOM_STEP 40

/// Loads the large icon of the command under the cursor, zooming in: 32x32 in
/// 256 colours (32 * 32 bytes, SELECTION_RECORD_LARGE_ICON), its scale growing
/// by LARGE_ICON_ZOOM_STEP a frame.
/// Other kinds of entry show none. The icon keeps its VRAM cache entry until
/// Resource_ResetPendingTransfer.
void SelectionMenu_LoadLargeIcon(void) {
  struct SelectionMenuWork *state = Ram_HeapSlots->selectionMenu;
  struct SelectionNode *node = SelectionMenu_GetCursorNode(state);
  struct SelectionNode *transfer;
  GlyphTransfer *buffer;
  u8 *table;
  void *resource;
  s32 no;

  if (node->kind != ICON_KIND_COMMAND && node->kind != ICON_KIND_YES_NO)
    return;

  buffer = (GlyphTransfer *)HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER,
                                           sizeof(GlyphTransfer));
  transfer = &state->records[SELECTION_RECORD_LARGE_ICON];
  no = node->base;
  table = Resource_GetTableEntry(RESOURCE_COMMAND_ICONS);
  {
    u8 **destination = &buffer->encoded;
    resource = table + ((u16 *)table)[node->base];
    *destination = resource;
  }
  Decompress_ByteLz(resource, buffer);

  if (transfer->kind == ICON_KIND_NONE)
    transfer->slot = VramCache_FindFreeEntry();
  transfer->tile = VramCache_Load(transfer->slot, 32 * 32, buffer);
  transfer->kind = ICON_KIND_COMMAND;
  transfer->base = no;
  transfer->scale = LARGE_ICON_ZOOM_STEP;
  transfer->scaleStep = LARGE_ICON_ZOOM_STEP;
  transfer->scaleEnd = 6 * LARGE_ICON_ZOOM_STEP;
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
}

/// Frees the large icon's VRAM cache entry, if it holds one, and marks it
/// empty.
void Resource_ResetPendingTransfer(void) {
  struct SelectionNode *transfer =
      &((struct SelectionMenuWork *)Ram_HeapSlots->selectionMenu)
           ->records[SELECTION_RECORD_LARGE_ICON];

  if (transfer->kind != ICON_KIND_NONE) {
    VramCache_FreeEntry(transfer->slot);
    transfer->kind = ICON_KIND_NONE;
  }
}

void PartyStatus_CreateWindow(s32 flags);
s32 FieldMenu_SelectCommand(s32);

/// Runs the field menu until the player picks something that leaves it, and
/// returns the chosen screen's result. Cancelling the menu returns an
/// undefined value, the missing result of PartyStatus_CloseWithMenu (0 under
/// UBFIX); its one caller, FieldSequence_Run, ignores the result. Its case for
/// command 0, which stores the trigger tile ahead in the field work's
/// examineCell, is unreachable.
s32 FieldMenu_Run(void) {
  s32 ret;
  s32 selection;
  struct FieldWork *state;

  state = Ram_HeapSlots->fieldWork;
  selection = 0;

  for (;;) {
    PartyStatus_OpenAndWait();
    selection = FieldMenu_SelectCommand(selection);
#ifdef UBFIX
    // UB: the original reads the void return of PartyStatus_CloseWithMenu,
    // called with no prototype in scope, as The Broken Seal's FieldMenu_Run
    // does.
    PartyStatus_CloseWithMenu();
    ret = 0;
#else
    ret = PartyStatus_CloseWithMenu();
#endif

    // A screen left without a choice (-1, or nonzero from the Djinn screen)
    // opens the field menu again.
    switch (selection) {
    case 0:
      ret = Actor_GetTriggerCellAheadOfCurrentFar();
      if (ret == 0)
        ret = CELL_BEHAVIOR_WALL;
      state->examineCell = ret;
      break;
    case FIELD_MENU_PSYNERGY:
      ret = PsynergyMenu_RunFar();
      if (ret == -1)
        continue;
      break;
    case FIELD_MENU_DJINN:
      ret = DjinnMenu_RunFar();
      if (ret != 0)
        continue;
      break;
    case FIELD_MENU_ITEM:
      ret = ItemMenu_RunFar();
      if (ret == -1)
        continue;
      break;
    case FIELD_MENU_STATUS:
      ret = StatusScreen_RunFar();
      if (ret == -1)
        continue;
      break;
    default:
      break;
    }
    break;
  }

  return ret;
}

/// Opens the party status window that shows with the field menu (flags 1) and
/// waits a frame.
void PartyStatus_OpenAndWait(void) {
  PartyStatus_CreateWindow(1);
  wait(1);
}

/// Closes the window in heap slot 16 and releases its block.
void PartyStatus_CloseWithMenu(void) { PartyStatus_Close(); }

/// Sets up an empty selection screen and waits a frame.
void SelectionMenu_CreateEmpty(void) {
  SelectionMenu_Create();
  SelectionMenu_Clear();
  wait(1);
}

void SelectionMenu_BuildEntries(u32);

/// Runs a selection row with the cursor on entry initialIndex, cancellable, and
/// returns the entry chosen or -1. initialIndex is a row index; cancellation
/// returns -1 encoded as u32.
u32 Menu_RunSelectionForValue(u32 value) {
  struct SelectionMenuWork *state = Ram_HeapSlots->selectionMenu;
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
#undef SELECTION_LAST_PLACE
#undef ARROW_SCROLL_FRAMES

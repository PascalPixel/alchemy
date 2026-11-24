/// @file
/// The selection screen, the row of icons a command, item or Psynergy is
/// picked from in battle and on the field, and the field menu.
// Part of window.c.

#include "window_internal.h"
#include "resource_ids.h"
#include "field/field_state.h"
#include "field/map_grid.h"
#include "flags.h"
#include "sounds.h"
#include "field/particle.h"
#include "window/menu_list.h"

/// The selection row's icons stand 16 pixels apart.
#define SELECTION_ICON_SPACING 16

/// The row's places on screen, its icons (two more, for those sliding in and
/// out) and the icons chosen so far.
#define SELECTION_ROW_PLACES 5
#define SELECTION_ROW_ICONS 7
#define SELECTION_CHOSEN_ICONS 5

/// The arrows at the row's ends: their records in SelectionMenuState.records,
/// and the side SelectionMenu_SetupArrow and SelectionMenu_DrawArrow take.
enum SelectionArrow { SELECTION_ARROW_LEFT = 0, SELECTION_ARROW_RIGHT = 1 };

/// SelectionMenuState.verticalScroll as the screen is made: the row's icons
/// drawn 32 pixels lower, below the bottom of the screen, and the cursor held
/// still, until SelectionMenu_Clear readies the list.
#define SELECTION_ROW_HIDDEN 32

/// SelectionMenuState.locked as the screen is made and once a yes or no window
/// has opened: nonzero keeps a yes or no row and its window at the bottom of
/// the screen, as Menu_RunSelectionForValue's 1 does.
#define SELECTION_YES_NO_AT_BOTTOM 999

/// One frame of an end arrow's animation: an 8x16 sprite of 256 colours.
#define ARROW_FRAME_BYTES (2 * TILE_SIZE_8BPP)
/// One frame of the pointing hand's animation: a 16x16 sprite of 256 colours.
#define HAND_FRAME_BYTES (4 * TILE_SIZE_8BPP)

// Called before their definitions.
s32 SelectionMenu_FindFreeIcon(s32 kind);
void SelectionMenu_LoadLargeIcon(void);
void SelectionMenu_Draw(void);
void SelectionMenu_DrawArrow(struct SelectionMenuState *state, s32 side);
void SelectionMenu_SetupArrow(struct SelectionMenuState *state, s32 index);
void SelectionMenu_StepRight(struct SelectionMenuState *state);
void SelectionMenu_StepLeft(struct SelectionMenuState *state);
void SelectionMenu_RedrawIcon(struct SelectionMenuState *state, u32 rowIndex);
void SelectionMenu_HighlightIcon(struct SelectionMenuState *state,
                                 u32 rowIndex);
void SelectionMenu_Scroll(struct SelectionMenuState *screen, s32 forward);
void SelectionMenu_SetupIcon(u32 kind, s32 base, struct SelectionNode *node,
                             s32 reuse);
u32 Menu_ConfirmSelection(struct SelectionMenuState *screen);
void PartyStatus_OpenAndWait(void);

extern const u8 gMenu_AnimatedCursorTiles[];

/// Allocates and resets the selection screen: the row of icons the player picks
/// from, with arrows at each end and a pointing hand over the chosen target,
/// whose picture takes a VRAM cache entry.
void SelectionMenu_Create(void) {
  struct SelectionMenuState *screen;
  struct SelectionSprite *visual;
  const u8 *table;
  s32 i;
  u16 clear = 0;

  screen = HeapSlot_AllocEwram(HEAP_SLOT_SELECTION_MENU,
                               sizeof(struct SelectionMenuState));
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
  } while (i != SELECTION_CHOSEN_ICONS);
  // The row has two icons more than the chosen ones.
  screen->records[i + SELECTION_RECORD_ROW].kind = ICON_KIND_NONE;
  screen->records[i + SELECTION_RECORD_ROW + 1].kind = ICON_KIND_NONE;
  screen->records[SELECTION_ARROW_LEFT].kind = FALSE;
  screen->records[SELECTION_ARROW_RIGHT].kind = FALSE;
  screen->records[SELECTION_ARROW_LEFT].y = 0;
  screen->records[SELECTION_ARROW_RIGHT].y = 0;
  table = gMenu_AnimatedCursorTiles;
  screen->records[SELECTION_RECORD_HAND].slot = VramCache_FindFreeEntry();
  screen->records[SELECTION_RECORD_HAND].tile = VramCache_Load(
      screen->records[SELECTION_RECORD_HAND].slot, HAND_FRAME_BYTES, table);
  screen->records[SELECTION_RECORD_HAND].kind = 0;
  screen->records[SELECTION_RECORD_HAND].scale = 0;
  screen->records[SELECTION_RECORD_LARGE_ICON].kind = ICON_KIND_NONE;
  visual = &screen->records[SELECTION_RECORD_HAND].sprite;
  visual->mode = OBJ_MODE_NORMAL;
  visual->mosaic = 0;
  visual->colors = OBJ_COLORS_256;
  visual->affine = OAM_AFFINE_OFF;
  visual->matrix = 0;
  visual->size = 1;
  visual->shape = OBJ_SHAPE_SQUARE;
  visual->priority = 0;
}

/// Empties the selection screen's list of entries; after an entry was chosen
/// (SELECTION_CURSOR_RESET) the next list starts again from its first entry.
void SelectionMenu_Clear(void) {
  struct SelectionMenuState *screen;

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

/// Adds an entry to the selection screen's list, up to
/// SELECTION_MENU_ENTRY_LIMIT. The kinds SelectionMenu_SetupIcon draws (enum
/// IconKind) are ICON_KIND_COMMAND, ICON_KIND_YES_NO, ICON_KIND_ITEM and
/// ICON_KIND_ABILITY; base is the command's, item's or ability's number.
void SelectionMenu_AddEntry(u32 kind, u32 base) {
  struct SelectionMenuState *state = Ram_HeapSlots->selectionMenu;
  u16 count = state->count;

  if (count != SELECTION_MENU_ENTRY_LIMIT) {
    state->kinds[count] = kind;
    state->bases[count] = base;
    state->count++;
  }
}

/// Makes the icons of the listed entries from top, as many as the row's places,
/// in a row along the bottom of the screen, and loads the large icon of the
/// one under the cursor.
void SelectionMenu_BuildEntries(u32 unused) {
  struct SelectionMenuState *screen = Ram_HeapSlots->selectionMenu;
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
    if (rowPosition == SELECTION_ROW_PLACES)
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

/// Does nothing; reached only through the window far-call table.
void SelectionMenu_ReservedNoOp(void) {}

/// Returns a free selection icon record of `kind` (enum SelectionIconGroup),
/// or NULL.
s32 SelectionMenu_FindFreeIcon(s32 kind) {
  struct SelectionMenuState *state =
      (struct SelectionMenuState *)Ram_HeapSlots->selectionMenu;
  s32 i;

  if (kind != SELECTION_ICON_ROW) {
    for (i = 0; i != SELECTION_CHOSEN_ICONS; i++) {
      if (state->records[i + SELECTION_RECORD_CHOSEN].kind == ICON_KIND_NONE)
        return (s32)&state->records[i + SELECTION_RECORD_CHOSEN];
    }
  } else {
    for (i = 0; i != SELECTION_ROW_ICONS; i++) {
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
  struct SelectionMenuState *state = Ram_HeapSlots->selectionMenu;
  struct SelectionSprite *cursorEntry =
      &state->records[SELECTION_RECORD_HAND].sprite;
  struct SelectionNode *cursor = &state->records[SELECTION_RECORD_HAND];
  struct SelectionNode *largeIcon =
      &state->records[SELECTION_RECORD_LARGE_ICON];
  struct SelectionNode *node;
  struct SelectionSprite *sprite;
  struct SelectionSprite *largeSprite;
  s32 place = 0;
  s32 depth;
  s32 step;
  struct ScreenPoint pos;

  node = state->head;
  while (node != 0) {
    sprite = &node->sprite;
    sprite->affine = OAM_AFFINE_OFF;
    sprite->matrix = 0;
    sprite->x = node->x;
    sprite->y = node->y;
    depth = OAM_DEPTH_WINDOW;
    if (state->verticalScroll != 0) {
      sprite->y += state->verticalScroll;
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
      sprite->x = node->x;
    } else if (place == state->cursorIndex) {
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
          sprite->mode = OBJ_MODE_SEMI_TRANSPARENT;
        } else {
          sprite->mode = OBJ_MODE_NORMAL;
        }
        if (node->kind == ICON_KIND_COMMAND) {
          sprite->mode = OBJ_MODE_SEMI_TRANSPARENT;
        }
      }
      Oam_AddSpriteAtDepth((s32 *)sprite, depth);
    }
    node = node->next;
    place++;
  }
  if (largeIcon->kind != ICON_KIND_NONE) {
    node = SelectionMenu_GetCursorNode(state);
    largeSprite = &largeIcon->sprite;
    largeSprite->mode = OBJ_MODE_NORMAL;
    largeSprite->affine = OAM_AFFINE_OFF;
    largeSprite->matrix = 0;
    largeSprite->mosaic = 0;
    largeSprite->colors = OBJ_COLORS_256;
    largeSprite->size = 2;
    largeSprite->shape = OBJ_SHAPE_SQUARE;
    largeSprite->priority = 0;
    largeSprite->tile = largeIcon->tile;
    // A 32x32 sprite over the icon, bobbing by half the hand's bob; while it
    // zooms in, its double-size area moves it 16 pixels up and left.
    largeSprite->x = node->x - 4;
    largeSprite->y =
        node->y + (gMenu_CursorBobOffsets[(gVBlankCount >> 1) & 15] >> 1) - 4;
    if (largeIcon->scale != largeIcon->scaleEnd) {
      state->transform.scaleX = largeIcon->scale;
      state->transform.scaleY = largeIcon->scale;
      state->transform.angle = 0;
      largeSprite->matrix = OamMatrix_AllocFromTransform(&state->transform);
      largeSprite->affine = OAM_AFFINE_DOUBLE_SIZE;
      largeSprite->x += (u16)-16;
      largeSprite->y += (u8)-16;
      largeIcon->scale = largeIcon->scale + largeIcon->scaleStep;
    }
    if (GameFlag_Get(FLAG_IN_BATTLE) != 0) {
      largeSprite->mode = OBJ_MODE_SEMI_TRANSPARENT;
    }
    Oam_AddSpriteAtDepth((s32 *)largeSprite, OAM_DEPTH_WINDOW + 8);
  }
  SelectionMenu_DrawArrow(state, SELECTION_ARROW_LEFT);
  SelectionMenu_DrawArrow(state, SELECTION_ARROW_RIGHT);
  node = state->path;
  while (node != 0) {
    sprite = &node->sprite;
    if (node->x != node->targetX) {
      node->x = node->x + node->dx;
    }
    if (node->y != node->targetY) {
      node->y = node->y + node->dy;
    }
    sprite->x = node->x;
    sprite->y = node->y;
    if (node->scale != node->scaleEnd) {
      node->scale = node->scale + node->scaleStep;
      state->transform.scaleX = node->scale;
      state->transform.scaleY = node->scale;
      state->transform.angle = 0;
      sprite->matrix = OamMatrix_AllocFromTransform(&state->transform);
      sprite->affine = OAM_AFFINE_DOUBLE_SIZE;
      sprite->x += (u16)-8;
      sprite->y += (u8)-8;
    } else {
      sprite->affine = OAM_AFFINE_OFF;
      sprite->matrix = 0;
    }
    if (GameFlag_Get(FLAG_IN_BATTLE) != 0) {
      if (state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND) {
        sprite->mode = OBJ_MODE_SEMI_TRANSPARENT;
      } else {
        sprite->mode = OBJ_MODE_NORMAL;
      }
      if (node->kind == ICON_KIND_COMMAND) {
        sprite->mode = OBJ_MODE_SEMI_TRANSPARENT;
      }
    }
    Oam_AddSpriteAtDepth((s32 *)sprite, OAM_DEPTH_WINDOW);
    node = node->next;
  }
  if (cursor->kind != ICON_KIND_NONE) {
    place = (s32)gMenu_AnimatedCursorTiles;
    {
      void *src =
          (void *)(place + ((gVBlankCount >> 2) & 15) * HAND_FRAME_BYTES);

      cursorEntry->tile = VramCache_Load(cursor->slot, HAND_FRAME_BYTES, src);
    }
    if (cursor->targetX != cursor->x) {
      step = (cursor->targetX - cursor->x) >> 1;
      if (step != 0) {
        cursor->x = cursor->x + step;
      } else {
        cursor->x = cursor->targetX;
      }
    }
    if (cursor->targetY != cursor->y) {
      if ((step = (cursor->targetY - cursor->y) >> 1) != 0) {
        cursor->y = cursor->y + step;
      } else {
        cursor->y = cursor->targetY;
      }
    }
    cursorEntry->y =
        cursor->y + gMenu_CursorBobOffsets[(gVBlankCount >> 2) & 15] - 32;
    cursorEntry->x = cursor->x - 4;
    if (GameFlag_Get(FLAG_IN_BATTLE) != 0) {
      if (state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND) {
        cursorEntry->mode = OBJ_MODE_SEMI_TRANSPARENT;
      } else {
        cursorEntry->mode = OBJ_MODE_NORMAL;
      }
    }
    Oam_AddSpriteAtDepth((s32 *)cursorEntry, OAM_DEPTH_WINDOW + 8);
  }
  state->status += 1;
}

/// The pictures of the selection row's right and left arrows
/// (sprite_cursors.s).
extern u8 gMenu_CursorActorTiles[], gMenu_CursorLeftActorTiles[];

/// Draws the selection screen's left or right arrow (enum SelectionArrow),
/// nudged outwards while it flashes after a scroll: the record's base holds the
/// frames left of the nudge (8 after each scroll) and is the nudge in pixels.
void SelectionMenu_DrawArrow(struct SelectionMenuState *state, s32 side) {
  u32 animationStep = (gVBlankCount >> 2) & 7;
  struct SelectionSprite *arrow;
  const u8 *arrowTiles;
  const u8 *tmp;

  if (state->records[side].kind == ICON_KIND_NONE)
    return;
  arrow = &state->records[side].sprite;
  arrow->x = state->records[side].x;
  arrow->y = state->records[side].y;
  if (side != SELECTION_ARROW_LEFT) {
    arrowTiles = gMenu_CursorActorTiles;
    if (state->records[SELECTION_ARROW_RIGHT].base != 0)
      arrow->x = arrow->x + state->records[SELECTION_ARROW_RIGHT].base;
  } else {
    arrowTiles = gMenu_CursorLeftActorTiles;
    if (state->records[SELECTION_ARROW_LEFT].base != 0)
      arrow->x = arrow->x - state->records[SELECTION_ARROW_LEFT].base;
  }
  tmp = arrowTiles + animationStep * ARROW_FRAME_BYTES;
  arrow->tile =
      VramCache_Load(state->records[side].slot, ARROW_FRAME_BYTES, tmp);
  if (GameFlag_Get(FLAG_IN_BATTLE)) {
    if (state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND)
      arrow->mode = OBJ_MODE_SEMI_TRANSPARENT;
    else
      arrow->mode = OBJ_MODE_NORMAL;
  }
  Oam_AddSpriteAtDepth(arrow, OAM_DEPTH_WINDOW - 2);
  if (state->records[side].base != 0)
    state->records[side].base--;
}

void WindowSprite_PrepareForRedraw(struct Window *);

/// Opens or refits the name window above the selection row and writes the name
/// of the icon under the cursor, or that nothing is known (ICON_KIND_ABILITY)
/// or held (ICON_KIND_ITEM). ICON_KIND_YES_NO is a yes or no question, placed
/// at the bottom or top of the screen. count is the name's width in tiles
/// inside the frame; 0 keeps the open window as it is. The window stays open
/// until SelectionMenu_Close closes it.
void SelectionMenu_OpenNameWindow(s32 mode, u32 count) {
  struct SelectionMenuState *screen;
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
        *slot = Window_Create(17, 17, 5, 3,
                              WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
      } else {
        *slot = Window_Create(17, 0, 5, 3,
                              WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
      }
      screen->verticalScroll = 0;
      screen->locked = SELECTION_YES_NO_AT_BOTTOM;
    } else {
      *slot = Window_Create(19 + ((9 - count) >> 1), 17, count + 2, 3,
                            WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
    }
    WindowSprite_PrepareForRedraw(screen->window);
  } else {
    if (count != 0 && window->width != count + 2) {
      Window_Close(window, WINDOW_FLAG_INSTANT);
      *slot = Window_Create(19 + ((9 - count) >> 1), 17, count + 2, 3,
                            WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
    }
    WindowSprite_PrepareForRedraw(screen->window);
  }
  if (screen->count != 0) {
    Text_DrawMessageAsTiles(node->message, screen->window, 0, 0);
  } else {
    switch (mode) {
    case ICON_KIND_ABILITY:
      Text_DrawMessageAsTiles((s32)MsgAbilityNotKnown, screen->window, 0, 0);
      break;
    case ICON_KIND_ITEM:
      Text_DrawMessageAsTiles((s32)MsgItemNotHeld, screen->window, 0, 0);
      break;
    }
  }
}

/// Closes the selection screen and frees its icons, tiles and state. The
/// chosen icons go too, so the choices are only in the indexes the loops
/// returned.
void SelectionMenu_Close(void) {
  struct SelectionMenuState *state = Ram_HeapSlots->selectionMenu;
  struct SelectionNode *node;

  SelectionMenu_StopDrawing();
  Window_Close(state->window, WINDOW_FLAG_INSTANT);
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
  SelectionMenu_FreeLargeIcon();
  // The arrows' entries, taken together by Menu_SetupSelectionBothSides.
  if (state->records[SELECTION_ARROW_LEFT].y != 0) {
    VramCache_FreeEntry(state->records[SELECTION_ARROW_LEFT].slot);
    // DEAD: the left arrow tested again, whose y is always the right one's;
    // the ROM keeps its load and branch.
    if (state->records[SELECTION_ARROW_LEFT].y != 0) {
      VramCache_FreeEntry(state->records[SELECTION_ARROW_RIGHT].slot);
    }
  }
  VramCache_FreeEntry(state->records[SELECTION_RECORD_HAND].slot);
  HeapSlot_Release(HEAP_SLOT_SELECTION_MENU);
}

/// Moves the row of icons to start at pixel (x, y), 16 pixels apart.
void SelectionMenu_SetPosition(u32 x, u32 y) {
  struct SelectionMenuState *state;
  struct SelectionNode *node;

  state = Ram_HeapSlots->selectionMenu;
  state->baseX = x;
  state->baseY = y;
  node = state->head;
  while (node != 0) {
    node->x = x;
    node->targetX = x;
    node->y = y;
    node->targetY = y;
    node = node->next;
    x += SELECTION_ICON_SPACING;
  }
}

/// Sets up both end arrows of the selection row.
void Menu_SetupSelectionBothSides(void) {
  void *menu = Ram_HeapSlots->selectionMenu;

  SelectionMenu_SetupArrow(menu, SELECTION_ARROW_LEFT);
  SelectionMenu_SetupArrow(menu, SELECTION_ARROW_RIGHT);
}

/// Sets up the left or right arrow of the selection row (enum SelectionArrow),
/// shown when there are entries beyond that end. The first call takes the
/// arrow's VRAM cache entry; later calls only move it and decide whether it
/// shows.
void SelectionMenu_SetupArrow(struct SelectionMenuState *state, s32 side) {
  struct SelectionSprite *arrow = &state->records[side].sprite;
  const u8 *arrowTiles = 0;
  u32 count;

  state->records[side].kind = 0;
  if (side != SELECTION_ARROW_LEFT) {
    const u8 *right = gMenu_CursorActorTiles;

    count = state->count;
    if (state->top != 0)
      count -= state->top;
    if (count > SELECTION_ROW_PLACES) {
      state->records[side].kind = 1;
      count = SELECTION_ROW_PLACES;
    }
    arrowTiles = right;
    // Just past the last of the icons shown.
    state->records[SELECTION_ARROW_RIGHT].x =
        state->baseX + SELECTION_ICON_SPACING * (count - 1) + 17;
  } else {
    arrowTiles = gMenu_CursorLeftActorTiles;
    state->records[SELECTION_ARROW_LEFT].x = state->baseX - 9;
    if (state->top != 0)
      state->records[SELECTION_ARROW_LEFT].kind = 1;
  }
  if (state->records[side].y == 0) {
    state->records[side].slot = VramCache_FindFreeEntry();
    state->records[side].tile = VramCache_Load(state->records[side].slot,
                                               ARROW_FRAME_BYTES, arrowTiles);
    state->records[side].y = state->baseY;
    state->records[side].base = 0;
    arrow->mode = OBJ_MODE_NORMAL;
    arrow->mosaic = 0;
    arrow->colors = OBJ_COLORS_256;
    arrow->affine = OAM_AFFINE_OFF;
    arrow->matrix = 0;
    arrow->size = 0;
    arrow->shape = OBJ_SHAPE_VERTICAL;
    arrow->priority = 0;
  }
}

/// Returns the icon under the cursor: the row's icon at place cursorIndex.
struct SelectionNode *
SelectionMenu_GetCursorNode(struct SelectionMenuState *screen) {
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
  struct SelectionMenuState *screen = Ram_HeapSlots->selectionMenu;

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
  struct SelectionMenuState *screen = Ram_HeapSlots->selectionMenu;
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

/// Unused: moves the cursor one entry right, scrolling the row at its last
/// place; it stops at the end, where SelectionMenu_StepRight wraps around.
UNUSED void Menu_MoveSelectionForward(struct SelectionMenuState *screen) {
  u32 nextEntry = screen->top + screen->cursorIndex + 1;

  if (nextEntry == screen->count) {
    return;
  }
  SelectionMenu_RedrawIcon(screen, screen->cursorIndex);
  screen->status = SELECTION_STATUS_SCROLLING;
  wait(1);
  screen->cursorIndex++;
  if (screen->cursorIndex == SELECTION_LAST_PLACE &&
      nextEntry + 1 < screen->count) {
    screen->cursorIndex--;
    screen->records[SELECTION_ARROW_RIGHT].base = ARROW_SCROLL_FRAMES;
    screen->top++;
    SelectionMenu_Scroll(screen, TRUE);
    if (screen->top + screen->cursorIndex + 2 == screen->count) {
      screen->records[SELECTION_ARROW_RIGHT].kind = FALSE;
    }
    screen->records[SELECTION_ARROW_LEFT].kind = TRUE;
  }
  screen->status = SELECTION_STATUS_SHOWN;
  SelectionMenu_HighlightIcon(screen, screen->cursorIndex);
  wait(1);
  SelectionMenu_OpenNameWindow(screen->head->kind, 0);
  wait(1);
}

/// Moves the cursor one entry left, scrolling the row at its second place; it
/// stops at the beginning, where SelectionMenu_StepLeft wraps around.
void Menu_MoveSelectionBackward(struct SelectionMenuState *screen) {
  if (screen->top == 0 && screen->cursorIndex == 0) {
    return;
  }
  SelectionMenu_RedrawIcon(screen, screen->cursorIndex);
  screen->status = SELECTION_STATUS_SCROLLING;
  wait(1);
  if (screen->cursorIndex == 1 && screen->top != 0) {
    screen->records[SELECTION_ARROW_LEFT].base = ARROW_SCROLL_FRAMES;
    screen->top--;
    SelectionMenu_Scroll(screen, FALSE);
    if (screen->top == 0) {
      screen->records[SELECTION_ARROW_LEFT].kind = FALSE;
    }
    screen->records[SELECTION_ARROW_RIGHT].kind = 1;
  } else {
    screen->cursorIndex--;
  }
  screen->status = SELECTION_STATUS_SHOWN;
  SelectionMenu_HighlightIcon(screen, screen->cursorIndex);
  wait(1);
  SelectionMenu_OpenNameWindow(screen->head->kind, 0);
  wait(1);
}

/// Moves the cursor one entry right, scrolling the row at its edge and
/// wrapping from the last entry to the first.
void SelectionMenu_StepRight(struct SelectionMenuState *state) {
  struct SelectionNode *node;
  u32 entryIndex;
  s32 y;
  u32 end;

  SelectionMenu_RedrawIcon(state, state->cursorIndex);
  state->status = SELECTION_STATUS_SCROLLING;
  wait(1);
  state->cursorIndex++;
  if (state->count > SELECTION_ROW_PLACES) {
    end = state->top + state->cursorIndex;
    if (end == state->count) {
      node = state->head;
      state->records[SELECTION_ARROW_LEFT].kind = FALSE;
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
      state->records[SELECTION_ARROW_RIGHT].kind = TRUE;
    } else if (state->cursorIndex == SELECTION_LAST_PLACE &&
               end + 1 < state->count) {
      state->cursorIndex--;
      state->records[SELECTION_ARROW_RIGHT].base = ARROW_SCROLL_FRAMES;
      state->top++;
      SelectionMenu_Scroll(state, TRUE);
      if (state->top + state->cursorIndex + 2 == state->count)
        state->records[SELECTION_ARROW_RIGHT].kind = FALSE;
      state->records[SELECTION_ARROW_LEFT].kind = TRUE;
    }
  } else if (state->cursorIndex == state->count) {
    state->cursorIndex = 0;
  }
  state->status = SELECTION_STATUS_SHOWN;
  SelectionMenu_HighlightIcon(state, state->cursorIndex);
  wait(1);
  SelectionMenu_OpenNameWindow(state->head->kind, 0);
  wait(1);
}

/// Moves the cursor one entry left, scrolling the row at its edge and
/// wrapping from the first entry to the last.
void SelectionMenu_StepLeft(struct SelectionMenuState *state) {
  struct SelectionNode *node;
  s32 i;
  s32 kind;

  SelectionMenu_RedrawIcon(state, state->cursorIndex);
  state->status = SELECTION_STATUS_SCROLLING;
  wait(1);
  if (state->count > SELECTION_ROW_PLACES) {
    if ((state->cursorIndex | state->top) != 0) {
      if (state->cursorIndex == 1 && state->top != 0) {
        state->records[SELECTION_ARROW_LEFT].base = ARROW_SCROLL_FRAMES;
        state->top--;
        SelectionMenu_Scroll(state, FALSE);
        if (state->top == 0)
          state->records[SELECTION_ARROW_LEFT].kind = FALSE;
        state->records[SELECTION_ARROW_RIGHT].kind = TRUE;
      } else {
        state->cursorIndex--;
      }
    } else {
      node = state->head;
      i = SELECTION_LAST_PLACE * SELECTION_ICON_SPACING;
      kind = 0;
      state->records[SELECTION_ARROW_RIGHT].kind = kind;
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
      while (i != state->count - SELECTION_ROW_PLACES)
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
      state->records[SELECTION_ARROW_LEFT].kind = TRUE;
    }
  } else if (state->cursorIndex != 0) {
    state->cursorIndex--;
  } else {
    state->cursorIndex = state->count - 1;
  }
  state->status = SELECTION_STATUS_SHOWN;
  SelectionMenu_HighlightIcon(state, state->cursorIndex);
  wait(1);
  kind = state->head->kind;
  SelectionMenu_OpenNameWindow(kind, 0);
  wait(1);
}

/// Redraws the command icon at row place rowIndex in its plain state, in its
/// own VRAM cache entry. Other kinds of icon are left alone.
void SelectionMenu_RedrawIcon(struct SelectionMenuState *state, u32 rowIndex) {
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
void SelectionMenu_HighlightIcon(struct SelectionMenuState *state,
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
void Menu_ClearBattleHighlight(struct SelectionMenuState *screen) {
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
/// The icons move 2 pixels a frame, and the arriving and leaving icons zoom by
/// an eighth a frame, so a scroll takes 8 frames.
void SelectionMenu_Scroll(struct SelectionMenuState *screen, s32 forward) {
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

/// Loads the icon of an entry of kind mode (enum IconKind: a command, a yes or
/// no, an item or a Psynergy) into a free sprite tile slot and returns the
/// slot, or -1 when none was free or mode is another kind.
s32 SelectionIcon_Load(u32 mode, s32 value) {
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
    ItemIcon_LoadLayersIntoSlot(
        value, ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_BROKEN, result);
    break;
  case ICON_KIND_ABILITY:
    Ability_BuildIconTiles(value, 1, &result, &output, 0);
    break;
  }
  return result;
}

/// Loads the icon of an entry of kind mode into sprite tile slot result (a free
/// slot when -1) and returns the slot, or -1 when none was free.
s32 SelectionIcon_LoadIntoSlot(s32 mode, s32 value, s32 result, s32 option) {
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
    ItemIcon_LoadLayersIntoSlot(
        value, ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_BROKEN | ICON_CURSED,
        result);
    break;
  case ICON_KIND_EQUIPMENT_ITEM:
    ItemIcon_LoadLayersIntoSlot(
        value, ICON_COUNT_ABOVE_ONE | ICON_BROKEN | ICON_CURSED, result);
    break;
  case ICON_KIND_ABILITY:
    Ability_BuildIconTiles(value, option, &result, &output, 1);
    break;
  case ICON_KIND_PATTERN:
    Icon_BuildMiscIconTiles(value, 0, result);
    break;
  }

  return result;
}

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
    Ability_BuildIconTiles(base, 1, &cacheSlot, &tile, reuse);
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
  sprite->mode = OBJ_MODE_NORMAL;
  sprite->colors = OBJ_COLORS_16;
  sprite->mosaic = 0;

  sprite->size = 1;
  sprite->shape = OBJ_SHAPE_SQUARE;
  sprite->palette = 0;
  sprite->tile = tile;
  sprite->priority = 0;
}

/// Chooses the entry under the cursor: the other icons gather into it and
/// vanish, and it joins the chosen icons at the top left.
u32 Menu_ConfirmSelection(struct SelectionMenuState *screen) {
  struct SelectionNode *node;
  struct SelectionNode *other = NULL;
  u32 place = 0;
  u32 index;
  struct SelectionSprite *sprite;

  index = screen->top + screen->cursorIndex;
  Menu_ClearBattleHighlight(screen);
  SelectionMenu_RedrawIcon(screen, screen->cursorIndex);
  screen->status = SELECTION_STATUS_SCROLLING;
  wait(1);
  screen->records[SELECTION_ARROW_LEFT].kind = 0;
  screen->records[SELECTION_ARROW_RIGHT].kind = 0;
  screen->records[SELECTION_RECORD_HAND].kind = 0;
  screen->records[SELECTION_RECORD_HAND].scale = 0;
  SelectionMenu_FreeLargeIcon();
  node = screen->head;
  for (; node != NULL && screen->cursorIndex != place; place++) {
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
  for (other = screen->path, place = 0; other != NULL; other = other->next) {
    node->targetX += SELECTION_ICON_SPACING;
    place++;
  }
  screen->pathTop[place] = screen->top;
  screen->pathCursor[place] = screen->cursorIndex;
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
  sprite->mode = OBJ_MODE_NORMAL;
  sprite->colors = OBJ_COLORS_16;
  sprite->mosaic = 0;
  sprite->size = 1;
  sprite->shape = OBJ_SHAPE_SQUARE;
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

// Six routines that do nothing, beside the menu arrows': nothing refers to the
// first, the fourth or the fifth, and the others only fill entries of the
// window far-call table, which nothing calls.

/// Does nothing.
UNUSED void MenuArrow_NoOpCallback(void) {}

/// Does nothing.
void MenuArrow_NoOpCallbackBank(void) {}

/// Does nothing.
void MenuArrow_NoOpCallbackA(void) {}

/// Does nothing.
UNUSED void MenuArrow_NoOpCallbackB(void) {}

/// Does nothing.
UNUSED void MenuArrow_NoOpCallbackC(void) {}

/// Does nothing.
void MenuArrow_NoOpCallbackD(void) {}

/// Sets up a sprite as the right-pointing arrow, in a free VRAM cache entry
/// returned in *slot, which MenuArrow_Release frees.
void MenuArrow_Create(struct MenuSprite *sprite, s32 *slot) {
  const void *data = gMenu_CursorActorTiles;
  s32 cacheSlot = VramCache_FindFreeEntry();

  *slot = cacheSlot;
  sprite->oam.f.tile = VramCache_Load(cacheSlot, ARROW_FRAME_BYTES, data);
  sprite->oam.f.mode = OBJ_MODE_NORMAL;
  sprite->oam.f.mosaic = 0;
  sprite->oam.f.colors = OBJ_COLORS_256;
  sprite->oam.f.affine = OAM_AFFINE_OFF;
  sprite->oam.f.affineIndex = 0;
  sprite->oam.f.size = 0;
  sprite->oam.f.shape = OBJ_SHAPE_VERTICAL;
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
void MenuArrow_Release(s32 slot) { VramCache_FreeEntry(slot); }

/// The large icon's zoom: it opens at this scale (in 8.8) and grows by as much
/// each frame until six steps, when SelectionMenu_Draw shows it at its own
/// size.
#define LARGE_ICON_ZOOM_STEP 40

/// Loads the large icon of the command under the cursor, zooming in: 32x32 in
/// 256 colours (SELECTION_RECORD_LARGE_ICON), its scale growing by
/// LARGE_ICON_ZOOM_STEP a frame. Other kinds of entry show none. The icon keeps
/// its VRAM cache entry until SelectionMenu_FreeLargeIcon.
void SelectionMenu_LoadLargeIcon(void) {
  struct SelectionMenuState *state = Ram_HeapSlots->selectionMenu;
  struct SelectionNode *node = SelectionMenu_GetCursorNode(state);
  struct SelectionNode *largeIcon;
  IconTransfer *buffer;
  u8 *table;
  void *resource;
  s32 command;

  if (node->kind != ICON_KIND_COMMAND && node->kind != ICON_KIND_YES_NO)
    return;

  buffer = (IconTransfer *)HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER,
                                          sizeof(IconTransfer));
  largeIcon = &state->records[SELECTION_RECORD_LARGE_ICON];
  command = node->base;
  table = Resource_GetTableEntry(RESOURCE_COMMAND_ICONS);
  {
    u8 **destination = &buffer->encoded;
    resource = table + ((u16 *)table)[node->base];
    *destination = resource;
  }
  Decompress_ByteLz(resource, buffer);

  if (largeIcon->kind == ICON_KIND_NONE)
    largeIcon->slot = VramCache_FindFreeEntry();
  largeIcon->tile = VramCache_Load(largeIcon->slot, COMMAND_ICON_BYTES, buffer);
  largeIcon->kind = ICON_KIND_COMMAND;
  largeIcon->base = command;
  largeIcon->scale = LARGE_ICON_ZOOM_STEP;
  largeIcon->scaleStep = LARGE_ICON_ZOOM_STEP;
  largeIcon->scaleEnd = 6 * LARGE_ICON_ZOOM_STEP;
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
}

/// Frees the large icon's VRAM cache entry, if it holds one, and marks it
/// empty.
void SelectionMenu_FreeLargeIcon(void) {
  struct SelectionNode *largeIcon =
      &((struct SelectionMenuState *)Ram_HeapSlots->selectionMenu)
           ->records[SELECTION_RECORD_LARGE_ICON];

  if (largeIcon->kind != ICON_KIND_NONE) {
    VramCache_FreeEntry(largeIcon->slot);
    largeIcon->kind = ICON_KIND_NONE;
  }
}

void PartyStatus_CreateWindow(s32 flags);
s32 FieldMenu_SelectCommand(s32);

/// Runs the field menu until the player picks something that leaves it, and
/// returns the chosen screen's result. Cancelling the menu returns an
/// undefined value, the missing result of PartyStatus_CloseWithMenu (0 under
/// UBFIX); its one caller, FieldSequence_Run, ignores the result. Its case for
/// command 0, which stores the trigger tile ahead in the field state's
/// examineCell, is unreachable.
s32 FieldMenu_Run(void) {
  s32 ret;
  s32 selection;
  struct FieldState *state;

  state = Ram_HeapSlots->fieldState;
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

/// Closes the party status window that shows with the field menu.
void PartyStatus_CloseWithMenu(void) { PartyStatus_Close(); }

/// Sets up an empty selection screen and waits a frame.
void SelectionMenu_CreateEmpty(void) {
  SelectionMenu_Create();
  SelectionMenu_Clear();
  wait(1);
}

void SelectionMenu_BuildEntries(u32);

/// Runs a selection row with the cursor on entry initialIndex, cancellable, and
/// returns the entry chosen or -1.
u32 Menu_RunSelectionForValue(u32 initialIndex) {
  struct SelectionMenuState *state = Ram_HeapSlots->selectionMenu;
  u32 result;

  state->cursorIndex = initialIndex;
  state->locked = 1;
  SelectionMenu_BuildEntries(initialIndex);
  Menu_SetupSelectionBothSides();
  SelectionMenu_OpenNameWindow(0, 5);
  SelectionMenu_StartDrawing();
  result = SelectionMenu_RunWithSounds(1);
  SelectionMenu_Close();
  return result;
}

#undef SELECTION_FIXED
#undef SELECTION_LAST_PLACE
#undef ARROW_SCROLL_FRAMES

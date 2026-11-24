/// @file
/// The selection screen's row of icons, and the field menu.
// Part of window.c.

#include "window_internal.h"
#include "field/map_grid.h"

/// The selection row's icons stand 16 pixels apart.
#define SELECTION_ICON_SPACING 16
/// The row shows five entries at a time, from its seven icon records; the
/// icons chosen so far take five more.
#define SELECTION_ROW_SHOWN 5
#define SELECTION_ROW_RECORDS 7
#define SELECTION_CHOSEN_RECORDS 5

/// The records of the arrows at the row's ends, before the row's
/// (SelectionMenuState.records): an arrow's kind is TRUE while it shows, and
/// its base counts the frames it stays nudged out after a scroll.
enum {
  SELECTION_ARROW_LEFT = 0,
  SELECTION_ARROW_RIGHT = 1,
};

/// The flags the name window opens with: WINDOW_FLAG_INSTANT, and a 4 that
/// Window_Create drops. A yes or no window opens with its kind,
/// ICON_KIND_YES_NO, as its flags, which come to the same.
#define NAME_WINDOW_FLAGS (WINDOW_FLAG_INSTANT | 4)

/// SelectionMenuState.verticalScroll as the screen is made: the row's icons
/// drawn 32 pixels lower, below the bottom of the screen, and the cursor held
/// still, until SelectionMenu_Clear readies the list.
#define SELECTION_ROW_HIDDEN 32

/// SelectionMenuState.locked as the screen is made and once a yes or no window
/// has opened: nonzero keeps a yes or no row and its window at the bottom of
/// the screen, as Menu_RunSelectionForValue's 1 does.
#define SELECTION_YES_NO_AT_BOTTOM 999

/// The large icon's zoom: it opens at this scale (in 8.8) and grows by as much
/// a frame up to six times it.
#define LARGE_ICON_ZOOM_STEP 40

/// One frame of an end arrow's animation: an 8x16 sprite of 256 colours.
#define ARROW_FRAME_BYTES (2 * TILE_SIZE_8BPP)
/// One frame of the pointing hand's animation: a 16x16 sprite of 256 colours.
#define HAND_FRAME_BYTES (4 * TILE_SIZE_8BPP)

/// Allocates and resets the selection screen: the row of icons the player picks
/// from, with arrows at each end and a pointing hand over the chosen target.
void SelectionMenu_Create(void) {
  struct SelectionMenuState *state;
  struct SelectionSprite *visual;
  const u8 *table;
  s32 i;
  u16 clear = 0;

  state = HeapSlot_AllocEwram(HEAP_SLOT_SELECTION_MENU,
                              sizeof(struct SelectionMenuState));
  state->head = NULL;
  state->path = NULL;
  state->window = NULL;
  state->unread = clear;
  state->top = clear;
  // The first list starts from its first entry.
  state->cursorIndex = SELECTION_CURSOR_RESET;
  state->verticalScroll = SELECTION_ROW_HIDDEN;
  state->count = clear;
  state->locked = SELECTION_YES_NO_AT_BOTTOM;
  for (i = 0; i != SELECTION_CHOSEN_RECORDS; i++) {
    state->records[i + SELECTION_RECORD_ROW].kind = ICON_KIND_NONE;
    state->records[i + SELECTION_RECORD_CHOSEN].kind = ICON_KIND_NONE;
  }
  // The row has two icons more than the chosen ones; i is 5.
  state->records[i + SELECTION_RECORD_ROW].kind = ICON_KIND_NONE;
  state->records[i + SELECTION_RECORD_ROW + 1].kind = ICON_KIND_NONE;
  state->records[SELECTION_ARROW_LEFT].kind = FALSE;
  state->records[SELECTION_ARROW_RIGHT].kind = FALSE;
  state->records[SELECTION_ARROW_LEFT].y = 0;
  state->records[SELECTION_ARROW_RIGHT].y = 0;
  table = gMenu_AnimatedCursorTiles;
  state->records[SELECTION_RECORD_HAND].slot = VramCache_FindFreeEntry();
  state->records[SELECTION_RECORD_HAND].tile = VramCache_Load(
      state->records[SELECTION_RECORD_HAND].slot, HAND_FRAME_BYTES, table);
  state->records[SELECTION_RECORD_HAND].kind = 0;
  state->records[SELECTION_RECORD_HAND].scale = 0;
  state->records[SELECTION_RECORD_LARGE_ICON].kind = ICON_KIND_NONE;
  visual = &state->records[SELECTION_RECORD_HAND].sprite;
  visual->mode = OBJ_MODE_NORMAL;
  visual->mosaic = 0;
  visual->colors = 1;
  visual->affine = SPRITE_AFFINE_OFF;
  visual->matrix = 0;
  visual->size = 1;
  visual->shape = SPRITE_SHAPE_SQUARE;
  visual->priority = 0;
}

/// Empties the selection screen's list of entries.
/// The high cursor bit requests resetting both the list origin and cursor.
void SelectionMenu_Clear(void) {
  struct SelectionMenuState *state;

  state = gSelectionMenuState;
  state->head = NULL;
  state->unread = 0;
  if (state->cursorIndex & SELECTION_CURSOR_RESET) {
    state->top = 0;
    state->cursorIndex = 0;
  }
  state->verticalScroll = 0;
  state->count = 0;
}

/// Adds an entry (kind and id) to the selection screen's list, up to 16. The
/// count starts at 0 when the screen is created or cleared.
void SelectionMenu_AddEntry(u32 kind, u32 base) {
  struct SelectionMenuState *state = gSelectionMenuState;
  u16 entryCount = state->count;

  if (entryCount != SELECTION_MENU_ENTRY_LIMIT) {
    state->kinds[entryCount] = kind;
    state->bases[entryCount] = base;
    state->count++;
  }
}

/// Creates icons for the first five listed entries from top, in a row along
/// the bottom of the screen.
void SelectionMenu_BuildEntries(u32 unused) {
  struct SelectionMenuState *state = gSelectionMenuState;
  u32 count = state->count;
  u32 entryIndex;
  struct SelectionNode *previous = 0;
  struct SelectionNode *node;
  s32 rowPosition = 0;

  for (entryIndex = state->top; entryIndex < count; entryIndex++) {
    s32 base = state->bases[entryIndex];
    u32 kind = state->kinds[entryIndex];

    node = SelectionMenu_FindFreeIcon(SELECTION_ICON_ROW);
    if (node == 0)
      break;
    SelectionMenu_SetupIcon(kind, base, node, 0);
    if (state->head == 0) {
      state->head = node;
      node->previous = 0;
    } else {
      previous->next = node;
      node->previous = previous;
    }
    node->next = 0;
    rowPosition++;
    previous = node;
    if (rowPosition == SELECTION_ROW_SHOWN)
      break;
  }

  // The row is centred on x 100, at y 140.
  state->baseX = 100 - rowPosition * SELECTION_ICON_SPACING / 2;
  state->baseY = 140;
  // rowPosition, the row's icon count, now steps an icon's spacing.
  for (previous = state->head, rowPosition = 0; previous != 0;
       previous = previous->next) {
    s32 x = state->baseX + rowPosition;
    s32 y;

    previous->x = x;
    y = state->baseY;
    previous->y = y;
    previous->targetX = x;
    previous->targetY = y;
    // While locked is 0 a yes or no row sits at the top of the screen.
    if (previous->kind == ICON_KIND_YES_NO && state->locked == 0) {
      previous->y = 6;
      previous->targetY = 6;
    }
    previous->dx = 0;
    previous->dy = 0;
    rowPosition += SELECTION_ICON_SPACING;
  }
  SelectionMenu_LoadLargeIcon();
}

/// An empty function, reached only through a bare entry of the window
/// far-call table (gamepak/far_call/window.s).
void SelectionMenu_ReservedNoOp(void) {}

/// Returns a free selection icon record of `kind` (enum SelectionIconGroup),
/// or NULL.
struct SelectionNode *SelectionMenu_FindFreeIcon(s32 kind) {
  struct SelectionMenuState *state = gSelectionMenuState;
  s32 i;

  if (kind != SELECTION_ICON_ROW) {
    for (i = 0; i != SELECTION_CHOSEN_RECORDS; i++) {
      if (state->records[i + SELECTION_RECORD_CHOSEN].kind == ICON_KIND_NONE)
        return &state->records[i + SELECTION_RECORD_CHOSEN];
    }
  } else {
    for (i = 0; i != SELECTION_ROW_RECORDS; i++) {
      if (state->records[i + SELECTION_RECORD_ROW].kind == ICON_KIND_NONE)
        return &state->records[i + SELECTION_RECORD_ROW];
    }
  }
  return NULL;
}

/// Starts drawing the selection screen each frame, in the frame pass
/// (TASK_PASS_FRAME).
void SelectionMenu_StartDrawing(void) {
  Task_Add(SelectionMenu_Draw, TASK_PASS_FRAME);
}

/// Stops drawing the selection screen.
void SelectionMenu_StopDrawing(void) { Task_Remove(SelectionMenu_Draw); }

/// The selection screen's per-frame task: slides and draws the row of icons,
/// the large icon over the chosen one, the end arrows, the icons already chosen
/// and the pointing hand.
void SelectionMenu_Draw(void) {
  struct SelectionMenuState *state = gSelectionMenuState;
  struct SelectionSprite *cursorEntry =
      &state->records[SELECTION_RECORD_HAND].sprite;
  struct SelectionNode *cursor = &state->records[SELECTION_RECORD_HAND];
  struct SelectionNode *largeIcon =
      &state->records[SELECTION_RECORD_LARGE_ICON];
  struct SelectionNode *node;
  struct SelectionSprite *sprite;
  struct SelectionSprite *commandSprite;
  s32 entryIndex = 0;
  s32 slot;
  s32 axisStep;
  struct ScreenPoint position;

  node = state->head;
  // Move and queue the visible row before drawing its overlays.
  while (node != 0) {
    sprite = &node->sprite;
    sprite->affine = SPRITE_AFFINE_OFF;
    sprite->matrix = 0;
    sprite->x = node->x;
    sprite->y = node->y;
    slot = OAM_DEPTH_WINDOW;
    if (state->verticalScroll != 0) {
      sprite->y += state->verticalScroll;
    } else if (node->x != node->targetX) {
      if (node->dx > 0) {
        if (node->x + node->dx > node->targetX) {
          node->x = node->targetX;
        } else {
          node->x += node->dx;
        }
      } else {
        if (node->x + node->dx < node->targetX) {
          node->x = node->targetX;
        } else {
          node->x += node->dx;
        }
      }
      sprite->x = node->x;
    } else if (entryIndex == state->cursorIndex) {
      slot = OAM_DEPTH_WINDOW + 1;
      if (cursor->kind != ICON_KIND_NONE) {
        if (BattleMotion_ProjectScaledPositionFar(
                node->base, ScreenPoint_Coordinates(&position)) != -1) {
          cursor->targetX = position.x;
          cursor->targetY = position.y;
          if (cursor->scale == 0) {
            cursor->x = position.x;
            cursor->y = position.y;
            cursor->scale = 1;
          }
        }
      }
    }
    if (node->scale != 0) {
      if (GameFlag_GetFar(FLAG_IN_BATTLE) != 0) {
        if (state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND) {
          sprite->mode = OBJ_MODE_SEMI_TRANSPARENT;
        } else {
          sprite->mode = OBJ_MODE_NORMAL;
        }
        if (node->kind == ICON_KIND_COMMAND) {
          sprite->mode = OBJ_MODE_SEMI_TRANSPARENT;
        }
      }
      Oam_AddSpriteAtDepth(sprite, slot);
    }
    node = node->next;
    entryIndex++;
  }
  // The selected command's large icon owns a separate cached tile block.
  if (largeIcon->kind != ICON_KIND_NONE) {
    node = SelectionMenu_GetCursorNode(state);
    commandSprite = &largeIcon->sprite;
    commandSprite->mode = OBJ_MODE_NORMAL;
    commandSprite->affine = SPRITE_AFFINE_OFF;
    commandSprite->matrix = 0;
    commandSprite->mosaic = 0;
    commandSprite->colors = 1;
    commandSprite->size = 2;
    commandSprite->shape = SPRITE_SHAPE_SQUARE;
    commandSprite->priority = 0;
    commandSprite->tile = largeIcon->tile;
    // A 32x32 sprite (size 2) over the icon, bobbing by half the cursor's
    // bob; while it zooms in, the double-size area moves it 16 up and left,
    // wrapped as the 9-bit x and the 8-bit y wrap.
    commandSprite->x = node->x - 4;
    commandSprite->y =
        node->y + (gMenuCursor_BobOffsets[(gVBlankCount >> 1) & 15] >> 1) - 4;
    if (largeIcon->scale != largeIcon->scaleEnd) {
      state->transform.scaleX = largeIcon->scale;
      state->transform.scaleY = largeIcon->scale;
      state->transform.angle = 0;
      commandSprite->matrix = OamMatrix_AllocFromTransform(&state->transform);
      commandSprite->affine = SPRITE_AFFINE_DOUBLE_SIZE;
      commandSprite->x += (u16)-16;
      commandSprite->y += (u8)-16;
      largeIcon->scale += largeIcon->scaleStep;
    }
    if (GameFlag_GetFar(FLAG_IN_BATTLE) != 0) {
      commandSprite->mode = OBJ_MODE_SEMI_TRANSPARENT;
    }
    Oam_AddSpriteAtDepth(commandSprite, OAM_DEPTH_WINDOW + 8);
  }
  SelectionMenu_DrawArrow(state, 0);
  SelectionMenu_DrawArrow(state, 1);
  // Chosen entries gather at the top-left independently of the scrolling row.
  node = state->path;
  while (node != 0) {
    sprite = &node->sprite;
    if (node->x != node->targetX) {
      node->x += node->dx;
    }
    if (node->y != node->targetY) {
      node->y += node->dy;
    }
    sprite->x = node->x;
    sprite->y = node->y;
    if (node->scale != node->scaleEnd) {
      node->scale += node->scaleStep;
      state->transform.scaleX = node->scale;
      state->transform.scaleY = node->scale;
      state->transform.angle = 0;
      sprite->matrix = OamMatrix_AllocFromTransform(&state->transform);
      sprite->affine = SPRITE_AFFINE_DOUBLE_SIZE;
      sprite->x += (u16)-8;
      sprite->y += (u8)-8;
    } else {
      sprite->affine = SPRITE_AFFINE_OFF;
      sprite->matrix = 0;
    }
    if (GameFlag_GetFar(FLAG_IN_BATTLE) != 0) {
      if (state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND) {
        sprite->mode = OBJ_MODE_SEMI_TRANSPARENT;
      } else {
        sprite->mode = OBJ_MODE_NORMAL;
      }
      if (node->kind == ICON_KIND_COMMAND) {
        sprite->mode = OBJ_MODE_SEMI_TRANSPARENT;
      }
    }
    Oam_AddSpriteAtDepth(sprite, OAM_DEPTH_WINDOW);
    node = node->next;
  }
  // Point at the selected battle target, smoothing each axis separately: the
  // hand covers half the distance left each frame.
  if (cursor->kind != ICON_KIND_NONE) {
    entryIndex = (s32)gMenu_AnimatedCursorTiles;
    {
      void *src = (void *)(entryIndex + (((gVBlankCount >> 2) & 15) << 8));

      cursorEntry->tile = VramCache_Load(cursor->slot, HAND_FRAME_BYTES, src);
    }
    if (cursor->targetX != cursor->x) {
      axisStep = (cursor->targetX - cursor->x) >> 1;
      if (axisStep != 0) {
        cursor->x += axisStep;
      } else {
        cursor->x = cursor->targetX;
      }
    }
    if (cursor->targetY != cursor->y) {
      if ((axisStep = (cursor->targetY - cursor->y) >> 1) != 0) {
        cursor->y += axisStep;
      } else {
        cursor->y = cursor->targetY;
      }
    }
    cursorEntry->y =
        cursor->y + gMenuCursor_BobOffsets[(gVBlankCount >> 2) & 15] - 32;
    cursorEntry->x = cursor->x - 4;
    if (GameFlag_GetFar(FLAG_IN_BATTLE) != 0) {
      if (state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND) {
        cursorEntry->mode = OBJ_MODE_SEMI_TRANSPARENT;
      } else {
        cursorEntry->mode = OBJ_MODE_NORMAL;
      }
    }
    Oam_AddSpriteAtDepth(cursorEntry, OAM_DEPTH_WINDOW + 8);
  }
  state->status += 1;
}

/// Draws the selection screen's left (index 0) or right (1) arrow, nudged
/// outwards while it flashes after a scroll: the record's base holds the
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
  if (side != 0) {
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
  if (GameFlag_GetFar(FLAG_IN_BATTLE)) {
    if (state->records[SELECTION_RECORD_HAND].kind == ICON_KIND_COMMAND)
      arrow->mode = OBJ_MODE_SEMI_TRANSPARENT;
    else
      arrow->mode = OBJ_MODE_NORMAL;
  }
  Oam_AddSpriteAtDepth(arrow, OAM_DEPTH_WINDOW - 2);
  if (state->records[side].base != 0)
    state->records[side].base--;
}

/// Opens or refits the name window above the selection row and writes the name
/// of the icon under the cursor, or that nothing is known (ICON_KIND_ABILITY)
/// or held (ICON_KIND_ITEM).
void SelectionMenu_OpenNameWindow(s32 mode, u32 count) {
  struct SelectionMenuState *state;
  struct SelectionNode *node;
  struct Window **slot;
  struct Window *window;

  state = gSelectionMenuState;
  node = SelectionMenu_GetCursorNode(state);
  slot = &state->window;
  window = *slot;
  if (window == 0) {
    // A yes or no window, 5 by 3 tiles, from column 17 at the bottom, or the
    // top while locked is 0.
    if (mode == ICON_KIND_YES_NO) {
      if (state->locked != 0) {
        *slot = Window_Create(17, 17, 5, 3, mode);
      } else {
        *slot = Window_Create(17, 0, 5, 3, mode);
      }
      state->verticalScroll = 0;
      state->locked = SELECTION_YES_NO_AT_BOTTOM;
    } else {
      *slot = Window_Create(19 + ((9 - count) >> 1), 17, count + 2, 3,
                            NAME_WINDOW_FLAGS);
    }
    WindowSprite_PrepareForRedraw(state->window);
  } else {
    if (count != 0 && window->width != count + 2) {
      Window_Close(window, WINDOW_FLAG_INSTANT);
      *slot = Window_Create(19 + ((9 - count) >> 1), 17, count + 2, 3,
                            NAME_WINDOW_FLAGS);
    }
    WindowSprite_PrepareForRedraw(state->window);
  }
  if (state->count != 0) {
    Text_DrawMessageAsTiles(node->message, state->window, 0, 0);
  } else {
    switch (mode) {
    case ICON_KIND_ABILITY:
      Text_DrawMessageAsTiles(MsgAbilityNotKnown, state->window, 0, 0);
      break;
    case ICON_KIND_ITEM:
      Text_DrawMessageAsTiles(MsgItemNotHeld, state->window, 0, 0);
      break;
    }
  }
}

/// Closes the selection screen and frees its icons, tiles and state block.
void SelectionMenu_Close(void) {
  struct SelectionMenuState *state = gSelectionMenuState;
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
  // An arrow's y is nonzero once SelectionMenu_SetupArrow has given it a slot.
  if (state->records[SELECTION_ARROW_LEFT].y != 0) {
    VramCache_FreeEntry(state->records[SELECTION_ARROW_LEFT].slot);
    if (state->records[SELECTION_ARROW_LEFT].y != 0) {
      VramCache_FreeEntry(state->records[SELECTION_ARROW_RIGHT].slot);
    }
  }
  VramCache_FreeEntry(state->records[SELECTION_RECORD_HAND].slot);
  HeapSlot_Release(HEAP_SLOT_SELECTION_MENU);
}

/// Moves the row of icons to start at pixel (x, y), 16 pixels apart.
void SelectionMenu_SetPosition(u32 x, u32 y) {
  struct SelectionMenuState *state = gSelectionMenuState;
  struct SelectionNode *node;

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
  struct SelectionMenuState *state;

  state = gSelectionMenuState;
  SelectionMenu_SetupArrow(state, 0);
  SelectionMenu_SetupArrow(state, 1);
}

/// Sets up the left (index 0) or right (1) arrow of the selection row, shown
/// when there are entries beyond that end.
void SelectionMenu_SetupArrow(struct SelectionMenuState *state, s32 side) {
  struct SelectionSprite *arrow = &state->records[side].sprite;
  const u8 *arrowTiles = 0;
  u32 count;

  state->records[side].kind = FALSE;
  if (side != 0) {
    const u8 *right = gMenu_CursorActorTiles;

    count = state->count;
    if (state->top != 0)
      count -= state->top;
    if (count > SELECTION_ROW_SHOWN) {
      state->records[side].kind = TRUE;
      count = SELECTION_ROW_SHOWN;
    }
    arrowTiles = right;
    // Just past the last of the at most five icons shown.
    state->records[SELECTION_ARROW_RIGHT].x =
        state->baseX + SELECTION_ICON_SPACING * (count - 1) + 17;
  } else {
    arrowTiles = gMenu_CursorLeftActorTiles;
    state->records[SELECTION_ARROW_LEFT].x = state->baseX - 9;
    if (state->top != 0)
      state->records[SELECTION_ARROW_LEFT].kind = TRUE;
  }
  if (state->records[side].y == 0) {
    state->records[side].slot = VramCache_FindFreeEntry();
    state->records[side].tile = VramCache_Load(state->records[side].slot,
                                               ARROW_FRAME_BYTES, arrowTiles);
    state->records[side].y = state->baseY;
    state->records[side].base = 0;
    arrow->mode = OBJ_MODE_NORMAL;
    arrow->mosaic = 0;
    arrow->colors = 1;
    arrow->affine = SPRITE_AFFINE_OFF;
    arrow->matrix = 0;
    arrow->size = 0;
    arrow->shape = SPRITE_SHAPE_TALL;
    arrow->priority = 0;
  }
}

/// Returns the node at the selection screen's cursor index.
/// cursorIndex is relative to head, not the number of queued entries.
struct SelectionNode *
SelectionMenu_GetCursorNode(struct SelectionMenuState *state) {
  struct SelectionNode *node = state->head;
  s32 i;

  for (i = 0; i != state->cursorIndex; ++i) {
    node = node->next;
  }
  return node;
}

/// The selection row's mode that only B leaves: the cursor stays and A chooses
/// nothing.
#define SELECTION_FIXED 999

/// The selection row's status while it scrolls, and once it shows.
#define SELECTION_STATUS_SCROLLING 33
#define SELECTION_STATUS_SHOWN 1

/// The row's last place, the fifth: the cursor stepping onto it with more
/// entries to come scrolls the row instead.
#define SELECTION_LAST_PLACE 4
#define ARROW_SCROLL_FRAMES 8

/// Runs the selection row until the player chooses (the chosen entry's index)
/// or, when mode allows it, cancels with B (-1).
s32 SelectionMenu_Run(s32 mode) {
  struct SelectionMenuState *state = gSelectionMenuState;

  SelectionMenu_HighlightIcon(state, 0);
  for (;;) {
    wait(1);
    // No input while the row is pushed down.
    if (state->verticalScroll != 0) {
      continue;
    }
    if (mode != SELECTION_FIXED) {
      if (gKeysRepeat & KEY_RIGHT) {
        SelectionMenu_StepRight(state);
      } else if (gKeysRepeat & KEY_LEFT) {
        SelectionMenu_StepLeft(state);
      } else if (gKeysPressed & KEY_A) {
        return Menu_ConfirmSelection(state);
      }
    }
    if (mode != 0 && (gKeysPressed & KEY_B)) {
      return -1;
    }
  }
}

/// SelectionMenu_Run with the menu's sounds: the cursor's move, a confirm, and
/// on a yes or no row a cancel for no; B, when mode allows it, cancels with its
/// sound.
u32 SelectionMenu_RunWithSounds(u32 mode) {
  struct SelectionMenuState *state = gSelectionMenuState;
  u32 result;

  for (;;) {
    wait(1);
    if (state->verticalScroll != 0) {
      continue;
    }

    if (mode != SELECTION_FIXED) {
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        SelectionMenu_StepRight(state);
      } else if (gKeysRepeat & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        SelectionMenu_StepLeft(state);
      }

      if (gKeysPressed & KEY_A) {
        // On a yes or no row, entry 0 (yes) sounds as a confirm and the
        // other as a cancel.
        result = state->top + state->cursorIndex;
        if (state->head->kind == ICON_KIND_YES_NO) {
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

    if (mode != 0 && (gKeysPressed & KEY_B)) {
      Sound_PlayFar(SE_MENU_CANCEL);
      return -1;
    }
  }
}

/// Moves the cursor one entry right, scrolling the row at its fourth place.
/// This variant stops at the end; SelectionMenu_StepRight also wraps around.
UNUSED void Menu_MoveSelectionForward(struct SelectionMenuState *state) {
  u32 nextEntry = state->top + state->cursorIndex + 1;

  if (nextEntry == state->count) {
    return;
  }
  SelectionMenu_RedrawIcon(state, state->cursorIndex);
  state->status = SELECTION_STATUS_SCROLLING;
  wait(1);
  state->cursorIndex++;
  if (state->cursorIndex == SELECTION_LAST_PLACE &&
      nextEntry + 1 < state->count) {
    state->cursorIndex--;
    state->records[SELECTION_ARROW_RIGHT].base = ARROW_SCROLL_FRAMES;
    state->top++;
    SelectionMenu_Scroll(state, TRUE);
    if (state->top + state->cursorIndex + 2 == state->count) {
      state->records[SELECTION_ARROW_RIGHT].kind = FALSE;
    }
    state->records[SELECTION_ARROW_LEFT].kind = TRUE;
  }
  state->status = SELECTION_STATUS_SHOWN;
  SelectionMenu_HighlightIcon(state, state->cursorIndex);
  wait(1);
  SelectionMenu_OpenNameWindow(state->head->kind, 0);
  wait(1);
}

/// Moves the cursor one entry left, scrolling the row at its second place. This
/// variant stops at the beginning; SelectionMenu_StepLeft also wraps around.
UNUSED void Menu_MoveSelectionBackward(struct SelectionMenuState *state) {
  u32 cursorIndex;

  if (state->top == 0 && state->cursorIndex == 0) {
    return;
  }
  SelectionMenu_RedrawIcon(state, state->cursorIndex);
  state->status = SELECTION_STATUS_SCROLLING;
  wait(1);
  cursorIndex = state->cursorIndex;
  if (cursorIndex == 1 && state->top != 0) {
    state->records[SELECTION_ARROW_LEFT].base = ARROW_SCROLL_FRAMES;
    state->top--;
    SelectionMenu_Scroll(state, FALSE);
    if (state->top == 0) {
      state->records[SELECTION_ARROW_LEFT].kind = FALSE;
    }
    state->records[SELECTION_ARROW_RIGHT].kind = cursorIndex;
  } else {
    state->cursorIndex--;
  }
  state->status = SELECTION_STATUS_SHOWN;
  SelectionMenu_HighlightIcon(state, state->cursorIndex);
  wait(1);
  SelectionMenu_OpenNameWindow(state->head->kind, 0);
  wait(1);
}

/// Moves the cursor one entry right, scrolling the row at its edge and
/// wrapping from the last entry to the first.
void SelectionMenu_StepRight(struct SelectionMenuState *state) {
  struct SelectionNode *node;
  u32 entryIndex;
  s32 x;
  u32 end;

  SelectionMenu_RedrawIcon(state, state->cursorIndex);
  state->status = SELECTION_STATUS_SCROLLING;
  wait(1);
  state->cursorIndex++;
  // With more entries than the five shown, stepping past the last gathers the
  // icons at the row's start (12 pixels a frame), refills them from the first
  // entry and fans them out again; stepping onto the fifth place with more to
  // come scrolls the row instead, the cursor staying on the fourth.
  if (state->count > SELECTION_ROW_SHOWN) {
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
      x = node->x + SELECTION_ICON_SPACING;
      node = node->next;
      while (node != NULL) {
        node->targetX = x;
        node->dx = 12;
        node = node->next;
        x += SELECTION_ICON_SPACING;
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
  // As SelectionMenu_StepRight, mirrored: from the second place with entries
  // before it the row scrolls back; from the first entry the icons gather at
  // the row's last place (12 pixels a frame), refill from the last five entries
  // and fan out again, with the cursor on the last.
  if (state->count > SELECTION_ROW_SHOWN) {
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
      // kind, which later takes the head's kind, carries the 0 first.
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
      while (i != state->count - SELECTION_ROW_SHOWN)
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
  u32 tile;
  u32 value;

  while (rowIndex != 0) {
    rowIndex--;
    node = node->next;
  }
  if (node->kind == ICON_KIND_COMMAND || node->kind == ICON_KIND_YES_NO) {
    u32 first = node->message - MsgCommandName;

    value = node->slot;
    Icon_BuildCommandIconTiles(first, 0, &value, &tile, 1);
  }
}

/// Redraws the command icon at row place rowIndex and loads its large icon.
void SelectionMenu_HighlightIcon(struct SelectionMenuState *state,
                                 u32 rowIndex) {
  struct SelectionNode *node = state->head;
  u32 tile;
  u32 value;

  while (rowIndex != 0) {
    rowIndex--;
    node = node->next;
  }
  if (node->kind == ICON_KIND_COMMAND || node->kind == ICON_KIND_YES_NO) {
    u32 resource = node->message - MsgCommandName;

    value = node->slot;
    Icon_BuildCommandIconTiles(resource, 0, &value, &tile, 1);
    SelectionMenu_LoadLargeIcon();
  }
}

/// Stops the battle's highlight of units (BattleHighlight_SetModesFar with mode
/// 0); Menu_ConfirmSelection calls it as an entry is chosen.
void Menu_ClearBattleHighlight(struct SelectionMenuState *state) {
  u16 data[6];
  struct SelectionNode *node = state->head;
  s32 count = 0;

  while (node != 0) {
    node = node->next;
    count++;
  }
  data[count] = BATTLE_UNIT_LIST_END;
  BattleHighlight_SetModesFar(data, 0);
}

/// Scrolls the row one entry: forward brings an icon in from the right as the
/// leftmost shrinks away, and back the reverse; waits until it has moved.
void SelectionMenu_Scroll(struct SelectionMenuState *state, s32 forward) {
  struct SelectionNode *node;
  struct SelectionNode *last;
  struct SelectionNode *first;
  s32 base;
  s32 kind;
  u32 entryIndex;
  s32 rowX;
  s32 rowY;

  if (forward != 0) {
    entryIndex = state->top + SELECTION_LAST_PLACE;
    base = state->bases[entryIndex];
    kind = state->kinds[entryIndex];
    node = SelectionMenu_FindFreeIcon(SELECTION_ICON_ROW);
    if (node == NULL)
      return;
    SelectionMenu_SetupIcon(kind, base, node, 0);
    rowX = state->baseX;
    node->x = rowX + (SELECTION_LAST_PLACE + 1) * SELECTION_ICON_SPACING;
    rowY = state->baseY;
    node->targetX = rowX + SELECTION_LAST_PLACE * SELECTION_ICON_SPACING;
    node->y = rowY;
    node->targetY = rowY;
    node->scaleStep = Q_8_8(1) / 8;
    node->scale = Q_8_8(1) / 8;
    node->scaleEnd = Q_8_8(1);
    node->dx = -2;
    last = node;
    node = state->head;
    node->scaleStep = -Q_8_8(1) / 8;
    node->scaleEnd = 0;
    for (;;) {
      node->targetX = node->x - SELECTION_ICON_SPACING;
      node->dx = -2;
      if (node->next == NULL)
        break;
      node = node->next;
    }
    node->next = last;
    last->next = NULL;
    last->previous = node;
    node = state->head;
    do {
      wait(1);
    } while (node->scale != 0);
    state->head = node->next;
    VramCache_FreeEntry(node->slot);
    node->kind = ICON_KIND_NONE;
    node = node->next;
    node->previous = NULL;
  } else {
    entryIndex = state->top;
    base = state->bases[entryIndex];
    kind = state->kinds[entryIndex];
    node = SelectionMenu_FindFreeIcon(SELECTION_ICON_ROW);
    if (node == NULL)
      return;
    SelectionMenu_SetupIcon(kind, base, node, 0);
    node->x = state->baseX - SELECTION_ICON_SPACING;
    node->y = state->baseY;
    node->targetY = node->y;
    node->dx = 2;
    node->scale = Q_8_8(1) / 8;
    node->scaleStep = Q_8_8(1) / 8;
    node->targetX = node->x + SELECTION_ICON_SPACING;
    node->scaleEnd = Q_8_8(1);
    first = node;
    node = state->head;
    node->previous = first;
    first->next = node;
    first->previous = NULL;
    state->head = first;
    node = first;
    for (;;) {
      node->targetX = node->x + SELECTION_ICON_SPACING;
      node->dx = 2;
      if (node->next == NULL)
        break;
      node = node->next;
    }
    node->scaleEnd = 0;
    node->scaleStep = -Q_8_8(1) / 8;
    node = state->head;
    do {
      wait(1);
    } while (node->scale != Q_8_8(1));
    while (node->next != NULL)
      node = node->next;
    VramCache_FreeEntry(node->slot);
    node->kind = ICON_KIND_NONE;
    node->previous->next = NULL;
  }
}

/// Loads the icon of an entry of kind mode (ICON_KIND_COMMAND,
/// ICON_KIND_YES_NO, ICON_KIND_ITEM, ICON_KIND_ABILITY or
/// ICON_KIND_TABLE_ENTRY) into a free sprite tile slot and returns it, or -1.
s32 SelectionIcon_Load(s32 mode, s32 value) {
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
  case ICON_KIND_TABLE_ENTRY:
    result = VramCache_FindFreeEntry();
    if (result == VRAM_CACHE_ENTRY_COUNT)
      return -1;
    MenuTitle_LoadFirstTile(value, 0, result);
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
  case ICON_KIND_TABLE_ENTRY:
    MenuTitle_LoadFirstTile(value, 0, result);
    break;
  }

  return result;
}

/// Sets up a selection icon for an entry: its icon tiles (in its old slot when
/// reuse is set), its name message and its sprite. kind must select a command,
/// item or Psynergy with the matching base ID.
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
  sprite->colors = 0;
  sprite->mosaic = 0;

  sprite->size = 1;
  sprite->shape = SPRITE_SHAPE_SQUARE;
  sprite->palette = 0;
  sprite->tile = tile;
  sprite->priority = 0;
}

/// Chooses the entry under the cursor: the other icons gather into it and
/// vanish, and it joins the chosen icons at the top left.
u32 Menu_ConfirmSelection(struct SelectionMenuState *state) {
  struct SelectionNode *node;
  struct SelectionNode *other = NULL;
  u32 nodeIndex = 0;
  u32 chosenIndex;
  struct SelectionSprite *sprite;

  chosenIndex = state->top + state->cursorIndex;
  Menu_ClearBattleHighlight(state);
  SelectionMenu_RedrawIcon(state, state->cursorIndex);
  state->status = SELECTION_STATUS_SCROLLING;
  wait(1);
  state->records[SELECTION_ARROW_LEFT].kind = FALSE;
  state->records[SELECTION_ARROW_RIGHT].kind = FALSE;
  state->records[SELECTION_RECORD_HAND].kind = 0;
  state->records[SELECTION_RECORD_HAND].scale = 0;
  SelectionMenu_FreeLargeIcon();
  node = state->head;
  for (; node != NULL && state->cursorIndex != nodeIndex; nodeIndex++) {
    node = node->next;
  }
  node->homeX = node->x;
  node->homeY = node->y;
  // The others slide halfway in each of two frames, then vanish.
  for (other = state->head; other != NULL; other = other->next) {
    if (other != node) {
      other->targetX = node->x;
      other->dx = (node->x - other->x) >> 1;
    }
  }
  wait(2);
  for (other = state->head; other != NULL; other = other->next) {
    if (other != node) {
      VramCache_FreeEntry(other->slot);
      other->kind = ICON_KIND_NONE;
    }
  }
  state->head = node;
  node->previous = NULL;
  node->next = NULL;
  // The chosen icons line up from x 4, 16 pixels apart; the new one goes
  // after the last, reaching its place over two frames, and its record moves
  // to the chosen list.
  node->targetX = 4;
  for (other = state->path, nodeIndex = 0; other != NULL; other = other->next) {
    node->targetX += SELECTION_ICON_SPACING;
    nodeIndex++;
  }
  state->pathTop[nodeIndex] = state->top;
  state->pathCursor[nodeIndex] = state->cursorIndex;
  node->dx = (node->targetX - node->x) >> 1;
  state->unread = 0;
  state->cursorIndex |= SELECTION_CURSOR_RESET;
  wait(2);
  other = SelectionMenu_FindFreeIcon(SELECTION_ICON_CHOSEN);
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
  sprite->colors = 0;
  sprite->mosaic = 0;
  sprite->size = 1;
  sprite->shape = SPRITE_SHAPE_SQUARE;
  sprite->palette = 0;
  sprite->tile = other->tile;
  node->kind = ICON_KIND_NONE;
  state->head = NULL;
  if (state->path != NULL) {
    node = state->path;
    while (node->next != NULL)
      node = node->next;
    node->next = other;
    other->previous = node;
    other->next = NULL;
  } else {
    state->path = other;
    other->previous = NULL;
    other->next = NULL;
  }
  return chosenIndex;
}

/// Six empty functions; nothing calls the first, the fourth or the fifth. The
/// others are reached only through bare entries of the window far-call table.
UNUSED void MenuArrow_NoOpCallback(void) {}
void MenuArrow_NoOpCallbackBank(void) {}
void MenuArrow_NoOpCallbackA(void) {}
UNUSED void MenuArrow_NoOpCallbackB(void) {}
UNUSED void MenuArrow_NoOpCallbackC(void) {}
void MenuArrow_NoOpCallbackD(void) {}

/// Sets up a sprite as the right-pointing arrow, in a free tile slot (returned
/// in *slot).
void MenuArrow_Create(struct MenuSprite *sprite, s32 *slot) {
  const void *data = gMenu_CursorActorTiles;
  s32 cacheSlot = VramCache_FindFreeEntry();

  *slot = cacheSlot;
  sprite->oam.f.tile = VramCache_Load(cacheSlot, ARROW_FRAME_BYTES, data);
  sprite->oam.f.mode = OBJ_MODE_NORMAL;
  sprite->oam.f.mosaic = 0;
  sprite->oam.f.colors = 1;
  sprite->oam.f.affine = SPRITE_AFFINE_OFF;
  sprite->oam.f.affineIndex = 0;
  sprite->oam.f.size = 0;
  sprite->oam.f.shape = SPRITE_SHAPE_TALL;
  sprite->oam.f.priority = 0;
}

/// Places an arrow sprite at screen pixel (x, y) and queues it for drawing
/// this frame, at depth slot 252; the caller calls it every frame it shows.
void MenuArrow_SetPosition(struct MenuSprite *sprite, s32 x, s32 y) {
  sprite->oam.f.x = x;
  sprite->oam.f.y = y;
  Oam_AddSpriteAtDepth(sprite, OAM_DEPTH_FRONT - 3);
}

/// Frees an arrow sprite's tile slot.
void MenuArrow_Release(s32 value) { VramCache_FreeEntry(value); }

/// Loads the large icon of the command under the cursor, zooming in: 32x32 in
/// 256 colours (32 * 32 bytes, record 15), its scale stepping up by
/// LARGE_ICON_ZOOM_STEP a frame, after which SelectionMenu_Draw shows it at its
/// own size.
void SelectionMenu_LoadLargeIcon(void) {
  struct SelectionMenuState *state = gSelectionMenuState;
  struct SelectionNode *selected = SelectionMenu_GetCursorNode(state);
  struct SelectionNode *largeIcon;
  IconTransfer *buffer;
  u8 *table;
  void *resource;
  s32 resourceId;

  if (selected->kind != ICON_KIND_COMMAND && selected->kind != ICON_KIND_YES_NO)
    return;

  buffer = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
  largeIcon = &state->records[SELECTION_RECORD_LARGE_ICON];
  resourceId = selected->base;
  // The resource starts with a halfword offset per command icon.
  table = Resource_GetTableEntry(RESOURCE_COMMAND_ICONS);
  {
    u8 **tiles = &buffer->encoded;
    resource = table + *(u16 *)(table + selected->base * 2);
    *tiles = resource;
  }
  Decompress_ByteLz(resource, buffer);

  if (largeIcon->kind == ICON_KIND_NONE)
    largeIcon->slot = VramCache_FindFreeEntry();
  largeIcon->tile = VramCache_Load(largeIcon->slot, 32 * 32, buffer);
  largeIcon->kind = ICON_KIND_COMMAND;
  largeIcon->base = resourceId;
  largeIcon->scale = LARGE_ICON_ZOOM_STEP;
  largeIcon->scaleStep = LARGE_ICON_ZOOM_STEP;
  largeIcon->scaleEnd = 6 * LARGE_ICON_ZOOM_STEP;
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
}

/// Frees the large command icon, if it is shown.
/// The large preview owns a cache slot independently of the visible row icons.
void SelectionMenu_FreeLargeIcon(void) {
  struct SelectionNode *largeIcon =
      &gSelectionMenuState->records[SELECTION_RECORD_LARGE_ICON];

  if (largeIcon->kind != ICON_KIND_NONE) {
    VramCache_FreeEntry(largeIcon->slot);
    largeIcon->kind = ICON_KIND_NONE;
  }
}

/// Runs the field menu until the player picks something that leaves it, and
/// returns the chosen screen's result. Cancelling the menu returns an
/// undefined value, the missing result of PartyStatus_CloseWithMenu (0 under
/// UBFIX); its one caller, FieldSequence_Run, ignores the result. Its case for
/// command 0, which stores the trigger tile ahead in the field state's
/// examineCell, is unreachable.
s32 FieldMenu_Run(void) {
  s32 ret;
  s32 selection;
  struct FieldState *field;

  field = gFieldState;
  selection = 0;

  for (;;) {
    PartyStatus_OpenAndWait();
    selection = FieldMenu_SelectCommand(selection);
    // German, Spanish, French and Italian: mark the menu open, which pauses
    // Reveal's sparkles, and, while Reveal is active, clear its sparkles.
#if TBS_PATCH_2
    field->menuOpen = 1;
    if (field->revealActive != 0) {
      FieldEffect_ReleaseRevealSparklesFar();
      wait(1);
    }
#endif
#ifdef UBFIX
    // UB: the original reads a void return. The default result is ignored by
    // the only caller, so give it a defined value after closing the window.
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
      field->examineCell = ret;
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

#if TBS_PATCH_2
  field->menuOpen = 0;
#endif
  return ret;
}

/// Opens the party status window that shows with the field menu (flags 1)
/// and waits a frame.
void PartyStatus_OpenAndWait(void) {
  PartyStatus_CreateWindow(1);
  wait(1);
}

/// Closes the party status window, kept in heap block 16 (gPartyStatusState),
/// and frees the block.
void PartyStatus_CloseWithMenu(void) { PartyStatus_Close(); }

/// Sets up an empty selection screen and waits a frame.
UNUSED void SelectionMenu_CreateEmpty(void) {
  SelectionMenu_Create();
  SelectionMenu_Clear();
  wait(1);
}

/// Runs a selection row with the cursor on entry initialIndex, cancellable, and
/// returns the entry chosen or -1. initialIndex is a row index; cancellation
/// returns -1 encoded as u32.
u32 Menu_RunSelectionForValue(u32 initialIndex) {
  struct SelectionMenuState *state = gSelectionMenuState;
  u32 result;

  // locked 1: a yes or no row stays at the bottom.
  state->cursorIndex = initialIndex;
  state->locked = 1;
  SelectionMenu_BuildEntries(initialIndex);
  Menu_SetupSelectionBothSides();
  SelectionMenu_OpenNameWindow(0, SELECTION_ROW_SHOWN);
  SelectionMenu_StartDrawing();
  result = SelectionMenu_RunWithSounds(1);
  SelectionMenu_Close();
  return result;
}

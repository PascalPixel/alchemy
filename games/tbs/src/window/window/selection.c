/// @file
/// The selection screen's row of icons, and the field menu.
// Part of window.c.

#include "../window_internal.h"

/// Two halfwords of a selection record from its base field on, the second
/// cleared as its cursor: SelectionScreen_Create frees records 7 and 8 through
/// it.
struct GlyphCursor {
  u16 unused; ///< the record's base
  u16 cursor; ///< the record's kind
};

/// Allocates and resets the selection screen: the row of icons the player picks
/// from, with arrows at each end and a pointing hand over the chosen target.
/// The work is HEAP_SLOT_SELECTION_SCREEN (gSelectionScreenWork), which
/// SelectionScreen_Close frees; the hand's picture takes a VRAM cache
/// entry for that long.
void SelectionScreen_Create(void) {
  struct SelectionScreenWork *screen;
  struct SelectionSprite *visual;
  const u8 *table;
  s32 i;
  u16 clear = 0;

  screen = AllocHeapSlotEwram(HEAP_SLOT_SELECTION_SCREEN,
                              sizeof(struct SelectionScreenWork));
  screen->head = NULL;
  screen->path = NULL;
  screen->window = NULL;
  screen->unread = clear;
  screen->top = clear;
  // Bit 7: the first list starts from its first entry. Until the list is
  // first cleared the row is drawn 32 pixels lower and takes no input.
  screen->cursorIndex = 128;
  screen->verticalScroll = 32;
  screen->count = clear;
  screen->locked = 999;
  // The records: 0 and 1 the end arrows, 2 to 8 the row's icons, 9 to 13 the
  // chosen icons, 14 the pointing hand and 15 the large icon.
  i = 0;
  do {
    screen->records[i + 2].kind = ICON_KIND_NONE;
    screen->records[i + 9].kind = ICON_KIND_NONE;
    i++;
  } while (i != 5);
  // Cleared through a view from each record's base field, which forms
  // screen + 372 and stores at #2; through kind the address differs.
  ((struct GlyphCursor *)&screen->records[7].base)->cursor = 0;
  ((struct GlyphCursor *)&screen->records[8].base)->cursor = 0;
  screen->records[0].kind = 0;
  screen->records[1].kind = 0;
  screen->records[0].y = 0;
  screen->records[1].y = 0;
  // The hand: a 16x16 sprite of 256 colours, 256 bytes a frame.
  table = gMenu_AnimatedCursorTiles;
  screen->records[14].slot = VramCache_FindFreeEntry();
  screen->records[14].tile =
      VramCache_Load(screen->records[14].slot, 256, table);
  screen->records[14].kind = 0;
  screen->records[14].scale = 0;
  screen->records[15].kind = ICON_KIND_NONE;
  visual = &screen->records[14].sprite;
  visual->mode = 0;
  visual->mosaic = 0;
  visual->colors = 1;
  visual->affine = 0;
  visual->matrix = 0;
  visual->size = 1;
  visual->shape = 0;
  visual->priority = 0;
}

/// Empties the selection screen's list of entries.
/// The high cursor bit requests resetting both the list origin and cursor.
void SelectionScreen_Clear(void) {
  struct SelectionScreenWork *screen;

  screen = gSelectionScreenWork;
  screen->head = NULL;
  screen->unread = 0;
  if (screen->cursorIndex & 0x80) {
    screen->top = 0;
    screen->cursorIndex = 0;
  }
  screen->verticalScroll = 0;
  screen->count = 0;
}

/// Adds an entry (kind and id) to the selection screen's list, up to 16.
/// The count starts at 0 when the screen is created or cleared. The kinds
/// SelectionScreen_SetupIcon draws (enum IconKind) are ICON_KIND_COMMAND,
/// ICON_KIND_YES_NO, ICON_KIND_ITEM and ICON_KIND_ABILITY; base is the
/// command's, item's or ability's number.
void SelectionScreen_AddEntry(u32 kind, u32 base) {
  struct SelectionScreenWork *state = gSelectionScreenWork;
  u16 entryCount = state->count;

  if (entryCount != SELECTION_SCREEN_ENTRY_LIMIT) {
    state->kinds[entryCount] = kind;
    state->bases[entryCount] = base;
    state->count++;
  }
}

/// Creates icons for the first five listed entries in a row along the bottom of
/// the screen.
/// Builds the linked row before placing the visible icons and loading the
/// preview.
void SelectionScreen_BuildEntries(u32 unused) {
  struct SelectionScreenWork *screen = gSelectionScreenWork;
  u32 count = screen->count;
  u32 entryIndex = screen->top;
  struct SelectionNode *previous = 0;
  struct SelectionNode *node;
  s32 rowPosition = 0;

  while (entryIndex < count) {
    s32 base = screen->bases[entryIndex];
    u32 kind = screen->kinds[entryIndex];

    node = SelectionScreen_FindFreeIcon(SELECTION_ICON_ROW);
    if (node == 0)
      break;
    SelectionScreen_SetupIcon(kind, base, node, 0);
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
    entryIndex++;
  }

  // Icons are 16 pixels apart: the row is centred on x 100, at y 140.
  screen->baseX = 100 - rowPosition * 8;
  screen->baseY = 140;
  // Reuse the count as a pixel offset while placing the completed row.
  // Separate count/offset locals change the native English object.
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
    rowPosition += 16;
  }
  SelectionScreen_LoadLargeIcon();
}

/// An empty function, reached only through a bare entry of the window
/// far-call table (system/far_call/WINDOW.S).
void SelectionScreen_ReservedNoOp(void) {}

/// Returns a free selection icon record of `kind` (enum SelectionIconGroup),
/// or NULL.
struct SelectionNode *SelectionScreen_FindFreeIcon(s32 kind) {
  struct SelectionScreenWork *state = gSelectionScreenWork;
  s32 i;

  if (kind != SELECTION_ICON_ROW) {
    for (i = 0; i != 5; i++) {
      if (state->records[i + 9].kind == ICON_KIND_NONE)
        return &state->records[i + 9];
    }
  } else {
    for (i = 0; i != 7; i++) {
      if (state->records[i + 2].kind == ICON_KIND_NONE)
        return &state->records[i + 2];
    }
  }
  return NULL;
}

/// Starts drawing the selection screen each frame, in the main loop's pass
/// (0xc80).
void SelectionScreen_StartDrawing(void) {
  Task_Add(SelectionScreen_Draw, 0xC80);
}

/// Stops drawing the selection screen.
void SelectionScreen_StopDrawing(void) { Task_Remove(SelectionScreen_Draw); }

/// The selection screen's per-frame callback: slides and draws the row of
/// icons, the large icon over the chosen one, the end arrows, the icons already
/// chosen and the pointing hand. The depth slots: 240 for the row and the
/// chosen icons, 241 for the icon under the cursor, 248 for the large icon and
/// the hand, 238 for the arrows. In battle the sprites are semi-transparent
/// (OBJ mode 1) while the hand is shown, and a command's icon and the large
/// icon always.
void SelectionScreen_Draw(void) {
  struct SelectionScreenWork *state = gSelectionScreenWork;
  struct SelectionSprite *cursorEntry = &state->records[14].sprite;
  struct SelectionNode *cursor = &state->records[14];
  struct SelectionNode *transfer = &state->records[15];
  struct SelectionNode *node;
  struct SelectionSprite *sprite;
  struct SelectionSprite *commandSprite;
  s32 entryIndex = 0;
  s32 slot;
  s32 axisStep;
  struct ScreenPoint position;

  node = state->head;
  // Move and queue the visible row before drawing its overlays. An icon
  // slides dx pixels a frame until it reaches its target; the one under the
  // cursor, once still, points the hand at its battle target.
  while (node != 0) {
    sprite = &node->sprite;
    sprite->affine = 0;
    sprite->matrix = 0;
    sprite->x = node->x;
    sprite->y = node->y;
    slot = 240;
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
      slot = 241;
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
        if (state->records[14].kind == ICON_KIND_COMMAND) {
          sprite->mode = 1;
        } else {
          sprite->mode = 0;
        }
        if (node->kind == ICON_KIND_COMMAND) {
          sprite->mode = 1;
        }
      }
      Oam_AddSpriteAtDepth(sprite, slot);
    }
    node = node->next;
    entryIndex++;
  }
  // The selected command's large icon owns a separate cached tile block.
  if (transfer->kind != ICON_KIND_NONE) {
    node = SelectionScreen_GetCursorNode(state);
    commandSprite = &transfer->sprite;
    commandSprite->mode = 0;
    commandSprite->affine = 0;
    commandSprite->matrix = 0;
    commandSprite->mosaic = 0;
    commandSprite->colors = 1;
    commandSprite->size = 2;
    commandSprite->shape = 0;
    commandSprite->priority = 0;
    commandSprite->tile = transfer->tile;
    // A 32x32 sprite (size 2) over the icon, bobbing by half the cursor's
    // bob; while it zooms in, the double-size area moves it 16 up and left
    // (0xfff0 is -16 in the 9-bit x, 0xf0 in the 8-bit y).
    commandSprite->x = node->x - 4;
    commandSprite->y =
        node->y + (gMenuCursor_BobOffsets[(gVBlankCount >> 1) & 15] >> 1) - 4;
    if (transfer->scale != transfer->scaleEnd) {
      state->transform.scaleX = transfer->scale;
      state->transform.scaleY = transfer->scale;
      state->transform.angle = 0;
      commandSprite->matrix = OamMatrix_AllocFromTransform(&state->transform);
      commandSprite->affine = 3;
      commandSprite->x += 0xfff0;
      commandSprite->y += 0xf0;
      transfer->scale += transfer->scaleStep;
    }
    if (GameFlag_GetFar(FLAG_IN_BATTLE) != 0) {
      commandSprite->mode = 1;
    }
    Oam_AddSpriteAtDepth(commandSprite, 248);
  }
  SelectionScreen_DrawArrow(state, 0);
  SelectionScreen_DrawArrow(state, 1);
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
      sprite->affine = 3;
      sprite->x += 0xfff8;
      sprite->y += 0xf8;
    } else {
      sprite->affine = 0;
      sprite->matrix = 0;
    }
    if (GameFlag_GetFar(FLAG_IN_BATTLE) != 0) {
      if (state->records[14].kind == ICON_KIND_COMMAND) {
        sprite->mode = 1;
      } else {
        sprite->mode = 0;
      }
      if (node->kind == ICON_KIND_COMMAND) {
        sprite->mode = 1;
      }
    }
    Oam_AddSpriteAtDepth(sprite, 240);
    node = node->next;
  }
  // Point at the selected battle target, smoothing each axis separately: the
  // hand covers half the distance left each frame. Its 16 frames of 256 bytes
  // change every 4 frames.
  if (cursor->kind != ICON_KIND_NONE) {
    entryIndex = (s32)gMenu_AnimatedCursorTiles;
    {
      void *src = (void *)(entryIndex + (((gVBlankCount >> 2) & 15) << 8));

      cursorEntry->tile = VramCache_Load(cursor->slot, 0x100, src);
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
      if (state->records[14].kind == ICON_KIND_COMMAND) {
        cursorEntry->mode = 1;
      } else {
        cursorEntry->mode = 0;
      }
    }
    Oam_AddSpriteAtDepth(cursorEntry, 248);
  }
  state->status += 1;
}

/// Draws the selection screen's left (index 0) or right (1) arrow, nudged
/// outwards while it flashes after a scroll: the record's base holds the
/// frames left of the nudge (8 after each scroll) and is the nudge in pixels.
void SelectionScreen_DrawArrow(struct SelectionScreenWork *state, s32 side) {
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
    arrowTiles = gMenu_CursorObjectTiles;
    if (state->records[1].base != 0)
      arrow->x = arrow->x + state->records[1].base;
  } else {
    arrowTiles = gMenu_CursorLeftObjectTiles;
    if (state->records[0].base != 0)
      arrow->x = arrow->x - state->records[0].base;
  }
  tmp = arrowTiles + animationStep * 128;
  arrow->tile = VramCache_Load(state->records[side].slot, 128, tmp);
  if (GameFlag_GetFar(FLAG_IN_BATTLE)) {
    if (state->records[14].kind == ICON_KIND_COMMAND)
      arrow->mode = 1;
    else
      arrow->mode = 0;
  }
  Oam_AddSpriteAtDepth(arrow, 238);
  if (state->records[side].base != 0)
    state->records[side].base--;
}

/// Opens or refits the name window above the selection row and writes the name
/// of the icon under the cursor, or that nothing is known (ICON_KIND_ABILITY)
/// or held (ICON_KIND_ITEM). ICON_KIND_YES_NO is a yes or no question, placed
/// at the bottom or top of the screen. count is the name's width in tiles
/// inside the frame; 0 keeps the open window as it is. The window stays open
/// until SelectionScreen_Close closes it.
void SelectionScreen_OpenNameWindow(s32 mode, u32 count) {
  struct SelectionScreenWork *screen;
  struct SelectionNode *node;
  struct Window **slot;
  struct Window *window;

  screen = gSelectionScreenWork;
  node = SelectionScreen_GetCursorNode(screen);
  slot = &screen->window;
  window = *slot;
  if (window == 0) {
    // A yes or no window, 5 by 3 tiles, from column 17 at the bottom, or the
    // top while locked is 0. Otherwise a window count + 2 tiles wide at row
    // 17, centred about 24.5 tiles from the left. Both take flags 6:
    // WINDOW_FLAG_INSTANT and bit 2, which no flag names.
    if (mode == ICON_KIND_YES_NO) {
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
    Text_DrawMessageAsTiles(node->message, screen->window, 0, 0);
  } else {
    switch (mode) {
    case ICON_KIND_ABILITY:
      Text_DrawMessageAsTiles(MsgAbilityNotKnown, screen->window, 0, 0);
      break;
    case ICON_KIND_ITEM:
      Text_DrawMessageAsTiles(MsgItemNotHeld, screen->window, 0, 0);
      break;
    }
  }
}

/// Closes the selection screen and frees its icons, tiles and work block (heap
/// slot 18). The chosen icons go too, so the choices are only in the indexes
/// the loops returned.
void SelectionScreen_Close(void) {
  struct SelectionScreenWork *state = gSelectionScreenWork;
  struct SelectionNode *node;

  SelectionScreen_StopDrawing();
  Window_Close(state->window, 2);
  WaitFrames(1);
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
  // An arrow's y is nonzero once SelectionScreen_SetupArrow has given it a
  // slot. The right arrow's slot is freed on the left arrow's y, read twice.
  if (state->records[0].y != 0) {
    VramCache_FreeEntry(state->records[0].slot);
    if (state->records[0].y != 0) {
      VramCache_FreeEntry(state->records[1].slot);
    }
  }
  VramCache_FreeEntry(state->records[14].slot);
  FreeHeapSlot(HEAP_SLOT_SELECTION_SCREEN);
}

/// Moves the row of icons to start at pixel (x, y), 16 pixels apart.
void SelectionScreen_SetPosition(u32 x, u32 y) {
  struct SelectionScreenWork *screen = gSelectionScreenWork;
  struct SelectionNode *node;

  screen->baseX = x;
  screen->baseY = y;
  node = screen->head;
  while (node != 0) {
    node->x = x;
    node->targetX = x;
    node->y = y;
    node->targetY = y;
    node = node->next;
    x += 16;
  }
}

/// Sets up both end arrows of the selection row.
void Menu_SetupSelectionBothSides(void) {
  struct SelectionScreenWork *state;

  state = gSelectionScreenWork;
  SelectionScreen_SetupArrow(state, 0);
  SelectionScreen_SetupArrow(state, 1);
}

/// Sets up the left (index 0) or right (1) arrow of the selection row, shown
/// when there are entries beyond that end. The first call takes the arrow's
/// VRAM cache entry, which it keeps until SelectionScreen_Close; later
/// calls only move it and decide whether it shows.
void SelectionScreen_SetupArrow(struct SelectionScreenWork *state, s32 side) {
  struct SelectionSprite *arrow = &state->records[side].sprite;
  const u8 *arrowTiles = 0;
  u32 count;

  state->records[side].kind = 0;
  if (side != 0) {
    /* FAKEMATCH(temporary): the tile pointer in a local, loaded early into r5;
     * as gMenu_CursorObjectTiles where right is read, 35 of 136 instructions
     * differ in tbs-en. */
    const u8 *right = gMenu_CursorObjectTiles;

    count = state->count;
    if (state->top != 0)
      count -= state->top;
    if (count > 5) {
      state->records[side].kind = 1;
      count = 5;
    }
    arrowTiles = right;
    // Just past the last of the at most five icons shown.
    state->records[1].x = state->baseX + 16 * (count - 1) + 17;
  } else {
    arrowTiles = gMenu_CursorLeftObjectTiles;
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
    arrow->matrix = 0;
    arrow->size = 0;
    arrow->shape = 2;
    arrow->priority = 0;
  }
}

/// Returns the node at the selection screen's cursor index.
/// cursorIndex is relative to head, not the number of queued entries.
struct SelectionNode *
SelectionScreen_GetCursorNode(struct SelectionScreenWork *screen) {
  struct SelectionNode *node = screen->head;
  s32 i;

  for (i = 0; i != screen->cursorIndex; ++i) {
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

/// Runs the selection row until the player chooses (the chosen entry's index)
/// or, when mode allows it, cancels with B (-1). SELECTION_FIXED allows no
/// moving or choosing; any mode but 0 allows cancelling. Reached only through
/// a bare entry of the window far-call table.
s32 SelectionScreen_Run(s32 mode) {
  struct SelectionScreenWork *screen = gSelectionScreenWork;

  SelectionScreen_HighlightIcon(screen, 0);
  for (;;) {
    WaitFrames(1);
    // No input while the row is pushed down.
    if (screen->verticalScroll != 0) {
      continue;
    }
    if (mode != SELECTION_FIXED) {
      if (gKeysRepeat & KEY_RIGHT) {
        SelectionScreen_StepRight(screen);
      } else if (gKeysRepeat & KEY_LEFT) {
        SelectionScreen_StepLeft(screen);
      } else if (gKeysPressed & KEY_A) {
        return Menu_ConfirmSelection(screen);
      }
    }
    if (mode != 0 && (gKeysPressed & KEY_B)) {
      return -1;
    }
  }
}

/// As SelectionScreen_Run, with the menu sounds.
u32 SelectionScreen_RunWithSounds(u32 mode) {
  struct SelectionScreenWork *screen = gSelectionScreenWork;
  u32 result;

  for (;;) {
    WaitFrames(1);
    if (screen->verticalScroll != 0) {
      continue;
    }

    if (mode != SELECTION_FIXED) {
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        SelectionScreen_StepRight(screen);
      } else if (gKeysRepeat & KEY_LEFT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        SelectionScreen_StepLeft(screen);
      }

      if (gKeysPressed & KEY_A) {
        // On a yes or no row, entry 0 (yes) sounds as a confirm and the
        // other as a cancel.
        result = screen->top + screen->cursorIndex;
        if (screen->head->kind == NODE_KIND_YES_NO) {
          if (result == 0)
            Sound_PlaySongFar(SE_MENU_CONFIRM);
          else
            Sound_PlaySongFar(SE_MENU_CANCEL);
        } else {
          Sound_PlaySongFar(SE_MENU_CONFIRM);
        }
        return result;
      }
    }

    if (mode != 0 && (gKeysPressed & KEY_B)) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      return -1;
    }
  }
}

/// Moves the cursor one entry right, scrolling the row at its fourth place.
/// This variant stops at the end; SelectionScreen_StepRight also wraps around.
/// Nothing calls it.
UNUSED void Menu_MoveSelectionForward(struct SelectionScreenWork *screen) {
  u32 nextEntry = screen->top + screen->cursorIndex + 1;

  if (nextEntry == screen->count) {
    return;
  }
  SelectionScreen_RedrawIcon(screen, screen->cursorIndex);
  screen->status = NODE_STATUS_SCROLLING;
  WaitFrames(1);
  screen->cursorIndex++;
  if (screen->cursorIndex == SELECTION_ROWS && nextEntry + 1 < screen->count) {
    screen->cursorIndex--;
    screen->records[1].base = ARROW_SCROLL_FRAMES;
    screen->top++;
    SelectionScreen_Scroll(screen, TRUE);
    if (screen->top + screen->cursorIndex + 2 == screen->count) {
      screen->records[1].kind = FALSE;
    }
    screen->records[0].kind = TRUE;
  }
  screen->status = NODE_STATUS_SHOWN;
  SelectionScreen_HighlightIcon(screen, screen->cursorIndex);
  WaitFrames(1);
  SelectionScreen_OpenNameWindow(screen->head->kind, 0);
  WaitFrames(1);
}

/// Moves the cursor one entry left, scrolling the row at its second place.
/// This variant stops at the beginning; SelectionScreen_StepLeft also wraps
/// around. Nothing calls it.
UNUSED void Menu_MoveSelectionBackward(struct SelectionScreenWork *screen) {
  u32 cursorIndex;

  if (screen->top == 0 && screen->cursorIndex == 0) {
    return;
  }
  SelectionScreen_RedrawIcon(screen, screen->cursorIndex);
  screen->status = NODE_STATUS_SCROLLING;
  WaitFrames(1);
  cursorIndex = screen->cursorIndex;
  if (cursorIndex == 1 && screen->top != 0) {
    screen->records[0].base = ARROW_SCROLL_FRAMES;
    screen->top--;
    SelectionScreen_Scroll(screen, FALSE);
    if (screen->top == 0) {
      screen->records[0].kind = FALSE;
    }
    screen->records[1].kind = cursorIndex;
  } else {
    screen->cursorIndex--;
  }
  screen->status = NODE_STATUS_SHOWN;
  SelectionScreen_HighlightIcon(screen, screen->cursorIndex);
  WaitFrames(1);
  SelectionScreen_OpenNameWindow(screen->head->kind, 0);
  WaitFrames(1);
}

/// Moves the cursor one entry right, scrolling the row at its edge and
/// wrapping from the last entry to the first.
void SelectionScreen_StepRight(struct SelectionScreenWork *state) {
  struct SelectionNode *node;
  u32 entryIndex;
  s32 y;
  u32 end;

  SelectionScreen_RedrawIcon(state, state->cursorIndex);
  state->status = NODE_STATUS_SCROLLING;
  WaitFrames(1);
  state->cursorIndex++;
  // With more entries than the five shown, stepping past the last gathers the
  // icons at the row's start (12 pixels a frame), refills them from the first
  // entry and fans them out again; stepping onto the fifth place with more to
  // come scrolls the row instead, the cursor staying on the fourth. A short
  // row just wraps the cursor.
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
        WaitFrames(1);
      node = state->head;
      if (node != NULL) {
        entryIndex = 0;
        do {
          SelectionScreen_SetupIcon(state->kinds[entryIndex],
                                    state->bases[entryIndex], node, 1);
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
      SelectionScreen_Scroll(state, 1);
      if (state->top + state->cursorIndex + 2 == state->count)
        state->records[1].kind = 0;
      state->records[0].kind = 1;
    }
  } else if (state->cursorIndex == state->count) {
    state->cursorIndex = 0;
  }
  state->status = NODE_STATUS_SHOWN;
  SelectionScreen_HighlightIcon(state, state->cursorIndex);
  WaitFrames(1);
  SelectionScreen_OpenNameWindow(state->head->kind, 0);
  WaitFrames(1);
}

/// Moves the cursor one entry left, scrolling the row at its edge and
/// wrapping from the first entry to the last.
void SelectionScreen_StepLeft(struct SelectionScreenWork *state) {
  struct SelectionNode *node;
  s32 i;

  SelectionScreen_RedrawIcon(state, state->cursorIndex);
  state->status = NODE_STATUS_SCROLLING;
  WaitFrames(1);
  // As SelectionScreen_StepRight, mirrored: from the second place with entries
  // before it the row scrolls back; from the first entry the icons gather at
  // the row's last place (12 pixels a frame), refill from the last five entries
  // and fan out again, with the cursor on the last.
  if (state->count > 5) {
    if ((state->cursorIndex | state->top) != 0) {
      if (state->cursorIndex == 1 && state->top != 0) {
        state->records[0].base = 8;
        state->top--;
        SelectionScreen_Scroll(state, 0);
        if (state->top == 0)
          state->records[0].kind = 0;
        state->records[1].kind = 1;
      } else {
        state->cursorIndex--;
      }
    } else {
      node = state->head;
      i = 64;
      {
        /* FAKEMATCH(pin): the zero in r0, live across the first loop as the
         * native has it; as a plain store of 0, 6 of 184 instructions differ
         * in tbs-en. */
        register s32 z asm("r0") = 0;
        state->records[1].kind = z;
      }
      while (node->next != NULL) {
        node->targetX = node->x + i;
        node->dx = 12;
        node = node->next;
        i -= 16;
      }
      node = state->head;
      while (node->x != node->targetX)
        WaitFrames(1);
      // Keep the native count-up: assigning count - 5 directly removes
      // 20 bytes from the English object.
      i = 0;
      while (i != state->count - 5)
        i++;
      node = state->head;
      state->top = i;
      state->cursorIndex = 4;
      while (node != NULL) {
        SelectionScreen_SetupIcon(state->kinds[i], state->bases[i], node, 1);
        node = node->next;
        i++;
      }
      node = state->head;
      i = state->baseX;
      while (node->next != NULL) {
        node->targetX = i;
        node->dx = -12;
        node = node->next;
        i += 16;
      }
      state->records[0].kind = 1;
    }
  } else if (state->cursorIndex != 0) {
    state->cursorIndex--;
  } else {
    state->cursorIndex = state->count - 1;
  }
  state->status = NODE_STATUS_SHOWN;
  SelectionScreen_HighlightIcon(state, state->cursorIndex);
  WaitFrames(1);
  SelectionScreen_OpenNameWindow(state->head->kind, 0);
  WaitFrames(1);
}

/// Redraws the command icon at row place rowIndex in its plain state, in its
/// own VRAM cache entry. Other kinds of icon are left alone.
void SelectionScreen_RedrawIcon(struct SelectionScreenWork *state,
                                u32 rowIndex) {
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
/// The small icon is drawn the same as SelectionScreen_RedrawIcon draws it;
/// the highlight is the large icon.
void SelectionScreen_HighlightIcon(struct SelectionScreenWork *state,
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
    SelectionScreen_LoadLargeIcon();
  }
}

/// Stops the battle's highlight of units (BattleHighlight_SetModesFar
/// with mode 0); Menu_ConfirmSelection calls it as an entry is chosen. It
/// passes a unit list with one place an icon, of which only the end marker
/// (0xff, BATTLE_UNIT_LIST_END) is written; mode 0 never reads the list.
void Menu_SendNodeCountList(struct SelectionScreenWork *screen) {
  u16 data[6];
  struct SelectionNode *node = screen->head;
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
void SelectionScreen_Scroll(struct SelectionScreenWork *screen, s32 forward) {
  struct SelectionNode *node;
  struct SelectionNode *last;
  struct SelectionNode *first;
  s32 base;
  s32 kind;
  u32 entryIndex;
  s32 rowX;
  s32 rowY;

  if (forward != 0) {
    entryIndex = screen->top + SELECTION_ROWS;
    base = screen->bases[entryIndex];
    kind = screen->kinds[entryIndex];
    node = SelectionScreen_FindFreeIcon(SELECTION_ICON_ROW);
    if (node == NULL)
      return;
    SelectionScreen_SetupIcon(kind, base, node, 0);
    rowX = screen->baseX;
    node->x = rowX + 80;
    rowY = screen->baseY;
    node->targetX = rowX + 64;
    node->y = rowY;
    node->targetY = rowY;
    node->scaleStep = 32;
    node->scale = 32;
    node->scaleEnd = 256;
    node->dx = -2;
    last = node;
    node = screen->head;
    node->scaleStep = -32;
    node->scaleEnd = 0;
    for (;;) {
      node->targetX = node->x - 16;
      node->dx = -2;
      if (node->next == NULL)
        break;
      node = node->next;
    }
    node->next = last;
    last->next = NULL;
    last->previous = node;
    node = screen->head;
    do {
      WaitFrames(1);
    } while (node->scale != 0);
    screen->head = node->next;
    VramCache_FreeEntry(node->slot);
    node->kind = ICON_KIND_NONE;
    node = node->next;
    node->previous = NULL;
  } else {
    entryIndex = screen->top;
    base = screen->bases[entryIndex];
    kind = screen->kinds[entryIndex];
    node = SelectionScreen_FindFreeIcon(SELECTION_ICON_ROW);
    if (node == NULL)
      return;
    SelectionScreen_SetupIcon(kind, base, node, 0);
    node->x = screen->baseX - 16;
    node->y = screen->baseY;
    node->targetY = node->y;
    node->dx = 2;
    node->scale = 32;
    node->scaleStep = 32;
    node->targetX = node->x + 16;
    node->scaleEnd = 256;
    first = node;
    node = screen->head;
    node->previous = first;
    first->next = node;
    first->previous = NULL;
    screen->head = first;
    node = first;
    for (;;) {
      node->targetX = node->x + 16;
      node->dx = 2;
      if (node->next == NULL)
        break;
      node = node->next;
    }
    node->scaleEnd = 0;
    node->scaleStep = -32;
    node = screen->head;
    do {
      WaitFrames(1);
    } while (node->scale != 256);
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
/// Items show their count above one and their equipped and broken marks.
/// The caller frees the slot.
s32 Resource_LoadByMode(s32 mode, s32 value) {
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
  case ICON_KIND_TABLE_ENTRY:
    result = VramCache_FindFreeEntry();
    if (result == VRAM_CACHE_ENTRY_COUNT)
      return -1;
    Resource_LoadTableEntry(value, 0, result);
    break;
  case ICON_KIND_ABILITY:
    Ability_LoadGlyph(value, 1, &result, &output, 0);
    break;
  }
  return result;
}

/// Loads the icon of an entry of kind mode into sprite tile slot result (a free
/// slot when -1) and returns the slot, or -1 when none was free. An item's icon
/// has its count above one and its equipped, broken and artifact marks,
/// ICON_KIND_EQUIPMENT_ITEM the same without the broken mark. For a
/// command option is the frame drawn under the icon; for Psynergy, whether the
/// icon frame is drawn.
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
        value, ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_ARTIFACT, result);
    break;
  case ICON_KIND_ABILITY:
    Ability_LoadGlyph(value, option, &result, &output, 1);
    break;
  case ICON_KIND_PATTERN:
    Icon_BuildMiscIconTiles(value, 0, result);
    break;
  case ICON_KIND_TABLE_ENTRY:
    Resource_LoadTableEntry(value, 0, result);
    break;
  }

  return result;
}

/// Sets up a selection icon for an entry: its icon tiles (in its old slot when
/// reuse is set), its name message and its sprite.
/// kind must select a command, item or Psynergy with the matching base ID.
/// Items and Psynergy are drawn over the icon frame; the sprite is 16x16 at
/// full size (0x100).
void SelectionScreen_SetupIcon(u32 kind, s32 base, struct SelectionNode *node,
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
  node->scale = 0x100;
  node->scaleEnd = 0x100;

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
/// vanish, and it joins the chosen icons at the top left. Returns the entry's
/// index.
/// The selected row node and a free chosen-icon record must exist before
/// confirming. The row is emptied (head NULL) and the hand, arrows and large
/// icon put away, so the caller lists the next row; bit 7 is set in
/// cursorIndex, which restarts that list at its first entry. The place the
/// choice was made from is kept in pathTop and pathCursor.
u32 Menu_ConfirmSelection(struct SelectionScreenWork *screen) {
  struct SelectionNode *node;
  struct SelectionNode *other = NULL;
  u32 nodeIndex = 0;
  u32 chosenIndex;
  struct SelectionSprite *sprite;

  chosenIndex = screen->top + screen->cursorIndex;
  Menu_SendNodeCountList(screen);
  SelectionScreen_RedrawIcon(screen, screen->cursorIndex);
  screen->status = NODE_STATUS_SCROLLING;
  WaitFrames(1);
  screen->records[0].kind = 0;
  screen->records[1].kind = 0;
  screen->records[14].kind = 0;
  screen->records[14].scale = 0;
  Resource_ResetPendingTransfer();
  node = screen->head;
  while (node != NULL && screen->cursorIndex != nodeIndex) {
    node = node->next;
    nodeIndex++;
  }
  node->homeX = node->x;
  node->homeY = node->y;
  // The others slide halfway in each of two frames, then vanish.
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      other->targetX = node->x;
      other->dx = (node->x - other->x) >> 1;
    }
  }
  WaitFrames(2);
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      VramCache_FreeEntry(other->slot);
      other->kind = ICON_KIND_NONE;
    }
  }
  screen->head = node;
  node->previous = NULL;
  node->next = NULL;
  // The chosen icons line up from x 4, 16 pixels apart; the new one goes
  // after the last, reaching its place over two frames, and its record moves
  // to the chosen list.
  node->targetX = 4;
  for (other = screen->path, nodeIndex = 0; other != NULL;
       other = other->next) {
    node->targetX += 16;
    nodeIndex++;
  }
  screen->pathTop[nodeIndex] = screen->top;
  screen->pathCursor[nodeIndex] = screen->cursorIndex;
  node->dx = (node->targetX - node->x) >> 1;
  screen->unread = 0;
  screen->cursorIndex |= 0x80;
  WaitFrames(2);
  other = SelectionScreen_FindFreeIcon(SELECTION_ICON_CHOSEN);
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
  return chosenIndex;
}

/// Six empty functions; nothing calls the first, the fourth or the fifth. The
/// others are reached only through bare entries of the window far-call table.
/// With no body or caller to give them a role, they are numbered in ROM order.
UNUSED void MenuArrow_NoOpCallback(void) {}
void MenuArrow_NoOpCallbackBank(void) {}
void MenuArrow_NoOpCallback1(void) {}
UNUSED void MenuArrow_NoOpCallback2(void) {}
UNUSED void MenuArrow_NoOpCallback3(void) {}
void MenuArrow_NoOpCallback4(void) {}

/// Sets up a sprite as the right-pointing arrow, in a free tile slot (returned
/// in *slot).
/// The caller owns the cache slot and releases it through
/// MenuArrow_Release.
void MenuArrow_Create(struct MenuSprite *sprite, s32 *slot) {
  const void *data = gMenu_CursorObjectTiles;
  s32 cacheSlot = VramCache_FindFreeEntry();

  *slot = cacheSlot;
  sprite->oam.f.tile = VramCache_Load(cacheSlot, 0x80, data);
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
/// this frame, at depth slot 252; the caller calls it every frame it shows.
void MenuArrow_SetPosition(struct MenuSprite *sprite, s32 x, s32 y) {
  sprite->oam.f.x = x;
  sprite->oam.f.y = y;
  Oam_AddSpriteAtDepth(sprite, 0xFC);
}

/// Frees an arrow sprite's tile slot.
void MenuArrow_Release(s32 value) { VramCache_FreeEntry(value); }

/// Loads the large icon of the command under the cursor, zooming in: 32x32 in
/// 256 colours (0x400 bytes, record 15), its scale stepping from 40 to 240 by
/// 40 a frame, after which SelectionScreen_Draw shows it at its own size.
/// Other kinds of entry show none. The icon keeps its VRAM cache entry until
/// Resource_ResetPendingTransfer.
void SelectionScreen_LoadLargeIcon(void) {
  struct SelectionScreenWork *state = gSelectionScreenWork;
  struct SelectionNode *selected = SelectionScreen_GetCursorNode(state);
  struct SelectionNode *transfer;
  GlyphTransfer *buffer;
  u8 *table;
  void *resource;
  s32 resourceId;

  if (selected->kind != ICON_KIND_COMMAND && selected->kind != ICON_KIND_YES_NO)
    return;

  buffer = AllocHeapSlot(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  transfer = &state->records[15];
  resourceId = selected->base;
  // The resource starts with a halfword offset per command icon.
  table = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  {
    u8 **tiles = &buffer->encoded;
    resource = table + *(u16 *)(table + selected->base * 2);
    *tiles = resource;
  }
  Decompress_ByteLz(resource, buffer);

  if (transfer->kind == ICON_KIND_NONE)
    transfer->slot = VramCache_FindFreeEntry();
  transfer->tile = VramCache_Load(transfer->slot, 0x400, buffer);
  transfer->kind = ICON_KIND_COMMAND;
  transfer->base = resourceId;
  transfer->scale = 40;
  transfer->scaleStep = 40;
  transfer->scaleEnd = 240;
  FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);
}

/// Frees the large command icon, if it is shown.
/// The large preview owns a cache slot independently of the visible row icons.
void Resource_ResetPendingTransfer(void) {
  struct SelectionNode *largeIcon = &gSelectionScreenWork->records[15];

  if (largeIcon->kind != ICON_KIND_NONE) {
    VramCache_FreeEntry(largeIcon->slot);
    largeIcon->kind = ICON_KIND_NONE;
  }
}

#if TBS_PATCH_2

#define HAS_LOCALIZED_MENU_GUARD 1
#endif

/// Runs the field menu until the player picks something that leaves it, and
/// returns the chosen screen's result. Cancelling the menu returns an
/// undefined value, the missing result of PartyStatus_CloseWithMenu (0 under
/// UBFIX); its one caller, Map_Run, ignores the result. Its case for
/// command 0, which stores the trigger tile ahead in the event work's
/// searchCell, is unreachable.
s32 FieldMenu_Run(void) {
  s32 ret;
  s32 selection;
  u8 *state;

  state = (u8 *)gFieldWork;
  selection = 0;

  for (;;) {
    PartyStatus_OpenAndWait();
    selection = FieldMenu_SelectCommand(selection);
    // German, Spanish, French and Italian: mark a menu open (event work byte
    // 0xcca, which pauses Reveal's sparkles) and, while Reveal is active
    // (FieldWork.revealActive at 0xcb8), clear its sparkles.
#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 1;
    if (*(s16 *)(state + 0xcb8) != 0) {
      FieldEffect_ClearRandomParticlesFar();
      WaitFrames(1);
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
    // opens the field menu again. Case 0 is unreachable:
    // FieldMenu_SelectCommand returns a FieldMenuCommand or -1; it would store
    // the trigger tile ahead of the leader (0xff for none) in searchCell.
    switch (selection) {
    case 0:
      ret = ObjectEvent_GetTriggerTileAheadOfCurrentFar();
      if (ret == 0)
        ret = 0xff;
      ((struct FieldWork *)state)->searchCell = ret;
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
      ret = SummaryScreen_RunFar();
      if (ret == -1)
        continue;
      break;
    default:
      break;
    }
    break;
  }

#if defined(HAS_LOCALIZED_MENU_GUARD)
  state[0xcca] = 0;
#endif
  return ret;
}

/// Opens the party status window that shows with the field menu (flags 1)
/// and waits a frame.
void PartyStatus_OpenAndWait(void) {
  PartyStatus_CreateWindow(1);
  WaitFrames(1);
}

/// Closes the party status window, kept in heap block 16 (gPartyStatusWork),
/// and frees the block.
void PartyStatus_CloseWithMenu(void) { PartyStatus_Close(); }

/// Sets up an empty selection screen and waits a frame.
/// Nothing calls it.
UNUSED void SelectionScreen_CreateEmpty(void) {
  SelectionScreen_Create();
  SelectionScreen_Clear();
  WaitFrames(1);
}

/// Runs a selection row with the cursor on entry initialIndex, cancellable,
/// and returns the entry chosen or -1.
/// initialIndex is a row index; cancellation returns -1 encoded as u32.
/// The caller sets up the screen and lists its entries first; this closes the
/// screen and frees it before returning.
u32 Menu_RunSelectionForValue(u32 initialIndex) {
  struct SelectionScreenWork *screen = gSelectionScreenWork;
  u32 result;

  // locked 1: a yes or no row stays at the bottom. The name window is 5
  // tiles wide inside its frame.
  screen->cursorIndex = initialIndex;
  screen->locked = 1;
  SelectionScreen_BuildEntries(initialIndex);
  Menu_SetupSelectionBothSides();
  SelectionScreen_OpenNameWindow(0, 5);
  SelectionScreen_StartDrawing();
  result = SelectionScreen_RunWithSounds(1);
  SelectionScreen_Close();
  return result;
}

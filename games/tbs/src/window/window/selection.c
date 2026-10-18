// Part of window.c, the window object, compiled only through it.
// The selection screen's row of icons, and the field menu.

#include "../window_internal.h"

struct GlyphCursor {
  u16 unused;
  u16 cursor;
};

// Clears a selection record's cursor.
static __inline__ void ResetCursor(struct GlyphCursor *cursor) {
  /* FAKEMATCH: the helper takes the node's base field as its record, so each
   * row store forms base + 372 and stores the cursor at #2, as the native does.
   * A plain work->records[7].kind = 0 folds the offset into 374 and stores at
   * #0 (differ 6). The typed SelectionNode helper also changes these stores. */
  cursor->cursor = 0;
}

// Allocates and resets the selection screen: the row of icons the player picks
// from, with arrows at each end and a pointing hand over the chosen target.
void UiGlyph_ResetWorkState(void) {
  struct SelectionScreen *work;
  struct SelectionSprite *visual;
  const u8 *table;
  s32 i;
  u16 clear = 0;

  work = Runtime_AllocateBlock(18, sizeof(struct SelectionScreen));
  work->head = NULL;
  work->path = NULL;
  work->window = NULL;
  work->unk39a = clear;
  work->top = clear;
  work->cursorIndex = 128;
  work->verticalScroll = 32;
  work->count = clear;
  work->locked = 999;
  i = 0;
  do {
    work->records[i + 2].kind = 0;
    work->records[i + 9].kind = 0;
    i++;
  } while (i != 5);
  ResetCursor((struct GlyphCursor *)&work->records[7].base);
  ResetCursor((struct GlyphCursor *)&work->records[8].base);
  work->records[0].kind = 0;
  work->records[1].kind = 0;
  work->records[0].y = 0;
  work->records[1].y = 0;
  table = gMenu_AnimatedCursorTiles;
  work->records[14].slot = Resource_FindFreeEntry();
  work->records[14].tile = VramCache_Load(work->records[14].slot, 256, table);
  work->records[14].kind = 0;
  work->records[14].scale = 0;
  work->records[15].kind = 0;
  visual = &work->records[14].sprite;
  visual->mode = 0;
  visual->mosaic = 0;
  visual->colors = 1;
  visual->affine = 0;
  visual->param = 0;
  visual->size = 1;
  visual->shape = 0;
  visual->priority = 0;
}

// Empties the selection screen's list of entries.
// The high cursor bit requests resetting both the list origin and cursor.
void Resource_ClearOwnerListAndCounters(void) {
  struct SelectionScreen *screen;

  screen = gResQueueWork;
  screen->head = NULL;
  screen->unk39a = 0;
  if (screen->cursorIndex & 0x80) {
    screen->top = 0;
    screen->cursorIndex = 0;
  }
  screen->verticalScroll = 0;
  screen->count = 0;
}

// Adds an entry (kind and id) to the selection screen's list, up to 16.
// The constructor and clear routine establish the count's 0-to-16 invariant.
void Resource_PushPendingPair(u32 kind, u32 base) {
  struct SelectionScreen *state = gResQueueWork;
  u16 entryCount = state->count;

  if (entryCount != UI_SELECTION_ENTRY_LIMIT) {
    state->kinds[entryCount] = kind;
    state->bases[entryCount] = base;
    state->count++;
  }
}

// Creates icons for the first five listed entries in a row along the bottom of
// the screen.
// Build the linked row before placing the visible icons and loading the
// preview.
void MenuSelection_BuildEntries(u32 unused) {
  struct SelectionScreen *screen = gResQueueWork;
  u32 count = screen->count;
  u32 entryIndex = screen->top;
  struct SelectionNode *previous = 0;
  struct SelectionNode *node;
  s32 rowPosition = 0;

  while (entryIndex < count) {
    s32 base = screen->bases[entryIndex];
    u32 kind = screen->kinds[entryIndex];

    node = SelectionScreen_FindFreeIcon(0);
    if (node == 0)
      break;
    SelectionScreen_SetupIcon(kind, base, node, 0);
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
    entryIndex++;
  }

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
    if (previous->kind == 6 && screen->locked == 0) {
      previous->y = 6;
      previous->targetY = 6;
    }
    previous->dx = 0;
    previous->dy = 0;
    rowPosition += 16;
  }
  SelectionScreen_LoadLargeIcon();
}

// Does nothing.
void UiWork_ReservedNoOp(void) {}

// Returns a free selection icon record: one of the seven row icons (kind 0) or
// of the five chosen icons (otherwise), or NULL.
struct SelectionNode *SelectionScreen_FindFreeIcon(s32 kind) {
  struct SelectionScreen *state = gResQueueWork;
  s32 i;

  if (kind != 0) {
    for (i = 0; i != 5; i++) {
      if (state->records[i + 9].kind == 0)
        return &state->records[i + 9];
    }
  } else {
    for (i = 0; i != 7; i++) {
      if (state->records[i + 2].kind == 0)
        return &state->records[i + 2];
    }
  }
  return NULL;
}

// Starts drawing the selection screen each frame.
void Resource_ScheduleOwnerResetDelayed(void) {
  Scheduler_AddOrUpdateCallback(SelectionScreen_Draw, 0xC80);
}

// Stops drawing the selection screen.
void Resource_ScheduleOwnerReset(void) {
  Scheduler_RemoveCallback(SelectionScreen_Draw);
}

// The selection screen's per-frame callback: slides and draws the row of
// icons, the large icon over the chosen one, the end arrows, the icons already
// chosen and the pointing hand.
void SelectionScreen_Draw(void) {
  struct SelectionScreen *state = gResQueueWork;
  struct SelectionSprite *cursorEntry = &state->records[14].sprite;
  struct SelectionNode *cursor = &state->records[14];
  struct SelectionNode *transfer = &state->records[15];
  struct SelectionNode *node;
  struct SelectionSprite *sprite;
  struct SelectionSprite *commandSprite;
  s32 entryIndex = 0;
  s32 slot;
  s32 axisStep;
  struct EffectPosition pos;

  node = state->head;
  // Move and queue the visible row before drawing its overlays.
  while (node != 0) {
    sprite = &node->sprite;
    sprite->affine = 0;
    sprite->param = 0;
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
      if (cursor->kind != 0) {
        if (BattleMotion_ProjectScaledPositionFar(
                node->base, EffectPosition_Coordinates(&pos)) != -1) {
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
      if (GameFlag_TestFar(0x103) != 0) {
        if (state->records[14].kind == 1) {
          sprite->mode = 1;
        } else {
          sprite->mode = 0;
        }
        if (node->kind == 1) {
          sprite->mode = 1;
        }
      }
      Runtime_PushSlotEntry(sprite, slot);
    }
    node = node->next;
    entryIndex++;
  }
  // The selected command's large icon owns a separate cached tile block.
  if (transfer->kind != 0) {
    node = Selection_GetCursorNode(state);
    commandSprite = &transfer->sprite;
    commandSprite->mode = 0;
    commandSprite->affine = 0;
    commandSprite->param = 0;
    commandSprite->mosaic = 0;
    commandSprite->colors = 1;
    commandSprite->size = 2;
    commandSprite->shape = 0;
    commandSprite->priority = 0;
    commandSprite->tile = transfer->tile;
    commandSprite->x = node->x - 4;
    commandSprite->y =
        node->y + (gMenu_CursorBobOffsets[(gFrameTick >> 1) & 15] >> 1) - 4;
    if (transfer->scale != transfer->scaleEnd) {
      state->transform.scaleX = transfer->scale;
      state->transform.scaleY = transfer->scale;
      state->transform.angle = 0;
      commandSprite->param = AffineMatrix_BuildForEffect(&state->transform);
      commandSprite->affine = 3;
      commandSprite->x += 0xfff0;
      commandSprite->y += 0xf0;
      transfer->scale += transfer->scaleStep;
    }
    if (GameFlag_TestFar(0x103) != 0) {
      commandSprite->mode = 1;
    }
    Runtime_PushSlotEntry(commandSprite, 248);
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
      sprite->param = AffineMatrix_BuildForEffect(&state->transform);
      sprite->affine = 3;
      sprite->x += 0xfff8;
      sprite->y += 0xf8;
    } else {
      sprite->affine = 0;
      sprite->param = 0;
    }
    if (GameFlag_TestFar(0x103) != 0) {
      if (state->records[14].kind == 1) {
        sprite->mode = 1;
      } else {
        sprite->mode = 0;
      }
      if (node->kind == 1) {
        sprite->mode = 1;
      }
    }
    Runtime_PushSlotEntry(sprite, 240);
    node = node->next;
  }
  // Point at the selected battle target, smoothing each axis separately.
  if (cursor->kind != 0) {
    entryIndex = (s32)gMenu_AnimatedCursorTiles;
    {
      void *src = (void *)(entryIndex + (((gFrameTick >> 2) & 15) << 8));

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
        cursor->y + gMenu_CursorBobOffsets[(gFrameTick >> 2) & 15] - 32;
    cursorEntry->x = cursor->x - 4;
    if (GameFlag_TestFar(0x103) != 0) {
      if (state->records[14].kind == 1) {
        cursorEntry->mode = 1;
      } else {
        cursorEntry->mode = 0;
      }
    }
    Runtime_PushSlotEntry(cursorEntry, 248);
  }
  state->status += 1;
}

// Draws the selection screen's left (index 0) or right (1) arrow, nudged
// outwards while it flashes after a scroll.
void SelectionScreen_DrawArrow(struct SelectionScreen *state, s32 side) {
  u32 animationStep = (gFrameTick >> 2) & 7;
  struct SelectionSprite *arrow;
  const u8 *arrowTiles;
  const u8 *tmp;

  if (state->records[side].kind == 0)
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
  if (GameFlag_TestFar(0x103)) {
    if (state->records[14].kind == 1)
      arrow->mode = 1;
    else
      arrow->mode = 0;
  }
  Runtime_PushSlotEntry(arrow, 238);
  if (state->records[side].base != 0)
    state->records[side].base--;
}

// Opens or refits the name window above the selection row and writes the name
// of the icon under the cursor, or that nothing is known (mode 4, Psynergy)
// or held (mode 2, items). mode 6 is a yes or no question, placed at the
// bottom or top of the screen.
void Menu_OpenSelectionWindow(s32 mode, u32 count) {
  struct SelectionScreen *screen;
  struct SelectionNode *node;
  struct UiWindow **slot;
  struct UiWindow *window;

  screen = gResQueueWork;
  node = Selection_GetCursorNode(screen);
  slot = &screen->window;
  window = *slot;
  if (window == 0) {
    if (mode == 6) {
      if (screen->locked != 0) {
        *slot = UiWindow_Create(17, 17, 5, 3, mode);
      } else {
        *slot = UiWindow_Create(17, 0, 5, 3, mode);
      }
      screen->verticalScroll = 0;
      screen->locked = 999;
    } else {
      *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
    }
    RenderOutput_PrepareForRedraw(screen->window);
  } else {
    if (count != 0 && window->width != count + 2) {
      UiWork_Finalize(window, 2);
      *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
    }
    RenderOutput_PrepareForRedraw(screen->window);
  }
  if (screen->count != 0) {
    UiText_DrawCharacterAtOffset(node->message, screen->window, 0, 0);
  } else {
    switch (mode) {
    case 4:
      UiText_DrawCharacterAtOffset(MsgAbilityNotKnown, screen->window, 0, 0);
      break;
    case 2:
      UiText_DrawCharacterAtOffset(MsgItemNotHeld, screen->window, 0, 0);
      break;
    }
  }
}

// Closes the selection screen and frees its icons, tiles and work block.
void Resource_ResetOwnerEntries(void) {
  struct SelectionScreen *state = gResQueueWork;
  struct SelectionNode *node;

  Resource_ScheduleOwnerReset();
  UiWork_Finalize(state->window, 2);
  WaitFrames(1);
  node = state->head;
  while (node != 0) {
    if (node->kind != 0) {
      Resource_ResetEntry(node->slot);
      node->kind = 0;
    }
    node = node->next;
  }
  node = state->path;
  while (node != 0) {
    if (node->kind != 0) {
      Resource_ResetEntry(node->slot);
      node->kind = 0;
    }
    node = node->next;
  }
  Resource_ResetPendingTransfer();
  if (state->records[0].y != 0) {
    Resource_ResetEntry(state->records[0].slot);
    if (state->records[0].y != 0) {
      Resource_ResetEntry(state->records[1].slot);
    }
  }
  Resource_ResetEntry(state->records[14].slot);
  Runtime_ReleaseHeapBlock(18);
}

// Moves the row of icons to start at pixel (x, y), 16 pixels apart.
void Menu_SetNodeCoordinates(u32 x, u32 y) {
  struct SelectionScreen *screen = gResQueueWork;
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

// Sets up both end arrows of the selection row.
void Menu_SetupSelectionBothSides(void) {
  struct SelectionScreen *state;

  state = gResQueueWork;
  SelectionScreen_SetupArrow(state, 0);
  SelectionScreen_SetupArrow(state, 1);
}

// Sets up the left (index 0) or right (1) arrow of the selection row, shown
// when there are entries beyond that end.
void SelectionScreen_SetupArrow(struct SelectionScreen *state, s32 side) {
  struct SelectionSprite *arrow = &state->records[side].sprite;
  const u8 *arrowTiles = 0;
  u32 count;

  state->records[side].kind = 0;
  if (side != 0) {
    /* FAKEMATCH: the separate local loads the tile pointer early into r5 and
     * makes its copy late, as the native does. Assigning
     * gMenu_CursorObjectTiles directly where right is read: differ 42; the same
     * at the top of the branch: differ 16 (tbs-en). */
    const u8 *right = gMenu_CursorObjectTiles;

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
    arrowTiles = gMenu_CursorLeftObjectTiles;
    state->records[0].x = state->baseX - 9;
    if (state->top != 0)
      state->records[0].kind = 1;
  }
  if (state->records[side].y == 0) {
    state->records[side].slot = Resource_FindFreeEntry();
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

// Returns the node at the selection screen's cursor index.
// cursorIndex is relative to head, not the number of queued entries.
struct SelectionNode *Selection_GetCursorNode(struct SelectionScreen *screen) {
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

// Runs the selection row until the player chooses (the chosen entry's index)
// or, when mode allows it, cancels with B (-1). SELECTION_FIXED allows no
// moving or choosing.
s32 Menu_SelectionLoop(s32 mode) {
  struct SelectionScreen *screen = gResQueueWork;

  SelectionScreen_HighlightIcon(screen, 0);
  for (;;) {
    WaitFrames(1);
    if (screen->verticalScroll != 0) {
      continue;
    }
    if (mode != SELECTION_FIXED) {
      if (gKeysRepeat & KEY_RIGHT) {
        Menu_StepRight(screen);
      } else if (gKeysRepeat & KEY_LEFT) {
        Menu_StepLeft(screen);
      } else if (gKeysPressed & KEY_A) {
        return Menu_ConfirmSelection(screen);
      }
    }
    if (mode != 0 && (gKeysPressed & KEY_B)) {
      return -1;
    }
  }
}

// As Menu_SelectionLoop, with the menu sounds.
u32 Menu_WaitForSelectionInput(u32 mode) {
  struct SelectionScreen *screen = gResQueueWork;
  u32 result;

  for (;;) {
    WaitFrames(1);
    if (screen->verticalScroll != 0) {
      continue;
    }

    if (mode != SELECTION_FIXED) {
      if (gKeysRepeat & KEY_RIGHT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        Menu_StepRight(screen);
      } else if (gKeysRepeat & KEY_LEFT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        Menu_StepLeft(screen);
      }

      if (gKeysPressed & KEY_A) {
        result = screen->top + screen->cursorIndex;
        if (screen->head->kind == NODE_KIND_YES_NO) {
          if (result == 0)
            AudioCommand_PlayFar(SE_MENU_CONFIRM);
          else
            AudioCommand_PlayFar(SE_MENU_CANCEL);
        } else {
          AudioCommand_PlayFar(SE_MENU_CONFIRM);
        }
        return result;
      }
    }

    if (mode != 0 && (gKeysPressed & KEY_B)) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      return -1;
    }
  }
}

// Moves the cursor one entry right, scrolling the row at its fourth place.
// This variant stops at the end; Menu_StepRight also wraps around.
void Menu_MoveSelectionForward(struct SelectionScreen *screen) {
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
    Menu_ScrollSelectionList(screen, TRUE);
    if (screen->top + screen->cursorIndex + 2 == screen->count) {
      screen->records[1].kind = FALSE;
    }
    screen->records[0].kind = TRUE;
  }
  screen->status = NODE_STATUS_SHOWN;
  SelectionScreen_HighlightIcon(screen, screen->cursorIndex);
  WaitFrames(1);
  Menu_OpenSelectionWindow(screen->head->kind, 0);
  WaitFrames(1);
}

// Moves the cursor one entry left, scrolling the row at its second place.
// This variant stops at the beginning; Menu_StepLeft also wraps around.
void Menu_MoveSelectionBackward(struct SelectionScreen *screen) {
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
    Menu_ScrollSelectionList(screen, FALSE);
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
  Menu_OpenSelectionWindow(screen->head->kind, 0);
  WaitFrames(1);
}

// Moves the cursor one entry right, scrolling the row at its edge and
// wrapping from the last entry to the first.
void Menu_StepRight(struct SelectionScreen *state) {
  struct SelectionNode *node;
  u32 entryIndex;
  s32 y;
  u32 end;

  SelectionScreen_RedrawIcon(state, state->cursorIndex);
  state->status = NODE_STATUS_SCROLLING;
  WaitFrames(1);
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
      Menu_ScrollSelectionList(state, 1);
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
  Menu_OpenSelectionWindow(state->head->kind, 0);
  WaitFrames(1);
}

// Moves the cursor one entry left, scrolling the row at its edge and
// wrapping from the first entry to the last.
void Menu_StepLeft(struct SelectionScreen *state) {
  struct SelectionNode *node;
  s32 i;

  SelectionScreen_RedrawIcon(state, state->cursorIndex);
  state->status = NODE_STATUS_SCROLLING;
  WaitFrames(1);
  if (state->count > 5) {
    if ((state->cursorIndex | state->top) != 0) {
      if (state->cursorIndex == 1 && state->top != 0) {
        state->records[0].base = 8;
        state->top--;
        Menu_ScrollSelectionList(state, 0);
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
        /* FAKEMATCH: the zero goes through r0. 2026-10-11: CSE follows the beq
         * into this block and reuses the known-zero OR register for a plain
         * store (differ 28); only a loop-end note before the else label stops
         * it, and a fresh zero then takes r3 (differ 4); r0 needs the zero live
         * across the first loop, which no instruction-free second use gives (a
         * second store after the loop: differ 13). */
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
  Menu_OpenSelectionWindow(state->head->kind, 0);
  WaitFrames(1);
}

// Redraws the command icon at row place index in its plain state.
void SelectionScreen_RedrawIcon(struct SelectionScreen *state, u32 rowIndex) {
  struct SelectionNode *node = state->head;
  u32 tile;
  u32 value;

  while (rowIndex != 0) {
    rowIndex--;
    node = node->next;
  }
  if (node->kind == 1 || node->kind == 6) {
    u32 first = node->message - MsgCommandName;

    value = node->slot;
    Ui_BuildPairedPatternsToSlot(first, 0, &value, &tile, 1);
  }
}

// Redraws the command icon at row place index and loads its large icon.
void SelectionScreen_HighlightIcon(struct SelectionScreen *state,
                                   u32 rowIndex) {
  struct SelectionNode *node = state->head;
  u32 tile;
  u32 value;

  while (rowIndex != 0) {
    rowIndex--;
    node = node->next;
  }
  if (node->kind == 1 || node->kind == 6) {
    u32 resource = node->message - MsgCommandName;

    value = node->slot;
    Ui_BuildPairedPatternsToSlot(resource, 0, &value, &tile, 1);
    SelectionScreen_LoadLargeIcon();
  }
}

// Tells the battle presentation how many icons the row shows.
void Menu_SendNodeCountList(struct SelectionScreen *screen) {
  u16 data[6];
  struct SelectionNode *node = screen->head;
  s32 count = 0;

  while (node != 0) {
    node = node->next;
    count++;
  }
  data[count] = 0xff;
  BattlePres_SetActorModesFar(data, 0);
}

// Scrolls the row one entry: forward brings an icon in from the right as the
// leftmost shrinks away, and back the reverse; waits until it has moved.
// Release the departing cache slot only after its shrinking animation finishes.
void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward) {
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
    node = SelectionScreen_FindFreeIcon(0);
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
    last->prev = node;
    node = screen->head;
    do {
      WaitFrames(1);
    } while (node->scale != 0);
    screen->head = node->next;
    Resource_ResetEntry(node->slot);
    node->kind = 0;
    node = node->next;
    node->prev = NULL;
  } else {
    entryIndex = screen->top;
    base = screen->bases[entryIndex];
    kind = screen->kinds[entryIndex];
    node = SelectionScreen_FindFreeIcon(0);
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
    node->prev = first;
    first->next = node;
    first->prev = NULL;
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
    Resource_ResetEntry(node->slot);
    node->kind = 0;
    node->prev->next = NULL;
  }
}

// Loads the icon of an entry of kind mode (1 and 6 commands, 2 items, 4
// Psynergy, 9 fixed images) into a free sprite tile slot and returns it, or
// -1.
s32 Resource_LoadByMode(s32 mode, s32 value) {
  s32 output;
  s32 result = -1;

  switch (mode) {
  case 1:
  case 6:
    Ui_BuildPairedPatternsToSlot(value, 0, &result, &output, 0);
    break;
  case 2:
    result = Resource_FindFreeEntry();
    if (result == VRAM_CACHE_ENTRY_COUNT)
      return -1;
    UiIcon_CopyResourceToSlot(value, 0x1a, result);
    break;
  case 9:
    result = Resource_FindFreeEntry();
    if (result == VRAM_CACHE_ENTRY_COUNT)
      return -1;
    RenderResource_LoadTableEntry(value, 0, result);
    break;
  case 4:
    Ability_LoadGlyph(value, 1, &result, &output, 0);
    break;
  }
  return result;
}

// Loads the icon of an entry of kind mode into sprite tile slot result (a free
// slot when -1) and returns the slot. Kind 7 is an item icon without its
// count, 8 one of the other icons.
s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option) {
  s32 output;
  s32 original = result;

  if (result == -1) {
    result = Resource_FindFreeEntry();
    if (result == VRAM_CACHE_ENTRY_COUNT)
      return original;
  }

  switch (mode) {
  case 1:
  case 6:
    Ui_BuildPairedPatternsToSlot(value, option, &result, &output, 1);
    break;
  case 2:
    UiIcon_CopyResourceToSlot(value, 58, result);
    break;
  case 7:
    UiIcon_CopyResourceToSlot(value, 42, result);
    break;
  case 4:
    Ability_LoadGlyph(value, option, &result, &output, 1);
    break;
  case 8:
    Ui_BuildPatternToSlot(value, 0, result);
    break;
  case 9:
    RenderResource_LoadTableEntry(value, 0, result);
    break;
  }

  return result;
}

// Sets up a selection icon for an entry: its icon tiles (in its old slot when
// reuse is set), its name message and its sprite.
// kind must select a command, item or Psynergy with the matching base ID.
void SelectionScreen_SetupIcon(u32 kind, s32 base, struct SelectionNode *node,
                               s32 reuse) {
  s32 cacheSlot;
  s32 tile;
  struct SelectionSprite *sprite;

  switch (kind) {
  case 1:
  case 6:
    if (reuse != 0)
      cacheSlot = node->slot;
    Ui_BuildPairedPatternsToSlot(base, 0, &cacheSlot, &tile, reuse);
    node->message = base + MsgCommandName;
    break;
  case 2:
    if (reuse != 0)
      cacheSlot = node->slot;
    UiIcon_BuildItemIconTiles(base, 1, &cacheSlot, &tile, reuse);
    node->message = base + MsgItemName;
    break;
  case 4:
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

// Chooses the entry under the cursor: the other icons gather into it and
// vanish, and it joins the chosen icons at the top left. Returns the entry's
// index.
// The selected row node and a free chosen-icon record must exist before
// confirming.
u32 Menu_ConfirmSelection(struct SelectionScreen *screen) {
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
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      other->targetX = node->x;
      other->dx = (node->x - other->x) >> 1;
    }
  }
  WaitFrames(2);
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      Resource_ResetEntry(other->slot);
      other->kind = 0;
    }
  }
  screen->head = node;
  node->prev = NULL;
  node->next = NULL;
  node->targetX = 4;
  for (other = screen->path, nodeIndex = 0; other != NULL;
       other = other->next) {
    node->targetX += 16;
    nodeIndex++;
  }
  screen->pathTop[nodeIndex] = screen->top;
  screen->pathCursor[nodeIndex] = screen->cursorIndex;
  node->dx = (node->targetX - node->x) >> 1;
  screen->unk39a = 0;
  screen->cursorIndex |= 0x80;
  WaitFrames(2);
  other = SelectionScreen_FindFreeIcon(1);
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
  return chosenIndex;
}

// Six empty functions.
void UiTextResource_NoOpCallback(void) {}
void UiTextResource_NoOpCallbackBank(void) {}
void UiTextResource_NoOpCallback1(void) {}
void UiTextResource_NoOpCallback2(void) {}
void UiTextResource_NoOpCallback3(void) {}
void UiTextResource_NoOpCallback4(void) {}

// Sets up a sprite as the right-pointing arrow, in a free tile slot (returned
// in *slot).
// The caller owns the cache slot and releases it through
// UiTextResource_Release.
void UiTextResource_Initialize(struct MenuSprite *sprite, s32 *slot) {
  const void *data = gMenu_CursorObjectTiles;
  s32 cacheSlot = Resource_FindFreeEntry();

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

// Places an arrow sprite at (x, y) and queues it for drawing.
void UiTextResource_SetPosition(struct MenuSprite *sprite, s32 x, s32 y) {
  sprite->oam.f.x = x;
  sprite->oam.f.y = y;
  Runtime_PushSlotEntry(sprite, 0xFC);
}

// Frees an arrow sprite's tile slot.
void UiTextResource_Release(s32 value) { Resource_ResetEntry(value); }

// Loads the large icon of the command under the cursor, zooming in.
void SelectionScreen_LoadLargeIcon(void) {
  struct SelectionScreen *state = gResQueueWork;
  struct SelectionNode *selected = Selection_GetCursorNode(state);
  struct SelectionNode *transfer;
  GlyphTransfer *buffer;
  u8 *table;
  void *resource;
  s32 resourceId;

  if (selected->kind != 1 && selected->kind != 6)
    return;

  buffer = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  transfer = &state->records[15];
  resourceId = selected->base;
  table = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  {
    u8 **tiles = &buffer->encoded;
    resource = table + *(u16 *)(table + selected->base * 2);
    *tiles = resource;
  }
  Resource_DecodeByteLz(resource, buffer);

  if (transfer->kind == 0)
    transfer->slot = Resource_FindFreeEntry();
  transfer->tile = VramCache_Load(transfer->slot, 0x400, buffer);
  transfer->kind = 1;
  transfer->base = resourceId;
  transfer->scale = 40;
  transfer->scaleStep = 40;
  transfer->scaleEnd = 240;
  Runtime_ReleaseHeapBlock(17);
}

// Frees the large command icon, if it is shown.
// The large preview owns a cache slot independently of the visible row icons.
void Resource_ResetPendingTransfer(void) {
  struct SelectionNode *largeIcon = &gResQueueWork->records[15];

  if (largeIcon->kind != 0) {
    Resource_ResetEntry(largeIcon->slot);
    largeIcon->kind = 0;
  }
}

#if TBS_PATCH_2

#define HAS_LOCALIZED_MENU_GUARD 1
#endif

// Runs the field menu until the player picks something that leaves it, and
// returns that choice's result.
s32 Menu_RunTopSelection(void) {
  s32 ret;
  s32 selection;
  u8 *state;

  state = (u8 *)gEventWork;
  selection = 0;

  for (;;) {
    UiWindow_OpenMode1AndWaitFrame();
    selection = Menu_SelectTopEntry(selection);
#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 1;
    if (*(s16 *)(state + 0xcb8) != 0) {
      BattleFx_ClearRandomParticlesFar();
      WaitFrames(1);
    }
#endif
#ifdef UBFIX
    // UB: the original reads a void return. The default result is ignored by
    // the only caller, so give it a defined value after closing the window.
    UiWork_CloseAndRelease();
    ret = 0;
#else
    ret = UiWork_CloseAndRelease();
#endif

    switch (selection) {
    case 0:
      ret = Object_GetTriggerTileAheadOfCurrentFar();
      if (ret == 0)
        ret = 0xff;
      ((struct EventWork *)state)->facingTrigger = ret;
      break;
    case 1:
      ret = Menu_OpenConfirmPromptFar();
      if (ret == -1)
        continue;
      break;
    case 2:
      ret = ItemMenu_RunFar();
      if (ret != 0)
        continue;
      break;
    case 3:
      ret = RunAssetSelectionScreenFar();
      if (ret == -1)
        continue;
      break;
    case 4:
      ret = Menu_OpenActionFlow();
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

// Opens the field menu's window and waits a frame.
void UiWindow_OpenMode1AndWaitFrame(void) {
  UiWindow_CreateWithLayoutBounds(1);
  WaitFrames(1);
}

// Closes the window kept in heap block 16 and frees the block.
void UiWork_CloseAndRelease(void) { UiWork_FinalizeAndReleaseBlock16(); }

// Sets up an empty selection screen and waits a frame.
void SelectionScreen_CreateEmpty(void) {
  UiGlyph_ResetWorkState();
  Resource_ClearOwnerListAndCounters();
  WaitFrames(1);
}

// Runs a selection row with the cursor on entry value, cancellable, and
// returns the entry chosen or -1.
// The parameter is an initial row index; cancellation returns -1 encoded as
// u32.
u32 Menu_RunSelectionForValue(u32 initialIndex) {
  struct SelectionScreen *screen = gResQueueWork;
  u32 result;

  screen->cursorIndex = initialIndex;
  screen->locked = 1;
  MenuSelection_BuildEntries(initialIndex);
  Menu_SetupSelectionBothSides();
  Menu_OpenSelectionWindow(0, 5);
  Resource_ScheduleOwnerResetDelayed();
  result = Menu_WaitForSelectionInput(1);
  Resource_ResetOwnerEntries();
  return result;
}

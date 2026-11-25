/// @file
/// The battle summon menu.
// Part of screens.c.

#include "screens_internal.h"

/// The tile column of a summon's first Djinn cost in the list: its element
/// icon, the count after it, two tiles for each element it needs.
#if LANGUAGE != LANGUAGE_JAPANESE
#define SUMMON_COST_X 13
#else
#define SUMMON_COST_X 10
#endif

/// The summons a page of the list shows.
#define SUMMON_LIST_ROWS 4

/// The battle summon list: the summons the party holds enough Djinn for
/// (Summon_ListLearnedFar, DjinnRecoveryTable.summonMask), SUMMON_LIST_ROWS to
/// a page, those `standby` can pay for first, each with its Djinn cost by
/// element, the others in TEXT_COLOR_RED.
s32 BattleSummonMenu_Run(s32 unusedA, s32 unusedB, const u8 *standby) {
  struct MenuSprite cursorStorage;
  struct MenuSprite sprites[SUMMON_LIST_ROWS];
  u8 learnedSummons[33]; // in menu order
  u8 orderedSummons[33]; // those standby can pay for first
  u16 text[64];
  s32 iconSlots[SUMMON_LIST_ROWS];
  u8 visibleSummons[SUMMON_LIST_ROWS];
  s32 glyph;
  s32 count;
  struct Window *window;
  struct WindowState *state;
  s32 drawnRow, drawnFirstEntry, selectedRow, firstEntry;
  s32 cursorHandle;
  struct Window *description, *counts;
  s32 mask, preferredRow;
  struct MenuPoint cursorPosition;
  const struct SummonDefinition *summon;
  struct BattleMenuState *navigation;
  struct MenuSprite *sprite;
  struct MenuSprite *cur;
  s32 orderedCount;
  s32 id, tile, affordable;
  s32 pressed, repeated, result;
  s32 index;

  state = WINDOW_STATE;
  drawnRow = -1;
  drawnFirstEntry = -1;
  // One 16x16 sprite.
  cursorHandle = (s16)VramCache_AllocEntry(128);
  description = Window_Create(0, 4, 30, 4,
                              WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                                  WINDOW_FLAG_BORDERED_CANVAS);
  counts = Window_Create(20, 8, 10, 3, MENU_WINDOW_FLAGS);
  mask = 0;
  BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
  window = Window_Create(SUMMON_MENU_WINDOW_X, 11, SUMMON_MENU_WINDOW_WIDTH, 9,
                         MENU_WINDOW_FLAGS);

  // Icon sprites for each row.
  for (index = 0; index < SUMMON_LIST_ROWS; index++) {
    s32 y = index * 2;

    sprite = &sprites[index];
    MenuSprite_Initialize(sprites[index], OBJ_SIZE_16X16);
    sprite->oam.f.x = window->x * 8 + 8;
    sprite->oam.f.y = (y + window->y) * 8 + 4;
  }
  for (index = 0; index < SUMMON_LIST_ROWS; index++) {
    s32 slot = VramCache_AllocEntry(128);
    iconSlots[index] = slot;
    sprites[index].oam.f.tile = VramCache_Reload(slot, -1);
  }

  // Move usable summons to the front in reverse traversal; keep others in
  // order.
  count = Summon_ListLearnedFar(learnedSummons);
  /* FAKEMATCH(dead): the first scan in a once-only block. */
  do {
    s32 i;

    orderedCount = 0;
    for (i = count - 1; i >= 0; i--) {
      const struct SummonDefinition *definition;
      s32 n, k;

      k = learnedSummons[i];
      definition = SummonDefinition_Get(k);
      for (n = 0; n < ELEMENT_COUNT; n++) {
        if (definition->djinnRequired[n] > standby[n])
          break;
      }
      if (n == ELEMENT_COUNT) {
        orderedSummons[orderedCount++] = k;
        learnedSummons[i] = SUMMON_END;
      }
    }
  } while (0);
  {
    s32 i, k;

    for (i = 0; i < count; i++) {
      k = learnedSummons[i];
      if (k != SUMMON_END)
        orderedSummons[orderedCount++] = k;
    }
  }
  orderedSummons[orderedCount] = SUMMON_END;

  for (;;) {
    /* FAKEMATCH(temporary): cur holds &cursorStorage beside sprite, which names
     * the cursor below; as &cursorStorage at each use. */
    cur = &cursorStorage;

    // Redraw when the page or row changes.
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      state->holdTilemapUpload = 1;
      Window_HighlightRect(window->x + 1, window->y + drawnRow * 2 + 1,
                           window->width - 2, 1, WINDOW_PALETTE);
      Text_FillCanvasBackground();

      // Description and required Djinni elements: mask has bit n for each
      // element n the summon needs, which blink below.
      {
        const struct SummonDefinition *definition;
        s32 element;

        definition =
            SummonDefinition_Get(orderedSummons[firstEntry + selectedRow]);
        BATTLE_MENU_MESSAGE_COPY(definition->abilityId + MsgAbilityDescription,
                                 text, BATTLE_MENU_MESSAGE_CAPACITY);
        Text_DrawWideStringAtOffset(text, (struct Window *)description, 0, 4);
        drawnRow = selectedRow;
        mask = 0;
        for (element = 0; element < ELEMENT_COUNT; element++) {
          if (definition->djinnRequired[element] != 0)
            mask |= 1 << element;
        }
      }

      // Standby counts and the four summons on this page.
      if (firstEntry != drawnFirstEntry) {
        WindowSprite_RedrawSavedRect(window);
        {
          s32 element;

          for (element = 0; element < ELEMENT_COUNT; element++) {
            Window_SetTilemapEntry(counts, element + LIST_TILE_SUMMON_ELEMENT,
                                   element * 2, 0, 0);
            Window_PutGlyph(counts, standby[element] + '0', element * 2 + 1, 0,
                            GLYPH_DRAW_CHARACTER);
          }
        }
        {
          s32 element, col, y;

          index = 0;
          if ((id = orderedSummons[firstEntry + index]) != SUMMON_END) {
          next:
            summon = SummonDefinition_Get(id);
            col = 0;
            for (element = 0; element < ELEMENT_COUNT; element++) {
              if (summon->djinnRequired[element] > standby[element])
                break;
            }
            affordable = element == ELEMENT_COUNT;
            Ability_BuildIconTiles(summon->abilityId & ACTION_ID_MASK, 0,
                                   &iconSlots[index], &glyph, 1);
            y = index * 2;
            sprites[index].oam.f.tile = glyph;
            // A summon the party cannot afford is drawn in TEXT_COLOR_RED.
            if (!affordable)
              Text_SetColor(TEXT_COLOR_RED);
            Text_DrawMessageAsTiles(SummonDefinition_Get(id)->abilityId +
                                        MsgAbilityName,
                                    window, 16, index * 16);
            for (element = 0; element < ELEMENT_COUNT; element++) {
              if (summon->djinnRequired[element] != 0) {
                Window_SetTilemapEntry(window,
                                       element + LIST_TILE_SUMMON_ELEMENT,
                                       col * 2 + SUMMON_COST_X, y, 0);
                Window_PutGlyph(window, summon->djinnRequired[element] + '0',
                                col * 2 + SUMMON_COST_X + 1, y,
                                GLYPH_DRAW_CHARACTER);
                col++;
              }
            }
            Text_SetColor(TEXT_COLOR_WHITE);
            visibleSummons[index++] = 1;
            if (index < SUMMON_LIST_ROWS &&
                (id = orderedSummons[firstEntry + index]) != SUMMON_END)
              goto next;
          }
          while (index < SUMMON_LIST_ROWS)
            visibleSummons[index++] = 0;
        }
        drawnFirstEntry = firstEntry;
      }

      // The page numbers.
      if (count > SUMMON_LIST_ROWS) {
        for (index = 0;
             index < (count + SUMMON_LIST_ROWS - 1) / SUMMON_LIST_ROWS;
             index++) {
          tile = index + PAGE_NUMBER_TILE;
          if (index == firstEntry / SUMMON_LIST_ROWS)
            tile = index + PAGE_NUMBER_LIT_TILE;
          Window_SetTilemapEntry(
              window, tile,
              window->width -
                  (count + SUMMON_LIST_ROWS - 1) / SUMMON_LIST_ROWS + index - 2,
              -1, 0);
        }
      }
      Window_HighlightRect(window->x + 1, window->y + selectedRow * 2 + 1,
                           window->width - 2, 1, HIGHLIGHT_PALETTE);
      state->dirty = 1;
      state->holdTilemapUpload = 0;
    }

    // Each frame: the row icons and the cursor.
    for (index = 0; index < SUMMON_LIST_ROWS; index++) {
      if (visibleSummons[index] != 0)
        Oam_AddSpriteAtDepth(&sprites[index], OAM_DEPTH_WINDOW);
    }

    // The cursor wobbles every four frames.
    MENU_PLACE_CURSOR(cursorPosition, window, selectedRow, 2, cur);
    cur->oam.f.tile =
        VramCache_Reload((u16)cursorHandle, (s32)gPointingHandTiles);
    cur->oam.f.x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    cur->oam.f.y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    // sprite, done with the row icons, now names the cursor.
    sprite = &cursorStorage;
    Oam_AddSpriteAtDepth(sprite, OAM_DEPTH_WINDOW + 2);

    // The standby counts of the elements the summon under the cursor needs
    // blink every eight frames.
    {
      s32 element, palette, blink;

      blink = gFrameCount & 8;
      for (element = 0; element < ELEMENT_COUNT; element++) {
        // HIGHLIGHT_PALETTE, one below, while the blink is on.
        palette = WINDOW_PALETTE - (blink != 0);
        if ((1 << element & mask) == 0)
          palette = WINDOW_PALETTE;
        Window_HighlightRect(counts->x + element * 2 + 1, counts->y + 1, 2, 1,
                             palette);
      }
    }

    // Page numbers (current page blinking) and left/right arrows.
    if (count > SUMMON_LIST_ROWS) {
      for (index = 0; index < (count + SUMMON_LIST_ROWS - 1) / SUMMON_LIST_ROWS;
           index++) {
        tile = index + PAGE_NUMBER_TILE;
        if ((gFrameCount & 15) <= 11 && index == firstEntry / SUMMON_LIST_ROWS)
          tile = index + PAGE_NUMBER_LIT_TILE;
        Window_SetTilemapEntry(
            window, tile,
            window->width - (count + SUMMON_LIST_ROWS - 1) / SUMMON_LIST_ROWS +
                index - 2,
            -1, 0);
      }
      Window_SetTilemapEntry(
          window, LIST_TILE_PAGES_LEFT,
          window->width - (count + SUMMON_LIST_ROWS - 1) / SUMMON_LIST_ROWS - 3,
          -1, 0);
      Window_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2,
                             -1, 0);
      Window_MarkTilemapRow(state, window->y - 1);
    }

    BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
    navigation = gBattleMenuState;
    pressed = gKeysPressed;
    repeated = gKeysRepeat;

    // Automatic input confirms every 120 frames, while the Djinn tutorial
    // runs.
    if (navigation->tutorialStep != 0) {
      repeated = 0;
      pressed = 0;
      if (navigation->tutorialDelay == 0) {
        navigation->tutorialDelay = 120;
        repeated = KEY_A;
        pressed = KEY_A;
      } else {
        navigation->tutorialDelay--;
      }
    }
    // A takes the summon under the cursor, affordable or not.
    if (pressed & KEY_A) {
      result = orderedSummons[firstEntry + selectedRow];
      break;
    }
    if (gBattleMenuState->timer == 0 || (pressed & KEY_B)) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (repeated & KEY_DOWN) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selectedRow++;
      if (selectedRow == SUMMON_LIST_ROWS || firstEntry + selectedRow == count)
        selectedRow = 0;
      preferredRow = selectedRow;
    } else if (repeated & KEY_UP) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      selectedRow--;
      if (selectedRow < 0) {
        // On the last page the cursor goes to its last entry.
        if (firstEntry == ((count - 1) / SUMMON_LIST_ROWS) * SUMMON_LIST_ROWS)
          selectedRow = count - firstEntry - 1;
        else
          selectedRow = SUMMON_LIST_ROWS - 1;
      }
      preferredRow = selectedRow;
    } else if (repeated & KEY_RIGHT) {
      // Next page, wrapping from last to first.
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      if (firstEntry + SUMMON_LIST_ROWS >= count) {
        if (firstEntry != 0) {
          firstEntry = 0;
          selectedRow = preferredRow;
        }
      } else {
        firstEntry += SUMMON_LIST_ROWS;
        selectedRow = preferredRow;
        if (firstEntry == ((count - 1) / SUMMON_LIST_ROWS) * SUMMON_LIST_ROWS) {
          selectedRow = count - firstEntry - 1;
          if (selectedRow > preferredRow)
            selectedRow = preferredRow;
        }
      }
    } else if (repeated & KEY_LEFT) {
      // Previous page, wrapping from first to last.
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      if (firstEntry != 0) {
        firstEntry -= SUMMON_LIST_ROWS;
        selectedRow = preferredRow;
      } else {
        firstEntry = ((count - 1) / SUMMON_LIST_ROWS) * SUMMON_LIST_ROWS;
        selectedRow = preferredRow;
        if (firstEntry != 0) {
          selectedRow = count - firstEntry - 1;
          if (selectedRow > preferredRow)
            selectedRow = preferredRow;
        }
      }
    }
    wait(1);
  }

  wait(1);
  for (index = 0; index < SUMMON_LIST_ROWS; index++)
    VramCache_FreeEntry(iconSlots[index]);
  VramCache_FreeEntry((u16)cursorHandle);
  Window_Close(counts, 1);
  Window_Close(description, 1);
  Window_Close(window, 1);
  wait(1);
  return result;
}

/// @file
/// The battle summon menu.
// Part of battle_command.c.

#include "../battle_command_internal.h"

// The tile column of a summon's first Djinn cost in the list: its element
// icon, the count after it, two tiles for each element it needs.
#if LANGUAGE != LANGUAGE_JAPANESE
#define SUMMON_COST_X 13
#else
#define SUMMON_COST_X 10
#endif

/// Ends the summon lists, and marks a summon already moved to the front.
#define SUMMON_END 32

/// The battle summon list: the summons the party holds enough Djinn for
/// (Summon_ListLearnedFar, BattleSummonState.availableMask), four to a page,
/// those `standby` can pay for first, each with its Djinn cost by element,
/// the others in colour 2. Above it the summon's description, and the
/// standby Djinn of each element, the elements it needs blinking. `standby`
/// is what the earlier commands this round leave
/// (BattleMenuWork.standbyDjinn). Returns the chosen summon, even one that
/// cannot be paid for (Battle_CollectPartyCommands then plays the error
/// sound and goes on), or -1 when cancelled or when the link battle's time
/// runs out. The Djinn tutorial presses A every 120 frames.
s32 SummonMenu_SelectSummon(s32 unused0, s32 unused1, const u8 *standby) {
  struct CursorSprite cursorStorage;
  struct MenuSprite sprites[4];
  u8 learnedSummons[33]; // The callable summons, in menu order
  u8 orderedSummons[33]; // Those standby can pay for first
  u16 text[64];
  s32 iconSlots[4];
  u8 visibleSummons[4];
  s32 glyph;
  s32 count;
  struct UiWindow *window;
  struct UiRenderWork *work;
  s32 drawnRow, drawnFirstEntry, selectedRow, firstEntry;
  s32 cursorHandle;
  struct UiWindow *description, *counts;
  s32 mask, preferredRow;
  struct MenuPoint cursorPosition;
  const struct SummonDefinition *summon;
  struct BattleMenuWork *navigation;
  struct MenuSprite *sprite;
  struct CursorSprite *cur;
  s32 orderedCount;
  s32 id, tile, affordable;
  s32 pressed, repeated, result;
  s32 index;

  work = UI_WORK;
  drawnRow = -1;
  drawnFirstEntry = -1;
  // One 16x16 sprite.
  cursorHandle = (s16)VramCache_AllocEntry(128);
  description = UiWindow_Create(0, 4, 30, 4, 42);
  counts = UiWindow_Create(20, 8, 10, 3, 6);
  mask = 0;
  BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
  window =
      UiWindow_Create(SUMMON_MENU_WINDOW_X, 11, SUMMON_MENU_WINDOW_WIDTH, 9, 6);

  // Icon sprites for each row.
  for (index = 0; index < 4; index++) {
    s32 y = index * 2;

    sprite = &sprites[index];
    MenuSprite_Initialize(sprites[index], MENU_SPRITE_SIZE_16);
    sprite->oam.f.x = window->x * 8 + 8;
    sprite->oam.f.y = (y + window->y) * 8 + 4;
  }
  for (index = 0; index < 4; index++) {
    s32 slot = VramCache_AllocEntry(128);
    iconSlots[index] = slot;
    sprites[index].oam.f.tile = VramCache_Reload(slot, -1);
  }

  // Move usable summons to the front in reverse traversal; keep others in
  // order. So the affordable ones come in reverse menu order
  // (gSummon_OrderList), the rest after them in menu order.
  count = Summon_ListLearnedFar(learnedSummons);
  /* FAKEMATCH(one-pass): the first scan's exit inside a block that runs once
   * leaves the main loop's select block in place; as a plain block, 88 of 946
   * instructions differ in tbs-en. */
  do {
    s32 i;

    orderedCount = 0;
    for (i = count - 1; i >= 0; i--) {
      const struct SummonDefinition *definition;
      s32 n, k;

      k = learnedSummons[i];
      definition = SummonDefinition_Get(k);
      for (n = 0; n <= 3; n++) {
        if (definition->djinnRequired[n] > standby[n])
          break;
      }
      if (n == 4) {
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
    cur = &cursorStorage;

    //------------------------------------------------------------------
    // Redraw when the page or row changes.
    //------------------------------------------------------------------
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      work->menuBusy = 1;
      Ui_SetRectHighlight(window->x + 1, window->y + drawnRow * 2 + 1,
                          window->width - 2, 1, 15);
      Ui_FillVramBlockPattern();

      // Description and required Djinni elements: mask has bit n for each
      // element n the summon needs, which blink below.
      {
        const struct SummonDefinition *definition;
        s32 element;

        definition =
            SummonDefinition_Get(orderedSummons[firstEntry + selectedRow]);
        BATTLE_MENU_MESSAGE_COPY(definition->nameMessageId +
                                     MsgAbilityDescription,
                                 text, BATTLE_MENU_MESSAGE_CAPACITY);
        UiText_RenderWideStringAtOffset(text, (struct UiWindow *)description, 0,
                                        4);
        drawnRow = selectedRow;
        mask = 0;
        for (element = 0; element <= 3; element++) {
          if (definition->djinnRequired[element] != 0)
            mask |= 1 << element;
        }
      }

      // Standby counts and the four summons on this page.
      if (firstEntry != drawnFirstEntry) {
        RenderOutput_RedrawSavedRect(window);
        {
          s32 element;

          for (element = 0; element <= 3; element++) {
            UiWindow_SetTilemapEntry(counts, element + SUMMON_ELEMENT_TILE,
                                     element * 2, 0, 0);
            UiWindow_PutGlyph(counts, standby[element] + '0', element * 2 + 1,
                              0, GLYPH_DRAW_CHARACTER);
          }
        }
        {
          s32 element, col, y;

          index = 0;
          // The rows are a loop written with goto (MATCHING.md): as a
          // do-while loop inside this test, 523 of 946 instructions differ in
          // tbs-en.
          if ((id = orderedSummons[firstEntry + index]) != SUMMON_END) {
          next:
            summon = SummonDefinition_Get(id);
            col = 0;
            for (element = 0; element <= 3; element++) {
              if (summon->djinnRequired[element] > standby[element])
                break;
            }
            affordable = element == 4;
            Ability_LoadGlyph(summon->nameMessageId & 0x3fff, 0,
                              &iconSlots[index], &glyph, 1);
            y = index * 2;
            sprites[index].oam.f.tile = glyph;
            // A summon the party cannot afford is drawn in colour 2.
            if (!affordable)
              UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(
                SummonDefinition_Get(id)->nameMessageId + MsgAbilityName,
                window, 16, index * 16);
            for (element = 0; element <= 3; element++) {
              if (summon->djinnRequired[element] != 0) {
                UiWindow_SetTilemapEntry(window, element + SUMMON_ELEMENT_TILE,
                                         col * 2 + SUMMON_COST_X, y, 0);
                UiWindow_PutGlyph(window, summon->djinnRequired[element] + '0',
                                  col * 2 + SUMMON_COST_X + 1, y,
                                  GLYPH_DRAW_CHARACTER);
                col++;
              }
            }
            UiWork_SetParamNibble(15);
            visibleSummons[index++] = 1;
            if (index <= 3 &&
                (id = orderedSummons[firstEntry + index]) != SUMMON_END)
              goto next;
          }
          while (index <= 3)
            visibleSummons[index++] = 0;
        }
        drawnFirstEntry = firstEntry;
      }

      // Page numbers
      if (count > 4) {
        for (index = 0; index < (count + 3) / 4; index++) {
          tile = index + LIST_TILE_PAGE_NUMBER;
          if (index == firstEntry / 4)
            tile = index + LIST_TILE_PAGE_NUMBER_LIT;
          UiWindow_SetTilemapEntry(
              window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
        }
      }
      Ui_SetRectHighlight(window->x + 1, window->y + selectedRow * 2 + 1,
                          window->width - 2, 1, 14);
      work->dirty = 1;
      work->menuBusy = 0;
    }

    //------------------------------------------------------------------
    // Per-frame display
    //------------------------------------------------------------------
    for (index = 0; index <= 3; index++) {
      if (visibleSummons[index] != 0)
        Runtime_PushSlotEntry((s32 *)&sprites[index], 240);
    }

    // The cursor wobbles every four frames.
    cursorPosition.x = window->x * 8 - 2;
    cursorPosition.y = (selectedRow * 2 + window->y) * 8 + 20;
    ((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
    ((u32 *)cur)[2] = 0;
    cur->tile =
        VramCache_Reload((u16)cursorHandle, (s32)gResource_FixedBlockBTiles);
    cur->x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    cur->y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    /* FAKEMATCH(temporary): the cursor pushed through sprite, re-pointed at it
     * here, keeps the frame loop's registers; as &cursor or cur pushed
     * directly, 41 of 946 instructions differ in tbs-en. */
    sprite = (struct MenuSprite *)&cursorStorage;
    Runtime_PushSlotEntry(sprite, 242);

    // Blink required elements' standby counts every eight frames.
    {
      s32 element, tile, blink;

      blink = gFrameCount & 8;
      for (element = 0; element <= 3; element++) {
        tile = 15 - (blink != 0);
        if ((1 << element & mask) == 0)
          tile = 15;
        Ui_SetRectHighlight(counts->x + element * 2 + 1, counts->y + 1, 2, 1,
                            tile);
      }
    }

    // Page numbers (current page blinking) and left/right arrows.
    if (count > 4) {
      for (index = 0; index < (count + 3) / 4; index++) {
        tile = index + LIST_TILE_PAGE_NUMBER;
        if ((gFrameCount & 15) <= 11 && index == firstEntry / 4)
          tile = index + LIST_TILE_PAGE_NUMBER_LIT;
        UiWindow_SetTilemapEntry(
            window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
      }
      UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_LEFT,
                               window->width - (count + 3) / 4 - 3, -1, 0);
      UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2,
                               -1, 0);
      UiRender_MarkRow(work, window->y - 1);
    }

    //------------------------------------------------------------------
    // Input
    //------------------------------------------------------------------
    BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
    navigation = gLinkCountdownWork;
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
    if (gLinkCountdownWork->timer == 0 || (pressed & KEY_B)) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (repeated & KEY_DOWN) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selectedRow++;
      if (selectedRow == 4 || firstEntry + selectedRow == count)
        selectedRow = 0;
      preferredRow = selectedRow;
    } else if (repeated & KEY_UP) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selectedRow--;
      if (selectedRow < 0) {
        // On the last page the cursor goes to its last entry.
        if (firstEntry == ((count - 1) / 4) * 4)
          selectedRow = count - firstEntry - 1;
        else
          selectedRow = 3;
      }
      preferredRow = selectedRow;
    } else if (repeated & KEY_RIGHT) {
      // Next page, wrapping from last to first.
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      if (firstEntry + 4 >= count) {
        if (firstEntry != 0) {
          firstEntry = 0;
          selectedRow = preferredRow;
        }
      } else {
        firstEntry += 4;
        selectedRow = preferredRow;
        if (firstEntry == ((count - 1) / 4) * 4) {
          selectedRow = count - firstEntry - 1;
          if (selectedRow > preferredRow)
            selectedRow = preferredRow;
        }
      }
    } else if (repeated & KEY_LEFT) {
      // Previous page, wrapping from first to last.
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      if (firstEntry != 0) {
        firstEntry -= 4;
        selectedRow = preferredRow;
      } else {
        firstEntry = ((count - 1) / 4) * 4;
        selectedRow = preferredRow;
        if (firstEntry != 0) {
          selectedRow = count - firstEntry - 1;
          if (selectedRow > preferredRow)
            selectedRow = preferredRow;
        }
      }
    }
    WaitFrames(1);
  }

  // Cleanup
  WaitFrames(1);
  for (index = 0; index < 4; index++)
    Resource_ResetEntry(iconSlots[index]);
  Resource_ResetEntry((u16)cursorHandle);
  UiWork_Finalize(counts, 1);
  UiWork_Finalize(description, 1);
  UiWork_Finalize(window, 1);
  WaitFrames(1);
  return result;
}

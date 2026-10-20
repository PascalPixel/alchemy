/// @file
/// The debug menus (scenes, flags, icon browsers) and the system menu's
/// choices.
/// Part of battle_command.c, the menu object, compiled only through it.

#include "../battle_command_internal.h"

/// Schedules the debug scene and entrance selector; 0xc80 is a scheduler order,
/// not a number of frames to wait. Nothing in the game calls it.
UNUSED void DebugMenu_ScheduleSceneSelector(void) {
#ifdef UBFIX
  Scheduler_AddOrUpdateCallback(Menu_RunSelection, 0xC80);
#else
  Scheduler_AddOrUpdateCallback((SchedulerCallback)Menu_RunSelection, 0xC80);
#endif
}

/// Draws the debug scene and entrance numbers with their category/area labels.
void Menu_DrawSelectionRow(struct UiWindow *work, s32 scene,
                           const s16 *entrance) {
  s16 sceneIndex = scene;
  s32 label =
      Scene_GetAreaNameIndexFar(sceneIndex, *entrance) + MsgDebugEntryName;
  RenderOutput_PrepareForRedraw(work);
  UiText_DrawNumber(sceneIndex, 3, work, 0, 14);
  UiText_DrawNumber(*entrance, 3, work, MENU_LABEL_X, 14);
  UiText_DrawString(gMenu_ColonString, work, MENU_TEXT_X, 0);
  UiText_DrawResource(sceneIndex + MsgDebugCategoryName, work, 0, 0);
  UiText_DrawString(gMenu_ColonString, work, MENU_TEXT_X, 14);
  UiText_DrawResource(label, work, MENU_LABEL_X, 0);
}

// The scheduler and the debug menu's far call ignore this selection result.
// Plain void removes two result moves (252 to 248 bytes in tbs-en) and
// reorders Event_SetDestinationFar's argument setup.
#ifdef UBFIX
void Menu_RunSelection(void) {
#else
s16 Menu_RunSelection(void) {
#endif
  s32 cursorSlot;
  s16 selectedField;
  s16 entrance;
  struct MenuSprite cursor;
  s16 scene;
  struct UiWindow *window;
  s16 result;

  window = 0;
  selectedField = 0;
  scene = gGameState.defaultScene;
  entrance = gGameState.defaultEntrance;
  window = UiWindow_Create(0, 7, 30, 5, 2);
  Menu_DrawSelectionRow(window, scene, &entrance);
  UiTextResource_Initialize(&cursor, &cursorSlot);

  while (gKeysHeld != 0)
    WaitFrames(1);

  for (;;) {
    result =
        Menu_HandleSelectionRowInput(window, scene, &entrance, &selectedField);
    if (result == -1) {
      UiTextResource_Release(cursorSlot);
      UiWork_Finalize(window, 2);
      Event_SetDestinationFar(scene, entrance);
#ifdef UBFIX
      return;
#else
      return result;
#endif
    }
    if (result == -2) {
      UiTextResource_Release(cursorSlot);
      UiWork_Finalize(window, 2);
#ifdef UBFIX
      return;
#else
      return result;
#endif
    }

    UiTextResource_SetPosition(&cursor, MENU_TEXT_X,
                               selectedField * 14 + MENU_TEXT_Y);
    scene = result;
    WaitFrames(1);
  }
}

/// Edits the debug scene or entrance, returning -1 to warp and -2 to cancel.
/// Shoulder buttons step by ten; keep the original 99-step entrance wrap.
s32 Menu_HandleSelectionRowInput(struct UiWindow *window, s16 scene,
                                 s16 *entrance, s16 *selectedField) {
  if (gKeysRepeat & KEY_A)
    return -1;
  if (gKeysRepeat & KEY_B)
    return -2;
  if (gKeysRepeat & KEY_DOWN) {
    *selectedField ^= 1;
  } else if (gKeysRepeat & KEY_UP) {
    *selectedField ^= 1;
  } else if (gKeysRepeat & KEY_RIGHT) {
    if (*selectedField == 0) {
      scene++;
    } else {
      (*entrance)++;
      if (*entrance > 99)
        *entrance = 0;
    }
    if (scene > 200)
      scene = 0;
    Menu_DrawSelectionRow(window, scene, entrance);
  } else if (gKeysRepeat & KEY_LEFT) {
    if (*selectedField == 0) {
      scene--;
    } else {
      (*entrance)--;
      if (*entrance < 0)
        *entrance = 99;
    }
    if (scene < 0)
      scene = 200;
    Menu_DrawSelectionRow(window, scene, entrance);
  } else if (gKeysRepeat & KEY_R) {
    if (*selectedField == 0) {
      *entrance = 0;
      scene += 10;
    } else {
      *entrance += 10;
      if (*entrance > 99)
        *entrance -= 99;
    }
    if (scene > 200)
      scene = 0;
    Menu_DrawSelectionRow(window, scene, entrance);
  } else if (gKeysRepeat & KEY_L) {
    if (*selectedField == 0) {
      *entrance = 0;
      scene -= 10;
    } else {
      *entrance -= 10;
      if (*entrance < 0)
        *entrance += 99;
    }
    if (scene < 0)
      scene = 200;
    Menu_DrawSelectionRow(window, scene, entrance);
  }
  return scene;
}

/// Shows a 16x16 flag grid per page and processes input until closed.
/// Start on page 8; always return 0.
s32 Menu_RunSelectionWithCursorObject(void) {
  struct MenuSprite cursorSprite; // Cursor object
  s32 cursor[2];                  // Cursor column and row
  s32 cursorSlot;
  s32 page;
  struct UiWindow *window;
  s32 change;

  page = 8;
  window = 0;
  cursor[0] = 0;
  cursor[1] = 0;
  window = UiWindow_Create(1, 0, 28, 20, 2);
  Menu_DrawFlagBitTable(window, page);
  UiTextResource_Initialize(&cursorSprite, &cursorSlot);
  for (;;) {
    WaitFrames(1);
    change = Menu_HandleFlagGridInput(window, &page, cursor);
    // B closes the table; a change of flag or page redraws it.
    if (change == -1)
      break;
    if (change == 1)
      Menu_DrawFlagBitTable(window, page);
    UiTextResource_SetPosition(&cursorSprite, cursor[0] * 8 + 58,
                               cursor[1] * 8 + 20);
  }
  UiTextResource_Release(cursorSlot);
  UiWork_Finalize(window, 2);
  return 0;
}

/// Writes the lowest count digits of value, up to five, most significant first
/// as uppercase hexadecimal in out; do not append a null terminator.
void Text_FormatHex(u32 value, u32 count, u8 *out) {
  u8 digits[8];
  u32 i;

  if (count > 5)
    count = 5;

  for (i = 0; i != count; i++) {
    u32 digit = value & 15;
    if (digit <= 9)
      digits[i] = digit + '0';
    else
      digits[i] = digit + 'A' - 10;
    value >>= 4;
  }

  for (i = count - 1; (s32)i >= 0; i--)
    *out++ = digits[i];
}

/// Draws one page of 256 flags as sixteen rows of hexadecimal labels and bits.
void Menu_DrawFlagBitTable(struct UiWindow *window, s32 page) {
  s32 row;
  s32 y;
  s32 flag;
  char label[5];
  char bits[17];

  RenderOutput_PrepareForRedraw(window);
  UiText_DrawStringInWindow((u8 *)gMenu_HexDigitsString, window, 48, 0);

  flag = page << 8;
  for (row = 0; row != 16; row++) {
    s32 i;

    y = row * 8 + 16;

    for (i = 0; i != 5; i++) {
      label[i] = 0;
    }
    Text_FormatHex(flag, 3, label);
    UiText_DrawStringInWindow((u8 *)label, window, 0, y);
    UiText_DrawStringInWindow((u8 *)gMenu_ColonString, window, 32, y);

    for (i = 0; i < 16; i++) {
      s32 set = GameFlag_TestFar(flag);
      bits[i] = (set != 0) + 48;
      flag++;
    }
    bits[i] = 0;
    UiText_DrawStringInWindow((u8 *)bits, window, 48, y);
  }
}

/// Moves within the flag grid, change pages, or toggles the selected flag.
/// Return 1 when flags/page need redrawing, -1 to close, and 0 otherwise.
s32 Menu_HandleFlagGridInput(struct UiWindow *window, s32 *page, s32 *cursor) {
  s32 *row = &cursor[1];

  if (gKeysRepeat & KEY_A) {
    s32 flag = ((*page << 4) + *row << 4) + cursor[0];

    if (GameFlag_TestFar(flag))
      GameFlag_ClearBitFar(flag);
    else
      GameFlag_SetBitFar(flag);
    return 1;
  }
  if ((gKeysPressed & KEY_B) || (gKeysRepeat & KEY_SELECT))
    return -1;
  if (gKeysRepeat & KEY_UP) {
    if (--*row < 0)
      *row = 15;
  } else if (gKeysRepeat & KEY_DOWN) {
    if (++*row > 15)
      *row = 0;
  } else if (gKeysRepeat & KEY_LEFT) {
    if (--cursor[0] < 0)
      cursor[0] = 15;
  } else if (gKeysRepeat & KEY_RIGHT) {
    if (++cursor[0] > 15)
      cursor[0] = 0;
  } else if ((gKeysRepeat & KEY_L) && (gKeysRepeat & KEY_START)) {
    *page -= 10;
    if (*page < 0)
      *page = 15;
    return 1;
  } else if ((gKeysRepeat & KEY_R) && (gKeysRepeat & KEY_START)) {
    *page += 10;
    if (*page > 15)
      *page = 0;
    return 1;
  } else if (gKeysRepeat & KEY_L) {
    if (--*page < 0)
      *page = 15;
    return 1;
  } else if (gKeysRepeat & KEY_R) {
    if (++*page > 15)
      *page = 0;
    return 1;
  }
  return 0;
}

/// Reserved menu entry.
void Menu_ReservedNoOp(void) {}

/// Sets the Djinni's collected flag, then adds it to the specified owner.
void Djinn_SetFoundFlagAndAdd(s32 owner, s32 element, s32 index) {
  GameFlag_SetBitFar((element * 0x14) + index + FLAG_DJINNI_0);
  Djinn_AddToOwnerFar(owner, element, index);
}

/// Handles Save, Sleep and Options from the field workspace. A cancelled save
/// or options screen reopens the workspace; cancelling the workspace returns
/// -1.
s32 Menu_RunWorkspaceResultLoop(void) {
  s32 result;

  result = Menu_RunWorkspaceSelectionLoop();
  if (result == -1) {
    return -1;
  }
  if (result == 0) {
    if (Save_WriteSelectedSlot() == -1) {
      return Menu_RunWorkspaceResultLoop();
    }
  } else if (result == 1) {
    UiText_ShowPositionedMessageAndWait(MsgSleepMode, 1);
    gSleepRequested = result;
  } else if (result == 2) {
    if (Menu_RunWorkspaceOptions() == -1) {
      return Menu_RunWorkspaceResultLoop();
    }
  }
  return 0;
}

/// Browses pages of item, Psynergy and status-pattern icons. Suspend the menu
/// controller while the browser owns its window and cache entries.
s32 DebugMenu_BrowseIcons(void) {
  s32 redraw;
  s32 tile;
  s32 page;
  s32 mode;
  struct UiWindow *window;
  s32 slot;
  s32 base;
  s32 i;
  s32 row;
  s32 x;
  s32 y;

  window = NULL;
  page = 0;
  mode = 0;
  redraw = 1;
  gMenuCtrlWork->suspended = redraw;
  WaitFrames(1);

  while (1) {
    if (gKeysRepeat & KEY_LEFT) {
      page--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      redraw = 1;
      page++;
    }
    if (gKeysRepeat & KEY_L) {
      mode--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_R) {
      redraw = 1;
      mode++;
    }
    if (gKeysRepeat & KEY_A)
      break;
    if (gKeysRepeat & KEY_B)
      break;
    if (redraw != 0) {
      redraw = 0;
      page = (page + 8) % 8;
      mode = (mode + 3) % 3;
      UiWork_Finalize(window, 2);
      window = UiWindow_Create(10, 0, 18, 12, 2);
      if (mode == 0)
        UiText_DrawStringInWindow(gWorkspace_ItemTitle, window, 0, 0);
      else if (mode == 1)
        UiText_DrawStringInWindow(gWorkspace_EnergyTitle, window, 0, 0);
      else
        UiText_DrawStringInWindow(gWorkspace_StatusTitle, window, 0, 0);
      UiText_DrawStringInWindow(gWorkspace_PageLabel, window, 0, 8);
      UiText_DrawNumberInWindow(page, 0, window, 40, 8);
      base = page << 5;
      UiText_DrawNumberInWindow(base, 3, window, 64, 8);
      UiText_DrawStringInWindow(gWorkspace_RangeDash, window, 88, 8);
      UiText_DrawNumberInWindow(base + 31, 3, window, 96, 8);
      for (i = 0; i <= 31; i++) {
        slot = -1;
        row = i / 8;
        x = (i - row * 8) * 16;
        y = row * 16 + 16;
        if (mode == 0) {
          UiIcon_BuildItemIconTiles(base + i, 1, &slot, &tile, mode);
          RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        } else if (mode == 1) {
          UiIcon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
          RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        } else {
          slot = Resource_FindFreeEntry();
          Ui_BuildPatternToSlot(i, 0, slot);
          RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        }
      }
    }
    WaitFrames(1);
  }
  UiWork_Finalize(window, 2);
  gMenuCtrlWork->suspended = 0;
  return 0;
}

#if !TBS_PATCH_2
#define DEBUG_GLYPH_NAME MsgDebugGlyphName
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define DEBUG_GLYPH_NAME MsgEnd
#else
#define DEBUG_GLYPH_NAME MsgDebugSecondForm
#endif

/// Browses the character and sprite portrait maps as one circular list. Show
/// each entry's glyph and label, then release both windows on A or B.
s32 DebugMenu_BrowseEntryGlyphs(void) {
  struct UiRenderWork *work;
  struct UiWindow *portrait;
  s32 redraw;
  struct UiWindow *window;
  s32 characterCount;
  s32 entryCount;
  s32 index;
  s32 i;
  s32 glyph;
  s32 tile;
  s32 slot;
  s32 cancelKey;
  u32 slotOffset;

  work = UI_WORK;
  redraw = 1;
  portrait = UiWindow_CreateWithSideObject(0, 0, 10, 5);
  window = UiWindow_Create(10, 10, 14, 3, 2);
  index = 0;
  for (i = 0; gPortrait_CharacterMap[i * 2] != -1; i++) {
  }
  characterCount = i;
  for (i = 0; gPortrait_SpriteMap[i * 2] != -1; i++) {
  }
  entryCount = characterCount + i;

  /* FAKEMATCH: the count-derived mask keeps the mask in r9 and work in fp; as
   * cancel = KEY_B, 12 of 179 instructions differ in tbs-en. */
  cancelKey = ((u32)(u16)characterCount >> 16) | KEY_B;
  /* FAKEMATCH: defining the offset before the loop keeps its load in r1;
   * defining it inside redraw uses r3 (4 instructions differ in all six
   * editions). */
  slotOffset = RENDER_SLOT_VALUE_OFFSET + 2;
  while (1) {
    if (gKeysRepeat & KEY_LEFT) {
      redraw = 1;
      index--;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      redraw = 1;
      index++;
    }
    if (gKeysRepeat & KEY_L) {
      redraw = 1;
      index -= 10;
    }
    if (gKeysRepeat & KEY_R) {
      redraw = 1;
      index += 10;
    }
    if (gKeysRepeat & KEY_A)
      break;
    if (gKeysRepeat & cancelKey)
      break;
    if (redraw) {
      redraw = 0;
      index = (index + entryCount) % entryCount;
      RenderOutput_PrepareForRedraw(window);
      if (index < characterCount)
        glyph = gPortrait_CharacterMap[index * 2 + 1];
      else
        glyph = gPortrait_SpriteMap[(index - characterCount) * 2 + 1] + 128;
      {
        /* FAKEMATCH(temporary): a word temporary for the slot keeps the
         * palette in r2 through the slot store; as the slot assigned directly
         * and 15 passed, 4 of 179 instructions differ in tbs-en. */
        s32 palette;
        s32 loadedSlot = *(u16 *)((u8 *)work + slotOffset);
        palette = 15;
        slot = loadedSlot;
        UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, palette, 1);
      }
      UiText_DrawNumberInWindow(index, 2, window, 0, 0);
      UiText_DrawCharacterAtOffset(index + DEBUG_GLYPH_NAME, window, 24, 0);
    }
    WaitFrames(1);
  }
  UiWork_Finalize(window, 2);
  UiWork_Finalize(portrait, 2);
  WaitFrames(1);
  return 0;
}

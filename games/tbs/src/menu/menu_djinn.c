/// @file
/// The Djinn menu: its screen, the Djinn list, help, and choosing the Djinn to
/// set, put on standby, give or trade.
// Part of menu.c.

#include "menu_internal.h"

/// The editions outside Japanese widen the Djinn list's highlight and lay the
/// stat preview out for fewer digits; from patch 1 a trade preview shows its
/// second side in another mode.
#if LANGUAGE != LANGUAGE_JAPANESE
#define DJINN_CURSOR_WIDTH 6
#else
#define DJINN_CURSOR_WIDTH 5
#endif

/// DjinnMenuState.pairEntries before a Djinni is chosen: every element and
/// number bit set, which no list entry has.
#define DJINN_ENTRY_NONE (DJINN_ENTRY_ELEMENT | DJINN_ENTRY_INDEX)

/// The tile of BG character block 1 the backdrop's digits are copied to.
#define DJINN_BACKDROP_DIGIT_TILE 128

/// The palettes the Djinn menu takes over, kept in DjinnMenuBackdrop.palette
/// while it is open: from DJINN_BACKDROP_PALETTE, a highlighted row's element
/// icons (the highlight's palette bit turns BASIC_COLORS_PALETTE into it, as it
/// turns WINDOW_PALETTE into HIGHLIGHT_PALETTE), the element icons' own, the
/// dimmed window colours a chooser puts behind it and the copy of the window
/// colours they are darkened from.
#define DJINN_BACKDROP_PALETTE 4
#define DJINN_DIM_PALETTE 6
#define DJINN_WINDOW_COPY_PALETTE 7

/// The mode of a trade preview's second side.
#if TBS_PATCH_1
#define DJINN_SWAP_PREVIEW_MODE DJINN_PREVIEW_TRADE
#else
#define DJINN_SWAP_PREVIEW_MODE DJINN_PREVIEW_ADD
#endif

/// The stat preview's columns; an arrow stands at x (in tiles) or, in the
/// international editions, at the pixel column iconX the preview sets.
#if LANGUAGE != LANGUAGE_JAPANESE
#define DJINN_PREVIEW_DIGITS 3
#define DJINN_PREVIEW_CURRENT_X 48
#define DJINN_PREVIEW_MAX_X 80
#define DJINN_PREVIEW_SLASH_X 72
#define DJINN_PREVIEW_LUCK_X 56
#define DJINN_PREVIEW_ARROW_X(x, iconX) (iconX)
#define DJINN_PREVIEW_PP_X 88
#else
#define DJINN_PREVIEW_DIGITS 4
#define DJINN_PREVIEW_CURRENT_X 32
#define DJINN_PREVIEW_MAX_X 72
#define DJINN_PREVIEW_SLASH_X 64
#define DJINN_PREVIEW_LUCK_X 48
#define DJINN_PREVIEW_ARROW_X(x, iconX) ((x) * 8 + 64)
#define DJINN_PREVIEW_PP_X 56
#endif

/// The y of the preview's stat row `row` (0 to 5: HP, PP, Attack, Defense,
/// Agility and Luck) in a window whose top is tile row y.
#define DJINN_PREVIEW_STAT_Y(y, row) ((y) * 8 + 56 + (row) * 8)
/// The x of a changed stat's new value on that row, for a window whose left
/// is tile column x.
#define DJINN_PREVIEW_VALUE_X(x) ((x) * 8 + 72)

/// The party's Djinn columns, one a member, 7 tiles wide, and the x of a
/// column's figure.
#define DJINN_COLUMN_TILES 7
#define DJINN_COLUMN_WIDTH (DJINN_COLUMN_TILES * 8)
#define DJINN_FIGURE_X(column) ((column) * DJINN_COLUMN_WIDTH + 48)

/// The cursor packed in DjinnMenuState.selection: column + row * 10.
#define DJINN_CURSOR_ROW_STRIDE 10

/// Wraps a cursor index that has stepped one past either end of modulus
/// entries: -1 becomes modulus - 1 and modulus becomes 0.
s32 Menu_WrapIndex(s32 value, s32 modulus) {
  return (value + modulus) % modulus;
}

/// Where the first party figure stands, over its Djinn column.
#define DJINN_MEMBER_FIGURE_X 32
#define DJINN_MEMBER_FIGURE_Y 61

/// Places the four party figures over their Djinn columns, at screen y
/// DJINN_MEMBER_FIGURE_Y plus yOffset. Returns that y.
s32 Menu_SetFirstActorRowCoordinates(s32 yOffset) {
  struct DjinnMenuState *menu;
  s32 memberX;
  s32 memberIndex;
  menu = gMenuState;
  yOffset += DJINN_MEMBER_FIGURE_Y;
  memberX = DJINN_MEMBER_FIGURE_X;
  for (memberIndex = 0; memberIndex < 4; memberIndex++) {
    menu->memberX[memberIndex] = memberX;
    menu->memberY[memberIndex] = yOffset;
    memberX += DJINN_COLUMN_WIDTH;
  }
  return yOffset;
}

/// Runs the Djinn menu over the field: the field menu's Djinn screen
/// (FieldMenu_Run, FIELD_MENU_DJINN), and the Djinn tutorial, which the world
/// map opens when Flint joins.
s32 DjinnMenu_Run(void) {
  struct DjinnMenuState *menu;
  u32 savedMessageSpeed;
  struct DjinnMenuBackdrop *backdrop;

  menu = HeapSlot_Alloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
  savedMessageSpeed = gMainRoutineState.messageSpeed;
  gMainRoutineState.messageSpeed = MESSAGE_SPEED_FAST;
  gActorSystemState->suspended = 1;
  Window_DrawBorderFar(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
  wait(1);
  Window_InitializeState(0);
  backdrop = Heap_AllocEwram(sizeof(*backdrop));
  menu->backdrop = backdrop;
  backdrop->tutorialTimer = 0;
  backdrop->tutorialStep = DJINN_TUTORIAL_OFF;
  // The tutorial's lesson, from the flags the world map sets for each.
  if (GameFlag_GetFar(FLAG_DJINN_LESSON)) {
    if (!GameFlag_GetFar(FLAG_DJINN_LESSON_PART_A)) {
      if (!GameFlag_GetFar(FLAG_DJINN_LESSON_PART_B))
        backdrop->tutorialStep = DJINN_TUTORIAL_GIVE;
      else
        backdrop->tutorialStep = DJINN_TUTORIAL_POWERS;
    } else {
      if (!GameFlag_GetFar(FLAG_DJINN_LESSON_PART_B))
        backdrop->tutorialStep = DJINN_TUTORIAL_RECOVERY;
      else
        backdrop->tutorialStep = DJINN_TUTORIAL_CLOSE;
    }
  }
  Menu_SuspendFieldTasks();
  Text_UseExtraTilesFar(1);
  Link_DrawPageArrowsFar(PAGE_ARROW_VRAM);
  menu->memberCount = Party_ListMembersFar(menu->members);
  Resource_LoadPairedBlocksIfAvailable();
  ItemMenu_Initialize(0, 3, 0, 7);
  Menu_SetFirstActorRowCoordinates(0);
  Palette_LightenHighlight(HIGHLIGHT_PALETTE);
  menu->helpWindow = Window_CreateFar(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
  menu->pairEntries[0] = DJINN_ENTRY_NONE;
  menu->selectedColumn[0] = 0;
  menu->selectedColumn[1] = 0;
  menu->selection[0] = 0;
  menu->selection[1] = 0;
  DjinnFigures_InitializeBottomRow((s32)menu->helpWindow, 0);
  DjinnMenu_RunScreen();
  DjinnFigures_ReleaseBottomRow();
  IconMenu_ReleaseArrows();
  wait(1);
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
  gActorSystemState->suspended = 0;
  Window_MarkVisibleTextTilesFar();
  Text_UseExtraTilesFar(0);
  Iwram_CopyWords(BG_CHAR_BLOCK(1), backdrop->tiles, sizeof(backdrop->tiles));
  Iwram_CopyWords((void *)&BG_PLTT_COLOR(DJINN_BACKDROP_PALETTE, 0),
                  backdrop->palette, sizeof(backdrop->palette));
  wait(1);
  Menu_ResumeFieldTasks();
  Window_EraseBorderRectFar(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
  Heap_Release(menu->backdrop);
  HeapSlot_Release(HEAP_SLOT_MENU);
  Party_ClearUnusableShortcutsFar();
  gMainRoutineState.messageSpeed = savedMessageSpeed;
  return 1;
}

/// Finds the moved Djinni in the list and selects its new position.
#define DJINN_RESELECT_PAIR_ENTRY(menu, i)                                     \
  do {                                                                         \
    s32 column;                                                                \
    s32 found;                                                                 \
    u8 entry;                                                                  \
                                                                               \
    column = (u16)((u32)(menu)->selection[1] % DJINN_LIST_ROWS);               \
    found = 0;                                                                 \
    entry = (menu)->pairEntries[0];                                            \
    for ((i) = 0; (i) < (menu)->backdrop->list.counts[column]; (i)++) {        \
      if (entry == (u8)(menu)->backdrop->list.ids[column][(i)]) {              \
        found = (i);                                                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    (menu)->selection[0] = column + found * DJINN_LIST_ROWS;                   \
  } while (0)

/// The screen chooses an operation, optionally previews it, then commits it.
enum DjinnMenuStep {
  /// After a chooser: a negative result closes the screen, any other goes back
  /// to choosing a Djinni.
  DJINN_STEP_HANDLE_RESULT = 0,
  DJINN_STEP_RESERVED = 1, ///< nothing sets it
  DJINN_STEP_SELECT_DJINN = 2,
  DJINN_STEP_SELECT_ACTION = 3, ///< what to do with the chosen Djinni
  /// After a second choice: back to the Djinn, or on to a trade's second
  /// Djinni or a gift's confirmation.
  DJINN_STEP_HANDLE_PAIR_RESULT = 4,
  DJINN_STEP_CONFIRM_SET = 5,
  DJINN_STEP_CONFIRM_STANDBY = 6,
  DJINN_STEP_CONFIRM_GIVE = 7,
  DJINN_STEP_SELECT_SECOND = 8, ///< the other Djinni of a trade
  DJINN_STEP_CONFIRM_TRADE = 9,
  DJINN_STEP_SHOW_STATUS =
      10, ///< the status of the chosen column's party member
  DJINN_STEP_APPLY_SET = 11,
  DJINN_STEP_APPLY_STANDBY = 12,
  DJINN_STEP_APPLY_GIVE = 13,
  DJINN_STEP_APPLY_TRADE = 14,
  DJINN_STEP_SHOW_HELP = 15
};

/// Chooses a Djinni, previews changes, then sets, stands by, gives or trades
/// it.
s32 DjinnMenu_RunScreen(void) {
  struct DjinnMenuState *menu;
  struct WindowSprite *cursor;
  s32 result;
  s32 step;
  s32 menuResult;
  s32 done;
  s32 listIndex;

  done = 0;
  result = 0;
  menuResult = 0;
  menu = gMenuState;
  cursor = menu->cursor;
  cursor->motion = ICON_HIDDEN;
  cursor->animationStep = 0;
  Menu_OpenBackdropScreen();
  wait(1);
  step = DJINN_STEP_SELECT_DJINN;
  do {
    switch (step) {
    case DJINN_STEP_HANDLE_RESULT:
      if (result < 0) {
        menuResult = -1;
        done = 1;
      }
      step = DJINN_STEP_SELECT_DJINN;
      break;
    case DJINN_STEP_RESERVED:
      break;
    case DJINN_STEP_SELECT_DJINN:
      Menu_SetFirstActorRowCoordinates(0);
      DjinnFigures_SetSlotPosition(1, 0, MENU_FIGURE_HIDDEN_Y, 0);
      result = DjinnMenu_SelectDjinn(DJINN_SELECT_DJINNI);
      step = DJINN_STEP_SHOW_HELP;
      if (result == DJINN_RESULT_HELP)
        break;
      step = DJINN_STEP_HANDLE_RESULT;
      if (result < 0)
        break;
      menu->selection[1] = menu->selectedColumn[0];
      step = DJINN_STEP_SHOW_STATUS;
      if (result == DJINN_RESULT_STATUS)
        break;
      step = DJINN_STEP_SELECT_ACTION;
      break;
    case DJINN_STEP_SHOW_STATUS:
      menu->member = menu->members[menu->selectedColumn[0]];
      menu->shownMember[0] = menu->members[menu->selectedColumn[0]];
      result = DjinnMenu_ShowMemberStatus();
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_DJINN;
      break;
    case DJINN_STEP_SHOW_HELP:
      result = DjinnMenu_ShowHelp();
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_DJINN;
      break;
    case DJINN_STEP_SELECT_SECOND:
      step = DJINN_STEP_HANDLE_RESULT;
      if (menu->pairCount == 0)
        break;
      result = DjinnMenu_SelectDjinn(DJINN_SELECT_ACTION);
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_HANDLE_PAIR_RESULT;
      if (result < 0)
        break;
      step = DJINN_STEP_CONFIRM_TRADE;
      break;
    // Choose what to do with it. The figures go up 8 pixels while
    // this chooser runs.
    case DJINN_STEP_SELECT_ACTION:
      DjinnMenu_DrawElementList(&menu->backdrop->list);
      Menu_SetFirstActorRowCoordinates(-8);
      menu->member = menu->members[menu->selectedColumn[0]];
      menu->shownMember[0] = menu->members[menu->selectedColumn[0]];
      DjinnFigures_SetSlotPosition(0, DJINN_FIGURE_X(menu->selectedColumn[0]),
                                   54, 0);
      result = DjinnMenu_SelectDjinn(DJINN_SELECT_ACTION);
      for (listIndex = 0; listIndex < menu->memberCount; listIndex++)
        menu->memberY[listIndex] += 8;
      if (result == DJINN_RESULT_START)
        done = 1;
      if (result < 0) {
        step = DJINN_STEP_SELECT_DJINN;
        break;
      }
      if (result == DJINN_RESULT_GIVE || result == DJINN_RESULT_TRADE ||
          result == DJINN_RESULT_GIVE_NOW || result == DJINN_RESULT_TRADE_NOW)
        menu->shownMember[1] = menu->members[menu->selectedColumn[1]];
      if (result < 0)
        step = DJINN_STEP_SELECT_DJINN;
      else if (result == DJINN_RESULT_SET)
        step = DJINN_STEP_CONFIRM_SET;
      else if (result == DJINN_RESULT_STANDBY)
        step = DJINN_STEP_CONFIRM_STANDBY;
      else if (result == DJINN_RESULT_GIVE) {
        menu->pending = DJINN_PENDING_CONFIRM;
        step = DJINN_STEP_CONFIRM_GIVE;
      } else if (result == DJINN_RESULT_TRADE) {
        menu->pending = DJINN_PENDING_CONFIRM;
        step = DJINN_STEP_CONFIRM_TRADE;
      } else if (result == DJINN_RESULT_SET_NOW)
        step = DJINN_STEP_APPLY_SET;
      else if (result == DJINN_RESULT_STANDBY_NOW)
        step = DJINN_STEP_APPLY_STANDBY;
      else if (result == DJINN_RESULT_GIVE_NOW) {
        menu->pending = DJINN_PENDING_CONFIRM;
        step = DJINN_STEP_APPLY_GIVE;
      } else if (result == DJINN_RESULT_TRADE_NOW) {
        menu->pending = DJINN_PENDING_CONFIRM;
        step = DJINN_STEP_APPLY_TRADE;
      }
      break;
    case DJINN_STEP_CONFIRM_GIVE:
      result = PsynergyList_RunCompareLoop(DJINN_COMPARE_GIVE);
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_ACTION;
      if (result < 0)
        break;
      // Confirmed: apply it at once.
      // Fall through.
    case DJINN_STEP_APPLY_GIVE:
      Sound_PlayFar(SE_RECOVERY);
      result = Djinn_TransferFar(menu->shownMember[0], menu->pairElement[0],
                                 menu->pairIndex[0], menu->shownMember[1]);
      Unit_CalculateFar(menu->shownMember[0]);
      Unit_CalculateFar(menu->shownMember[1]);
      menu->cursor->motion = ICON_HIDDEN;
      WindowSprite_ClearListFar(menu->listWindow);
      DjinnMenu_BuildLists(menu->backdrop);
      DJINN_RESELECT_PAIR_ENTRY(menu, listIndex);
      menu->cursor->motion = ICON_SHOWN;
      step = DJINN_STEP_HANDLE_RESULT;
      break;
    case DJINN_STEP_CONFIRM_STANDBY:
      result = PsynergyList_RunCompareLoop(DJINN_COMPARE_STANDBY);
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_ACTION;
      if (result < 0)
        break;
      // Confirmed: apply it at once.
      // Fall through.
    case DJINN_STEP_APPLY_STANDBY:
      Sound_PlayFar(SE_NOTE_HIGH);
      Djinn_PutOnStandbyFar(menu->shownMember[0], menu->pairElement[0],
                            menu->pairIndex[0]);
      result = (s32)DjinnRecovery_AddStandbyFar(
          menu->shownMember[0], menu->pairElement[0], menu->pairIndex[0]);
      Unit_CalculateFar(menu->shownMember[0]);
      menu->cursor->motion = ICON_HIDDEN;
      WindowSprite_ClearListFar(menu->listWindow);
      menu->cursor->motion = ICON_SHOWN;
      step = DJINN_STEP_SELECT_DJINN;
      break;
    case DJINN_STEP_CONFIRM_TRADE:
      result = PsynergyList_RunCompareLoop(DJINN_COMPARE_TRADE);
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_ACTION;
      if (result < 0)
        break;
      // Confirmed: apply it at once.
      // Fall through.
    case DJINN_STEP_APPLY_TRADE:
      Sound_PlayFar(SE_RECOVERY);
      Djinn_TransferFar(menu->shownMember[0], menu->pairElement[0],
                        menu->pairIndex[0], menu->shownMember[1]);
      result = Djinn_TransferFar(menu->shownMember[1], menu->pairElement[1],
                                 menu->pairIndex[1], menu->shownMember[0]);
      Unit_CalculateFar(menu->shownMember[0]);
      Unit_CalculateFar(menu->shownMember[1]);
      DjinnMenu_BuildLists(menu->backdrop);
      DJINN_RESELECT_PAIR_ENTRY(menu, listIndex);
      menu->cursor->motion = ICON_SHOWN;
      step = DJINN_STEP_SELECT_DJINN;
      break;
    case DJINN_STEP_HANDLE_PAIR_RESULT:
      if (result == DJINN_RESULT_BACK) {
        menuResult = result;
        step = DJINN_STEP_SELECT_DJINN;
      } else if (menu->pending & DJINN_PENDING_SECOND)
        step = DJINN_STEP_SELECT_SECOND;
      else if (menu->pending & DJINN_PENDING_CONFIRM)
        step = DJINN_STEP_CONFIRM_GIVE;
      break;
    case DJINN_STEP_CONFIRM_SET:
      result = PsynergyList_RunCompareLoop(DJINN_COMPARE_SET);
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_ACTION;
      if (result < 0)
        break;
      // Confirmed: apply it at once.
      // Fall through.
    case DJINN_STEP_APPLY_SET:
      Sound_PlayFar(SE_DJINNI_SET);
      Djinn_SetFar(menu->shownMember[0], menu->pairElement[0],
                   menu->pairIndex[0]);
      result = DjinnRecovery_RemoveFar(
          menu->shownMember[0], menu->pairElement[0], menu->pairIndex[0]);
      Unit_CalculateFar(menu->shownMember[0]);
      menu->cursor->motion = ICON_HIDDEN;
      WindowSprite_ClearListFar(menu->listWindow);
      menu->cursor->motion = ICON_SHOWN;
      step = DJINN_STEP_SELECT_DJINN;
      break;
    default:
      done = 1;
      break;
    }
  } while (done == 0);
  return menuResult;
}

/// Writes copies of WINDOW_PALETTE, BASIC_COLORS_PALETTE and
/// DJINN_WINDOW_COPY_PALETTE, each brightnessDelta lighter in every channel
/// (clamped to 0 to RGB_LEVEL_MAX), into the palette before each:
/// HIGHLIGHT_PALETTE, DJINN_BACKDROP_PALETTE and DJINN_DIM_PALETTE. The last
/// copy is 12 levels darker instead.
void DjinnMenu_AdjustPalettes(s32 brightnessDelta) {
  s32 copyIndex;
  s32 paletteNumber;
  s32 colorIndex;
  s32 paletteIndex;

  paletteNumber = WINDOW_PALETTE;
  for (copyIndex = 0; copyIndex <= 2; copyIndex++) {
    for (colorIndex = 0; colorIndex < PALETTE_COLORS; colorIndex++) {
      u32 color;
      s32 blue;
      s32 green;
      s32 red;
      s32 adjustedColor;

      paletteIndex = paletteNumber * PALETTE_COLORS + colorIndex;
      color = ((u16 *)BG_PLTT)[paletteIndex];
      blue = RGB_BLUE(color);
      green = RGB_GREEN(color);
      red = RGB_RED(color);
      blue += brightnessDelta;
      green += brightnessDelta;
      red += brightnessDelta;
      if (blue > RGB_LEVEL_MAX)
        blue = RGB_LEVEL_MAX;
      if (green > RGB_LEVEL_MAX)
        green = RGB_LEVEL_MAX;
      if (red > RGB_LEVEL_MAX)
        red = RGB_LEVEL_MAX;
      if (blue < 0)
        blue = 0;
      if (green < 0)
        green = 0;
      if (red < 0)
        red = 0;
      adjustedColor = RGB(red, green, blue);
      ((s16 *)BG_PLTT - PALETTE_COLORS)[paletteIndex] = adjustedColor;
    }
    if (copyIndex == 0) {
      paletteNumber = BASIC_COLORS_PALETTE;
    } else {
      paletteNumber = DJINN_WINDOW_COPY_PALETTE;
      brightnessDelta = -12;
    }
  }
}

/// PsynergyList_Diff's marks on an action id: Psynergy the change gives,
/// drawn in TEXT_COLOR_YELLOW, and Psynergy it takes away, in TEXT_COLOR_RED.
#define DJINN_PREVIEW_GAINED 0x8000
#define DJINN_PREVIEW_LOST 0x4000

/// Draws the backdrop behind the Djinn menu's list: its window, tiles and
/// palettes.
MenuBackdropResult Menu_OpenBackdropScreen(void) {
  // UB: the implicit int call conflicts with DrawElementList's void definition.
  // The sole caller ignores this wrapper's result.
  struct DjinnMenuState *menu = gMenuState;
  struct DjinnMenuBackdrop *backdrop = menu->backdrop;

  Window_UpdateOrCreate(&menu->listWindow, 0, 5, 30, 15, WINDOW_FLAG_INSTANT);
  wait(1);
  Iwram_CopyWords(backdrop->tiles, BG_CHAR_BLOCK(1), sizeof(backdrop->tiles));
  Iwram_CopyWords(backdrop->palette,
                  (void *)&BG_PLTT_COLOR(DJINN_BACKDROP_PALETTE, 0),
                  sizeof(backdrop->palette));
  Iwram_FillWords(BG_CHAR_BLOCK(1), sizeof(backdrop->tiles), PIXELS_4BPP(3));
  // Every colour of the backdrop's palettes RGB(21, 10, 21), two to a word.
  Iwram_FillWords((void *)&BG_PLTT_COLOR(DJINN_BACKDROP_PALETTE, 0),
                  sizeof(backdrop->palette),
                  RGB(21, 10, 21) << 16 | RGB(21, 10, 21));
  Graphics_ExpandVramTilesByColorTableFar(
      (u16 *)(BG_CHAR_BLOCK(1) + DJINN_BACKDROP_DIGIT_TILE * TILE_SIZE_4BPP));
  Iwram_CopyWords(BG_CHAR_BLOCK(1) + DJINN_CHANGE_ARROW_TILE * TILE_SIZE_4BPP,
                  gMenu_ChangeArrowTile, TILE_SIZE_4BPP);
  Dma_Set(Palette_GetBasicSpriteColors(),
          (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  Dma_Set((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0),
          (void *)&BG_PLTT_COLOR(DJINN_WINDOW_COPY_PALETTE, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  DjinnMenu_AdjustPalettes(8);
  BG_PLTT_COLOR(DJINN_WINDOW_COPY_PALETTE, WINDOW_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  BG_PLTT_COLOR(DJINN_DIM_PALETTE, WINDOW_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
#ifdef UBFIX
  DjinnMenu_DrawElementList(&backdrop->list);
#else
  return DjinnMenu_DrawElementList(&backdrop->list);
#endif
}

/// Lists the actions after a preview, marking gains, then appends the losses.
/// The two output counts distinguish gained and lost actions.
s32 PsynergyList_Diff(struct PsynergyEntry *before, struct PsynergyEntry *after,
                      u16 *actionsOut, s32 *gainedCountOut, s32 *lostCountOut) {
  s32 actionIndex;
  s32 otherIndex;
  s32 actionCount;
  s32 gainedCount;
  s32 lostCount;

  actionCount = 0;
  gainedCount = 0;
  lostCount = 0;
  for (actionIndex = 0;
       actionIndex < PSYNERGY_SLOTS && after[actionIndex].encodedAction != 0;
       actionIndex++) {
    actionsOut[actionCount] =
        after[actionIndex].encodedAction & PSYNERGY_ABILITY_MASK;
    actionCount++;
    for (otherIndex = 0; otherIndex < PSYNERGY_SLOTS; otherIndex++) {
      if (((after[actionIndex].encodedAction ^
            before[otherIndex].encodedAction) &
           PSYNERGY_ABILITY_MASK) == 0)
        break;
    }
    if (otherIndex == PSYNERGY_SLOTS) {
      gainedCount++;
      actionsOut[actionCount - 1] |= DJINN_PREVIEW_GAINED;
    }
  }
  for (actionIndex = 0;
       actionIndex < PSYNERGY_SLOTS && before[actionIndex].encodedAction != 0;
       actionIndex++) {
    for (otherIndex = 0; otherIndex < PSYNERGY_SLOTS; otherIndex++) {
      if (((before[actionIndex].encodedAction ^
            after[otherIndex].encodedAction) &
           PSYNERGY_ABILITY_MASK) == 0)
        break;
    }
    if (otherIndex == PSYNERGY_SLOTS) {
      lostCount++;
      actionsOut[actionCount] =
          (before[actionIndex].encodedAction & PSYNERGY_ABILITY_MASK) |
          DJINN_PREVIEW_LOST;
      actionCount++;
    }
  }
  *gainedCountOut = gainedCount;
  *lostCountOut = lostCount;
  return actionCount;
}

/// Rebuilds the Djinn lists in table: each party member's packed Djinn entries
/// and their count.
s32 DjinnMenu_BuildLists(void *table) {
  struct DjinnMenuState *menu;
  s32 memberIndex;
  u16 (*entries)[DJINN_LIST_ROWS];
  u16 *members;
  s8 *counts;
  s16 entryCount;
  s32 memberCount;

  menu = gMenuState;
  memberIndex = 0;
  if (memberIndex < menu->memberCount) {
    counts = ((struct DjinnListTable *)table)->counts;
    members = menu->members;
    entries = ((struct DjinnListTable *)table)->ids;
    do {
      entryCount = Djinn_ListUnitEntries(*entries, *members, -1);
      memberCount = menu->memberCount;
      memberIndex += 1;
      *counts = entryCount;
      members += 1;
      counts += 1;
      entries++;
    } while (memberIndex < memberCount);
  }
#ifdef UBFIX
  return 0;
#endif
}

/// Draws each party member's Djinn column, including set and recovering
/// entries. Rebuilds table's lists first.
void DjinnMenu_DrawElementList(struct DjinnListTable *table) {
  struct DjinnMenuState *menu;
  s32 listIndex;
  s32 row;
  s32 element;
  struct WindowState *state;
  s32 lineIndex;
  s32 canChangeState;
  u16 *entries;

  menu = gMenuState;
  state = HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW);
  state->holdTilemapUpload = 1;
  listIndex = 0;
  if (menu->memberCount != 0) {
    do {
      table->counts[listIndex] = Djinn_ListUnitEntries(
          table->ids[listIndex], menu->members[listIndex], -1);
      listIndex++;
    } while (listIndex < menu->memberCount);
  }
  WindowSprite_RedrawSavedRectFar(menu->listWindow);
  Text_DrawMessageAsTilesFar(MsgUnleashEffect, menu->listWindow, 0, 80);
  for (row = 0; row < menu->memberCount; row++) {
    entries = table->ids[row];
    lineIndex = 0;
    for (element = 0; element < DJINN_ELEMENTS; element++) {
      for (listIndex = 0; listIndex < table->counts[row]; listIndex++) {
        if (element != DJINN_ELEMENT(entries[listIndex]))
          continue;
        // TEXT_COLOR_RED when not set; TEXT_COLOR_YELLOW over it when it is
        // neither set nor ready to set, that is recovering (or held by a
        // summon).
        if ((entries[listIndex] & DJINN_ENTRY_IS_SET) == 0)
          Text_SetColorFar(TEXT_COLOR_RED);
        canChangeState = 0;
        if (Djinn_CanSetFar(DJINN_MEMBER(entries[listIndex]),
                            DJINN_ELEMENT(entries[listIndex]),
                            DJINN_NUMBER(entries[listIndex])) ||
            Djinn_IsSetFar(DJINN_MEMBER(entries[listIndex]),
                           DJINN_ELEMENT(entries[listIndex]),
                           DJINN_NUMBER(entries[listIndex])))
          canChangeState = 1;
        if (!canChangeState)
          Text_SetColorFar(TEXT_COLOR_YELLOW);
        Window_SetTilemapEntryFar(
            menu->listWindow,
            DJINN_ELEMENT(entries[listIndex]) + DJINN_ELEMENT_TILE,
            row * DJINN_COLUMN_TILES + 1, lineIndex + 2, TILEMAP_ENTRY_PLAIN);
        Text_DrawMessageAsTilesFar(
            MsgDjinnName +
                DJINN_ELEMENT(entries[listIndex]) * DJINN_PER_ELEMENT +
                DJINN_NUMBER(entries[listIndex]),
            menu->listWindow, row * DJINN_COLUMN_WIDTH + 16,
            lineIndex * 8 + 16);
        lineIndex++;
        Text_SetColorFar(TEXT_COLOR_NORMAL);
      }
    }
  }
  Window_DrawDividerLineFar(menu->listWindow, 0, 10, 28, 10);
  WINDOW_STATE->dirty = 1;
  state->holdTilemapUpload = 0;
}

/// Sets the palette of a rectangle of window's interior, (x, y) and its size in
/// tiles from the interior's corner, through Window_HighlightRectFar.
s32 Window_SetInteriorPaletteBitRect(const struct Window *window, s32 x, s32 y,
                                     s32 width, s32 height, s32 palette) {
  Window_HighlightRectFar(window->x + x + 1, window->y + y + 1, width, height,
                          palette);
#ifdef UBFIX
  return 0;
#endif
}

/// Moves each tile of a screen rectangle (in tiles) that shows WINDOW_PALETTE
/// to palette, and marks each row changed for upload.
void Window_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette) {
  struct WindowState *state;
  u16 *tileCursor;
  s32 column;
  u32 tile;
  u32 masked;

  state = WINDOW_STATE;
  palette = BG_TILE_PALETTE(palette);
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > DISPLAY_TILE_WIDTH - 1)
    width = DISPLAY_TILE_WIDTH - x;
  if (y < 0) {
    height += y;
    y = 0;
  }
  // BUG: tests the bottom against the screen's width but clips to its rows, so
  // a rectangle ending in rows 20 to 28 is kept whole and recolours past the
  // 32 by 20 tilemap.
#ifdef BUGFIX
  if (y + height > DISPLAY_TILE_HEIGHT - 1)
#else
  if (y + height > DISPLAY_TILE_WIDTH - 1)
#endif
    height = DISPLAY_TILE_HEIGHT - y;

  if (width > 0 && height > 0) {
    do {
      tileCursor = &((u16 (*)[BG_SCREEN_WIDTH])state->tilemap)[y][x];
      for (column = width; column != 0; column--) {
        tile = *tileCursor;
        if (BG_TILE_PALETTE_OF(tile) == WINDOW_PALETTE) {
          masked = tile & ~BG_TILE_PALETTE(0xf);
          *tileCursor = masked | palette;
        }
        tileCursor++;
      }
      Window_MarkTilemapRow(state, y);
      height--;
      y++;
    } while (height != 0);
  }
}

/// Window_SetRectPalette on a rectangle of window's interior, (x, y) and its
/// size in tiles from the interior's corner.
void Window_SetInteriorRectPalette(void *window, s32 x, s32 y, s32 width,
                                   s32 height, s32 palette) {
  Window_SetRectPalette(((struct Window *)window)->x + x + 1,
                        ((struct Window *)window)->y + y + 1, width, height,
                        palette);
}

/// The help's topics: Djinn, Set, Standby, Summon, Recovery, Class and
/// Psynergy.
#define DJINN_HELP_TOPICS 7

/// The Djinn menu's help, opened by Select on the first chooser: the list of
/// topics with the chosen one's text typed beside it.
s32 DjinnMenu_ShowHelp(void) {
  struct Window *helpWindow;
  struct Window *statusWindow;
  struct DjinnMenuState *menu;
  struct WindowState *windowState;
  s32 result;
  s32 previous;
  s32 selection;
  struct Window *list;
  struct Message *typing;
  s32 message;
  s32 helpIndex;
  s32 listMessage;

  menu = gMenuState;
  windowState = HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW);
  result = 0;
  previous = 0;
  selection = 0;
  WindowSprite_ClearListFar(menu->listWindow);
  wait(1);
  WindowSprite_PrepareForRedrawFar(menu->helpWindow);
  message = MsgDjinnInfoPrompt;
  Text_DrawMessageAsTilesFar(message, menu->helpWindow, 0, 0);
  message++;
  Text_DrawMessageAsTilesFar(message, menu->helpWindow, 0, 16);
  // Dim the party window and the Djinn list behind the help.
  Window_SetRectPalette(1, 1, 11, 3, DJINN_DIM_PALETTE);
  Window_SetInteriorRectPalette((void *)menu->listWindow, 0, 0, 28, 10,
                                DJINN_DIM_PALETTE);
  list = Window_CreateFar(0, 9, 8, 10, 6);
  helpWindow = Window_CreateFar(8, 12, 22, 7, WINDOW_FLAG_INSTANT);
  statusWindow = Window_CreateFar(8, 9, 22, 3, WINDOW_FLAG_INSTANT);
  Window_MarkVisibleTextTilesFar();
  for (helpIndex = 0, listMessage = MsgDjinnInfoTopic;
       helpIndex < DJINN_HELP_TOPICS; helpIndex++)
    Text_DrawMessageAsTilesFar(helpIndex + listMessage, list, 0, helpIndex * 8);
  do {
    WindowSprite_PrepareForRedrawFar(statusWindow);
    Text_DrawMessageFar(selection + MsgDjinnInfoTopic, statusWindow, 0, 0);
    typing = (struct Message *)Message_OpenDecodedFar(
        helpWindow, selection + MsgDjinnInfoText);
    Window_SetInteriorPaletteBitRect(list, 0, previous, 6, 1, WINDOW_PALETTE);
    Window_SetInteriorPaletteBitRect(list, 0, selection, 6, 1,
                                     HIGHLIGHT_PALETTE);
    previous = selection;
    for (;;) {
      MenuCursor_Position(-12, (list->y + selection) * 8 + 8);
      wait(1);
      if (gKeysRepeat & (KEY_RIGHT | KEY_DOWN)) {
        selection++;
        selection = Menu_WrapIndex(selection, DJINN_HELP_TOPICS);
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        break;
      }
      if (gKeysRepeat & (KEY_LEFT | KEY_UP)) {
        selection--;
        selection = Menu_WrapIndex(selection, DJINN_HELP_TOPICS);
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        break;
      } else if (gKeysPressed & KEY_START) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = DJINN_RESULT_START;
        break;
      } else if (gKeysPressed & (KEY_B | KEY_SELECT)) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = DJINN_RESULT_BACK;
        break;
      } else if (gKeysPressed & KEY_A) {
        if (Message_AllTypedFar()) {
          selection++;
          selection = Menu_WrapIndex(selection, DJINN_HELP_TOPICS);
          Sound_PlayFar(SE_MENU_CONFIRM);
          break;
        }
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      }
    }
    // Before the next topic: free the page arrow's VRAM slot the text took,
    // and stop its typing.
    if (windowState->glyphResource != NO_GLYPH_RESOURCE) {
      VramCache_FreeEntry(windowState->glyphResource);
      windowState->glyphResource = NO_GLYPH_RESOURCE;
    }
    WINDOW_STATE->pageArrowShown = 0;
    WindowSprite_PrepareForRedrawFar(helpWindow);
    {
      struct Window *window = typing->window;

      window->zoomStage = 0;
      window->duration = 0;
      window->state = 0;
    }
    typing->window = NULL;
  } while (result == 0);
  WINDOW_STATE->holdTilemapUpload = 1;
  WindowSprite_ClearListFar(statusWindow);
  WindowSprite_ClearListFar(helpWindow);
  wait(1);
  Window_CloseFar(statusWindow, 1);
  Window_CloseFar(list, 1);
  Window_CloseFar(helpWindow, 1);
  Window_MarkVisibleTextTilesFar();
  if (result == DJINN_RESULT_START) {
    WindowSprite_PrepareForRedrawFar(menu->helpWindow);
    WindowSprite_PrepareForRedrawFar(menu->listWindow);
    WindowSprite_PrepareForRedrawFar(menu->statusWindow);
    WINDOW_STATE->holdTilemapUpload = 0;
  }
  Task_Add(Menu_PlacePartyFigures, TASK_PASS_FRAME);
  return result;
}

/// DJINN_ENTRY_IS_SET read as a signed halfword, for entries built in s32.
#define DJINN_ENTRY_SET ((s16)DJINN_ENTRY_IS_SET)

/// The flags DjinnMenu_SelectDjinn keeps for each party member's column.
enum DjinnColumnFlag {
  DJINN_COLUMN_ON_HEADER = 1,        ///< the cursor is on the column's header
  DJINN_COLUMN_HEADER_CHOOSABLE = 2, ///< its header can be chosen
  DJINN_COLUMN_EMPTY = 4             ///< no Djinni to choose
};

/// What DjinnMenu_SelectDjinn redraws: everything, or after a cursor move,
/// which also makes the Djinni figure grow in again.
enum DjinnRedraw { DJINN_REDRAW_ALL = 1, DJINN_REDRAW_MOVED = 2 };

/// The Djinn list's chooser. DJINN_SELECT_DJINNI picks a Djinni in any party
/// member's column: R sets it or puts it on standby at once, R held with Select
/// does so for every Djinni, L shows the member's status and Select the help.
/// DJINN_SELECT_ACTION picks what to do with it, by the column chosen. While
/// the tutorial runs, its steps press the buttons.
s32 DjinnMenu_SelectDjinn(s32 mode) {
  s8 columnFlags[DJINN_LIST_MEMBERS];
  u8 requiresTrade[16];
  struct DjinnMenuState *menu = gMenuState;
  struct DjinnMenuBackdrop *backdrop = menu->backdrop;
  struct Unit *unit;
  struct Window *window;
  s32 redraw = DJINN_REDRAW_ALL;
  u32 refresh;
  s32 i;
  s32 j;
  s32 selectedRow;
  s32 memberColumn = (u16)(menu->selection[mode] % DJINN_CURSOR_ROW_STRIDE);
  s32 djinnRow = (u16)(menu->selection[mode] / DJINN_CURSOR_ROW_STRIDE);
  s32 savedRow = 0;
  u32 djinn = 0;
  s32 groupMode = 0;
  s32 setAll = 0;
  s32 result;
  s32 message;
  u32 pressed;
  s32 count;
  u32 repeat;
  struct Window *messageWindow;
  s32 tutorialStep;
  s8 *status;

  // Each column's DJINN_COLUMN_ flags.
  selectedRow = -1;
  status = columnFlags;
  for (i = 0; i < DJINN_LIST_MEMBERS; i++)
    status[i] = 0;
  if (mode == DJINN_SELECT_DJINNI) {
    DjinnMenu_DrawElementList(&backdrop->list);
    for (i = 0; i < menu->memberCount; i++) {
      if (backdrop->list.counts[i] == 0)
        status[i] = DJINN_COLUMN_EMPTY;
    }
    savedRow = djinnRow;
    for (i = 0; i < menu->memberCount; i++) {
      if (status[memberColumn] == DJINN_COLUMN_EMPTY) {
        memberColumn++;
        memberColumn = Menu_WrapIndex(memberColumn, menu->memberCount);
      }
    }
  } else {
    Djinn_MarkTradeOnlyRecipients((s8 *)requiresTrade, menu->selectedColumn[0]);
    for (i = 0; i < menu->memberCount; i++) {
      if (i == menu->selectedColumn[0]) {
        columnFlags[i] = DJINN_COLUMN_EMPTY | DJINN_COLUMN_HEADER_CHOOSABLE |
                         DJINN_COLUMN_ON_HEADER;
      } else if (requiresTrade[i] != 0) {
        columnFlags[i] = 0;
      } else {
        columnFlags[i] = DJINN_COLUMN_HEADER_CHOOSABLE | DJINN_COLUMN_ON_HEADER;
        if (backdrop->list.counts[i] == 0)
          status[i] = columnFlags[i] | DJINN_COLUMN_EMPTY;
      }
    }
  }
  // DJINN_SELECT_ACTION: keep the first Djinni highlighted, dim the rest of its
  // column, and label each column's header with what choosing it does.
  if (mode == DJINN_SELECT_ACTION) {
    s32 fromX;
    s32 fromY;
    u16 sourceCursor;
    s32 n;
    sourceCursor = menu->selection[0];
    fromX = (u16)((u32)sourceCursor % (u32)DJINN_CURSOR_ROW_STRIDE);
    fromY = (u16)((u32)sourceCursor / (u32)DJINN_CURSOR_ROW_STRIDE);
    Window_SetInteriorPaletteBitRect(menu->listWindow,
                                     fromX * DJINN_COLUMN_TILES + 1, fromY + 2,
                                     DJINN_CURSOR_WIDTH, 1, HIGHLIGHT_PALETTE);
    Window_SetInteriorRectPalette(menu->listWindow,
                                  fromX * DJINN_COLUMN_TILES + 1, 2,
                                  DJINN_CURSOR_WIDTH, 7, DJINN_DIM_PALETTE);
    for (n = 0; n < menu->memberCount; n++) {
      if (n == menu->selectedColumn[0]) {
        if (menu->pairEntries[0] & DJINN_ENTRY_IS_SET)
          message = MsgDjinnStandbyAction;
        else
          message = MsgDjinnSetAction;
      } else if (columnFlags[n] & DJINN_COLUMN_HEADER_CHOOSABLE) {
        message = MsgDjinnGiveAction;
      } else {
        message = MsgDjinnTradeAction;
      }
      Text_DrawMessageAsTilesFar(message, menu->listWindow,
                                 n * DJINN_COLUMN_WIDTH + 8, 8);
    }
  }
  WindowSprite_RedrawSavedRectFar(menu->helpWindow);
  menu->cursor->motion = ICON_SHOWN;
  for (;;) {
    if (redraw != 0) {
      refresh = redraw;
      redraw = 0;
      selectedRow = -1;
      if (!(columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER))
        selectedRow = djinnRow;
      window = menu->statusWindow;
      unit = Unit_GetFar(menu->members[memberColumn]);
      WindowSprite_RedrawSavedRectFar(window);
      Text_DrawStringAtOffsetFar(unit->name, window, 0, 0);
      Text_DrawMessageAsTilesFar(unit->classId + MsgClassName, window, 0, 8);
      Text_DrawStringAtOffsetFar(gDjinnMenu_TextLevel, window, 48, 0);
      Text_DrawNumberInWindowFar(unit->level, 2, window, 72, 0);
      if (mode == DJINN_SELECT_DJINNI)
        Text_DrawMessageAsTilesFar(MsgDjinnCharacterStatus, window, 0, 16);
      if (selectedRow != -1)
        djinn = backdrop->list.ids[memberColumn][selectedRow];
      WindowSprite_RedrawSavedRectFar(menu->helpWindow);
      // DJINN_SELECT_ACTION's help: `<member>'s`, the element icon and the
      // Djinni's name, an ability name argument (its action, from
      // ABILITY_DJINNI_FIRST), then "What will you do?".
      if (mode == DJINN_SELECT_ACTION) {
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
        Window_SetTilemapEntryFar(menu->helpWindow,
                                  DJINN_ELEMENT(menu->pairEntries[0]) +
                                      DJINN_ELEMENT_TILE,
                                  0, 0, TILEMAP_ENTRY_PLAIN);
        MessageArgument_PushFar(menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_PushFar(
            DJINN_ELEMENT(menu->pairEntries[0]) * DJINN_PER_ELEMENT +
                DJINN_NUMBER(menu->pairEntries[0]) + ABILITY_DJINNI_FIRST,
            MESSAGE_ARGUMENT_ABILITY);
        Text_DrawMessageAsTilesFar(MsgDjinnOwnerLabel, menu->helpWindow, 8, 0);
#else
        MessageArgument_PushFar(menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
        Text_DrawMessageAsTilesFar(MsgDjinnOwnerLabel, menu->helpWindow, 0, 0);
        MessageArgument_PushFar(
            DJINN_ELEMENT(menu->pairEntries[0]) * DJINN_PER_ELEMENT +
                DJINN_NUMBER(menu->pairEntries[0]) + ABILITY_DJINNI_FIRST,
            MESSAGE_ARGUMENT_ABILITY);
        Window_SetTilemapEntryFar(menu->helpWindow,
                                  DJINN_ELEMENT(menu->pairEntries[0]) +
                                      DJINN_ELEMENT_TILE,
                                  6, 0, TILEMAP_ENTRY_PLAIN);
        Text_DrawMessageAsTilesFar(MsgDjinnOwnerLabel + 1, menu->helpWindow, 56,
                                   0);
#endif
        Text_DrawMessageAsTilesFar(MsgDjinnOwnerLabel + 2, menu->helpWindow, 0,
                                   8);
      }
      // The Djinni's figure, its element's: standing for one on standby or
      // recovering, walking for one that is set.
      if (selectedRow == -1) {
        DjinnFigures_SetSlotPosition(mode, 0, MENU_FIGURE_HIDDEN_Y, 0);
      } else if (mode == DJINN_SELECT_DJINNI) {
        if (groupMode) {
          if (setAll == 0) {
            Text_DrawMessageAsTilesFar(MsgDjinnSetAll, menu->helpWindow, 0, 0);
          } else {
            Text_DrawMessageAsTilesFar(MsgDjinnAllStandby, menu->helpWindow, 0,
                                       0);
          }
          if (Djinn_CanSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                              DJINN_NUMBER(djinn)) ||
              Djinn_IsSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                             DJINN_NUMBER(djinn))) {
            if (Djinn_CanSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                                DJINN_NUMBER(djinn)))
              DjinnFigures_ReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
            else
              DjinnFigures_ReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_WALK);
            DjinnFigures_SetSlotPosition(mode, DJINN_FIGURE_X(memberColumn), 62,
                                         0);
          } else {
            Text_DrawMessageAsTilesFar(MsgDjinnRecovering, menu->helpWindow, 0,
                                       16);
            DjinnFigures_ReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
            DjinnFigures_SetSlotPosition(mode, DJINN_FIGURE_X(memberColumn), 62,
                                         1);
          }
          if (refresh & DJINN_REDRAW_MOVED)
            DjinnFigures_SetSlotPhase(mode, 0);
        } else {
          Text_DrawMessageAsTilesFar(MsgDjinnChoose, menu->helpWindow, 0, 0);
          if (Djinn_CanSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                              DJINN_NUMBER(djinn)) ||
              Djinn_IsSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                             DJINN_NUMBER(djinn))) {
            if (Djinn_CanSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                                DJINN_NUMBER(djinn))) {
              Text_DrawMessageAsTilesFar(MsgDjinnChoose + 3, menu->helpWindow,
                                         0, 16);
              DjinnFigures_ReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
            } else {
              Text_DrawMessageAsTilesFar(MsgDjinnChoose + 2, menu->helpWindow,
                                         0, 16);
              DjinnFigures_ReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_WALK);
            }
            DjinnFigures_SetSlotPosition(mode, DJINN_FIGURE_X(memberColumn), 62,
                                         0);
          } else {
            Text_DrawMessageAsTilesFar(MsgDjinnChoose + 4, menu->helpWindow, 0,
                                       16);
            DjinnFigures_ReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
            DjinnFigures_SetSlotPosition(mode, DJINN_FIGURE_X(memberColumn), 62,
                                         1);
          }
          if (refresh & DJINN_REDRAW_MOVED)
            DjinnFigures_SetSlotPhase(mode, 0);
        }
      } else {
        if (Djinn_CanSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                            DJINN_NUMBER(djinn)) ||
            Djinn_IsSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                           DJINN_NUMBER(djinn))) {
          if (Djinn_CanSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                              DJINN_NUMBER(djinn)))
            DjinnFigures_ReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
          else
            DjinnFigures_ReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_WALK);
          DjinnFigures_SetSlotPosition(mode, DJINN_FIGURE_X(memberColumn), 54,
                                       0);
        } else {
          DjinnFigures_ReplaceSlot(mode, DJINN_ELEMENT(djinn), ANIM_STAND);
          DjinnFigures_SetSlotPosition(mode, DJINN_FIGURE_X(memberColumn), 54,
                                       1);
        }
        if (refresh & DJINN_REDRAW_MOVED)
          DjinnFigures_SetSlotPhase(mode, 0);
      }
      WindowSprite_ClearListFar(menu->listWindow);
      if (selectedRow != -1) {
        Text_DrawMessageAsTilesFar(MsgUnleashEffect, menu->listWindow, 0, 80);
#if LANGUAGE != LANGUAGE_JAPANESE
        Window_ClearInteriorTilesFar(menu->listWindow, 0, 96, 224, 104);
        Text_DrawMessageAsTilesFar(DJINN_ELEMENT(djinn) * DJINN_PER_ELEMENT +
                                       DJINN_NUMBER(djinn) + MsgDjinnEffect,
                                   menu->listWindow, 0, 96);
#else
        Text_DrawMessageFar(DJINN_ELEMENT(djinn) * DJINN_PER_ELEMENT +
                                DJINN_NUMBER(djinn) + MsgDjinnEffect,
                            menu->listWindow, 0, 96);
#endif
      }
      if (!(columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER))
        Window_SetInteriorPaletteBitRect(
            menu->listWindow, memberColumn * DJINN_COLUMN_TILES + 1,
            djinnRow + 2, DJINN_CURSOR_WIDTH, 1, HIGHLIGHT_PALETTE);
      WINDOW_STATE->dirty = 1;
    }
    if (columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER)
      MenuCursor_Position(memberColumn * DJINN_COLUMN_WIDTH - 8, 52);
    else
      MenuCursor_Position(memberColumn * DJINN_COLUMN_WIDTH - 8,
                          djinnRow * 8 + 60);
    wait(1);
    // Holding R after R on a Djinni keeps the group commands up; they
    // end when R is let go or pressed again (the latch is cleared when they
    // began).
    if (!(gKeysHeld & KEY_R) || (gKeysPressedLatch & KEY_R)) {
      if (groupMode)
        redraw = DJINN_REDRAW_ALL;
      groupMode = 0;
      setAll = 0;
    }
    repeat = gKeysRepeat;
    pressed = gKeysPressed;
    // The tutorial plays the menu itself: the steps press A, B, Right and
    // Left, after waits counted in frames.
    tutorialStep = backdrop->tutorialStep;
    if (tutorialStep != DJINN_TUTORIAL_OFF) {
      pressed = 0;
      backdrop->tutorialTimer++;
      repeat = 0;
      switch (tutorialStep) {
      case DJINN_TUTORIAL_CLOSE:
        while (!(gKeysPressed & KEY_A)) {
          MenuCursor_Position(150, 26);
          wait(1);
        }
        pressed = KEY_B;
        repeat = KEY_B;
        break;
      // The recovery lesson: after its two messages, three recovery countdowns
      // and Isaac's Flint goes back on standby.
      case DJINN_TUTORIAL_RECOVERY:
        if (backdrop->tutorialTimer == 60) {
          // "After a Djinni has been used to summon, it must spend time in
          // recovery."
          messageWindow = Message_OpenWindowFar(MsgDjinnTutorialRecovery, 9, 9,
                                                MESSAGE_WINDOW_ZOOM);
          gMainRoutineState.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (Message_AllTypedFar() == 0)
            wait(1);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          wait(1);
          // "And after we recover, we will set ourselves again and be ready to
          // attack."
          messageWindow = Message_OpenWindowFar(MsgDjinnTutorialSetAgain, 9, 9,
                                                MESSAGE_WINDOW_ZOOM);
          gMainRoutineState.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (Message_AllTypedFar() == 0)
            wait(1);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          backdrop->tutorialTimer = 0;
          DjinnRecovery_CountDownFar();
          DjinnRecovery_CountDownFar();
          DjinnRecovery_CountDownFar();
          Djinn_PutOnStandbyFar(ISAAC, ELEMENT_VENUS, DJINNI_FLINT);
          DjinnRecovery_AddStandbyFar(ISAAC, ELEMENT_VENUS, DJINNI_FLINT);
          Unit_CalculateFar(ISAAC);
          repeat = KEY_B;
          pressed = KEY_B;
        }
        break;
      case DJINN_TUTORIAL_GIVE:
        if (backdrop->tutorialTimer == 60) {
          // "To give your Djinni to someone else, move it like this."
          messageWindow = Message_OpenWindowFar(MsgDjinnTutorialGive, 9, 9,
                                                MESSAGE_WINDOW_ZOOM);
          gMainRoutineState.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (Message_AllTypedFar() == 0)
            wait(1);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 2;
        }
        break;
      case 2:
      case 4:
      case 8:
      case 9:
      case 22:
        if (backdrop->tutorialTimer == 90) {
          repeat = KEY_A;
          pressed = KEY_A;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case 3:
        if (backdrop->tutorialTimer == 90) {
          repeat = KEY_RIGHT;
          pressed = KEY_RIGHT;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 4;
        }
        break;
      case DJINN_TUTORIAL_SET_AFTER_GIVE:
      case 7:
        if (backdrop->tutorialTimer == 60) {
          // "After choosing who will take the Djinni, you can set it."
          messageWindow = Message_OpenWindowFar(MsgDjinnTutorialSetAfterGive, 9,
                                                9, MESSAGE_WINDOW_ZOOM);
          gMainRoutineState.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (Message_AllTypedFar() == 0)
            wait(1);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 8;
        }
        break;
      case 13:
        if (backdrop->tutorialTimer == 40) {
          backdrop->tutorialTimer = 0;
          repeat = KEY_B;
          pressed = KEY_B;
          backdrop->tutorialStep++;
        }
        break;
      case DJINN_TUTORIAL_POWERS:
      case 16:
      case 17:
        if (backdrop->tutorialTimer == 40) {
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case 15:
        if (backdrop->tutorialTimer == 60) {
          gMainRoutineState.messageSpeed = MESSAGE_SPEED_NORMAL;
          // "Each Djinni has different powers."
          messageWindow = Message_OpenWindowFar(MsgDjinnTutorialPowers, 9, 9,
                                                MESSAGE_WINDOW_ZOOM);
          MenuCursor_Slide(2, 146);
          while (Message_AllTypedFar() == 0)
            wait(1);
          while (!(gKeysPressed & KEY_A)) {
            MenuCursor_Position(2, 146);
            wait(1);
          }
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          wait(1);
          // "Once we are set, you can unleash us to use our powers."
          messageWindow = Message_OpenWindowFar(MsgDjinnTutorialUnleash, 9, 9,
                                                MESSAGE_WINDOW_ZOOM);
          while (Message_AllTypedFar() == 0)
            wait(1);
          while (!(gKeysPressed & KEY_A)) {
            MenuCursor_Position(2, 146);
            wait(1);
          }
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          wait(1);
          // "So, let me demonstrate the power of my attack, Isaac."
          messageWindow = Message_OpenWindowFar(MsgDjinnTutorialDemonstrate, 9,
                                                9, MESSAGE_WINDOW_ZOOM);
          while (Message_AllTypedFar() == 0)
            wait(1);
          while (!(gKeysPressed & KEY_A)) {
            MenuCursor_Position(2, 146);
            wait(1);
          }
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          wait(1);
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 16;
        }
        break;
      case 18:
      case 20:
        if (backdrop->tutorialTimer == 90) {
          repeat = KEY_A;
          pressed = KEY_A;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 21;
        }
        break;
      case 21:
        if (backdrop->tutorialTimer == 90) {
          backdrop->tutorialTimer = 0;
          repeat = KEY_LEFT;
          pressed = KEY_LEFT;
          backdrop->tutorialStep = 22;
        }
        break;
      case 24:
        if (backdrop->tutorialTimer == 60) {
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 25;
          repeat = KEY_B;
          pressed = KEY_B;
        }
        break;
      }
    }
    // DJINN_SELECT_DJINNI's R: put the Djinni on standby or set it at once,
    // unless it is recovering.
    if (mode == DJINN_SELECT_DJINNI && (pressed & KEY_R)) {
      if (selectedRow == -1) {
        Sound_PlayFar(SE_MENU_ERROR);
        continue;
      }
      i = 0;
      if (Djinn_CanSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                          DJINN_NUMBER(djinn)) ||
          Djinn_IsSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                         DJINN_NUMBER(djinn)))
        i = 1;
      groupMode = 1;
      gKeysPressedLatch = 0;
      if (i == 0) {
        Sound_PlayFar(SE_MENU_ERROR);
        WindowSprite_ClearListFar(menu->listWindow);
        Window_ClearInteriorTilesFar(menu->listWindow, 0, 80, 216, 104);
        Text_DrawMessageFar(MsgDjinnNowRecovering, menu->listWindow, 0, 96);
        redraw = DJINN_REDRAW_ALL;
        continue;
      }
      if (djinn & DJINN_ENTRY_IS_SET) {
        Sound_PlayFar(SE_NOTE_HIGH);
        Djinn_PutOnStandbyFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                              DJINN_NUMBER(djinn));
        DjinnRecovery_AddStandbyFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                                    DJINN_NUMBER(djinn));
      } else {
        Sound_PlayFar(SE_DJINNI_SET);
        Djinn_SetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                     DJINN_NUMBER(djinn));
        DjinnRecovery_RemoveFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                                DJINN_NUMBER(djinn));
      }
      Unit_CalculateFar(DJINN_MEMBER(djinn));
      DjinnMenu_DrawElementList(&backdrop->list);
      redraw = DJINN_REDRAW_ALL;
    }
    if (mode == DJINN_SELECT_DJINNI && (pressed & KEY_L)) {
      result = DJINN_RESULT_STATUS;
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if ((pressed & KEY_A) ||
        (mode == DJINN_SELECT_ACTION && (pressed & KEY_R))) {
      i = 1;
      if (!(columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER)) {
        if (selectedRow == -1) {
          Sound_PlayFar(SE_MENU_ERROR);
          continue;
        }
        i = 0;
        if (Djinn_CanSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                            DJINN_NUMBER(djinn)) ||
            Djinn_IsSetFar(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
                           DJINN_NUMBER(djinn)))
          i = 1;
      }
      if (i == 0) {
        Sound_PlayFar(SE_MENU_ERROR);
        WindowSprite_ClearListFar(menu->listWindow);
        Window_ClearInteriorTilesFar(menu->listWindow, 0, 80, 216, 104);
        Text_DrawMessageFar(MsgDjinnNowRecovering, menu->listWindow, 0, 96);
        continue;
      }
      if (mode == DJINN_SELECT_ACTION) {
        if (columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER) {
          if (memberColumn == menu->selectedColumn[0]) {
            if (menu->pairEntries[0] & DJINN_ENTRY_IS_SET)
              result = DJINN_RESULT_STANDBY;
            else
              result = DJINN_RESULT_SET;
          } else {
            result = DJINN_RESULT_GIVE;
          }
        } else {
          result = DJINN_RESULT_TRADE;
        }
      } else {
        result = DJINN_RESULT_CHOSEN;
      }
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if (pressed & KEY_START) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_START;
      break;
    }
    if (pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_BACK;
      break;
    }
    // Select: with R held, set every Djinni that is not recovering, or, the
    // next time, put every one on standby; otherwise open the help.
    if (mode == DJINN_SELECT_DJINNI && (pressed & KEY_SELECT)) {
      if (groupMode) {
        setAll ^= 1;
        if (setAll)
          Sound_PlayFar(SE_DJINNI_SET);
        else
          Sound_PlayFar(SE_NOTE_HIGH);
        for (i = 0; i < menu->memberCount; i++) {
          for (j = 0; j < backdrop->list.counts[i]; j++) {
            u32 entry = backdrop->list.ids[i][j];
            s32 usable;
            usable = 0;
            if (Djinn_CanSetFar(DJINN_MEMBER(entry), DJINN_ELEMENT(entry),
                                DJINN_NUMBER(entry)) ||
                Djinn_IsSetFar(DJINN_MEMBER(entry), DJINN_ELEMENT(entry),
                               DJINN_NUMBER(entry)))
              usable = 1;
            if (usable) {
              if (setAll) {
                if (!(entry & DJINN_ENTRY_IS_SET)) {
                  Djinn_SetFar(DJINN_MEMBER(entry), DJINN_ELEMENT(entry),
                               DJINN_NUMBER(entry));
                  DjinnRecovery_RemoveFar(DJINN_MEMBER(entry),
                                          DJINN_ELEMENT(entry),
                                          DJINN_NUMBER(entry));
                  Unit_CalculateFar(DJINN_MEMBER(entry));
                }
              } else if (entry & DJINN_ENTRY_IS_SET) {
                Djinn_PutOnStandbyFar(DJINN_MEMBER(entry), DJINN_ELEMENT(entry),
                                      DJINN_NUMBER(entry));
                DjinnRecovery_AddStandbyFar(DJINN_MEMBER(entry),
                                            DJINN_ELEMENT(entry),
                                            DJINN_NUMBER(entry));
                Unit_CalculateFar(DJINN_MEMBER(entry));
              }
            }
          }
        }
        DjinnMenu_DrawElementList(&backdrop->list);
        redraw = DJINN_REDRAW_ALL;
      } else {
        result = DJINN_RESULT_HELP;
        Window_SetInteriorPaletteBitRect(
            menu->listWindow, memberColumn * DJINN_COLUMN_TILES + 1,
            djinnRow + 2, DJINN_CURSOR_WIDTH, 1, WINDOW_PALETTE);
        Sound_PlayFar(SE_MENU_CONFIRM);
        break;
      }
    }
    if (repeat & KEY_UP) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      if (columnFlags[memberColumn] & DJINN_COLUMN_EMPTY)
        continue;
      Window_SetInteriorPaletteBitRect(
          menu->listWindow, memberColumn * DJINN_COLUMN_TILES + 1, djinnRow + 2,
          DJINN_CURSOR_WIDTH, 1, WINDOW_PALETTE);
      if (columnFlags[memberColumn] & DJINN_COLUMN_EMPTY)
        continue;
      if (columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER) {
        columnFlags[memberColumn] &= ~DJINN_COLUMN_ON_HEADER;
        djinnRow = 0;
      } else if (djinnRow == 0 &&
                 (columnFlags[memberColumn] & DJINN_COLUMN_HEADER_CHOOSABLE)) {
        columnFlags[memberColumn] |= DJINN_COLUMN_ON_HEADER;
        redraw = DJINN_REDRAW_MOVED;
        continue;
      }
      djinnRow--;
      count = backdrop->list.counts[memberColumn];
      if (count == 0)
        count = 1;
      djinnRow = Menu_WrapIndex(djinnRow, count);
      savedRow = djinnRow;
      redraw = DJINN_REDRAW_MOVED;
      continue;
    }
    if (repeat & KEY_DOWN) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      if (columnFlags[memberColumn] & DJINN_COLUMN_EMPTY)
        continue;
      Window_SetInteriorPaletteBitRect(
          menu->listWindow, memberColumn * DJINN_COLUMN_TILES + 1, djinnRow + 2,
          DJINN_CURSOR_WIDTH, 1, WINDOW_PALETTE);
      djinnRow++;
      count = backdrop->list.counts[memberColumn];
      if (count == 0)
        count = 1;
      djinnRow = Menu_WrapIndex(djinnRow, count);
      if ((columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER) &&
          !(columnFlags[memberColumn] & DJINN_COLUMN_EMPTY)) {
        columnFlags[memberColumn] &= ~DJINN_COLUMN_ON_HEADER;
        djinnRow = 0;
      } else if (djinnRow == 0 &&
                 (columnFlags[memberColumn] & DJINN_COLUMN_HEADER_CHOOSABLE)) {
        columnFlags[memberColumn] |= DJINN_COLUMN_ON_HEADER;
      }
      savedRow = djinnRow;
      redraw = DJINN_REDRAW_MOVED;
      continue;
    }
    if (repeat & KEY_LEFT) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      if (!(columnFlags[memberColumn] & DJINN_COLUMN_EMPTY))
        Window_SetInteriorPaletteBitRect(
            menu->listWindow, memberColumn * DJINN_COLUMN_TILES + 1,
            djinnRow + 2, DJINN_CURSOR_WIDTH, 1, WINDOW_PALETTE);
      memberColumn--;
      memberColumn = Menu_WrapIndex(memberColumn, menu->memberCount);
      if (mode == DJINN_SELECT_DJINNI) {
        for (i = 0; i < menu->memberCount; i++) {
          if (columnFlags[memberColumn] == DJINN_COLUMN_EMPTY) {
            memberColumn--;
            memberColumn = Menu_WrapIndex(memberColumn, menu->memberCount);
          }
        }
      }
      djinnRow = savedRow;
      count = backdrop->list.counts[memberColumn];
      if (count == 0)
        count = 1;
      djinnRow = Menu_WrapIndex(djinnRow, count);
      redraw = DJINN_REDRAW_MOVED;
      continue;
    }
    if (repeat & KEY_RIGHT) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      if (!(columnFlags[memberColumn] & DJINN_COLUMN_EMPTY))
        Window_SetInteriorPaletteBitRect(
            menu->listWindow, memberColumn * DJINN_COLUMN_TILES + 1,
            djinnRow + 2, DJINN_CURSOR_WIDTH, 1, WINDOW_PALETTE);
      memberColumn++;
      memberColumn = Menu_WrapIndex(memberColumn, menu->memberCount);
      if (mode == DJINN_SELECT_DJINNI) {
        for (i = 0; i < menu->memberCount; i++) {
          if (columnFlags[memberColumn] == DJINN_COLUMN_EMPTY) {
            memberColumn++;
            memberColumn = Menu_WrapIndex(memberColumn, menu->memberCount);
          }
        }
      }
      djinnRow = savedRow;
      count = backdrop->list.counts[memberColumn];
      if (count == 0)
        count = 1;
      djinnRow = Menu_WrapIndex(djinnRow, count);
      redraw = DJINN_REDRAW_MOVED;
    }
  }
  menu->selectedColumn[mode] = memberColumn;
  if (selectedRow != -1) {
    u32 id;
    u32 entry;

    menu->pairEntries[mode] = id =
        backdrop->list.ids[memberColumn][selectedRow];
    entry = (u16)id;
    menu->pairIndex[mode] = entry % (DJINN_ENTRY_INDEX + 1);
    menu->pairElement[mode] = DJINN_ELEMENT(entry);
    menu->pairMember[mode] = DJINN_MEMBER(entry);
  }
  menu->selection[mode] = memberColumn + djinnRow * DJINN_CURSOR_ROW_STRIDE;
  return result;
}

/// Lists owned Djinn by element and index, marking those that are set.
/// For element -1, entries also encode their party member.
s32 Djinn_ListUnitEntries(u16 *entriesOut, s32 memberId, s32 element) {
  struct Unit *unit = Unit_GetFar(memberId);
  s32 count = 0;
  s32 elementIndex;
  s32 djinnIndex;
  s32 encodedDjinn;

  if (element == -1) {
    for (elementIndex = 0; elementIndex < DJINN_ELEMENTS; elementIndex++) {
      for (djinnIndex = 0; djinnIndex < DJINN_PER_ELEMENT; djinnIndex++) {
        if (unit->djinnSet[elementIndex] & (1 << djinnIndex)) {
          encodedDjinn = (elementIndex << 5) | djinnIndex | DJINN_ENTRY_SET;
          encodedDjinn |= memberId << 8;
          entriesOut[count++] = encodedDjinn;
        } else if (unit->djinnHeld[elementIndex] & (1 << djinnIndex)) {
          entriesOut[count++] =
              (elementIndex << 5) | djinnIndex | (memberId << 8);
        }
      }
    }
  } else {
    for (djinnIndex = 0; djinnIndex < DJINN_PER_ELEMENT; djinnIndex++) {
      if (unit->djinnSet[element] & (1 << djinnIndex))
        entriesOut[count++] = (element << 5) | djinnIndex | DJINN_ENTRY_SET;
      else if (unit->djinnHeld[element] & (1 << djinnIndex))
        entriesOut[count++] = (element << 5) | djinnIndex;
    }
  }
  return count;
}

/// Draws both sides of the give or trade preview for the selected page: the
/// receiver in the second window and the giver in the first. mode is a
/// DjinnCompareOperation; only DJINN_COMPARE_TRADE differs, swapping the two
/// Djinn. Returns 1.
s32 DjinnMenu_DrawPairPreview(s32 mode, s32 pageIndex) {
  struct DjinnMenuState *menu;

  menu = gMenuState;
  if (mode == DJINN_COMPARE_TRADE) {
    DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->pairMember[1], 1,
                              mode, DJINN_PREVIEW_TRADE, pageIndex, 1);
    DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pairMember[0], mode, 1,
                              DJINN_PREVIEW_TRADE, pageIndex, mode);
  } else {
    DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->shownMember[1], 1,
                              0, DJINN_SWAP_PREVIEW_MODE, pageIndex, 1);
    DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->shownMember[0], 0, 0,
                              DJINN_PREVIEW_UNSET, pageIndex, 0);
  }
  return 1;
}

/// Copies a full unit for a temporary Djinn-stat preview.
static __inline__ void Unit_Copy(struct Unit *destination,
                                 struct Unit *source) {
  Iwram_CopyWords(destination, source, sizeof(struct Unit));
}

/// Saves the unit, applies a hypothetical Djinn change, draws its differences
/// and restores the original unit before returning.
s32 DjinnMenu_DrawStatPreview(struct Window *window, s32 x, s32 y, s32 memberId,
                              s32 removePairSlot, s32 addPairSlot,
                              s32 previewMode, s32 page, s32 side) {
  struct Unit *unit;
  struct Unit *originalUnit;
  s32 giveElement;
  s32 giveIndex;
  s32 giveSet;
  s32 takeElement;
  s32 takeIndex;
  s32 takeSet;
  s32 lost;
  s32 gained;
  // PsynergyList_Diff can write up to 64 entries here in the worst case
  // (32 kept or gained and 32 lost).
  u16 actions[48];
  struct DjinnMenuState *menu;

  unit = Unit_GetFar(memberId);
  menu = gMenuState;
  giveElement = menu->pairElement[removePairSlot];
  giveIndex = menu->pairIndex[removePairSlot];
  giveSet = menu->pairEntries[removePairSlot] & DJINN_ENTRY_IS_SET;
  takeElement = menu->pairElement[addPairSlot];
  takeIndex = menu->pairIndex[addPairSlot];
  takeSet = menu->pairEntries[addPairSlot] & DJINN_ENTRY_IS_SET;
  originalUnit = Heap_Alloc(sizeof(struct Unit));
  Unit_Copy(originalUnit, unit);

  if (page == 0) {
    if (previewMode == DJINN_PREVIEW_NO_CHANGE) {
      Text_DrawNumberInWindowFar(unit->hpMax, DJINN_PREVIEW_DIGITS, window,
                                 x * 8 + DJINN_PREVIEW_MAX_X,
                                 DJINN_PREVIEW_STAT_Y(y, 0));
      Text_DrawNumberInWindowFar(unit->ppMax, DJINN_PREVIEW_DIGITS, window,
                                 x * 8 + DJINN_PREVIEW_MAX_X,
                                 DJINN_PREVIEW_STAT_Y(y, 1));
      Text_DrawNumberInWindowFar(unit->hp, DJINN_PREVIEW_DIGITS, window,
                                 x * 8 + DJINN_PREVIEW_CURRENT_X,
                                 DJINN_PREVIEW_STAT_Y(y, 0));
      Text_DrawNumberInWindowFar(unit->pp, DJINN_PREVIEW_DIGITS, window,
                                 x * 8 + DJINN_PREVIEW_CURRENT_X,
                                 DJINN_PREVIEW_STAT_Y(y, 1));
      Text_DrawStringAtOffsetFar(gDjinnMenu_TextSlash, window,
                                 x * 8 + DJINN_PREVIEW_SLASH_X,
                                 DJINN_PREVIEW_STAT_Y(y, 0));
      Text_DrawStringAtOffsetFar(gDjinnMenu_TextSlash, window,
                                 x * 8 + DJINN_PREVIEW_SLASH_X,
                                 DJINN_PREVIEW_STAT_Y(y, 1));
    } else {
      Text_DrawNumberInWindowFar(unit->hp, DJINN_PREVIEW_DIGITS, window,
                                 x * 8 + DJINN_PREVIEW_CURRENT_X,
                                 DJINN_PREVIEW_STAT_Y(y, 0));
      Text_DrawNumberInWindowFar(unit->pp, DJINN_PREVIEW_DIGITS, window,
                                 x * 8 + DJINN_PREVIEW_CURRENT_X,
                                 DJINN_PREVIEW_STAT_Y(y, 1));
    }
    Text_DrawNumberInWindowFar(unit->atk, DJINN_PREVIEW_DIGITS, window,
                               x * 8 + DJINN_PREVIEW_CURRENT_X,
                               DJINN_PREVIEW_STAT_Y(y, 2));
    Text_DrawNumberInWindowFar(unit->def, DJINN_PREVIEW_DIGITS, window,
                               x * 8 + DJINN_PREVIEW_CURRENT_X,
                               DJINN_PREVIEW_STAT_Y(y, 3));
    Text_DrawNumberInWindowFar(unit->agi, DJINN_PREVIEW_DIGITS, window,
                               x * 8 + DJINN_PREVIEW_CURRENT_X,
                               DJINN_PREVIEW_STAT_Y(y, 4));
    Text_DrawNumberInWindowFar(unit->luck, 2, window,
                               x * 8 + DJINN_PREVIEW_LUCK_X,
                               DJINN_PREVIEW_STAT_Y(y, 5));
  }

  // Only the temporary unit changes; the saved record is restored below.
  switch (previewMode) {
  case DJINN_PREVIEW_ADD_AND_SET:
    Djinn_GiveFar(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    Djinn_SetFar(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    break;
  case DJINN_PREVIEW_UNSET:
    giveIndex &= DJINN_ENTRY_INDEX;
    Djinn_PutOnStandbyFar(memberId, giveElement, giveIndex);
    break;
  case DJINN_PREVIEW_TRADE:
    if (giveSet) {
      giveIndex &= DJINN_ENTRY_INDEX;
      Djinn_PutOnStandbyFar(memberId, giveElement, giveIndex);
    }
    Djinn_GiveFar(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    if (takeSet)
      Djinn_SetFar(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    break;
  case DJINN_PREVIEW_ADD:
    Djinn_GiveFar(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    if (takeSet)
      Djinn_SetFar(memberId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    break;
  }

  Unit_CalculateFar(memberId);
  unit = Unit_GetFar(memberId);

  if (page == 0) {
    s32 label;

    Text_DrawStringAtOffsetFar(unit->name, window, x * 8 + 40, y * 8);
    Text_DrawStringAtOffsetFar(gDjinnMenu_TextLevel, window, x * 8 + 40,
                               y * 8 + 16);
    Text_DrawNumberInWindowFar(unit->level, 2, window, x * 8 + 88, y * 8 + 16);
    label = MsgPreviewStatLabel;
    Text_DrawMessageAsTilesFar(label, window, x * 8,
                               DJINN_PREVIEW_STAT_Y(y, 0));
    Text_DrawMessageAsTilesFar(label + 1, window, x * 8,
                               DJINN_PREVIEW_STAT_Y(y, 1));
    Text_DrawMessageAsTilesFar(label + 2, window, x * 8,
                               DJINN_PREVIEW_STAT_Y(y, 2));
    Text_DrawMessageAsTilesFar(label + 3, window, x * 8,
                               DJINN_PREVIEW_STAT_Y(y, 3));
    Text_DrawMessageAsTilesFar(label + 4, window, x * 8,
                               DJINN_PREVIEW_STAT_Y(y, 4));
    Text_DrawMessageAsTilesFar(label + 5, window, x * 8,
                               DJINN_PREVIEW_STAT_Y(y, 5));
    Text_DrawMessageAsTilesFar(MsgClassName + originalUnit->classId, window,
                               x * 8, y * 8 + 32);
  }

  {
    s32 i = 0;

    if (page == 0) {
      s32 column;
#if LANGUAGE != LANGUAGE_JAPANESE
      s32 iconX;
#endif
      s32 textX;

      if (originalUnit->classId != unit->classId) {
        textX = x * 8;
        Text_DrawMessageAsTilesFar(MsgClassName + unit->classId, window, textX,
                                   y * 8 + 48);
        // The down arrow from the class to the next, a tile of BG
        // character block 1, the block after the windows' own.
        Window_SetTilemapEntryFar(window,
                                  WINDOW_TILE(BG_CHAR_SIZE / TILE_SIZE_4BPP +
                                              DJINN_CHANGE_ARROW_TILE),
                                  x + 2, 5, TILEMAP_ENTRY_PLAIN);
      }
      column = x;
      if (originalUnit->classId != unit->classId)
        column += 5;
      // Each element's icon and its count of set Djinn: WINDOW_TILE('0') + n is
      // the window font's digit n in WINDOW_PALETTE.
      for (i = 0; i < DJINN_ELEMENTS; i++) {
        Window_SetTilemapEntryFar(window, DJINN_ELEMENT_TILE + i,
                                  column + i * 2, y + 5, TILEMAP_ENTRY_PLAIN);
        Window_SetTilemapEntryFar(
            window, unit->djinnSetCounts[i] + WINDOW_TILE('0'),
            column + i * 2 + 1, y + 5, TILEMAP_ENTRY_PLAIN);
      }
#if LANGUAGE != LANGUAGE_JAPANESE
      iconX = x * 8 + 70;
#endif
      if (unit->hp != originalUnit->hp) {
        Text_DrawNumberInWindowFar(unit->hp, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 0));
        if (unit->hp > originalUnit->hp)
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 0), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 0), STAT_ARROW_DOWN);
      }
      if (unit->pp != originalUnit->pp) {
        Text_DrawNumberInWindowFar(unit->pp, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 1));
        if (unit->pp > originalUnit->pp)
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 1), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 1), STAT_ARROW_DOWN);
      }
      if (unit->atk != originalUnit->atk) {
        Text_DrawNumberInWindowFar(unit->atk, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 2));
        if (unit->atk > originalUnit->atk)
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 2), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 2), STAT_ARROW_DOWN);
      }
      if (unit->def != originalUnit->def) {
        Text_DrawNumberInWindowFar(unit->def, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 3));
        if (unit->def > originalUnit->def)
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 3), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 3), STAT_ARROW_DOWN);
      }
      if (unit->agi != originalUnit->agi) {
        Text_DrawNumberInWindowFar(unit->agi, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 4));
        if (unit->agi > originalUnit->agi)
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 4), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 4), STAT_ARROW_DOWN);
      }
      if (unit->luck != originalUnit->luck) {
        Text_DrawNumberInWindowFar(unit->luck, 2, window, x * 8 + 88,
                                   DJINN_PREVIEW_STAT_Y(y, 5));
        if (unit->luck > originalUnit->luck)
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 5), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX),
              DJINN_PREVIEW_STAT_Y(y, 5), STAT_ARROW_DOWN);
      }
    }
  }

  // Pages 1 on: the Psynergy after the change, five to a page (six with no
  // change), then the key below: "* Psynergy Gained" in TEXT_COLOR_YELLOW, "*
  // Psynergy Lost" in TEXT_COLOR_RED, or "* No change".
  if (page > 0) {
    s32 rows;
    s32 first;
    s8 count;
    s8 row;
    s8 line;

    rows = 6 - (previewMode != DJINN_PREVIEW_NO_CHANGE);
    first = rows * (page - 1);
    count = PsynergyList_Diff(originalUnit->psynergy, unit->psynergy, actions,
                              &gained, &lost);
    line = 0;
    for (row = 0; first < count && row < rows; row++, first++) {
      Icon_CreateWithLoadedResource(window, x * 8, (y + line * 2) * 8 + 4,
                                    actions[first] & PSYNERGY_ABILITY_MASK);
      if (actions[first] & DJINN_PREVIEW_GAINED)
        Text_SetColorFar(TEXT_COLOR_YELLOW);
      else if (actions[first] & DJINN_PREVIEW_LOST)
        Text_SetColorFar(TEXT_COLOR_RED);
      else
        Text_SetColorFar(TEXT_COLOR_NORMAL);
      Text_DrawMessageAsTilesFar(MsgAbilityName +
                                     (actions[first] & PSYNERGY_ABILITY_MASK),
                                 window, x * 8 + 16, (y + line * 2) * 8 + 8);
      Text_DrawNumberAtOffsetFar(Ability_Get(actions[first])->ppCost, 2, window,
                                 x * 8 + 88, (y + line * 2) * 8 + 8);
      line++;
    }
    Text_SetColorFar(TEXT_COLOR_NORMAL);
    Text_DrawMessageAsTilesFar(MsgPsynergyPp, window,
                               x * 8 + DJINN_PREVIEW_PP_X, y * 8);
    if (previewMode != DJINN_PREVIEW_NO_CHANGE) {
      s32 lines;

      lines = 0;
      if (gained) {
        Text_SetColorFar(TEXT_COLOR_YELLOW);
        Text_DrawMessageAsTilesFar(MsgPsynergyGained, window, x * 8,
                                   y * 8 + 88);
        lines = 1;
      }
      if (lost) {
        Text_SetColorFar(TEXT_COLOR_RED);
        Text_DrawMessageAsTilesFar(MsgPsynergyLost, window, x * 8,
                                   (y + lines) * 8 + 88);
        lines++;
      }
      if (lines == 0)
        Text_DrawMessageAsTilesFar(MsgPsynergyUnchanged, window, x * 8,
                                   y * 8 + 88);
      Text_SetColorFar(TEXT_COLOR_NORMAL);
      Window_DrawDividerLineFar(window, 0, 11, 13, 11);
    }
    WINDOW_STATE->dirty = 1;
  }

  if (page == 0)
    Portrait_CreateFar(memberId, 0, side, window, 0, 0);
  // None of the preview's class, stat or action changes escape this call.
  Unit_Copy(unit, originalUnit);
  Heap_Release(originalUnit);
  return 1;
}

#undef DJINN_ENTRY_NONE
#undef DJINN_BACKDROP_DIGIT_TILE
#undef DJINN_BACKDROP_PALETTE
#undef DJINN_DIM_PALETTE
#undef DJINN_WINDOW_COPY_PALETTE
#undef DJINN_HELP_TOPICS
#undef DJINN_MEMBER_FIGURE_X
#undef DJINN_MEMBER_FIGURE_Y
#undef DJINN_CURSOR_WIDTH
#undef DJINN_SWAP_PREVIEW_MODE
#undef DJINN_PREVIEW_DIGITS
#undef DJINN_PREVIEW_CURRENT_X
#undef DJINN_PREVIEW_MAX_X
#undef DJINN_PREVIEW_SLASH_X
#undef DJINN_PREVIEW_LUCK_X
#undef DJINN_PREVIEW_ARROW_X
#undef DJINN_PREVIEW_PP_X
#undef DJINN_RESELECT_PAIR_ENTRY
#undef DJINN_PREVIEW_GAINED
#undef DJINN_PREVIEW_LOST
#undef DJINN_ENTRY_SET
#undef DJINN_COLUMN_TILES
#undef DJINN_COLUMN_WIDTH
#undef DJINN_FIGURE_X
#undef DJINN_CURSOR_ROW_STRIDE
#undef DJINN_PREVIEW_VALUE_X

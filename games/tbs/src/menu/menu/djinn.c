/// @file
/// The Djinn menu: its screen, the Djinn list, help, and choosing the Djinn to
/// set, put on standby, give or trade.
// Part of menu.c.

#include "../menu_internal.h"

// The international editions widen the Djinn list's highlight, show a trade
// preview's second side in another mode and lay the stat preview out for
// fewer digits.
//
// The highlight over a row of the Djinn list, in tiles.
#if LANGUAGE != LANGUAGE_JAPANESE
#define DJINN_CURSOR_WIDTH 6
#else
#define DJINN_CURSOR_WIDTH 5
#endif

#if TBS_PATCH_1
#define DJINN_SWAP_PREVIEW_MODE DJINN_PREVIEW_TRADE
#else
#define DJINN_SWAP_PREVIEW_MODE DJINN_PREVIEW_ADD
#endif

// The stat preview's columns; an arrow stands at x (in tiles) or, in the
// international editions, at the pixel column iconX the preview sets.
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

/// Wraps a cursor index that has stepped one past either end of modulus
/// entries: -1 becomes modulus - 1 and modulus becomes 0.
s32 Menu_WrapIndex(s32 value, s32 modulus) {
  return (value + modulus) % modulus;
}

/// Places the four party figures over their Djinn columns, 56 pixels apart
/// from x 32, at screen y 61 plus yOffset. Returns that y.
s32 Menu_SetFirstObjectRowCoordinates(s32 yOffset) {
  struct DjinnMenuWork *menu;
  s32 ownerX;
  s32 ownerIndex;
  menu = gMenuWork;
  yOffset += 0x3D;
  ownerX = 0x20;
  for (ownerIndex = 0; ownerIndex < 4; ownerIndex++) {
    menu->ownerX[ownerIndex] = ownerX;
    menu->ownerY[ownerIndex] = yOffset;
    ownerX += 0x38;
  }
  return yOffset;
}

/// Runs the Djinn menu over the field: the field menu's Djinn screen
/// (Menu_RunTopSelection, FIELD_MENU_DJINN), and the Djinn tutorial, which the
/// world map opens as DjinnMenu_Open when Flint joins. Types messages at
/// MESSAGE_SPEED_FAST while it runs (the tutorial's steps set
/// MESSAGE_SPEED_NORMAL for their messages, which stays until the menu closes)
/// and puts the player's speed back. Takes the menu work (HEAP_SLOT_MENU) and
/// the backdrop (struct DjinnMenuBackdrop, from the alternate pool), and frees
/// both before returning, after putting back the field tiles and palettes the
/// backdrop saved. Returns 1, which makes the field menu open again.
s32 DjinnMenu_Run(void) {
  struct DjinnMenuWork *menu;
  u32 savedMessageSpeed;
  s32 enabled;
  struct DjinnMenuBackdrop *backdrop;
  s32 zero;

  menu = Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  savedMessageSpeed = gGameState.messageSpeed;
  gGameState.messageSpeed = MESSAGE_SPEED_FAST;
  enabled = gMenuCtrlWork->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  UiWindow_InitializeWork(0);
  backdrop = Runtime_BumpAllocateAlternatePool(0x2130);
  menu->backdrop = backdrop;
  backdrop->tutorialTimer = 0;
  backdrop->tutorialStep = DJINN_TUTORIAL_OFF;
  // The tutorial's lesson, from the flags the world map sets for each.
  if (GameFlag_TestFar(FLAG_DJINN_LESSON)) {
    if (!GameFlag_TestFar(FLAG_DJINN_LESSON_PART_A)) {
      if (!GameFlag_TestFar(FLAG_DJINN_LESSON_PART_B))
        backdrop->tutorialStep = enabled;
      else
        backdrop->tutorialStep = DJINN_TUTORIAL_POWERS;
    } else {
      if (!GameFlag_TestFar(FLAG_DJINN_LESSON_PART_B))
        backdrop->tutorialStep = DJINN_TUTORIAL_RECOVERY;
      else
        backdrop->tutorialStep = DJINN_TUTORIAL_CLOSE;
    }
  }
  Menu_SuspendFieldTasks();
  UiWork_SetAltFlagAndClearTableFar(1);
  Link_DrawShiftedTilePairFar(UI_PAGE_ARROW_VRAM);
  menu->ownerCount = Party_ListActiveOwnersFar(menu->owners);
  Resource_LoadPairedBlocksIfAvailable();
  ItemMenu_Initialize(0, 3, 0, 7);
  Menu_SetFirstObjectRowCoordinates(0);
  Palette_LightenBankHighlight(14);
  menu->helpWindow = UiWindow_CreateFar(13, 0, 17, 5, 2);
  menu->pairEntries[0] = 255;
  zero = 0;
  menu->selectedColumn[0] = zero;
  menu->selectedColumn[1] = zero;
  menu->selection[0] = 0;
  menu->selection[1] = 0;
  FourObjectMotion_InitializeBottomRow((s32)menu->helpWindow, 0);
  DjinnMenu_RunScreen();
  FourObjectMotion_ClearSlotsAndScheduleAlt();
  Menu_ResetTwoResourceEntries();
  WaitFrames(1);
  ItemMenu_Close();
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  gMenuCtrlWork->suspended = 0;
  UiWindow_MarkVisibleTileAttributesFar();
  UiWork_SetAltFlagAndClearTableFar(0);
  Iwram_CopyWords(BG_CHAR_BLOCK(1), backdrop->tiles, 0x2000);
  Iwram_CopyWords((void *)&BG_PLTT_COLOR(4, 0), backdrop->palette, 128);
  WaitFrames(1);
  Menu_ResumeFieldTasks();
  UiWindow_EraseBorderRectFar(0, 0, 30, 20);
  MemFree(menu->backdrop);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
  Event_ClearInvalidPackedValuesFar();
  gGameState.messageSpeed = savedMessageSpeed;
  return 1;
}

/// Finds the moved Djinni in the list and selects its new position.
/* FAKEMATCH(one-pass): the do { } while (0) ends the selection store's
 * scheduling region at both uses; as a plain { } block, 4 of 560 instructions
 * differ in tbs-en. */
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
  DJINN_STEP_HANDLE_RESULT = 0,
  DJINN_STEP_RESERVED = 1, ///< nothing sets it
  DJINN_STEP_SELECT_DJINN = 2,
  DJINN_STEP_SELECT_ACTION = 3,
  DJINN_STEP_HANDLE_PAIR_RESULT = 4,
  DJINN_STEP_CONFIRM_SET = 5,
  DJINN_STEP_CONFIRM_STANDBY = 6,
  DJINN_STEP_CONFIRM_GIVE = 7,
  DJINN_STEP_SELECT_SECOND = 8,
  DJINN_STEP_CONFIRM_TRADE = 9,
  DJINN_STEP_SHOW_STATUS = 10,
  DJINN_STEP_APPLY_SET = 11,
  DJINN_STEP_APPLY_STANDBY = 12,
  DJINN_STEP_APPLY_GIVE = 13,
  DJINN_STEP_APPLY_TRADE = 14,
  DJINN_STEP_SHOW_HELP = 15
};

/// Chooses a Djinni, previews changes, then sets, stands by, gives or trades
/// it. Confirmation states fall through to their matching commit states on
/// success. Loops until the player leaves the first chooser (B or Start:
/// returns -1) or presses Start further in (usually 0); DjinnMenu_Run ignores
/// the result.
s32 DjinnMenu_RunScreen(void) {
  struct DjinnMenuWork *menu;
  struct RenderOutput *cursor;
  s32 result;
  s32 step;
  s32 menuResult;
  s32 done;
  s32 listIndex;

  done = 0;
  result = 0;
  menuResult = 0;
  menu = gMenuWork;
  cursor = menu->cursor;
  cursor->active = ICON_HIDDEN;
  cursor->animStep = 0;
  Menu_OpenBackdropScreen();
  WaitFrames(1);
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
      Menu_SetFirstObjectRowCoordinates(0);
      FourObjectMotion_SetSlotPosition(1, 0, 200, 0);
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
      menu->owner = menu->owners[menu->selectedColumn[0]];
      menu->shownOwner[0] = menu->owners[menu->selectedColumn[0]];
      result = DjinnMenu_ShowOwnerStatus();
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
      Menu_SetFirstObjectRowCoordinates(-8);
      menu->owner = menu->owners[menu->selectedColumn[0]];
      menu->shownOwner[0] = menu->owners[menu->selectedColumn[0]];
      FourObjectMotion_SetSlotPosition(0, menu->selectedColumn[0] * 56 + 48, 54,
                                       0);
      result = DjinnMenu_SelectDjinn(DJINN_SELECT_ACTION);
      for (listIndex = 0; listIndex < menu->ownerCount; listIndex++)
        menu->ownerY[listIndex] += 8;
      if (result == DJINN_RESULT_START)
        done = 1;
      if (result < 0) {
        step = DJINN_STEP_SELECT_DJINN;
        break;
      }
      if (result == DJINN_RESULT_GIVE || result == DJINN_RESULT_TRADE ||
          result == DJINN_RESULT_GIVE_NOW || result == DJINN_RESULT_TRADE_NOW)
        menu->shownOwner[1] = menu->owners[menu->selectedColumn[1]];
      if (result < 0)
        step = DJINN_STEP_SELECT_DJINN;
      else if (result == DJINN_RESULT_SET)
        step = DJINN_STEP_CONFIRM_SET;
      else if (result == DJINN_RESULT_STANDBY)
        step = DJINN_STEP_CONFIRM_STANDBY;
      else if (result == DJINN_RESULT_GIVE) {
        menu->pending = 2;
        step = DJINN_STEP_CONFIRM_GIVE;
      } else if (result == DJINN_RESULT_TRADE) {
        menu->pending = 2;
        step = DJINN_STEP_CONFIRM_TRADE;
      } else if (result == DJINN_RESULT_SET_NOW)
        step = DJINN_STEP_APPLY_SET;
      else if (result == DJINN_RESULT_STANDBY_NOW)
        step = DJINN_STEP_APPLY_STANDBY;
      else if (result == DJINN_RESULT_GIVE_NOW) {
        menu->pending = 2;
        step = DJINN_STEP_APPLY_GIVE;
      } else if (result == DJINN_RESULT_TRADE_NOW) {
        menu->pending = 2;
        step = DJINN_STEP_APPLY_TRADE;
      }
      break;
    case DJINN_STEP_CONFIRM_GIVE:
      result = OwnerAction_RunCompareLoop(DJINN_COMPARE_GIVE);
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_ACTION;
      if (result < 0)
        break;
      // Confirmed: apply it at once.
      // Fall through.
    case DJINN_STEP_APPLY_GIVE:
      AudioCommand_PlayFar(SE_RECOVERY);
      result = Djinn_TransferFar(menu->shownOwner[0], menu->pairElement[0],
                                 menu->pairIndex[0], menu->shownOwner[1]);
      UnitCalcFar(menu->shownOwner[0]);
      UnitCalcFar(menu->shownOwner[1]);
      menu->cursor->active = ICON_HIDDEN;
      RenderOutput_ClearListFar(menu->listWindow);
      DjinnMenu_BuildLists(menu->backdrop);
      DJINN_RESELECT_PAIR_ENTRY(menu, listIndex);
      menu->cursor->active = ICON_SHOWN;
      step = DJINN_STEP_HANDLE_RESULT;
      break;
    case DJINN_STEP_CONFIRM_STANDBY:
      result = OwnerAction_RunCompareLoop(DJINN_COMPARE_STANDBY);
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_ACTION;
      if (result < 0)
        break;
      // Confirmed: apply it at once.
      // Fall through.
    case DJINN_STEP_APPLY_STANDBY:
      AudioCommand_PlayFar(SE_175);
      Djinn_DeactivateFar(menu->shownOwner[0], menu->pairElement[0],
                          menu->pairIndex[0]);
      result = (s32)DjinnRecovery_AddStandbyFar(
          menu->shownOwner[0], menu->pairElement[0], menu->pairIndex[0]);
      UnitCalcFar(menu->shownOwner[0]);
      menu->cursor->active = ICON_HIDDEN;
      RenderOutput_ClearListFar(menu->listWindow);
      menu->cursor->active = ICON_SHOWN;
      step = DJINN_STEP_SELECT_DJINN;
      break;
    case DJINN_STEP_CONFIRM_TRADE:
      result = OwnerAction_RunCompareLoop(DJINN_COMPARE_TRADE);
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_ACTION;
      if (result < 0)
        break;
      // Confirmed: apply it at once.
      // Fall through.
    case DJINN_STEP_APPLY_TRADE:
      AudioCommand_PlayFar(SE_RECOVERY);
      Djinn_TransferFar(menu->shownOwner[0], menu->pairElement[0],
                        menu->pairIndex[0], menu->shownOwner[1]);
      result = Djinn_TransferFar(menu->shownOwner[1], menu->pairElement[1],
                                 menu->pairIndex[1], menu->shownOwner[0]);
      UnitCalcFar(menu->shownOwner[0]);
      UnitCalcFar(menu->shownOwner[1]);
      DjinnMenu_BuildLists(menu->backdrop);
      DJINN_RESELECT_PAIR_ENTRY(menu, listIndex);
      menu->cursor->active = ICON_SHOWN;
      step = DJINN_STEP_SELECT_DJINN;
      break;
    case DJINN_STEP_HANDLE_PAIR_RESULT:
      if (result == DJINN_RESULT_BACK) {
        menuResult = result;
        step = DJINN_STEP_SELECT_DJINN;
      } else if (menu->pending & 1)
        step = DJINN_STEP_SELECT_SECOND;
      else if (menu->pending & 2)
        step = DJINN_STEP_CONFIRM_GIVE;
      break;
    case DJINN_STEP_CONFIRM_SET:
      result = OwnerAction_RunCompareLoop(DJINN_COMPARE_SET);
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_ACTION;
      if (result < 0)
        break;
      // Confirmed: apply it at once.
      // Fall through.
    case DJINN_STEP_APPLY_SET:
      AudioCommand_PlayFar(SE_139);
      Djinn_ActivateFar(menu->shownOwner[0], menu->pairElement[0],
                        menu->pairIndex[0]);
      result = DjinnRecovery_RemoveFar(
          menu->shownOwner[0], menu->pairElement[0], menu->pairIndex[0]);
      UnitCalcFar(menu->shownOwner[0]);
      menu->cursor->active = ICON_HIDDEN;
      RenderOutput_ClearListFar(menu->listWindow);
      menu->cursor->active = ICON_SHOWN;
      step = DJINN_STEP_SELECT_DJINN;
      break;
    default:
      done = 1;
      break;
    }
  } while (done == 0);
  return menuResult;
}

/// Writes copies of background palettes 15, 5 and 7, each brightnessDelta
/// lighter in every channel (clamped to 0-31), into palettes 14, 4 and 6.
/// Palette 7's copy is 12 darker instead. Menu_OpenBackdropScreen calls it
/// with 8: 14 becomes the row highlight, 4 the highlighted element icons and
/// 6 the dimmed text behind the help (DjinnMenu_ShowHelp).
void Graphics_AdjustPaletteBank(s32 brightnessDelta) {
  s32 bankIndex;
  s32 bank;
  s32 colorIndex;
  s32 mask;
  s32 paletteIndex;

  bank = 15;
  bankIndex = 0;
  mask = 31;
  do {
    for (colorIndex = 0; colorIndex <= 15; colorIndex++) {
      u32 color;
      s32 blue;
      s32 green;
      s32 red;
      s32 adjustedColor;

      paletteIndex = bank * 16 + colorIndex;
      color = ((u16 *)BG_PLTT)[paletteIndex];
      blue = (color >> 10) & mask;
      green = (color >> 5) & mask;
      red = color & mask;
      blue += brightnessDelta;
      green += brightnessDelta;
      red += brightnessDelta;
      if (blue > 31)
        blue = 31;
      if (green > 31)
        green = 31;
      if (red > 31)
        red = 31;
      if (blue < 0)
        blue = 0;
      if (green < 0)
        green = 0;
      if (red < 0)
        red = 0;
      adjustedColor = blue << 10;
      adjustedColor |= green << 5;
      adjustedColor |= red;
      // Into the palette before this one.
      ((s16 *)BG_PLTT - 16)[paletteIndex] = adjustedColor;
    }
    if (bankIndex == 0) {
      bank = 5;
    } else {
      bank = 7;
      brightnessDelta = -12;
    }
    bankIndex++;
  } while (bankIndex <= 2);
}

/// The tile of BG character block 1 the backdrop's frame tile is copied to;
/// the preview screens blink it.
#define DJINN_BACKDROP_FRAME_TILE 150

/// OwnerAction_DiffSlots' marks on an action id: Psynergy the change gives,
/// drawn in colour 4, and Psynergy it takes away, in colour 2.
#define DJINN_PREVIEW_GAINED 0x8000
#define DJINN_PREVIEW_LOST 0x4000

/// Draws the backdrop behind the Djinn menu's list: its window, tiles and
/// palettes. First saves BG character block 1's first 8 KB and palettes 4 to 7
/// in the backdrop, which DjinnMenu_Run puts back as the menu closes. Palette
/// 5 gets the basic sprite colours, with the window background as colour 14,
/// for the element icons (DJINN_ELEMENT_TILE); palette 7 a copy of 15; then
/// Graphics_AdjustPaletteBank(8) makes 14, 4 and 6 from 15, 5 and 7.
MenuBackdropResult Menu_OpenBackdropScreen(void) {
  // UB: the implicit int call conflicts with DrawElementList's void definition.
  // The sole caller ignores this wrapper's result. Plain void changes the
  // native tbs-en return from pop {r1}; bx r1 to pop {r0}; bx r0.
  struct DjinnMenuWork *menu = gMenuWork;
  struct DjinnMenuBackdrop *backdrop = menu->backdrop;

  UiWindow_UpdateOrCreate(&menu->listWindow, 0, 5, 30, 15, 2);
  WaitFrames(1);
  Iwram_CopyWords(backdrop->tiles, BG_CHAR_BLOCK(1), 0x2000);
  Iwram_CopyWords(backdrop->palette, (void *)&BG_PLTT_COLOR(4, 0), 128);
  Iwram_FillWords(BG_CHAR_BLOCK(1), 0x2000, 0x33333333);
  Iwram_FillWords((void *)&BG_PLTT_COLOR(4, 0), 128, 0x55555555);
  Graphics_ExpandVramTilesByColorTableFar((u16 *)(BG_CHAR_BLOCK(1) + 0x1000));
  Iwram_CopyWords(BG_CHAR_BLOCK(1) + DJINN_BACKDROP_FRAME_TILE * TILE_SIZE_4BPP,
                  gMenu_BackdropFrameTile, TILE_SIZE_4BPP);
  Dma_Set((const void *)Palette_GetBasicObjColors(),
          (void *)&BG_PLTT_COLOR(5, 0), DMA_ENABLE << 16 | 16, REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  Dma_Set((void *)&BG_PLTT_COLOR(15, 0), (void *)&BG_PLTT_COLOR(7, 0),
          DMA_ENABLE << 16 | 16, REG_DMA3);
  Graphics_AdjustPaletteBank(8);
  BG_PLTT_COLOR(7, 4) = BG_PLTT_COLOR(15, 4);
  BG_PLTT_COLOR(6, 4) = BG_PLTT_COLOR(15, 4);
#ifdef UBFIX
  DjinnMenu_DrawElementList(&backdrop->list);
#else
  return DjinnMenu_DrawElementList(&backdrop->list);
#endif
}

/// Lists the actions after a preview, marking gains, then appends the losses.
/// The two output counts distinguish gained and lost actions.
///
/// Writes every action of after (up to 32), with DJINN_PREVIEW_GAINED on
/// those before lacks, then each action of before that after lacks, with
/// DJINN_PREVIEW_LOST. Returns the number written, up to 64.
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *before,
                          struct OwnerActionSlot *after, u16 *actionsOut,
                          s32 *gainedCountOut, s32 *lostCountOut) {
  s32 actionIndex;
  s32 otherIndex;
  s32 actionCount;
  s32 gainedCount;
  s32 lostCount;

  actionCount = 0;
  gainedCount = 0;
  lostCount = 0;
  for (actionIndex = 0;
       actionIndex < 32 && after[actionIndex].encodedAction != 0;
       actionIndex++) {
    actionsOut[actionCount] =
        after[actionIndex].encodedAction & OWNER_ACTION_ID_MASK;
    actionCount++;
    for (otherIndex = 0; otherIndex < 32; otherIndex++) {
      if (((after[actionIndex].encodedAction ^
            before[otherIndex].encodedAction) &
           OWNER_ACTION_ID_MASK) == 0)
        break;
    }
    if (otherIndex == 32) {
      gainedCount++;
      actionsOut[actionCount - 1] |= DJINN_PREVIEW_GAINED;
    }
  }
  for (actionIndex = 0;
       actionIndex < 32 && before[actionIndex].encodedAction != 0;
       actionIndex++) {
    for (otherIndex = 0; otherIndex < 32; otherIndex++) {
      if (((before[actionIndex].encodedAction ^
            after[otherIndex].encodedAction) &
           OWNER_ACTION_ID_MASK) == 0)
        break;
    }
    if (otherIndex == 32) {
      lostCount++;
      actionsOut[actionCount] =
          (before[actionIndex].encodedAction & OWNER_ACTION_ID_MASK) |
          DJINN_PREVIEW_LOST;
      actionCount++;
    }
  }
  *gainedCountOut = gainedCount;
  *lostCountOut = lostCount;
  return actionCount;
}

/// Rebuilds the Djinn lists in table: each owner's packed Djinn entries and
/// their count.
s32 DjinnMenu_BuildLists(void *table) {
  struct DjinnMenuWork *menu;
  s32 ownerIndex;
  u16 (*entries)[DJINN_LIST_ROWS];
  u16 *owners;
  s8 *counts;
  s16 entryCount;
  s32 ownerCount;

  menu = gMenuWork;
  ownerIndex = 0;
  if (ownerIndex < menu->ownerCount) {
    counts = ((struct DjinnListTable *)table)->counts;
    owners = menu->owners;
    entries = ((struct DjinnListTable *)table)->ids;
    do {
      entryCount = Djinn_ListOwnerEntries(*entries, *owners, -1);
      ownerCount = menu->ownerCount;
      ownerIndex += 1;
      *counts = entryCount;
      owners += 1;
      counts += 1;
      entries++;
    } while (ownerIndex < ownerCount);
  }
#ifdef UBFIX
  return 0;
#endif
}

/// Draws each owner's Djinn column, including set and recovering entries.
/// Rebuilds table's lists first. Each name follows its element's icon: in the
/// normal colour when set, colour 2 on standby and colour 4 while recovering.
/// Holds the tilemap upload (menuBusy) until the list is drawn.
void DjinnMenu_DrawElementList(struct DjinnListTable *table) {
  struct DjinnMenuWork *menu;
  s32 listIndex;
  s32 row;
  s32 element;
  struct UiWork *ui;
  s32 lineIndex;
  s32 canChangeState;
  u16 *entries;

  menu = WORK_SLOT(HEAP_SLOT_MENU);
  ui = WORK_SLOT(HEAP_SLOT_WINDOW);
  ui->menuBusy = 1;
  listIndex = 0;
  if (menu->ownerCount != 0) {
    do {
      table->counts[listIndex] = Djinn_ListOwnerEntries(
          table->ids[listIndex], menu->owners[listIndex], -1);
      listIndex++;
    } while (listIndex < menu->ownerCount);
  }
  RenderOutput_RedrawSavedRectFar(menu->listWindow);
  UiText_DrawCharacterAtOffsetFar(MsgUnleashEffect, menu->listWindow, 0, 80);
  for (row = 0; row < menu->ownerCount; row++) {
    entries = table->ids[row];
    lineIndex = 0;
    for (element = 0; element < DJINN_ELEMENTS; element++) {
      for (listIndex = 0; listIndex < table->counts[row]; listIndex++) {
        if (element != (entries[listIndex] & DJINN_ENTRY_ELEMENT) >> 5)
          continue;
        // Colour 2 when not set; colour 4 over it when it is neither set nor
        // ready to set, that is recovering (or held by a summon).
        if ((entries[listIndex] & DJINN_ENTRY_IS_SET) == 0)
          UiWork_SetParamNibbleFar(2);
        canChangeState = 0;
        if (Djinn_IsReadyFar((entries[listIndex] & DJINN_ENTRY_OWNER) >> 8,
                             (entries[listIndex] & DJINN_ENTRY_ELEMENT) >> 5,
                             entries[listIndex] & DJINN_ENTRY_INDEX) ||
            Djinn_IsActiveFar((entries[listIndex] & DJINN_ENTRY_OWNER) >> 8,
                              (entries[listIndex] & DJINN_ENTRY_ELEMENT) >> 5,
                              entries[listIndex] & DJINN_ENTRY_INDEX))
          canChangeState = 1;
        if (!canChangeState)
          UiWork_SetParamNibbleFar(4);
        UiWindow_SetTilemapEntryFar(
            menu->listWindow,
            ((entries[listIndex] & DJINN_ENTRY_ELEMENT) >> 5) +
                DJINN_ELEMENT_TILE,
            row * 7 + 1, lineIndex + 2, 0);
        UiText_DrawCharacterAtOffsetFar(
            MsgDjinnName +
                ((entries[listIndex] & DJINN_ENTRY_ELEMENT) >> 5) *
                    DJINN_PER_ELEMENT +
                (entries[listIndex] & DJINN_ENTRY_INDEX),
            menu->listWindow, row * 56 + 16, lineIndex * 8 + 16);
        lineIndex++;
        UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
      }
    }
  }
  UiWindow_DrawDividerLineFar(menu->listWindow, 0, 10, 28, 10);
  ((struct UiWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->dirty = 1;
  ui->menuBusy = 0;
}

/// Sets the palette of a rectangle of window's interior, (x, y) and its size in
/// tiles from the interior's corner, through UiWindow_SetPaletteBitRectFar.
/// Only bit 12 of each map entry changes, so palette 15 and 14 swap (the row
/// highlight) and so do 5 and 4 (the element icons).
s32 UiWindow_SetInteriorPaletteBitRect(const struct UiWindow *window, s32 x,
                                       s32 y, s32 width, s32 height,
                                       s32 palette) {
  UiWindow_SetPaletteBitRectFar(window->x + x + 1, window->y + y + 1, width,
                                height, palette);
#ifdef UBFIX
  return 0;
#endif
}

/// Moves each tile of a screen rectangle (in tiles) that shows palette 15 to
/// palette, and marks each row changed for upload. Clipped as
/// Render_SetTilemapFlagRect clips: to the screen's columns, and to its rows
/// only once the rectangle reaches row 29. The Djinn menu dims its windows
/// with palette 6 behind the help and the second chooser.
void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette) {
  struct UiRenderWork *render;
  u16 *tileCursor;
  s32 column;
  u32 tile;
  u32 masked;

  render = (struct UiRenderWork *)gWindowWork[0];
  palette <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29)
    width = 30 - x;
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29)
    height = 20 - y;

  if (width > 0 && height > 0) {
    do {
      tileCursor = &((u16 (*)[32])render->tilemap)[y][x];
      for (column = width; column != 0; column--) {
        tile = *tileCursor;
        if (((tile >> 12) & 15) == 15) {
          masked = tile & 0xffff0fff;
          *tileCursor = masked | palette;
        }
        tileCursor++;
      }
      UiRender_MarkRow(render, y);
      height--;
      y++;
    } while (height != 0);
  }
}

/// UiWindow_SetRectPalette on a rectangle of window's interior, (x, y) and its
/// size in tiles from the interior's corner.
void UiWindow_SetInteriorRectPalette(void *window, s32 x, s32 y, s32 width,
                                     s32 height, s32 palette) {
  UiWindow_SetRectPalette(((struct UiWindow *)window)->x + x + 1,
                          ((struct UiWindow *)window)->y + y + 1, width, height,
                          palette);
}

/// The Djinn menu's help, opened by Select on the first chooser: a list of
/// seven topics (Djinn, Set, Standby, Summon, Recovery, Class, Psynergy) with
/// the chosen one's text typed beside it. The d-pad moves through the topics,
/// and A moves on once the text has finished. Returns DJINN_RESULT_BACK for B
/// or Select, back to the list, or DJINN_RESULT_START for Start, which closes
/// the Djinn menu.
s32 DjinnMenu_ShowHelp(void) {
  struct UiWindow *helpWindow;
  struct UiWindow *statusWindow;
  struct DjinnMenuWork *menu;
  struct UiRenderWork *render;
  s32 result;
  s32 previous;
  s32 selection;
  struct UiWindow *list;
  struct UiChannelSlot *channel;
  s32 message;
  s32 helpIndex;
  s32 listMessage;

  menu = WORK_SLOT(HEAP_SLOT_MENU);
  render = WORK_SLOT(HEAP_SLOT_WINDOW);
  result = 0;
  previous = 0;
  selection = 0;
  RenderOutput_ClearListFar(menu->listWindow);
  WaitFrames(1);
  RenderOutput_PrepareForRedrawFar(menu->helpWindow);
  message = MsgDjinnInfoPrompt;
  UiText_DrawCharacterAtOffsetFar(message, menu->helpWindow, 0, 0);
  message++;
  UiText_DrawCharacterAtOffsetFar(message, menu->helpWindow, 0, 16);
  // Dim the party window and the Djinn list behind the help: palette 6 is
  // palette 15's text 12 steps darker (Menu_OpenBackdropScreen).
  UiWindow_SetRectPalette(1, 1, 11, 3, 6);
  UiWindow_SetInteriorRectPalette((void *)menu->listWindow, 0, 0, 28, 10, 6);
  list = UiWindow_CreateFar(0, 9, 8, 10, 6);
  helpWindow = UiWindow_CreateFar(8, 12, 22, 7, 2);
  statusWindow = UiWindow_CreateFar(8, 9, 22, 3, 2);
  UiWindow_MarkVisibleTileAttributesFar();
  helpIndex = 0;
  listMessage = MsgDjinnInfoTopic;
  do {
    UiText_DrawCharacterAtOffsetFar(helpIndex + listMessage, list, 0,
                                    helpIndex * 8);
    helpIndex++;
  } while (helpIndex <= 6);
  do {
    RenderOutput_PrepareForRedrawFar(statusWindow);
    UiText_DrawResourceFar(selection + MsgDjinnInfoTopic, statusWindow, 0, 0);
    channel = (struct UiChannelSlot *)UiText_OpenEntryMessageFar(
        helpWindow, selection + MsgDjinnInfoText);
    UiWindow_SetInteriorPaletteBitRect(list, 0, previous, 6, 1, 15);
    UiWindow_SetInteriorPaletteBitRect(list, 0, selection, 6, 1, 14);
    previous = selection;
    for (;;) {
      UiMenu_PositionCursor(-12, (list->y + selection) * 8 + 8);
      WaitFrames(1);
      if (gKeysRepeat & (KEY_RIGHT | KEY_DOWN)) {
        selection++;
        selection = Menu_WrapIndex(selection, 7);
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        break;
      }
      if (gKeysRepeat & (KEY_LEFT | KEY_UP)) {
        selection--;
        selection = Menu_WrapIndex(selection, 7);
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        break;
      } else if (gKeysPressed & KEY_START) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        result = DJINN_RESULT_START;
        break;
      } else if (gKeysPressed & (KEY_B | KEY_SELECT)) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        result = DJINN_RESULT_BACK;
        break;
      } else if (gKeysPressed & KEY_A) {
        if (UiWork_IsCompleteFar()) {
          selection++;
          selection = Menu_WrapIndex(selection, 7);
          AudioCommand_PlayFar(SE_MENU_CONFIRM);
          break;
        }
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      }
    }
    // Before the next topic: free the page arrow's VRAM slot the text took
    // (99: none), and stop the text's channel typing.
    if (render->glyphResource != 99) {
      Resource_ResetEntry(render->glyphResource);
      render->glyphResource = 99;
    }
    ((struct UiRenderWork *)gWindowWork[0])->pageArrowShown = 0;
    RenderOutput_PrepareForRedrawFar(helpWindow);
    {
      struct UiWindow *window = channel->work;

      window->frame = 0;
      window->duration = 0;
      window->state = 0;
    }
    channel->work = NULL;
  } while (result == 0);
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 1;
  RenderOutput_ClearListFar(statusWindow);
  RenderOutput_ClearListFar(helpWindow);
  WaitFrames(1);
  UiWork_FinalizeFar(statusWindow, 1);
  UiWork_FinalizeFar(list, 1);
  UiWork_FinalizeFar(helpWindow, 1);
  UiWindow_MarkVisibleTileAttributesFar();
  if (result == DJINN_RESULT_START) {
    RenderOutput_PrepareForRedrawFar(menu->helpWindow);
    RenderOutput_PrepareForRedrawFar(menu->listWindow);
    RenderOutput_PrepareForRedrawFar(menu->statusWindow);
    ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 0;
  }
  Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
  return result;
}

/// DJINN_ENTRY_IS_SET as a negative int, for entries built in s32.
#define DJINN_ENTRY_SET (-0x8000)

/// The flags DjinnMenu_SelectDjinn keeps for each member's column.
enum DjinnColumnFlag {
  DJINN_COLUMN_ON_HEADER = 1,        ///< the cursor is on the column's header
  DJINN_COLUMN_HEADER_CHOOSABLE = 2, ///< its header can be chosen
  DJINN_COLUMN_EMPTY = 4             ///< no Djinni to choose
};

/// The Djinn list's chooser. DJINN_SELECT_DJINNI chooses a Djinni in any
/// member's column; DJINN_SELECT_ACTION, for the Djinni chosen, chooses what to
/// do with it: its own column's header (Set or Standby), another member's
/// header (Give), or a Djinni in another column to trade with. The help window
/// says what R and Select do; the figures and the unleash text follow the
/// cursor.
///
/// The Djinni chooser returns DJINN_RESULT_CHOSEN for A, DJINN_RESULT_STATUS
/// for L and DJINN_RESULT_HELP for Select. R puts the Djinni on standby or sets
/// it at once; holding R, Select sets every Djinni or puts every one on
/// standby. The action chooser returns DJINN_RESULT_SET, _STANDBY, _GIVE or
/// _TRADE (A or R). Both return DJINN_RESULT_BACK for B and DJINN_RESULT_START
/// for Start, and record the choice in the work's selectedColumn, selection,
/// pairEntries, pairIndex, pairElement and pairOwner for mode.
///
/// During the tutorial (backdrop->tutorialStep) the sampled keys are replaced:
/// each step waits a set number of frames, shows its messages (the Djinn
/// powers' three waiting for the player's A after each), or presses a key
/// itself; DJINN_TUTORIAL_CLOSE waits for the player's A, then presses B.
s32 DjinnMenu_SelectDjinn(s32 mode) {
  s8 columnFlags[8];
  u8 requiresTrade[16];
  struct DjinnMenuWork *menu = gMenuWork;
  struct DjinnMenuBackdrop *backdrop = menu->backdrop;
  struct BattleUnit *unit;
  struct UiWindow *window;
  s32 redraw = 1;
  u32 refresh;
  s32 i;
  s32 j;
  s32 selectedRow;
  s32 ownerColumn = (u16)(menu->selection[mode] % 10);
  s32 djinnRow = (u16)(menu->selection[mode] / 10);
  s32 savedRow = 0;
  u32 djinn = 0;
  s32 groupMode = 0;
  s32 setAll = 0;
  s32 result;
  s32 message;
  u32 pressed;
  s32 count;
  u32 repeat;
  struct UiWindow *messageWindow;
  s32 tutorialStep;
  s8 *status;

  // Each column's DJINN_COLUMN_ flags. The Djinni chooser skips an empty
  // column; in the action chooser the chosen Djinni's own column offers only
  // its header, a member it can be given to its header too, and a member who
  // must trade none.
  selectedRow = -1;
  status = columnFlags;
  for (i = 7; i >= 0; i--)
    status[i] = 0;
  if (mode == DJINN_SELECT_DJINNI) {
    DjinnMenu_DrawElementList(&backdrop->list);
    for (i = 0; i < menu->ownerCount; i++) {
      if (backdrop->list.counts[i] == 0)
        status[i] = DJINN_COLUMN_EMPTY;
    }
    savedRow = djinnRow;
    for (i = 0; i < menu->ownerCount; i++) {
      if (status[ownerColumn] == DJINN_COLUMN_EMPTY) {
        ownerColumn++;
        ownerColumn = Menu_WrapIndex(ownerColumn, menu->ownerCount);
      }
    }
  } else {
    Djinn_MarkBalancedEntries((s8 *)requiresTrade, menu->selectedColumn[0]);
    for (i = 0; i < menu->ownerCount; i++) {
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
  // column (palette 6), and label each column's header with what choosing it
  // does.
  if (mode == DJINN_SELECT_ACTION) {
    s32 fromX;
    s32 fromY;
    u16 sourceCursor;
    s32 n;
    sourceCursor = menu->selection[0];
    fromX = (u16)((u32)sourceCursor % (u32)10);
    fromY = (u16)((u32)sourceCursor / (u32)10);
    UiWindow_SetInteriorPaletteBitRect(menu->listWindow, fromX * 7 + 1,
                                       fromY + 2, DJINN_CURSOR_WIDTH, 1, 14);
    UiWindow_SetInteriorRectPalette(menu->listWindow, fromX * 7 + 1, 2,
                                    DJINN_CURSOR_WIDTH, 7, 6);
    for (n = 0; n < menu->ownerCount; n++) {
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
      UiText_DrawCharacterAtOffsetFar(message, menu->listWindow, n * 56 + 8, 8);
    }
  }
  RenderOutput_RedrawSavedRectFar(menu->helpWindow);
  menu->cursor->active = ICON_SHOWN;
  // redraw is 1 for a full redraw and 2 after a cursor move, which also makes
  // the Djinni figure grow in again (FourObjectMotion_SetSlotPhase 0).
  for (;;) {
    if (redraw != 0) {
      refresh = redraw;
      redraw = 0;
      selectedRow = -1;
      if (!(columnFlags[ownerColumn] & DJINN_COLUMN_ON_HEADER))
        selectedRow = djinnRow;
      window = menu->statusWindow;
      unit = UnitGetFar(menu->owners[ownerColumn]);
      RenderOutput_RedrawSavedRectFar(window);
      UiText_DrawStringAtOffsetFar(unit->name, window, 0, 0);
      UiText_DrawCharacterAtOffsetFar(unit->classIndex + MsgClassName, window,
                                      0, 8);
      UiText_DrawStringAtOffsetFar(gDjinnMenu_TextLevel, window, 48, 0);
      UiText_DrawNumberInWindowFar(unit->level, 2, window, 72, 0);
      if (mode == DJINN_SELECT_DJINNI)
        UiText_DrawCharacterAtOffsetFar(MsgDjinnCharacterStatus, window, 0, 16);
      if (selectedRow != -1)
        djinn = backdrop->list.ids[ownerColumn][selectedRow];
      RenderOutput_RedrawSavedRectFar(menu->helpWindow);
      // DJINN_SELECT_ACTION's help: `<owner>'s`, the element icon and the
      // Djinni's name, an ability name argument (its action, from
      // ABILITY_DJINNI_FIRST), then "What will you do?". Spanish, French and
      // Italian put the icon first and the name in the one message.
      if (mode == DJINN_SELECT_ACTION) {
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
        UiWindow_SetTilemapEntryFar(
            menu->helpWindow,
            DJINN_ELEMENT(menu->pairEntries[0]) + DJINN_ELEMENT_TILE, 0, 0, 0);
        UiWork_PushValueSlotFar(menu->shownOwner[0], MSG_ARG_ACTOR);
        UiWork_PushValueSlotFar(DJINN_ELEMENT(menu->pairEntries[0]) * 20 +
                                    DJINN_NUMBER(menu->pairEntries[0]) + 300,
                                MSG_ARG_ABILITY);
        UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel, menu->helpWindow, 8,
                                        0);
#else
        UiWork_PushValueSlotFar(menu->shownOwner[0], MSG_ARG_ACTOR);
        UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel, menu->helpWindow, 0,
                                        0);
        UiWork_PushValueSlotFar(DJINN_ELEMENT(menu->pairEntries[0]) * 20 +
                                    DJINN_NUMBER(menu->pairEntries[0]) + 300,
                                MSG_ARG_ABILITY);
        UiWindow_SetTilemapEntryFar(
            menu->helpWindow,
            DJINN_ELEMENT(menu->pairEntries[0]) + DJINN_ELEMENT_TILE, 6, 0, 0);
        UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel + 1,
                                        menu->helpWindow, 56, 0);
#endif
        UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel + 2,
                                        menu->helpWindow, 0, 8);
      }
      // The Djinni's figure, its element's: animation 1 for one on standby or
      // recovering, 2 for one that is set. A recovering one gets
      // FourObjectMotion_SetSlotPosition's negative mark, which
      // FourObjectMotion_UpdateBottomRow turns into facing 0x8000 rather than
      // 0x4000. On a header the figure goes off screen (y 200).
      if (selectedRow == -1) {
        FourObjectMotion_SetSlotPosition(mode, 0, 200, 0);
      } else if (mode == DJINN_SELECT_DJINNI) {
        if (groupMode) {
          if (setAll == 0) {
            UiText_DrawCharacterAtOffsetFar(MsgDjinnSetAll, menu->helpWindow, 0,
                                            0);
          } else {
            UiText_DrawCharacterAtOffsetFar(MsgDjinnAllStandby,
                                            menu->helpWindow, 0, 0);
          }
          if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                               DJINN_NUMBER(djinn)) ||
              Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                DJINN_NUMBER(djinn))) {
            if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                 DJINN_NUMBER(djinn)))
              FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
            else
              FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
            FourObjectMotion_SetSlotPosition(mode, ownerColumn * 56 + 48, 62,
                                             0);
          } else {
            UiText_DrawCharacterAtOffsetFar(MsgDjinnRecovering,
                                            menu->helpWindow, 0, 16);
            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
            FourObjectMotion_SetSlotPosition(mode, ownerColumn * 56 + 48, 62,
                                             1);
          }
          if (refresh & 2)
            FourObjectMotion_SetSlotPhase(mode, 0);
        } else {
          UiText_DrawCharacterAtOffsetFar(MsgDjinnChoose, menu->helpWindow, 0,
                                          0);
          if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                               DJINN_NUMBER(djinn)) ||
              Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                DJINN_NUMBER(djinn))) {
            if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                 DJINN_NUMBER(djinn))) {
              UiText_DrawCharacterAtOffsetFar(MsgDjinnChoose + 3,
                                              menu->helpWindow, 0, 16);
              FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
            } else {
              UiText_DrawCharacterAtOffsetFar(MsgDjinnChoose + 2,
                                              menu->helpWindow, 0, 16);
              FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
            }
            FourObjectMotion_SetSlotPosition(mode, ownerColumn * 56 + 48, 62,
                                             0);
          } else {
            UiText_DrawCharacterAtOffsetFar(MsgDjinnChoose + 4,
                                            menu->helpWindow, 0, 16);
            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
            FourObjectMotion_SetSlotPosition(mode, ownerColumn * 56 + 48, 62,
                                             1);
          }
          if (refresh & 2)
            FourObjectMotion_SetSlotPhase(mode, 0);
        }
      } else {
        if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                             DJINN_NUMBER(djinn)) ||
            Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                              DJINN_NUMBER(djinn))) {
          if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                               DJINN_NUMBER(djinn)))
            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
          else
            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
          FourObjectMotion_SetSlotPosition(mode, ownerColumn * 56 + 48, 54, 0);
        } else {
          FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
          FourObjectMotion_SetSlotPosition(mode, ownerColumn * 56 + 48, 54, 1);
        }
        if (refresh & 2)
          FourObjectMotion_SetSlotPhase(mode, 0);
      }
      RenderOutput_ClearListFar(menu->listWindow);
      if (selectedRow != -1) {
        UiText_DrawCharacterAtOffsetFar(MsgUnleashEffect, menu->listWindow, 0,
                                        80);
#if LANGUAGE != LANGUAGE_JAPANESE
        UiWindow_ClearInteriorTilesFar(menu->listWindow, 0, 96, 224, 104);
        UiText_DrawCharacterAtOffsetFar(
            DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + MsgDjinnEffect,
            menu->listWindow, 0, 96);
#else
        UiText_DrawResourceFar(DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) +
                                   MsgDjinnEffect,
                               menu->listWindow, 0, 96);
#endif
      }
      if (!(columnFlags[ownerColumn] & DJINN_COLUMN_ON_HEADER))
        UiWindow_SetInteriorPaletteBitRect(menu->listWindow,
                                           ownerColumn * 7 + 1, djinnRow + 2,
                                           DJINN_CURSOR_WIDTH, 1, 14);
      ((struct UiWork *)gWindowWork[0])->dirty = 1;
    }
    if (columnFlags[ownerColumn] & DJINN_COLUMN_ON_HEADER)
      UiMenu_PositionCursor(ownerColumn * 56 - 8, 52);
    else
      UiMenu_PositionCursor(ownerColumn * 56 - 8, djinnRow * 8 + 60);
    WaitFrames(1);
    // Holding R after R on a Djinni keeps the group commands up; they
    // end when R is let go or pressed again (the latch is cleared when they
    // began).
    if (!(gKeysHeld & KEY_R) || (gKeysPressedLatch & KEY_R)) {
      if (groupMode)
        redraw = 1;
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
          UiMenu_PositionCursor(150, 26);
          WaitFrames(1);
        }
        pressed = KEY_B;
        repeat = KEY_B;
        break;
      // The recovery lesson: after its two messages, three recovery ticks
      // and Isaac's Flint goes back on standby.
      case DJINN_TUTORIAL_RECOVERY:
        if (backdrop->tutorialTimer == 60) {
          messageWindow =
              UiText_OpenMessageWindowFar(MsgDjinnTutorialRecovery, 9, 9, 1);
          gGameState.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          WaitFrames(1);
          messageWindow =
              UiText_OpenMessageWindowFar(MsgDjinnTutorialSetAgain, 9, 9, 1);
          gGameState.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          backdrop->tutorialTimer = 0;
          DjinnRecovery_TickFar();
          DjinnRecovery_TickFar();
          DjinnRecovery_TickFar();
          Djinn_DeactivateFar(ISAAC, ELEMENT_VENUS, DJINNI_FLINT);
          DjinnRecovery_AddStandbyFar(ISAAC, ELEMENT_VENUS, DJINNI_FLINT);
          UnitCalcFar(ISAAC);
          repeat = KEY_B;
          pressed = KEY_B;
        }
        break;
      case DJINN_TUTORIAL_GIVE:
        if (backdrop->tutorialTimer == 60) {
          messageWindow =
              UiText_OpenMessageWindowFar(MsgDjinnTutorialGive, 9, 9, 1);
          gGameState.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          UiWork_FinalizeFar(messageWindow, 1);
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
      case 6:
      case 7:
        if (backdrop->tutorialTimer == 60) {
          messageWindow = UiText_OpenMessageWindowFar(
              MsgDjinnTutorialSetAfterGive, 9, 9, 1);
          gGameState.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          UiWork_FinalizeFar(messageWindow, 1);
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
          gGameState.messageSpeed = MESSAGE_SPEED_NORMAL;
          messageWindow =
              UiText_OpenMessageWindowFar(MsgDjinnTutorialPowers, 9, 9, 1);
          UiMenu_SlideCursor(2, 146);
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          while (!(gKeysPressed & KEY_A)) {
            UiMenu_PositionCursor(2, 146);
            WaitFrames(1);
          }
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          WaitFrames(1);
          messageWindow =
              UiText_OpenMessageWindowFar(MsgDjinnTutorialUnleash, 9, 9, 1);
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          while (!(gKeysPressed & KEY_A)) {
            UiMenu_PositionCursor(2, 146);
            WaitFrames(1);
          }
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          WaitFrames(1);
          messageWindow =
              UiText_OpenMessageWindowFar(MsgDjinnTutorialDemonstrate, 9, 9, 1);
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          while (!(gKeysPressed & KEY_A)) {
            UiMenu_PositionCursor(2, 146);
            WaitFrames(1);
          }
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawElementList(&backdrop->list);
          WaitFrames(1);
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
        AudioCommand_PlayFar(SE_MENU_ERROR);
        continue;
      }
      i = 0;
      if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                           DJINN_NUMBER(djinn)) ||
          Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                            DJINN_NUMBER(djinn)))
        i = 1;
      groupMode = 1;
      gKeysPressedLatch = 0;
      if (i == 0) {
        AudioCommand_PlayFar(SE_MENU_ERROR);
        RenderOutput_ClearListFar(menu->listWindow);
        UiWindow_ClearInteriorTilesFar(menu->listWindow, 0, 80, 216, 104);
        UiText_DrawResourceFar(MsgDjinnNowRecovering, menu->listWindow, 0, 96);
        redraw = 1;
        continue;
      }
      if (djinn & DJINN_ENTRY_IS_SET) {
        AudioCommand_PlayFar(SE_175);
        Djinn_DeactivateFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                            DJINN_NUMBER(djinn));
        DjinnRecovery_AddStandbyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                    DJINN_NUMBER(djinn));
      } else {
        AudioCommand_PlayFar(SE_139);
        Djinn_ActivateFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                          DJINN_NUMBER(djinn));
        DjinnRecovery_RemoveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                DJINN_NUMBER(djinn));
      }
      UnitCalcFar(DJINN_OWNER(djinn));
      DjinnMenu_DrawElementList(&backdrop->list);
      redraw = 1;
    }
    if (mode == DJINN_SELECT_DJINNI && (pressed & KEY_L)) {
      result = DJINN_RESULT_STATUS;
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if ((pressed & KEY_A) ||
        (mode == DJINN_SELECT_ACTION && (pressed & KEY_R))) {
      i = 1;
      if (!(columnFlags[ownerColumn] & DJINN_COLUMN_ON_HEADER)) {
        if (selectedRow == -1) {
          AudioCommand_PlayFar(SE_MENU_ERROR);
          continue;
        }
        i = 0;
        if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                             DJINN_NUMBER(djinn)) ||
            Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                              DJINN_NUMBER(djinn)))
          i = 1;
      }
      if (i == 0) {
        AudioCommand_PlayFar(SE_MENU_ERROR);
        RenderOutput_ClearListFar(menu->listWindow);
        UiWindow_ClearInteriorTilesFar(menu->listWindow, 0, 80, 216, 104);
        UiText_DrawResourceFar(MsgDjinnNowRecovering, menu->listWindow, 0, 96);
        continue;
      }
      if (mode == DJINN_SELECT_ACTION) {
        if (columnFlags[ownerColumn] & DJINN_COLUMN_ON_HEADER) {
          if (ownerColumn == menu->selectedColumn[0]) {
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
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if (pressed & KEY_START) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_START;
      break;
    }
    if (pressed & KEY_B) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_BACK;
      break;
    }
    // Select: with R held, set every Djinni that is not recovering, or, the
    // next time, put every one on standby; otherwise open the help.
    if (mode == DJINN_SELECT_DJINNI && (pressed & KEY_SELECT)) {
      if (groupMode) {
        setAll ^= 1;
        if (setAll)
          AudioCommand_PlayFar(SE_139);
        else
          AudioCommand_PlayFar(SE_175);
        for (i = 0; i < menu->ownerCount; i++) {
          for (j = 0; j < backdrop->list.counts[i]; j++) {
            u32 entry = backdrop->list.ids[i][j];
            s32 usable;
            usable = 0;
            if (Djinn_IsReadyFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
                                 DJINN_NUMBER(entry)) ||
                Djinn_IsActiveFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
                                  DJINN_NUMBER(entry)))
              usable = 1;
            if (usable) {
              if (setAll) {
                if (!(entry & DJINN_ENTRY_IS_SET)) {
                  Djinn_ActivateFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
                                    DJINN_NUMBER(entry));
                  DjinnRecovery_RemoveFar(DJINN_OWNER(entry),
                                          DJINN_ELEMENT(entry),
                                          DJINN_NUMBER(entry));
                  UnitCalcFar(DJINN_OWNER(entry));
                }
              } else if (entry & DJINN_ENTRY_IS_SET) {
                Djinn_DeactivateFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
                                    DJINN_NUMBER(entry));
                DjinnRecovery_AddStandbyFar(DJINN_OWNER(entry),
                                            DJINN_ELEMENT(entry),
                                            DJINN_NUMBER(entry));
                UnitCalcFar(DJINN_OWNER(entry));
              }
            }
          }
        }
        DjinnMenu_DrawElementList(&backdrop->list);
        redraw = 1;
      } else {
        result = DJINN_RESULT_HELP;
        UiWindow_SetInteriorPaletteBitRect(menu->listWindow,
                                           ownerColumn * 7 + 1, djinnRow + 2,
                                           DJINN_CURSOR_WIDTH, 1, 15);
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        break;
      }
    }
    if (repeat & KEY_UP) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      if (columnFlags[ownerColumn] & DJINN_COLUMN_EMPTY)
        continue;
      UiWindow_SetInteriorPaletteBitRect(menu->listWindow, ownerColumn * 7 + 1,
                                         djinnRow + 2, DJINN_CURSOR_WIDTH, 1,
                                         15);
      if (columnFlags[ownerColumn] & DJINN_COLUMN_EMPTY)
        continue;
      if (columnFlags[ownerColumn] & DJINN_COLUMN_ON_HEADER) {
        columnFlags[ownerColumn] &= ~1;
        djinnRow = 0;
      } else if (djinnRow == 0 &&
                 (columnFlags[ownerColumn] & DJINN_COLUMN_HEADER_CHOOSABLE)) {
        columnFlags[ownerColumn] |= 1;
        redraw = 2;
        continue;
      }
      djinnRow--;
      count = backdrop->list.counts[ownerColumn];
      if (count == 0)
        count = 1;
      djinnRow = Menu_WrapIndex(djinnRow, count);
      savedRow = djinnRow;
      redraw = 2;
      continue;
    }
    if (repeat & KEY_DOWN) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      if (columnFlags[ownerColumn] & DJINN_COLUMN_EMPTY)
        continue;
      UiWindow_SetInteriorPaletteBitRect(menu->listWindow, ownerColumn * 7 + 1,
                                         djinnRow + 2, DJINN_CURSOR_WIDTH, 1,
                                         15);
      djinnRow++;
      count = backdrop->list.counts[ownerColumn];
      if (count == 0)
        count = 1;
      djinnRow = Menu_WrapIndex(djinnRow, count);
      if ((columnFlags[ownerColumn] & DJINN_COLUMN_ON_HEADER) &&
          !(columnFlags[ownerColumn] & DJINN_COLUMN_EMPTY)) {
        columnFlags[ownerColumn] &= ~1;
        djinnRow = 0;
      } else if (djinnRow == 0 &&
                 (columnFlags[ownerColumn] & DJINN_COLUMN_HEADER_CHOOSABLE)) {
        columnFlags[ownerColumn] |= 1;
      }
      savedRow = djinnRow;
      redraw = 2;
      continue;
    }
    if (repeat & KEY_LEFT) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      if (!(columnFlags[ownerColumn] & DJINN_COLUMN_EMPTY))
        UiWindow_SetInteriorPaletteBitRect(menu->listWindow,
                                           ownerColumn * 7 + 1, djinnRow + 2,
                                           DJINN_CURSOR_WIDTH, 1, 15);
      ownerColumn--;
      ownerColumn = Menu_WrapIndex(ownerColumn, menu->ownerCount);
      if (mode == DJINN_SELECT_DJINNI) {
        for (i = 0; i < menu->ownerCount; i++) {
          if (columnFlags[ownerColumn] == 4) {
            ownerColumn--;
            ownerColumn = Menu_WrapIndex(ownerColumn, menu->ownerCount);
          }
        }
      }
      djinnRow = savedRow;
      count = backdrop->list.counts[ownerColumn];
      if (count == 0)
        count = 1;
      djinnRow = Menu_WrapIndex(djinnRow, count);
      redraw = 2;
      continue;
    }
    if (repeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      if (!(columnFlags[ownerColumn] & DJINN_COLUMN_EMPTY))
        UiWindow_SetInteriorPaletteBitRect(menu->listWindow,
                                           ownerColumn * 7 + 1, djinnRow + 2,
                                           DJINN_CURSOR_WIDTH, 1, 15);
      ownerColumn++;
      ownerColumn = Menu_WrapIndex(ownerColumn, menu->ownerCount);
      if (mode == DJINN_SELECT_DJINNI) {
        for (i = 0; i < menu->ownerCount; i++) {
          if (columnFlags[ownerColumn] == 4) {
            ownerColumn++;
            ownerColumn = Menu_WrapIndex(ownerColumn, menu->ownerCount);
          }
        }
      }
      djinnRow = savedRow;
      count = backdrop->list.counts[ownerColumn];
      if (count == 0)
        count = 1;
      djinnRow = Menu_WrapIndex(djinnRow, count);
      redraw = 2;
    }
  }
  menu->selectedColumn[mode] = ownerColumn;
  if (selectedRow != -1) {
    u32 entry;
    menu->pairEntries[mode] = entry =
        backdrop->list.ids[ownerColumn][selectedRow];
    menu->pairIndex[mode] = entry % 32;
    menu->pairElement[mode] = DJINN_ELEMENT(entry);
    menu->pairOwner[mode] = DJINN_OWNER(entry);
    /* FAKEMATCH(barrier): an empty asm keeps entry live past the owner's
     * extraction, so the owner mask keeps its register; without it, 48 of
     * 2058 instructions differ in tbs-en. */
    asm("" : "+r"(result) : "r"(entry));
  }
  // The cursor, packed as column + row * 10.
  menu->selection[mode] = ownerColumn + djinnRow * 10;
  return result;
}

/// Lists owned Djinn by element and index, marking those that are set.
/// For element -1, entries also encode their owner.
///
/// Writes one Djinn list entry (DJINN_ENTRY_) for each Djinni of element the
/// unit holds, or of every element for -1, and returns how many. The Djinn
/// menu passes a DjinnListTable row, which holds DJINN_LIST_ROWS (10).
s32 Djinn_ListOwnerEntries(u16 *entriesOut, s32 ownerId, s32 element) {
  struct BattleUnit *unit = UnitGetFar(ownerId);
  s32 count = 0;
  s32 elementIndex;
  s32 djinnIndex;
  s32 encodedDjinn;

  if (element == -1) {
    for (elementIndex = 0; elementIndex < 4; elementIndex++) {
      for (djinnIndex = 0; djinnIndex < 20; djinnIndex++) {
        if (unit->djinnActive[elementIndex] & (1 << djinnIndex)) {
          encodedDjinn = (elementIndex << 5) | djinnIndex | DJINN_ENTRY_SET;
          encodedDjinn |= ownerId << 8;
          entriesOut[count++] = encodedDjinn;
        } else if (unit->djinnAvailable[elementIndex] & (1 << djinnIndex)) {
          entriesOut[count++] =
              (elementIndex << 5) | djinnIndex | (ownerId << 8);
        }
      }
    }
  } else {
    for (djinnIndex = 0; djinnIndex < 20; djinnIndex++) {
      if (unit->djinnActive[element] & (1 << djinnIndex))
        entriesOut[count++] = (element << 5) | djinnIndex | DJINN_ENTRY_SET;
      else if (unit->djinnAvailable[element] & (1 << djinnIndex))
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
  struct DjinnMenuWork *menu;

  menu = gMenuWork;
  if (mode == DJINN_COMPARE_TRADE) {
    DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->pairOwner[1], 1,
                              mode, DJINN_PREVIEW_TRADE, pageIndex, 1);
    DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pairOwner[0], mode, 1,
                              DJINN_PREVIEW_TRADE, pageIndex, mode);
  } else {
    DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->shownOwner[1], 1,
                              0, DJINN_SWAP_PREVIEW_MODE, pageIndex, 1);
    DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->shownOwner[0], 0, 0,
                              DJINN_PREVIEW_UNSET, pageIndex, 0);
  }
  return 1;
}

/// Copies a full unit for a temporary Djinn-stat preview.
static __inline__ void Unit_Copy(struct BattleUnit *destination,
                                 struct BattleUnit *source) {
  Iwram_CopyWords(destination, source, sizeof(struct BattleUnit));
}

/// Saves the unit, applies a hypothetical Djinn change, draws its differences
/// and restores the original unit before returning.
///
/// previewMode says the change (enum DjinnPreviewMode). Page 0 shows the
/// stats, class and set Djinn counts, with an arrow beside each stat that
/// changes; page n the nth page of the Psynergy list, new Psynergy in colour 4
/// and lost Psynergy in colour 2. side is the portrait slot. The saved copy is
/// a heap block freed before returning. Returns 1.
s32 DjinnMenu_DrawStatPreview(struct UiWindow *window, s32 x, s32 y,
                              s32 ownerId, s32 removePairSlot, s32 addPairSlot,
                              s32 previewMode, s32 page, s32 side) {
  struct BattleUnit *unit;
  struct BattleUnit *originalUnit;
  s32 giveElement;
  s32 giveIndex;
  s32 giveSet;
  s32 takeElement;
  s32 takeIndex;
  s32 takeSet;
  s32 lost;
  s32 gained;
  // OwnerAction_DiffSlots can write up to 64 entries here in the worst case
  // (32 kept or gained and 32 lost).
  u16 actions[48];
  struct DjinnMenuWork *menu;

  unit = UnitGetFar(ownerId);
  menu = gMenuWork;
  giveElement = menu->pairElement[removePairSlot];
  giveIndex = menu->pairIndex[removePairSlot];
  giveSet = menu->pairEntries[removePairSlot] & DJINN_ENTRY_IS_SET;
  takeElement = menu->pairElement[addPairSlot];
  takeIndex = menu->pairIndex[addPairSlot];
  takeSet = menu->pairEntries[addPairSlot] & DJINN_ENTRY_IS_SET;
  originalUnit = MemAlloc(sizeof(struct BattleUnit));
  Unit_Copy(originalUnit, unit);

  if (page == 0) {
    if (previewMode == DJINN_PREVIEW_NO_CHANGE) {
      UiText_DrawNumberInWindowFar(unit->maxHp, DJINN_PREVIEW_DIGITS, window,
                                   x * 8 + DJINN_PREVIEW_MAX_X, y * 8 + 56);
      UiText_DrawNumberInWindowFar(unit->maxPp, DJINN_PREVIEW_DIGITS, window,
                                   x * 8 + DJINN_PREVIEW_MAX_X, y * 8 + 64);
      UiText_DrawNumberInWindowFar(unit->hp, DJINN_PREVIEW_DIGITS, window,
                                   x * 8 + DJINN_PREVIEW_CURRENT_X, y * 8 + 56);
      UiText_DrawNumberInWindowFar(unit->pp, DJINN_PREVIEW_DIGITS, window,
                                   x * 8 + DJINN_PREVIEW_CURRENT_X, y * 8 + 64);
      UiText_DrawStringAtOffsetFar(gDjinnMenu_TextSlash, window,
                                   x * 8 + DJINN_PREVIEW_SLASH_X, y * 8 + 56);
      UiText_DrawStringAtOffsetFar(gDjinnMenu_TextSlash, window,
                                   x * 8 + DJINN_PREVIEW_SLASH_X, y * 8 + 64);
    } else {
      UiText_DrawNumberInWindowFar(unit->hp, DJINN_PREVIEW_DIGITS, window,
                                   x * 8 + DJINN_PREVIEW_CURRENT_X, y * 8 + 56);
      UiText_DrawNumberInWindowFar(unit->pp, DJINN_PREVIEW_DIGITS, window,
                                   x * 8 + DJINN_PREVIEW_CURRENT_X, y * 8 + 64);
    }
    UiText_DrawNumberInWindowFar(unit->atk, DJINN_PREVIEW_DIGITS, window,
                                 x * 8 + DJINN_PREVIEW_CURRENT_X, y * 8 + 72);
    UiText_DrawNumberInWindowFar(unit->def, DJINN_PREVIEW_DIGITS, window,
                                 x * 8 + DJINN_PREVIEW_CURRENT_X, y * 8 + 80);
    UiText_DrawNumberInWindowFar(unit->agi, DJINN_PREVIEW_DIGITS, window,
                                 x * 8 + DJINN_PREVIEW_CURRENT_X, y * 8 + 88);
    UiText_DrawNumberInWindowFar(unit->luck, 2, window,
                                 x * 8 + DJINN_PREVIEW_LUCK_X, y * 8 + 96);
  }

  // Only the temporary unit changes; the saved record is restored below.
  switch (previewMode) {
  case DJINN_PREVIEW_ADD_AND_SET:
    Djinn_AddToOwnerFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    Djinn_ActivateFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    break;
  case DJINN_PREVIEW_UNSET:
    giveIndex &= DJINN_ENTRY_INDEX;
    Djinn_DeactivateFar(ownerId, giveElement, giveIndex);
    break;
  case DJINN_PREVIEW_TRADE:
    if (giveSet) {
      giveIndex &= DJINN_ENTRY_INDEX;
      Djinn_DeactivateFar(ownerId, giveElement, giveIndex);
    }
    Djinn_AddToOwnerFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    if (takeSet)
      Djinn_ActivateFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    break;
  case DJINN_PREVIEW_ADD:
    Djinn_AddToOwnerFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    if (takeSet)
      Djinn_ActivateFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    break;
  }

  UnitCalcFar(ownerId);
  unit = UnitGetFar(ownerId);

  if (page == 0) {
    s32 label;

    UiText_DrawStringAtOffsetFar(unit->name, window, x * 8 + 40, y * 8);
    UiText_DrawStringAtOffsetFar(gDjinnMenu_TextLevel, window, x * 8 + 40,
                                 y * 8 + 16);
    UiText_DrawNumberInWindowFar(unit->level, 2, window, x * 8 + 88,
                                 y * 8 + 16);
    label = MsgPreviewStatLabel;
    UiText_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
    UiText_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
    UiText_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
    UiText_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
    UiText_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
    UiText_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
    UiText_DrawCharacterAtOffsetFar(MsgClassName + originalUnit->classIndex,
                                    window, x * 8, y * 8 + 32);
  }

  {
    s32 i = 0;

    if (page == 0) {
      s32 column;
#if LANGUAGE != LANGUAGE_JAPANESE
      s32 iconX;
#endif
      s32 px;

      if (originalUnit->classIndex != unit->classIndex) {
        px = x * 8;
        UiText_DrawCharacterAtOffsetFar(MsgClassName + unit->classIndex, window,
                                        px, y * 8 + 48);
        UiWindow_SetTilemapEntryFar(window, 0xf296, x + 2, 5, 0);
      }
      column = x;
      if (originalUnit->classIndex != unit->classIndex)
        column += 5;
      // Each element's icon and its count of set Djinn: 0xf030 + n is the
      // window font's digit n (character 0x30 + n) in palette 15.
      for (i = 0; i < 4; i++) {
        UiWindow_SetTilemapEntryFar(window, DJINN_ELEMENT_TILE + i,
                                    column + i * 2, y + 5, 0);
        UiWindow_SetTilemapEntryFar(window, unit->djinnActiveCounts[i] + 0xf030,
                                    column + i * 2 + 1, y + 5, 0);
      }
#if LANGUAGE != LANGUAGE_JAPANESE
      iconX = x * 8 + 70;
#endif
      if (unit->hp != originalUnit->hp) {
        UiText_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 72,
                                     y * 8 + 56);
        if (unit->hp > originalUnit->hp)
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 56, 0);
        else
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 56, 1);
      }
      if (unit->pp != originalUnit->pp) {
        UiText_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 72,
                                     y * 8 + 64);
        if (unit->pp > originalUnit->pp)
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 64, 0);
        else
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 64, 1);
      }
      if (unit->atk != originalUnit->atk) {
        UiText_DrawNumberInWindowFar(unit->atk, 4, window, x * 8 + 72,
                                     y * 8 + 72);
        if (unit->atk > originalUnit->atk)
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 72, 0);
        else
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 72, 1);
      }
      if (unit->def != originalUnit->def) {
        UiText_DrawNumberInWindowFar(unit->def, 4, window, x * 8 + 72,
                                     y * 8 + 80);
        if (unit->def > originalUnit->def)
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 80, 0);
        else
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 80, 1);
      }
      if (unit->agi != originalUnit->agi) {
        UiText_DrawNumberInWindowFar(unit->agi, 4, window, x * 8 + 72,
                                     y * 8 + 88);
        if (unit->agi > originalUnit->agi)
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 88, 0);
        else
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 88, 1);
      }
      if (unit->luck != originalUnit->luck) {
        UiText_DrawNumberInWindowFar(unit->luck, 2, window, x * 8 + 88,
                                     y * 8 + 96);
        if (unit->luck > originalUnit->luck)
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 96, 0);
        else
          UiIcon_DrawVariantWithTileOffset(
              window, DJINN_PREVIEW_ARROW_X(x, iconX), y * 8 + 96, 1);
      }
    }
  }

  // Pages 1 on: the Psynergy after the change, five to a page (six with no
  // change), then the key below: "* Psynergy Gained" in colour 4, "* Psynergy
  // Lost" in colour 2, or "* No change".
  if (page > 0) {
    s32 rows;
    s32 first;
    s8 count;
    s8 row;
    s8 line;

    rows = 6 - (previewMode != 3);
    first = rows * (page - 1);
    count = OwnerAction_DiffSlots(originalUnit->actionSlots, unit->actionSlots,
                                  actions, &gained, &lost);
    line = 0;
    for (row = 0; first < count && row < rows; row++, first++) {
      UiIcon_CreateWithLoadedResource(window, x * 8, (y + line * 2) * 8 + 4,
                                      actions[first] & OWNER_ACTION_ID_MASK);
      if (actions[first] & 0x8000)
        UiWork_SetParamNibbleFar(4);
      else if (actions[first] & 0x4000)
        UiWork_SetParamNibbleFar(2);
      else
        UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
      UiText_DrawCharacterAtOffsetFar(
          MsgAbilityName + (actions[first] & OWNER_ACTION_ID_MASK), window,
          x * 8 + 16, (y + line * 2) * 8 + 8);
      UiText_DrawNumberAtOffsetFar(BattleActGet(actions[first])->ppCost, 2,
                                   window, x * 8 + 88, (y + line * 2) * 8 + 8);
      line++;
    }
    UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
    UiText_DrawCharacterAtOffsetFar(MsgPsynergyPp, window,
                                    x * 8 + DJINN_PREVIEW_PP_X, y * 8);
    if (previewMode != 3) {
      s32 lines;

      lines = 0;
      if (gained) {
        UiWork_SetParamNibbleFar(4);
        UiText_DrawCharacterAtOffsetFar(MsgPsynergyGained, window, x * 8,
                                        y * 8 + 88);
        lines = 1;
      }
      if (lost) {
        UiWork_SetParamNibbleFar(2);
        UiText_DrawCharacterAtOffsetFar(MsgPsynergyLost, window, x * 8,
                                        (y + lines) * 8 + 88);
        lines++;
      }
      if (lines == 0)
        UiText_DrawCharacterAtOffsetFar(MsgPsynergyUnchanged, window, x * 8,
                                        y * 8 + 88);
      UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
      UiWindow_DrawDividerLineFar(window, 0, 11, 13, 11);
    }
    ((struct UiWork *)gWindowWork[0])->dirty = 1;
  }

  if (page == 0)
    SideObject_CreateFar(ownerId, 0, side, window, 0, 0);
  // None of the preview's class, stat or action changes escape this call.
  Unit_Copy(unit, originalUnit);
  MemFree(originalUnit);
  return 1;
}

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
#undef DJINN_BACKDROP_FRAME_TILE
#undef DJINN_PREVIEW_GAINED
#undef DJINN_PREVIEW_LOST
#undef DJINN_ENTRY_SET

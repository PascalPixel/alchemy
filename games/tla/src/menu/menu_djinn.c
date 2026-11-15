/// @file
/// The Djinn menu: its backdrop screen, the Djinn list and the screen's loop.
// Part of menu.c.

#include "menu_internal.h"
#include "resource_ids.h"
#include "flags.h"
#include "sounds.h"

/// The party's Djinn columns, one a member, 7 tiles wide, and the x of a
/// column's figure.
#define DJINN_COLUMN_TILES 7
#define DJINN_COLUMN_WIDTH (DJINN_COLUMN_TILES * 8)
#define DJINN_FIGURE_X(column) ((column) * DJINN_COLUMN_WIDTH + 48)

/// The cursor packed in DjinnMenuWork.selection: column + row * 10.
#define DJINN_CURSOR_ROW_STRIDE 10

/// Wraps a cursor index that has stepped one past either end of modulus
/// entries: -1 becomes modulus - 1 and modulus becomes 0.
s32 Menu_GetModuloOfSum(s32 value, s32 modulus) {
  return (value + modulus) % modulus;
}

/// Places the four party figures over their Djinn columns, 56 pixels apart
/// from x 32, at screen y 61 plus yOffset. Returns that y.
s32 Menu_SetFirstActorRowCoordinates(s32 yOffset) {
  struct DjinnMenuWork *menu;
  s32 memberX;
  s32 memberIndex;
  menu = Ram_HeapSlots->menuWork;
  yOffset += 61;
  memberX = 32;
  for (memberIndex = 0; memberIndex < 4; memberIndex++) {
    menu->memberX[memberIndex] = memberX;
    menu->memberY[memberIndex] = yOffset;
    memberX += DJINN_COLUMN_WIDTH;
  }
  return yOffset;
}

/// Returns the Djinn tutorial's message on giving a Djinni: the second set's
/// while flag 0x175 is set.
s32 Menu_GetDjinnGiveMessage(void) {
  if (GameFlag_Get(0x175))
    return (s32)MsgDjinnGiveTutorialB;
  return (s32)MsgDjinnGiveTutorialA;
}

void Resource_LoadPairedBlocksIfAvailable(void);

s32 DjinnMenu_RunScreen(void);

/// Runs the Djinn menu over the field: the field menu's Djinn screen
/// (FieldMenu_Run, FIELD_MENU_DJINN), and the Djinn tutorial. Types messages at
/// MESSAGE_SPEED_FAST while it runs (the tutorial's steps set
/// MESSAGE_SPEED_NORMAL for their messages, which stays until the menu closes)
/// and puts the player's speed back. Takes the menu work (HEAP_SLOT_MENU) and
/// the backdrop (struct DjinnMenuBackdrop, from the EWRAM pool), and frees
/// both before returning, after putting back the field tiles and palettes the
/// backdrop saved. Returns 1, which makes the field menu open again.
s32 DjinnMenu_Run(void) {
  struct DjinnMenuWork *menu;
  u32 savedMessageSpeed;
  s32 enabled;
  struct DjinnMenuBackdrop *backdrop;
  s32 zero;

  menu = HeapSlot_Alloc(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  savedMessageSpeed = gMainRoutineWork.messageSpeed;
  gMainRoutineWork.messageSpeed = MESSAGE_SPEED_FAST;
  enabled = Ram_HeapSlots->actorSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_InitializeWork(0);
  backdrop = Heap_AllocEwram(sizeof(*backdrop));
  menu->backdrop = backdrop;
  backdrop->tutorialTimer = 0;
  backdrop->tutorialStep = DJINN_TUTORIAL_OFF;
  // The tutorial's lesson, from the flags the world map sets for each.
  if (GameFlag_Get(FLAG_DJINN_LESSON)) {
    if (!GameFlag_Get(FLAG_DJINN_LESSON_PART_A)) {
      if (!GameFlag_Get(FLAG_DJINN_LESSON_PART_B))
        backdrop->tutorialStep = enabled;
      else
        backdrop->tutorialStep = DJINN_TUTORIAL_POWERS;
    } else {
      if (!GameFlag_Get(FLAG_DJINN_LESSON_PART_B))
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
  menu->pairEntries[0] = 255;
  zero = 0;
  menu->selectedColumn[0] = zero;
  menu->selectedColumn[1] = zero;
  menu->selection[0] = 0;
  menu->selection[1] = 0;
  DjinnFigures_InitializeTopRow();
  DjinnMenu_RunScreen();
  DjinnFigures_ReleaseTopRow();
  Menu_ResetTwoResourceEntries();
  wait(1);
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  Ram_HeapSlots->actorSystem->suspended = 0;
  Window_MarkVisibleTextTilesFar();
  Text_UseExtraTilesFar(0);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), backdrop->tiles,
                  sizeof(backdrop->tiles));
  Iwram_CopyWords((void *)&BG_PLTT_COLOR(4, 0), backdrop->palette, 128);
  wait(1);
  Menu_ResumeFieldTasks();
  Window_EraseBorderRectFar(0, 0, 30, 20);
  Heap_Release(menu->backdrop);
  HeapSlot_Release(HEAP_SLOT_MENU);
  Party_ClearUnusableShortcutsFar();
  gMainRoutineWork.messageSpeed = savedMessageSpeed;
  return 1;
}

void Menu_OpenBackdropScreen(void);

s32 DjinnMenu_SelectDjinn(s32 mode);
s32 DjinnMenu_ShowMemberStatus(void);
s32 DjinnMenu_ShowHelp(void);
s32 PsynergyList_RunCompareLoop(u32 mode);
s32 Djinn_TransferFar(s32 owner, s32 element, s32 index, s32 recipient);

u32 Djinn_PutOnStandbyFar(s32 owner, s32 element, s32 index);
s32 DjinnRecovery_AddStandbyFar(s32 owner, s32 element, s32 index);

void DjinnMenu_BuildLists(struct DjinnMenuBackdrop *backdrop);

// Find the moved Djinni in the list and select its new position.
#define DJINN_RESELECT_PAIR_ENTRY(menu, i)                                     \
  do {                                                                         \
    s32 column;                                                                \
    s32 found;                                                                 \
    u8 entry;                                                                  \
                                                                               \
    column = (u16)((u32)(menu)->selection[1] % 10);                            \
    found = 0;                                                                 \
    entry = (menu)->pairEntries[0];                                            \
    for ((i) = 0; (i) < (menu)->backdrop->counts[column]; (i)++) {             \
      if (entry == (u8)(menu)->backdrop->ids[column][(i)]) {                   \
        found = (i);                                                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    (menu)->selection[0] = column + found * 10;                                \
  } while (0)

/// The screen chooses an operation, optionally previews it, then commits it.
/// The Broken Seal's steps 1, 4 and 8, its trade's second choice, are gone.
enum DjinnMenuStep {
  /// After a chooser: a negative result closes the screen, any other goes back
  /// to choosing a Djinni.
  DJINN_STEP_HANDLE_RESULT = 0,
  DJINN_STEP_SELECT_DJINN = 2,
  DJINN_STEP_SELECT_ACTION = 3, ///< what to do with the chosen Djinni
  DJINN_STEP_CONFIRM_SET = 5,
  DJINN_STEP_CONFIRM_STANDBY = 6,
  DJINN_STEP_CONFIRM_GIVE = 7,
  DJINN_STEP_CONFIRM_TRADE = 9,
  /// The status of the chosen column's party member.
  DJINN_STEP_SHOW_STATUS = 10,
  DJINN_STEP_APPLY_SET = 11,
  DJINN_STEP_APPLY_STANDBY = 12,
  DJINN_STEP_APPLY_GIVE = 13,
  DJINN_STEP_APPLY_TRADE = 14,
  DJINN_STEP_SHOW_HELP = 15
};

/// The Djinn menu's screen loop, one step at a time: DjinnMenu_ShowMemberStatus
/// (step 10) and DjinnMenu_ShowHelp (step 15) are its status and help pages,
/// and the page arrows are hidden before each confirmation.
s32 DjinnMenu_RunScreen(void) {
  // icon holds the cursor, then each arrow in turn.
  struct DjinnMenuWork *menu;
  struct WindowSprite *icon;
  s32 result;
  s32 step;
  s32 menuResult;
  s32 done;
  s32 listIndex;

  done = 0;
  result = 0;
  menuResult = 0;
  menu = Ram_HeapSlots->menuWork;
  icon = menu->cursor;
  icon->motion = ICON_HIDDEN;
  icon->animStep = 0;
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
    case DJINN_STEP_SELECT_DJINN:
      Menu_SetFirstActorRowCoordinates(0);
      DjinnFigures_SetSlotPosition(1, 0, 200);
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
      icon = menu->pageArrows[0];
      icon->motion = ICON_HIDDEN;
      icon = menu->pageArrows[1];
      icon->motion = ICON_HIDDEN;
      result = DjinnMenu_ShowMemberStatus();
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_DJINN;
      break;
    case DJINN_STEP_SHOW_HELP:
      icon = menu->pageArrows[0];
      icon->motion = ICON_HIDDEN;
      icon = menu->pageArrows[1];
      icon->motion = ICON_HIDDEN;
      result = DjinnMenu_ShowHelp();
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_DJINN;
      break;
    case DJINN_STEP_SELECT_ACTION:
      Menu_SetFirstActorRowCoordinates(-8);
      menu->member = menu->members[menu->selectedColumn[0]];
      menu->shownMember[0] = menu->members[menu->selectedColumn[0]];
      DjinnFigures_SetSlotPosition(0, DJINN_FIGURE_X(menu->selectedColumn[0]),
                                   54);
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
      icon = menu->pageArrows[0];
      icon->motion = ICON_HIDDEN;
      icon = menu->pageArrows[1];
      icon->motion = ICON_HIDDEN;
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
      icon = menu->pageArrows[0];
      icon->motion = ICON_HIDDEN;
      icon = menu->pageArrows[1];
      icon->motion = ICON_HIDDEN;
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
      result = DjinnRecovery_AddStandbyFar(
          menu->shownMember[0], menu->pairElement[0], menu->pairIndex[0]);
      Unit_CalculateFar(menu->shownMember[0]);
      menu->cursor->motion = ICON_HIDDEN;
      WindowSprite_ClearListFar(menu->listWindow);
      menu->cursor->motion = ICON_SHOWN;
      step = DJINN_STEP_SELECT_DJINN;
      break;
    case DJINN_STEP_CONFIRM_TRADE:
      icon = menu->pageArrows[0];
      icon->motion = ICON_HIDDEN;
      icon = menu->pageArrows[1];
      icon->motion = ICON_HIDDEN;
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
    case DJINN_STEP_CONFIRM_SET:
      icon = menu->pageArrows[0];
      icon->motion = ICON_HIDDEN;
      icon = menu->pageArrows[1];
      icon->motion = ICON_HIDDEN;
      result = PsynergyList_RunCompareLoop(DJINN_COMPARE_SET);
      if (result == DJINN_RESULT_START)
        done = 1;
      step = DJINN_STEP_SELECT_ACTION;
      if (result < 0)
        break;
      // Confirmed: apply it at once.
      // Fall through.
    case DJINN_STEP_APPLY_SET:
      Sound_PlayFar(SE_139);
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

#undef DJINN_RESELECT_PAIR_ENTRY

/// Writes copies of background palettes 15, 5 and 7, each brightnessDelta
/// lighter in every channel (clamped to 0-31), into palettes 14, 4 and 6;
/// palette 7's copy is 12 darker instead.
void DjinnMenu_AdjustPalettes(s32 brightnessDelta) {
  s32 copyIndex;
  s32 paletteNumber;
  s32 colorIndex;
  s32 channelMask;
  s32 paletteIndex;

  paletteNumber = WINDOW_PALETTE;
  copyIndex = 0;
  channelMask = RGB_LEVEL_MAX;
  do {
    for (colorIndex = 0; colorIndex < PALETTE_COLORS; colorIndex++) {
      u32 color;
      s32 blue;
      s32 green;
      s32 red;
      s32 adjustedColor;

      paletteIndex = paletteNumber * PALETTE_COLORS + colorIndex;
      color = ((u16 *)BG_PLTT)[paletteIndex];
      blue = (color >> 10) & channelMask;
      green = (color >> 5) & channelMask;
      red = color & channelMask;
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
      adjustedColor = blue << 10;
      adjustedColor |= green << 5;
      adjustedColor |= red;
      ((s16 *)BG_PLTT - PALETTE_COLORS)[paletteIndex] = adjustedColor;
    }
    if (copyIndex == 0) {
      paletteNumber = 5;
    } else {
      paletteNumber = 7;
      brightnessDelta = -12;
    }
    copyIndex++;
  } while (copyIndex <= 2);
}

void Graphics_ExpandVramTilesByColorTableFar(void *tiles);
s32 Palette_GetBasicSpriteColors(void);

/// Opens the Djinn menu's backdrop screen: saves the tiles and palettes it
/// covers, draws the backdrop, lists the owners (DjinnMenu_BuildLists), then
/// makes the list's cursor and the two page arrows and places the arrows at the
/// list window's corners.
void Menu_OpenBackdropScreen(void) {
  struct DjinnMenuWork *menu = Ram_HeapSlots->menuWork;
  struct DjinnMenuBackdrop *backdrop = menu->backdrop;
  struct RenderInput *window;
  struct WindowSprite *icon;
  s32 entry;
  s32 x;
  s32 y;

  Window_UpdateOrCreate(&menu->listWindow, 0, 5, 30, 15, 2);
  wait(1);
  Iwram_CopyWords(backdrop->tiles, BG_CHAR_BLOCK(1), 0x2000);
  Iwram_CopyWords(backdrop->palette, (void *)&BG_PLTT_COLOR(4, 0), 128);
  Iwram_FillWords(BG_CHAR_BLOCK(1), 0x2000, 0x33333333);
  Iwram_FillWords((void *)&BG_PLTT_COLOR(4, 0), 128, 0x55555555);
  Graphics_ExpandVramTilesByColorTableFar(BG_CHAR_BLOCK(1) + 0x1000);
  Iwram_CopyWords(BG_CHAR_BLOCK(1) + 150 * 32, gMenu_BackdropBorderTile, 32);
  Dma_Set(Palette_GetBasicSpriteColors(), (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  Dma_Set((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0),
          (void *)&BG_PLTT_COLOR(7, 0), DMA_COPY_HALFWORDS(16), REG_DMA3);
  DjinnMenu_AdjustPalettes(8);
  BG_PLTT_COLOR(7, 4) = BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  BG_PLTT_COLOR(6, 4) = BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  DjinnMenu_BuildLists(backdrop);

  window = menu->statusWindow;
  WindowSprite_ClearListFar(window);
  icon = Icon_CreateWithResource(window, -8, 11);
  icon->motion = ICON_HIDDEN;
  menu->cursor = icon;
  entry = VramCache_FindFreeEntry();
  if (entry < 96) {
    VramBlock_LoadResourceFar(entry, 128, (s32)&ResourceId_ShopGemTiles);
    icon = WindowSprite_CreateFar(entry, 0x40000000, window, 0, 0);
    icon->motion = ICON_HIDDEN;
    menu->pageArrows[0] = icon;
    icon->sprite.oam.matrix |= 8;
    icon = WindowSprite_CreateFar(entry, 0x40000000, window, 0, 0);
    icon->motion = ICON_HIDDEN;
    menu->pageArrows[1] = icon;
  }

  window = menu->listWindow;
  icon = menu->pageArrows[0];
  x = window->x * 8;
  icon->sprite.oam.x = icon->x = x;
  y = window->y * 8 + 4;
  icon->sprite.oam.y = icon->y = y;
  icon = menu->pageArrows[1];
  x += 224;
  icon->sprite.oam.x = icon->x = x;
  icon->sprite.oam.y = icon->y = y;
}

/// PsynergyList_Diff's marks on an action id: Psynergy the change gives,
/// drawn in TEXT_COLOR_YELLOW, and Psynergy it takes away, in TEXT_COLOR_RED.
#define DJINN_PREVIEW_GAINED 0x8000
#define DJINN_PREVIEW_LOST 0x4000

/// Lists the actions after a preview, marking gains, then appends the losses.
/// The two output counts distinguish gained and lost actions.
///
/// Writes every action of after (up to 32), with PREVIEW_GAINED on those
/// before lacks, then each action of before that after lacks, with
/// PREVIEW_LOST. Returns the number written, up to 64.
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

s32 Djinn_ListUnitEntries(u16 *entriesOut, s32 memberId, s32 element);

/// Lists each party member's Djinn of every element (Djinn_ListUnitEntries)
/// into the backdrop's lists, with their counts.
void DjinnMenu_BuildLists(struct DjinnMenuBackdrop *backdrop) {
  struct ItemMenuWork *menu = (struct ItemMenuWork *)Ram_HeapSlots->menuWork;
  s32 i;

  for (i = 0; i < menu->partyCount; i++) {
    s32 owner = (u16)menu->memberIds[i];

    backdrop->counts[i] = Djinn_ListUnitEntries(backdrop->ids[i], owner, -1);
  }
}

extern u8 MsgDjinnName[]; ///< the first djinni's name

s32 Djinn_CanSetFar(s32 owner, s32 element, s32 index);
s32 Djinn_IsSetFar(s32 owner, s32 element, s32 index);

/// The state of a packed djinni entry (owner in bits 8-11, element in 5-7,
/// index in 0-4, bit 15 set): 2 when the djinni is neither ready nor active,
/// otherwise whether bit 15 is set. DjinnMenu_DrawPage colours each name by it.
s32 DjinnMenu_GetEntryState(u32 entry) {
  s32 owner = DJINN_MEMBER(entry);
  s32 element = DJINN_ELEMENT(entry);
  s32 index = DJINN_NUMBER(entry);
  s32 state;

  if (Djinn_CanSetFar(owner, element, index) ||
      Djinn_IsSetFar(owner, element, index)) {
    u32 set = entry & DJINN_ENTRY_IS_SET;

    state = ((0u - set) | set) >> 31;
  } else {
    state = 2;
  }
  return state;
}

/// Draws a packed djinni entry in a window: its name 8 pixels right of (x, y)
/// and its element's icon tile at the tile under (x, y), then resets the text
/// colour. The names run 20 to an element.
void DjinnMenu_DrawEntry(s32 window, s32 x, s32 y, s32 entry) {
  s32 element = (entry & DJINN_ENTRY_ELEMENT) / 32;

  Text_DrawMessageAsTilesFar((s32)MsgDjinnName + element * DJINN_PER_ELEMENT +
                                 (entry & 31),
                             window, x + 8, y);
  Window_SetTilemapEntryFar(window, element + ELEMENT_ICON_TILE, x / 8, y / 8,
                            0);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
}

void DjinnMenu_DrawPage(struct DjinnMenuBackdrop *backdrop, s32 page) {
  struct DjinnMenuWork *menu = Ram_HeapSlots->menuWork;
  u8 *tiles = Ram_HeapSlots->windowWork;
  s32 window = menu->listWindow;
  s32 owners;
  s32 column;
  s32 element;
  s32 i;
  s32 x;
  s32 y;
  s32 djinn;

  tiles[6] = 1;
  WindowSprite_RedrawSavedRectFar(window);
  owners = Party_CountMembersFar();
  djinn = Party_SumDjinnCountsFar(-1);
  if (owners != 0 && (djinn - 1) / owners <= 6) {
    Text_DrawMessageAsTilesFar((s32)MsgDjinnUnleashEffectLabel, window, 0, 80);
    Window_DrawDividerLineFar(window, 0, 10, 28, 10);
  }
  DjinnMenu_BuildLists(backdrop);
  for (column = 0, x = 8; column <= 3; column++, x += 56) {
    u16 *entries = backdrop->ids[page + column];

    y = 16;
    if (menu->memberSprites[column] != NULL) {
      for (element = 0; element <= 3; element++) {
        for (i = 0; i < backdrop->counts[page + column]; i++) {
          if (element == DJINN_ELEMENT(entries[i])) {
            switch (DjinnMenu_GetEntryState(entries[i])) {
            case 0:
              Text_SetColorFar(TEXT_COLOR_RED);
              break;
            case 1:
              Text_SetColorFar(TEXT_COLOR_NORMAL);
              break;
            default:
              Text_SetColorFar(TEXT_COLOR_YELLOW);
              break;
            }
            DjinnMenu_DrawEntry(window, x, y, entries[i]);
            y += 8;
          }
        }
      }
      // From patch 4 each column waits a frame after it is drawn.
#if TLA_PATCH_4
      wait(1);
#endif
    }
  }
  // The Japanese release reads the window tiles again for the first store
  // and does not wait; from patch 1 the page waits three frames.
#if TLA_PATCH_1
  tiles[3] = 1;
  tiles[6] = 0;
  wait(3);
#else
  Ram_HeapSlots->windowWork[3] = 1;
  tiles[6] = 0;
#endif
}

// A grid cell's highlight is a tile narrower in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define GRID_CELL_WIDTH 5
#else
#define GRID_CELL_WIDTH 6
#endif

void DjinnMenu_DrawFirstPage(struct DjinnMenuBackdrop *backdrop) {
  DjinnMenu_DrawPage(backdrop, 0);
}

void Window_SetInteriorPaletteBitRect(struct RenderInput *panel, s32 x, s32 y,
                                      s32 width, s32 height, s32 style) {
  Window_HighlightRectFar((s16)panel->x + x + 1, (s16)panel->y + y + 1, width,
                          height, style);
}

/// Highlights the cell of a four-column grid at index's column and row's line,
/// with style 14 when selected and 15 otherwise.
void UiPanel_HighlightGridCell(struct RenderInput *panel, s32 index, s32 row,
                               s32 selected) {
  s32 style = ((0u - selected) | selected) >> 31;

  style = 15 - style;
  Window_SetInteriorPaletteBitRect(panel, index % 4 * 7 + 1, row + 2,
                                   GRID_CELL_WIDTH, 1, style);
}

#undef GRID_CELL_WIDTH

/// Gives every cell of a tile rectangle that uses palette 15 the palette
/// given instead, clipped to the screen, and marks its rows for upload.
void Window_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette) {
  struct WindowWork *tiles = (struct WindowWork *)Ram_HeapSlots->windowWork;

  palette <<= 12;

  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > (DISPLAY_TILE_WIDTH - 1)) {
    width = DISPLAY_TILE_WIDTH - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = DISPLAY_TILE_HEIGHT - y;
  }

  if (width > 0 && height > 0) {
    do {
      u16 *p = &tiles->tilemap[y][x];
      s32 col = width;

      if (col != 0) {
        do {
          u32 tile = *p;

          if (((tile >> 12) & 15) == WINDOW_PALETTE) {
            tile &= ~0xf000;
            tile |= palette;
            *p = tile;
          }
          col--;
          p++;
        } while (col != 0);
      }
      /* FAKEMATCH(dead): the dirty bit's update in a once-only block. */
      do {
        tiles->dirty |= 2 << ((u32)y >> 2);
      } while (0);

      height--;
      y++;
    } while (height != 0);
  }
}

/// Window_SetRectPalette on a rectangle of window's interior, (x, y) and its
/// size in tiles from the interior's corner.
void Window_SetInteriorRectPalette(struct RenderInput *panel, s32 x, s32 y,
                                   s32 width, s32 height, s32 palette) {
  Window_SetRectPalette((s16)panel->x + x + 1, (s16)panel->y + y + 1, width,
                        height, palette);
}

struct UiChannelSlot *Message_OpenDecodedFar(void *window, s32 message);

/// The Djinn menu's help, opened by Select on the first chooser: a list of
/// seven topics (Djinn, Set, Standby, Summon, Recovery, Class, Psynergy) with
/// the chosen one's text typed beside it. The d-pad moves through the topics,
/// and A moves on once the text has finished. Returns DJINN_RESULT_BACK for B
/// or Select, back to the list, or DJINN_RESULT_START for Start, which closes
/// the Djinn menu.
s32 DjinnMenu_ShowHelp(void) {
  struct RenderInput *helpWindow;
  struct RenderInput *statusWindow;
  struct DjinnMenuWork *menu;
  struct WindowWork *render;
  s32 result;
  s32 previous;
  s32 selection;
  struct RenderInput *list;
  struct UiChannelSlot *channel;
  s32 message;
  s32 helpIndex;
  s32 listMessage;

  menu = Ram_HeapSlots->menuWork;
  selection = 0;
  render = (struct WindowWork *)Ram_HeapSlots->windowWork;
  result = 0;
  previous = 0;
  WindowSprite_ClearListFar(menu->listWindow);
  wait(1);
  WindowSprite_PrepareForRedrawFar(menu->helpWindow);
  message = (s32)MsgDjinnInfoPrompt;
  Text_DrawMessageAsTilesFar(message, menu->helpWindow, 0, 0);
  message++;
  Text_DrawMessageAsTilesFar(message, menu->helpWindow, 0, 16);
  Window_SetRectPalette(1, 1, 11, 3, 6);
  Window_SetInteriorRectPalette(menu->listWindow, 0, 0, 28, 10, 6);
  list = Window_CreateFar(0, 9, 8, 10, 6);
  helpWindow = Window_CreateFar(8, 12, 22, 7, WINDOW_FLAG_INSTANT);
  statusWindow = Window_CreateFar(8, 9, 22, 3, WINDOW_FLAG_INSTANT);
  Window_MarkVisibleTextTilesFar();
  helpIndex = 0;
  listMessage = (s32)MsgDjinnInfoTopic;
  do {
    /* FAKEMATCH(temporary): the topic's row held in y before the call. */
    s32 y = helpIndex * 8;

    Text_DrawMessageAsTilesFar(helpIndex + listMessage, list, 0, y);
    helpIndex++;
  } while (helpIndex <= 6);
  do {
    WindowSprite_PrepareForRedrawFar(statusWindow);
    Text_DrawMessageFar(selection + (s32)MsgDjinnInfoTopic, statusWindow, 0, 0);
    channel =
        Message_OpenDecodedFar(helpWindow, selection + (s32)MsgDjinnInfoText);
    Window_SetInteriorPaletteBitRect(list, 0, previous, 6, 1, WINDOW_PALETTE);
    Window_SetInteriorPaletteBitRect(list, 0, selection, 6, 1,
                                     HIGHLIGHT_PALETTE);
    previous = selection;
    for (;;) {
      MenuCursor_Position(-12, ((s16)list->y + selection) * 8 + 8);
      wait(1);
      if (gInput.repeat & (KEY_RIGHT | KEY_DOWN)) {
        selection++;
        selection = Menu_GetModuloOfSum(selection, 7);
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        break;
      }
      if (gInput.repeat & (KEY_LEFT | KEY_UP)) {
        selection--;
        selection = Menu_GetModuloOfSum(selection, 7);
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        break;
      } else if (gInput.pressed & KEY_START) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = -2;
        break;
      } else if (gInput.pressed & (KEY_B | KEY_SELECT)) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = -1;
        break;
      } else if (gInput.pressed & KEY_A) {
        if (Message_AllTypedFar()) {
          selection++;
          selection = Menu_GetModuloOfSum(selection, 7);
          Sound_PlayFar(SE_MENU_CONFIRM);
          break;
        }
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      }
    }
    if (render->glyphResource != NO_GLYPH_RESOURCE) {
      VramCache_FreeEntry(render->glyphResource);
      render->glyphResource = NO_GLYPH_RESOURCE;
    }
    ((struct WindowWork *)Ram_HeapSlots->windowWork)->pageArrowShown = 0;
    WindowSprite_PrepareForRedrawFar(helpWindow);
    {
      struct Window *window = channel->work;

      window->zoomStage = 0;
      window->duration = 0;
      window->state = 0;
    }
    channel->work = 0;
  } while (result == 0);
  ((struct WindowWork *)Ram_HeapSlots->windowWork)->holdTilemapUpload = 1;
  WindowSprite_ClearListFar(statusWindow);
  WindowSprite_ClearListFar(helpWindow);
  wait(1);
  Window_CloseFar(statusWindow, 1);
  Window_CloseFar(list, 1);
  Window_CloseFar(helpWindow, 1);
  Window_MarkVisibleTextTilesFar();
  if (result == -2) {
    WindowSprite_PrepareForRedrawFar(menu->helpWindow);
    WindowSprite_PrepareForRedrawFar(menu->listWindow);
    WindowSprite_PrepareForRedrawFar(menu->statusWindow);
    ((struct WindowWork *)Ram_HeapSlots->windowWork)->holdTilemapUpload = 0;
  }
  Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PASS_VBLANK);
  return result;
}

// A grid cell's highlight is a tile narrower in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define GRID_CELL_WIDTH 5
#else
#define GRID_CELL_WIDTH 6
#endif

// Spanish, French and Italian name the Djinni before its owner in one message
// on the trade's help line, so the Djinni's icon leads the line.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define TRADE_HELP_DJINNI_FIRST 1
#else
#define TRADE_HELP_DJINNI_FIRST 0
#endif

extern u8 gDjinnMenu_TextLevel[];

s32 Djinn_MarkTradeOnlyRecipients(s8 *table, s32 self);

void Menu_SetPageArrowsB(s32 index, s32 count);
void DjinnFigures_SetSlotDepth(s32 index, s32 frame);
void DjinnFigures_ReplaceSlot(s32 index, s32 kind, s32 animation);

void DjinnRecovery_CountDownFar(void);
s32 Menu_GetModuloOfSum(s32 value, s32 count) __attribute__((const));

// Shows one tutorial message and waits for its text to finish.
#define DJINN_TUTORIAL_MESSAGE(window, offset)                                 \
  do {                                                                         \
    (window) =                                                                 \
        Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + (offset), 9, 9, 1); \
    gMainRoutineWork.messageSpeed = MESSAGE_SPEED_NORMAL;                      \
    while (Message_AllTypedFar() == 0)                                         \
      wait(1);                                                                 \
  } while (0)

/// The Djinn list's chooser.
s32 DjinnMenu_SelectDjinn(s32 mode) {
  s8 columnFlags[8];
  u8 requiresTrade[16];
  struct DjinnMenuWork *menu = Ram_HeapSlots->menuWork;
  struct DjinnMenuBackdrop *backdrop = menu->backdrop;
  s32 helpWindow = menu->helpWindow;
  s32 listWindow = menu->listWindow;
  struct Unit *unit;
  s32 window;
  u32 redraw = 4;
  s32 i;
  s32 j;
  s32 selectedRow;
  s32 memberColumn = (u16)(menu->selection[mode] % 10);
  s32 djinnRow = (u16)(menu->selection[mode] / 10);
  s32 savedRow = 0;
  u32 djinn = 0;
  s32 groupMode = 0;
  s32 setAll = 0;
  s32 owner = 0;
  s32 element = 0;
  s32 number = 0;
  s32 state = 0;
  s32 result;
  s32 message;
  u32 pressed;
  u32 repeat;
  s32 messageWindow;
  s8 *status;

  selectedRow = -1;
  status = columnFlags;
  for (i = 7; i >= 0; i--)
    status[i] = 0;
  if (mode == 0) {
    savedRow = djinnRow;
    for (i = 0; i < menu->memberCount; i++) {
      if (backdrop->counts[i] == 0)
        status[i] = 4;
    }
    for (i = 0; i < menu->memberCount; i++) {
      if (status[memberColumn] == 4) {
        memberColumn++;
        memberColumn = Menu_GetModuloOfSum(memberColumn, menu->memberCount);
      }
    }
  } else {
    Djinn_MarkTradeOnlyRecipients((s8 *)requiresTrade, menu->selectedColumn[0]);
    for (i = 0; i < menu->memberCount; i++) {
      if (i == menu->selectedColumn[0]) {
        columnFlags[i] = 7;
      } else if (requiresTrade[i] != 0) {
        columnFlags[i] = 0;
      } else {
        columnFlags[i] = 3;
        if (backdrop->counts[i] == 0)
          status[i] = columnFlags[i] | 4;
      }
    }
  }
  menu->cursor->motion = ICON_SHOWN;
  for (;;) {
    if (redraw != 0) {
      selectedRow = -1;
      if (!(columnFlags[memberColumn] & 1))
        selectedRow = djinnRow;
      if (columnFlags[memberColumn] & 1)
        MenuCursor_Position(memberColumn % 4 * 56 - 8, 52);
      else
        MenuCursor_Position(memberColumn % 4 * 56 - 8, djinnRow * 8 + 60);
      if (redraw >> 2) {
        s32 page = memberColumn / 4 * 4;

        Menu_CreateOwnerEntryObjects(page);
        DjinnMenu_DrawPage(backdrop, page);
        if (mode == 1) {
          s32 fromX;
          s32 fromY;
          u16 sourceCursor;
          s32 x;
          s32 height;

          sourceCursor = menu->selection[0];
          fromX = (u16)((u32)sourceCursor % (u32)10);
          fromY = (u16)((u32)sourceCursor / (u32)10);
          if (fromX / 4 * 4 == page) {
            height = backdrop->counts[fromX];
            UiPanel_HighlightGridCell(listWindow, fromX, fromY, 1);
            Window_SetInteriorRectPalette(listWindow, (fromX - page) * 7 + 1, 2,
                                          GRID_CELL_WIDTH, height, 6);
            menu->slotY[0] = 54;
          } else {
            menu->slotY[0] = 200;
          }
          for (i = 0, x = 8; i <= 3; i++, x += 56) {
            if (menu->memberSprites[i] != NULL) {
              if (page + i == menu->selectedColumn[0]) {
                if (menu->pairEntries[0] & 0x8000)
                  message = (s32)MsgDjinnStandbyAction;
                else
                  message = (s32)MsgDjinnSetAction;
              } else if (columnFlags[page + i] & 2) {
                message = (s32)MsgDjinnGiveAction;
              } else {
                message = (s32)MsgDjinnTradeAction;
              }
              Text_DrawMessageAsTilesFar(message, listWindow, x, 8);
            }
          }
        }
        Menu_SetPageArrowsB(memberColumn, menu->memberCount);
#if !TLA_PATCH_1
        wait(1);
#endif
      }
      if (!(columnFlags[memberColumn] & 1))
        UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 1);
      if (selectedRow != -1)
        djinn = backdrop->ids[memberColumn][selectedRow];
      owner = DJINN_MEMBER(djinn);
      element = DJINN_ELEMENT(djinn);
      number = DJINN_NUMBER(djinn);
      state = DjinnMenu_GetEntryState(djinn);
      if (selectedRow == -1) {
        DjinnFigures_SetSlotPosition(mode, 0, 200);
      } else {
        s32 frame;
        s32 animation;

        switch (state) {
        case 0:
          animation = 1;
          frame = 0x4000;
          break;
        case 1:
          animation = 2;
          frame = 0x4000;
          break;
        default:
          animation = 1;
          frame = 0x8000;
          break;
        }
        DjinnFigures_SetSlotPosition(mode, memberColumn % 4 * 56 + 48,
                                     mode == 0 ? 62 : 54);
        DjinnFigures_SetSlotDepth(mode, frame);
        if (redraw & 2)
          DjinnFigures_SetSlotPhase(mode, 0);
        DjinnFigures_ReplaceSlot(mode, element, animation);
      }
      window = menu->statusWindow;
      unit = Unit_Get(menu->members[memberColumn]);
      WindowSprite_RedrawSavedRectFar(window);
      Text_DrawStringAtOffsetFar(unit->name, window, 0, 0);
      Text_DrawMessageAsTilesFar(unit->classId + (s32)MsgClassName, window, 0,
                                 8);
      Text_DrawStringAtOffsetFar(gDjinnMenu_TextLevel, window, 48, 0);
      Text_DrawNumberInWindowFar(unit->level, 2, window, 72, 0);
      if (mode == 0)
        Text_DrawMessageAsTilesFar((s32)MsgDjinnCharacterStatus, window, 0, 16);
      WindowSprite_RedrawSavedRectFar(helpWindow);
      if (mode == 0 && selectedRow != -1) {
        if (groupMode) {
          if (setAll == 0)
            Text_DrawMessageAsTilesFar((s32)MsgDjinnSetAll, helpWindow, 0, 0);
          else
            Text_DrawMessageAsTilesFar((s32)MsgDjinnAllStandby, helpWindow, 0,
                                       0);
          if (state == 2)
            Text_DrawMessageAsTilesFar((s32)MsgDjinnRecovering, helpWindow, 0,
                                       16);
        } else {
          Text_DrawMessageAsTilesFar((s32)MsgDjinnChoose, helpWindow, 0, 0);
          switch (state) {
          case 0:
            message = (s32)MsgDjinnChoose + 3;
            break;
          case 1:
            message = (s32)MsgDjinnChoose + 2;
            break;
          default:
            message = (s32)MsgDjinnRecovering;
            break;
          }
          Text_DrawMessageAsTilesFar(message, helpWindow, 0, 16);
        }
      }
#if TLA_PATCH_1
      wait(1);
#endif
      if (mode == 1) {
#if TRADE_HELP_DJINNI_FIRST
        Window_SetTilemapEntryFar(
            helpWindow, DJINN_ELEMENT(menu->pairEntries[0]) + ELEMENT_ICON_TILE,
            0, 0, 0);
        MessageArgument_PushFar(menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_PushFar(DJINN_ELEMENT(menu->pairEntries[0]) * 20 +
                                    DJINN_NUMBER(menu->pairEntries[0]) + 300,
                                MESSAGE_ARGUMENT_ABILITY);
        Text_DrawMessageAsTilesFar((s32)MsgDjinnOwnerLabel, helpWindow, 8, 0);
#else
        MessageArgument_PushFar(menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
        Text_DrawMessageAsTilesFar((s32)MsgDjinnOwnerLabel, helpWindow, 0, 0);
        MessageArgument_PushFar(DJINN_ELEMENT(menu->pairEntries[0]) * 20 +
                                    DJINN_NUMBER(menu->pairEntries[0]) + 300,
                                MESSAGE_ARGUMENT_ABILITY);
        Window_SetTilemapEntryFar(
            helpWindow, DJINN_ELEMENT(menu->pairEntries[0]) + ELEMENT_ICON_TILE,
            6, 0, 0);
        Text_DrawMessageAsTilesFar((s32)MsgDjinnOwnerLabel + 1, helpWindow, 56,
                                   0);
#endif
        Text_DrawMessageAsTilesFar((s32)MsgDjinnOwnerLabel + 2, helpWindow, 0,
                                   8);
      }
#if TLA_PATCH_1
      wait(1);
#endif
      WindowSprite_ClearListFar(listWindow);
      if (selectedRow != -1) {
#if TLA_PATCH_1
        Window_ClearInteriorTilesFar(listWindow, 0, 96, 224, 104);
#endif
        // Japanese draws the effect as a resource string.
#if LANGUAGE == LANGUAGE_JAPANESE
        Text_DrawMessageFar(element * 20 + number + (s32)MsgDjinnEffect,
                            listWindow, 0, 96);
#else
        Text_DrawMessageAsTilesFar(element * 20 + number + (s32)MsgDjinnEffect,
                                   listWindow, 0, 96);
#endif
#if TLA_PATCH_4
      } else {
        Window_ClearInteriorTilesFar(listWindow, 0, 96, 224, 104);
#endif
      }
      ((struct WindowWork *)Ram_HeapSlots->windowWork)->dirty = 1;
      redraw = 0;
    }
    wait(1);
    if (!(gInput.held & KEY_R) || (gInput.pendingPressed & KEY_R)) {
      if (groupMode)
        redraw = 1;
      groupMode = 0;
      setAll = 0;
    }
    repeat = gInput.repeat;
    pressed = gInput.pressed;
    if (backdrop->tutorialStep != 0) {
      pressed = 0;
      repeat = 0;
      backdrop->tutorialTimer++;
      switch (backdrop->tutorialStep) {
      case 28:
        while (!(gInput.pressed & KEY_A)) {
          MenuCursor_Position(150, 26);
          wait(1);
        }
        pressed = 2;
        repeat = 2;
        break;
      case 27:
        if (backdrop->tutorialTimer == 60) {
          DJINN_TUTORIAL_MESSAGE(messageWindow, 12);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          wait(1);
          DJINN_TUTORIAL_MESSAGE(messageWindow, 13);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          backdrop->tutorialTimer = 0;
          DjinnRecovery_CountDownFar();
          DjinnRecovery_CountDownFar();
          DjinnRecovery_CountDownFar();
          Djinn_PutOnStandbyFar(4, 0, 7);
          DjinnRecovery_AddStandbyFar(4, 0, 7);
          Unit_CalculateFar(4);
          repeat = 2;
          goto pressB;
        }
        break;
      case 1:
        if (backdrop->tutorialTimer == 60) {
          DJINN_TUTORIAL_MESSAGE(messageWindow, 0);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
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
          repeat = 1;
          pressed = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case 3:
        if (backdrop->tutorialTimer == 90) {
          repeat = 16;
          pressed = 16;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 4;
        }
        break;
      case 6:
      case 7:
        if (backdrop->tutorialTimer == 60) {
          DJINN_TUTORIAL_MESSAGE(messageWindow, 1);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 8;
        }
        break;
      case 13:
        if (backdrop->tutorialTimer == 40) {
          repeat = 2;
          pressed = 2;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case 14:
      case 16:
      case 17:
        if (backdrop->tutorialTimer == 40) {
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case 15:
        if (backdrop->tutorialTimer == 60) {
          gMainRoutineWork.messageSpeed = MESSAGE_SPEED_NORMAL;
          messageWindow =
              Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + 4, 9, 9, 1);
          MenuCursor_Slide(2, 146);
          while (Message_AllTypedFar() == 0)
            wait(1);
          while (!(gInput.pressed & KEY_A)) {
            MenuCursor_Position(2, 146);
            wait(1);
          }
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          wait(1);
          messageWindow =
              Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + 5, 9, 9, 1);
          while (Message_AllTypedFar() == 0)
            wait(1);
          while (!(gInput.pressed & KEY_A)) {
            MenuCursor_Position(2, 146);
            wait(1);
          }
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          wait(1);
          messageWindow =
              Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + 6, 9, 9, 1);
          while (Message_AllTypedFar() == 0)
            wait(1);
          while (!(gInput.pressed & KEY_A)) {
            MenuCursor_Position(2, 146);
            wait(1);
          }
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          wait(1);
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 16;
        }
        break;
      case 18:
      case 20:
        if (backdrop->tutorialTimer == 90) {
          repeat = 1;
          pressed = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 21;
        }
        break;
      case 21:
        if (backdrop->tutorialTimer == 90) {
          repeat = 32;
          pressed = 32;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 22;
        }
        break;
      case 24:
        if (backdrop->tutorialTimer == 60) {
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 25;
          repeat = 2;
        pressB:
          pressed = 2;
        }
        break;
      }
    }
    // R on a Djinni: put it on standby or set it at once.
    if (mode == 0 && (pressed & KEY_R)) {
      if (selectedRow == -1) {
        Sound_PlayFar(SE_MENU_ERROR);
        continue;
      }
      groupMode = 1;
      gInput.pendingPressed = 0;
      switch (state) {
      case 0:
        Sound_PlayFar(SE_139);
        Djinn_SetFar(owner, element, number);
        DjinnRecovery_RemoveFar(owner, element, number);
        break;
      case 1:
        Sound_PlayFar(SE_NOTE_HIGH);
        Djinn_PutOnStandbyFar(owner, element, number);
        DjinnRecovery_AddStandbyFar(owner, element, number);
        break;
      default:
        Sound_PlayFar(SE_MENU_ERROR);
        redraw = 1;
        continue;
      }
      Unit_CalculateFar(owner);
      DjinnMenu_DrawPage(backdrop, memberColumn / 4 * 4);
      redraw = 1;
    }
    if (mode == 0 && (pressed & KEY_L)) {
      result = 7;
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if ((pressed & KEY_A) || (mode == 1 && (pressed & KEY_R))) {
      if (selectedRow != -1 && state == 2) {
        Sound_PlayFar(SE_MENU_ERROR);
        WindowSprite_ClearListFar(listWindow);
#if TLA_PATCH_1
        Window_ClearInteriorTilesFar(listWindow, 0, 96, 224, 104);
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
        Text_DrawMessageFar((s32)MsgDjinnNowRecovering, listWindow, 0, 96);
#else
        Text_DrawMessageAsTilesFar((s32)MsgDjinnNowRecovering, listWindow, 0,
                                   96);
#endif
        continue;
      }
      if (mode == 1) {
        if (memberColumn == menu->selectedColumn[0]) {
          if (menu->pairEntries[0] & 0x8000)
            result = 2;
          else
            result = 1;
        } else if (columnFlags[memberColumn] & 1) {
          result = 3;
        } else {
          result = 4;
        }
      } else {
        result = 1;
      }
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if (pressed & KEY_START) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -2;
      break;
    }
    if (pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    // Select: with R held, set every Djinni not recovering or, the next
    // time, put every one on standby; otherwise open the help.
    if (mode == 0 && (pressed & KEY_SELECT)) {
      if (groupMode) {
        setAll ^= 1;
        if (setAll)
          Sound_PlayFar(SE_139);
        else
          Sound_PlayFar(SE_NOTE_HIGH);
        for (i = 0; i < menu->memberCount; i++) {
          for (j = 0; j < backdrop->counts[i]; j++) {
            u32 entry = backdrop->ids[i][j];
            s32 entryOwner = DJINN_MEMBER(entry);
            s32 entryElement = DJINN_ELEMENT(entry);
            s32 entryNumber = DJINN_NUMBER(entry);

            switch (DjinnMenu_GetEntryState(entry)) {
            case 0:
              if (setAll) {
                Djinn_SetFar(entryOwner, entryElement, entryNumber);
                DjinnRecovery_RemoveFar(entryOwner, entryElement, entryNumber);
                Unit_CalculateFar(entryOwner);
              }
              break;
            case 1:
              if (!setAll) {
                Djinn_PutOnStandbyFar(entryOwner, entryElement, entryNumber);
                DjinnRecovery_AddStandbyFar(entryOwner, entryElement,
                                            entryNumber);
                Unit_CalculateFar(entryOwner);
              }
              break;
            }
          }
        }
        DjinnMenu_DrawPage(backdrop, memberColumn / 4 * 4);
        redraw = 1;
      } else {
        UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
        result = 10;
        Sound_PlayFar(SE_MENU_CONFIRM);
        break;
      }
    }
    if (repeat & KEY_UP) {
      if (columnFlags[memberColumn] & 4)
        continue;
      if (backdrop->counts[memberColumn] == 0)
        continue;
      UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      if (columnFlags[memberColumn] & 1) {
        columnFlags[memberColumn] &= ~1;
        djinnRow = 0;
      } else if (djinnRow == 0 && (columnFlags[memberColumn] & 2)) {
        columnFlags[memberColumn] |= 1;
        redraw = 2;
        continue;
      }
      djinnRow--;
      djinnRow = Menu_GetModuloOfSum(djinnRow, backdrop->counts[memberColumn]);
      savedRow = djinnRow;
      redraw = 2;
      continue;
    }
    if (repeat & KEY_DOWN) {
      if (columnFlags[memberColumn] & 4)
        continue;
      if (backdrop->counts[memberColumn] == 0)
        continue;
      UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      djinnRow++;
      djinnRow = Menu_GetModuloOfSum(djinnRow, backdrop->counts[memberColumn]);
      if (columnFlags[memberColumn] & 1) {
        columnFlags[memberColumn] &= ~1;
        djinnRow = 0;
      } else if (djinnRow == 0 && (columnFlags[memberColumn] & 2)) {
        columnFlags[memberColumn] |= 1;
      }
      savedRow = djinnRow;
      redraw = 2;
      continue;
    }
    if (repeat & KEYS_HORIZONTAL) {
      s32 oldColumn;

      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      oldColumn = memberColumn;
      if (!(columnFlags[memberColumn] & 4))
        UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
      if (repeat & KEY_LEFT)
        memberColumn--;
      else
        memberColumn++;
      if (memberColumn < 0)
        memberColumn = 0;
      if (memberColumn > menu->memberCount - 1)
        memberColumn = menu->memberCount - 1;
      if (mode == 0) {
        for (i = 0; i < menu->memberCount; i++) {
          if (columnFlags[memberColumn] != 4)
            break;
          if (repeat & KEY_LEFT)
            memberColumn--;
          else
            memberColumn++;
          if (memberColumn < 0 || memberColumn >= menu->memberCount) {
            memberColumn = oldColumn;
            break;
          }
        }
      }
      djinnRow = savedRow;
      result = backdrop->counts[memberColumn];
      if (result == 0)
        result = 1;
      djinnRow = Menu_GetModuloOfSum(djinnRow, result);
      redraw = 2;
      if (oldColumn / 4 * 4 != memberColumn / 4 * 4)
        redraw = 6;
    }
  }
  menu->selectedColumn[mode] = memberColumn;
  if (selectedRow != -1) {
    u32 id;
    u32 entry;

    menu->pairEntries[mode] = id = backdrop->ids[memberColumn][selectedRow];
    entry = (u16)id;
    menu->pairIndex[mode] = entry % 32;
    menu->pairElement[mode] = DJINN_ELEMENT(entry);
    menu->pairMember[mode] = DJINN_MEMBER(entry);
  }
  menu->selection[mode] = memberColumn + djinnRow * 10;
  DjinnFigures_SetSlotDepth(mode, 0x4000);
  return result;
}

#undef DJINN_TUTORIAL_MESSAGE

#define DJINN_ENTRY_SET (-0x8000)

/// Lists owned Djinn by element and index, marking those that are set.
/// For element -1, entries also encode their party member.
s32 Djinn_ListUnitEntries(u16 *entriesOut, s32 memberId, s32 element) {
  struct Unit *unit = Unit_Get(memberId);
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

#undef DJINN_ENTRY_SET

s32 DjinnMenu_DrawStatPreview(s32 window, s32 x, s32 y, s32 ownerId,
                              s32 removePairSlot, s32 addPairSlot,
                              s32 previewMode, s32 page, s32 side);

/// Draws both sides of a Djinn preview for page pageIndex: the second member
/// in the second window and the first in the first. Mode 0 draws the pair
/// members, both as a trade (DJINN_PREVIEW_TRADE); any other mode the shown
/// members, the second gaining the Djinni and the first losing it. Returns 1.
s32 DjinnMenu_DrawPairPreview(s32 mode, s32 pageIndex) {
  s32 savedMode;
  u8 first;
  u8 owner;
  struct DjinnMenuWork *menu;

  menu = Ram_HeapSlots->menuWork;
  if (mode == DJINN_COMPARE_TRADE) {
    savedMode = mode;
    first = menu->pairMember[1];
    DjinnMenu_DrawStatPreview((s32)menu->secondWindow, 0, 0, first, 1, mode,
                              DJINN_PREVIEW_TRADE, pageIndex, 1);
    DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pairMember[0], mode, 1,
                              DJINN_PREVIEW_TRADE, pageIndex, mode);
  } else {
    owner = menu->shownMember[1];
    DjinnMenu_DrawStatPreview((s32)menu->secondWindow, 0, 0, owner, 1, 0,
                              DJINN_PREVIEW_ADD, pageIndex, 1);
    DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->shownMember[0], 0, 0,
                              DJINN_PREVIEW_UNSET, pageIndex, 0);
  }
  return 1;
}

// The stat preview's columns, in pixels from x * 8: the Japanese edition draws
// Attack, Defense and Agility with four digits from HP's column and Luck a tile
// further left, sets every arrow in one column, and puts the Psynergy page's PP
// heading further left.
//
// The Japanese text leaves the preview's messages unnamed, so each is written
// by its place after a named one: "HP", the first of the six stat labels, 13
// after "Luck rises"; "* Psynergy Gained" and "* Psynergy Lost", the two after
// the status help; "* No change", the one after "Trade".
#if LANGUAGE != LANGUAGE_JAPANESE
#define DJINN_PREVIEW_DIGITS 3
#define DJINN_PREVIEW_CURRENT_X 48
#define DJINN_PREVIEW_LUCK_X 56
#define DJINN_PREVIEW_POOL_ARROW_X 62 ///< beside HP and PP
#define DJINN_PREVIEW_STAT_ARROW_X 70 ///< beside the other stats
#define DJINN_PREVIEW_PP_X 88
#define MSG_STAT_LABELS ((s32)MsgKey0d0e)
#define MSG_PSYNERGY_GAINED ((s32)MsgKey10d3)
#define MSG_PSYNERGY_LOST ((s32)MsgKey10d4)
#define MSG_NO_CHANGE ((s32)MsgKey10d9)
#else
#define DJINN_PREVIEW_DIGITS 4
#define DJINN_PREVIEW_CURRENT_X 32
#define DJINN_PREVIEW_LUCK_X 48
#define DJINN_PREVIEW_POOL_ARROW_X 64
#define DJINN_PREVIEW_STAT_ARROW_X 64
#define DJINN_PREVIEW_PP_X 56
#define MSG_STAT_LABELS ((s32)MsgLuckRises + 13)
#define MSG_PSYNERGY_GAINED ((s32)MsgDjinnStatusHelp + 1)
#define MSG_PSYNERGY_LOST ((s32)MsgDjinnStatusHelp + 2)
#define MSG_NO_CHANGE ((s32)MsgDjinnCompareTrade + 1)
#endif

/// HP and PP, four digits wide in every edition, and their maximum and the
/// slash before it when nothing changes.
#define DJINN_PREVIEW_POOL_X 32
#define DJINN_PREVIEW_MAX_X 72
#define DJINN_PREVIEW_SLASH_X 64
/// The y of the preview's stat row `row` (0 to 5: HP, PP, Attack, Defense,
/// Agility and Luck) in a window whose top is tile row y.
#define DJINN_PREVIEW_STAT_Y(y, row) ((y) * 8 + 56 + (row) * 8)
/// The x of a changed stat's new value on that row, for a window whose left
/// is tile column x.
#define DJINN_PREVIEW_VALUE_X(x) ((x) * 8 + 72)

extern const char gDjinnMenu_TextSlash[];

#if LANGUAGE != LANGUAGE_JAPANESE

#else

#endif

s32 Icon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y,
                                  s32 resourceId);
s32 Icon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y,
                                   s32 variant);

/// Previews a djinni change on a member's status: the unit is saved, the change
/// applied and the unit recalculated; page 0 draws the stats with an arrow
/// beside each one that changes, later pages the Psynergy gained and lost; then
/// the saved unit is copied back.
s32 DjinnMenu_DrawStatPreview(s32 window, s32 x, s32 y, s32 ownerId,
                              s32 removePairSlot, s32 addPairSlot,
                              s32 previewMode, s32 page, s32 side) {
  // One constant for its whole life: the djinn tile loop starts from it.
  s32 firstElement = 0;
  // One constant for its whole life: the closing copy's size.
  s32 size;
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
  u16 actions[48];
  struct DjinnMenuWork *menu;

  unit = Unit_Get(ownerId);
  menu = Ram_HeapSlots->menuWork;
  giveElement = menu->pairElement[removePairSlot];
  giveIndex = menu->pairIndex[removePairSlot];
  giveSet = menu->pairEntries[removePairSlot] & DJINN_ENTRY_IS_SET;
  takeElement = menu->pairElement[addPairSlot];
  takeIndex = menu->pairIndex[addPairSlot];
  takeSet = menu->pairEntries[addPairSlot] & DJINN_ENTRY_IS_SET;
  originalUnit = Heap_Alloc(sizeof(struct Unit));
  Iwram_CopyWordsEntry(originalUnit, unit, sizeof(struct Unit));

  if (page == 0) {
    if (previewMode == DJINN_PREVIEW_NO_CHANGE) {
      Text_DrawNumberInWindowFar(unit->hpMax, 4, window,
                                 x * 8 + DJINN_PREVIEW_MAX_X,
                                 DJINN_PREVIEW_STAT_Y(y, 0));
      Text_DrawNumberInWindowFar(unit->ppMax, 4, window,
                                 x * 8 + DJINN_PREVIEW_MAX_X,
                                 DJINN_PREVIEW_STAT_Y(y, 1));
      Text_DrawNumberInWindowFar(unit->hp, 4, window,
                                 x * 8 + DJINN_PREVIEW_POOL_X,
                                 DJINN_PREVIEW_STAT_Y(y, 0));
      Text_DrawNumberInWindowFar(unit->pp, 4, window,
                                 x * 8 + DJINN_PREVIEW_POOL_X,
                                 DJINN_PREVIEW_STAT_Y(y, 1));
      Text_DrawStringAtOffsetFar((s32)gDjinnMenu_TextSlash, window,
                                 x * 8 + DJINN_PREVIEW_SLASH_X,
                                 DJINN_PREVIEW_STAT_Y(y, 0));
      Text_DrawStringAtOffsetFar((s32)gDjinnMenu_TextSlash, window,
                                 x * 8 + DJINN_PREVIEW_SLASH_X,
                                 DJINN_PREVIEW_STAT_Y(y, 1));
    } else {
      Text_DrawNumberInWindowFar(unit->hp, 4, window,
                                 x * 8 + DJINN_PREVIEW_POOL_X,
                                 DJINN_PREVIEW_STAT_Y(y, 0));
      Text_DrawNumberInWindowFar(unit->pp, 4, window,
                                 x * 8 + DJINN_PREVIEW_POOL_X,
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

  switch (previewMode) {
  case DJINN_PREVIEW_ADD_AND_SET:
    Djinn_GiveFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    Djinn_SetFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    break;
  case DJINN_PREVIEW_UNSET:
    giveIndex &= DJINN_ENTRY_INDEX;
    Djinn_PutOnStandbyFar(ownerId, giveElement, giveIndex);
    break;
  case DJINN_PREVIEW_TRADE:
    if (giveSet) {
      giveIndex &= DJINN_ENTRY_INDEX;
      Djinn_PutOnStandbyFar(ownerId, giveElement, giveIndex);
    }
    Djinn_GiveFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    if (takeSet)
      Djinn_SetFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    break;
  case DJINN_PREVIEW_ADD:
    Djinn_GiveFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    if (takeSet)
      Djinn_SetFar(ownerId, takeElement, takeIndex & DJINN_ENTRY_INDEX);
    break;
  }

  Unit_CalculateFar(ownerId);
  unit = Unit_Get(ownerId);

  if (page == 0) {
    s32 label;

    Text_DrawStringAtOffsetFar((s32)unit->name, window, x * 8 + 40, y * 8);
    Text_DrawStringAtOffsetFar((s32)gDjinnMenu_TextLevel, window, x * 8 + 40,
                               y * 8 + 16);
    Text_DrawNumberInWindowFar(unit->level, 2, window, x * 8 + 88, y * 8 + 16);
    label = MSG_STAT_LABELS;
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
    Text_DrawMessageAsTilesFar((s32)MsgClassName + originalUnit->classId,
                               window, x * 8, y * 8 + 32);
  }

  {
    s8 i = 0;

    if (page == 0) {
      s32 column;

      if (originalUnit->classId != unit->classId) {
        Text_DrawMessageAsTilesFar((s32)MsgClassName + unit->classId, window,
                                   x * 8, y * 8 + 48);
        Window_SetTilemapEntryFar(window, WINDOW_TILE(662), x + 2, 5, 0);
      }
      column = x;
      if (originalUnit->classId != unit->classId)
        column += 5;
      for (i = firstElement; i < 4; i++) {
        s32 tileColumn;
        s32 tile = ELEMENT_ICON_TILE + i;

        tileColumn = column + i * 2;
        Window_SetTilemapEntryFar(window, tile, tileColumn, y + 5, 0);
        tileColumn++;
        Window_SetTilemapEntryFar(window,
                                  unit->djinnActiveCounts[i] + WINDOW_TILE('0'),
                                  tileColumn, y + 5, 0);
      }
      // An empty once-only block: it ends the tile loop's block before the
      // stats.
      do {
      } while (0);
      if (unit->hp != originalUnit->hp) {
        Text_DrawNumberInWindowFar(unit->hp, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 0));
        if (unit->hp > originalUnit->hp)
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 0), 0);
        else
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 0), 1);
      }
      if (unit->pp != originalUnit->pp) {
        Text_DrawNumberInWindowFar(unit->pp, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 1));
        if (unit->pp > originalUnit->pp)
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 1), 0);
        else
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 1), 1);
      }
      if (unit->atk != originalUnit->atk) {
        Text_DrawNumberInWindowFar(unit->atk, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 2));
        if (unit->atk > originalUnit->atk)
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 2), 0);
        else
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 2), 1);
      }
      if (unit->def != originalUnit->def) {
        Text_DrawNumberInWindowFar(unit->def, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 3));
        if (unit->def > originalUnit->def)
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 3), 0);
        else
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 3), 1);
      }
      if (unit->agi != originalUnit->agi) {
        Text_DrawNumberInWindowFar(unit->agi, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 4));
        if (unit->agi > originalUnit->agi)
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 4), 0);
        else
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 4), 1);
      }
      if (unit->luck != originalUnit->luck) {
        Text_DrawNumberInWindowFar(unit->luck, 2, window, x * 8 + 88,
                                   DJINN_PREVIEW_STAT_Y(y, 5));
        if (unit->luck > originalUnit->luck)
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 5), 0);
        else
          Icon_DrawVariantWithTileOffset(window,
                                         x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
                                         DJINN_PREVIEW_STAT_Y(y, 5), 1);
      }
    }
  }

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
      Text_DrawMessageAsTilesFar((s32)MsgAbilityName +
                                     (actions[first] & PSYNERGY_ABILITY_MASK),
                                 window, x * 8 + 16, (y + line * 2) * 8 + 8);
      Text_DrawNumberAtOffsetFar(Ability_Get(actions[first])->ppCost, 2, window,
                                 x * 8 + 88, (y + line * 2) * 8 + 8);
      line++;
    }
    Text_SetColorFar(TEXT_COLOR_NORMAL);
    Text_DrawMessageAsTilesFar((s32)MsgPsynergyPp, window,
                               x * 8 + DJINN_PREVIEW_PP_X, y * 8);
    if (previewMode != DJINN_PREVIEW_NO_CHANGE) {
      s32 lines;

      lines = 0;
      if (gained) {
        Text_SetColorFar(TEXT_COLOR_YELLOW);
        Text_DrawMessageAsTilesFar(MSG_PSYNERGY_GAINED, window, x * 8,
                                   y * 8 + 88);
        lines = 1;
      }
      if (lost) {
        Text_SetColorFar(TEXT_COLOR_RED);
        Text_DrawMessageAsTilesFar(MSG_PSYNERGY_LOST, window, x * 8,
                                   (y + lines) * 8 + 88);
        lines++;
      }
      if (lines == 0)
        Text_DrawMessageAsTilesFar(MSG_NO_CHANGE, window, x * 8, y * 8 + 88);
      Text_SetColorFar(TEXT_COLOR_NORMAL);
      Window_DrawDividerLineFar(window, 0, 11, 13, 11);
    }
    ((struct WindowWork *)Ram_HeapSlots->windowWork)->dirty = 1;
  }

  size = sizeof(struct Unit);
  if (page == 0)
    Portrait_CreateFar(ownerId, 0, side, window, 0, 0);
  Iwram_CopyWordsEntry(unit, originalUnit, size);
  Heap_Release(originalUnit);
  return 1;
}

#undef MSG_NO_CHANGE

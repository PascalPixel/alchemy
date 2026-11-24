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
/// The y of the chosen Djinni's figure over its column, and its y while the
/// Djinni is being chosen.
#define DJINN_FIGURE_Y 54
#define DJINN_FIGURE_CHOOSING_Y 62

/// DjinnMenuState.pairEntries before a Djinni is chosen: every element and
/// number bit set, which no list entry has.
#define DJINN_ENTRY_NONE (DJINN_ENTRY_ELEMENT | DJINN_ENTRY_INDEX)

/// The tile of BG character block 1 the backdrop's digits are copied to.
#define DJINN_BACKDROP_DIGIT_TILE 128

/// The cursor packed in DjinnMenuState.selection: column + row * 10.
#define DJINN_CURSOR_ROW_STRIDE 10

/// Wraps a cursor index that has stepped one past either end of modulus
/// entries: -1 becomes modulus - 1 and modulus becomes 0.
s32 Menu_GetModuloOfSum(s32 value, s32 modulus) {
  return (value + modulus) % modulus;
}

/// Places the four party figures over their Djinn columns, 56 pixels apart
/// from x 32, at screen y 61 plus yOffset. Returns that y.
s32 Menu_SetFirstActorRowCoordinates(s32 yOffset) {
  struct DjinnMenuState *menu;
  s32 memberX;
  s32 memberIndex;
  menu = Ram_HeapSlots->menuState;
  yOffset += 61;
  memberX = 32;
  for (memberIndex = 0; memberIndex < 4; memberIndex++) {
    menu->memberX[memberIndex] = memberX;
    menu->memberY[memberIndex] = yOffset;
    memberX += DJINN_COLUMN_WIDTH;
  }
  return yOffset;
}

/// Returns the Djinn tutorial's first line: the second copy's once
/// FLAG_ALREADY_KNOWS_DJINN is set.
s32 Menu_GetDjinnGiveMessage(void) {
  if (GameFlag_Get(FLAG_ALREADY_KNOWS_DJINN))
    return (s32)MsgDjinnGiveTutorialB;
  return (s32)MsgDjinnGiveTutorialA;
}

void Resource_LoadPairedBlocksIfAvailable(void);

s32 DjinnMenu_RunScreen(void);

/// Runs the Djinn menu over the field: the field menu's Djinn screen
/// (FieldMenu_Run, FIELD_MENU_DJINN), and the Djinn tutorial. Types messages at
/// MESSAGE_SPEED_FAST while it runs (the tutorial's steps set
/// MESSAGE_SPEED_NORMAL for their messages, which stays until the menu closes)
/// and puts the player's speed back. Takes the menu state (HEAP_SLOT_MENU) and
/// the backdrop (struct DjinnMenuBackdrop, from the EWRAM pool), and frees
/// both before returning, after putting back the field tiles and palettes the
/// backdrop saved. Returns 1, which makes the field menu open again.
s32 DjinnMenu_Run(void) {
  struct DjinnMenuState *menu;
  u32 savedMessageSpeed;
  s32 enabled;
  struct DjinnMenuBackdrop *backdrop;

  menu = HeapSlot_Alloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
  savedMessageSpeed = gMainRoutineState.messageSpeed;
  gMainRoutineState.messageSpeed = MESSAGE_SPEED_FAST;
  enabled = Ram_HeapSlots->actorSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_InitializeState(0);
  backdrop = Heap_AllocEwram(sizeof(*backdrop));
  menu->backdrop = backdrop;
  backdrop->tutorialTimer = 0;
  backdrop->tutorialStep = DJINN_TUTORIAL_OFF;
  // The tutorial's lesson, from the flags the world map sets for each.
  if (GameFlag_Get(FLAG_DJINN_LESSON)) {
    if (!GameFlag_Get(FLAG_DJINN_LESSON_PART_A)) {
      if (!GameFlag_Get(FLAG_DJINN_LESSON_PART_B))
        backdrop->tutorialStep = enabled; // DJINN_TUTORIAL_GIVE
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
  menu->pairEntries[0] = DJINN_ENTRY_NONE;
  menu->selectedColumn[0] = 0;
  menu->selectedColumn[1] = 0;
  menu->selection[0] = 0;
  menu->selection[1] = 0;
  DjinnFigures_InitializeTopRow();
  DjinnMenu_RunScreen();
  DjinnFigures_ReleaseTopRow();
  Menu_FreeArrows();
  wait(1);
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  Ram_HeapSlots->actorSystem->suspended = 0;
  Window_MarkVisibleTextTilesFar();
  Text_UseExtraTilesFar(0);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), backdrop->tiles,
                  sizeof(backdrop->tiles));
  Iwram_CopyWords((void *)&BG_PLTT_COLOR(4, 0), backdrop->palette,
                  sizeof(backdrop->palette));
  wait(1);
  Menu_ResumeFieldTasks();
  Window_EraseBorderRectFar(0, 0, 30, 20);
  Heap_Release(menu->backdrop);
  HeapSlot_Release(HEAP_SLOT_MENU);
  Party_ClearUnusableShortcutsFar();
  gMainRoutineState.messageSpeed = savedMessageSpeed;
  return 1;
}

void Menu_OpenBackdropScreen(void);

s32 DjinnMenu_SelectDjinn(s32 mode);
s32 DjinnMenu_ShowMemberStatus(void);
s32 DjinnMenu_ShowHelp(void);
s32 PsynergyList_RunCompareLoop(u32 mode);
/// Moves a Djinni from owner to recipient, set or on standby as it was;
/// returns -1 when it cannot.
s32 Djinn_TransferFar(s32 owner, s32 element, s32 index, s32 recipient);

u32 Djinn_PutOnStandbyFar(s32 owner, s32 element, s32 index);
s32 DjinnRecovery_AddStandbyFar(s32 owner, s32 element, s32 index);

void DjinnMenu_BuildLists(struct DjinnMenuBackdrop *backdrop);

/// Find the moved Djinni in the list and select its new position.
#define DJINN_RESELECT_PAIR_ENTRY(menu, i)                                     \
  do {                                                                         \
    s32 column;                                                                \
    s32 found;                                                                 \
    u8 entry;                                                                  \
                                                                               \
    column = (u16)((u32)(menu)->selection[1] % DJINN_CURSOR_ROW_STRIDE);       \
    found = 0;                                                                 \
    entry = (menu)->pairEntries[0];                                            \
    for ((i) = 0; (i) < (menu)->backdrop->counts[column]; (i)++) {             \
      if (entry == (u8)(menu)->backdrop->ids[column][(i)]) {                   \
        found = (i);                                                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    (menu)->selection[0] = column + found * DJINN_CURSOR_ROW_STRIDE;           \
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

/// The Djinn menu's screen loop, one step at a time: choose a Djinni, then what
/// to do with it (set it, put it on standby, give it or trade it), preview the
/// change and apply it; L shows the party member's status and Select the help.
/// Returns -1 once the player leaves the menu.
s32 DjinnMenu_RunScreen(void) {
  // icon holds the cursor, then each arrow in turn.
  struct DjinnMenuState *menu;
  struct WindowSprite *icon;
  s32 result;
  s32 step;
  s32 menuResult;
  s32 done;
  s32 listIndex;

  done = 0;
  result = 0;
  menuResult = 0;
  menu = Ram_HeapSlots->menuState;
  icon = menu->cursor;
  icon->motion = ICON_HIDDEN;
  icon->animationStep = 0;
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
      DjinnFigures_SetSlotPosition(1, 0, MENU_FIGURE_HIDDEN_Y);
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
                                   DJINN_FIGURE_Y);
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

#undef DJINN_RESELECT_PAIR_ENTRY

/// The palette DjinnMenu_AdjustPalettes darkens WINDOW_PALETTE into: the Djinn
/// list behind the help, and the column a Djinni is taken from.
#define DJINN_DIM_PALETTE 6
/// The Djinn help's topics: Djinn, Set, Standby, Summon, Recovery, Class and
/// Psynergy.
#define DJINN_HELP_TOPICS 7

/// Writes copies of background palettes 15, 5 and 7, each brightnessDelta
/// lighter in every channel (clamped to 0-31), into palettes 14, 4 and 6;
/// palette 7's copy is 12 darker instead.
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
      paletteNumber = 5;
    } else {
      paletteNumber = 7;
      brightnessDelta = -12;
    }
  }
}

/// Copies the window font's ten digit tiles twice into tiles, the first copy
/// with colour 1 drawn as colour 10.
void Graphics_ExpandVramTilesByColorTableFar(void *tiles);
s32 Palette_GetBasicSpriteColors(void);

/// Opens the Djinn menu's backdrop screen: saves the tiles and palettes it
/// covers, draws the backdrop, lists the party's Djinn (DjinnMenu_BuildLists),
/// then
/// makes the list's cursor and the two page arrows and places the arrows at the
/// list window's corners.
void Menu_OpenBackdropScreen(void) {
  struct DjinnMenuState *menu = Ram_HeapSlots->menuState;
  struct DjinnMenuBackdrop *backdrop = menu->backdrop;
  struct Window *window;
  struct WindowSprite *icon;
  s32 entry;
  s32 x;
  s32 y;

  Window_UpdateOrCreate(&menu->listWindow, 0, 5, 30, 15, WINDOW_FLAG_INSTANT);
  wait(1);
  Iwram_CopyWords(backdrop->tiles, BG_CHAR_BLOCK(1), sizeof(backdrop->tiles));
  Iwram_CopyWords(backdrop->palette, (void *)&BG_PLTT_COLOR(4, 0),
                  sizeof(backdrop->palette));
  Iwram_FillWords(BG_CHAR_BLOCK(1), sizeof(backdrop->tiles), PIXELS_4BPP(3));
  Iwram_FillWords((void *)&BG_PLTT_COLOR(4, 0), sizeof(backdrop->palette),
                  PIXELS_4BPP(5));
  Graphics_ExpandVramTilesByColorTableFar(
      BG_CHAR_BLOCK(1) + DJINN_BACKDROP_DIGIT_TILE * TILE_SIZE_4BPP);
  Iwram_CopyWords(DJINN_BACKDROP_BORDER_VRAM, gMenu_BackdropBorderTile,
                  TILE_SIZE_4BPP);
  Dma_Set(Palette_GetBasicSpriteColors(),
          (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  Dma_Set((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0),
          (void *)&BG_PLTT_COLOR(7, 0), DMA_COPY_HALFWORDS(16), REG_DMA3);
  DjinnMenu_AdjustPalettes(8);
  BG_PLTT_COLOR(7, WINDOW_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  BG_PLTT_COLOR(6, WINDOW_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  DjinnMenu_BuildLists(backdrop);

  window = menu->statusWindow;
  WindowSprite_ClearListFar(window);
  icon = Icon_CreateWithResource(window, -8, 11);
  icon->motion = ICON_HIDDEN;
  menu->cursor = icon;
  entry = VramCache_FindFreeEntry();
  if (entry < VRAM_CACHE_ENTRY_COUNT) {
    VramBlock_LoadResourceFar(entry, ICON_TILE_BYTES, RESOURCE_SHOP_GEM_TILES);
    icon =
        WindowSprite_CreateFar(entry, WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
    icon->motion = ICON_HIDDEN;
    menu->pageArrows[0] = icon;
    icon->sprite.oam.matrix |= 8;
    icon =
        WindowSprite_CreateFar(entry, WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
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

/// Lists the Psynergy a party member has after a Djinn change, marking with
/// DJINN_PREVIEW_GAINED those they lacked before, then appends what they had
/// and lose, marked DJINN_PREVIEW_LOST. Returns how many it wrote, and the
/// gains and losses in the two counts.
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
  struct ItemMenuState *menu = (struct ItemMenuState *)Ram_HeapSlots->menuState;
  s32 i;

  for (i = 0; i < menu->partyCount; i++) {
    s32 owner = (u16)menu->memberIds[i];

    backdrop->counts[i] = Djinn_ListUnitEntries(backdrop->ids[i], owner, -1);
  }
}

s32 Djinn_CanSetFar(s32 owner, s32 element, s32 index);
s32 Djinn_IsSetFar(s32 owner, s32 element, s32 index);

/// The state of a Djinni's list entry (enum DjinniState): recovering when it
/// can be neither set nor is set, otherwise set or on standby by
/// DJINN_ENTRY_IS_SET. DjinnMenu_DrawPage colours each name by it.
s32 DjinnMenu_GetEntryState(u32 entry) {
  s32 owner = DJINN_MEMBER(entry);
  s32 element = DJINN_ENTRY_ELEMENT_OF(entry);
  s32 index = DJINN_NUMBER(entry);
  s32 state;

  if (Djinn_CanSetFar(owner, element, index) ||
      Djinn_IsSetFar(owner, element, index)) {
    u32 set = entry & DJINN_ENTRY_IS_SET;

    state = NONZERO(set);
  } else {
    state = DJINNI_STATE_RECOVERING;
  }
  return state;
}

/// Draws a packed djinni entry in a window: its name 8 pixels right of (x, y)
/// and its element's icon tile at the tile under (x, y), then resets the text
/// colour. The names run 20 to an element.
void DjinnMenu_DrawEntry(s32 window, s32 x, s32 y, s32 entry) {
  s32 element = (entry & DJINN_ENTRY_ELEMENT) / (DJINN_ENTRY_INDEX + 1);

  Text_DrawMessageAsTilesFar((s32)MsgDjinnName + element * DJINN_PER_ELEMENT +
                                 DJINN_NUMBER(entry),
                             window, x + 8, y);
  Window_SetTilemapEntryFar(window, element + ELEMENT_ICON_TILE, x / 8, y / 8,
                            0);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
}

/// Draws four party members' Djinn into the list window, one column of 56
/// pixels each from party member page: each member's Djinn grouped by element,
/// standby in red, set in white and the rest in yellow. Over the list it
/// labels the unleash effects when the party has few enough Djinn to show them.
void DjinnMenu_DrawPage(struct DjinnMenuBackdrop *backdrop, s32 page) {
  struct DjinnMenuState *menu = Ram_HeapSlots->menuState;
  struct WindowState *state = Ram_HeapSlots->windowState;
  s32 window = menu->listWindow;
  s32 owners;
  s32 column;
  s32 element;
  s32 i;
  s32 x;
  s32 y;
  s32 djinn;

  state->holdTilemapUpload = 1;
  WindowSprite_RedrawSavedRectFar(window);
  owners = Party_CountMembersFar();
  djinn = Party_SumDjinnCountsFar(-1);
  if (owners != 0 && (djinn - 1) / owners <= 6) {
    Text_DrawMessageAsTilesFar((s32)MsgDjinnUnleashEffectLabel, window, 0, 80);
    Window_DrawDividerLineFar(window, 0, 10, 28, 10);
  }
  DjinnMenu_BuildLists(backdrop);
  for (column = 0, x = 8; column <= 3; column++, x += DJINN_COLUMN_WIDTH) {
    u16 *entries = backdrop->ids[page + column];

    y = 16;
    if (menu->memberSprites[column] != NULL) {
      for (element = 0; element < ELEMENT_COUNT; element++) {
        for (i = 0; i < backdrop->counts[page + column]; i++) {
          if (element == DJINN_ENTRY_ELEMENT_OF(entries[i])) {
            switch (DjinnMenu_GetEntryState(entries[i])) {
            case DJINNI_STATE_STANDBY:
              Text_SetColorFar(TEXT_COLOR_RED);
              break;
            case DJINNI_STATE_SET:
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
  state->dirty = 1;
  state->holdTilemapUpload = 0;
  wait(3);
#else
  ((struct WindowState *)Ram_HeapSlots->windowState)->dirty = 1;
  state->holdTilemapUpload = 0;
#endif
}

/// A grid cell's highlight is a tile narrower in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define GRID_CELL_WIDTH 5
#else
#define GRID_CELL_WIDTH 6
#endif

/// Draws the Djinn list's first page, the first four party members.
void DjinnMenu_DrawFirstPage(struct DjinnMenuBackdrop *backdrop) {
  DjinnMenu_DrawPage(backdrop, 0);
}

/// Sets the palette of a rectangle of panel's interior, (x, y) and its size in
/// tiles from the interior's corner, through Window_HighlightRectFar.
void Window_SetInteriorPaletteBitRect(struct Window *panel, s32 x, s32 y,
                                      s32 width, s32 height, s32 palette) {
  Window_HighlightRectFar((s16)panel->x + x + 1, (s16)panel->y + y + 1, width,
                          height, palette);
}

/// Highlights the cell of a four-column grid at index's column and row's line,
/// in HIGHLIGHT_PALETTE when selected and WINDOW_PALETTE otherwise.
void UiPanel_HighlightGridCell(struct Window *panel, s32 index, s32 row,
                               s32 selected) {
  s32 palette = NONZERO(selected);

  palette = WINDOW_PALETTE - palette;
  Window_SetInteriorPaletteBitRect(panel, index % 4 * DJINN_COLUMN_TILES + 1,
                                   row + 2, GRID_CELL_WIDTH, 1, palette);
}

#undef GRID_CELL_WIDTH

/// Gives every cell of a tile rectangle that uses WINDOW_PALETTE the palette
/// given instead, clipped to the screen, and marks its rows for upload.
void Window_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette) {
  struct WindowState *tiles = (struct WindowState *)Ram_HeapSlots->windowState;

  palette = BG_TILE_PALETTE(palette);

  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > DISPLAY_TILE_WIDTH - 1) {
    width = DISPLAY_TILE_WIDTH - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  // BUG: tests the bottom against the screen's width but clips to its rows, so
  // a rectangle ending in rows 20 to 28 is kept whole and recolours past the
  // 32 by 20 tilemap.
#ifdef BUGFIX
  if (y + height > DISPLAY_TILE_HEIGHT - 1) {
#else
  if (y + height > DISPLAY_TILE_WIDTH - 1) {
#endif
    height = DISPLAY_TILE_HEIGHT - y;
  }

  if (width > 0 && height > 0) {
    do {
      u16 *entry = &tiles->tilemap[y][x];
      s32 column = width;

      if (column != 0) {
        do {
          u32 tile = *entry;

          if (BG_TILE_PALETTE_OF(tile) == WINDOW_PALETTE) {
            tile &= ~BG_TILE_PALETTE(0xf);
            tile |= palette;
            *entry = tile;
          }
          column--;
          entry++;
        } while (column != 0);
      }
      WINDOW_MARK_TILEMAP_ROW(tiles, y);

      height--;
      y++;
    } while (height != 0);
  }
}

/// Window_SetRectPalette on a rectangle of window's interior, (x, y) and its
/// size in tiles from the interior's corner.
void Window_SetInteriorRectPalette(struct Window *panel, s32 x, s32 y,
                                   s32 width, s32 height, s32 palette) {
  Window_SetRectPalette((s16)panel->x + x + 1, (s16)panel->y + y + 1, width,
                        height, palette);
}

/// Builds a message's render entries and queues them in window; returns 0 when
/// the message is empty or there is no window.
struct Message *Message_OpenDecodedFar(void *window, s32 message);

/// The Djinn menu's help, opened by Select on the first chooser: a list of
/// seven topics (Djinn, Set, Standby, Summon, Recovery, Class, Psynergy) with
/// the chosen one's text typed beside it. The d-pad moves through the topics,
/// and A moves on once the text has finished. Returns DJINN_RESULT_BACK for B
/// or Select, back to the list, or DJINN_RESULT_START for Start, which closes
/// the Djinn menu.
s32 DjinnMenu_ShowHelp(void) {
  struct Window *helpWindow;
  struct Window *statusWindow;
  struct DjinnMenuState *menu;
  struct WindowState *render;
  s32 result;
  s32 previous;
  s32 selection;
  struct Window *list;
  struct Message *channel;
  s32 message;
  s32 helpIndex;
  s32 listMessage;

  menu = Ram_HeapSlots->menuState;
  selection = 0;
  render = (struct WindowState *)Ram_HeapSlots->windowState;
  result = 0;
  previous = 0;
  WindowSprite_ClearListFar(menu->listWindow);
  wait(1);
  WindowSprite_PrepareForRedrawFar(menu->helpWindow);
  message = (s32)MsgDjinnInfoPrompt;
  Text_DrawMessageAsTilesFar(message, menu->helpWindow, 0, 0);
  message++;
  Text_DrawMessageAsTilesFar(message, menu->helpWindow, 0, 16);
  // "Select the items" and "Select:Djinni selection"
  Window_SetRectPalette(1, 1, 11, 3, DJINN_DIM_PALETTE);
  Window_SetInteriorRectPalette(menu->listWindow, 0, 0, 28, 10,
                                DJINN_DIM_PALETTE);
  list = Window_CreateFar(0, 9, 8, 10, 6);
  helpWindow = Window_CreateFar(8, 12, 22, 7, WINDOW_FLAG_INSTANT);
  statusWindow = Window_CreateFar(8, 9, 22, 3, WINDOW_FLAG_INSTANT);
  Window_MarkVisibleTextTilesFar();
  listMessage = (s32)MsgDjinnInfoTopic;
  for (helpIndex = 0; helpIndex < DJINN_HELP_TOPICS; helpIndex++) {
    s32 y = helpIndex * 8;

    Text_DrawMessageAsTilesFar(helpIndex + listMessage, list, 0, y);
  }
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
        selection = Menu_GetModuloOfSum(selection, DJINN_HELP_TOPICS);
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        break;
      }
      if (gInput.repeat & (KEY_LEFT | KEY_UP)) {
        selection--;
        selection = Menu_GetModuloOfSum(selection, DJINN_HELP_TOPICS);
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        break;
      } else if (gInput.pressed & KEY_START) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = DJINN_RESULT_START;
        break;
      } else if (gInput.pressed & (KEY_B | KEY_SELECT)) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = DJINN_RESULT_BACK;
        break;
      } else if (gInput.pressed & KEY_A) {
        if (Message_AllTypedFar()) {
          selection++;
          selection = Menu_GetModuloOfSum(selection, DJINN_HELP_TOPICS);
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
    ((struct WindowState *)Ram_HeapSlots->windowState)->pageArrowShown = 0;
    WindowSprite_PrepareForRedrawFar(helpWindow);
    {
      struct Window *window = channel->window;

      window->zoomStage = 0;
      window->duration = 0;
      window->state = 0;
    }
    channel->window = 0;
  } while (result == 0);
  ((struct WindowState *)Ram_HeapSlots->windowState)->holdTilemapUpload = 1;
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
    ((struct WindowState *)Ram_HeapSlots->windowState)->holdTilemapUpload = 0;
  }
  Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PASS_FRAME);
  return result;
}

// A grid cell's highlight is a tile narrower in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define GRID_CELL_WIDTH 5
#else
#define GRID_CELL_WIDTH 6
#endif

/// Spanish, French and Italian name the Djinni before its owner in one message
/// on the trade's help line, so the Djinni's icon leads the line.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define TRADE_HELP_DJINNI_FIRST 1
#else
#define TRADE_HELP_DJINNI_FIRST 0
#endif

/// The level label: `Lv`; `Nv.` in Spanish, `Niv` in French.
extern u8 gDjinnMenu_TextLevel[];

s32 Djinn_MarkTradeOnlyRecipients(s8 *table, s32 self);

void Menu_SetPageArrowsLeftRight(s32 index, s32 count);
void DjinnFigures_SetSlotFacing(s32 index, s32 facing);
void DjinnFigures_ReplaceSlot(s32 index, s32 kind, s32 animation);

void DjinnRecovery_CountDownFar(void);
s32 Menu_GetModuloOfSum(s32 value, s32 count) __attribute__((const));

/// Shows one tutorial message and waits for its text to finish.
#define DJINN_TUTORIAL_MESSAGE(window, offset)                                 \
  do {                                                                         \
    (window) =                                                                 \
        Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + (offset), 9, 9, 1); \
    gMainRoutineState.messageSpeed = MESSAGE_SPEED_NORMAL;                     \
    while (Message_AllTypedFar() == 0)                                         \
      wait(1);                                                                 \
  } while (0)

/// The flags DjinnMenu_SelectDjinn keeps for each party member's column.
enum DjinnColumnFlag {
  DJINN_COLUMN_ON_HEADER = 1,        ///< the cursor is on the column's header
  DJINN_COLUMN_HEADER_CHOOSABLE = 2, ///< its header can be chosen
  DJINN_COLUMN_EMPTY = 4             ///< no Djinni to choose
};

/// What DjinnMenu_SelectDjinn redraws on its next pass.
enum DjinnRedraw {
  DJINN_REDRAW_CURSOR = 1, ///< the cursor, the figure and the help lines
  DJINN_REDRAW_DJINNI = 2, ///< as for the cursor, the figure from its start
  DJINN_REDRAW_PAGE = 4    ///< the four columns of the page as well
};

/// The Djinn list's two choosers (enum DjinnSelectMode). Choosing a Djinni, the
/// d-pad moves over every party member's column, R sets the Djinni or puts it
/// on standby at once, R held with Select does so to them all, L shows the
/// party member's status and Select the help. Choosing what to do with it, the
/// cursor picks its own column to set it or put it on standby, another
/// member's header to give it, or one of their Djinn to trade. While the
/// tutorial runs, it presses the keys itself. Returns an enum DjinnMenuResult.
s32 DjinnMenu_SelectDjinn(s32 mode) {
  s8 columnFlags[8];
  u8 requiresTrade[16];
  struct DjinnMenuState *menu = Ram_HeapSlots->menuState;
  struct DjinnMenuBackdrop *backdrop = menu->backdrop;
  s32 helpWindow = menu->helpWindow;
  s32 listWindow = menu->listWindow;
  struct Unit *unit;
  s32 window;
  u32 redraw = DJINN_REDRAW_PAGE;
  s32 i;
  s32 j;
  s32 selectedRow;
  s32 memberColumn = (u16)(menu->selection[mode] % DJINN_CURSOR_ROW_STRIDE);
  s32 djinnRow = (u16)(menu->selection[mode] / DJINN_CURSOR_ROW_STRIDE);
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
  for (i = 0; i < 8; i++)
    status[i] = 0;
  if (mode == DJINN_SELECT_DJINNI) {
    savedRow = djinnRow;
    for (i = 0; i < menu->memberCount; i++) {
      if (backdrop->counts[i] == 0)
        status[i] = DJINN_COLUMN_EMPTY;
    }
    for (i = 0; i < menu->memberCount; i++) {
      if (status[memberColumn] == DJINN_COLUMN_EMPTY) {
        memberColumn++;
        memberColumn = Menu_GetModuloOfSum(memberColumn, menu->memberCount);
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
        if (backdrop->counts[i] == 0)
          status[i] = columnFlags[i] | DJINN_COLUMN_EMPTY;
      }
    }
  }
  menu->cursor->motion = ICON_SHOWN;
  for (;;) {
    if (redraw != 0) {
      selectedRow = -1;
      if (!(columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER))
        selectedRow = djinnRow;
      if (columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER)
        MenuCursor_Position(memberColumn % 4 * DJINN_COLUMN_WIDTH - 8, 52);
      else
        MenuCursor_Position(memberColumn % 4 * DJINN_COLUMN_WIDTH - 8,
                            djinnRow * 8 + 60);
      if (redraw >> 2) { // DJINN_REDRAW_PAGE
        s32 page = memberColumn / 4 * 4;

        Menu_CreateOwnerEntryObjects(page);
        DjinnMenu_DrawPage(backdrop, page);
        if (mode == DJINN_SELECT_ACTION) {
          s32 fromX;
          s32 fromY;
          u16 sourceCursor;
          s32 x;
          s32 height;

          sourceCursor = menu->selection[0];
          fromX = (u16)((u32)sourceCursor % (u32)DJINN_CURSOR_ROW_STRIDE);
          fromY = (u16)((u32)sourceCursor / (u32)DJINN_CURSOR_ROW_STRIDE);
          if (fromX / 4 * 4 == page) {
            height = backdrop->counts[fromX];
            UiPanel_HighlightGridCell(listWindow, fromX, fromY, 1);
            Window_SetInteriorRectPalette(
                listWindow, (fromX - page) * DJINN_COLUMN_TILES + 1, 2,
                GRID_CELL_WIDTH, height, DJINN_DIM_PALETTE);
            menu->slotY[0] = DJINN_FIGURE_Y;
          } else {
            menu->slotY[0] = MENU_FIGURE_HIDDEN_Y;
          }
          for (i = 0, x = 8; i <= 3; i++, x += DJINN_COLUMN_WIDTH) {
            if (menu->memberSprites[i] != NULL) {
              if (page + i == menu->selectedColumn[0]) {
                if (menu->pairEntries[0] & DJINN_ENTRY_IS_SET)
                  message = (s32)MsgDjinnStandbyAction;
                else
                  message = (s32)MsgDjinnSetAction;
              } else if (columnFlags[page + i] &
                         DJINN_COLUMN_HEADER_CHOOSABLE) {
                message = (s32)MsgDjinnGiveAction;
              } else {
                message = (s32)MsgDjinnTradeAction;
              }
              Text_DrawMessageAsTilesFar(message, listWindow, x, 8);
            }
          }
        }
        Menu_SetPageArrowsLeftRight(memberColumn, menu->memberCount);
#if !TLA_PATCH_1
        wait(1);
#endif
      }
      if (!(columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER))
        UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 1);
      if (selectedRow != -1)
        djinn = backdrop->ids[memberColumn][selectedRow];
      owner = DJINN_MEMBER(djinn);
      element = DJINN_ENTRY_ELEMENT_OF(djinn);
      number = DJINN_NUMBER(djinn);
      state = DjinnMenu_GetEntryState(djinn);
      if (selectedRow == -1) {
        DjinnFigures_SetSlotPosition(mode, 0, MENU_FIGURE_HIDDEN_Y);
      } else {
        s32 facing;
        s32 animation;

        switch (state) {
        case DJINNI_STATE_STANDBY:
          animation = 1;
          facing = FACING_SOUTH;
          break;
        case DJINNI_STATE_SET:
          animation = 2;
          facing = FACING_SOUTH;
          break;
        default:
          animation = 1;
          facing = FACING_WEST;
          break;
        }
        DjinnFigures_SetSlotPosition(mode, DJINN_FIGURE_X(memberColumn % 4),
                                     mode == DJINN_SELECT_DJINNI
                                         ? DJINN_FIGURE_CHOOSING_Y
                                         : DJINN_FIGURE_Y);
        DjinnFigures_SetSlotFacing(mode, facing);
        if (redraw & DJINN_REDRAW_DJINNI)
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
      if (mode == DJINN_SELECT_DJINNI)
        Text_DrawMessageAsTilesFar((s32)MsgDjinnCharacterStatus, window, 0, 16);
      WindowSprite_RedrawSavedRectFar(helpWindow);
      if (mode == DJINN_SELECT_DJINNI && selectedRow != -1) {
        if (groupMode) {
          if (setAll == 0)
            Text_DrawMessageAsTilesFar((s32)MsgDjinnSetAll, helpWindow, 0, 0);
          else
            Text_DrawMessageAsTilesFar((s32)MsgDjinnAllStandby, helpWindow, 0,
                                       0);
          if (state == DJINNI_STATE_RECOVERING)
            Text_DrawMessageAsTilesFar((s32)MsgDjinnRecovering, helpWindow, 0,
                                       16);
        } else {
          Text_DrawMessageAsTilesFar((s32)MsgDjinnChoose, helpWindow, 0, 0);
          switch (state) {
          case DJINNI_STATE_STANDBY:
            message = (s32)MsgDjinnChoose + 3; // "R:Set Select:Help"
            break;
          case DJINNI_STATE_SET:
            message = (s32)MsgDjinnChoose + 2; // "R:Standby Select:Help"
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
      if (mode == DJINN_SELECT_ACTION) {
#if TRADE_HELP_DJINNI_FIRST
        Window_SetTilemapEntryFar(helpWindow,
                                  DJINN_ENTRY_ELEMENT_OF(menu->pairEntries[0]) +
                                      ELEMENT_ICON_TILE,
                                  0, 0, 0);
        MessageArgument_PushFar(menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_PushFar(
            DJINN_ENTRY_ELEMENT_OF(menu->pairEntries[0]) * DJINN_PER_ELEMENT +
                DJINN_NUMBER(menu->pairEntries[0]) + ABILITY_DJINNI_FIRST,
            MESSAGE_ARGUMENT_ABILITY);
        Text_DrawMessageAsTilesFar((s32)MsgDjinnOwnerLabel, helpWindow, 8, 0);
#else
        MessageArgument_PushFar(menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
        Text_DrawMessageAsTilesFar((s32)MsgDjinnOwnerLabel, helpWindow, 0, 0);
        MessageArgument_PushFar(
            DJINN_ENTRY_ELEMENT_OF(menu->pairEntries[0]) * DJINN_PER_ELEMENT +
                DJINN_NUMBER(menu->pairEntries[0]) + ABILITY_DJINNI_FIRST,
            MESSAGE_ARGUMENT_ABILITY);
        Window_SetTilemapEntryFar(helpWindow,
                                  DJINN_ENTRY_ELEMENT_OF(menu->pairEntries[0]) +
                                      ELEMENT_ICON_TILE,
                                  6, 0, 0);
        Text_DrawMessageAsTilesFar((s32)MsgDjinnOwnerLabel + 1, helpWindow, 56,
                                   0);
#endif
        // "What will you do?"
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
        Text_DrawMessageFar(element * DJINN_PER_ELEMENT + number +
                                (s32)MsgDjinnEffect,
                            listWindow, 0, 96);
#else
        Text_DrawMessageAsTilesFar(element * DJINN_PER_ELEMENT + number +
                                       (s32)MsgDjinnEffect,
                                   listWindow, 0, 96);
#endif
#if TLA_PATCH_4
      } else {
        Window_ClearInteriorTilesFar(listWindow, 0, 96, 224, 104);
#endif
      }
      ((struct WindowState *)Ram_HeapSlots->windowState)->dirty = 1;
      redraw = 0;
    }
    wait(1);
    if (!(gInput.held & KEY_R) || (gInput.pendingPressed & KEY_R)) {
      if (groupMode)
        redraw = DJINN_REDRAW_CURSOR;
      groupMode = 0;
      setAll = 0;
    }
    repeat = gInput.repeat;
    pressed = gInput.pressed;
    if (backdrop->tutorialStep != DJINN_TUTORIAL_OFF) {
      pressed = 0;
      repeat = 0;
      backdrop->tutorialTimer++;
      switch (backdrop->tutorialStep) {
      case DJINN_TUTORIAL_CLOSE:
        while (!(gInput.pressed & KEY_A)) {
          MenuCursor_Position(150, 26);
          wait(1);
        }
        pressed = KEY_B;
        repeat = KEY_B;
        break;
      case DJINN_TUTORIAL_RECOVERY:
        if (backdrop->tutorialTimer == 60) {
          DJINN_TUTORIAL_MESSAGE(messageWindow, DJINN_TUTORIAL_LINE_RECOVERY);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          wait(1);
          DJINN_TUTORIAL_MESSAGE(messageWindow, DJINN_TUTORIAL_LINE_SET_AGAIN);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          backdrop->tutorialTimer = 0;
          DjinnRecovery_CountDownFar();
          DjinnRecovery_CountDownFar();
          DjinnRecovery_CountDownFar();
          Djinn_PutOnStandbyFar(FELIX, ELEMENT_VENUS, DJINNI_ECHO);
          DjinnRecovery_AddStandbyFar(FELIX, ELEMENT_VENUS, DJINNI_ECHO);
          Unit_CalculateFar(FELIX);
          repeat = KEY_B;
          pressed = KEY_B;
        }
        break;
      case DJINN_TUTORIAL_GIVE:
        if (backdrop->tutorialTimer == 60) {
          DJINN_TUTORIAL_MESSAGE(messageWindow, DJINN_TUTORIAL_LINE_GIVE);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = DJINN_TUTORIAL_GIVE_PICK;
        }
        break;
      case DJINN_TUTORIAL_GIVE_PICK:
      case DJINN_TUTORIAL_GIVE_DROP:
      case DJINN_TUTORIAL_SET_PICK:
      case DJINN_TUTORIAL_SET_CHOOSE:
      case DJINN_TUTORIAL_POWERS_CHOOSE:
        if (backdrop->tutorialTimer == 90) {
          repeat = KEY_A;
          pressed = KEY_A;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case DJINN_TUTORIAL_GIVE_MOVE:
        if (backdrop->tutorialTimer == 90) {
          repeat = KEY_RIGHT;
          pressed = KEY_RIGHT;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = DJINN_TUTORIAL_GIVE_DROP;
        }
        break;
      case DJINN_TUTORIAL_SET:
      case DJINN_TUTORIAL_SET_SPARE:
        if (backdrop->tutorialTimer == 60) {
          DJINN_TUTORIAL_MESSAGE(messageWindow,
                                 DJINN_TUTORIAL_LINE_SET_AFTER_GIVE);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = DJINN_TUTORIAL_SET_PICK;
        }
        break;
      case DJINN_TUTORIAL_SET_DONE:
        if (backdrop->tutorialTimer == 40) {
          repeat = KEY_B;
          pressed = KEY_B;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case DJINN_TUTORIAL_POWERS:
      case DJINN_TUTORIAL_POWERS_WAIT:
      case DJINN_TUTORIAL_POWERS_WAIT_MORE:
        if (backdrop->tutorialTimer == 40) {
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case DJINN_TUTORIAL_POWERS_TALK:
        if (backdrop->tutorialTimer == 60) {
          gMainRoutineState.messageSpeed = MESSAGE_SPEED_NORMAL;
          messageWindow = Message_OpenWindowFar(
              Menu_GetDjinnGiveMessage() + DJINN_TUTORIAL_LINE_POWERS, 9, 9, 1);
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
          messageWindow = Message_OpenWindowFar(Menu_GetDjinnGiveMessage() +
                                                    DJINN_TUTORIAL_LINE_UNLEASH,
                                                9, 9, 1);
          while (Message_AllTypedFar() == 0)
            wait(1);
          while (!(gInput.pressed & KEY_A)) {
            MenuCursor_Position(2, 146);
            wait(1);
          }
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          wait(1);
          messageWindow = Message_OpenWindowFar(
              Menu_GetDjinnGiveMessage() + DJINN_TUTORIAL_LINE_DEMONSTRATE, 9,
              9, 1);
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
          backdrop->tutorialStep = DJINN_TUTORIAL_POWERS_WAIT;
        }
        break;
      case DJINN_TUTORIAL_POWERS_PICK:
      case DJINN_TUTORIAL_POWERS_PICK_SPARE:
        if (backdrop->tutorialTimer == 90) {
          repeat = KEY_A;
          pressed = KEY_A;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = DJINN_TUTORIAL_POWERS_MOVE;
        }
        break;
      case DJINN_TUTORIAL_POWERS_MOVE:
        if (backdrop->tutorialTimer == 90) {
          repeat = KEY_LEFT;
          pressed = KEY_LEFT;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = DJINN_TUTORIAL_POWERS_CHOOSE;
        }
        break;
      case DJINN_TUTORIAL_POWERS_DONE:
        if (backdrop->tutorialTimer == 60) {
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = DJINN_TUTORIAL_POWERS_END;
          repeat = KEY_B;
          pressed = KEY_B;
        }
        break;
      }
    }
    // R on a Djinni: put it on standby or set it at once.
    if (mode == DJINN_SELECT_DJINNI && (pressed & KEY_R)) {
      if (selectedRow == -1) {
        Sound_PlayFar(SE_MENU_ERROR);
        continue;
      }
      groupMode = 1;
      gInput.pendingPressed = 0;
      switch (state) {
      case DJINNI_STATE_STANDBY:
        Sound_PlayFar(SE_DJINNI_SET);
        Djinn_SetFar(owner, element, number);
        DjinnRecovery_RemoveFar(owner, element, number);
        break;
      case DJINNI_STATE_SET:
        Sound_PlayFar(SE_NOTE_HIGH);
        Djinn_PutOnStandbyFar(owner, element, number);
        DjinnRecovery_AddStandbyFar(owner, element, number);
        break;
      default:
        Sound_PlayFar(SE_MENU_ERROR);
        redraw = DJINN_REDRAW_CURSOR;
        continue;
      }
      Unit_CalculateFar(owner);
      DjinnMenu_DrawPage(backdrop, memberColumn / 4 * 4);
      redraw = DJINN_REDRAW_CURSOR;
    }
    if (mode == DJINN_SELECT_DJINNI && (pressed & KEY_L)) {
      result = DJINN_RESULT_STATUS;
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if ((pressed & KEY_A) ||
        (mode == DJINN_SELECT_ACTION && (pressed & KEY_R))) {
      if (selectedRow != -1 && state == DJINNI_STATE_RECOVERING) {
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
      if (mode == DJINN_SELECT_ACTION) {
        if (memberColumn == menu->selectedColumn[0]) {
          if (menu->pairEntries[0] & DJINN_ENTRY_IS_SET)
            result = DJINN_RESULT_STANDBY;
          else
            result = DJINN_RESULT_SET;
        } else if (columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER) {
          result = DJINN_RESULT_GIVE;
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
    // Select: with R held, set every Djinni not recovering or, the next
    // time, put every one on standby; otherwise open the help.
    if (mode == DJINN_SELECT_DJINNI && (pressed & KEY_SELECT)) {
      if (groupMode) {
        setAll ^= 1;
        if (setAll)
          Sound_PlayFar(SE_DJINNI_SET);
        else
          Sound_PlayFar(SE_NOTE_HIGH);
        for (i = 0; i < menu->memberCount; i++) {
          for (j = 0; j < backdrop->counts[i]; j++) {
            u32 entry = backdrop->ids[i][j];
            s32 entryOwner = DJINN_MEMBER(entry);
            s32 entryElement = DJINN_ENTRY_ELEMENT_OF(entry);
            s32 entryNumber = DJINN_NUMBER(entry);

            switch (DjinnMenu_GetEntryState(entry)) {
            case DJINNI_STATE_STANDBY:
              if (setAll) {
                Djinn_SetFar(entryOwner, entryElement, entryNumber);
                DjinnRecovery_RemoveFar(entryOwner, entryElement, entryNumber);
                Unit_CalculateFar(entryOwner);
              }
              break;
            case DJINNI_STATE_SET:
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
        redraw = DJINN_REDRAW_CURSOR;
      } else {
        UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
        result = DJINN_RESULT_HELP;
        Sound_PlayFar(SE_MENU_CONFIRM);
        break;
      }
    }
    if (repeat & KEY_UP) {
      if (columnFlags[memberColumn] & DJINN_COLUMN_EMPTY)
        continue;
      if (backdrop->counts[memberColumn] == 0)
        continue;
      UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      if (columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER) {
        columnFlags[memberColumn] &= ~DJINN_COLUMN_ON_HEADER;
        djinnRow = 0;
      } else if (djinnRow == 0 &&
                 (columnFlags[memberColumn] & DJINN_COLUMN_HEADER_CHOOSABLE)) {
        columnFlags[memberColumn] |= DJINN_COLUMN_ON_HEADER;
        redraw = DJINN_REDRAW_DJINNI;
        continue;
      }
      djinnRow--;
      djinnRow = Menu_GetModuloOfSum(djinnRow, backdrop->counts[memberColumn]);
      savedRow = djinnRow;
      redraw = DJINN_REDRAW_DJINNI;
      continue;
    }
    if (repeat & KEY_DOWN) {
      if (columnFlags[memberColumn] & DJINN_COLUMN_EMPTY)
        continue;
      if (backdrop->counts[memberColumn] == 0)
        continue;
      UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      djinnRow++;
      djinnRow = Menu_GetModuloOfSum(djinnRow, backdrop->counts[memberColumn]);
      if (columnFlags[memberColumn] & DJINN_COLUMN_ON_HEADER) {
        columnFlags[memberColumn] &= ~DJINN_COLUMN_ON_HEADER;
        djinnRow = 0;
      } else if (djinnRow == 0 &&
                 (columnFlags[memberColumn] & DJINN_COLUMN_HEADER_CHOOSABLE)) {
        columnFlags[memberColumn] |= DJINN_COLUMN_ON_HEADER;
      }
      savedRow = djinnRow;
      redraw = DJINN_REDRAW_DJINNI;
      continue;
    }
    if (repeat & KEYS_HORIZONTAL) {
      s32 oldColumn;

      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      oldColumn = memberColumn;
      if (!(columnFlags[memberColumn] & DJINN_COLUMN_EMPTY))
        UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
      if (repeat & KEY_LEFT)
        memberColumn--;
      else
        memberColumn++;
      if (memberColumn < 0)
        memberColumn = 0;
      if (memberColumn > menu->memberCount - 1)
        memberColumn = menu->memberCount - 1;
      if (mode == DJINN_SELECT_DJINNI) {
        for (i = 0; i < menu->memberCount; i++) {
          if (columnFlags[memberColumn] != DJINN_COLUMN_EMPTY)
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
      redraw = DJINN_REDRAW_DJINNI;
      if (oldColumn / 4 * 4 != memberColumn / 4 * 4)
        redraw = DJINN_REDRAW_PAGE | DJINN_REDRAW_DJINNI;
    }
  }
  menu->selectedColumn[mode] = memberColumn;
  if (selectedRow != -1) {
    u32 id;
    u32 entry;

    menu->pairEntries[mode] = id = backdrop->ids[memberColumn][selectedRow];
    entry = (u16)id;
    menu->pairIndex[mode] = entry % (DJINN_ENTRY_INDEX + 1);
    menu->pairElement[mode] = DJINN_ENTRY_ELEMENT_OF(entry);
    menu->pairMember[mode] = DJINN_MEMBER(entry);
  }
  menu->selection[mode] = memberColumn + djinnRow * DJINN_CURSOR_ROW_STRIDE;
  DjinnFigures_SetSlotFacing(mode, FACING_SOUTH);
  return result;
}

#undef DJINN_TUTORIAL_MESSAGE

/// DJINN_ENTRY_IS_SET as a negative int, for entries built in s32.
#define DJINN_ENTRY_SET (-DJINN_ENTRY_IS_SET)

/// Lists party member memberId's Djinn of element as list entries, set ones
/// marked; for element -1, every element's, each entry with its party member.
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
          encodedDjinn = (elementIndex << DJINN_ENTRY_ELEMENT_SHIFT) |
                         djinnIndex | DJINN_ENTRY_SET;
          encodedDjinn |= memberId << DJINN_ENTRY_MEMBER_SHIFT;
          entriesOut[count++] = encodedDjinn;
        } else if (unit->djinnHeld[elementIndex] & (1 << djinnIndex)) {
          entriesOut[count++] = (elementIndex << DJINN_ENTRY_ELEMENT_SHIFT) |
                                djinnIndex |
                                (memberId << DJINN_ENTRY_MEMBER_SHIFT);
        }
      }
    }
  } else {
    for (djinnIndex = 0; djinnIndex < DJINN_PER_ELEMENT; djinnIndex++) {
      if (unit->djinnSet[element] & (1 << djinnIndex))
        entriesOut[count++] = (element << DJINN_ENTRY_ELEMENT_SHIFT) |
                              djinnIndex | DJINN_ENTRY_SET;
      else if (unit->djinnHeld[element] & (1 << djinnIndex))
        entriesOut[count++] =
            (element << DJINN_ENTRY_ELEMENT_SHIFT) | djinnIndex;
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
  struct DjinnMenuState *menu;

  menu = Ram_HeapSlots->menuState;
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

/// The stat preview's columns, in pixels from x * 8: the Japanese edition draws
/// Attack, Defense and Agility with four digits from HP's column and Luck a
/// tile further left, sets every arrow in one column, and puts the Psynergy
/// page's PP heading further left.
///
/// The Japanese text leaves the preview's messages unnamed, so each is written
/// by its place after a named one: "HP", the first of the six stat labels, 13
/// after "Luck rises"; "* Psynergy Gained" and "* Psynergy Lost", the two after
/// the status help; "* No change", the one after "Trade".
#if LANGUAGE != LANGUAGE_JAPANESE
#define DJINN_PREVIEW_DIGITS 3
#define DJINN_PREVIEW_CURRENT_X 48
#define DJINN_PREVIEW_LUCK_X 56
#define DJINN_PREVIEW_POOL_ARROW_X 62 ///< beside HP and PP
#define DJINN_PREVIEW_STAT_ARROW_X 70 ///< beside the other stats
#define DJINN_PREVIEW_PP_X 88
#define STAT_LABELS_MESSAGE ((s32)MsgPreviewStatLabel)
#define PSYNERGY_GAINED_MESSAGE ((s32)MsgPsynergyGained)
#define PSYNERGY_LOST_MESSAGE ((s32)MsgPsynergyLost)
#define NO_CHANGE_MESSAGE ((s32)MsgPsynergyUnchanged)
#else
#define DJINN_PREVIEW_DIGITS 4
#define DJINN_PREVIEW_CURRENT_X 32
#define DJINN_PREVIEW_LUCK_X 48
#define DJINN_PREVIEW_POOL_ARROW_X 64
#define DJINN_PREVIEW_STAT_ARROW_X 64
#define DJINN_PREVIEW_PP_X 56
#define STAT_LABELS_MESSAGE ((s32)MsgLuckRises + 13)
#define PSYNERGY_GAINED_MESSAGE ((s32)MsgDjinnStatusHelp + 1)
#define PSYNERGY_LOST_MESSAGE ((s32)MsgDjinnStatusHelp + 2)
#define NO_CHANGE_MESSAGE ((s32)MsgDjinnCompareTrade + 1)
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

/// `/` between the current and the maximum HP and PP in the Djinn preview.
extern const char gDjinnMenu_TextSlash[];

s32 Icon_CreateWithLoadedResource(struct Window *window, s32 x, s32 y,
                                  s32 resourceId);
s32 Icon_DrawVariantWithTileOffset(struct Window *window, s32 x, s32 y,
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
  struct DjinnMenuState *menu;

  unit = Unit_Get(ownerId);
  menu = Ram_HeapSlots->menuState;
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
    Text_DrawNumberInWindowFar(unit->attack, DJINN_PREVIEW_DIGITS, window,
                               x * 8 + DJINN_PREVIEW_CURRENT_X,
                               DJINN_PREVIEW_STAT_Y(y, 2));
    Text_DrawNumberInWindowFar(unit->defense, DJINN_PREVIEW_DIGITS, window,
                               x * 8 + DJINN_PREVIEW_CURRENT_X,
                               DJINN_PREVIEW_STAT_Y(y, 3));
    Text_DrawNumberInWindowFar(unit->agility, DJINN_PREVIEW_DIGITS, window,
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
    label = STAT_LABELS_MESSAGE;
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
        // The down arrow from the class to the next, a tile of BG
        // character block 1, the block after the windows' own.
        Window_SetTilemapEntryFar(window,
                                  WINDOW_TILE(BG_CHAR_SIZE / TILE_SIZE_4BPP +
                                              DJINN_BACKDROP_BORDER_TILE),
                                  x + 2, 5, 0);
      }
      column = x;
      if (originalUnit->classId != unit->classId)
        column += 5;
      for (i = firstElement; i < ELEMENT_COUNT; i++) {
        s32 tileColumn;
        s32 tile = ELEMENT_ICON_TILE + i;

        tileColumn = column + i * 2;
        Window_SetTilemapEntryFar(window, tile, tileColumn, y + 5, 0);
        tileColumn++;
        Window_SetTilemapEntryFar(window,
                                  unit->djinnSetCounts[i] + WINDOW_TILE('0'),
                                  tileColumn, y + 5, 0);
      }
      /* FAKEMATCH(dead): an empty once-only block after the tile loop; as no
       * block. */
      do {
      } while (0);
      if (unit->hp != originalUnit->hp) {
        Text_DrawNumberInWindowFar(unit->hp, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 0));
        if (unit->hp > originalUnit->hp)
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 0), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 0), STAT_ARROW_DOWN);
      }
      if (unit->pp != originalUnit->pp) {
        Text_DrawNumberInWindowFar(unit->pp, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 1));
        if (unit->pp > originalUnit->pp)
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 1), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 1), STAT_ARROW_DOWN);
      }
      if (unit->attack != originalUnit->attack) {
        Text_DrawNumberInWindowFar(unit->attack, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 2));
        if (unit->attack > originalUnit->attack)
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 2), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 2), STAT_ARROW_DOWN);
      }
      if (unit->defense != originalUnit->defense) {
        Text_DrawNumberInWindowFar(unit->defense, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 3));
        if (unit->defense > originalUnit->defense)
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 3), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 3), STAT_ARROW_DOWN);
      }
      if (unit->agility != originalUnit->agility) {
        Text_DrawNumberInWindowFar(unit->agility, 4, window,
                                   DJINN_PREVIEW_VALUE_X(x),
                                   DJINN_PREVIEW_STAT_Y(y, 4));
        if (unit->agility > originalUnit->agility)
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 4), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 4), STAT_ARROW_DOWN);
      }
      if (unit->luck != originalUnit->luck) {
        Text_DrawNumberInWindowFar(unit->luck, 2, window, x * 8 + 88,
                                   DJINN_PREVIEW_STAT_Y(y, 5));
        if (unit->luck > originalUnit->luck)
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 5), STAT_ARROW_UP);
        else
          Icon_DrawVariantWithTileOffset(
              window, x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
              DJINN_PREVIEW_STAT_Y(y, 5), STAT_ARROW_DOWN);
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
        Text_DrawMessageAsTilesFar(PSYNERGY_GAINED_MESSAGE, window, x * 8,
                                   y * 8 + 88);
        lines = 1;
      }
      if (lost) {
        Text_SetColorFar(TEXT_COLOR_RED);
        Text_DrawMessageAsTilesFar(PSYNERGY_LOST_MESSAGE, window, x * 8,
                                   (y + lines) * 8 + 88);
        lines++;
      }
      if (lines == 0)
        Text_DrawMessageAsTilesFar(NO_CHANGE_MESSAGE, window, x * 8,
                                   y * 8 + 88);
      Text_SetColorFar(TEXT_COLOR_NORMAL);
      Window_DrawDividerLineFar(window, 0, 11, 13, 11);
    }
    ((struct WindowState *)Ram_HeapSlots->windowState)->dirty = 1;
  }

  size = sizeof(struct Unit);
  if (page == 0)
    Portrait_CreateFar(ownerId, 0, side, window, 0, 0);
  Iwram_CopyWordsEntry(unit, originalUnit, size);
  Heap_Release(originalUnit);
  return 1;
}

#undef NO_CHANGE_MESSAGE

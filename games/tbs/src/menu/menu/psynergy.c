// Part of menu.c, the menu object, compiled only through it.
// The Psynergy menu: the screen events open, the member and Psynergy lists,
// targets, and the L and R shortcuts.

#include "../menu_internal.h"

// The German, Spanish, French and Italian Psynergy menus save the window
// tiles they draw over and restore them after.
#if TBS_PATCH_2
#define PSYNERGY_PROMPT_TILES BG_CHAR_BLOCK(1)
#define PSYNERGY_PROMPT_TILES_SIZE 0x2000
#endif

// The highlight over the Psynergy list's rows.
#if TBS_PATCH_1
#define PSYNERGY_DETAIL_ROW_X 0
#define PSYNERGY_DETAIL_ROW_WIDTH 15
#else
#define PSYNERGY_DETAIL_ROW_X 1
#define PSYNERGY_DETAIL_ROW_WIDTH 14
#endif

// The Psynergy list's page icons, "PP" label, names and hand cursor.
#if TBS_PATCH_1
#define PSYNERGY_PAGE_ICONS_X 112
#define PSYNERGY_PP_LABEL_X 96
#define PSYNERGY_NAME_X 16
#define PSYNERGY_LIST_CURSOR_X 88
#else
#define PSYNERGY_PAGE_ICONS_X 120
#define PSYNERGY_PP_LABEL_X 72
#define PSYNERGY_NAME_X 32
#define PSYNERGY_LIST_CURSOR_X 98
#endif

// The Psynergy menu an event opens (window command 1): the field menu's
// Psynergy screen over the field, which stops meanwhile. When the player picks
// a Psynergy to use on the field, it is recorded in the event work's
// psynergyRequest (owner << 10 | Psynergy) and 1 is returned; -1 when the
// player backs out.
s32 Menu_OpenConfirmPrompt(void) {
#if TBS_PATCH_2
  void *saved = Runtime_BumpAllocateAlternatePool(PSYNERGY_PROMPT_TILES_SIZE);
#endif
  struct PsynergyMenuState *state =
      Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
  s32 owner;
  s32 unused;
  s32 psynergy;
  s32 result;

  ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  UiWindow_InitializeWork(0);
  state->ownerCount = Party_ListActiveOwnersFar(state->ownerTable);
  ItemMenu_Init(0, 3, 0, 7);
  state->messageWindow = UiWindow_CreateFar(13, 0, 17, 3, 2);
  Palette_LightenBankHighlight(14);
  Link_DrawShiftedTilePairFar(MENU_PAGE_ARROW_VRAM);
#if TBS_PATCH_2
  Scheduler_EnableOverlayCallbacksWithFlags();
  Iwram_CopyWords(saved, PSYNERGY_PROMPT_TILES, PSYNERGY_PROMPT_TILES_SIZE);
  Iwram_FillWords(PSYNERGY_PROMPT_TILES, PSYNERGY_PROMPT_TILES_SIZE,
                  0x33333333);
  UiWork_SetAltFlagAndClearTableFar(1);
#endif
  // Let the player choose.
  Menu_CancelSoundReset();
  result = Menu_ResolveSelectedAction(&owner, &unused, &psynergy);
  Menu_EnsureCancelSound();
  if (result == 1) {
    struct EventWork *target = WORK_SLOT(HEAP_SLOT_EVENT);
    u16 request;
    BattleActGet(OWNER_ACTION_ID_MASK & state->paneAction[0]);
    request = psynergy | (owner << 10);
    target->psynergyRequest = request;
  }
  // Close the menu and give the screen back to the field.
  RenderOutput_ClearListFar((void *)state->statusWindow);
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menuBusy = 1;
  ItemMenu_Close();
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
  ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 0;
#if TBS_PATCH_2
  UiWindow_MarkVisibleTileAttributesFar();
  UiWork_SetAltFlagAndClearTableFar(0);
  Iwram_CopyWords(PSYNERGY_PROMPT_TILES, saved, PSYNERGY_PROMPT_TILES_SIZE);
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menuBusy = 0;
  MemFree(saved);
  WaitFrames(1);
  Scheduler_DisableOverlayCallbacksWithFlags();
#endif
  WaitFrames(1);
  UiWindow_EraseBorderRectFar(0, 0, 30, 20);
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menuBusy = 0;
  Event_ClearInvalidPackedValuesFar();
  return result;
}

// The steps of the Psynergy menu (Menu_ResolveSelectedAction).
enum PsynergyMenuStep {
  PSYNERGY_STEP_OWNER,
  PSYNERGY_STEP_LIST,
  PSYNERGY_STEP_CLASSIFY,
  PSYNERGY_STEP_TARGET,
  PSYNERGY_STEP_APPLY,
};

// Runs the Psynergy menu: choose a party member and one of their Psynergy,
// then use it on a member (or the whole party), or, in the shortcut modes, set
// it as the L or R shortcut. Returns 1 with the owner and Psynergy in the out
// parameters when the chosen Psynergy works on the field, to be used once the
// menu closes; -1 when the player backs out or presses Start; otherwise the
// result of the last use.
s32 Menu_ResolveSelectedAction(s32 *outOwner, s32 *unused, s32 *outAction) {
  struct PsynergyMenuState *menu;
  s32 useResult;
  s32 step;
  s32 done;
  s32 chosenAction;
  s32 targetMode;
  s32 actionEntry;
  s32 chosenTarget;
  struct BattleAct *action;

  step = PSYNERGY_STEP_OWNER;
  done = 0;
  menu = gMenuWork;
  useResult = 0;

  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    switch (step) {
    // Choose whose Psynergy to look at.
    case PSYNERGY_STEP_OWNER: {
      s32 cancelled;
      s32 chosen;

      menu->paneRow[0] = 0;
      ItemMenu_DrawMsg(0, MsgPsynergyChooseOwner);
      chosen = PsynergyMenu_SelectPartySlot(0);
      cancelled = -1;
      /* FAKEMATCH(dead): a redundant copy of -1 into cancelled and a test of
       * equality as an XOR, which end the CSE block so that -1 and done's 1
       * take the ROM's registers; as if (chosen == -1) setting done and
       * useResult to -1, 6 of 317 instructions differ in tbs-en. */
      if (chosen == -1)
        cancelled = chosen;
      if (((u32)chosen ^ (u32)cancelled) == 0) {
        done = 1;
        useResult = cancelled;
      }
      RenderOutput_RedrawSavedRectFar(menu->infoWindow);
      step = PSYNERGY_STEP_LIST;
      break;
    }

    // Choose one of their Psynergy; in the shortcut modes, set it as the L or
    // R shortcut and start again.
    case PSYNERGY_STEP_LIST:
      WaitFrames(1);
      UnitGetFar(menu->ownerIds[0]);
      step = PSYNERGY_STEP_OWNER;
      if (menu->psynergyCount != 0) {
        {
          struct PsynergyMenuState *savedWork = menu;

          /* FAKEMATCH(dead): menu cleared and restored under a test that is
           * never true here, which puts labels between the psynergyCount and
           * mode loads, so that the mode's offset is built from its own
           * constant (movs #154, lsls #2) as the bytes do; without the two
           * tests, 2 of 317 instructions differ in tbs-en. */
          if (done)
            menu = NULL;
          if (done)
            menu = savedWork;
        }
        switch (menu->mode) {
        case 0: // use
          ItemMenu_DrawMsg(0, MsgPsynergyChooseAbility);
          break;
        case 1: // set the L shortcut
          ItemMenu_DrawMsg(0, MsgPsynergyChooseForL);
          break;
        case 2: // set the R shortcut
          ItemMenu_DrawMsg(0, MsgPsynergyChooseForR);
          break;
        }
        ItemMenu_PosCategory();
        Menu_DrawOwnerStatusPanel(menu->statusWindow, menu->ownerIds[0], 0,
                                  STATUS_PANEL_EXPERIENCE);
        chosenAction = PsynergyMenu_RunList(0);
        step = PSYNERGY_STEP_OWNER;
        if (chosenAction != -1) {
          step = PSYNERGY_STEP_CLASSIFY;
          if (menu->mode != 0) {
            if (menu->mode == 1) {
              PsynergyMenu_SetShortcut(menu->ownerIds[0], chosenAction, 0);
              RenderOutput_ClearListFar((void *)menu->infoWindow);
              InventoryMenu_ShowModalMessage(MsgShortcutSetL, -1, -1);
#if !TBS_PATCH_1
              RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
            } else {
              PsynergyMenu_SetShortcut(menu->ownerIds[0], chosenAction, 1);
              RenderOutput_ClearListFar((void *)menu->infoWindow);
              InventoryMenu_ShowModalMessage(MsgShortcutSetR, -1, -1);
#if !TBS_PATCH_1
              RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
            }
            step = PSYNERGY_STEP_OWNER;
          }
        }
      }
      break;

    // Choose the member to use it on.
    case PSYNERGY_STEP_TARGET:
      ItemMenu_DrawMsg(0, MsgPsynergyChooseTarget);
      chosenTarget = PsynergyMenu_SelectTarget(0);
      step = PSYNERGY_STEP_APPLY;
      if (chosenTarget == -1) {
        menu->flags |= 1;
        step = PSYNERGY_STEP_LIST;
      }
      break;

    // How is it used? On one member, on the whole party, or on the field
    // once the menu closes.
    case PSYNERGY_STEP_CLASSIFY:
      targetMode = PsynergyMenu_ClassifySelectedPsynergy();
      if (targetMode == 1) {
        step = PSYNERGY_STEP_TARGET;
        break;
      }
      if (targetMode == 2) {
        menu->ownerIds[1] = MENU_TARGET_WHOLE_PARTY;
        step = PSYNERGY_STEP_APPLY;
        break;
      }
      done = 1;
      useResult = 1;
      *outOwner = menu->ownerIds[0];
      *outAction = menu->paneAction[0] & OWNER_ACTION_ID_MASK;
      break;

    // Use it: pay its PP, show the result and go back to the list, or to the
    // target when it failed on one member.
    case PSYNERGY_STEP_APPLY:
      chosenTarget = 0;
      actionEntry = menu->paneAction[0];
      useResult = BattleEffect_ApplyToTargets(actionEntry, menu->ownerIds[0],
                                              menu->ownerIds[1], 0);
      if (menu->ownerIds[1] == MENU_TARGET_WHOLE_PARTY) {
        menu->ownerIds[1] = menu->ownerIds[0];
        chosenTarget = MENU_TARGET_WHOLE_PARTY;
      }
      if (useResult != -1) {
        action = BattleActGet(menu->paneAction[0] & OWNER_ACTION_ID_MASK);
        Unit_AddPpFar(menu->ownerIds[0], -action->ppCost);
      }
      UnitCalcFar(menu->ownerIds[0]);
      if (useResult != -1) {
        Menu_DrawOwnerStatusPanel(menu->statusWindow, menu->ownerIds[1], 0,
                                  STATUS_PANEL_EXPERIENCE);
        Ability_PlayUseAnimation(menu->paneAction[0] & OWNER_ACTION_ID_MASK);
        RenderOutput_ClearListFar((void *)menu->infoWindow);
        InventoryMenu_ShowModalMessage(menu->useResult + MsgItemUseResult, 0,
                                       -1);
      } else {
        AudioCommand_PlayFar(SE_MENU_ERROR);
        RenderOutput_ClearListFar((void *)menu->infoWindow);
        InventoryMenu_ShowModalMessage(menu->useResult + MsgItemUseResult,
                                       useResult, useResult);
      }
      if (useResult != -1) {
        useResult = 1;
        menu->flags |= 1;
        step = PSYNERGY_STEP_LIST;
      } else {
        menu->skipSlide = 1;
        if (chosenTarget == MENU_TARGET_WHOLE_PARTY) {
          menu->flags |= 1;
          step = PSYNERGY_STEP_LIST;
        } else {
          step = PSYNERGY_STEP_TARGET;
        }
      }
      break;

    default:
      done = 1;
      break;
    }
  }

  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0) {
    useResult = -1;
  }
  return useResult;
}

// Says how the selected Psynergy is used from the menu: 0 on the field once the
// menu closes (its effect has a field event), 2 on the whole party (range
// 0xff), 1 on one chosen member when its target mode is 2, and 0 otherwise.
s32 PsynergyMenu_ClassifySelectedPsynergy(void) {
  struct BattleAct *action;
  s32 targetModeDifference;
  s32 classification;

  action = BattleActGet(OWNER_ACTION_ID_MASK &
                        ((struct PsynergyMenuState *)gMenuWork)->paneAction[0]);
  if (BattleFx_HasMatchingEvent5Far(action->type0c) != 0) {
    return 0;
  }
  classification = 2;
  if (action->range != 0xff) {
    u8 kind = action->targetMode;
    // 1 when kind is 2, else 0.
    targetModeDifference = kind ^ 2;
    classification = (0 - targetModeDifference) | targetModeDifference;
    classification = (u32)classification >> 0x1f;
    classification = 1 - classification;
  }
  return classification;
}

// Lets the player choose a party member in pane partySlot (0 or 1), starting
// from the one chosen last, with each member's Psynergy shown as the cursor
// passes. Returns the chosen member's id, or -1 when the player backs out.
s32 PsynergyMenu_SelectPartySlot(s32 partySlot) {
  /* FAKEMATCH(temporary): the pane's tab, icon and owner reached through
   * byte offsets from the work held in scalars, which keeps partySlot in its
   * own register beside the icon's offset; as menu->paneIcon[partySlot],
   * menu->tabIndex[partySlot] and menu->ownerTable[ownerIndex], 19 of 75
   * instructions differ in tbs-en. */
  struct PsynergyMenuState *menu = gMenuWork;
  s32 tabOffset = partySlot + ((u8 *)menu->tabIndex - (u8 *)menu);
  s32 iconOffset = partySlot * sizeof(menu->paneIcon[0]) +
                   ((u8 *)menu->paneIcon - (u8 *)menu);
  struct RenderOutput *icon;
  u8 ownerCount;
  s32 ownerIndex;
  s32 ownerOffset;
  s32 tableOffset;
  s32 ownerId;
  struct BattleUnit *unit;
  u16 *actions;
  u8 *counts;
  s32 actionCount;
  s32 result;
  s32 iconOffset2;

  result = 0;
  icon = *(struct RenderOutput **)((u8 *)menu + iconOffset);
  icon->active = ICON_SHOWN;
  icon->animStep = result;
  menu->cursorIcon->active = ICON_HIDDEN;
  counts = (u8 *)menu + ((u8 *)menu->tabCounts - (u8 *)menu->tabIndex);
  ownerIndex = *(s8 *)((u8 *)menu + tabOffset);
  ownerCount = menu->ownerCount;
  counts[tabOffset] = ownerCount;

  if (ownerIndex == -1) {
    *((u8 *)menu + tabOffset) = 0;
    ownerOffset = 0;
  } else {
    ownerOffset = ownerIndex * sizeof(menu->ownerTable[0]);
    UiMenu_SlideCursor(ownerIndex * 24 - 10, 16);
  }

  tableOffset = ownerOffset + ((u8 *)menu->ownerTable - (u8 *)menu);
  ownerId = *(u16 *)((u8 *)menu + tableOffset);
  unit = UnitGetFar(ownerId);
  actions = menu->psynergies;
  actionCount = PsynergyMenu_CollectActions(unit, actions, 2);
  menu->psynergyCount = actionCount;
  result = PsynergyMenu_SetupActionIcons(menu->ownerTable, actions);

  iconOffset2 = partySlot * sizeof(menu->paneIcon[0]) +
                ((u8 *)menu->paneIcon - (u8 *)menu);
  icon = *(struct RenderOutput **)((u8 *)menu + iconOffset2);
  UiIcon_PrepareObject(icon);
  WaitFrames(1);
  return result;
}

// The party member row of the Psynergy menu. Left and Right move between
// members, showing each one's Psynergy, status and shortcuts; A chooses a
// member who has Psynergy to use, L or R one who has field Psynergy to set as
// that shortcut (menu mode 1 or 2), and B backs out. Returns the chosen
// member's id, or -1.
s32 PsynergyMenu_SetupActionIcons(u16 *ownerIds, u16 *unused) {
  struct PsynergyMenuState *menu;
  s32 selection;
  s32 ownerCount;
  s32 redraw;
  s32 result;
  struct BattleUnit *unit;
  struct UiWindow *window;
  s32 shown;
  s32 i;
  u16 *actions;
  s32 shortcutCount;
  s8 fieldCount;

  menu = gMenuWork;
  selection = menu->tabIndex[0];
  ownerCount = menu->tabCounts[0];
  redraw = 1;
  result = 0;
  shown = 0;
  menu->mode = result;
  unit = UnitGetFar(ownerIds[selection]);
  if (UiWindow_UpdateOrCreate(&menu->psynergyWindow, 13, 3, 17, 10, 2))
    Menu_SpawnIconEntries((struct InventoryMenuState *)menu,
                          menu->psynergyWindow);
  if (UiWindow_UpdateOrCreate(&menu->shortcutWindow, 13, 13, 17, 4, 2)) {
    menu->cursorIcon = RenderOutput_CreateFromResourceFar(
        2, 0, menu->shortcutWindow, 0, result);
    menu->cursorIcon->active = ICON_HIDDEN;
  }
  while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      selection = (ownerCount + selection) % ownerCount;
      window = menu->statusWindow;
      unit = UnitGetFar(ownerIds[selection]);
      PsynergyMenu_RefreshOwnerPsynergy(ownerIds[selection]);
      Menu_DrawOwnerStatusPanel(window, ownerIds[selection], 0,
                                STATUS_PANEL_EXPERIENCE);
      PsynergyMenu_DrawShortcuts(menu->shortcutWindow, ownerIds[selection]);
      PsynergyMenu_CallIconRoutineWithValue(menu, ownerIds[selection]);
      for (i = 3; i >= 0; i--)
        menu->rowPositions[i] = 30;
      // Raise the chosen member.
      menu->rowPositions[selection] = 26;
      if (!GameFlag_TestFar(FLAG_MENU_INFO_KEPT) && !shown) {
        RenderOutput_ClearListFar((void *)menu->infoWindow);
#if TBS_PATCH_1
        RenderOutput_RedrawSavedRectFar((void *)menu->infoWindow);
#endif
        UiText_DrawWorkValueWithLabel(menu->infoWindow);
        shown = 1;
      } else {
        GameFlag_ClearBitFar(FLAG_MENU_INFO_KEPT);
      }
    }
    UiMenu_PositionCursor(selection * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      if (menu->psynergyCount) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        result = ownerIds[selection];
        break;
      }
      AudioCommand_PlayFar(SE_MENU_ERROR);
    }
    // L or R: set that shortcut, if the member has field Psynergy for it.
    if ((gKeysPressed & KEY_L) || (gKeysPressed & KEY_R)) {
      result = ownerIds[selection];
      if (gKeysPressed & KEY_L)
        menu->mode = 1;
      else
        menu->mode = 2;
      actions = MemAlloc(32 * sizeof(u16));
      fieldCount =
          PsynergyMenu_CollectActions((struct BattleUnit *)unit, actions, 1);
      MemFree(actions);
      shortcutCount = fieldCount;
      if (shortcutCount == 0) {
        menu->mode = shortcutCount;
        AudioCommand_PlayFar(SE_MENU_ERROR);
      } else {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        break;
      }
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selection--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selection++;
      redraw = 1;
    }
  }
  menu->tabIndex[0] = selection;
  menu->selectedOwner = ownerIds[selection];
  menu->ownerIds[0] = ownerIds[selection];
  return result;
}

// Shows member ownerId's Psynergy icons in the Psynergy window, eight to a
// row, or "no Psynergy" when they have none.
void PsynergyMenu_RefreshOwnerPsynergy(s32 ownerId) {
  u16 *psynergies;
  struct PsynergyMenuState *menu;
  struct BattleUnit *owner;

  menu = gMenuWork;
  owner = UnitGetFar(ownerId);
  psynergies = menu->psynergies;
  menu->psynergyCount = PsynergyMenu_CollectActions(owner, psynergies, 2);
  RenderOutput_RedrawSavedRectFar((void *)menu->psynergyWindow);
  PsynergyMenu_RefreshOwnerEntries(108, 32, 8);
  PsynergyMenu_DrawPsynergyIcons(psynergies);
  if (menu->psynergyCount == 0) {
    UiText_DrawCharacterAtOffsetFar(MsgPsynergyMenuEmpty, menu->psynergyWindow,
                                    0, 24);
  }
}

// Return 1, and do nothing; nothing calls them.
UNUSED s32 PsynergyMenu_ReturnTrue(void) { return 1; }

UNUSED void PsynergyMenu_NoOp(void) {}

// Chooses the party member to use the selected Psynergy on. In mode 0 each
// member's status shows as the cursor passes, with the Psynergy's description
// once. Returns the chosen member's id, or -1 when the player backs out.
s32 PsynergyMenu_SelectTarget(s32 targetMode) {

  struct PsynergyMenuState *menu;
  s32 selection;
  s32 count;
  s32 redraw;
  s32 result;
  s32 shown;
  struct RenderOutput *marker;

  menu = gMenuWork;
  selection = menu->tabIndex[1];
  count = menu->ownerCount;
  redraw = 1;
  result = 0;
  shown = 0;
  UnitGetFar(menu->ownerTable[menu->tabIndex[0]]);
  UiMenu_SlideCursor(selection * 24 - 10, 16);
  while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      selection = (selection + count) % count;
      UnitGetFar(menu->ownerTable[selection]);
      marker = menu->paneIcon[1];
      marker->oam.x = marker->x =
          (((menu->auxiliaryWindow)->x + selection * 3) << 3) - 2;
      if (targetMode == 0) {
        Menu_DrawOwnerStatusPanel(menu->statusWindow,
                                  menu->ownerTable[selection], 0,
                                  STATUS_PANEL_EXPERIENCE);
        PsynergyMenu_CallIconRoutineWithValue(menu,
                                              menu->ownerTable[selection]);
        if (!GameFlag_TestFar(FLAG_MENU_INFO_KEPT) && !shown) {
          MENU_RESET_INFO_WINDOW((void *)menu->infoWindow);
          MENU_DRAW_INFO_TEXT((menu->paneAction[0] & OWNER_ACTION_ID_MASK) +
                                  MsgAbilityDescription,
                              menu->infoWindow, 0, 0);
          shown = 1;
        } else {
          GameFlag_ClearBitFar(FLAG_MENU_INFO_KEPT);
        }
      }
    }
    UiMenu_PositionCursor(selection * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      result = menu->ownerTable[selection];
      break;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selection--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selection++;
      redraw = 1;
    }
  }
  marker = menu->paneIcon[1];
  menu->tabIndex[1] = selection;
  UiIcon_PrepareObject(marker);
  marker->active = ICON_HIDDEN;
  WaitFrames(1);
  menu->tabIndex[1] = selection;
  menu->selectedOwner = menu->ownerTable[selection];
  menu->ownerIds[1] = menu->ownerTable[selection];
  return result;
}

// A Psynergy shortcut: its owner above bit 10, the Psynergy's id below.
#define PSYNERGY_SHORTCUT_OWNER_SHIFT 10
#define PSYNERGY_SHORTCUT_ID_MASK 0x3ff

// Sets the L (shortcutIndex 0) or R (shortcutIndex 1) Psynergy shortcut:
// ownerId << 10 | the Psynergy's id. Returns 1.
s32 PsynergyMenu_SetShortcut(s32 ownerId, s32 actionId, s32 shortcutIndex) {
  s32 id = actionId & OWNER_ACTION_ID_MASK;
  s32 code = (ownerId << PSYNERGY_SHORTCUT_OWNER_SHIFT) | id;

  if (shortcutIndex == 0) {
    gGameState.firstShortcut = code;
  } else {
    gGameState.secondShortcut = code;
  }
  return 1;
}

// Writes the L and R Psynergy shortcuts into window: each one's Psynergy and
// owner, or that it is empty, under a heading that says how to change them.
// A name too wide for the line hides the owner (international) or drops the
// colon after "L" and "R" (Japanese). Returns 1.
#if LANGUAGE != LANGUAGE_JAPANESE

s32 PsynergyMenu_DrawShortcuts(struct UiWindow *window, s32 ownerId) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  s32 wide;

  if (gGameState.firstShortcut != 0 && gGameState.secondShortcut != 0)
    UiText_DrawCharacterAtOffsetFar(MsgShortcutChangeHelp, window, 0, -8);
  else
    UiText_DrawCharacterAtOffsetFar(MsgShortcutLabel, window, 0, -8);

  UiText_GetResourceDimensionsFar(
      (gGameState.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
      &left, &top, &width, &height);
  if (width < 0 || width > 10)
    wide = 1;
  else
    wide = 0;
  if (gGameState.firstShortcut != 0) {
    UiWork_PushValueSlotFar(
        gGameState.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK, 4);
    UiText_DrawCharacterAtOffsetFar(MsgShortcutNameL, window, 0, 0);
    if (wide == 0)
      UiText_DrawStringAtOffsetFar(
          UnitGetFar(gGameState.firstShortcut >> PSYNERGY_SHORTCUT_OWNER_SHIFT)
              ->name,
          window, 80, 0);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyL, window, 0, 0);
  }

  UiText_GetResourceDimensionsFar(
      (gGameState.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
      &left, &top, &width, &height);
  if (width < 0 || width > 10)
    wide = 1;
  else
    wide = 0;
  if (gGameState.secondShortcut != 0) {
    UiWork_PushValueSlotFar(
        gGameState.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK, 4);
    UiText_DrawCharacterAtOffsetFar(MsgShortcutNameR, window, 0, 8);
    if (wide == 0)
      UiText_DrawStringAtOffsetFar(
          UnitGetFar(gGameState.secondShortcut >> PSYNERGY_SHORTCUT_OWNER_SHIFT)
              ->name,
          window, 80, 8);
    UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyR, window, 0, 8);
  }
  return 1;
}
#else

s32 PsynergyMenu_DrawShortcuts(struct UiWindow *window, s32 owner) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  u16 wide;

  if (gGameState.firstShortcut != 0 && gGameState.secondShortcut != 0)
    UiText_DrawCharacterAtOffsetFar(MsgShortcutChangeHelp, window, 0, -8);
  else
    UiText_DrawCharacterAtOffsetFar(MsgShortcutLabel, window, 0, -8);

  UiText_GetResourceDimensionsFar(
      (gGameState.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
      &left, &top, &width, &height);
  if (width < 0 || width > 11)
    wide = 1;
  else
    wide = 0;
  if (wide)
    UiText_DrawStringInWindowFar((u8 *)gMenu_ShortcutLString, window, 0, 0);
  else
    UiText_DrawStringInWindowFar((u8 *)gMenu_ShortcutLColonString, window, 0,
                                 0);
  if (gGameState.firstShortcut != 0) {
    if (wide)
      UiText_DrawCharacterAtOffsetFar(
          (gGameState.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
              MsgAbilityName,
          window, 8, 0);
    else
      UiText_DrawCharacterAtOffsetFar(
          (gGameState.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
              MsgAbilityName,
          window, 16, 0);
    UiText_DrawStringAtOffsetFar(
        UnitGetFar(gGameState.firstShortcut >> PSYNERGY_SHORTCUT_OWNER_SHIFT)
            ->name,
        window, 80, 0);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyL, window, 24, 0);
  }

  UiText_GetResourceDimensionsFar(
      (gGameState.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
      &left, &top, &width, &height);
  if (width < 0 || width > 11)
    wide = 1;
  else
    wide = 0;
  if (wide)
    UiText_DrawStringInWindowFar((u8 *)gMenu_ShortcutRString, window, 0, 8);
  else
    UiText_DrawStringInWindowFar((u8 *)gMenu_ShortcutRColonString, window, 0,
                                 8);
  if (gGameState.secondShortcut != 0) {
    if (wide)
      UiText_DrawCharacterAtOffsetFar(
          (gGameState.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
              MsgAbilityName,
          window, 8, 8);
    else
      UiText_DrawCharacterAtOffsetFar(
          (gGameState.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
              MsgAbilityName,
          window, 16, 8);
    UiText_DrawStringAtOffsetFar(
        UnitGetFar(gGameState.secondShortcut >> PSYNERGY_SHORTCUT_OWNER_SHIFT)
            ->name,
        window, 80, 8);
    UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyR, window, 24, 8);
  }
  return 1;
}
#endif

// Sets up a Psynergy grid screen: the cursor window with the party's figures,
// a 30 by 15 window below with a hidden grid cursor and a portrait, and 16
// Psynergy icon sprites in two rows of eight from (96, 8), on an 8 by 2 grid.
void PsynergyMenu_CreateEntryGrid(void) {
  struct UiWindow *window;
  struct PsynergyMenuState *menu;
  struct RenderOutput *cursor;
  s32 index;
  s32 x;
  s32 y;
  struct RenderOutput **output;

  menu = gMenuWork;
  window = UiMenu_CreateCursor(menu);
  PsynergyMenu_InitializeEntryObjects((void *)window, 2, 2, 8, 0);

  window = UiWindow_CreateFar(0, 5, 30, 15, 2);
  menu->psynergyWindow = window;
  menu->selectedColumn = 0;
  menu->selectedRow = 0;
  menu->columnCount = 8;
  menu->rowCount = 2;

  cursor =
      (struct RenderOutput *)UiIcon_CreateWithResourceVariant(window, 0, 4);
  cursor->active = ICON_HIDDEN;
  menu->entryGridCursor = cursor;
  SideObject_CreateFar(0, 0, 0, window, 0, 0);

  y = 8;
  index = 0;
  output = &menu->entryIcons[0];
  x = 96;
  do {
    *output++ = RenderOutput_CreateFromResourceFar(4, index, window, x, y);
    index++;
    x += 16;
  } while (index <= 7);

  index = 8;
  y = 24;
  output = &menu->entryIcons[8];
  x = 96;
  do {
    *output++ = RenderOutput_CreateFromResourceFar(4, index, window, x, y);
    index++;
    x += 16;
  } while (index <= 15);
}

// Releases the party figures and closes the Psynergy grid screen's windows.
void PsynergyMenu_CloseWindows(void) {
  struct PsynergyMenuState *menu;

  menu = gMenuWork;
  Menu_ReleaseEntryObjects();
  UiWork_FinalizeFar((void *)menu->auxiliaryWindow, 1);
  UiWork_FinalizeFar((void *)menu->psynergyWindow, 1);
  UiWork_FinalizeFar((void *)menu->messageWindow, 1);
}

// Loads each listed Psynergy's icon into its icon sprite (32 entries) and hides
// the icons of empty entries.
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies) {
  s32 remaining;
  struct RenderOutput **icons;
  u16 *p;
  s32 psynergyId;

  icons = ((struct PsynergyMenuState *)gMenuWork)->entryIcons;
  p = psynergies;
  remaining = 31;
  do {
    psynergyId = *p++;
    if (psynergyId != 0) {
      Resource_LoadByModeIntoSlotFar(4, psynergyId, (u8)(*icons)->index, 0);
    }
    icons++;
    remaining--;
  } while (remaining >= 0);
  Menu_HideEmptyEntryIcons(psynergies);
}

// Lists unit's Psynergy into actions (32 entries, cleared first) and returns
// how many there are. mode 1 lists only the Psynergy that work on the field
// (those that can be shortcuts); mode 2 those with a field effect or usable
// in the field; any other mode those first, then the rest.
s32 PsynergyMenu_CollectActions(struct BattleUnit *unit, u16 *actions,
                                s32 mode) {
  /* FAKEMATCH(dead): each entry cleared twice through the byte-offset
   * pointer, which makes loop.c replace the counter with the pointer as the
   * ROM does; with a single store, 15 of 129 instructions differ in tbs-en. */
  s32 clearOffset;
  s32 passCount;
  s32 actionCount;
  s32 slotIndex;
  s32 passIndex;

  passCount = (mode != 2) ? 4 : 3;
  for (clearOffset = 62; clearOffset >= 0; clearOffset -= 2) {
    u16 *q = (u16 *)((u8 *)actions + clearOffset);
    *q = 0;
    *q = 0;
  }
  actionCount = 0;

  // type0c is the Psynergy's field effect. Passes 1 and 2 add nothing.
  if (mode == 1) {
    for (slotIndex = 0; slotIndex < 32; slotIndex++) {
      if (unit->actionSlots[slotIndex].encodedAction != 0) {
        if (BattleActGet(unit->actionSlots[slotIndex].encodedAction &
                         OWNER_ACTION_ID_MASK)
                ->type0c != 0) {
          actions[actionCount] = unit->actionSlots[slotIndex].encodedAction;
          actionCount++;
        }
      }
    }
  } else {
    for (passIndex = 0; passIndex < passCount; passIndex++) {
      for (slotIndex = 0; slotIndex < 32; slotIndex++) {
        if (unit->actionSlots[slotIndex].encodedAction != 0) {
          struct BattleAct *ability =
              BattleActGet(unit->actionSlots[slotIndex].encodedAction &
                           OWNER_ACTION_ID_MASK);

          if (passIndex == 0 &&
              (ability->type0c != 0 ||
               (ability->flags & ACTION_USABLE_IN_FIELD) != 0)) {
            actions[actionCount] = unit->actionSlots[slotIndex].encodedAction;
            actionCount++;
          } else if (passIndex == 1) {
          } else if (passIndex == 2) {
          } else if (passIndex == 3 && ability->type0c == 0 &&
                     (ability->flags & ACTION_USABLE_IN_FIELD) == 0) {
            actions[actionCount] = unit->actionSlots[slotIndex].encodedAction;
            actionCount++;
          }
        }
      }
    }
  }

  return actionCount;
}

// Fills result with the Psynergy list of the member in pane index: their unit,
// how many Psynergy are listed, and the page and row of the one chosen last for
// them (kept within the list), five to a page. Returns 1.
s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index) {
  s32 unitBits;
  struct PsynergyMenuState *menu = gMenuWork;
  s32 entryCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selectedIndex;
  s32 ownerId;

  unitBits = (s32)UnitGetFar(menu->ownerIds[index]);
  entryCount = menu->psynergyCount;
  ownerId = menu->ownerIds[index];
  selectedIndex = menu->selectedIndexByOwner[ownerId];
  if ((selectedIndex + 1) > entryCount) {
    selectedIndex = entryCount - 1;
  }
  page = selectedIndex / LIST_PAGE_SIZE;
  row = selectedIndex % LIST_PAGE_SIZE;
  pageCount = entryCount / LIST_PAGE_SIZE;
  if (entryCount % LIST_PAGE_SIZE != 0) {
    pageCount++;
  }
  result->ownerState = unitBits;
  result->page = page;
  result->pageCount = pageCount;
  result->row = row;
  result->entryCount = entryCount;
  result->selectedIndex = selectedIndex;
  return 1;
}

// PsynergyMenuState.flags: the Psynergy list shows the shortcut help (Select
// held).
#define PSYNERGY_MENU_SHORTCUT_HELP 2

// Updates the Psynergy list for the row under the cursor: puts the Psynergy's
// description in the info window, unless a kept message is there, and
// highlights its row of the five. window and work are not used.
s32 PsynergyMenu_DrawDetailPage(struct UiWindow *window, s32 *work,
                                struct MenuResult *state) {
  struct PsynergyMenuState *menu;
  s32 actionIndex;
  s32 rowIndex;

  menu = gMenuWork;
  actionIndex = state->page * LIST_PAGE_SIZE;
  actionIndex += state->row;
  state->selectedIndex = actionIndex;

  if (GameFlag_TestFar(FLAG_MENU_INFO_KEPT) == 0) {
    MENU_RESET_INFO_WINDOW(menu->infoWindow);
    WaitFrames(1);

    actionIndex = state->selectedIndex;
    if (menu->psynergies[actionIndex] != 0) {
      s32 description =
          (menu->psynergies[actionIndex] & 0x1ff) + MsgAbilityDescription;
      MENU_DRAW_INFO_TEXT(description, menu->infoWindow, 0, 0);
    }
  } else {
    // BUG: clears scene flag 0x2ff instead of FLAG_MENU_INFO_KEPT, so a kept
    // message keeps the descriptions away until another screen clears it.
#ifdef BUGFIX
    GameFlag_ClearBitFar(FLAG_MENU_INFO_KEPT);
#else
    GameFlag_ClearBitFar(0x2ff);
#endif
  }

  rowIndex = 0;
  do {
    if (rowIndex == state->row) {
      Render_SetTilemapFlagRect((void *)menu->psynergyWindow,
                                PSYNERGY_DETAIL_ROW_X, rowIndex * 2 + 1,
                                PSYNERGY_DETAIL_ROW_WIDTH, 1,
                                14); // highlighted
    } else {
      Render_SetTilemapFlagRect((void *)menu->psynergyWindow,
                                PSYNERGY_DETAIL_ROW_X, rowIndex * 2 + 1,
                                PSYNERGY_DETAIL_ROW_WIDTH, 1,
                                15); // normal
    }
    rowIndex++;
  } while (rowIndex < LIST_PAGE_SIZE);

  WaitFrames(1);
  return 1;
}

// Draws one page of the Psynergy list into window: up to five names with their
// icons and PP costs, a Psynergy the member cannot afford in the no-HP colour
// and one restricted here in the low-HP colour, the page numbers, and the help
// line (how to set a shortcut while Select is held, else how to change member).
s32 PsynergyMenu_DrawActionPage(struct UiWindow *window, s32 unused,
                                const struct MenuResult *pageState) {
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  s32 cursor;
  struct BattleUnit *owner;
  struct BattleAct *ability;
  struct PsynergyMenuState *menu = gMenuWork;

  (void)unused;

  RenderOutput_RedrawSavedRectFar((void *)window);
  UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

  if (PSYNERGY_MENU_SHORTCUT_HELP & menu->flags) {
    UiText_DrawCharacterAtOffsetFar(MsgShortcutHelp, window, 0, 88);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgChangeCharacterHelp, window, HELP_TEXT_X,
                                    88);
  }

  firstEntry = pageState->page * LIST_PAGE_SIZE;
  visibleCount = (u8)(pageState->entryCount - firstEntry);
  if (visibleCount > LIST_PAGE_SIZE) {
    visibleCount = LIST_PAGE_SIZE;
  }

  Menu_SetPageIcons(LIST_PAGE_SIZE, firstEntry, window, PSYNERGY_PAGE_ICONS_X,
                    34);
  Menu_DrawPageIndicator(window, pageState->entryCount, LIST_PAGE_SIZE,
                         pageState->page, 15);
  UiText_DrawCharacterAtOffsetFar(MsgPsynergyPp, window, PSYNERGY_PP_LABEL_X,
                                  0);

  row = 0;
  if (visibleCount > row) {
    cursor = firstEntry;
    do {
      owner = UnitGetFar(menu->ownerIds[0]);
      ability = BattleActGet(OWNER_ACTION_ID_MASK & menu->psynergies[cursor]);

      if (ability->ppCost > owner->pp) {
        UiWork_SetParamNibbleFar(TEXT_COLOR_NO_HP);
      } else if (PsynergyMenu_IsActionRestricted(
                     OWNER_ACTION_ID_MASK & menu->psynergies[cursor]) != 0) {
        UiWork_SetParamNibbleFar(TEXT_COLOR_LOW_HP);
      } else {
        UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
      }

      UiText_DrawCharacterAtOffsetFar(
          (OWNER_ACTION_ID_MASK & menu->psynergies[cursor]) + MsgAbilityName,
          window, PSYNERGY_NAME_X, row * 16 + 8);
      UiText_DrawNumberAtOffsetFar(ability->ppCost, 2, window, 104,
                                   row * 16 + 8);
      UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);

      row++;
      cursor++;
    } while (visibleCount > row);
  }

  return 1;
}

// The Psynergy list of the member in paneIndex, five a page. The cursor's
// Psynergy is described in the info window and its icon animates; L and R
// switch to the previous or next member who has Psynergy to list. In the use
// mode (menu mode 0), A chooses a Psynergy the member can use and afford;
// holding Select shows the shortcut help, and L or R with Select set the
// Psynergy as that shortcut. In the shortcut modes (1 and 2) only field
// Psynergy are listed and A chooses one. Returns the chosen Psynergy, or -1
// when the player backs out.
s32 PsynergyMenu_RunList(s32 paneIndex) {
  struct PsynergyMenuState *menu;
  struct BattleAct *ability;
  struct RenderOutput *icon;
  struct UiWindow *window;
  s32 changed;
  s32 moved;
  s8 listMode;
  s32 member;
  u8 i;
  s32 result;
  s32 redraw;
  s32 done;
  struct BattleUnit *owner;
  s32 previous;
  s32 shortcutHelp;
  s32 work[5];
  struct MenuResult state;

  menu = gMenuWork;
  result = 0;
  previous = 0;
  shortcutHelp = 0;
  menu->paneIcon[paneIndex]->active = ICON_HIDDEN;
  UiWindow_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, 2);
  window = menu->listWindow;
  done = 0;

  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    owner = UnitGetFar(menu->ownerIds[paneIndex]);
    if (menu->mode != 0) {
      menu->psynergyCount =
          PsynergyMenu_CollectActions(owner, menu->psynergies, 1);
    } else {
      menu->psynergyCount =
          PsynergyMenu_CollectActions(owner, menu->psynergies, 2);
    }
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
    PsynergyMenu_BuildPageResult(&state, paneIndex);
    redraw = 1;
    changed = 1;
    menu->paneIcon[paneIndex]->active = ICON_SHOWN;

    while (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
      UiMenu_PositionCursor(PSYNERGY_LIST_CURSOR_X, state.row * 16 + 36);

      if (changed != 0) {
        changed = 0;
        if (menu->psynergies[previous] != 0) {
          icon = menu->entryIcons[previous];
          UiIcon_PrepareObject(icon);
        }
        if (redraw != 0) {
          redraw = 0;
          WaitFrames(1);
          PsynergyMenu_DrawActionPage(window, 0, &state);
        }
        PsynergyMenu_DrawDetailPage(window, work, &state);
        menu->paneAction[paneIndex] = menu->psynergies[state.selectedIndex];
        menu->cursorIcon->active = ICON_HIDDEN;
        if (menu->psynergies[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          // Animate the selected icon.
          icon->active = 9;
          icon->animStep = 0;
          icon->sentinel = 250;
        }
        for (i = 0; i < menu->ownerCount; i++) {
          AnimationObjects_SelectAnimationFar(menu->tabObjects[i], 1);
        }
      }

      WaitFrames(1);
      previous = state.selectedIndex;

      if ((gKeysHeld & KEY_SELECT) == 0) {
        moved = Menu_HandlePageInput(0, state.entryCount, LIST_PAGE_SIZE,
                                     &state.row, &state.page);
      } else {
        moved = -1;
      }
      if (moved == 1) {
        redraw = 1;
        changed = 1;
      }
      if (moved == 0) {
        changed = 1;
      }
      if (moved == -1) {
        changed = 0;
      }

      // Select shows the shortcut help while held, for field Psynergy.
      if (menu->mode == 0) {
        if ((gKeysPressed & KEY_SELECT) != 0 && shortcutHelp == 0) {
          ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                                 OWNER_ACTION_ID_MASK);
          if (ability->type0c == 0) {
            AudioCommand_PlayFar(SE_MENU_ERROR);
          } else {
            AudioCommand_PlayFar(SE_174); // the Use sound
            shortcutHelp = 1;
            menu->flags |= PSYNERGY_MENU_SHORTCUT_HELP;
            UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
            UiText_DrawCharacterAtOffsetFar(MsgShortcutHelp, window, 0, 88);
          }
        }
        if ((gKeysHeld & KEY_SELECT) == 0 && shortcutHelp == 1) {
          shortcutHelp = 0;
          menu->flags &= ~PSYNERGY_MENU_SHORTCUT_HELP;
          UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
          UiText_DrawCharacterAtOffsetFar(MsgChangeCharacterHelp, window,
                                          HELP_TEXT_X, 88);
        }
      }

      // A: set the shortcut (modes 1 and 2), or use the Psynergy if allowed
      // and affordable.
      if ((gKeysPressed & KEY_A) != 0) {
        if (menu->mode != 0) {
          AudioCommand_PlayFar(SE_130); // the shortcut sound
          result = menu->psynergies[state.selectedIndex];
          done = 1;
          break;
        }
        if (menu->psynergies[state.selectedIndex] != 0) {
          if (PsynergyMenu_IsActionRestricted(
                  menu->psynergies[state.selectedIndex]) != 0) {
            AudioCommand_PlayFar(SE_MENU_ERROR);
            continue;
          }
          ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                                 OWNER_ACTION_ID_MASK);
          if (ability->ppCost > owner->pp) {
            AudioCommand_PlayFar(SE_MENU_ERROR);
          } else {
            AudioCommand_PlayFar(SE_173); // the choice sound
            result = menu->psynergies[state.selectedIndex];
            done = 1;
            break;
          }
        }
      }

      if ((gKeysPressed & KEY_B) != 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        break;
      }

      // L and R: the previous or next member with Psynergy to list.
      if (((gKeysRepeat & KEY_R) != 0 || (gKeysRepeat & KEY_L) != 0) &&
          (gKeysHeld & KEY_SELECT) == 0) {
        listMode = menu->mode != 0 ? 1 : 2;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        menu->selectedIndexByOwner[menu->ownerIds[paneIndex]] =
            state.selectedIndex;
        member = menu->tabIndex[paneIndex];
        do {
          if ((gKeysRepeat & KEY_R) != 0) {
            member++;
          } else {
            member--;
          }
          member = (member + menu->ownerCount) % menu->ownerCount;
          menu->selectedOwner = menu->ownerTable[member];
          menu->ownerIds[0] = menu->ownerTable[member];
          menu->psynergyCount = PsynergyMenu_CollectActions(
              UnitGetFar(menu->ownerIds[0]), menu->psynergies, listMode);
        } while (menu->psynergyCount == 0);
        menu->tabIndex[paneIndex] = member;
        for (i = 0; i < 4; i++) {
          menu->rowPositions[i] = 30;
        }
        menu->rowPositions[member] = 26;
        Menu_DrawOwnerStatusPanel(menu->statusWindow, menu->ownerTable[member],
                                  0, STATUS_PANEL_EXPERIENCE);
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerTable[member]);
        break;
      }

      // Select + L or R: make the field Psynergy that shortcut.
      if ((gKeysPressed & KEY_L) != 0 && (gKeysHeld & KEY_SELECT) != 0) {
        ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                               OWNER_ACTION_ID_MASK);
        if (ability->type0c == 0) {
          AudioCommand_PlayFar(SE_MENU_ERROR);
        } else {
          AudioCommand_PlayFar(SE_130); // the shortcut sound
          if (PsynergyMenu_SetShortcut(menu->ownerIds[paneIndex],
                                       menu->psynergies[state.selectedIndex],
                                       0) != 0) {
            result = menu->psynergies[state.selectedIndex];
            menu->mode = 1;
            done = 1;
            break;
          }
        }
      }

      if ((gKeysPressed & KEY_R) != 0 && (gKeysHeld & KEY_SELECT) != 0) {
        ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                               OWNER_ACTION_ID_MASK);
        if (ability->type0c == 0) {
          AudioCommand_PlayFar(SE_MENU_ERROR);
        } else {
          if (PsynergyMenu_SetShortcut(menu->ownerIds[paneIndex],
                                       menu->psynergies[state.selectedIndex],
                                       1) != 0) {
            AudioCommand_PlayFar(SE_130); // the shortcut sound
            result = menu->psynergies[state.selectedIndex];
            menu->mode = 2;
            done = 1;
            break;
          }
        }
      }
    }
  }

  menu->flags &= ~PSYNERGY_MENU_SHORTCUT_HELP;
  UiIcon_PrepareObject(menu->entryGridCursor);
  menu->paneRow[paneIndex] = state.selectedIndex;
  menu->selectedIndexByOwner[menu->ownerIds[paneIndex]] = state.selectedIndex;
  menu->paneAction[paneIndex] = result;
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0) {
    result = -1;
  }
  WaitFrames(1);
  return result;
}

// Returns 1 when actionEntry cannot be used from the
// menu: it has no field effect and is not usable both in the field and in
// battle.
s32 PsynergyMenu_IsActionRestricted(s32 actionEntry) {
  // Mask the unsigned entry without overflowing a signed left shift.
  struct BattleAct *action = BattleActGet(((u32)actionEntry << 18) >> 18);

  if (action->type0c != 0 ||
      (action->flags & (ACTION_USABLE_IN_FIELD | ACTION_USABLE_IN_BATTLE)) ==
          (ACTION_USABLE_IN_FIELD | ACTION_USABLE_IN_BATTLE))
    return 0;
  return 1;
}

#undef PSYNERGY_PROMPT_TILES
#undef PSYNERGY_PROMPT_TILES_SIZE
#undef PSYNERGY_DETAIL_ROW_X
#undef PSYNERGY_DETAIL_ROW_WIDTH
#undef PSYNERGY_PAGE_ICONS_X
#undef PSYNERGY_PP_LABEL_X
#undef PSYNERGY_NAME_X
#undef PSYNERGY_LIST_CURSOR_X
#undef PSYNERGY_SHORTCUT_OWNER_SHIFT
#undef PSYNERGY_SHORTCUT_ID_MASK
#undef PSYNERGY_MENU_SHORTCUT_HELP

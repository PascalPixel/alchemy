/// @file
/// The Psynergy menu: the field menu's Psynergy screen, the party member and
/// Psynergy lists, targets, and the L and R shortcuts.
// Part of menu.c.

#include "menu_internal.h"
#include "flags.h"
#include "sounds.h"
#include "field/field_sequence.h"

/// The bits of a listed Psynergy the info window's description is found by:
/// nine, where the list's other reads take PSYNERGY_ABILITY_MASK's fourteen.
#define ABILITY_DESCRIPTION_MASK 0x1ff

s32 PsynergyMenu_IsActionRestricted(s32 actionEntry);

s32 PsynergyMenu_RunSteps(s32 *outMember, s32 *unused, s32 *outAction);

/// The field menu's Psynergy screen (FieldMenu_Run, FIELD_MENU_PSYNERGY), over
/// the field, which stops meanwhile. A Psynergy chosen to use on the field goes
/// to FieldState.psynergy with the party member who uses it. Returns
/// PsynergyMenu_RunSteps' result.
s32 PsynergyMenu_Run(void) {
  // German, Spanish, French and Italian also suspend the field tasks and keep
  // character block 1's tiles aside, filled with colour 3, while the menu is
  // open.
  struct ItemMenuState *menu;
#if TLA_PATCH_2
  void *savedTiles;
#endif
  s32 psynergy;
  s32 unused;
  s32 member;
  s32 result;

  menu = HeapSlot_Alloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
#if TLA_PATCH_2
  savedTiles = Heap_AllocEwram(MENU_SAVED_TILE_BYTES);
#endif
  Ram_HeapSlots->actorSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Menu_InitializeState(0);
#if TLA_PATCH_2
  Menu_SuspendFieldTasks();
  Iwram_CopyWords(savedTiles, (void *)BG_CHAR_BLOCK(1), MENU_SAVED_TILE_BYTES);
  Iwram_FillWords((void *)BG_CHAR_BLOCK(1), MENU_SAVED_TILE_BYTES,
                  PIXELS_4BPP(3));
  Text_UseExtraTilesFar(1);
#endif
  menu->partyCount = Party_ListMembersFar(menu->memberIds);
  ItemMenu_Initialize(0, 3, 0, 7);
  menu->questionWindow = Window_CreateFar(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
  Palette_LightenHighlight(HIGHLIGHT_PALETTE);
  PageArrows_DrawFar(PAGE_ARROW_VRAM);

  Menu_StartWatchingStart();
  result = PsynergyMenu_RunSteps(&member, &unused, &psynergy);
  Menu_StopWatchingStart();
  if (result == 1) {
    struct FieldState *target = Ram_HeapSlots->fieldState;
    u16 request;

    // DEAD: Ability_Get's result is not used; the ROM keeps the call.
    Ability_Get(PSYNERGY_ABILITY_MASK & menu->selectedItems[0]);
    request = psynergy | (member << ACT_UNIT_SHIFT);
    target->psynergy = request;
  }

  WindowSprite_ClearListFar(menu->statusWindow);
  ((struct WindowState *)Ram_HeapSlots->windowState)->holdTilemapUpload = 1;
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  HeapSlot_Release(HEAP_SLOT_MENU);
  Ram_HeapSlots->actorSystem->suspended = 0;
#if TLA_PATCH_2
  Window_MarkVisibleTextTilesFar();
  Text_UseExtraTilesFar(0);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), savedTiles, MENU_SAVED_TILE_BYTES);
  ((struct WindowState *)Ram_HeapSlots->windowState)->holdTilemapUpload = 0;
  Heap_Release(savedTiles);
  wait(1);
  Menu_ResumeFieldTasks();
#endif
  wait(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  ((struct WindowState *)Ram_HeapSlots->windowState)->holdTilemapUpload = 0;
  Party_ClearUnusableShortcutsFar();
  return result;
}

/// Returns whether the scene has a trigger for field Psynergy type on what it
/// would act on.
s32 Scene_HasPsynergyTriggerFar(s32 type);

/// PsynergyMenuState.flags: the Psynergy list shows the shortcut help (Select
/// held).
#define PSYNERGY_MENU_SHORTCUT_HELP 2

#define MENU_TARGET_WHOLE_PARTY 9 ///< memberIds[1] for the whole party

/// PsynergyMenuState.flags: set after a use or a cancelled target. The Item
/// screen's command grid reads the same bit; nothing in the Psynergy screen
/// does.
#define PSYNERGY_MENU_USED 1

/// The steps of the Psynergy menu (PsynergyMenu_RunSteps).
enum PsynergyMenuStep {
  PSYNERGY_STEP_MEMBER,
  PSYNERGY_STEP_LIST,
  /// how the Psynergy is used (PsynergyMenu_ClassifySelectedPsynergy)
  PSYNERGY_STEP_CLASSIFY,
  PSYNERGY_STEP_TARGET,
  PSYNERGY_STEP_APPLY,
};

/// The Psynergy shortcut a choice sets (PsynergyMenu_SetShortcut).
enum PsynergyShortcut {
  PSYNERGY_SHORTCUT_L,
  PSYNERGY_SHORTCUT_R,
};

/// What the Psynergy menu is open for (PsynergyMenuState.mode).
enum PsynergyMenuMode {
  PSYNERGY_MODE_USE,
  PSYNERGY_MODE_SET_L,
  PSYNERGY_MODE_SET_R,
};

s16 Unit_AddPpFar(s32 unit, s32 amount);

s32 PsynergyMenu_SetShortcut(s32 memberId, s32 id, s32 second);

s32 PsynergyMenu_ClassifySelectedPsynergy(void);
s32 PsynergyMenu_SelectPartySlot(s32 paneIndex);
s32 PsynergyMenu_SelectTarget(s32 browseOnly);
s32 PsynergyMenu_RunList(s32 paneIndex);

/// Runs the Psynergy menu's steps: choose a
/// party member and one of their Psynergy, then use it on a member (or the
/// whole party), or, in the shortcut modes, set it as the L or R shortcut.
/// Returns 1 with the member and Psynergy in the out parameters when the
/// chosen Psynergy works on the field, to be used once the menu closes; -1
/// when the player backs out or presses Start; otherwise the result of the
/// last use.
s32 PsynergyMenu_RunSteps(s32 *outMember, s32 *unused, s32 *outAction) {
  struct PsynergyMenuState *menu;
  s32 useResult;
  s32 step;
  s32 done;
  s32 chosenAction;
  s32 targetMode;
  s32 actionEntry;
  s32 chosenTarget;
  struct AbilityDefinition *action;

  step = PSYNERGY_STEP_MEMBER;
  done = 0;
  menu = Ram_HeapSlots->menuState;
  useResult = 0;

  while (done == 0 && GameFlag_Get(FLAG_MENU_START_PRESSED) == 0) {
    switch (step) {
    // Choose whose Psynergy to look at.
    case PSYNERGY_STEP_MEMBER: {
      s8 cancelled = -1;
      s32 chosen;

      menu->sideRow[0] = 0;
      ItemMenu_DrawMessage(0, (s32)MsgPsynergyChooseOwner);
      chosen = PsynergyMenu_SelectPartySlot(0);
      /* FAKEMATCH(dead): the test written as an XOR. */
      if (((u32)chosen ^ (u32)cancelled) == 0) {
        done = 1;
        useResult = cancelled;
      }
      WindowSprite_RedrawSavedRectFar(menu->infoWindow);
      step = PSYNERGY_STEP_LIST;
      break;
    }

    // Choose one of their Psynergy; in the shortcut modes, set it as the L or
    // R shortcut and start again.
    case PSYNERGY_STEP_LIST:
      wait(1);
      // DEAD: Unit_Get's result is not used; the ROM keeps the call.
      Unit_Get(menu->memberIds[0]);
      if (menu->psynergyCount == 0) {
        step = PSYNERGY_STEP_MEMBER;
      } else {
        switch (menu->mode) {
        case PSYNERGY_MODE_USE:
          ItemMenu_DrawMessage(0, (s32)MsgPsynergyChooseAbility);
          break;
        case PSYNERGY_MODE_SET_L:
          ItemMenu_DrawMessage(0, (s32)MsgPsynergyChooseForL);
          break;
        case PSYNERGY_MODE_SET_R:
          ItemMenu_DrawMessage(0, (s32)MsgPsynergyChooseForR);
          break;
        }
        Menu_HideEntryIcons();
        StatusPanel_Draw(menu->statusWindow, menu->memberIds[0], 0,
                         STATUS_PANEL_EXPERIENCE);
        chosenAction = PsynergyMenu_RunList(0);
        step = PSYNERGY_STEP_MEMBER;
        if (chosenAction != -1) {
          step = PSYNERGY_STEP_CLASSIFY;
          if (menu->mode != PSYNERGY_MODE_USE) {
            if (menu->mode == PSYNERGY_MODE_SET_L) {
              PsynergyMenu_SetShortcut(menu->memberIds[0], chosenAction,
                                       PSYNERGY_SHORTCUT_L);
              WindowSprite_ClearListFar(menu->infoWindow);
              ItemMenu_ShowMessage((s32)MsgShortcutSetL, -1, -1);
              WindowSprite_RedrawSavedRectFar(menu->infoWindow);
            } else {
              PsynergyMenu_SetShortcut(menu->memberIds[0], chosenAction,
                                       PSYNERGY_SHORTCUT_R);
              WindowSprite_ClearListFar(menu->infoWindow);
              ItemMenu_ShowMessage((s32)MsgShortcutSetR, -1, -1);
              WindowSprite_RedrawSavedRectFar(menu->infoWindow);
            }
            step = PSYNERGY_STEP_MEMBER;
          }
        }
      }
      break;

    // Choose the member to use it on.
    case PSYNERGY_STEP_TARGET:
      ItemMenu_DrawMessage(0, (s32)MsgPsynergyChooseTarget);
      chosenTarget = PsynergyMenu_SelectTarget(0);
      step = PSYNERGY_STEP_APPLY;
      if (chosenTarget == -1) {
        menu->flags |= PSYNERGY_MENU_USED;
        step = PSYNERGY_STEP_LIST;
      }
      break;

    // How is it used? On one member, on the whole party, or on the field
    // once the menu closes.
    case PSYNERGY_STEP_CLASSIFY:
      targetMode = PsynergyMenu_ClassifySelectedPsynergy();
      if (targetMode == ITEM_USE_MODE_CHOOSE_MEMBER) {
        step = PSYNERGY_STEP_TARGET;
        break;
      }
      if (targetMode == ITEM_USE_MODE_WHOLE_PARTY) {
        menu->memberIds[1] = MENU_TARGET_WHOLE_PARTY;
        step = PSYNERGY_STEP_APPLY;
        break;
      }
      done = 1;
      useResult = 1;
      *outMember = menu->memberIds[0];
      *outAction = menu->sideAction[0] & PSYNERGY_ABILITY_MASK;
      break;

    // Use it: pay its PP, show the result and go back to the list, or to the
    // target when it failed on one member.
    case PSYNERGY_STEP_APPLY:
      chosenTarget = 0;
      actionEntry = menu->sideAction[0];
      useResult = Ability_ApplyToTargets(actionEntry, menu->memberIds[0],
                                         menu->memberIds[1], 0);
      if (menu->memberIds[1] == MENU_TARGET_WHOLE_PARTY) {
        menu->memberIds[1] = menu->memberIds[0];
        chosenTarget = MENU_TARGET_WHOLE_PARTY;
      }
      if (useResult != -1) {
        action = Ability_Get(menu->sideAction[0] & PSYNERGY_ABILITY_MASK);
        Unit_AddPpFar(menu->memberIds[0], -action->ppCost);
      }
      Unit_CalculateFar(menu->memberIds[0]);
      if (useResult != -1) {
        StatusPanel_Draw(menu->statusWindow, menu->memberIds[1], 0,
                         STATUS_PANEL_EXPERIENCE);
        Ability_PlayUseSound(menu->sideAction[0] & PSYNERGY_ABILITY_MASK);
        WindowSprite_ClearListFar(menu->infoWindow);
        ItemMenu_ShowMessage(menu->useResult + (s32)MsgItemUseResult, 0, -1);
      } else {
        Sound_PlayFar(SE_MENU_ERROR);
        WindowSprite_ClearListFar(menu->infoWindow);
        ItemMenu_ShowMessage(menu->useResult + (s32)MsgItemUseResult, useResult,
                             useResult);
      }
      if (useResult == -1) {
        menu->skipSlide = 1;
        if (chosenTarget == MENU_TARGET_WHOLE_PARTY) {
          menu->flags |= PSYNERGY_MENU_USED;
          step = PSYNERGY_STEP_LIST;
        } else {
          step = PSYNERGY_STEP_TARGET;
        }
      } else {
        useResult = 1;
        menu->flags |= PSYNERGY_MENU_USED;
        step = PSYNERGY_STEP_LIST;
      }
      break;

    default:
      done = 1;
      break;
    }
  }

  if (GameFlag_Get(FLAG_MENU_START_PRESSED) != 0)
    useResult = -1;
  return useResult;
}

/// Says how the selected Psynergy is used from the menu (enum ItemUseMode): on
/// the field once the menu closes when the scene has an event for its field
/// Psynergy on what the leader faces, on the whole party for
/// ABILITY_RANGE_ALL, on one chosen party member when it targets
/// ABILITY_TARGET_ALLIES, and otherwise on the field.
s32 PsynergyMenu_ClassifySelectedPsynergy(void) {
  struct AbilityDefinition *action;
  s32 targetModeDifference;
  s32 classification;

  action = Ability_Get(
      PSYNERGY_ABILITY_MASK &
      ((struct PsynergyMenuState *)Ram_HeapSlots->menuState)->sideAction[0]);
  if (Scene_HasPsynergyTriggerFar(action->fieldPsynergy) != 0)
    return ITEM_USE_MODE_FROM_FIELD;
  classification = ITEM_USE_MODE_WHOLE_PARTY;
  if (action->range != ABILITY_RANGE_ALL) {
    u8 kind = action->targetMode;

    // ITEM_USE_MODE_CHOOSE_MEMBER when it targets ABILITY_TARGET_ALLIES,
    // ITEM_USE_MODE_FROM_FIELD otherwise.
    targetModeDifference = kind ^ ABILITY_TARGET_ALLIES;
    classification = (0 - targetModeDifference) | targetModeDifference;
    classification = (u32)classification >> 31;
    classification = 1 - classification;
  }
  return classification;
}

s32 PsynergyMenu_CollectActions(struct Unit *unit, u16 *actions, s32 mode);
s32 PsynergyMenu_SelectMember(u16 *ownerIds, u16 *unused);

/// Shows side partySlot's icon, collects the actions of the member chosen
/// there last (the first when none was) and runs PsynergyMenu_SelectMember on
/// them. Returns its result.
s32 PsynergyMenu_SelectPartySlot(s32 partySlot) {
  struct PsynergyMenuState *menu = Ram_HeapSlots->menuState;
  s32 memberIndex = menu->tabIndex[partySlot];
  struct WindowSprite *icon;
  struct Unit *unit;
  u16 *actions;
  s32 result;

  result = 0;
  icon = menu->sideIcon[partySlot];
  icon->motion = ICON_SHOWN;
  icon->animationStep = result;
  menu->cursorIcon->motion = ICON_HIDDEN;
  menu->tabCounts[partySlot] = menu->memberCount;

  if (memberIndex == -1) {
    menu->tabIndex[partySlot] = 0;
    memberIndex = 0;
  } else {
    MenuCursor_Slide(memberIndex * 24 - 10, 16);
  }

  unit = Unit_Get(menu->memberTable[memberIndex]);
  actions = menu->psynergies;
  menu->psynergyCount =
      PsynergyMenu_CollectActions(unit, actions, PSYNERGY_LIST_FIELD);
  result = PsynergyMenu_SelectMember(menu->memberTable, actions);
  icon = menu->sideIcon[partySlot];
  Icon_Show(icon);
  wait(1);
  return result;
}

void PsynergyMenu_RefreshMemberPsynergy(s32 ownerId);

s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner);

/// The party member row of the Psynergy menu: moving shows each member's
/// Psynergy, status and shortcuts; A chooses a member who has Psynergy, L or R
/// one to set that shortcut from, and B backs out. Returns the chosen member's
/// id, or -1.
s32 PsynergyMenu_SelectMember(u16 *ownerIds, u16 *unused) {
  struct PsynergyMenuState *menu;
  s32 selection;
  s32 count;
  s32 redraw;
  s32 result;
  void *unit;
  s32 window;
  s32 shown;
  u16 *actions;
  s8 fieldCount;
  s32 shortcutCount;

  menu = Ram_HeapSlots->menuState;
  selection = menu->tabIndex[0];
  count = menu->tabCounts[0];
  redraw = 2;
  result = 0;
  shown = 0;
  menu->mode = result;
  unit = Unit_Get(ownerIds[selection]);
  if (Window_UpdateOrCreate(&menu->psynergyWindow, 13, 3, 17, 10,
                            WINDOW_FLAG_INSTANT))
    Menu_SpawnIconEntries(menu, menu->psynergyWindow);
  if (Window_UpdateOrCreate(&menu->shortcutWindow, 13, 13, 17, 4,
                            WINDOW_FLAG_INSTANT)) {
    menu->cursorIcon = WindowSprite_CreateFromResourceFar(
        ICON_KIND_ITEM, 0, menu->shortcutWindow, 0, result);
    menu->cursorIcon->motion = ICON_HIDDEN;
  }
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      window = menu->statusWindow;
      unit = Unit_Get(ownerIds[selection]);
      PsynergyMenu_RefreshMemberPsynergy(ownerIds[selection]);
      StatusPanel_Draw(window, ownerIds[selection], 0, STATUS_PANEL_EXPERIENCE);
      PsynergyMenu_DrawShortcuts(menu->shortcutWindow, ownerIds[selection]);
      Menu_ReloadMemberPortrait(menu, ownerIds[selection]);
      if (redraw == 2) {
        Menu_CreatePartyFigures(selection / 4 * 4);
        wait(1);
      }
      Menu_MarkChosenMember(menu->auxiliaryWindow, selection, count);
      if (!GameFlag_Get(FLAG_INFO_WINDOW_HOLDS_MESSAGE) && !shown) {
        WindowSprite_ClearListFar(menu->infoWindow);
#if TLA_PATCH_1
        // Japanese does not redraw the info window here.
        WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#endif
        Menu_DrawCoins(menu->infoWindow);
        shown = 1;
      } else {
        GameFlag_Clear(FLAG_INFO_WINDOW_HOLDS_MESSAGE);
      }
    }
    MenuCursor_Position((selection % 4) * 24 - 10, 16);
    wait(1);
    if (gInput.pressed & KEY_A) {
      if (menu->psynergyCount) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        result = ownerIds[selection];
        break;
      }
      Sound_PlayFar(SE_MENU_ERROR);
    }
    if ((gInput.pressed & KEY_L) || (gInput.pressed & KEY_R)) {
      result = ownerIds[selection];
      if (gInput.pressed & KEY_L)
        menu->mode = PSYNERGY_MODE_SET_L;
      else
        menu->mode = PSYNERGY_MODE_SET_R;
      actions = Heap_Alloc(32 * sizeof(u16));
      fieldCount =
          PsynergyMenu_CollectActions(unit, actions, PSYNERGY_LIST_SHORTCUTS);
      Heap_Release(actions);
      shortcutCount = fieldCount;
      if (shortcutCount == 0) {
        menu->mode = shortcutCount;
        Sound_PlayFar(SE_MENU_ERROR);
      } else {
        Sound_PlayFar(SE_MENU_CONFIRM);
        break;
      }
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    redraw = Menu_MoveGridCursor(&selection, count, 4);
  }
  menu->tabIndex[0] = selection;
  menu->selectedMember = ownerIds[selection];
  menu->memberIds[0] = ownerIds[selection];
  menu->pageArrows[0]->motion = ICON_HIDDEN;
  menu->pageArrows[1]->motion = ICON_HIDDEN;
  return result;
}

void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);

/// Collects the owner's Psynergy and redraws the list, saying when it is
/// empty.
void PsynergyMenu_RefreshMemberPsynergy(s32 ownerId) {
  u16 *psynergies;
  struct PsynergyMenuState *menu;
  void *owner;

  menu = Ram_HeapSlots->menuState;
  owner = Unit_Get(ownerId);
  psynergies = menu->psynergies;
  menu->psynergyCount =
      PsynergyMenu_CollectActions(owner, psynergies, PSYNERGY_LIST_FIELD);
  WindowSprite_RedrawSavedRectFar(menu->psynergyWindow);
  PsynergyMenu_PlaceEntryIcons(108, 32, 8);
  PsynergyMenu_DrawPsynergyIcons(psynergies);
  if (menu->psynergyCount == 0) {
    Text_DrawMessageAsTilesFar((s32)MsgPsynergyMenuEmpty, menu->psynergyWindow,
                               0, 24);
  }
}

/// Chooses the party member to use the selected Psynergy on, showing each one's
/// status and the Psynergy's description unless browseOnly is set.
s32 PsynergyMenu_SelectTarget(s32 browseOnly) {
  struct PsynergyMenuState *menu;
  s32 selection;
  s32 count;
  s32 redraw;
  s32 result;
  s32 shown;
  struct WindowSprite *icon;
  s32 column;

  menu = Ram_HeapSlots->menuState;
  selection = menu->tabIndex[1];
  count = menu->memberCount;
  result = 0;
  shown = 0;
  // DEAD: Unit_Get's result is not used; the ROM keeps the call.
  Unit_Get(menu->memberTable[menu->tabIndex[0]]);
  redraw = 2;
  MenuCursor_Slide(selection * 24 - 10, 16);
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      // DEAD: Unit_Get's result is not used; the ROM keeps the call.
      Unit_Get(menu->memberTable[selection]);
      icon = menu->sideIcon[1];
      column = (selection % 4) * 3;
      icon->sprite.oam.x = icon->x =
          (menu->auxiliaryWindow->x + column) * 8 - 2;
      if (browseOnly == 0) {
        StatusPanel_Draw(menu->statusWindow, menu->memberTable[selection], 0,
                         STATUS_PANEL_EXPERIENCE);
        Menu_ReloadMemberPortrait(menu, menu->memberTable[selection]);
        if (redraw == 2) {
          Menu_CreatePartyFigures(selection / 4 * 4);
          wait(1);
        }
        Menu_MarkChosenMember(menu->auxiliaryWindow, selection, count);
        if (!GameFlag_Get(FLAG_INFO_WINDOW_HOLDS_MESSAGE) && !shown) {
          // The Japanese release clears the info window; from patch 1 it
          // redraws the window's saved rectangle.
#if TLA_PATCH_1
          WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#else
          WindowSprite_ClearListFar(menu->infoWindow);
#endif
          // Japanese draws the description as a resource string.
#if LANGUAGE == LANGUAGE_JAPANESE
          Text_DrawMessageFar((menu->sideAction[0] & PSYNERGY_ABILITY_MASK) +
                                  MsgAbilityDescription,
                              menu->infoWindow, 0, 0);
#else
          Text_DrawMessageAsTilesFar(
              (menu->sideAction[0] & PSYNERGY_ABILITY_MASK) +
                  MsgAbilityDescription,
              menu->infoWindow, 0, 0);
#endif
          shown = 1;
        } else {
          GameFlag_Clear(FLAG_INFO_WINDOW_HOLDS_MESSAGE);
        }
      }
    }
    MenuCursor_Position((selection % 4) * 24 - 10, 16);
    wait(1);
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = menu->memberTable[selection];
      break;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    redraw = Menu_MoveGridCursor(&selection, count, 4);
  }
  icon = menu->sideIcon[1];
  Icon_Show(icon);
  icon->motion = ICON_HIDDEN;
  wait(1);
  menu->tabIndex[1] = selection;
  menu->selectedMember = menu->memberTable[selection];
  menu->memberIds[1] = menu->memberTable[selection];
  selection = menu->tabIndex[0];
  count = menu->tabCounts[0];
  Menu_CreatePartyFigures(selection / 4 * 4);
  Menu_MarkChosenMember(menu->auxiliaryWindow, selection, count);
  menu->pageArrows[0]->motion = ICON_HIDDEN;
  menu->pageArrows[1]->motion = ICON_HIDDEN;
  return result;
}

/// Sets the L Psynergy shortcut, or the R one when second is set, to the
/// Psynergy id used by party member memberId. Returns 1.
s32 PsynergyMenu_SetShortcut(s32 memberId, s32 id, s32 second) {
  u32 packed = (memberId << ACT_UNIT_SHIFT) | (id & PSYNERGY_ABILITY_MASK);

  if (second == 0)
    gMainRoutineState.firstShortcut = packed;
  else {
    struct MainRoutineState *party = &gMainRoutineState;

    party->secondShortcut = packed;
  }
  return 1;
}

#if LANGUAGE == LANGUAGE_JAPANESE
/// Japanese drops the colon after "L" and "R" and draws the ability's name 8
/// pixels further left when that name is wider than this.
#define COLON_NAME_WIDTH_LIMIT 11
#elif TLA_PATCH_2
/// German, Spanish, French and Italian leave out the owner's name when the
/// ability's name is wider than this.
#define OWNER_NAME_WIDTH_LIMIT 10
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
/// `L`, before a wide shortcut name.
extern const u8 gMenu_ShortcutLString[];
/// `L:`, before the L shortcut's name.
extern const u8 gMenu_ShortcutLColonString[];
/// `R`, before a wide shortcut name.
extern const u8 gMenu_ShortcutRString[];
/// `R:`, before the R shortcut's name.
extern const u8 gMenu_ShortcutRColonString[];
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
/// Draws the L and R shortcuts into window: a heading, then for each its
/// Psynergy and the party member who uses it, or how to set it. German,
/// Spanish, French and Italian leave out the party member's name beside a wide
/// Psynergy name. Returns 1; owner is not used.
s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  u8 *name;
#if TLA_PATCH_2
  s32 wide;
#endif

  if (gMainRoutineState.firstShortcut != 0 &&
      gMainRoutineState.secondShortcut != 0)
    // "L R:Change shortcut"
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 1, window, 0, -8);
  else
    // "Shortcut"
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetL - 2, window, 0, -8);

  name = &MsgAbilityName[gMainRoutineState.firstShortcut & ACT_ID_MASK];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
#if TLA_PATCH_2
  if (width < 0 || width > OWNER_NAME_WIDTH_LIMIT)
    wide = 1;
  else
    wide = 0;
#endif
  if (gMainRoutineState.firstShortcut != 0) {
    MessageArgument_PushFar(gMainRoutineState.firstShortcut & ACT_ID_MASK,
                            MESSAGE_ARGUMENT_ABILITY);
    // "L:" and the Psynergy's name
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 4, window, 0, 0);
#if TLA_PATCH_2
    if (!wide)
#endif
      Text_DrawStringAtOffsetFar(
          (s32)Unit_Get(gMainRoutineState.firstShortcut >> ACT_UNIT_SHIFT),
          window, 80, 0);
  } else {
    // "L:Set to L"
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 2, window, 0, 0);
  }

  name = &MsgAbilityName[gMainRoutineState.secondShortcut & ACT_ID_MASK];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
#if TLA_PATCH_2
  if (width < 0 || width > OWNER_NAME_WIDTH_LIMIT)
    wide = 1;
  else
    wide = 0;
#endif
  if (gMainRoutineState.secondShortcut != 0) {
    MessageArgument_PushFar(gMainRoutineState.secondShortcut & ACT_ID_MASK,
                            MESSAGE_ARGUMENT_ABILITY);
    // "R:" and the Psynergy's name
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 5, window, 0, 8);
#if TLA_PATCH_2
    if (!wide)
#endif
      Text_DrawStringAtOffsetFar(
          (s32)Unit_Get(gMainRoutineState.secondShortcut >> ACT_UNIT_SHIFT),
          window, 80, 8);
  } else {
    // "R:Set to R"
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 3, window, 0, 8);
  }
  return 1;
}
#else
/// Japanese: draws "L" or "L:" and "R" or "R:" itself before each shortcut's
/// name, dropping the colon for a wide name. Returns 1.
s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  u8 *name;

  if (gMainRoutineState.firstShortcut != 0 &&
      gMainRoutineState.secondShortcut != 0)
    // "L R:Change shortcut"
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 1, window, 0, -8);
  else
    // "Shortcut"
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetL - 2, window, 0, -8);

  name = &MsgAbilityName[gMainRoutineState.firstShortcut & ACT_ID_MASK];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
  if (gMainRoutineState.firstShortcut != 0) {
    if (width < 0 || width > COLON_NAME_WIDTH_LIMIT) {
      Text_DrawStringInWindowFar(gMenu_ShortcutLString, window, 0, 0);
      Text_DrawMessageAsTilesFar((s32)name, window, 8, 0);
    } else {
      Text_DrawStringInWindowFar(gMenu_ShortcutLColonString, window, 0, 0);
      Text_DrawMessageAsTilesFar((s32)name, window, 16, 0);
    }
    Text_DrawStringAtOffsetFar(
        (s32)Unit_Get(gMainRoutineState.firstShortcut >> ACT_UNIT_SHIFT),
        window, 80, 0);
  } else {
    Text_DrawStringInWindowFar(gMenu_ShortcutLColonString, window, 0, 0);
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 2, window, 24, 0);
  }

  name = &MsgAbilityName[gMainRoutineState.secondShortcut & ACT_ID_MASK];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
  if (gMainRoutineState.secondShortcut != 0) {
    if (width < 0 || width > COLON_NAME_WIDTH_LIMIT) {
      Text_DrawStringInWindowFar(gMenu_ShortcutRString, window, 0, 8);
      Text_DrawMessageAsTilesFar((s32)name, window, 8, 8);
    } else {
      Text_DrawStringInWindowFar(gMenu_ShortcutRColonString, window, 0, 8);
      Text_DrawMessageAsTilesFar((s32)name, window, 16, 8);
    }
    Text_DrawStringAtOffsetFar(
        (s32)Unit_Get(gMainRoutineState.secondShortcut >> ACT_UNIT_SHIFT),
        window, 80, 8);
  } else {
    Text_DrawStringInWindowFar(gMenu_ShortcutRColonString, window, 0, 8);
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 3, window, 24, 8);
  }
  return 1;
}
#endif

#undef OWNER_NAME_WIDTH_LIMIT

/// Loads each listed Psynergy's icon into its icon sprite (32 entries) and
/// hides the icons of empty entries.
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies) {
  s32 i;
  struct WindowSprite **icons;
  u16 *p;
  s32 psynergyId;

  icons = ((struct PsynergyMenuState *)Ram_HeapSlots->menuState)->entryIcons;
  p = psynergies;
  for (i = 0; i < PSYNERGY_SLOTS; i++) {
    psynergyId = *p++;
    if (psynergyId != 0) {
      SelectionIcon_LoadIntoSlotFar(ICON_KIND_ABILITY, psynergyId,
                                    (u8)(*icons)->index, 0);
    }
    icons++;
  }
  Menu_HideEmptyEntryIcons(psynergies);
}

/// Lists a unit's Psynergy into actions (cleared first) by mode (enum
/// PsynergyListMode) and returns how many.
s32 PsynergyMenu_CollectActions(struct Unit *unit, u16 *actions, s32 mode) {
  s32 i;
  s32 count;
  struct AbilityDefinition *ability;
  s32 offset;

  count = 0;
  for (i = 0; i < PSYNERGY_SLOTS; i++)
    actions[i] = 0;

  if (mode == PSYNERGY_LIST_SHORTCUTS) {
    for (i = 0; i < PSYNERGY_SLOTS; i++) {
      if (unit->psynergy[i].encodedAction != 0) {
        if (Ability_Get(unit->psynergy[i].encodedAction & PSYNERGY_ABILITY_MASK)
                ->fieldPsynergy != 0) {
          actions[count] = unit->psynergy[i].encodedAction;
          count++;
        }
      }
    }
  } else {
    for (i = 0; i < PSYNERGY_SLOTS; i++) {
      if (unit->psynergy[i].encodedAction != 0) {
        ability = Ability_Get(unit->psynergy[i].encodedAction &
                              PSYNERGY_ABILITY_MASK);
        if (ability->fieldPsynergy != 0 ||
            (ability->flags & ABILITY_USABLE_IN_FIELD) != 0) {
          actions[count] = unit->psynergy[i].encodedAction;
          count++;
        }
      }
    }
    if (mode != PSYNERGY_LIST_FIELD) {
      for (i = 0; i < PSYNERGY_SLOTS; i++) {
        if (unit->psynergy[i].encodedAction != 0) {
          ability = Ability_Get(unit->psynergy[i].encodedAction &
                                PSYNERGY_ABILITY_MASK);
          if (ability->fieldPsynergy == 0 &&
              (ability->flags & ABILITY_USABLE_IN_FIELD) == 0) {
            actions[count] = unit->psynergy[i].encodedAction;
            count++;
          }
        }
      }
    }
  }
  return count;
}

/// Sets list up over the Psynergy listed for the party member in side index,
/// five a page, its cursor on the one chosen last there (or the last one).
s32 PsynergyMenu_BuildPagedList(struct PagedList *result, s32 index) {
  s32 ownerState;
  struct PsynergyMenuState *menu = Ram_HeapSlots->menuState;
  s32 entryCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selectedIndex;
  s32 owner;

  ownerState = Unit_Get(menu->memberIds[index]);
  entryCount = menu->psynergyCount;
  owner = menu->memberIds[index];
  selectedIndex = menu->selectedIndexByMember[owner];
  if ((selectedIndex + 1) > entryCount) {
    selectedIndex = entryCount - 1;
  }
  page = selectedIndex / LIST_PAGE_SIZE;
  row = selectedIndex % LIST_PAGE_SIZE;
  pageCount = entryCount / LIST_PAGE_SIZE;
  if (entryCount % LIST_PAGE_SIZE != 0) {
    pageCount++;
  }
  result->memberState = ownerState;
  result->page = page;
  result->pageCount = pageCount;
  result->row = row;
  result->entryCount = entryCount;
  result->selectedIndex = selectedIndex;
  return 1;
}

/// Updates the Psynergy list for the row under the cursor: puts the Psynergy's
/// description in the info window, unless a kept message is there, and
/// highlights its row of the five. window and scratch are not used.
s32 PsynergyMenu_ShowSelectedRow(s32 window, s32 *scratch,
                                 struct PagedList *state) {
  struct PsynergyMenuState *menu;
  s32 combined;
  s32 row;

  menu = Ram_HeapSlots->menuState;
  combined = state->page * LIST_PAGE_SIZE;
  combined += state->row;
  state->selectedIndex = combined;

  if (GameFlag_Get(FLAG_INFO_WINDOW_HOLDS_MESSAGE) == 0) {
    // As in the item list: the Japanese release clears the info window, and
    // from patch 1 it redraws the window's saved rectangle.
#if TLA_PATCH_1
    WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#else
    WindowSprite_ClearListFar(menu->infoWindow);
#endif
    wait(1);

    combined = state->selectedIndex;
    if (menu->psynergies[combined] != 0) {
      s32 masked = (s32)(MsgAbilityDescription + (menu->psynergies[combined] &
                                                  ABILITY_DESCRIPTION_MASK));

      // Japanese draws the description as a resource string.
#if LANGUAGE == LANGUAGE_JAPANESE
      Text_DrawMessageFar(masked, menu->infoWindow, 0, 0);
#else
      Text_DrawMessageAsTilesFar(masked, menu->infoWindow, 0, 0);
#endif
      // From patch 2 the menu waits a frame after drawing it.
#if TLA_PATCH_2
      wait(1);
#endif
    }
  } else {
    // BUG: clears FLAG_SCENE_TEMP_FF instead of FLAG_INFO_WINDOW_HOLDS_MESSAGE,
    // so a kept message keeps the descriptions away until another screen
    // clears it.
#ifdef BUGFIX
    GameFlag_Clear(FLAG_INFO_WINDOW_HOLDS_MESSAGE);
#else
    GameFlag_Clear(FLAG_SCENE_TEMP_FF);
#endif
  }

  for (row = 0; row < LIST_PAGE_SIZE; row++) {
    if (row == state->row) {
      Window_SetHighlightRect(menu->psynergyWindow, 1, row * 2 + 1, 14, 1,
                              HIGHLIGHT_PALETTE);
    } else {
      Window_SetHighlightRect(menu->psynergyWindow, 1, row * 2 + 1, 14, 1,
                              WINDOW_PALETTE);
    }
  }

  wait(1);
  return 1;
}

/// The Japanese page draws its page icons, PP heading and names at other
/// columns.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PSYNERGY_PAGE_ICONS_X 120
#define PSYNERGY_PP_HEADING_X 72
#define PSYNERGY_NAME_X 32
#else
#define PSYNERGY_PAGE_ICONS_X 112
#define PSYNERGY_PP_HEADING_X 96
#define PSYNERGY_NAME_X 16
#endif

/// Draws one page of the Psynergy list: each name and PP cost, red when the PP
/// is short and yellow when it cannot be used from the menu.
s32 PsynergyMenu_DrawPage(s32 window, s32 unused,
                          const struct PagedList *state) {
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  s32 cursor;
  struct Unit *owner;
  struct AbilityDefinition *ability;
  struct PsynergyMenuState *menu = Ram_HeapSlots->menuState;

  (void)unused;

  WindowSprite_RedrawSavedRectFar(window);
  Window_DrawDividerLineFar(window, 0, 11, 16, 11);

  if (PSYNERGY_MENU_SHORTCUT_HELP & menu->flags) {
    // "L R:Set shortcut"
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetL - 1, window, 0, 88);
  } else {
    Text_DrawMessageAsTilesFar((s32)MsgChangeCharacterHelp, window,
                               CHANGE_HELP_X, 88);
  }

  firstEntry = state->page * LIST_PAGE_SIZE;
  visibleCount = (u8)(state->entryCount - firstEntry);
  if (visibleCount > LIST_PAGE_SIZE) {
    visibleCount = LIST_PAGE_SIZE;
  }

  Menu_SetPageIcons(LIST_PAGE_SIZE, firstEntry, window, PSYNERGY_PAGE_ICONS_X,
                    34);
  PagedList_DrawPageNumbers(window, state->entryCount, LIST_PAGE_SIZE,
                            state->page, PAGE_NUMBERS_RIGHT);
  // "PP"
  Text_DrawMessageAsTilesFar((s32)MsgPsynergyChooseTarget + 2, window,
                             PSYNERGY_PP_HEADING_X, 0);

  row = 0;
  if (visibleCount > row) {
    cursor = firstEntry;
    do {
      owner = Unit_Get(menu->memberIds[0]);
      ability = Ability_Get(PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]);

      if (ability->ppCost > owner->pp) {
        Text_SetColorFar(TEXT_COLOR_RED);
      } else if (PsynergyMenu_IsActionRestricted(
                     PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]) != 0) {
        Text_SetColorFar(TEXT_COLOR_YELLOW);
      } else {
        Text_SetColorFar(TEXT_COLOR_NORMAL);
      }

      Text_DrawMessageAsTilesFar(
          (PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]) +
              (s32)MsgAbilityName,
          window, PSYNERGY_NAME_X, row * 16 + 8);
      Text_DrawNumberAtOffsetFar(ability->ppCost, 2, window, 104, row * 16 + 8);
      Text_SetColorFar(TEXT_COLOR_NORMAL);

      row++;
      cursor++;
    } while (visibleCount > row);
  }

  return 1;
}

#undef PSYNERGY_PAGE_ICONS_X
#undef PSYNERGY_PP_HEADING_X
#undef PSYNERGY_NAME_X

/// The Japanese list puts its cursor further right.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PSYNERGY_LIST_CURSOR_X 98
#else
#define PSYNERGY_LIST_CURSOR_X 88
#endif

s32 PsynergyMenu_IsActionRestricted(s32 actionEntry);

/// The Psynergy list of the party member in side paneIndex, five a page. Up and
/// Down move the cursor, Left and Right turn pages, L and R change party
/// member, and Select held shows how to set a shortcut, L or R then setting
/// it. Returns the chosen Psynergy entry, or -1 when the player backs out.
s32 PsynergyMenu_RunList(s32 paneIndex) {
  struct PsynergyMenuState *menu;
  struct AbilityDefinition *ability;
  struct WindowSprite *icon;
  s32 window;
  s32 changed;
  s32 moved;
  s32 listMode;
  s32 member;
  s32 ownerCount;
  u8 i;
  struct Unit *owner;
  s32 result;
  s32 redraw;
  s32 done;
  s32 previous;
  s32 shortcutHelp;
  s32 scratch[5];
  struct PagedList state;

  menu = Ram_HeapSlots->menuState;
  result = 0;
  previous = 0;
  shortcutHelp = 0;
  menu->sideIcon[paneIndex]->motion = ICON_HIDDEN;
  Window_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, WINDOW_FLAG_INSTANT);
  window = menu->listWindow;
  done = 0;

  while (done == 0 && !GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    owner = Unit_Get(menu->memberIds[paneIndex]);
    if (menu->mode != PSYNERGY_MODE_USE)
      menu->psynergyCount = PsynergyMenu_CollectActions(
          owner, menu->psynergies, PSYNERGY_LIST_SHORTCUTS);
    else
      menu->psynergyCount = PsynergyMenu_CollectActions(owner, menu->psynergies,
                                                        PSYNERGY_LIST_FIELD);
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
    PsynergyMenu_BuildPagedList(&state, paneIndex);
    changed = 1;
    redraw = 1;
    menu->sideIcon[paneIndex]->motion = ICON_SHOWN;

    while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
      MenuCursor_Position(PSYNERGY_LIST_CURSOR_X, state.row * 16 + 36);

      if (changed != 0) {
        changed = 0;
        if (menu->psynergies[previous] != 0) {
          icon = menu->entryIcons[previous];
          Icon_Show(icon);
        }
        if (redraw != 0) {
          redraw = 0;
          wait(1);
          PsynergyMenu_DrawPage(window, 0, &state);
        }
        PsynergyMenu_ShowSelectedRow(window, scratch, &state);
        menu->sideAction[paneIndex] = menu->psynergies[state.selectedIndex];
        menu->cursorIcon->motion = ICON_HIDDEN;
        if (menu->psynergies[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          icon->motion = WINDOW_SPRITE_MOTION_SWELL;
          icon->animationStep = 0;
          icon->depth = OAM_DEPTH_FRONT - 5;
        }
        for (i = 0; i < menu->memberCount; i++)
          Sprite_StartAnimationFar(menu->tabSprites[i], ANIM_STAND);
      }

      wait(1);
      previous = state.selectedIndex;

      if ((gInput.held & KEY_SELECT) == 0)
        moved = PagedList_HandleInput(0, state.entryCount, LIST_PAGE_SIZE,
                                      &state.row, &state.page);
      else
        moved = -1;
      if (moved == 1) {
        redraw = 1;
        changed = 1;
      }
      if (moved == 0)
        changed = 1;
      if (moved == -1)
        changed = 0;

      if (menu->mode == PSYNERGY_MODE_USE) {
        if ((gInput.pressed & KEY_SELECT) && shortcutHelp == 0) {
          ability = Ability_Get(menu->psynergies[state.selectedIndex] &
                                PSYNERGY_ABILITY_MASK);
          if (ability->fieldPsynergy == 0) {
            Sound_PlayFar(SE_MENU_ERROR);
          } else {
            Sound_PlayFar(SE_NOTE_MID);
            shortcutHelp = 1;
            menu->flags |= PSYNERGY_MENU_SHORTCUT_HELP;
            Window_ClearInteriorTilesFar(window, 0, 88, 120, 96);
            Text_DrawMessageAsTilesFar(MsgShortcutHelp, window, 0, 88);
          }
        }
        if ((gInput.held & KEY_SELECT) == 0 && shortcutHelp == 1) {
          shortcutHelp = 0;
          menu->flags &= ~PSYNERGY_MENU_SHORTCUT_HELP;
          Window_ClearInteriorTilesFar(window, 0, 88, 120, 96);
          Text_DrawMessageAsTilesFar(MsgChangeCharacterHelp, window,
                                     CHANGE_HELP_X, 88);
        }
      }

      if (gInput.pressed & KEY_A) {
        if (menu->mode != PSYNERGY_MODE_USE) {
          Sound_PlayFar(SE_PSYNERGY_START);
          result = menu->psynergies[state.selectedIndex];
          done = 1;
          break;
        }
        if (menu->psynergies[state.selectedIndex] != 0) {
          if (PsynergyMenu_IsActionRestricted(
                  menu->psynergies[state.selectedIndex]) != 0) {
            Sound_PlayFar(SE_MENU_ERROR);
            continue;
          }
          ability = Ability_Get(menu->psynergies[state.selectedIndex] &
                                PSYNERGY_ABILITY_MASK);
          if (ability->ppCost > owner->pp) {
            Sound_PlayFar(SE_MENU_ERROR);
          } else {
            Sound_PlayFar(SE_NOTE_LOW);
            result = menu->psynergies[state.selectedIndex];
            done = 1;
            break;
          }
        }
      }

      if (gInput.pressed & KEY_B) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        break;
      }

      if (((gInput.repeat & KEY_R) || (gInput.repeat & KEY_L)) &&
          (gInput.held & KEY_SELECT) == 0) {
        member = menu->tabIndex[paneIndex];
        ownerCount = menu->memberCount;
        listMode = menu->mode != PSYNERGY_MODE_USE ? PSYNERGY_LIST_SHORTCUTS
                                                   : PSYNERGY_LIST_FIELD;
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        menu->selectedIndexByMember[menu->memberIds[paneIndex]] =
            state.selectedIndex;
        do {
          if (gInput.repeat & KEY_R)
            member++;
          else
            member--;
          member = (member + ownerCount) % ownerCount;
          menu->selectedMember = menu->memberTable[member];
          menu->memberIds[0] = menu->memberTable[member];
          menu->psynergyCount = PsynergyMenu_CollectActions(
              Unit_Get(menu->memberIds[0]), menu->psynergies, listMode);
        } while (menu->psynergyCount == 0);
        menu->tabIndex[paneIndex] = member;
        Menu_CreatePartyFigures(member / 4 * 4);
        Menu_MarkChosenMember(menu->auxiliaryWindow, member, ownerCount);
        menu->pageArrows[0]->motion = ICON_HIDDEN;
        menu->pageArrows[1]->motion = ICON_HIDDEN;
        wait(1);
        StatusPanel_Draw(menu->statusWindow, menu->memberTable[member], 0,
                         STATUS_PANEL_EXPERIENCE);
        Menu_ReloadMemberPortrait(menu, menu->memberTable[member]);
        break;
      }

      if ((gInput.pressed & KEY_L) && (gInput.held & KEY_SELECT)) {
        ability = Ability_Get(menu->psynergies[state.selectedIndex] &
                              PSYNERGY_ABILITY_MASK);
        if (ability->fieldPsynergy == 0) {
          Sound_PlayFar(SE_MENU_ERROR);
        } else if (PsynergyMenu_SetShortcut(
                       menu->memberIds[paneIndex],
                       menu->psynergies[state.selectedIndex],
                       PSYNERGY_SHORTCUT_L) != 0) {
          Sound_PlayFar(SE_PSYNERGY_START);
          result = menu->psynergies[state.selectedIndex];
          menu->mode = PSYNERGY_MODE_SET_L;
          done = 1;
          break;
        }
      }

      if ((gInput.pressed & KEY_R) && (gInput.held & KEY_SELECT)) {
        ability = Ability_Get(menu->psynergies[state.selectedIndex] &
                              PSYNERGY_ABILITY_MASK);
        if (ability->fieldPsynergy == 0) {
          Sound_PlayFar(SE_MENU_ERROR);
        } else if (PsynergyMenu_SetShortcut(
                       menu->memberIds[paneIndex],
                       menu->psynergies[state.selectedIndex],
                       PSYNERGY_SHORTCUT_R) != 0) {
          Sound_PlayFar(SE_PSYNERGY_START);
          result = menu->psynergies[state.selectedIndex];
          menu->mode = PSYNERGY_MODE_SET_R;
          done = 1;
          break;
        }
      }
    }
  }

  menu->flags &= ~PSYNERGY_MENU_SHORTCUT_HELP;
  Icon_Show(menu->entryGridCursor);
  menu->sideRow[paneIndex] = state.selectedIndex;
  menu->selectedIndexByMember[menu->memberIds[paneIndex]] = state.selectedIndex;
  menu->sideAction[paneIndex] = result;
  if (GameFlag_Get(FLAG_MENU_START_PRESSED) != 0)
    result = -1;
  wait(1);
  return result;
}

#undef PSYNERGY_LIST_CURSOR_X

/// A Psynergy entry's action id, its PSYNERGY_ABILITY_MASK bits, taken out by
/// shifts.
#define PSYNERGY_ABILITY_OF(entry) (((u32)(entry) << 18) >> 18)

/// Returns 1 when actionEntry cannot be used from the menu: it has no field
/// effect and is not usable both in the field and in battle.
s32 PsynergyMenu_IsActionRestricted(s32 actionEntry) {
  struct AbilityDefinition *action =
      Ability_Get(PSYNERGY_ABILITY_OF(actionEntry));

  if (action->fieldPsynergy != 0 ||
      (action->flags & (ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE)) ==
          (ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE))
    return 0;
  return 1;
}

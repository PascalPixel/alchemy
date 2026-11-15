/// @file
/// The Psynergy menu: the field menu's Psynergy screen, the party member and
/// Psynergy lists, targets, and the L and R shortcuts.
// Part of menu.c.

#include "menu_internal.h"
#include "flags.h"
#include "sounds.h"

#define PSYNERGY_PROMPT_TILES_SIZE 8192

/// A Psynergy shortcut, and the field's Psynergy request packed the same way:
/// its party member above bit 10, the Psynergy's id below.
#define PSYNERGY_SHORTCUT_MEMBER_SHIFT 10
#define PSYNERGY_SHORTCUT_ID_MASK 0x3ff

s32 PsynergyMenu_IsActionRestricted(s32 no);

s32 PsynergyMenu_RunSteps(s32 *ownerOut, s32 *targetOut, s32 *itemOut);

/// A second item picker an event script opens, built as ItemMenu_Run: opens
/// the item menu over the field and runs its command loop; when that returns 1,
/// looks up the chosen item's action (unused) and records owner << 10 | item in
/// the event work at 0x166. Returns the loop's result.
s32 PsynergyMenu_Run(void) {
  // German, Spanish, French and Italian also suspend the field tasks and keep
  // the tiles at 0x06004000 aside, filled with a pattern, while the picker is
  // open.
  struct ItemMenuWork *screen;
#if TLA_PATCH_2
  void *savedTiles;
#endif
  s32 item;
  s32 target;
  s32 owner;
  s32 result;

  screen = HeapSlot_Alloc(HEAP_SLOT_MENU, 0xcec);
#if TLA_PATCH_2
  savedTiles = Heap_AllocEwram(0x2000);
#endif
  Ram_HeapSlots->actorSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_InitializeWork(0);
#if TLA_PATCH_2
  Menu_SuspendFieldTasks();
  Iwram_CopyWords(savedTiles, (void *)BG_CHAR_BLOCK(1), 0x2000);
  Iwram_FillWords((void *)BG_CHAR_BLOCK(1), 0x2000, 0x33333333);
  Text_UseExtraTilesFar(1);
#endif
  screen->partyCount = Party_ListMembersFar(screen->memberIds);
  ItemMenu_Initialize(0, 3, 0, 7);
  screen->questionWindow = Window_CreateFar(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
  Palette_LightenHighlight(HIGHLIGHT_PALETTE);
  Link_DrawPageArrowsFar(PAGE_ARROW_VRAM);

  Menu_StartWatchingStart();
  result = PsynergyMenu_RunSteps(&owner, &target, &item);
  Menu_StopWatchingStart();
  if (result == 1) {
    struct FieldWork *event = Ram_HeapSlots->fieldWork;
    u16 request;

    Ability_Get(PSYNERGY_ABILITY_MASK & screen->selectedItems[0]);
    request = item | (owner << PSYNERGY_SHORTCUT_MEMBER_SHIFT);
    event->psynergy = request;
  }

  WindowSprite_ClearListFar(screen->statusWindow);
  Ram_HeapSlots->windowWork[6] = 1;
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  HeapSlot_Release(HEAP_SLOT_MENU);
  Ram_HeapSlots->actorSystem->suspended = 0;
#if TLA_PATCH_2
  Window_MarkVisibleTextTilesFar();
  Text_UseExtraTilesFar(0);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), savedTiles,
                  PSYNERGY_PROMPT_TILES_SIZE);
  Ram_HeapSlots->windowWork[6] = 0;
  Heap_Release(savedTiles);
  wait(1);
  Menu_ResumeFieldTasks();
#endif
  wait(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  Ram_HeapSlots->windowWork[6] = 0;
  Party_ClearUnusableShortcutsFar();
  return result;
}

s32 Scene_HasPsynergyTriggerFar(s32 type);

/// PsynergyMenuWork.flags: the Psynergy list shows the shortcut help (Select
/// held).
#define PSYNERGY_MENU_SHORTCUT_HELP 2

#define MENU_TARGET_WHOLE_PARTY 9 ///< memberIds[1] for the whole party

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

/// What the Psynergy menu is open for (PsynergyMenuWork.mode).
enum PsynergyMenuMode {
  PSYNERGY_MODE_USE,
  PSYNERGY_MODE_SET_L,
  PSYNERGY_MODE_SET_R,
};

s16 Unit_AddPpFar(s32 unit, s32 amount);

s32 PsynergyMenu_SetShortcut(s32 kind, s32 id, s32 second);

s32 PsynergyMenu_ClassifySelectedPsynergy(void);
s32 PsynergyMenu_SelectPartySlot(s32 paneIndex);
s32 PsynergyMenu_SelectTarget(s32 paneIndex);
s32 PsynergyMenu_RunList(s32 paneIndex);

/// Runs the Psynergy menu, The Broken Seal's PsynergyMenu_RunSteps: choose a
/// party member and one of their Psynergy, then use it on a member (or the
/// whole party), or, in the shortcut modes, set it as the L or R shortcut.
/// Returns 1 with the member and Psynergy in the out parameters when the
/// chosen Psynergy works on the field, to be used once the menu closes; -1
/// when the player backs out or presses Start; otherwise the result of the
/// last use.
s32 PsynergyMenu_RunSteps(s32 *outMember, s32 *unused, s32 *outAction) {
  struct PsynergyMenuWork *menu;
  s32 useResult;
  s32 step;
  s32 done;
  s32 chosenAction;
  s32 targetMode;
  s32 actionEntry;
  s32 chosenTarget;
  struct BattleAct *action;

  step = PSYNERGY_STEP_MEMBER;
  done = 0;
  menu = Ram_HeapSlots->menuWork;
  useResult = 0;

  while (done == 0 && GameFlag_Get(FLAG_MENU_START_PRESSED) == 0) {
    switch (step) {
    // Choose whose Psynergy to look at.
    case PSYNERGY_STEP_MEMBER: {
      s32 cancelled;
      s32 chosen;

      menu->sideRow[0] = 0;
      ItemMenu_DrawMessage(0, (s32)MsgPsynergyChooseOwner);
      chosen = PsynergyMenu_SelectPartySlot(0);
      cancelled = -1;
      /* FAKEMATCH(dead): a redundant copy of -1 and an XOR test give -1 its own
       * register. */
      if (chosen == -1)
        cancelled = chosen;
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
      Unit_Get(menu->memberIds[0]);
      step = PSYNERGY_STEP_MEMBER;
      if (menu->psynergyCount != 0) {
        {
          struct PsynergyMenuWork *savedWork = menu;

          /* FAKEMATCH(dead): menu cleared and restored under a false test. */
          if (done)
            menu = NULL;
          if (done)
            menu = savedWork;
        }
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
        ItemMenu_PositionCategory();
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
        u32 entry = menu->sideAction[0];
        u32 mask;

        mask = PSYNERGY_ABILITY_MASK;
        mask &= entry;
        action = Ability_Get(mask);
        Unit_AddPpFar(menu->memberIds[0], -action->ppCost);
      }
      Unit_CalculateFar(menu->memberIds[0]);
      if (useResult != -1) {
        void *window;
        s32 target = menu->memberIds[1];

        /* FAKEMATCH(temporary): the target read before the window. */
        window = menu->statusWindow;
        StatusPanel_Draw(window, target, 0, STATUS_PANEL_EXPERIENCE);
        {
          u32 entry = menu->sideAction[0];
          u32 mask;

          mask = PSYNERGY_ABILITY_MASK;
          mask &= entry;
          Ability_PlayUseAnimation(mask);
        }
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
          menu->flags |= 1;
          step = PSYNERGY_STEP_LIST;
        } else {
          step = PSYNERGY_STEP_TARGET;
        }
      } else {
        useResult = 1;
        menu->flags |= 1;
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

/// How the selected psynergy is used: 0 acts at once, 1 picks a target,
/// 2 cannot be used here.
/// Says how the selected Psynergy is used from the menu: 0 on the field once
/// the menu closes (the scene has an event for its field Psynergy,
/// fieldPsynergy, and what the leader faces), 2 on the whole party (range
/// 0xff), 1 on one chosen party member when its target mode is 2, and 0
/// otherwise.
s32 PsynergyMenu_ClassifySelectedPsynergy(void) {
  struct PsynergyMenuWork *menu;
  struct BattleAct *psynergy;
  s32 diff;
  s32 ret;
  /* FAKEMATCH(pin): the action fixed to r3. */
  register u32 action asm("r3");

  menu = Ram_HeapSlots->menuWork;
  action = menu->sideAction[0];
  psynergy = Ability_Get(0x3fff & action);
  if (Scene_HasPsynergyTriggerFar(psynergy->fieldEffect) != 0)
    return 0;
  ret = 2;
  if (psynergy->range != 0xff) {
    u8 kind = psynergy->targetMode;

    diff = kind ^ 2;
    ret = (0 - diff) | diff;
    ret = (u32)ret >> 31;
    ret = 1 - ret;
  }
  return ret;
}

s32 PsynergyMenu_CollectActions(struct Unit *unit, u16 *actions, s32 mode);
s32 PsynergyMenu_SelectMember(u16 *ownerIds, u16 *unused);

/// Shows side partySlot's icon, collects the actions of the member chosen
/// there last (the first when none was) and runs PsynergyMenu_SelectMember on
/// them. Returns its result.
s32 PsynergyMenu_SelectPartySlot(s32 partySlot) {
  struct PsynergyMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 memberIndex = menu->tabIndex[partySlot];
  struct WindowSprite *icon;
  struct Unit *unit;
  u16 *actions;
  s32 result;

  result = 0;
  icon = menu->sideIcon[partySlot];
  icon->motion = ICON_SHOWN;
  icon->animStep = result;
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
  Icon_PrepareActor(icon);
  wait(1);
  return result;
}

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((struct InputState *)&gInput)->pressed)

void PsynergyMenu_RefreshMemberPsynergy(s32 ownerId);

s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner);

/// The party member row of the Psynergy menu.
s32 PsynergyMenu_SelectMember(u16 *ownerIds, u16 *unused) {
  struct PsynergyMenuWork *menu;
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

  menu = Ram_HeapSlots->menuWork;
  selection = menu->tabIndex[0];
  count = menu->tabCounts[0];
  redraw = 2;
  result = 0;
  shown = 0;
  menu->mode = result;
  unit = Unit_Get(ownerIds[selection]);
  if (Window_UpdateOrCreate(&menu->psynergyWindow, 13, 3, 17, 10, 2))
    Menu_SpawnIconEntries(menu, menu->psynergyWindow);
  if (Window_UpdateOrCreate(&menu->shortcutWindow, 13, 13, 17, 4, 2)) {
    menu->cursorIcon = WindowSprite_CreateFromResourceFar(
        2, 0, menu->shortcutWindow, 0, result);
    menu->cursorIcon->motion = ICON_HIDDEN;
  }
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      window = menu->statusWindow;
      unit = Unit_Get(ownerIds[selection]);
      PsynergyMenu_RefreshMemberPsynergy(ownerIds[selection]);
      StatusPanel_Draw(window, ownerIds[selection], 0, 0);
      PsynergyMenu_DrawShortcuts(menu->shortcutWindow, ownerIds[selection]);
      PsynergyMenu_CallIconRoutineWithValue(menu, ownerIds[selection]);
      if (redraw == 2) {
        Menu_CreateOwnerEntryObjects(selection / 4 * 4);
        wait(1);
      }
      Menu_HighlightPageEntry(menu->auxiliaryWindow, selection, count);
      if (!GameFlag_Get(0x151) && !shown) {
        WindowSprite_ClearListFar(menu->infoWindow);
#if TLA_PATCH_1
        // Japanese does not redraw the info window here.
        WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#endif
        Menu_DrawCoins(menu->infoWindow);
        shown = 1;
      } else {
        GameFlag_Clear(0x151);
      }
    }
    MenuCursor_Position((selection % 4) * 24 - 10, 16);
    wait(1);
    if (KEYS_PRESSED & 1) {
      if (menu->psynergyCount) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        result = ownerIds[selection];
        break;
      }
      Sound_PlayFar(SE_MENU_ERROR);
    }
    if ((KEYS_PRESSED & KEY_L) || (KEYS_PRESSED & KEY_R)) {
      result = ownerIds[selection];
      if (KEYS_PRESSED & KEY_L)
        menu->mode = 1;
      else
        menu->mode = 2;
      actions = Heap_Alloc(32 * sizeof(u16));
      fieldCount = PsynergyMenu_CollectActions(unit, actions, 1);
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
    if (KEYS_PRESSED & 2) {
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

#undef KEYS_PRESSED

void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);

/// Collects the owner's Psynergy and redraws the list, saying when it is
/// empty.
void PsynergyMenu_RefreshMemberPsynergy(s32 ownerId) {
  u16 *psynergies;
  struct PsynergyMenuWork *menu;
  void *owner;

  menu = Ram_HeapSlots->menuWork;
  owner = Unit_Get(ownerId);
  psynergies = menu->psynergies;
  menu->psynergyCount =
      PsynergyMenu_CollectActions(owner, psynergies, PSYNERGY_LIST_FIELD);
  WindowSprite_RedrawSavedRectFar(menu->psynergyWindow);
  PsynergyMenu_RefreshMemberEntries(0x6c, 0x20, 8);
  PsynergyMenu_DrawPsynergyIcons(psynergies);
  if (menu->psynergyCount == 0) {
    Text_DrawMessageAsTilesFar((s32)MsgPsynergyMenuEmpty, menu->psynergyWindow,
                               0, 0x18);
  }
}

/// Chooses the party member to use the selected Psynergy on.
s32 PsynergyMenu_SelectTarget(s32 browseOnly) {
  struct PsynergyMenuWork *menu;
  s32 selection;
  s32 count;
  s32 redraw;
  s32 result;
  s32 shown;
  struct WindowSprite *icon;
  s32 column;

  menu = Ram_HeapSlots->menuWork;
  selection = menu->tabIndex[1];
  count = menu->memberCount;
  result = 0;
  shown = 0;
  Unit_Get(menu->memberTable[menu->tabIndex[0]]);
  redraw = 2;
  MenuCursor_Slide(selection * 24 - 10, 16);
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      Unit_Get(menu->memberTable[selection]);
      icon = menu->sideIcon[1];
      column = (selection % 4) * 3;
      icon->sprite.oam.x = icon->x =
          (menu->auxiliaryWindow->x + column) * 8 - 2;
      if (browseOnly == 0) {
        StatusPanel_Draw(menu->statusWindow, menu->memberTable[selection], 0,
                         0);
        PsynergyMenu_CallIconRoutineWithValue(menu,
                                              menu->memberTable[selection]);
        if (redraw == 2) {
          Menu_CreateOwnerEntryObjects(selection / 4 * 4);
          wait(1);
        }
        Menu_HighlightPageEntry(menu->auxiliaryWindow, selection, count);
        if (!GameFlag_Get(0x151) && !shown) {
          // The Japanese release clears the info window; from patch 1 it
          // redraws the window's saved rectangle.
#if TLA_PATCH_1
          WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#else
          WindowSprite_ClearListFar(menu->infoWindow);
#endif
          // Japanese draws the description as a resource string.
#if LANGUAGE == LANGUAGE_JAPANESE
          Text_DrawMessageFar((menu->sideAction[0] & 0x3fff) +
                                  MsgAbilityDescription,
                              menu->infoWindow, 0, 0);
#else
          Text_DrawMessageAsTilesFar((menu->sideAction[0] & 0x3fff) +
                                         MsgAbilityDescription,
                                     menu->infoWindow, 0, 0);
#endif
          shown = 1;
        } else {
          GameFlag_Clear(0x151);
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
  Icon_PrepareActor(icon);
  icon->motion = ICON_HIDDEN;
  wait(1);
  menu->tabIndex[1] = selection;
  menu->selectedMember = menu->memberTable[selection];
  menu->memberIds[1] = menu->memberTable[selection];
  selection = menu->tabIndex[0];
  count = menu->tabCounts[0];
  Menu_CreateOwnerEntryObjects(selection / 4 * 4);
  Menu_HighlightPageEntry(menu->auxiliaryWindow, selection, count);
  menu->pageArrows[0]->motion = ICON_HIDDEN;
  menu->pageArrows[1]->motion = ICON_HIDDEN;
  return result;
}

/// Sets the L or R Psynergy shortcut (enum PsynergyShortcut):
/// memberId << 10 | the Psynergy's id. Returns 1.
s32 PsynergyMenu_SetShortcut(s32 kind, s32 id, s32 second) {
  u32 packed = (kind << PSYNERGY_SHORTCUT_MEMBER_SHIFT) | (id & 0x3fff);

  if (second == 0)
    gMainRoutineWork.firstShortcut = packed;
  else {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->secondShortcut = packed;
  }
  return 1;
}

#if LANGUAGE == LANGUAGE_JAPANESE
// Japanese drops the colon after "L" and "R" and draws the ability's name 8
// pixels further left when that name is wider than this.
#define COLON_NAME_WIDTH_LIMIT 11
#elif TLA_PATCH_2
// German, Spanish, French and Italian leave out the owner's name when the
// ability's name is wider than this.
#define OWNER_NAME_WIDTH_LIMIT 10
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
extern const u8 gMenu_ShortcutLString[];
extern const u8 gMenu_ShortcutLColonString[];
extern const u8 gMenu_ShortcutRString[];
extern const u8 gMenu_ShortcutRColonString[];
#endif

#if LANGUAGE == LANGUAGE_JAPANESE

#else
#endif

/// Draws the two packed values of gMainRoutineWork (owner << 10 | ability), the
/// shortcuts' messages, into a window: a heading (one when both are set,
/// another otherwise), then for each its ability's name and the owner's name,
/// or an empty-slot message. Measures each ability's name first. The
/// international editions draw the name through MessageArgument_PushFar and a
/// label message; German, Spanish, French and Italian leave out the owner's
/// name when the ability's name is wide. Japanese draws "L" or "L:" (and "R"
/// or "R:") itself, then the name, dropping the colon for a wide name. Returns
/// 1. owner is not used.
#if LANGUAGE != LANGUAGE_JAPANESE
s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  u8 *name;
#if TLA_PATCH_2
  s32 wide;
#endif

  if (gMainRoutineWork.firstShortcut != 0 &&
      gMainRoutineWork.secondShortcut != 0)
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 1, window, 0, -8);
  else
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetL - 2, window, 0, -8);

  name = &MsgAbilityName[gMainRoutineWork.firstShortcut & 0x3ff];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
#if TLA_PATCH_2
  if (width < 0 || width > OWNER_NAME_WIDTH_LIMIT)
    wide = 1;
  else
    wide = 0;
#endif
  if (gMainRoutineWork.firstShortcut != 0) {
    MessageArgument_PushFar(gMainRoutineWork.firstShortcut &
                                PSYNERGY_SHORTCUT_ID_MASK,
                            MESSAGE_ARGUMENT_ABILITY);
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 4, window, 0, 0);
#if TLA_PATCH_2
    if (!wide)
#endif
      Text_DrawStringAtOffsetFar((s32)Unit_Get(gMainRoutineWork.firstShortcut >>
                                               PSYNERGY_SHORTCUT_MEMBER_SHIFT),
                                 window, 80, 0);
  } else {
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 2, window, 0, 0);
  }

  name = &MsgAbilityName[gMainRoutineWork.secondShortcut & 0x3ff];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
#if TLA_PATCH_2
  if (width < 0 || width > OWNER_NAME_WIDTH_LIMIT)
    wide = 1;
  else
    wide = 0;
#endif
  if (gMainRoutineWork.secondShortcut != 0) {
    MessageArgument_PushFar(gMainRoutineWork.secondShortcut &
                                PSYNERGY_SHORTCUT_ID_MASK,
                            MESSAGE_ARGUMENT_ABILITY);
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 5, window, 0, 8);
#if TLA_PATCH_2
    if (!wide)
#endif
      Text_DrawStringAtOffsetFar(
          (s32)Unit_Get(gMainRoutineWork.secondShortcut >>
                        PSYNERGY_SHORTCUT_MEMBER_SHIFT),
          window, 80, 8);
  } else {
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 3, window, 0, 8);
  }
  return 1;
}
#else
s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  u8 *name;

  if (gMainRoutineWork.firstShortcut != 0 &&
      gMainRoutineWork.secondShortcut != 0)
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 1, window, 0, -8);
  else
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetL - 2, window, 0, -8);

  name = &MsgAbilityName[gMainRoutineWork.firstShortcut & 0x3ff];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
  if (gMainRoutineWork.firstShortcut != 0) {
    if (width < 0 || width > COLON_NAME_WIDTH_LIMIT) {
      Text_DrawStringInWindowFar(gMenu_ShortcutLString, window, 0, 0);
      Text_DrawMessageAsTilesFar((s32)name, window, 8, 0);
    } else {
      Text_DrawStringInWindowFar(gMenu_ShortcutLColonString, window, 0, 0);
      Text_DrawMessageAsTilesFar((s32)name, window, 16, 0);
    }
    Text_DrawStringAtOffsetFar((s32)Unit_Get(gMainRoutineWork.firstShortcut >>
                                             PSYNERGY_SHORTCUT_MEMBER_SHIFT),
                               window, 80, 0);
  } else {
    Text_DrawStringInWindowFar(gMenu_ShortcutLColonString, window, 0, 0);
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 2, window, 24, 0);
  }

  name = &MsgAbilityName[gMainRoutineWork.secondShortcut & 0x3ff];
  Text_GetMessageSizeFar(name, &left, &top, &width, &height);
  if (gMainRoutineWork.secondShortcut != 0) {
    if (width < 0 || width > COLON_NAME_WIDTH_LIMIT) {
      Text_DrawStringInWindowFar(gMenu_ShortcutRString, window, 0, 8);
      Text_DrawMessageAsTilesFar((s32)name, window, 8, 8);
    } else {
      Text_DrawStringInWindowFar(gMenu_ShortcutRColonString, window, 0, 8);
      Text_DrawMessageAsTilesFar((s32)name, window, 16, 8);
    }
    Text_DrawStringAtOffsetFar((s32)Unit_Get(gMainRoutineWork.secondShortcut >>
                                             PSYNERGY_SHORTCUT_MEMBER_SHIFT),
                               window, 80, 8);
  } else {
    Text_DrawStringInWindowFar(gMenu_ShortcutRColonString, window, 0, 8);
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetR + 3, window, 24, 8);
  }
  return 1;
}
#endif

#undef OWNER_NAME_WIDTH_LIMIT

/// A Psynergy entry's icon sprite as the Psynergy menu reads its VRAM cache
/// slot.
struct PsynergyEntryIcon {
  u8 unk00[14];
  u8 slot;
};

/// The menu work as the Psynergy menu's icons read it: each entry's icon.
struct PsynergyEntryMenu {
  u8 unk000[0x4c];
  struct PsynergyEntryIcon *entryIcons[32];
};

/// Loads each listed Psynergy's icon into its icon sprite (32 entries) and
/// hides the icons of empty entries.
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies) {
  s32 i;
  struct WindowSprite **icons;
  u16 *p;
  s32 psynergyId;

  icons = ((struct PsynergyMenuWork *)Ram_HeapSlots->menuWork)->entryIcons;
  p = psynergies;
  for (i = 0; i < PSYNERGY_SLOTS; i++) {
    psynergyId = *p++;
    if (psynergyId != 0) {
      Resource_LoadByModeIntoSlotFar(ICON_KIND_ABILITY, psynergyId,
                                     (u8)(*icons)->index, 0);
    }
    icons++;
  }
  Menu_HideEmptyEntryIcons(psynergies);
}

// Lists a unit's Psynergy for a menu.

/// Lists a unit's Psynergy into actions (32 entries, cleared first) and returns
/// how many: mode 1 those with a field effect, any other mode those first and
/// then, unless mode is 2, the rest.
s32 PsynergyMenu_CollectActions(struct Unit *unit, u16 *actions, s32 mode) {
  s32 i;
  s32 count;
  struct BattleAct *ability;
  s32 offset;

  count = 0;
  for (i = 0; i < 32; i++)
    actions[i] = 0;

  if (mode == 1) {
    for (i = 0; i < 32; i++) {
      if (unit->psynergy[i].encodedAction != 0) {
        if (Ability_Get(unit->psynergy[i].encodedAction & PSYNERGY_ABILITY_MASK)
                ->fieldEffect != 0) {
          actions[count] = unit->psynergy[i].encodedAction;
          count++;
        }
      }
    }
  } else {
    for (i = 0; i < 32; i++) {
      if (unit->psynergy[i].encodedAction != 0) {
        ability = Ability_Get(unit->psynergy[i].encodedAction &
                              PSYNERGY_ABILITY_MASK);
        if (ability->fieldEffect != 0 || (ability->flags & 0x40) != 0) {
          actions[count] = unit->psynergy[i].encodedAction;
          count++;
        }
      }
    }
    if (mode != 2) {
      for (i = 0; i < 32; i++) {
        if (unit->psynergy[i].encodedAction != 0) {
          ability = Ability_Get(unit->psynergy[i].encodedAction &
                                PSYNERGY_ABILITY_MASK);
          if (ability->fieldEffect == 0 && (ability->flags & 0x40) == 0) {
            actions[count] = unit->psynergy[i].encodedAction;
            count++;
          }
        }
      }
    }
  }
  return count;
}

// Psynergy menu workspace (known fields only).

/// Split the selected slot owner's Psynergy into pages of five rows.
s32 PsynergyMenu_BuildPagedList(struct MenuResult *result, s32 index) {
  s32 ownerState;
  struct PsynergyMenuWork *menu = Ram_HeapSlots->menuWork;
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
  result->ownerState = ownerState;
  result->page = page;
  result->pageCount = pageCount;
  result->row = row;
  result->entryCount = entryCount;
  result->selectedIndex = selectedIndex;
  return 1;
}

// Psynergy menu workspace; only identified fields are declared here.

// Shows the selected row's description and sets the five rows' backgrounds.
/// Updates the Psynergy list for the row under the cursor: puts the Psynergy's
/// description in the info window, unless a kept message is there, and
/// highlights its row of the five. window and work are not used.
s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work,
                                struct MenuResult *state) {
  struct PsynergyMenuWork *menu;
  s32 combined;
  s32 row;

  menu = Ram_HeapSlots->menuWork;
  combined = state->page * LIST_PAGE_SIZE;
  combined += state->row;
  state->selectedIndex = combined;

  if (GameFlag_Get(0x151) == 0) {
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
      s32 masked =
          (s32)(MsgAbilityDescription + (menu->psynergies[combined] & 0x1ff));

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
    GameFlag_Clear(FLAG_SCENE_TEMP_FF);
  }

  row = 0;
  do {
    if (row == state->row) {
      Render_SetTilemapFlagRect(menu->psynergyWindow, 1, row * 2 + 1, 14, 1,
                                HIGHLIGHT_PALETTE);
    } else {
      Render_SetTilemapFlagRect(menu->psynergyWindow, 1, row * 2 + 1, 14, 1,
                                WINDOW_PALETTE);
    }
    row++;
  } while (row <= 4);

  wait(1);
  return 1;
}

// The Japanese page draws its help line, page icons, heading and names at
// other columns.
#if LANGUAGE == LANGUAGE_JAPANESE
#define HELP_X 16
#define PAGE_X 120
#define HEADING_X 72
#define NAME_X 32
#else
#define HELP_X 0
#define PAGE_X 112
#define HEADING_X 96
#define NAME_X 16
#endif

// The psynergy menu work block, as this function reads it.

/// Draws one page of the Psynergy list (name, PP cost, greyed when the PP is
/// short or the action restricted): The Broken Seal's
/// PsynergyMenu_DrawActionPage with this game's work-block offsets.
s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused,
                                const struct MenuResult *state) {
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  s32 cursor;
  struct Unit *owner;
  struct BattleAct *ability;
  struct PsynergyMenuWork *menu = Ram_HeapSlots->menuWork;

  (void)unused;

  WindowSprite_RedrawSavedRectFar(window);
  Window_DrawDividerLineFar(window, 0, 11, 16, 11);

  if (PSYNERGY_MENU_SHORTCUT_HELP & *(u16 *)((u8 *)menu + 0x21c)) {
    Text_DrawMessageAsTilesFar((s32)MsgShortcutSetL - 1, window, 0, 88);
  } else {
    Text_DrawMessageAsTilesFar((s32)MsgArrangeItemsHelp + 2, window, HELP_X,
                               88);
  }

  firstEntry = state->page * LIST_PAGE_SIZE;
  visibleCount = (u8)(state->entryCount - firstEntry);
  if (visibleCount > LIST_PAGE_SIZE) {
    visibleCount = LIST_PAGE_SIZE;
  }

  Menu_SetPageIcons(LIST_PAGE_SIZE, firstEntry, window, PAGE_X, 0x22);
  PagedList_DrawPageNumbers(window, state->entryCount, LIST_PAGE_SIZE,
                            state->page, 15);
  Text_DrawMessageAsTilesFar((s32)MsgPsynergyChooseTarget + 2, window,
                             HEADING_X, 0);

  row = 0;
  if (visibleCount > row) {
    cursor = firstEntry * 2 + 0x1c4;
    do {
      owner = Unit_Get(menu->memberIds[0]);
      ability = Ability_Get(PSYNERGY_ABILITY_MASK &
                            *(const u16 *)(cursor + (s32)menu));

      if (ability->ppCost > owner->pp) {
        Text_SetColorFar(TEXT_COLOR_RED);
      } else if (PsynergyMenu_IsActionRestricted(
                     PSYNERGY_ABILITY_MASK &
                     *(const u16 *)(cursor + (s32)menu)) != 0) {
        Text_SetColorFar(TEXT_COLOR_YELLOW);
      } else {
        Text_SetColorFar(TEXT_COLOR_NORMAL);
      }

      Text_DrawMessageAsTilesFar(
          (PSYNERGY_ABILITY_MASK & *(const u16 *)(cursor + (s32)menu)) +
              (s32)MsgAbilityName,
          window, NAME_X, row * 16 + 8);
      Text_DrawNumberAtOffsetFar(ability->ppCost, 2, window, 104, row * 16 + 8);
      Text_SetColorFar(TEXT_COLOR_NORMAL);

      row++;
      cursor += 2;
    } while (visibleCount > row);
  }

  return 1;
}

#undef PAGE_X
#undef HEADING_X
#undef NAME_X

// The Japanese list puts its cursor and its help line at other columns.
#if LANGUAGE == LANGUAGE_JAPANESE
#define CURSOR_X 98
#define CHANGE_HELP_X 16
#else
#define CURSOR_X 88
#define CHANGE_HELP_X 0
#endif

s32 PsynergyMenu_IsActionRestricted(s32 actionEntry);

/// PsynergyMenu_RunList runs the Psynergy list of the member in paneIndex.
s32 PsynergyMenu_RunList(s32 paneIndex) {
  struct PsynergyMenuWork *menu;
  struct BattleAct *ability;
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
  s32 work[5];
  struct MenuResult state;

  menu = Ram_HeapSlots->menuWork;
  result = 0;
  previous = 0;
  shortcutHelp = 0;
  menu->sideIcon[paneIndex]->motion = ICON_HIDDEN;
  Window_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, 2);
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
      MenuCursor_Position(CURSOR_X, state.row * 16 + 36);

      if (changed != 0) {
        changed = 0;
        if (menu->psynergies[previous] != 0) {
          icon = menu->entryIcons[previous];
          Icon_PrepareActor(icon);
        }
        if (redraw != 0) {
          redraw = 0;
          wait(1);
          PsynergyMenu_DrawActionPage(window, 0, &state);
        }
        PsynergyMenu_DrawDetailPage(window, work, &state);
        menu->sideAction[paneIndex] = menu->psynergies[state.selectedIndex];
        menu->cursorIcon->motion = ICON_HIDDEN;
        if (menu->psynergies[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          icon->motion = WINDOW_SPRITE_MOTION_SWELL;
          icon->animStep = 0;
          icon->sentinel = OAM_DEPTH_FRONT - 5;
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
          if (ability->fieldEffect == 0) {
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
        Menu_CreateOwnerEntryObjects(member / 4 * 4);
        Menu_HighlightPageEntry(menu->auxiliaryWindow, member, ownerCount);
        menu->pageArrows[0]->motion = ICON_HIDDEN;
        menu->pageArrows[1]->motion = ICON_HIDDEN;
        wait(1);
        StatusPanel_Draw(menu->statusWindow, menu->memberTable[member], 0,
                         STATUS_PANEL_EXPERIENCE);
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->memberTable[member]);
        break;
      }

      if ((gInput.pressed & KEY_L) && (gInput.held & KEY_SELECT)) {
        ability = Ability_Get(menu->psynergies[state.selectedIndex] &
                              PSYNERGY_ABILITY_MASK);
        if (ability->fieldEffect == 0) {
          Sound_PlayFar(SE_MENU_ERROR);
        } else if (PsynergyMenu_SetShortcut(
                       menu->memberIds[paneIndex],
                       menu->psynergies[state.selectedIndex], 0) != 0) {
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
        if (ability->fieldEffect == 0) {
          Sound_PlayFar(SE_MENU_ERROR);
        } else if (PsynergyMenu_SetShortcut(
                       menu->memberIds[paneIndex],
                       menu->psynergies[state.selectedIndex], 1) != 0) {
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
  Icon_PrepareActor(menu->entryGridCursor);
  menu->sideRow[paneIndex] = state.selectedIndex;
  menu->selectedIndexByMember[menu->memberIds[paneIndex]] = state.selectedIndex;
  menu->sideAction[paneIndex] = result;
  if (GameFlag_Get(FLAG_MENU_START_PRESSED) != 0)
    result = -1;
  wait(1);
  return result;
}

#undef CURSOR_X
#undef CHANGE_HELP_X

s32 PsynergyMenu_IsActionRestricted(s32 no) {
  u8 *action = Ability_Get((u32)(no << 18) >> 18);
  u32 flags;

  if (action[6] == 0) {
    flags = action[1] & 0xc0;
    no = 1;
    if (flags != 0xc0)
      return no;
  }
  no = 0;
  return no;
}

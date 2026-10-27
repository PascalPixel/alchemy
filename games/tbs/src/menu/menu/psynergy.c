/// @file
/// The Psynergy menu: the field menu's Psynergy screen, the party member and
/// Psynergy lists, targets, and the L and R shortcuts.
// Part of menu.c.

#include "../menu_internal.h"

// The German, Spanish, French and Italian Psynergy menus save the window
// tiles they draw over and restore them after.
#if TBS_PATCH_2
#define PSYNERGY_PROMPT_TILES BG_CHAR_BLOCK(1)
#define PSYNERGY_PROMPT_TILES_SIZE 0x2000
#endif

// The highlight over the Psynergy list's rows.
#if LANGUAGE != LANGUAGE_JAPANESE
#define PSYNERGY_DETAIL_ROW_X 0
#define PSYNERGY_DETAIL_ROW_WIDTH 15
#else
#define PSYNERGY_DETAIL_ROW_X 1
#define PSYNERGY_DETAIL_ROW_WIDTH 14
#endif

// The Psynergy list's page icons, "PP" label, names and hand cursor.
#if LANGUAGE != LANGUAGE_JAPANESE
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

/// The field menu's Psynergy screen (FieldMenu_Run,
/// FIELD_MENU_PSYNERGY), over the field, which stops meanwhile. Psynergy used
/// on a party member takes effect here. When the player picks Psynergy to use
/// on the field, it is recorded as the field's Psynergy request (FieldWork's
/// psynergy, member << 10 | Psynergy), which FieldLoop_Run hands to
/// Field_UsePsynergy once the menu has closed, and 1 is returned; -1 when the
/// player backs out or presses Start (the field menu then opens again). Owns
/// the menu work (HEAP_SLOT_MENU) while it runs and frees it before returning.
s32 PsynergyMenu_Run(void) {
#if TBS_PATCH_2
  void *saved = AllocEwram(PSYNERGY_PROMPT_TILES_SIZE);
#endif
  struct PsynergyMenuWork *menu = AllocHeapSlot(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  s32 member;
  s32 unused;
  s32 psynergy;
  s32 result;

  ((struct ActorSystemWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_ACTOR_SYSTEM))
      ->suspended = 1;
  Window_DrawFrameFar(0, 0, 30, 20);
  WaitTicks(1);
  Window_InitializeWork(0);
  menu->memberCount = Party_ListMembersFar(menu->memberTable);
  ItemMenu_Initialize(0, 3, 0, 7);
  menu->messageWindow = Window_CreateFar(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
  Palette_LightenBankHighlight(HIGHLIGHT_PALETTE);
  Link_DrawPageArrowsFar(PAGE_ARROW_VRAM);
#if TBS_PATCH_2
  Menu_SuspendFieldTasks();
  Iwram_CopyWords(saved, PSYNERGY_PROMPT_TILES, PSYNERGY_PROMPT_TILES_SIZE);
  Iwram_FillWords(PSYNERGY_PROMPT_TILES, PSYNERGY_PROMPT_TILES_SIZE,
                  0x33333333);
  Text_UseExtraTilesFar(1);
#endif
  // Let the player choose.
  Menu_CancelSoundReset();
  result = PsynergyMenu_RunSteps(&member, &unused, &psynergy);
  Menu_EnsureCancelSound();
  if (result == 1) {
    struct FieldWork *target = HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD);
    u16 request;
    // The action looked up here is not used.
    Ability_Get(PSYNERGY_ABILITY_MASK & menu->sideAction[0]);
    request = psynergy | (member << 10);
    target->psynergy = request;
  }
  // Close the menu and give the screen back to the field.
  WindowSprite_ClearListFar((void *)menu->statusWindow);
  ((struct WindowWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW))->holdTilemapUpload =
      1;
  ItemMenu_Close();
  Window_DrawFrameFar(0, 0, 30, 20);
  FreeHeapSlot(HEAP_SLOT_MENU);
  ((struct ActorSystemWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_ACTOR_SYSTEM))
      ->suspended = 0;
#if TBS_PATCH_2
  Window_MarkVisibleTextTilesFar();
  Text_UseExtraTilesFar(0);
  Iwram_CopyWords(PSYNERGY_PROMPT_TILES, saved, PSYNERGY_PROMPT_TILES_SIZE);
  ((struct WindowWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW))->holdTilemapUpload =
      0;
  Free(saved);
  WaitTicks(1);
  Menu_ResumeFieldTasks();
#endif
  WaitTicks(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  ((struct WindowWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW))->holdTilemapUpload =
      0;
  Event_ClearInvalidPackedValuesFar();
  return result;
}

/// The steps of the Psynergy menu (PsynergyMenu_RunSteps).
enum PsynergyMenuStep {
  PSYNERGY_STEP_MEMBER,
  PSYNERGY_STEP_LIST,
  /// how the Psynergy is used (PsynergyMenu_ClassifySelectedPsynergy)
  PSYNERGY_STEP_CLASSIFY,
  PSYNERGY_STEP_TARGET,
  PSYNERGY_STEP_APPLY,
};

/// Runs the Psynergy menu: choose a party member and one of their Psynergy,
/// then use it on a party member (or the whole party), or, in the shortcut
/// modes, set it as the L or R shortcut. Returns 1 with the party member and
/// Psynergy in the out parameters when the chosen Psynergy works on the field,
/// to be used once the menu closes; -1 when the player backs out or presses
/// Start; otherwise the result of the last use.
s32 PsynergyMenu_RunSteps(s32 *outMember, s32 *unused, s32 *outAction) {
  struct PsynergyMenuWork *menu;
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
  menu = gMenuWork;
  useResult = 0;

  while (done == 0 && GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
    switch (step) {
    // Choose whose Psynergy to look at.
    case PSYNERGY_STEP_MEMBER: {
      s32 cancelled;
      s32 chosen;

      menu->sideRow[0] = 0;
      ItemMenu_DrawMessage(0, MsgPsynergyChooseOwner);
      chosen = PsynergyMenu_SelectPartySlot(0);
      cancelled = -1;
      /* FAKEMATCH(dead): a redundant copy of -1 and an XOR test give -1 and 1
       * the ROM's registers; as if (chosen == -1) setting done and useResult,
       * 6 of 317 instructions differ in tbs-en. */
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
      WaitTicks(1);
      Unit_GetFar(menu->memberIds[0]);
      step = PSYNERGY_STEP_MEMBER;
      if (menu->psynergyCount != 0) {
        {
          struct PsynergyMenuWork *savedWork = menu;

          /* FAKEMATCH(dead): menu cleared and restored under a false test, so
           * the mode's offset is built from its own constant; without the two
           * tests, 2 of 317 instructions differ in tbs-en. */
          if (done)
            menu = NULL;
          if (done)
            menu = savedWork;
        }
        switch (menu->mode) {
        case PSYNERGY_MODE_USE:
          ItemMenu_DrawMessage(0, MsgPsynergyChooseAbility);
          break;
        case PSYNERGY_MODE_SET_L:
          ItemMenu_DrawMessage(0, MsgPsynergyChooseForL);
          break;
        case PSYNERGY_MODE_SET_R:
          ItemMenu_DrawMessage(0, MsgPsynergyChooseForR);
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
              WindowSprite_ClearListFar((void *)menu->infoWindow);
              ItemMenu_ShowMessage(MsgShortcutSetL, -1, -1);
#if !TBS_PATCH_1
              WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#endif
            } else {
              PsynergyMenu_SetShortcut(menu->memberIds[0], chosenAction,
                                       PSYNERGY_SHORTCUT_R);
              WindowSprite_ClearListFar((void *)menu->infoWindow);
              ItemMenu_ShowMessage(MsgShortcutSetR, -1, -1);
#if !TBS_PATCH_1
              WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#endif
            }
            step = PSYNERGY_STEP_MEMBER;
          }
        }
      }
      break;

    // Choose the party member to use it on.
    case PSYNERGY_STEP_TARGET:
      ItemMenu_DrawMessage(0, MsgPsynergyChooseTarget);
      chosenTarget = PsynergyMenu_SelectTarget(0);
      step = PSYNERGY_STEP_APPLY;
      if (chosenTarget == -1) {
        menu->flags |= 1;
        step = PSYNERGY_STEP_LIST;
      }
      break;

    // How is it used? On one party member, on the whole party, or on the field
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
    // target when it failed on one party member.
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
        Ability_PlayUseAnimation(menu->sideAction[0] & PSYNERGY_ABILITY_MASK);
        WindowSprite_ClearListFar((void *)menu->infoWindow);
        ItemMenu_ShowMessage(menu->useResult + MsgItemUseResult, 0, -1);
      } else {
        Sound_PlaySongFar(SE_MENU_ERROR);
        WindowSprite_ClearListFar((void *)menu->infoWindow);
        ItemMenu_ShowMessage(menu->useResult + MsgItemUseResult, useResult,
                             useResult);
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

  if (GameFlag_GetFar(FLAG_MENU_START_PRESSED) != 0) {
    useResult = -1;
  }
  return useResult;
}

/// Says how the selected Psynergy is used from the menu: 0 on the field once
/// the menu closes (the scene has an event for its field Psynergy,
/// fieldPsynergy, and what the leader faces), 2 on the whole party (range
/// 0xff), 1 on one chosen party member when its target mode is 2, and 0
/// otherwise.
s32 PsynergyMenu_ClassifySelectedPsynergy(void) {
  struct AbilityDefinition *action;
  s32 targetModeDifference;
  s32 classification;

  action = Ability_Get(PSYNERGY_ABILITY_MASK &
                       ((struct PsynergyMenuWork *)gMenuWork)->sideAction[0]);
  if (Scene_HasFieldPsynergyTriggerFar(action->fieldPsynergy) != 0) {
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

/// Lets the player choose a party member in side partySlot (0 or 1), starting
/// from the one chosen last, with each party member's Psynergy shown as the
/// cursor passes. Returns the chosen party member's id, or -1 when the player
/// backs out.
s32 PsynergyMenu_SelectPartySlot(s32 partySlot) {
  struct PsynergyMenuWork *menu = gMenuWork;
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

  unit = Unit_GetFar(menu->memberTable[memberIndex]);
  actions = menu->psynergies;
  menu->psynergyCount =
      PsynergyMenu_CollectActions(unit, actions, PSYNERGY_LIST_FIELD);
  result = PsynergyMenu_SelectMember(menu->memberTable, actions);
  icon = menu->sideIcon[partySlot];
  Icon_PrepareActor(icon);
  WaitTicks(1);
  return result;
}

/// The party member row of the Psynergy menu. Left and Right move between
/// party members, showing each one's Psynergy, status and shortcuts; A chooses
/// a party member who has Psynergy to use, L or R one who has field Psynergy to
/// set as that shortcut (PSYNERGY_MODE_SET_L or _SET_R), and B backs out.
/// Returns the chosen party member's id, or -1.
s32 PsynergyMenu_SelectMember(u16 *memberIds, u16 *unused) {
  struct PsynergyMenuWork *menu;
  s32 selection;
  s32 memberCount;
  s32 redraw;
  s32 result;
  struct Unit *unit;
  struct Window *window;
  s32 shown;
  s32 i;
  u16 *actions;
  s32 shortcutCount;
  s8 fieldCount;

  menu = gMenuWork;
  selection = menu->tabIndex[0];
  memberCount = menu->tabCounts[0];
  redraw = 1;
  result = 0;
  shown = 0;
  menu->mode = result;
  unit = Unit_GetFar(memberIds[selection]);
  if (Window_UpdateOrCreate(&menu->psynergyWindow, 13, 3, 17, 10, 2))
    Menu_SpawnIconEntries((struct ItemMenuWork *)menu, menu->psynergyWindow);
  if (Window_UpdateOrCreate(&menu->shortcutWindow, 13, 13, 17, 4, 2)) {
    menu->cursorIcon = WindowSprite_CreateFromResourceFar(
        ICON_KIND_ITEM, 0, menu->shortcutWindow, 0, result);
    menu->cursorIcon->motion = ICON_HIDDEN;
  }
  while (!GameFlag_GetFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      selection = (memberCount + selection) % memberCount;
      window = menu->statusWindow;
      unit = Unit_GetFar(memberIds[selection]);
      PsynergyMenu_RefreshMemberPsynergy(memberIds[selection]);
      StatusPanel_Draw(window, memberIds[selection], 0,
                       STATUS_PANEL_EXPERIENCE);
      PsynergyMenu_DrawShortcuts(menu->shortcutWindow, memberIds[selection]);
      PsynergyMenu_CallIconRoutineWithValue(menu, memberIds[selection]);
      for (i = 3; i >= 0; i--)
        menu->rowPositions[i] = 30;
      // Raise the chosen party member.
      menu->rowPositions[selection] = 26;
      if (!GameFlag_GetFar(FLAG_MENU_INFO_KEPT) && !shown) {
        WindowSprite_ClearListFar((void *)menu->infoWindow);
#if TBS_PATCH_1
        WindowSprite_RedrawSavedRectFar((void *)menu->infoWindow);
#endif
        Text_DrawWorkValueWithLabel(menu->infoWindow);
        shown = 1;
      } else {
        GameFlag_ClearFar(FLAG_MENU_INFO_KEPT);
      }
    }
    MenuCursor_Position(selection * 24 - 10, 16);
    WaitTicks(1);
    if (gKeysPressed & KEY_A) {
      if (menu->psynergyCount) {
        Sound_PlaySongFar(SE_MENU_CONFIRM);
        result = memberIds[selection];
        break;
      }
      Sound_PlaySongFar(SE_MENU_ERROR);
    }
    // L or R: set that shortcut, if the party member has field Psynergy for it.
    if ((gKeysPressed & KEY_L) || (gKeysPressed & KEY_R)) {
      result = memberIds[selection];
      if (gKeysPressed & KEY_L)
        menu->mode = PSYNERGY_MODE_SET_L;
      else
        menu->mode = PSYNERGY_MODE_SET_R;
      actions = Alloc(32 * sizeof(u16));
      fieldCount = PsynergyMenu_CollectActions((struct Unit *)unit, actions,
                                               PSYNERGY_LIST_SHORTCUTS);
      Free(actions);
      shortcutCount = fieldCount;
      if (shortcutCount == 0) {
        menu->mode = shortcutCount;
        Sound_PlaySongFar(SE_MENU_ERROR);
      } else {
        Sound_PlaySongFar(SE_MENU_CONFIRM);
        break;
      }
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gKeysRepeat & KEY_LEFT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      selection--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      selection++;
      redraw = 1;
    }
  }
  menu->tabIndex[0] = selection;
  menu->selectedMember = memberIds[selection];
  menu->memberIds[0] = memberIds[selection];
  return result;
}

/// Shows party member memberId's Psynergy icons in the Psynergy window, eight
/// to a row, or "no Psynergy" when they have none.
void PsynergyMenu_RefreshMemberPsynergy(s32 memberId) {
  u16 *psynergies;
  struct PsynergyMenuWork *menu;
  struct Unit *member;

  menu = gMenuWork;
  member = Unit_GetFar(memberId);
  psynergies = menu->psynergies;
  menu->psynergyCount =
      PsynergyMenu_CollectActions(member, psynergies, PSYNERGY_LIST_FIELD);
  WindowSprite_RedrawSavedRectFar((void *)menu->psynergyWindow);
  PsynergyMenu_RefreshMemberEntries(108, 32, 8);
  PsynergyMenu_DrawPsynergyIcons(psynergies);
  if (menu->psynergyCount == 0) {
    Text_DrawMessageAsTilesFar(MsgPsynergyMenuEmpty, menu->psynergyWindow, 0,
                               24);
  }
}

UNUSED s32 PsynergyMenu_ReturnTrue(void) { return 1; }

UNUSED void PsynergyMenu_NoOp(void) {}

/// Chooses the party member to use the selected Psynergy on. In mode 0 each
/// party member's status shows as the cursor passes, with the Psynergy's
/// description once. Returns the chosen party member's id, or -1 when the
/// player backs out; also records the choice in the work's tabIndex[1] and
/// memberIds[1].
s32 PsynergyMenu_SelectTarget(s32 targetMode) {

  struct PsynergyMenuWork *menu;
  s32 selection;
  s32 count;
  s32 redraw;
  s32 result;
  s32 shown;
  struct WindowSprite *marker;

  menu = gMenuWork;
  selection = menu->tabIndex[1];
  count = menu->memberCount;
  redraw = 1;
  result = 0;
  shown = 0;
  Unit_GetFar(menu->memberTable[menu->tabIndex[0]]);
  MenuCursor_Slide(selection * 24 - 10, 16);
  while (!GameFlag_GetFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      selection = (selection + count) % count;
      Unit_GetFar(menu->memberTable[selection]);
      // The marker over the party member, three tiles a party member. Nothing
      // in TBS stores sideIcon[1], so it is still NULL from
      // Window_InitializeWork: these writes go to the BIOS area at address 0,
      // which ignores them, and no marker is drawn.
      marker = menu->sideIcon[1];
      marker->oam.x = marker->x =
          (((menu->auxiliaryWindow)->x + selection * 3) << 3) - 2;
      if (targetMode == 0) {
        StatusPanel_Draw(menu->statusWindow, menu->memberTable[selection], 0,
                         STATUS_PANEL_EXPERIENCE);
        PsynergyMenu_CallIconRoutineWithValue(menu,
                                              menu->memberTable[selection]);
        if (!GameFlag_GetFar(FLAG_MENU_INFO_KEPT) && !shown) {
          MENU_RESET_DESCRIPTION_WINDOW((void *)menu->infoWindow);
          MENU_DRAW_DESCRIPTION((menu->sideAction[0] & PSYNERGY_ABILITY_MASK) +
                                    MsgAbilityDescription,
                                menu->infoWindow, 0, 0);
          shown = 1;
        } else {
          GameFlag_ClearFar(FLAG_MENU_INFO_KEPT);
        }
      }
    }
    MenuCursor_Position(selection * 24 - 10, 16);
    WaitTicks(1);
    if (gKeysPressed & KEY_A) {
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      result = menu->memberTable[selection];
      break;
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gKeysRepeat & KEY_LEFT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      selection--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      selection++;
      redraw = 1;
    }
  }
  // The marker is NULL here too (see above).
  marker = menu->sideIcon[1];
  menu->tabIndex[1] = selection;
  Icon_PrepareActor(marker);
  marker->motion = ICON_HIDDEN;
  WaitTicks(1);
  menu->tabIndex[1] = selection;
  menu->selectedMember = menu->memberTable[selection];
  menu->memberIds[1] = menu->memberTable[selection];
  return result;
}

/// A Psynergy shortcut: its party member above bit 10, the Psynergy's id below.
#define PSYNERGY_SHORTCUT_MEMBER_SHIFT 10
#define PSYNERGY_SHORTCUT_ID_MASK 0x3ff

/// Sets the L or R Psynergy shortcut (enum PsynergyShortcut):
/// memberId << 10 | the Psynergy's id. Returns 1.
s32 PsynergyMenu_SetShortcut(s32 memberId, s32 abilityId, s32 shortcutIndex) {
  s32 id = abilityId & PSYNERGY_ABILITY_MASK;
  s32 code = (memberId << PSYNERGY_SHORTCUT_MEMBER_SHIFT) | id;

  if (shortcutIndex == 0) {
    gCommon.firstShortcut = code;
  } else {
    gCommon.secondShortcut = code;
  }
  return 1;
}

// Writes the L and R Psynergy shortcuts into window: each one's Psynergy and
// party member, or that it is empty, under a heading that says how to change
// them. A name too wide for the line (measured as a window, over 10 tiles; 11
// in Japanese) hides the party member (international) or drops the colon after
// "L" and "R" (Japanese). Returns 1.
#if LANGUAGE != LANGUAGE_JAPANESE

s32 PsynergyMenu_DrawShortcuts(struct Window *window, s32 memberId) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  s32 wide;

  if (gCommon.firstShortcut != 0 && gCommon.secondShortcut != 0)
    Text_DrawMessageAsTilesFar(MsgShortcutChangeHelp, window, 0, -8);
  else
    Text_DrawMessageAsTilesFar(MsgShortcutLabel, window, 0, -8);

  Text_GetMessageSizeFar((gCommon.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
                             MsgAbilityName,
                         &left, &top, &width, &height);
  if (width < 0 || width > 10)
    wide = 1;
  else
    wide = 0;
  if (gCommon.firstShortcut != 0) {
    MessageArgument_PushFar(gCommon.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK,
                            MESSAGE_ARGUMENT_ABILITY);
    Text_DrawMessageAsTilesFar(MsgShortcutNameL, window, 0, 0);
    if (wide == 0)
      Text_DrawStringAtOffsetFar(
          Unit_GetFar(gCommon.firstShortcut >> PSYNERGY_SHORTCUT_MEMBER_SHIFT)
              ->name,
          window, 80, 0);
  } else {
    Text_DrawMessageAsTilesFar(MsgShortcutEmptyL, window, 0, 0);
  }

  Text_GetMessageSizeFar((gCommon.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
                             MsgAbilityName,
                         &left, &top, &width, &height);
  if (width < 0 || width > 10)
    wide = 1;
  else
    wide = 0;
  if (gCommon.secondShortcut != 0) {
    MessageArgument_PushFar(gCommon.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK,
                            MESSAGE_ARGUMENT_ABILITY);
    Text_DrawMessageAsTilesFar(MsgShortcutNameR, window, 0, 8);
    if (wide == 0)
      Text_DrawStringAtOffsetFar(
          Unit_GetFar(gCommon.secondShortcut >> PSYNERGY_SHORTCUT_MEMBER_SHIFT)
              ->name,
          window, 80, 8);
    Text_SetColorFar(TEXT_COLOR_NORMAL);
  } else {
    Text_DrawMessageAsTilesFar(MsgShortcutEmptyR, window, 0, 8);
  }
  return 1;
}
#else

s32 PsynergyMenu_DrawShortcuts(struct Window *window, s32 member) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  u16 wide;

  if (gCommon.firstShortcut != 0 && gCommon.secondShortcut != 0)
    Text_DrawMessageAsTilesFar(MsgShortcutChangeHelp, window, 0, -8);
  else
    Text_DrawMessageAsTilesFar(MsgShortcutLabel, window, 0, -8);

  Text_GetMessageSizeFar((gCommon.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
                             MsgAbilityName,
                         &left, &top, &width, &height);
  if (width < 0 || width > 11)
    wide = 1;
  else
    wide = 0;
  if (wide)
    Text_DrawStringInWindowFar((u8 *)gMenu_ShortcutLString, window, 0, 0);
  else
    Text_DrawStringInWindowFar((u8 *)gMenu_ShortcutLColonString, window, 0, 0);
  if (gCommon.firstShortcut != 0) {
    if (wide)
      Text_DrawMessageAsTilesFar(
          (gCommon.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
          window, 8, 0);
    else
      Text_DrawMessageAsTilesFar(
          (gCommon.firstShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
          window, 16, 0);
    Text_DrawStringAtOffsetFar(
        Unit_GetFar(gCommon.firstShortcut >> PSYNERGY_SHORTCUT_MEMBER_SHIFT)
            ->name,
        window, 80, 0);
  } else {
    Text_DrawMessageAsTilesFar(MsgShortcutEmptyL, window, 24, 0);
  }

  Text_GetMessageSizeFar((gCommon.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) +
                             MsgAbilityName,
                         &left, &top, &width, &height);
  if (width < 0 || width > 11)
    wide = 1;
  else
    wide = 0;
  if (wide)
    Text_DrawStringInWindowFar((u8 *)gMenu_ShortcutRString, window, 0, 8);
  else
    Text_DrawStringInWindowFar((u8 *)gMenu_ShortcutRColonString, window, 0, 8);
  if (gCommon.secondShortcut != 0) {
    if (wide)
      Text_DrawMessageAsTilesFar(
          (gCommon.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
          window, 8, 8);
    else
      Text_DrawMessageAsTilesFar(
          (gCommon.secondShortcut & PSYNERGY_SHORTCUT_ID_MASK) + MsgAbilityName,
          window, 16, 8);
    Text_DrawStringAtOffsetFar(
        Unit_GetFar(gCommon.secondShortcut >> PSYNERGY_SHORTCUT_MEMBER_SHIFT)
            ->name,
        window, 80, 8);
    Text_SetColorFar(TEXT_COLOR_NORMAL);
  } else {
    Text_DrawMessageAsTilesFar(MsgShortcutEmptyR, window, 24, 8);
  }
  return 1;
}
#endif

/// Sets up a Psynergy grid screen: the cursor window with the party's figures,
/// a 30 by 15 window below with a hidden grid cursor and a portrait, and 16
/// Psynergy icon sprites in two rows of eight from (96, 8), on an 8 by 2 grid.
/// Nothing calls it or PsynergyMenu_CloseWindows: the Psynergy screen shows a
/// paged list instead.
void PsynergyMenu_CreateEntryGrid(void) {
  struct Window *window;
  struct PsynergyMenuWork *menu;
  struct WindowSprite *cursor;
  s32 index;
  s32 x;
  s32 y;
  struct WindowSprite **sprite;

  menu = gMenuWork;
  window = MenuCursor_Create(menu);
  PsynergyMenu_InitializeEntryActors((void *)window, 2, 2, 8, 0);

  window = Window_CreateFar(0, 5, 30, 15, WINDOW_FLAG_INSTANT);
  menu->psynergyWindow = window;
  menu->selectedColumn = 0;
  menu->selectedRow = 0;
  menu->columnCount = 8;
  menu->rowCount = 2;

  cursor = (struct WindowSprite *)Icon_CreateWithResourceVariant(window, 0, 4);
  cursor->motion = ICON_HIDDEN;
  menu->entryGridCursor = cursor;
  Portrait_CreateFar(0, 0, 0, window, 0, 0);

  y = 8;
  index = 0;
  sprite = &menu->entryIcons[0];
  x = 96;
  do {
    *sprite++ = WindowSprite_CreateFromResourceFar(ICON_KIND_ABILITY, index,
                                                   window, x, y);
    index++;
    x += 16;
  } while (index <= 7);

  index = 8;
  y = 24;
  sprite = &menu->entryIcons[8];
  x = 96;
  do {
    *sprite++ = WindowSprite_CreateFromResourceFar(ICON_KIND_ABILITY, index,
                                                   window, x, y);
    index++;
    x += 16;
  } while (index <= 15);
}

/// Releases the party figures and closes the Psynergy grid screen's windows.
/// Nothing calls it.
void PsynergyMenu_CloseWindows(void) {
  struct PsynergyMenuWork *menu;

  menu = gMenuWork;
  Menu_ReleaseEntryActors();
  Window_CloseFar((void *)menu->auxiliaryWindow, 1);
  Window_CloseFar((void *)menu->psynergyWindow, 1);
  Window_CloseFar((void *)menu->messageWindow, 1);
}

/// Loads each listed Psynergy's icon into its icon sprite (32 entries) and
/// hides the icons of empty entries.
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies) {
  s32 remaining;
  struct WindowSprite **icons;
  u16 *p;
  s32 psynergyId;

  icons = ((struct PsynergyMenuWork *)gMenuWork)->entryIcons;
  p = psynergies;
  remaining = 31;
  do {
    psynergyId = *p++;
    if (psynergyId != 0) {
      Resource_LoadByModeIntoSlotFar(ICON_KIND_ABILITY, psynergyId,
                                     (u8)(*icons)->index, 0);
    }
    icons++;
    remaining--;
  } while (remaining >= 0);
  Menu_HideEmptyEntryIcons(psynergies);
}

/// Lists unit's Psynergy into actions (32 entries, cleared first) and returns
/// how many there are, as mode (enum PsynergyListMode) chooses; any mode
/// other than PSYNERGY_LIST_SHORTCUTS and PSYNERGY_LIST_FIELD lists as
/// PSYNERGY_LIST_ALL.
s32 PsynergyMenu_CollectActions(struct Unit *unit, u16 *actions, s32 mode) {
  s32 clearIndex;
  s32 passCount;
  s32 actionCount;
  s32 slotIndex;
  s32 passIndex;

  passCount = (mode != PSYNERGY_LIST_FIELD) ? 4 : 3;
  for (clearIndex = 0; clearIndex < 32; clearIndex++)
    actions[clearIndex] = 0;
  actionCount = 0;

  // fieldPsynergy is the Psynergy's field effect. Passes 1 and 2 add nothing.
  if (mode == PSYNERGY_LIST_SHORTCUTS) {
    for (slotIndex = 0; slotIndex < 32; slotIndex++) {
      if (unit->psynergy[slotIndex].encodedAction != 0) {
        if (Ability_Get(unit->psynergy[slotIndex].encodedAction &
                        PSYNERGY_ABILITY_MASK)
                ->fieldPsynergy != 0) {
          actions[actionCount] = unit->psynergy[slotIndex].encodedAction;
          actionCount++;
        }
      }
    }
  } else {
    for (passIndex = 0; passIndex < passCount; passIndex++) {
      for (slotIndex = 0; slotIndex < 32; slotIndex++) {
        if (unit->psynergy[slotIndex].encodedAction != 0) {
          struct AbilityDefinition *ability = Ability_Get(
              unit->psynergy[slotIndex].encodedAction & PSYNERGY_ABILITY_MASK);

          if (passIndex == 0 &&
              (ability->fieldPsynergy != 0 ||
               (ability->flags & ABILITY_USABLE_IN_FIELD) != 0)) {
            actions[actionCount] = unit->psynergy[slotIndex].encodedAction;
            actionCount++;
          } else if (passIndex == 1) {
          } else if (passIndex == 2) {
          } else if (passIndex == 3 && ability->fieldPsynergy == 0 &&
                     (ability->flags & ABILITY_USABLE_IN_FIELD) == 0) {
            actions[actionCount] = unit->psynergy[slotIndex].encodedAction;
            actionCount++;
          }
        }
      }
    }
  }

  return actionCount;
}

/// Fills result with the Psynergy list of the party member in side index: their
/// unit, how many Psynergy are listed, and the page and row of the one chosen
/// last for them (kept within the list), five to a page. Returns 1.
s32 PsynergyMenu_BuildPagedList(struct PagedList *result, s32 index) {
  s32 unitBits;
  struct PsynergyMenuWork *menu = gMenuWork;
  s32 entryCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selectedIndex;
  s32 memberId;

  unitBits = (s32)Unit_GetFar(menu->memberIds[index]);
  entryCount = menu->psynergyCount;
  memberId = menu->memberIds[index];
  selectedIndex = menu->selectedIndexByMember[memberId];
  if ((selectedIndex + 1) > entryCount) {
    selectedIndex = entryCount - 1;
  }
  page = selectedIndex / LIST_PAGE_SIZE;
  row = selectedIndex % LIST_PAGE_SIZE;
  pageCount = entryCount / LIST_PAGE_SIZE;
  if (entryCount % LIST_PAGE_SIZE != 0) {
    pageCount++;
  }
  result->memberState = unitBits;
  result->page = page;
  result->pageCount = pageCount;
  result->row = row;
  result->entryCount = entryCount;
  result->selectedIndex = selectedIndex;
  return 1;
}

/// PsynergyMenuWork.flags: the Psynergy list shows the shortcut help (Select
/// held).
#define PSYNERGY_MENU_SHORTCUT_HELP 2

/// Updates the Psynergy list for the row under the cursor: puts the Psynergy's
/// description in the info window, unless a kept message is there, and
/// highlights its row of the five. window and work are not used.
s32 PsynergyMenu_DrawDetailPage(struct Window *window, s32 *work,
                                struct PagedList *state) {
  struct PsynergyMenuWork *menu;
  s32 actionIndex;
  s32 rowIndex;

  menu = gMenuWork;
  actionIndex = state->page * LIST_PAGE_SIZE;
  actionIndex += state->row;
  state->selectedIndex = actionIndex;

  if (GameFlag_GetFar(FLAG_MENU_INFO_KEPT) == 0) {
    MENU_RESET_DESCRIPTION_WINDOW(menu->infoWindow);
    WaitTicks(1);

    actionIndex = state->selectedIndex;
    if (menu->psynergies[actionIndex] != 0) {
      s32 description =
          (menu->psynergies[actionIndex] & 0x1ff) + MsgAbilityDescription;
      MENU_DRAW_DESCRIPTION(description, menu->infoWindow, 0, 0);
    }
  } else {
    // BUG: clears FLAG_SCENE_TEMP_FF instead of FLAG_MENU_INFO_KEPT, so a kept
    // message keeps the descriptions away until another screen clears it.
#ifdef BUGFIX
    GameFlag_ClearFar(FLAG_MENU_INFO_KEPT);
#else
    GameFlag_ClearFar(FLAG_SCENE_TEMP_FF);
#endif
  }

  rowIndex = 0;
  do {
    if (rowIndex == state->row) {
      Render_SetTilemapFlagRect(
          (void *)menu->psynergyWindow, PSYNERGY_DETAIL_ROW_X, rowIndex * 2 + 1,
          PSYNERGY_DETAIL_ROW_WIDTH, 1, HIGHLIGHT_PALETTE);
    } else {
      Render_SetTilemapFlagRect((void *)menu->psynergyWindow,
                                PSYNERGY_DETAIL_ROW_X, rowIndex * 2 + 1,
                                PSYNERGY_DETAIL_ROW_WIDTH, 1, WINDOW_PALETTE);
    }
    rowIndex++;
  } while (rowIndex < LIST_PAGE_SIZE);

  WaitTicks(1);
  return 1;
}

/// Draws one page of the Psynergy list into window: up to five names with their
/// icons and PP costs, Psynergy the party member cannot afford in the no-HP
/// colour and one restricted here in the low-HP colour, the page numbers, and
/// the help line (how to set a shortcut while Select is held, else how to
/// change party member).
s32 PsynergyMenu_DrawActionPage(struct Window *window, s32 unused,
                                const struct PagedList *pageState) {
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  s32 cursor;
  struct Unit *member;
  struct AbilityDefinition *ability;
  struct PsynergyMenuWork *menu = gMenuWork;

  (void)unused;

  WindowSprite_RedrawSavedRectFar((void *)window);
  Window_DrawDividerLineFar(window, 0, 11, 16, 11);

  if (PSYNERGY_MENU_SHORTCUT_HELP & menu->flags) {
    Text_DrawMessageAsTilesFar(MsgShortcutHelp, window, 0, 88);
  } else {
    Text_DrawMessageAsTilesFar(MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
  }

  firstEntry = pageState->page * LIST_PAGE_SIZE;
  visibleCount = (u8)(pageState->entryCount - firstEntry);
  if (visibleCount > LIST_PAGE_SIZE) {
    visibleCount = LIST_PAGE_SIZE;
  }

  Menu_SetPageIcons(LIST_PAGE_SIZE, firstEntry, window, PSYNERGY_PAGE_ICONS_X,
                    34);
  PagedList_DrawPageNumbers(window, pageState->entryCount, LIST_PAGE_SIZE,
                            pageState->page, 15);
  Text_DrawMessageAsTilesFar(MsgPsynergyPp, window, PSYNERGY_PP_LABEL_X, 0);

  row = 0;
  if (visibleCount > row) {
    cursor = firstEntry;
    do {
      member = Unit_GetFar(menu->memberIds[0]);
      ability = Ability_Get(PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]);

      if (ability->ppCost > member->pp) {
        Text_SetColorFar(TEXT_COLOR_NO_HP);
      } else if (PsynergyMenu_IsActionRestricted(
                     PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]) != 0) {
        Text_SetColorFar(TEXT_COLOR_LOW_HP);
      } else {
        Text_SetColorFar(TEXT_COLOR_NORMAL);
      }

      Text_DrawMessageAsTilesFar(
          (PSYNERGY_ABILITY_MASK & menu->psynergies[cursor]) + MsgAbilityName,
          window, PSYNERGY_NAME_X, row * 16 + 8);
      Text_DrawNumberAtOffsetFar(ability->ppCost, 2, window, 104, row * 16 + 8);
      Text_SetColorFar(TEXT_COLOR_NORMAL);

      row++;
      cursor++;
    } while (visibleCount > row);
  }

  return 1;
}

/// The Psynergy list of the party member in sideIndex, five a page. The
/// cursor's Psynergy is described in the info window and its icon animates; L
/// and R switch to the previous or next party member who has Psynergy to list.
/// In PSYNERGY_MODE_USE, A chooses Psynergy the party member can use and
/// afford; holding Select shows the shortcut help, and L or R with Select set
/// the Psynergy as that shortcut. In the shortcut modes only field Psynergy are
/// listed and A chooses one. Returns the chosen Psynergy, or -1 when the player
/// backs out.
s32 PsynergyMenu_RunList(s32 sideIndex) {
  struct PsynergyMenuWork *menu;
  struct AbilityDefinition *ability;
  struct WindowSprite *icon;
  struct Window *window;
  s32 changed;
  s32 moved;
  s8 listMode;
  s32 member;
  u8 i;
  s32 result;
  s32 redraw;
  s32 done;
  struct Unit *memberUnit;
  s32 previous;
  s32 shortcutHelp;
  s32 work[5];
  struct PagedList state;

  menu = gMenuWork;
  result = 0;
  previous = 0;
  shortcutHelp = 0;
  menu->sideIcon[sideIndex]->motion = ICON_HIDDEN;
  Window_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, 2);
  window = menu->listWindow;
  done = 0;

  while (done == 0 && GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
    memberUnit = Unit_GetFar(menu->memberIds[sideIndex]);
    if (menu->mode != PSYNERGY_MODE_USE) {
      menu->psynergyCount = PsynergyMenu_CollectActions(
          memberUnit, menu->psynergies, PSYNERGY_LIST_SHORTCUTS);
    } else {
      menu->psynergyCount = PsynergyMenu_CollectActions(
          memberUnit, menu->psynergies, PSYNERGY_LIST_FIELD);
    }
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
    PsynergyMenu_BuildPagedList(&state, sideIndex);
    redraw = 1;
    changed = 1;
    menu->sideIcon[sideIndex]->motion = ICON_SHOWN;

    while (GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
      MenuCursor_Position(PSYNERGY_LIST_CURSOR_X, state.row * 16 + 36);

      if (changed != 0) {
        changed = 0;
        if (menu->psynergies[previous] != 0) {
          icon = menu->entryIcons[previous];
          Icon_PrepareActor(icon);
        }
        if (redraw != 0) {
          redraw = 0;
          WaitTicks(1);
          PsynergyMenu_DrawActionPage(window, 0, &state);
        }
        PsynergyMenu_DrawDetailPage(window, work, &state);
        menu->sideAction[sideIndex] = menu->psynergies[state.selectedIndex];
        menu->cursorIcon->motion = ICON_HIDDEN;
        if (menu->psynergies[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          // Animate the selected icon: 9 is WINDOW_SPRITE_MOTION_SWELL, in
          // depth slot 250, as in ItemMenu_RunList.
          icon->motion = 9;
          icon->animationStep = 0;
          icon->sentinel = 250;
        }
        for (i = 0; i < menu->memberCount; i++) {
          Sprite_StartAnimFar(menu->tabSprites[i], 1);
        }
      }

      WaitTicks(1);
      previous = state.selectedIndex;

      if ((gKeysHeld & KEY_SELECT) == 0) {
        moved = PagedList_HandleInput(0, state.entryCount, LIST_PAGE_SIZE,
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
      if (menu->mode == PSYNERGY_MODE_USE) {
        if ((gKeysPressed & KEY_SELECT) != 0 && shortcutHelp == 0) {
          ability = Ability_Get(menu->psynergies[state.selectedIndex] &
                                PSYNERGY_ABILITY_MASK);
          if (ability->fieldPsynergy == 0) {
            Sound_PlaySongFar(SE_MENU_ERROR);
          } else {
            Sound_PlaySongFar(SE_NOTE_MID); // the Use sound
            shortcutHelp = 1;
            menu->flags |= PSYNERGY_MENU_SHORTCUT_HELP;
            Window_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
            Text_DrawMessageAsTilesFar(MsgShortcutHelp, window, 0, 88);
          }
        }
        if ((gKeysHeld & KEY_SELECT) == 0 && shortcutHelp == 1) {
          shortcutHelp = 0;
          menu->flags &= ~PSYNERGY_MENU_SHORTCUT_HELP;
          Window_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
          Text_DrawMessageAsTilesFar(MsgChangeCharacterHelp, window,
                                     HELP_TEXT_X, 88);
        }
      }

      // A: set the shortcut, or use the Psynergy if allowed
      // and affordable.
      if ((gKeysPressed & KEY_A) != 0) {
        if (menu->mode != PSYNERGY_MODE_USE) {
          Sound_PlaySongFar(SE_PSYNERGY_START); // the shortcut sound
          result = menu->psynergies[state.selectedIndex];
          done = 1;
          break;
        }
        if (menu->psynergies[state.selectedIndex] != 0) {
          if (PsynergyMenu_IsActionRestricted(
                  menu->psynergies[state.selectedIndex]) != 0) {
            Sound_PlaySongFar(SE_MENU_ERROR);
            continue;
          }
          ability = Ability_Get(menu->psynergies[state.selectedIndex] &
                                PSYNERGY_ABILITY_MASK);
          if (ability->ppCost > memberUnit->pp) {
            Sound_PlaySongFar(SE_MENU_ERROR);
          } else {
            Sound_PlaySongFar(SE_NOTE_LOW); // the choice sound
            result = menu->psynergies[state.selectedIndex];
            done = 1;
            break;
          }
        }
      }

      if ((gKeysPressed & KEY_B) != 0) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        break;
      }

      // L and R: the previous or next party member with Psynergy to list.
      if (((gKeysRepeat & KEY_R) != 0 || (gKeysRepeat & KEY_L) != 0) &&
          (gKeysHeld & KEY_SELECT) == 0) {
        listMode = menu->mode != PSYNERGY_MODE_USE ? PSYNERGY_LIST_SHORTCUTS
                                                   : PSYNERGY_LIST_FIELD;
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        menu->selectedIndexByMember[menu->memberIds[sideIndex]] =
            state.selectedIndex;
        member = menu->tabIndex[sideIndex];
        do {
          if ((gKeysRepeat & KEY_R) != 0) {
            member++;
          } else {
            member--;
          }
          member = (member + menu->memberCount) % menu->memberCount;
          menu->selectedMember = menu->memberTable[member];
          menu->memberIds[0] = menu->memberTable[member];
          menu->psynergyCount = PsynergyMenu_CollectActions(
              Unit_GetFar(menu->memberIds[0]), menu->psynergies, listMode);
        } while (menu->psynergyCount == 0);
        menu->tabIndex[sideIndex] = member;
        for (i = 0; i < 4; i++) {
          menu->rowPositions[i] = 30;
        }
        menu->rowPositions[member] = 26;
        StatusPanel_Draw(menu->statusWindow, menu->memberTable[member], 0,
                         STATUS_PANEL_EXPERIENCE);
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->memberTable[member]);
        break;
      }

      // Select + L or R: make the field Psynergy that shortcut.
      if ((gKeysPressed & KEY_L) != 0 && (gKeysHeld & KEY_SELECT) != 0) {
        ability = Ability_Get(menu->psynergies[state.selectedIndex] &
                              PSYNERGY_ABILITY_MASK);
        if (ability->fieldPsynergy == 0) {
          Sound_PlaySongFar(SE_MENU_ERROR);
        } else {
          Sound_PlaySongFar(SE_PSYNERGY_START); // the shortcut sound
          if (PsynergyMenu_SetShortcut(menu->memberIds[sideIndex],
                                       menu->psynergies[state.selectedIndex],
                                       0) != 0) {
            result = menu->psynergies[state.selectedIndex];
            menu->mode = PSYNERGY_MODE_SET_L;
            done = 1;
            break;
          }
        }
      }

      if ((gKeysPressed & KEY_R) != 0 && (gKeysHeld & KEY_SELECT) != 0) {
        ability = Ability_Get(menu->psynergies[state.selectedIndex] &
                              PSYNERGY_ABILITY_MASK);
        if (ability->fieldPsynergy == 0) {
          Sound_PlaySongFar(SE_MENU_ERROR);
        } else {
          if (PsynergyMenu_SetShortcut(menu->memberIds[sideIndex],
                                       menu->psynergies[state.selectedIndex],
                                       1) != 0) {
            Sound_PlaySongFar(SE_PSYNERGY_START); // the shortcut sound
            result = menu->psynergies[state.selectedIndex];
            menu->mode = PSYNERGY_MODE_SET_R;
            done = 1;
            break;
          }
        }
      }
    }
  }

  menu->flags &= ~PSYNERGY_MENU_SHORTCUT_HELP;
  Icon_PrepareActor(menu->entryGridCursor);
  menu->sideRow[sideIndex] = state.selectedIndex;
  menu->selectedIndexByMember[menu->memberIds[sideIndex]] = state.selectedIndex;
  menu->sideAction[sideIndex] = result;
  if (GameFlag_GetFar(FLAG_MENU_START_PRESSED) != 0) {
    result = -1;
  }
  WaitTicks(1);
  return result;
}

/// Returns 1 when actionEntry cannot be used from the menu: it has no field
/// effect and is not usable both in the field and in battle. The Psynergy list
/// draws such Psynergy in the low-HP colour and refuses A on it.
s32 PsynergyMenu_IsActionRestricted(s32 actionEntry) {
  // Mask the unsigned entry without overflowing a signed left shift.
  struct AbilityDefinition *action =
      Ability_Get(((u32)actionEntry << 18) >> 18);

  if (action->fieldPsynergy != 0 ||
      (action->flags & (ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE)) ==
          (ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE))
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
#undef PSYNERGY_SHORTCUT_MEMBER_SHIFT
#undef PSYNERGY_SHORTCUT_ID_MASK
#undef PSYNERGY_MENU_SHORTCUT_HELP

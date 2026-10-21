/// @file
/// Collecting the party's battle commands.
// Part of battle_command.c.

#include "../battle_command_internal.h"

/// Classifies ability `act` for the target selection (the `kind` of
/// BattleTarget_RunSelection, enum BattleTargetHint), which shows what it
/// would do on a party member and, for the curing and reviving hints, starts
/// on one who needs it. A later test wins: an effect over the ability
/// kind, revival over all.
s32 Battle_ClassifyEntryKind(const struct BattleAct *act) {
  s32 signedEffect;
  s32 abilityKind;
  s32 targetKind;
  u8 effect;

  targetKind = TARGET_HINT_NONE;
  abilityKind = act->flags & ABILITY_KIND_MASK;
  if (abilityKind == ABILITY_KIND_HEAL_HP)
    targetKind = TARGET_HINT_HP;
  if (abilityKind == ABILITY_KIND_HEAL_PP)
    targetKind = TARGET_HINT_PP;
  signedEffect = (s8)act->effect;
  effect = signedEffect;
  if (effect == ABILITY_EFFECT_CURE_POISON)
    targetKind = TARGET_HINT_CURE_POISON;
  if (effect == ABILITY_EFFECT_RESTORE)
    targetKind = TARGET_HINT_RESTORE;
  if (effect == ABILITY_EFFECT_CURE_ALL)
    targetKind = TARGET_HINT_CURE_ALL;
  if (Battle_IsReviveEffectFar(act->effect) != 0)
    targetKind = TARGET_HINT_REVIVE;
  return targetKind;
}

/// Collects the party's commands for a round into `commands`, one
/// for each of the `commandCount` units in `actors` (a member with two
/// actions is listed twice), and returns how many it wrote: commandCount, 1
/// when the party flees (CMD_FLEE for the first actor), or -1 when the link
/// fails. Each record gets its command (CMD_), parameter (the ability, the
/// summon, the Djinni entry or the inventory place), target
/// (BattleTarget_RunSelection's code), range and priority: the unit's
/// Agility plus up to a sixteenth more at random, halved for its second
/// action in a row. Attack keeps the parameter, and Defend the parameter and
/// target, that the command chosen before it left (0 and 256 at first).
///
/// It runs the party's row (Fight, Flee, Status), then each member's row and
/// the menus its command opens; B in a menu goes back a step, B on a
/// member's row to the member before, and from the first back to the
/// party's row. In a link battle both consoles first wait for each other
/// ("VSST" in the link signature), publish "EDVS" when done, and wait for
/// the partner to finish ("Waiting for opponent's input."). Its work,
/// BattleMenuWork (gLinkCountdownWork), lives in heap slot 57 until it
/// returns. BattlePres_BuildUnitEntries calls it (battle/battle/round.c).
s32 Battle_CollectPartyCommands(struct BattleActionRecord *commands,
                                u16 *actors, s32 commandCount) {
  struct BattleMenuWork *state;
  struct BattleWork *battle;
  struct BattleWork *linkBattle;
  u16 *peerSignature;
  u16 *transferWords;
  struct BattleActionRecord *command;
  struct BattleUnit *unit;
  struct UiWindow *commandWindow;
  struct MenuSprite *iconSprite;
  u16 signatureHeader[2];
  s32 target;
  s32 actorId;
  s32 mode;
  s32 commandKind;
  s32 parameter;
  s32 largeIconSlot;
  s32 smallIconSlot;
  s32 missedLinkFrames;
  s32 commandIndex;
  s32 k;
  s32 priority;

  // An empty ID list clears every party panel's highlight and marks no unit
  // before the next actor is chosen.
  void BattleMenu_ClearPartyHighlights(void) {
    u16 data[2];
    data[0] = 0xff;
    BattleLayout_HighlightPartyPanels(data);
    BattlePres_SetActorModesFar(data, 1);
  }

  // Set up the shared command UI before the optional link handshake: VRAM
  // for the chosen command's 32x32 icon (1 KB) and the member's portrait,
  // the lists' page numbers (tile 0x300), page arrows (0x334) and range
  // icons (0x314), and no Djinni chosen yet (0x80000000).
  target = 256;
  commandKind = 0;
  parameter = 0;
  largeIconSlot = VramCache_AllocEntry(1024);
  smallIconSlot = VramCache_AllocEntry(512);
  state = Runtime_AllocateBlock(57, 260);
  Resource_LoadIndexedIntoBuffer(largeIconSlot, 0);
  Graphics_ExpandVramTilesByColorTable((u16 *)0x06006000);
  Link_DrawShiftedTilePair(0x06006680);
  Menu_BuildLocalizedPatternTiles();
  battle = gBattleWork;
  for (k = 0; k < 8; k++) {
    state->chosenDjinn[k] = 0x80000000;
  }
  for (k = 0; k < 3; k++) {
    state->iconShown[k] = 0;
  }
  state->targetSceneOffset = 0;
  state->sceneOffset = 0;
  state->timer = -1;
  state->commandSelection[0] = 0;
  state->commandSelection[1] = 0;
  state->linkTimer = 0;
  state->timerHidden = 0;
  state->timerWindow = 0;
  // Both peers announce the command-entry phase before accepting commands:
  // this side writes "VSST" into halfwords 4 to 7 of its signature, then
  // waits for the partner's "VSST" (or "EDVS", already done) while its first
  // four read "EXEC". A pause, 25 frames without the link, or a partner not
  // in "EXEC" gives up (-1).
  if (battle->twoSided != 0) {
    state->linkTimer = 1;
    transferWords = (u16 *)gSerialTransfer.payload;
    transferWords[4] = 'V';
    transferWords[5] = 'S';
    transferWords[6] = 'S';
    transferWords[7] = 'T';
    missedLinkFrames = 0;
    for (;;) {
      peerSignature = (u16 *)&gLinkPeerSignatures[battle->linkSide ^ 1];
      if (battle->linkPaused != 0) {
        commandCount = -1;
        state->linkTimer = 0;
        break;
      }
      if ((gLinkStatus & 3) != 3) {
        if (++missedLinkFrames > 24) {
          commandCount = -1;
          state->linkTimer = 0;
          break;
        }
      } else {
        missedLinkFrames = 0;
        if (peerSignature[4] == 'V' && peerSignature[5] == 'S' &&
            peerSignature[6] == 'S' && peerSignature[7] == 'T') {
          break;
        }
        if (peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
            peerSignature[6] == 'V' && peerSignature[7] == 'S') {
          break;
        }
        if (!(peerSignature[0] == 'E' && peerSignature[1] == 'X' &&
              peerSignature[2] == 'E' && peerSignature[3] == 'C')) {
          commandCount = -1;
          state->linkTimer = 0;
          break;
        }
      }
      WaitFrames(1);
    }
  }
  Scheduler_AddOrUpdateCallback(UpdateLinkSessionCountdown, 3200);

  while (1) {
    BattleMenu_ClearPartyHighlights();
    state->iconShown[2] = 0;
    state->commandActor = -1;
    state->tutorialStep = 0;
    if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0) {
      state->tutorialStep = 1;
      state->tutorialDelay = 60;
    }
    // The party's row; with no member to choose, as if Fight were taken.
    if (commandCount > 0) {
      mode = Menu_RunCommandRow(COMMAND_ROW_PARTY);
    } else {
      mode = COMMAND_ICON_FIGHT;
    }
    // Status: the party's status screens, from the first member; the
    // debug build with Start held lists the enemies instead.
    if (mode == COMMAND_ICON_STATUS) {
      void *block;
      s32 selection;
      s32 handle;

      block = MemAlloc(12);
      if (gDebugMode != 0 && (gKeysHeld & KEY_START) != 0) {
        selection = BATTLE_SIDE_ENEMIES;
      } else {
        selection = BATTLE_SIDE_PARTY;
      }
      handle = BattleParty_ListActorIdsFar(selection, block);
      WaitFrames(1);
      Ui_RunOwnerStatusScreen(block, handle, *(u16 *)block);
      MemFree(block);
      continue;
    }
    // Flee, unless the battle forbids it: one CMD_FLEE for the party,
    // first in the round (priority 0x7ffe).
    if (mode == COMMAND_ICON_FLEE) {
      if (UiText_ShowLocalizedMessageAndWait() != 0) {
        continue;
      }
      commandCount = 1;
      command = commands;
      command->unitId = actors[0];
      command->priority = 0x7ffe;
      command->command = CMD_FLEE;
      command->parameter = 0;
      command->target = 256;
      break;
    }
    if (mode != 14) {
      break;
    }
    AudioCommand_PlayFar(SE_CAST);
    commandIndex = 0;
    if (commandIndex >= commandCount) {
      break;
    }

    // Each member in turn. A command starts from the standby Djinn the
    // earlier ones leave, so two summons cannot spend the same Djinn.
    for (;;) {
      if (commandIndex == 0) {
        BattleParty_CountStandbyDjinnFar(0, state->standbyDjinn[0]);
      } else {
        s32 k;

        for (k = 0; k < 4; k++) {
          state->standbyDjinn[commandIndex][k] =
              state->standbyDjinn[commandIndex - 1][k];
        }
      }

      command = &commands[commandIndex];
      actorId = actors[commandIndex];
      // UB: UnitGetFar returns a pointer, not the cast's doubleword result.
#ifdef UBFIX
      unit = UnitGetFar(actorId);
#else
      /* FAKEMATCH(wrapper): called as returning a doubleword, which sets r1,
       * so sched2 loads i * 2 first; as UnitGetFar(actorId), 1 of 1727
       * instructions differ in tbs-en. */
      unit = (struct BattleUnit *)(s32)((s64 (*)(s32))UnitGetFar)(actorId);
#endif
      // The member's row starts on Attack; its portrait shows at the left.
      gLinkCountdownWork->commandActor = actorId;
      gLinkCountdownWork->commandSelection[1] = 0;
      iconSprite = &gLinkCountdownWork->icons[2];
      iconSprite->oam.word.attr01 = 0x80000400;
      iconSprite->oam.word.attr23 = 0;
      iconSprite->oam.f.tile =
          UiGlyph_LoadPortrait(unit->classId, smallIconSlot);
      iconSprite->oam.f.x = BATTLE_CLASS_ICON_X;
      iconSprite->oam.f.y = 0x80;
      iconSprite->oam.f.palette = 14;
      state->iconShown[2] = 1;

    mark_visible:
      UiWindow_MarkVisibleTileAttributes();
      gLinkCountdownWork->iconShown[0] = 0;
      gLinkCountdownWork->chosenDjinn[commandIndex] = 0x80000000;
      /* FAKEMATCH(dead): twelve dead stores restore the native frame's spill
       * slots at sp+24 to sp+32; without them, 26 of 1727 instructions differ
       * in tbs-en. */
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      // signatureHeader is the member alone, 0xff-ended: marked in the
      // battle and highlighted in the party status window.
      signatureHeader[0] = (u16)actorId;
      signatureHeader[1] = 255;
      BattlePres_SetActorModesFar(signatureHeader, 1);
      BattleLayout_HighlightPartyPanels(signatureHeader);
      WaitFrames(1);
      mode = Menu_RunCommandRow(COMMAND_ROW_MEMBER);
      WaitFrames(1);
      if (mode == -2) {
        void *block;
        s32 handle;

        block = MemAlloc(12);
        handle = BattleParty_ListActorIdsFar(1, block);
        state->iconShown[2] = 0;
        WaitFrames(1);
        Ui_RunOwnerStatusScreen(block, handle, actorId);
        state->iconShown[2] = 1;
        MemFree(block);
        continue;
      }
      BattlePres_SetActorModesFar(signatureHeader, 0);
      // B: back to the member before, or from the first to the party's row.
      if (mode == -1) {
        if (commandIndex == 0) {
          break;
        }
        commandIndex--;
        continue;
      }

      // Out of time in a link battle: Defend.
      if (gLinkCountdownWork->timer == 0) {
        mode = COMMAND_ICON_DEFEND;
      }
      iconSprite = &gLinkCountdownWork->icons[0];
      iconSprite->oam.word.attr23 = 0;
      iconSprite->oam.word.attr01 = 0x80002400;
      iconSprite->oam.f.tile =
          Resource_LoadIndexedIntoBuffer(largeIconSlot, mode);
      if (mode == COMMAND_ICON_DJINN) {
        iconSprite->oam.f.x = 128;
      } else {
        iconSprite->oam.f.x = 96;
      }
      iconSprite->oam.f.y = 136;
      state->iconShown[0] = 1;
      // Each command's menus; commandKind is the CMD_ value recorded. B in a
      // target selection goes back to the command's list (for Attack, to the
      // member's row), B in a list to the row.
      switch (mode) {
      case COMMAND_ICON_ATTACK:
        // Attack: one enemy, range 1; CMD_ATTACK.
        commandWindow = UiWindow_Create(11, 17, 11, 3, 6);
        UiText_DrawCharacterAtOffset(MsgCommandName, commandWindow,
                                     BATTLE_COMMAND_NAME_X, 0);
        state->icons[0].oam.f.x = 64;
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(actorId, 1, 1, TARGET_HINT_NONE);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = CMD_ATTACK;
        target = mode;
        command->range = 1;
        break;

      case COMMAND_ICON_PSYNERGY: {
        struct MenuSprite *iconSprite;
        struct BattleAct *ability;
        struct UiRenderWork *ui;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 cnt;
        s32 k;
        s32 entryValue;

        // Psynergy: the member's abilities usable in battle, from its 32
        // action slots (pick keeps each one's slot); then the ability's
        // window, colour 2 when the PP fall short and 9 while sealed, and
        // its target; CMD_PSYNERGY with the ability as parameter.
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = 0;
        BATTLE_MENU_SET_CURSOR(mode, mode, mode);
      psynergy_menu:
        list = state->listAbilities;
        pick = state->listSlots;
        state->icons[0].oam.f.x = 48;
        cnt = 0;
        k = 0;
        entryValue = unit->actionSlots[k].encodedAction & ACTION_ID_MASK;
        if (entryValue != 0) {
          do {
            if ((BattleActGet(entryValue)->flags & 0x80) != 0) {
              pick[cnt] = k;
              list[cnt] = entryValue;
              cnt++;
            }
            k++;
            if (k == 32) {
              break;
            }
            entryValue = unit->actionSlots[k].encodedAction & ACTION_ID_MASK;
          } while (entryValue != 0);
        }
        pick[cnt] = 0;
        list[cnt] = 0;
        mode = BattleMenu_RunActionSelection(actorId, list, cnt);
        if (mode == -1) {
          goto mark_visible;
        }
        parameter =
            unit->actionSlots[pick[mode]].encodedAction & ACTION_ID_MASK;
        ability = BattleActGet(parameter);
        // cost holds the ability's range, not its PP.
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        ui = UI_WORK;
        commandWindow = UiWindow_Create(8, 17, 18, 3, 6);
        state->icons[0].oam.f.x = 40;
        iconSprite->oam.word.attr01 = 0x40000400;
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
        /* FAKEMATCH(barrier): counted as 4 bytes, the empty asm puts the pool
         * after mov r1, r10 as native (constant 40); without it, 13 of 1727
         * instructions differ in tbs-en. */
        __asm__ volatile("");
        iconSprite->oam.f.tile =
            Resource_LoadIndexedEntryToBuffer(parameter, handle);
        state->iconShown[1] = 1;
        ui->level = 5;
        if ((s32)ability->ppCost > (s32)unit->pp) {
          UiWork_SetParamNibble(2);
        } else if (unit->psySeal != 0) {
          UiWork_SetParamNibble(9);
        }
        UiText_DrawCharacterAtOffset(parameter + MsgAbilityName, commandWindow,
                                     16, 0);
        UiText_DrawNumberAtOffset(ability->ppCost, 2, commandWindow, 104, 0);
        ui->level = 15;
        UiWork_SetParamNibble(15);
        // 61471 is 0xf01f, the PP label's first tile; 20481 below is 0x5001,
        // SUMMON_ELEMENT_TILE.
        UiWindow_SetTilemapEntry(commandWindow, 61471, 11, 0, 0);
        UiWindow_SetTilemapEntry(commandWindow, PSYNERGY_COST_SECOND_TILE, 12,
                                 0, 0);
        if (ability->element != ELEMENT_NONE) {
          UiWindow_SetTilemapEntry(commandWindow, ability->element + 20481, 15,
                                   0, 0);
        }
        command->range = cost;
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(
            actorId, ability->targetMode, cost,
            Battle_ClassifyEntryKind((const struct BattleAct *)ability));
        state->iconShown[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto psynergy_menu;
        }
        commandKind = CMD_PSYNERGY;
        target = mode;
        break;
      }

      case COMMAND_ICON_SUMMON: {
        const struct SummonDefinition *summon;
        struct BattleAct *ability;
        struct MenuSprite *iconSprite;
        s32 handle;
        s32 ok;
        s32 j;
        s32 n;

        // Summon (the labels djinn_menu here and summon_menu under Djinn
        // have each other's names): the summon list from this command's
        // standby Djinn, the summon's window with its cost (its name in
        // colour 2 and the error sound when it cannot be paid for), and its
        // target; CMD_SUMMON with the summon as parameter.
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        BATTLE_MENU_SET_CURSOR(0, 0, 0);
      djinn_menu:
        state->icons[0].oam.f.x = BATTLE_SUMMON_ICON_X;
        BattleLayout_HighlightPartyPanels(signatureHeader);
        mode = SummonMenu_SelectSummon(0, 0, state->standbyDjinn[commandIndex]);
        BattleLayout_HighlightPartyPanels(signatureHeader);
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = CMD_SUMMON;
        parameter = mode;
        WaitFrames(1);
        iconSprite = &state->icons[1];
        summon = SummonDefinition_Get(mode);
        ability = BattleActGet(summon->nameMessageId);
        handle = VramCache_AllocEntry(128);
        commandWindow =
            UiWindow_Create(10, 17, BATTLE_SUMMON_WINDOW_WIDTH, 3, 6);
        n = 0;
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] > state->standbyDjinn[commandIndex][j]) {
            break;
          }
        }
        ok = (j == 4);
        state->icons[0].oam.f.x = 56;
        iconSprite->oam.word.attr01 = 0x40000400;
        iconSprite->oam.word.attr23 = 0;
        /* FAKEMATCH(barrier): counted as 4 bytes, the empty asm puts the pool
         * after mov r2, r9 as native (constant 56); without it, 34 of 1727
         * instructions differ in tbs-en. */
        __asm__ volatile("");
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile = Resource_LoadIndexedEntryToBuffer(
            summon->nameMessageId & 0x3fff, handle);
        state->iconShown[1] = 1;
        if (ok == 0) {
          UiWork_SetParamNibble(2);
        }
        UiText_DrawCharacterAtOffset(SummonDefinition_Get(mode)->nameMessageId +
                                         MsgAbilityName,
                                     commandWindow, 16, 0);
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] != 0) {
            UiWindow_SetTilemapEntry(commandWindow, j + 20481,
                                     n * 2 + BATTLE_SUMMON_ELEMENT_COLUMN, 0,
                                     0);
            UiWindow_PutGlyph(commandWindow, summon->djinnRequired[j] + 48,
                              n * 2 + BATTLE_SUMMON_COST_COLUMN, 0, 0);
            n++;
          }
        }
        if (ok != 0) {
          AudioCommand_PlayFar(SE_MENU_CONFIRM);
        } else {
          AudioCommand_PlayFar(SE_MENU_ERROR);
        }
        mode = BattleTarget_RunSelection(
            actorId, ability->targetMode, ability->range,
            Battle_ClassifyEntryKind((const struct BattleAct *)ability));
        command->range = ability->range;
        state->iconShown[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto djinn_menu;
        }
        // Spend the cost from this command's standby Djinn: an element that
        // falls short is left at 0, and the elements after it are not spent.
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] > state->standbyDjinn[commandIndex][j]) {
            state->standbyDjinn[commandIndex][j] = 0;
            break;
          }
          state->standbyDjinn[commandIndex][j] -= summon->djinnRequired[j];
        }
        target = mode;
        break;
      }

      case COMMAND_ICON_DJINN: {
        struct BattleAct *ability;
        struct UiWindow *hint;
        s32 cost;
        s32 element;
        s32 index;

        // Djinn: the member's Djinn list; CMD_DJINN with the Djinni's entry
        // as parameter, kept in chosenDjinn so no later command takes it. A
        // set Djinni is unleashed: its ability's target, then in the
        // tutorial a hint. A standby one is set on the member itself
        // ("Set Djinni", target mode 4, kind 7), its name in colour 2.
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        BATTLE_MENU_SET_CURSOR(0, 0, 0);
      summon_menu:
        state->icons[0].oam.f.x = 144;
        gLinkCountdownWork->chosenDjinn[commandIndex] = 0x80000000;
        mode = Battle_SelectAbility(actorId, 1);
        command->range = 0;
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = CMD_DJINN;
        parameter = mode;
        gLinkCountdownWork->chosenDjinn[commandIndex] = mode;
        element = (parameter >> 8) & 15;
        index = parameter & 255;
        if (Djinn_IsActiveFar(actorId, element, index) != 0) {
          ability = BattleActGet(Djinn_GetAbilityIdFar(element, index));
          cost = ability->range;
          commandWindow = UiWindow_Create(11, 17, 10, 3, 6);
          state->icons[0].oam.f.x = 64;
          UiWindow_SetTilemapEntry(commandWindow, element + 20481, 0, 0, 0);
          UiText_DrawCharacterAtOffset(element * 20 + index + MsgDjinnName,
                                       commandWindow, 16, 0);
          command->range = cost;
          WaitFrames(1);
          AudioCommand_PlayFar(SE_MENU_CONFIRM);
          mode = BattleTarget_RunSelection(
              actorId, ability->targetMode, cost,
              Battle_ClassifyEntryKind((const struct BattleAct *)ability));
          if (state->tutorialStep != 0) {
            hint =
                UiText_ShowMessageAndWaitComplete(MsgDjinnUnleashHint, 15, 8);
            while (UiWork_IsComplete() == 0) {
              WaitFrames(1);
            }
            UiWork_Finalize(hint, 1);
            state->tutorialStep++;
            state->tutorialDelay = 45;
          }
          UiWork_Finalize(commandWindow, 1);
          if (mode == -1) {
            goto summon_menu;
          }
          target = mode;
          break;
        }
        commandWindow = UiWindow_Create(11, 17, 10, 3, 6);
        state->icons[0].oam.f.x = 64;
        UiWork_SetParamNibble(2);
        UiWindow_SetTilemapEntry(commandWindow, element + 20481, 0, 0, 0);
        UiText_DrawCharacterAtOffset(element * 20 + index + MsgDjinnName,
                                     commandWindow, 16, 0);
        UiWork_SetParamNibble(15);
        command->range = 1;
        WaitFrames(1);
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(actorId, 4, 0, TARGET_HINT_SET_DJINNI);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto summon_menu;
        }
        target = mode;
        break;
      }

      case COMMAND_ICON_ITEM: {
        struct MenuSprite *iconSprite;
        struct BattleAct *ability;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 cnt;
        s32 k;
        s32 entryValue;
        s32 selection;

        // Item: the member's 15 inventory places, the usable items first
        // and the rest after (pick keeps each one's place); then the item's
        // window and its action's target; CMD_ITEM with the place as
        // parameter.
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = 0;
        BATTLE_MENU_SET_CURSOR(mode, mode, mode);
      item_menu:
        state->icons[0].oam.f.x = 96;
        pick = state->listSlots;
        list = state->listAbilities;
        cnt = 0;
        k = 0;
        entryValue = unit->inventory[k];
        if (entryValue != 0) {
          do {
            Item_GetDirectFar(entryValue);
            if (Item_ClassifyUseAbility(actorId, entryValue) == 0) {
              list[cnt] = entryValue;
              pick[cnt++] = k;
            }
            k++;
            if (k == 15) {
              break;
            }
            entryValue = unit->inventory[k];
          } while (entryValue != 0);
        }
        k = 0;
        entryValue = unit->inventory[k];
        if (entryValue != 0) {
          do {
            Item_GetDirectFar(entryValue);
            if (Item_ClassifyUseAbility(actorId, entryValue) != 0) {
              list[cnt] = entryValue;
              pick[cnt++] = k;
            }
            k++;
            if (k == 15) {
              break;
            }
            entryValue = unit->inventory[k];
          } while (entryValue != 0);
        }
        list[cnt] = 0;
        mode = ItemList_SelectEntry(actorId, list, cnt);
        if (mode == -1) {
          goto mark_visible;
        }
        selection = pick[mode];
        parameter = selection;
        ability = BattleActGet(
            Item_GetDirectFar(unit->inventory[selection])->actionId);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        commandWindow = UiWindow_Create(9, 17, BATTLE_ITEM_WINDOW_WIDTH, 3, 6);
        state->icons[0].oam.f.x = 48;
        iconSprite->oam.word.attr01 = 0x40000400;
        iconSprite->oam.word.attr23 = 0;
        /* FAKEMATCH(barrier): counted as 4 bytes, the empty asm puts the pool
         * after ldr r1, [sp, #72] as native (constant 48); without it, 6 of
         * 1727 instructions differ in tbs-en. */
        __asm__ volatile("");
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile =
            ItemIcon_LoadIntoSlot(unit->inventory[selection], handle);
        state->iconShown[1] = 1;
        UiText_DrawCharacterAtOffset((unit->inventory[selection] & 0x1ff) +
                                         MsgItemName,
                                     commandWindow, 24, 0);
        command->range = cost;
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(
            actorId, ability->targetMode, cost,
            Battle_ClassifyEntryKind((const struct BattleAct *)ability));
        state->iconShown[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto item_menu;
        }
        commandKind = CMD_ITEM;
        target = mode;
        break;
      }

      case COMMAND_ICON_DEFEND:
        // Defend: CMD_DEFEND, nothing to choose.
        commandKind = CMD_DEFEND;
        break;
      }

      AudioCommand_PlayFar(SE_110);
      command->unitId = (u16)actorId;
      // Draw the priority random value once, only for a nonzero agility.
      priority = unit->agi;
      if (priority != 0) {
        priority += (u32)(unit->agi * Random16()) >> 20;
      }
      command->priority = priority;
      if (commandIndex != 0 &&
          actors[commandIndex] == actors[commandIndex - 1]) {
        command->priority = (s16)priority / 2;
      }
      // A priority past 32767 reads negative as s16 and becomes 2000.
      if ((s16)command->priority < 0) {
        command->priority = 2000;
      }
      command->command = commandKind;
      command->parameter = parameter;
      command->target = target;
      commandIndex++;
      if (commandIndex < commandCount) {
        continue;
      }
      break;
    }

    if (commandIndex < commandCount) {
      continue;
    }
    break;
  }

  // Publish completion, then wait for the peer to finish or disconnect.
  if (state->linkTimer != 0) {
    transferWords = (u16 *)gSerialTransfer.payload;
    transferWords[4] = 'E';
    transferWords[5] = 'D';
    transferWords[6] = 'V';
    transferWords[7] = 'S';
  }
  if (state->timerWindow != 0) {
    UiWork_Finalize((struct UiWindow *)state->timerWindow, 1);
  }
  Resource_ResetEntry(smallIconSlot);
  Resource_ResetEntry(largeIconSlot);
  Scheduler_RemoveCallback(UpdateLinkSessionCountdown);
  if (state->linkTimer != 0) {
    // Keep the completion wait visible while the peer finishes its commands.
    s32 Link_DrawWaitingForOpponent(void) {
      u16 data[64];
      BATTLE_MENU_MESSAGE_COPY(MsgWaitingForOpponent, data,
                               BATTLE_MENU_MESSAGE_CAPACITY);
      UiText_RenderWideStringAtOffset(
          data, (struct UiWindow *)state->timerWindow, 0, 4);
#ifdef UBFIX
      return 0;
#endif
    }

    // The wait message shows only when the countdown never opened its
    // window, that is while the partner is still choosing; a timer window
    // was closed above and is only forgotten here. mode counts the frames
    // without the link: 25 give up (-1).
    linkBattle = gBattleWork;
    mode = 0;
    if (state->timerWindow == 0 && linkBattle->linkPaused == 0) {
      state->timerWindow = (s32)UiWindow_Create(0, 16, 30, 4, 42);
      Ui_FillVramBlockPattern();
      Link_DrawWaitingForOpponent();
    } else {
      state->timerWindow = 0;
    }
    for (;;) {
      peerSignature = (u16 *)&gLinkPeerSignatures[linkBattle->linkSide ^ 1];
      if (linkBattle->linkPaused != 0) {
        commandCount = -1;
        break;
      }
      if ((gLinkStatus & 3) != 3) {
        mode++;
        if (mode > 24) {
          commandCount = -1;
          break;
        }
      } else {
        mode = 0;
        if (peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
            peerSignature[6] == 'V' && peerSignature[7] == 'S') {
          break;
        }
        if (!(peerSignature[4] == 'V' && peerSignature[5] == 'S' &&
              peerSignature[6] == 'S' && peerSignature[7] == 'T')) {
          mode = 1;
        }
      }
      WaitFrames(1);
    }
    if (state->timerWindow != 0) {
      UiWork_Finalize((struct UiWindow *)state->timerWindow, 1);
    }
  }
  Camera_ConfigureSceneFar(0);
  Runtime_ReleaseHeapBlock(57);
  return commandCount;
}

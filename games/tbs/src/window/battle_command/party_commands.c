// Part of battle_command.c, the menu object, compiled only through it.
// Collecting the party's battle commands.

#include "../battle_command_internal.h"

// Return the target selection category for an ability's effect.
// 0 other, 1 restore HP, 2 restore PP, 3 cure poison, 4 cure status ailments,
// 5 revive, 6 cure all status ailments.
// act: ability data
s32 Battle_ClassifyEntryKind(const struct BattleAct *act) {
  s32 signedEffect;
  s32 abilityKind;
  s32 targetKind;
  u8 effect;

  targetKind = 0;
  abilityKind = act->flags & ABILITY_KIND_MASK;
  if (abilityKind == ABILITY_KIND_HEAL_HP)
    targetKind = 1;
  if (abilityKind == ABILITY_KIND_HEAL_PP)
    targetKind = 2;
  signedEffect = (s8)act->effect;
  effect = signedEffect;
  if (effect == ABILITY_EFFECT_CURE_POISON)
    targetKind = 3;
  if (effect == ABILITY_EFFECT_RESTORE)
    targetKind = 4;
  if (effect == ABILITY_EFFECT_CURE_ALL)
    targetKind = 6;
  // Revive.
  if (Battle_IsReviveEffectFar(act->effect) != 0)
    targetKind = 5;
  return targetKind;
}

// Collect commands in party order, including target, cost and randomized
// priority. Cancelling returns to the preceding choice; linked battles
// exchange completion signatures before releasing the command UI.
s32 Battle_CollectPartyCommands(struct BattleActionRecord *commands,
                                u16 *actors, s32 commandCount) {
  struct LinkRoundState *state;
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

  // An empty ID list dims every party panel before the next actor is chosen.
  void BattleMenu_ClearPartyHighlights(void) {
    u16 data[2];
    data[0] = 0xff;
    BattleLayout_HighlightPartyPanels(data);
    BattlePres_SetActorModesFar(data, 1);
  }

  // Set up the shared command UI before the optional link handshake.
  target = 256;
  commandKind = 0;
  parameter = 0;
  largeIconSlot = VramCache_AllocEntry(1024);
  smallIconSlot = VramCache_AllocEntry(512);
  state = Runtime_AllocateBlock(57, 260);
  Resource_LoadIndexedIntoBuffer(largeIconSlot, 0);
  Graphics_ExpandVramTilesByColorTable(0x06006000);
  Link_DrawShiftedTilePair(0x06006680);
  Menu_BuildLocalizedPatternTiles();
  battle = gBattleWork;
  for (k = 0; k < 8; k++) {
    state->selected[k] = 0x80000000;
  }
  for (k = 0; k < 3; k++) {
    state->entryActive[k] = 0;
  }
  state->targetOffset = 0;
  state->currentOffset = 0;
  state->timer = -1;
  state->unk3c = 0;
  state->unk40 = 0;
  state->enabled = 0;
  state->secondaryHandle = 0;
  state->displayHandle = 0;
  // Both peers announce the command-entry phase before accepting commands.
  if (battle->twoSided != 0) {
    state->enabled = 1;
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
        state->enabled = 0;
        break;
      }
      if ((gLinkStatus & 3) != 3) {
        if (++missedLinkFrames > 24) {
          commandCount = -1;
          state->enabled = 0;
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
          state->enabled = 0;
          break;
        }
      }
      WaitFrames(1);
    }
  }
  Scheduler_AddOrUpdateCallback(UpdateLinkSessionCountdown, 3200);

  while (1) {
    BattleMenu_ClearPartyHighlights();
    state->entryActive[2] = 0;
    state->actorId = -1;
    state->hintCount = 0;
    if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0) {
      state->hintCount = 1;
      state->hintTimer = 60;
    }
    if (commandCount > 0) {
      mode = Ui_RunSelectionScreen(0);
    } else {
      mode = 14;
    }
    if (mode == 7) {
      void *block;
      s32 selection;
      s32 handle;

      block = MemAlloc(12);
      if (gDebugMode != 0 && (gKeysHeld & KEY_START) != 0) {
        selection = 2;
      } else {
        selection = 1;
      }
      handle = BattleParty_ListActorIdsFar(selection, block);
      WaitFrames(1);
      Ui_RunOwnerStatusScreen(block, handle, *(u16 *)block);
      MemFree(block);
      continue;
    }
    if (mode == 4) {
      if (UiText_ShowLocalizedMessageAndWait() != 0) {
        continue;
      }
      commandCount = 1;
      command = commands;
      command->unitId = actors[0];
      command->priority = 0x7ffe;
      command->command = 99;
      command->parameter = 0;
      command->target = 256;
      break;
    }
    if (mode != 14) {
      break;
    }
    AudioCommand_PlayFar(SE_154);
    commandIndex = 0;
    if (commandIndex >= commandCount) {
      break;
    }

    for (;;) {
      if (commandIndex == 0) {
        BattlePlacement_CountValidEntriesFar(0, state->cost[0]);
      } else {
        s32 k;

        for (k = 0; k < 4; k++) {
          state->cost[commandIndex][k] = state->cost[commandIndex - 1][k];
        }
      }

      command = &commands[commandIndex];
      actorId = actors[commandIndex];
      // UB: UnitGetFar returns a pointer, not the cast's doubleword result.
#ifdef UBFIX
      unit = UnitGetFar(actorId);
#else
      /* FAKEMATCH: instruction order only: called through a cast as
         returning a doubleword, the call also sets r1, so the r1
         reload for classId no longer depends on the in reload and
         sched2 loads i * 2 before in, as native (by its prototype,
         differ 2). */
      unit = (struct BattleUnit *)(s32)((s64 (*)(s32))UnitGetFar)(actorId);
#endif
      COUNTDOWN->actorId = actorId;
      COUNTDOWN->unk40 = 0;
      iconSprite = &COUNTDOWN->entries[2];
      iconSprite->oam.word.attr01 = 0x80000400;
      iconSprite->oam.word.attr23 = 0;
      iconSprite->oam.f.tile =
          Ui_LoadEntryForKind(unit->classId, smallIconSlot);
      iconSprite->oam.f.x = BATTLE_CLASS_ICON_X;
      iconSprite->oam.f.y = 0x80;
      iconSprite->oam.f.palette = 14;
      state->entryActive[2] = 1;

    mark_visible:
      UiWindow_MarkVisibleTileAttributes();
      COUNTDOWN->entryActive[0] = 0;
      COUNTDOWN->selected[commandIndex] = 0x80000000;
      /* FAKEMATCH: twelve dead stores restore the native frame's spill slots (i
       * << 1, i << 2 and i << 4 at sp+32 to sp+24, above &hdr at sp+20);
       * without them differ 50 in every edition, every row a spill offset. */
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
      signatureHeader[0] = (u16)actorId;
      signatureHeader[1] = 255;
      BattlePres_SetActorModesFar(signatureHeader, 1);
      BattleLayout_HighlightPartyPanels(signatureHeader);
      WaitFrames(1);
      mode = Ui_RunSelectionScreen(1);
      WaitFrames(1);
      if (mode == -2) {
        void *block;
        s32 handle;

        block = MemAlloc(12);
        handle = BattleParty_ListActorIdsFar(1, block);
        state->entryActive[2] = 0;
        WaitFrames(1);
        Ui_RunOwnerStatusScreen(block, handle, actorId);
        state->entryActive[2] = 1;
        MemFree(block);
        continue;
      }
      BattlePres_SetActorModesFar(signatureHeader, 0);
      if (mode == -1) {
        if (commandIndex == 0) {
          break;
        }
        commandIndex--;
        continue;
      }

      if (COUNTDOWN->timer == 0) {
        mode = 3;
      }
      iconSprite = &COUNTDOWN->entries[0];
      iconSprite->oam.word.attr23 = 0;
      iconSprite->oam.word.attr01 = 0x80002400;
      iconSprite->oam.f.tile =
          Resource_LoadIndexedIntoBuffer(largeIconSlot, mode);
      if (mode == 15) {
        iconSprite->oam.f.x = 128;
      } else {
        iconSprite->oam.f.x = 96;
      }
      iconSprite->oam.f.y = 136;
      state->entryActive[0] = 1;
      switch (mode) {
      case 0:
        commandWindow = UiWindow_Create(11, 17, 11, 3, 6);
        UiText_DrawCharacterAtOffset(MsgCommandName, commandWindow,
                                     BATTLE_COMMAND_NAME_X, 0);
        state->entries[0].oam.f.x = 64;
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(actorId, 1, 1, 0);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = 0;
        target = mode;
        command->range = 1;
        break;

      case 1: {
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

        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = 0;
        MenuCell_SetCursor(mode, mode, mode);
      psynergy_menu:
        list = state->list;
        pick = state->pick;
        state->entries[0].oam.f.x = 48;
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
        cost = ability->range;
        iconSprite = &state->entries[1];
        handle = VramCache_AllocEntry(128);
        ui = UI_WORK;
        commandWindow = UiWindow_Create(8, 17, 18, 3, 6);
        state->entries[0].oam.f.x = 40;
        iconSprite->oam.word.attr01 = 0x40000400;
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
        /* FAKEMATCH: instruction order only (pool placement): arm_reorg counts
         * this empty asm as a 4-byte insn, so the pool goes in after mov r1,
         * r10 as native, whose constant 40 acts as a range-60
         * (zero_extendhisi2) pool load (differ 14). 2026-10-11: arm_reorg runs
         * before shorten_branches and counts the empty asm at its 4-byte
         * default length while it emits nothing; the halfword constant fix
         * needs that much more estimated length after it, and no C statement or
         * single pattern-length change (strh, str, orr, and tested over all 413
         * English objects) supplies it. */
        __asm__ volatile("");
        iconSprite->oam.f.tile =
            Resource_LoadIndexedEntryToBuffer(parameter, handle);
        state->entryActive[1] = 1;
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
        UiWindow_SetTilemapEntry(commandWindow, 61471, 11, 0, 0);
        UiWindow_SetTilemapEntry(commandWindow, PSYNERGY_COST_SECOND_TILE, 12,
                                 0, 0);
        if (ability->element != 4) {
          UiWindow_SetTilemapEntry(commandWindow, ability->element + 20481, 15,
                                   0, 0);
        }
        command->range = cost;
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(
            actorId, ability->targetMode, cost,
            Battle_ClassifyEntryKind((const struct BattleAct *)ability));
        state->entryActive[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto psynergy_menu;
        }
        commandKind = 1;
        target = mode;
        break;
      }

      case 16: {
        const struct SummonDefinition *summon;
        struct BattleAct *ability;
        struct MenuSprite *iconSprite;
        s32 handle;
        s32 ok;
        s32 j;
        s32 n;

        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        MenuCell_SetCursor(0, 0, 0);
      djinn_menu:
        state->entries[0].oam.f.x = BATTLE_SUMMON_ICON_X;
        BattleLayout_HighlightPartyPanels(signatureHeader);
        mode = SummonMenu_SelectSummon(0, 0, state->cost[commandIndex]);
        BattleLayout_HighlightPartyPanels(signatureHeader);
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = 6;
        parameter = mode;
        WaitFrames(1);
        iconSprite = &state->entries[1];
        summon = SummonDefinition_Get(mode);
        ability = BattleActGet(summon->nameMessageId);
        handle = VramCache_AllocEntry(128);
        commandWindow =
            UiWindow_Create(10, 17, BATTLE_SUMMON_WINDOW_WIDTH, 3, 6);
        n = 0;
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] > state->cost[commandIndex][j]) {
            break;
          }
        }
        ok = (j == 4);
        state->entries[0].oam.f.x = 56;
        iconSprite->oam.word.attr01 = 0x40000400;
        iconSprite->oam.word.attr23 = 0;
        /* FAKEMATCH: instruction order only (pool placement, schedule): counted
         * as 4 bytes, the empty asm puts the pool after mov r2, r9 as native
         * (constant 56) and keeps ok = 1 - (j != 4) among the slot stores
         * (differ 12). 2026-10-11: arm_reorg runs before shorten_branches and
         * counts the empty asm at its 4-byte default length while it emits
         * nothing; the halfword constant fix needs that much more estimated
         * length after it, and no C statement or single pattern-length change
         * (strh, str, orr, and tested over all 413 English objects) supplies
         * it. */
        __asm__ volatile("");
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile = Resource_LoadIndexedEntryToBuffer(
            summon->nameMessageId & 0x3fff, handle);
        state->entryActive[1] = 1;
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
        state->entryActive[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto djinn_menu;
        }
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] > state->cost[commandIndex][j]) {
            state->cost[commandIndex][j] = 0;
            break;
          }
          state->cost[commandIndex][j] -= summon->djinnRequired[j];
        }
        target = mode;
        break;
      }

      case 15: {
        struct BattleAct *ability;
        struct UiWindow *hint;
        s32 cost;
        s32 element;
        s32 index;

        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        MenuCell_SetCursor(0, 0, 0);
      summon_menu:
        state->entries[0].oam.f.x = 144;
        COUNTDOWN->selected[commandIndex] = 0x80000000;
        mode = Battle_SelectAbility(actorId, 1);
        command->range = 0;
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = 5;
        parameter = mode;
        COUNTDOWN->selected[commandIndex] = mode;
        element = (parameter >> 8) & 15;
        index = parameter & 255;
        if (Djinn_IsActiveFar(actorId, element, index) != 0) {
          ability = BattleActGet(Djinn_GetAbilityIdFar(element, index));
          cost = ability->range;
          commandWindow = UiWindow_Create(11, 17, 10, 3, 6);
          state->entries[0].oam.f.x = 64;
          UiWindow_SetTilemapEntry(commandWindow, element + 20481, 0, 0, 0);
          UiText_DrawCharacterAtOffset(element * 20 + index + MsgDjinnName,
                                       commandWindow, 16, 0);
          command->range = cost;
          WaitFrames(1);
          AudioCommand_PlayFar(SE_MENU_CONFIRM);
          mode = BattleTarget_RunSelection(
              actorId, ability->targetMode, cost,
              Battle_ClassifyEntryKind((const struct BattleAct *)ability));
          if (state->hintCount != 0) {
            hint =
                UiText_ShowMessageAndWaitComplete(MsgDjinnUnleashHint, 15, 8);
            while (UiWork_IsComplete() == 0) {
              WaitFrames(1);
            }
            UiWork_Finalize(hint, 1);
            state->hintCount++;
            state->hintTimer = 45;
          }
          UiWork_Finalize(commandWindow, 1);
          if (mode == -1) {
            goto summon_menu;
          }
          target = mode;
          break;
        }
        commandWindow = UiWindow_Create(11, 17, 10, 3, 6);
        state->entries[0].oam.f.x = 64;
        UiWork_SetParamNibble(2);
        UiWindow_SetTilemapEntry(commandWindow, element + 20481, 0, 0, 0);
        UiText_DrawCharacterAtOffset(element * 20 + index + MsgDjinnName,
                                     commandWindow, 16, 0);
        UiWork_SetParamNibble(15);
        command->range = 1;
        WaitFrames(1);
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(actorId, 4, 0, 7);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto summon_menu;
        }
        target = mode;
        break;
      }

      case 2: {
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

        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = 0;
        MenuCell_SetCursor(mode, mode, mode);
      item_menu:
        state->entries[0].oam.f.x = 96;
        pick = state->pick;
        list = state->list;
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
        iconSprite = &state->entries[1];
        handle = VramCache_AllocEntry(128);
        commandWindow = UiWindow_Create(9, 17, BATTLE_ITEM_WINDOW_WIDTH, 3, 6);
        state->entries[0].oam.f.x = 48;
        iconSprite->oam.word.attr01 = 0x40000400;
        iconSprite->oam.word.attr23 = 0;
        /* FAKEMATCH: instruction order only (pool placement): counted as 4
         * bytes, the empty asm puts the pool after ldr r1, [sp, #72] as native
         * (constant 48) (differ 10). 2026-10-11: arm_reorg runs before
         * shorten_branches and counts the empty asm at its 4-byte default
         * length while it emits nothing; the halfword constant fix needs that
         * much more estimated length after it, and no C statement or single
         * pattern-length change (strh, str, orr, and tested over all 413
         * English objects) supplies it. */
        __asm__ volatile("");
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile = Resource_LoadKind26EntryToBuffer(
            unit->inventory[selection], handle);
        state->entryActive[1] = 1;
        UiText_DrawCharacterAtOffset((unit->inventory[selection] & 0x1ff) +
                                         MsgItemName,
                                     commandWindow, 24, 0);
        command->range = cost;
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(
            actorId, ability->targetMode, cost,
            Battle_ClassifyEntryKind((const struct BattleAct *)ability));
        state->entryActive[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto item_menu;
        }
        commandKind = 2;
        target = mode;
        break;
      }

      case 3:
        commandKind = 3;
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
  if (state->enabled != 0) {
    transferWords = (u16 *)gSerialTransfer.payload;
    transferWords[4] = 'E';
    transferWords[5] = 'D';
    transferWords[6] = 'V';
    transferWords[7] = 'S';
  }
  if (state->displayHandle != 0) {
    UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
  }
  Resource_ResetEntry(smallIconSlot);
  Resource_ResetEntry(largeIconSlot);
  Scheduler_RemoveCallback(UpdateLinkSessionCountdown);
  if (state->enabled != 0) {
    // Keep the completion wait visible while the peer finishes its commands.
    s32 Link_DrawWaitingForOpponent(void) {
      u16 data[64];
      BATTLE_MENU_MESSAGE_COPY(MsgWaitingForOpponent, data,
                               BATTLE_MENU_MESSAGE_CAPACITY);
      UiText_RenderWideStringAtOffset(
          data, (struct UiWindow *)state->displayHandle, 0, 4);
    }

    linkBattle = gBattleWork;
    mode = 0;
    if (state->displayHandle == 0 && linkBattle->linkPaused == 0) {
      state->displayHandle = (s32)UiWindow_Create(0, 16, 30, 4, 42);
      Ui_FillVramBlockPattern();
      Link_DrawWaitingForOpponent();
    } else {
      state->displayHandle = 0;
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
    if (state->displayHandle != 0) {
      UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
    }
  }
  Camera_ConfigureSceneFar(0);
  Runtime_ReleaseHeapBlock(57);
  return commandCount;
}

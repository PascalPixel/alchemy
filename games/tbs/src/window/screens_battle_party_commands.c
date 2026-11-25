/// @file
/// Collecting the party's battle commands.
// Part of screens.c.

#include "screens_internal.h"

/// Flee's priority, above every other action's: the party flees first in the
/// turn.
#define FLEE_PRIORITY 32766
/// The priority an action takes when its Agility roll runs past 32767.
#define OVERFLOWED_PRIORITY 2000

/// Classifies ability `act` for the target selection (the `kind` of
/// BattleTarget_RunSelection, enum BattleTargetHint), which shows what it would
/// do on a party member and, for the curing and reviving hints, starts on one
/// who needs it.
s32 Battle_ClassifyEntryKind(const struct AbilityDefinition *act) {
  s32 abilityKind;
  s32 targetKind;

  targetKind = TARGET_HINT_NONE;
  abilityKind = act->flags & ABILITY_FORMULA_MASK;
  if (abilityKind == ABILITY_FORMULA_HEAL_HP)
    targetKind = TARGET_HINT_HP;
  if (abilityKind == ABILITY_FORMULA_HEAL_PP)
    targetKind = TARGET_HINT_PP;
  if (act->effect == ABILITY_EFFECT_CURE_POISON)
    targetKind = TARGET_HINT_CURE_POISON;
  if (act->effect == ABILITY_EFFECT_RESTORE)
    targetKind = TARGET_HINT_RESTORE;
  if (act->effect == ABILITY_EFFECT_CURE_ALL)
    targetKind = TARGET_HINT_CURE_ALL;
  if (Battle_IsReviveEffectFar(act->effect) != 0)
    targetKind = TARGET_HINT_REVIVE;
  return targetKind;
}

/// Collects the party's commands for a turn into `commands`, one for each of
/// the `commandCount` units in `actors` (a party member with two actions is
/// listed twice), and returns how many it wrote: commandCount, 1 when the party
/// flees (COMMAND_FLEE for the first actor), or -1 when the link fails.
s32 Battle_CollectPartyCommands(struct BattleAction *commands, u16 *actors,
                                s32 commandCount) {
  struct BattleMenuState *state;
  // Default: each local given NULL or 0 here is set for real before it is
  // read.
  struct BattleState *battle = NULL;
  struct BattleState *linkBattle = NULL;
  u16 *peerSignature = NULL;
  u16 *transferWords = NULL;
  struct BattleAction *command = NULL;
  struct Unit *unit = NULL;
  struct Window *commandWindow = NULL;
  struct MenuSprite *iconSprite = NULL;
  u16 memberList[2];
  s32 target;
  s32 unitId;
  s32 mode = 0;
  s32 commandKind;
  s32 parameter;
  s32 largeIconSlot;
  s32 smallIconSlot;
  s32 missedLinkFrames = 0;
  s32 commandIndex = 0;
  s32 k = 0;
  s32 priority;

  // An empty ID list clears every party panel's highlight and marks no unit
  // before the next actor is chosen.
  void PartyStatus_ClearHighlights(void) {
    u16 data[2];
    data[0] = BATTLE_UNIT_LIST_END;
    PartyStatus_HighlightMembers(data);
    BattleHighlight_SetModesFar(data, HIGHLIGHT_SELECTED);
  }

  // Set up the shared command UI before the optional link handshake: VRAM
  // for the chosen command's 32x32 icon (1 KB) and the party member's portrait,
  // the lists' page numbers (PAGE_DIGITS_TILE), page arrows
  // (LIST_PAGE_ARROW_VRAM) and range icons (RANGE_ICON_FIRST_TILE), and no
  // Djinni chosen yet (BATTLE_MENU_NO_DJINNI).
  target = PLACE_PARTY;
  commandKind = 0;
  parameter = 0;
  largeIconSlot = VramCache_AllocEntry(COMMAND_ICON_BYTES);
  smallIconSlot = VramCache_AllocEntry(512);
  state = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_MENU,
                              sizeof(struct BattleMenuState));
  CommandIcon_ReloadSlot(largeIconSlot, 0);
  Graphics_ExpandVramTilesByColorTable(
      (u16 *)(BG_CHAR_BLOCK(0) + PAGE_DIGITS_TILE * TILE_SIZE_4BPP));
  PageArrows_Draw(LIST_PAGE_ARROW_VRAM);
  RangeIcon_BuildTiles();
  battle = gBattleState;
  for (k = 0; k < 8; k++) {
    state->chosenDjinn[k] = BATTLE_MENU_NO_DJINNI;
  }
  for (k = 0; k < BATTLE_MENU_ICON_COUNT; k++) {
    state->iconShown[k] = 0;
  }
  state->targetViewOffset = 0;
  state->viewOffset = 0;
  state->timerWindow = NULL;
  state->timer = -1;
  state->commandSelection[0] = 0;
  state->commandSelection[1] = 0;
  state->linkTimer = 0;
  state->timerHidden = 0;
  // Both peers announce the command-entry phase before accepting commands: this
  // side writes "VSST" into halfwords 4 to 7 of its signature, then waits for
  // the partner's "VSST" (or "EDVS", already done) while its first four read
  // "EXEC".
  if (battle->isLink != 0) {
    state->linkTimer = 1;
    transferWords = (u16 *)gSerialTransfer.payload;
    transferWords[4] = 'V';
    transferWords[5] = 'S';
    transferWords[6] = 'S';
    transferWords[7] = 'T';
    missedLinkFrames = 0;
    for (;;) {
      peerSignature = gLinkPeerSignatures[battle->linkSide ^ 1];
      if (battle->linkPaused != 0) {
        commandCount = -1;
        state->linkTimer = 0;
        break;
      }
      if ((gLinkStatus & LINK_STATUS_HEARD_BOTH) != LINK_STATUS_HEARD_BOTH) {
        if (++missedLinkFrames > LINK_MAX_MISSED_FRAMES) {
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
      wait(1);
    }
  }
  Task_Add(BattleMenu_Update, TASK_PASS_FRAME);

  while (1) {
    PartyStatus_ClearHighlights();
    state->iconShown[BATTLE_MENU_ICON_PORTRAIT] = 0;
    state->commandActor = -1;
    state->tutorialStep = 0;
    if (GameFlag_GetFar(FLAG_DJINN_LESSON) != 0) {
      state->tutorialStep = 1;
      state->tutorialDelay = 60;
    }
    // The party's row; with no party member to choose, as if Fight were taken.
    if (commandCount > 0) {
      mode = BattleCommandRow_Run(COMMAND_ROW_PARTY);
    } else {
      mode = COMMAND_ICON_FIGHT;
    }
    // Status: the party's Status screens, from the first party member; the
    // debug build with Start held lists the enemies instead.
    if (mode == COMMAND_ICON_STATUS) {
      u16 *block;
      s32 selection;
      s32 handle;

      block = Heap_Alloc(12);
      if (gDebugMode != 0 && (gKeysHeld & KEY_START) != 0) {
        selection = BATTLE_SIDE_MASK_ENEMIES;
      } else {
        selection = BATTLE_SIDE_MASK_PARTY;
      }
      handle = BattleUnit_ListIdsFar(selection, block);
      wait(1);
      BattleStatusScreen_Run(block, handle, block[0]);
      Heap_Release(block);
      continue;
    }
    // Flee, unless the battle forbids it: one COMMAND_FLEE for the party,
    // first in the turn.
    if (mode == COMMAND_ICON_FLEE) {
      if (Battle_RefuseFlee() != 0) {
        continue;
      }
      commandCount = 1;
      command = commands;
      command->unitId = actors[0];
      command->priority = FLEE_PRIORITY;
      command->command = COMMAND_FLEE;
      command->parameter = 0;
      command->target = PLACE_PARTY;
      break;
    }
    if (mode != COMMAND_ICON_FIGHT) {
      break;
    }
    Sound_PlayFar(SE_CAST);
    commandIndex = 0;
    if (commandIndex >= commandCount) {
      break;
    }

    // Each party member in turn.
    for (;;) {
      if (commandIndex == 0) {
        BattleDjinn_CountStandbyFar(0, state->standbyDjinn[0]);
      } else {
        s32 k;

        for (k = 0; k < ELEMENT_COUNT; k++) {
          state->standbyDjinn[commandIndex][k] =
              state->standbyDjinn[commandIndex - 1][k];
        }
      }

      command = &commands[commandIndex];
      unitId = actors[commandIndex];
      // UB: Unit_GetFar returns a pointer, not the cast's doubleword result.
#ifdef UBFIX
      unit = Unit_GetFar(unitId);
#else
      unit = (struct Unit *)(s32)((s64 (*)(s32))Unit_GetFar)(unitId);
#endif
      // The party member's row starts on Attack; its portrait shows at the
      // left.
      gBattleMenuState->commandActor = unitId;
      gBattleMenuState->commandSelection[1] = 0;
      iconSprite = &gBattleMenuState->icons[BATTLE_MENU_ICON_PORTRAIT];
      iconSprite->oam.word.attr01 =
          OBJ_SIZE_32X32 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
      iconSprite->oam.word.attr23 = 0;
      iconSprite->oam.f.tile = Portrait_Load(unit->id, smallIconSlot);
      iconSprite->oam.f.x = BATTLE_PORTRAIT_X;
      iconSprite->oam.f.y = 128;
      iconSprite->oam.f.palette = PORTRAIT_PALETTE;
      state->iconShown[BATTLE_MENU_ICON_PORTRAIT] = 1;

    mark_visible:
      Window_MarkVisibleTextTiles();
      gBattleMenuState->iconShown[BATTLE_MENU_ICON_COMMAND] = 0;
      gBattleMenuState->chosenDjinn[commandIndex] = BATTLE_MENU_NO_DJINNI;
      // memberList is the party member alone, ended by
      // BATTLE_UNIT_LIST_END: marked in the battle and highlighted in the party
      // status window.
      memberList[0] = unitId;
      memberList[1] = BATTLE_UNIT_LIST_END;
      BattleHighlight_SetModesFar(memberList, HIGHLIGHT_SELECTED);
      PartyStatus_HighlightMembers(memberList);
      wait(1);
      mode = BattleCommandRow_Run(COMMAND_ROW_MEMBER);
      wait(1);
      if (mode == COMMAND_ROW_STATUS) {
        void *block;
        s32 handle;

        block = Heap_Alloc(12);
        handle = BattleUnit_ListIdsFar(BATTLE_SIDE_MASK_PARTY, block);
        state->iconShown[BATTLE_MENU_ICON_PORTRAIT] = 0;
        wait(1);
        BattleStatusScreen_Run(block, handle, unitId);
        state->iconShown[BATTLE_MENU_ICON_PORTRAIT] = 1;
        Heap_Release(block);
        continue;
      }
      BattleHighlight_SetModesFar(memberList, HIGHLIGHT_OFF);
      // B: back to the party member before, or from the first to the party's
      // row.
      if (mode == COMMAND_ROW_CANCELLED) {
        if (commandIndex == 0) {
          break;
        }
        commandIndex--;
        continue;
      }

      // Out of time in a link battle: Defend.
      if (gBattleMenuState->timer == 0) {
        mode = COMMAND_ICON_DEFEND;
      }
      iconSprite = &gBattleMenuState->icons[BATTLE_MENU_ICON_COMMAND];
      MenuSprite_Initialize(*iconSprite,
                            OBJ_SIZE_32X32 | OBJ_256_COLORS |
                                OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
      iconSprite->oam.f.tile = CommandIcon_ReloadSlot(largeIconSlot, mode);
      if (mode == COMMAND_ICON_DJINN) {
        iconSprite->oam.f.x = 128;
      } else {
        iconSprite->oam.f.x = 96;
      }
      iconSprite->oam.f.y = 136;
      state->iconShown[BATTLE_MENU_ICON_COMMAND] = 1;
      // Each command's menus; commandKind is the COMMAND_ value recorded.
      switch (mode) {
      case COMMAND_ICON_ATTACK:
        // Attack: one enemy, range 1; COMMAND_ATTACK.
        commandWindow = Window_Create(11, 17, 11, 3, MENU_WINDOW_FLAGS);
        Text_DrawMessageAsTiles(MsgCommandName, commandWindow,
                                BATTLE_COMMAND_NAME_X, 0);
        state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 64;
        Sound_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(unitId, ABILITY_TARGET_FOES, 1,
                                         TARGET_HINT_NONE);
        Window_Close(commandWindow, 1);
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = COMMAND_ATTACK;
        target = mode;
        command->range = 1;
        break;

      case COMMAND_ICON_PSYNERGY: {
        struct MenuSprite *iconSprite;
        struct AbilityDefinition *ability;
        struct WindowState *windowState;
        u16 *list;
        u8 *pick;
        s32 range;
        s32 handle;
        s32 count;
        s32 k;
        s32 entryValue;

        // Psynergy: the party member's abilities usable in battle, from its 32
        // action slots (pick keeps each one's slot); then the ability's
        // window, TEXT_COLOR_RED when the PP fall short and TEXT_COLOR_PINK
        // while sealed, and its target; COMMAND_PSYNERGY with the ability as
        // parameter.
        Sound_PlayFar(SE_MENU_CONFIRM);
        // DEAD: mode cleared before the list sets it; the ROM keeps its mov.
        mode = 0;
        BATTLE_MENU_SET_CURSOR(0, 0, 0);
      psynergy_list:
        list = state->listAbilities;
        pick = state->listSlots;
        state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 48;
        count = 0;
        for (k = 0; k != PSYNERGY_SLOTS &&
                    (entryValue =
                         unit->psynergy[k].encodedAction & ACTION_ID_MASK) != 0;
             k++) {
          if ((Ability_Get(entryValue)->flags & BATTLE_OK) != 0) {
            pick[count] = k;
            list[count] = entryValue;
            count++;
          }
        }
        pick[count] = 0;
        list[count] = 0;
        mode = BattlePsynergyMenu_Run(unitId, list, count);
        if (mode == -1) {
          goto mark_visible;
        }
        parameter = unit->psynergy[pick[mode]].encodedAction & ACTION_ID_MASK;
        ability = Ability_Get(parameter);
        range = ability->range;
        iconSprite = &state->icons[BATTLE_MENU_ICON_ABILITY];
        handle = VramCache_AllocEntry(128);
        windowState = WINDOW_STATE;
        commandWindow = Window_Create(8, 17, 18, 3, MENU_WINDOW_FLAGS);
        state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 40;
        iconSprite->oam.word.attr01 =
            OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
        CAMELOT_ASM("an asm with no instruction, text unknown: the literal "
                    "pool after this sprite's stores sits where the ROM has it "
                    "only if exactly 4 more bytes are counted between the load "
                    "of the Psynergy icon's x (40) and the call to "
                    "AbilityIcon_LoadIntoSlot than the instructions emitted, "
                    "in all six editions; only a one-line asm counts bytes and "
                    "emits nothing in this compiler; placed anywhere in that "
                    "window, this point is one choice")
        __asm__ volatile("");
        iconSprite->oam.f.tile = AbilityIcon_LoadIntoSlot(parameter, handle);
        state->iconShown[BATTLE_MENU_ICON_ABILITY] = 1;
        windowState->textPalette = BASIC_COLORS_PALETTE;
        if (ability->ppCost > unit->pp) {
          Text_SetColor(TEXT_COLOR_RED);
        } else if (unit->psySeal != 0) {
          Text_SetColor(TEXT_COLOR_PINK);
        }
        Text_DrawMessageAsTiles(parameter + MsgAbilityName, commandWindow, 16,
                                0);
        Text_DrawNumberAtOffset(ability->ppCost, 2, commandWindow, 104, 0);
        windowState->textPalette = WINDOW_PALETTE;
        Text_SetColor(TEXT_COLOR_WHITE);
        Window_SetTilemapEntry(commandWindow, PSYNERGY_COST_FIRST_TILE, 11, 0,
                               0);
        Window_SetTilemapEntry(commandWindow, PSYNERGY_COST_SECOND_TILE, 12, 0,
                               0);
        if (ability->element != ELEMENT_NONE) {
          Window_SetTilemapEntry(commandWindow,
                                 ability->element + LIST_TILE_SUMMON_ELEMENT,
                                 15, 0, 0);
        }
        command->range = range;
        Sound_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(unitId, ability->targetMode, range,
                                         Battle_ClassifyEntryKind(ability));
        state->iconShown[BATTLE_MENU_ICON_ABILITY] = 0;
        VramCache_FreeEntry(handle);
        Window_Close(commandWindow, 1);
        if (mode == -1) {
          goto psynergy_list;
        }
        commandKind = COMMAND_PSYNERGY;
        target = mode;
        break;
      }

      case COMMAND_ICON_SUMMON: {
        const struct SummonDefinition *summon;
        struct AbilityDefinition *ability;
        struct MenuSprite *iconSprite;
        s32 handle;
        s32 ok;
        s32 j;
        s32 n;

        // Summon: the summon list from this command's
        // standby Djinn, the summon's window with its cost (its name in
        // TEXT_COLOR_RED and the error sound when it cannot be paid for), and
        // its target; COMMAND_SUMMON with the summon as parameter.
        Sound_PlayFar(SE_MENU_CONFIRM);
        BATTLE_MENU_SET_CURSOR(0, 0, 0);
      summon_list:
        state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = BATTLE_SUMMON_ICON_X;
        PartyStatus_HighlightMembers(memberList);
        mode = BattleSummonMenu_Run(0, 0, state->standbyDjinn[commandIndex]);
        PartyStatus_HighlightMembers(memberList);
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = COMMAND_SUMMON;
        parameter = mode;
        wait(1);
        iconSprite = &state->icons[BATTLE_MENU_ICON_ABILITY];
        summon = SummonDefinition_Get(mode);
        ability = Ability_Get(summon->abilityId);
        handle = VramCache_AllocEntry(128);
        commandWindow = Window_Create(10, 17, BATTLE_SUMMON_WINDOW_WIDTH, 3,
                                      MENU_WINDOW_FLAGS);
        n = 0;
        for (j = 0; j < ELEMENT_COUNT; j++) {
          if (summon->djinnRequired[j] > state->standbyDjinn[commandIndex][j]) {
            break;
          }
        }
        ok = (j == ELEMENT_COUNT);
        state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 56;
        iconSprite->oam.word.attr01 =
            OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        iconSprite->oam.word.attr23 = 0;
        CAMELOT_ASM("an asm with no instruction, text unknown: the literal "
                    "pool after this sprite's stores sits where the ROM has it "
                    "only if exactly 4 more bytes are counted between the load "
                    "of the summon icon's x (56) and the call to "
                    "AbilityIcon_LoadIntoSlot than the instructions emitted, "
                    "in all six editions; only a one-line asm counts bytes and "
                    "emits nothing in this compiler; and the second scheduling "
                    "pass keeps the row's constant and subtraction on either "
                    "side of this point, which no length difference can do")
        __asm__ volatile("");
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile = AbilityIcon_LoadIntoSlot(
            summon->abilityId & ACTION_ID_MASK, handle);
        state->iconShown[BATTLE_MENU_ICON_ABILITY] = 1;
        if (ok == 0) {
          Text_SetColor(TEXT_COLOR_RED);
        }
        Text_DrawMessageAsTiles(SummonDefinition_Get(mode)->abilityId +
                                    MsgAbilityName,
                                commandWindow, 16, 0);
        for (j = 0; j < ELEMENT_COUNT; j++) {
          if (summon->djinnRequired[j] != 0) {
            Window_SetTilemapEntry(commandWindow, j + LIST_TILE_SUMMON_ELEMENT,
                                   n * 2 + BATTLE_SUMMON_ELEMENT_COLUMN, 0, 0);
            Window_PutGlyph(commandWindow, summon->djinnRequired[j] + '0',
                            n * 2 + BATTLE_SUMMON_COST_COLUMN, 0,
                            GLYPH_DRAW_CHARACTER);
            n++;
          }
        }
        if (ok != 0) {
          Sound_PlayFar(SE_MENU_CONFIRM);
        } else {
          Sound_PlayFar(SE_MENU_ERROR);
        }
        mode = BattleTarget_RunSelection(unitId, ability->targetMode,
                                         ability->range,
                                         Battle_ClassifyEntryKind(ability));
        command->range = ability->range;
        state->iconShown[BATTLE_MENU_ICON_ABILITY] = 0;
        VramCache_FreeEntry(handle);
        Window_Close(commandWindow, 1);
        if (mode == -1) {
          goto summon_list;
        }
        // Spend the cost from this command's standby Djinn: an element that
        // falls short is left at 0, and the elements after it are not spent.
        for (j = 0; j < ELEMENT_COUNT; j++) {
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
        struct AbilityDefinition *ability;
        struct Window *hint;
        s32 range;
        s32 element;
        s32 index;

        // Djinn: the party member's Djinn list; COMMAND_DJINN with the Djinni's
        // entry as parameter, kept in chosenDjinn so no later command takes it.
        Sound_PlayFar(SE_MENU_CONFIRM);
        BATTLE_MENU_SET_CURSOR(0, 0, 0);
      djinn_list:
        state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 144;
        gBattleMenuState->chosenDjinn[commandIndex] = BATTLE_MENU_NO_DJINNI;
        mode = BattleDjinnMenu_Run(unitId, 1);
        command->range = 0;
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = COMMAND_DJINN;
        parameter = mode;
        gBattleMenuState->chosenDjinn[commandIndex] = mode;
        element = DJINN_COMMAND_ELEMENT(parameter);
        index = DJINN_COMMAND_INDEX(parameter);
        if (Djinn_IsSetFar(unitId, element, index) != 0) {
          ability = Ability_Get(Djinn_GetAbilityIdFar(element, index));
          range = ability->range;
          commandWindow = Window_Create(11, 17, 10, 3, MENU_WINDOW_FLAGS);
          state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 64;
          Window_SetTilemapEntry(commandWindow,
                                 element + LIST_TILE_SUMMON_ELEMENT, 0, 0, 0);
          Text_DrawMessageAsTiles(element * DJINN_PER_ELEMENT + index +
                                      MsgDjinnName,
                                  commandWindow, 16, 0);
          command->range = range;
          wait(1);
          Sound_PlayFar(SE_MENU_CONFIRM);
          mode = BattleTarget_RunSelection(unitId, ability->targetMode, range,
                                           Battle_ClassifyEntryKind(ability));
          if (state->tutorialStep != 0) {
            hint = Message_ShowAndWait(MsgDjinnUnleashHint, 15, 8);
            while (Message_AllTyped() == 0) {
              wait(1);
            }
            Window_Close(hint, 1);
            state->tutorialStep++;
            state->tutorialDelay = 45;
          }
          Window_Close(commandWindow, 1);
          if (mode == -1) {
            goto djinn_list;
          }
          target = mode;
          break;
        }
        commandWindow = Window_Create(11, 17, 10, 3, MENU_WINDOW_FLAGS);
        state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 64;
        Text_SetColor(TEXT_COLOR_RED);
        Window_SetTilemapEntry(commandWindow,
                               element + LIST_TILE_SUMMON_ELEMENT, 0, 0, 0);
        Text_DrawMessageAsTiles(element * DJINN_PER_ELEMENT + index +
                                    MsgDjinnName,
                                commandWindow, 16, 0);
        Text_SetColor(TEXT_COLOR_WHITE);
        command->range = 1;
        wait(1);
        Sound_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(unitId, ABILITY_TARGET_SELF, 0,
                                         TARGET_HINT_SET_DJINNI);
        Window_Close(commandWindow, 1);
        if (mode == -1) {
          goto djinn_list;
        }
        target = mode;
        break;
      }

      case COMMAND_ICON_ITEM: {
        struct MenuSprite *iconSprite;
        struct AbilityDefinition *ability;
        u16 *list;
        u8 *pick;
        s32 range;
        s32 handle;
        s32 count;
        s32 k;
        s32 entryValue;
        s32 selection;

        // Item: the party member's 15 inventory places, the usable items first
        // and the rest after (pick keeps each one's place); then the item's
        // window and its action's target; COMMAND_ITEM with the place as
        // parameter.
        Sound_PlayFar(SE_MENU_CONFIRM);
        // DEAD: mode cleared before the list sets it; the ROM keeps its mov.
        mode = 0;
        BATTLE_MENU_SET_CURSOR(0, 0, 0);
      item_list:
        state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 96;
        pick = state->listSlots;
        list = state->listAbilities;
        count = 0;
        for (k = 0;
             k != INVENTORY_SLOTS && (entryValue = unit->inventory[k]) != 0;
             k++) {
          Item_GetDirectFar(entryValue);
          if (Item_CheckBattleUse(unitId, entryValue) == BATTLE_ITEM_USABLE) {
            list[count] = entryValue;
            pick[count++] = k;
          }
        }
        for (k = 0;
             k != INVENTORY_SLOTS && (entryValue = unit->inventory[k]) != 0;
             k++) {
          Item_GetDirectFar(entryValue);
          if (Item_CheckBattleUse(unitId, entryValue) != BATTLE_ITEM_USABLE) {
            list[count] = entryValue;
            pick[count++] = k;
          }
        }
        list[count] = 0;
        mode = BattleItemMenu_Run(unitId, list, count);
        if (mode == -1) {
          goto mark_visible;
        }
        selection = pick[mode];
        parameter = selection;
        ability = Ability_Get(
            Item_GetDirectFar(unit->inventory[selection])->abilityId);
        range = ability->range;
        iconSprite = &state->icons[BATTLE_MENU_ICON_ABILITY];
        handle = VramCache_AllocEntry(128);
        commandWindow = Window_Create(9, 17, BATTLE_ITEM_WINDOW_WIDTH, 3,
                                      MENU_WINDOW_FLAGS);
        state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 48;
        iconSprite->oam.word.attr01 =
            OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        iconSprite->oam.word.attr23 = 0;
        CAMELOT_ASM("an asm with no instruction, text unknown: the literal "
                    "pool after this sprite's stores sits where the ROM has it "
                    "only if exactly 4 more bytes are counted between the load "
                    "of the item icon's x (48) and the call to "
                    "ItemIcon_LoadIntoSlot than the instructions emitted, in "
                    "all six editions; only a one-line asm counts bytes and "
                    "emits nothing in this compiler; placed anywhere in that "
                    "window, this point is one choice")
        __asm__ volatile("");
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile =
            ItemIcon_LoadIntoSlot(unit->inventory[selection], handle);
        state->iconShown[BATTLE_MENU_ICON_ABILITY] = 1;
        Text_DrawMessageAsTiles(
            (unit->inventory[selection] & INVENTORY_ITEM_ID_MASK) + MsgItemName,
            commandWindow, 24, 0);
        command->range = range;
        Sound_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(unitId, ability->targetMode, range,
                                         Battle_ClassifyEntryKind(ability));
        state->iconShown[BATTLE_MENU_ICON_ABILITY] = 0;
        VramCache_FreeEntry(handle);
        Window_Close(commandWindow, 1);
        if (mode == -1) {
          goto item_list;
        }
        commandKind = COMMAND_ITEM;
        target = mode;
        break;
      }

      case COMMAND_ICON_DEFEND:
        // Defend: COMMAND_DEFEND, nothing to choose.
        commandKind = COMMAND_DEFEND;
        break;
      }

      Sound_PlayFar(SE_CHIME);
      command->unitId = unitId;
      // Draw the priority random value once, only for a nonzero agility.
      priority = unit->agility;
      if (priority != 0) {
        priority += (u32)(unit->agility * Random16()) >> 20;
      }
      command->priority = priority;
      if (commandIndex != 0 &&
          actors[commandIndex] == actors[commandIndex - 1]) {
        command->priority = (s16)priority / 2;
      }
      // A priority past 32767 reads negative as s16.
      if ((s16)command->priority < 0) {
        command->priority = OVERFLOWED_PRIORITY;
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
    Window_Close(state->timerWindow, 1);
  }
  VramCache_FreeEntry(smallIconSlot);
  VramCache_FreeEntry(largeIconSlot);
  Task_Remove(BattleMenu_Update);
  if (state->linkTimer != 0) {
    // Keep the completion wait visible while the peer finishes its commands.
    // UB: declared to return a value it never returns.
    s32 Link_DrawWaitingForOpponent(void) {
      u16 data[64];
      BATTLE_MENU_MESSAGE_COPY(MsgWaitingForOpponent, data,
                               BATTLE_MENU_MESSAGE_CAPACITY);
      Text_DrawWideStringAtOffset(data, state->timerWindow, 0, 4);
#ifdef UBFIX
      return 0;
#endif
    }

    // The wait message shows only when the countdown never opened its
    // window, that is while the partner is still choosing; a timer window
    // was closed above and is only forgotten here. mode counts the frames
    // without the link, and past LINK_MAX_MISSED_FRAMES gives up (-1).
    linkBattle = gBattleState;
    mode = 0;
    if (state->timerWindow == 0 && linkBattle->linkPaused == 0) {
      state->timerWindow =
          Window_Create(0, 16, 30, 4,
                        WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                            WINDOW_FLAG_BORDERED_CANVAS);
      Text_FillCanvasBackground();
      Link_DrawWaitingForOpponent();
    } else {
      state->timerWindow = 0;
    }
    for (;;) {
      peerSignature = gLinkPeerSignatures[linkBattle->linkSide ^ 1];
      if (linkBattle->linkPaused != 0) {
        commandCount = -1;
        break;
      }
      if ((gLinkStatus & LINK_STATUS_HEARD_BOTH) != LINK_STATUS_HEARD_BOTH) {
        mode++;
        if (mode > LINK_MAX_MISSED_FRAMES) {
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
      wait(1);
    }
    if (state->timerWindow != 0) {
      Window_Close(state->timerWindow, 1);
    }
  }
  BattleCamera_SetCommandViewFar(0);
  HeapSlot_Release(HEAP_SLOT_BATTLE_MENU);
  return commandCount;
}

#undef FLEE_PRIORITY
#undef OVERFLOWED_PRIORITY

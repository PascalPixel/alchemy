/// @file
/// Collecting the party's battle commands, then the icon menus: a row of
/// command icons with a label window, the selected icon pulsing, as the field
/// menu, the File Screen, the transfer screens, the shops, the sanctums, Yes or
/// No and Coins or Lucky Medals use it.
// Part of screens.c.

#include "window_internal.h"
#include "flags.h"
#include "sounds.h"
#include "menu/djinn_menu.h"
#include "system/affine.h"
#include "field/flag_call.h"
#include "gamepak/far_call/game.h"
#include "window/item_icon.h"
#include "window/menu_list.h"
#include "gamepak/far_call/battle.h"
#include "window/screens.h"
#include "window/window.h"
#include "resource_ids.h"

/// While a party member's commands are chosen (Battle_CollectPartyCommands),
/// where the Japanese layout differs: the chosen summon's window and the
/// columns of its first Djinn cost, the element icon and then the count, and
/// the chosen item's window, in tiles; the party member's portrait, and the
/// chosen command's icon beside the summon list, beside the chosen summon and
/// beside the Djinn list, in pixels.
#if LANGUAGE != LANGUAGE_JAPANESE
#define SUMMON_WINDOW_X 10
#define BATTLE_SUMMON_WINDOW_WIDTH 17
#define BATTLE_SUMMON_ELEMENT_COLUMN 11
#define BATTLE_SUMMON_COST_COLUMN 12
#define BATTLE_ITEM_WINDOW_WIDTH 16
#define BATTLE_PORTRAIT_X 0
#define BATTLE_SUMMON_ICON_X 80
#define SUMMON_CHOSEN_ICON_X 56
#define DJINN_MENU_ICON_X 140
#else
#define SUMMON_WINDOW_X 9
#define BATTLE_SUMMON_WINDOW_WIDTH 16
#define BATTLE_SUMMON_ELEMENT_COLUMN 10
#define BATTLE_SUMMON_COST_COLUMN 11
#define BATTLE_ITEM_WINDOW_WIDTH 15
#define BATTLE_PORTRAIT_X 4
#define BATTLE_SUMMON_ICON_X 88
#define SUMMON_CHOSEN_ICON_X 48
#define DJINN_MENU_ICON_X 144
#endif

/// Where "Attack" is written in its window once chosen, in pixels.
#if LANGUAGE == LANGUAGE_FRENCH
#define BATTLE_COMMAND_NAME_X 14
#else
#define BATTLE_COMMAND_NAME_X 16
#endif

/// The chosen command's icon beside the Psynergy list, in pixels.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
#define PSYNERGY_MENU_ICON_X 40
#else
#define PSYNERGY_MENU_ICON_X 48
#endif

/// The chosen command's icon beside the window that names Attack, the chosen
/// Psynergy, the chosen Djinni or the chosen item, and beside the item list, in
/// pixels.
#define ATTACK_ICON_X 64
#define PSYNERGY_CHOSEN_ICON_X 40
#define DJINN_CHOSEN_ICON_X 64
#define ITEM_MENU_ICON_X 96
#define ITEM_CHOSEN_ICON_X 48

/// Flee's priority, above every other action's: the party flees first in the
/// turn.
#define FLEE_PRIORITY 32766
/// The priority an action takes when its Agility roll runs past 32767.
#define OVERFLOWED_PRIORITY 2000

#include "system/heap_slots.h"
#include "system/serial.h"

void Graphics_ExpandVramTilesByColorTable(u16 *tiles);

/// The x of the first command icon's sprite.
#define COMMAND_ICON_X(state) ((state)->icons[0].oam.f.x)

/// Collects the party's commands for a turn into `commands`, one for each of
/// the `commandCount` units in `actors`, with two nested functions: the party
/// panels' clear and the link wait's message (Link_DrawWaitingForOpponent).
/// Unlike The Broken Seal's, each round first appends the forced actions of
/// statused members (BattleParty_SplitChoosers), whose count is added to the
/// result; B on the party's row undoes a reserve swap; Switch swaps a reserve
/// member in (BattleParty_ChooseReserve after choosing the slot); Status lists
/// the reserve as well; and the Djinn tutorial's hint follows an unleash.
s32 Battle_CollectPartyCommands(struct BattleAction *commands, u16 *actors,
                                s32 commandCount) {
  u16 highlighted[2];
  struct BattleMenuState *state;
  struct BattleAction *commandsOut;
  s32 forcedCount;
  struct BattleAction *command;
  struct Unit *unit;
  s32 target;
  s32 actorId;
  s32 commandKind;
  s32 parameter;
  s32 largeIconSlot;
  s32 smallIconSlot;
  s32 commandIndex;
  s32 summonIconSlot;
  s32 choice;
  struct Window *commandWindow;

  void Battle_DrawPartyPanelsWithEmptyList(void) {
    u16 data[2];

    data[0] = BATTLE_UNIT_LIST_END;
    PartyStatus_HighlightMembers(data);
    BattleHighlight_SetModesFar(data, HIGHLIGHT_SELECTED);
  }

  target = PLACE_PARTY;
  commandsOut = NULL;
  choice = 0;
  commandKind = 0;
  parameter = 0;
  largeIconSlot = VramCache_AllocEntry(COMMAND_ICON_BYTES);
  smallIconSlot = VramCache_AllocEntry(512);
  state = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_MENU,
                              sizeof(struct BattleMenuState));
  forcedCount = 0;
  CommandIcon_ReloadSlot(largeIconSlot, 0);
  Graphics_ExpandVramTilesByColorTable(
      (u16 *)(BG_CHAR_BLOCK(0) + PAGE_DIGITS_TILE * TILE_SIZE_4BPP));
  PageArrows_Draw(LIST_PAGE_ARROW_VRAM);
  Menu_BuildLocalizedPatternTiles();
  state->swappedUnit = -1;
  state->lastTarget = -1;
  {
    struct BattleState *battle = Ram_HeapSlots->battleState;
    s32 k;

    for (k = 0; k < 8; k++)
      state->chosenDjinn[k] = BATTLE_MENU_NO_DJINNI;
    for (k = 0; k < 3; k++)
      state->iconShown[k] = 0;
    state->targetViewOffset = 0;
    state->viewOffset = 0;
    state->commandSelection[0] = 0;
    state->commandSelection[1] = 0;
    state->linkTimer = 0;
    state->timerHidden = 0;
    state->timer = -1;
    state->timerWindow = 0;
    if (battle->isLink != 0) {
      s32 missedLinkFrames;
      u16 *signature;

      state->linkTimer = 1;
      signature = (u16 *)gSerialTransfer.payload;
      signature[4] = 'V';
      signature[5] = 'S';
      signature[6] = 'S';
      signature[7] = 'T';
      missedLinkFrames = 0;
      for (;;) {
        u16 *peerSignature = gLinkPeerSignatures[battle->linkSide ^ 1];

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
              peerSignature[6] == 'S' && peerSignature[7] == 'T')
            break;
          if (peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
              peerSignature[6] == 'V' && peerSignature[7] == 'S')
            break;
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
  }
  Task_Add(BattleMenu_UpdateEachFrame, TASK_PASS_FRAME);

  for (;;) {
    commandsOut = commands;
    if (commandCount >= 0) {
      forcedCount = 0;
      commandCount =
          BattleParty_SplitChoosers(actors, &commandsOut, &forcedCount);
    }
    Battle_DrawPartyPanelsWithEmptyList();
    state->iconShown[2] = 0;
    state->commandActor = -1;
    state->tutorialStep = 0;
    if (GameFlag_Get(FLAG_DJINN_LESSON)) {
      state->tutorialStep = 1;
      state->tutorialDelay = 60;
    }
    if (commandCount >= 0)
      choice = BattleCommandRow_Run(COMMAND_ROW_PARTY, commandCount);
    else
      choice = COMMAND_ICON_FIGHT;

    // B on the party's row: undo the last reserve swap.
    if (choice == -1) {
      s32 swapped = state->swappedUnit;
      struct BattleState *battle;
      s32 old;
      s32 shortSwapped;
      s32 k;

      if (swapped == -1)
        continue;
      battle = Ram_HeapSlots->battleState;
      old = battle->partyUnits[state->swappedPlace];
      battle->partyUnits[state->swappedPlace] = swapped;
      shortSwapped = (s16)swapped;
      for (k = 0; k < 8; k++) {
        if (gMainRoutineState.party[k] == shortSwapped)
          gMainRoutineState.party[k] = old;
        else if (gMainRoutineState.party[k] == old)
          gMainRoutineState.party[k] = shortSwapped;
      }
      PartyStatus_Draw(Ram_HeapSlots->battleState->partyStatus);
      BattleObject_PutFar();
      state->swappedUnit = -1;
      BattleSummon_UpdateAvailabilityFar();
      BATTLE_MENU_STATE->commandSelection[0] = 1;
      continue;
    }
    // Switch: choose a party place, then the reserve member for it.
    if (choice == COMMAND_ICON_SWITCH) {
      Sound_PlayFar(SE_MENU_CONFIRM);
    choose_place:
      choice = BattleTarget_RunSelection(0, ABILITY_TARGET_ALLIES, 1,
                                         TARGET_HINT_LAST_TARGET);
      if (choice == -1)
        continue;
      choice = BattleParty_ChooseReserve(choice);
      if (choice == -1)
        goto choose_place;
      BattleSummon_UpdateAvailabilityFar();
      BATTLE_MENU_STATE->commandSelection[0] = 0;
      continue;
    }
    // Status: the summary screens of the party and the reserve, or in the
    // debug build with Start held the enemies'.
    if (choice == COMMAND_ICON_STATUS) {
      u16 *block = Heap_Alloc(18);
      s32 side;
      s32 count;

      if (gDebugMode != 0 && (gInput.held & KEY_START))
        side = BATTLE_SIDE_MASK_ENEMIES;
      else
        side = BATTLE_SIDE_MASK_PARTY;
      count = BattleUnit_ListIdsFar(side, block);
      if (side == BATTLE_SIDE_MASK_PARTY)
        count += BattleParty_PrepareReserveOwnersFar(block + count);
      wait(1);
      BattleStatusScreen_Run(block, count, block[0]);
      Heap_Release(block);
      continue;
    }
    // Flee: one COMMAND_FLEE for the party, first in the turn.
    if (choice == COMMAND_ICON_FLEE) {
      if (Battle_RefuseFlee() != 0)
        continue;
      commandCount = 1;
      command = commands;
      command->unitId = actors[0];
      forcedCount = 0;
      command->priority = FLEE_PRIORITY;
      command->command = COMMAND_FLEE;
      command->parameter = 0;
      command->target = PLACE_PARTY;
      break;
    }
    if (choice != COMMAND_ICON_FIGHT)
      break;
    Sound_PlayFar(SE_CAST);
    commandIndex = 0;
    if (commandIndex >= commandCount)
      break;

    for (;;) {
      if (commandIndex == 0) {
        BattleDjinn_CountStandbyFar(0, state->standbyDjinn[0]);
      } else {
        s32 k;

        for (k = 0; k < 4; k++)
          state->standbyDjinn[commandIndex][k] =
              state->standbyDjinn[commandIndex - 1][k];
      }
      command = &commandsOut[commandIndex];
      actorId = actors[commandIndex];
      // UB: Unit_Get returns a pointer, not the cast's doubleword result.
#ifdef UBFIX
      unit = Unit_Get(actorId);
#else
      unit = (struct Unit *)(s32)((s64 (*)(s32))Unit_Get)(actorId);
#endif
      {
        struct MenuSprite *iconSprite;

        BATTLE_MENU_STATE->commandActor = actorId;
        BATTLE_MENU_STATE->commandSelection[1] = 0;
        iconSprite = &BATTLE_MENU_STATE->icons[2];
        iconSprite->oam.word.attr01 =
            OBJ_SIZE_32X32 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.tile = Portrait_Load(unit->enemyId, smallIconSlot);
        iconSprite->oam.f.x = BATTLE_PORTRAIT_X;
        iconSprite->oam.f.y = 128;
        iconSprite->oam.f.palette = PORTRAIT_PALETTE;
        state->iconShown[2] = 1;
      }

    mark_visible:
      Window_MarkVisibleTextTiles();
      {
        /* FAKEMATCH(temporary): navigation, a block's local holding the battle
         * menu's state for two stores; as BATTLE_MENU_STATE at each. */
        struct BattleMenuState *navigation = BATTLE_MENU_STATE;

        navigation->iconShown[0] = 0;
        navigation->chosenDjinn[commandIndex] = BATTLE_MENU_NO_DJINNI;
      }
      highlighted[0] = actorId;
      highlighted[1] = BATTLE_UNIT_LIST_END;
      BattleHighlight_SetModesFar(highlighted, HIGHLIGHT_SELECTED);
      PartyStatus_HighlightMembers(highlighted);
      wait(1);
      choice = BattleCommandRow_Run(COMMAND_ROW_MEMBER, 0);
      wait(1);
      if (choice == -2) {
        u16 *block = Heap_Alloc(12);
        s32 count = BattleUnit_ListIdsFar(BATTLE_SIDE_MASK_PARTY, block);

        state->iconShown[2] = 0;
        wait(1);
        BattleStatusScreen_Run(block, count, actorId);
        state->iconShown[2] = 1;
        Heap_Release(block);
        continue;
      }
      BattleHighlight_SetModesFar(highlighted, HIGHLIGHT_OFF);
      if (choice == -1) {
        if (commandIndex == 0)
          break;
        commandIndex--;
        continue;
      }
      {
        struct MenuSprite *iconSprite = &BATTLE_MENU_STATE->icons[0];

        if (BATTLE_MENU_STATE->timer == 0)
          choice = COMMAND_ICON_DEFEND;
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.word.attr01 = OBJ_SIZE_32X32 | OBJ_256_COLORS |
                                      OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        iconSprite->oam.f.tile = CommandIcon_ReloadSlot(largeIconSlot, choice);
        if (choice == COMMAND_ICON_DJINN)
          iconSprite->oam.f.x = 128;
        else
          iconSprite->oam.f.x = 96;
        iconSprite->oam.f.y = 136;
        state->iconShown[0] = 1;
      }

      switch (choice) {
      case COMMAND_ICON_ATTACK:
        commandWindow = Window_Create(11, 17, 11, 3,
                                      WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
        Text_DrawMessageAsTiles((s32)MsgCommandName, commandWindow,
                                BATTLE_COMMAND_NAME_X, 0);
        COMMAND_ICON_X(state) = ATTACK_ICON_X;
        Sound_PlayFar(SE_MENU_CONFIRM);
        choice = BattleTarget_RunSelection(actorId, ABILITY_TARGET_FOES, 1,
                                           TARGET_HINT_NONE);
        Window_Close(commandWindow, 1);
        if (choice == -1)
          goto mark_visible;
        commandKind = COMMAND_ATTACK;
        target = choice;
        command->range = 1;
        break;

      case COMMAND_ICON_PSYNERGY: {
        s32 k;
        struct MenuSprite *iconSprite;
        struct AbilityDefinition *ability;
        struct WindowState *windowState;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 count;
        s32 entryValue;

        Sound_PlayFar(SE_MENU_CONFIRM);
        // DEAD: choice cleared before the list sets it; the ROM keeps its mov.
        choice = 0;
        BATTLE_MENU_SET_CURSOR(0, 0, 0);
      psynergy_menu:
        list = state->listAbilities;
        pick = state->listSlots;
        COMMAND_ICON_X(state) = PSYNERGY_MENU_ICON_X;
        count = 0;
        for (k = 0; k != PSYNERGY_SLOTS &&
                    (entryValue = unit->psynergy[k].encodedAction &
                                  PSYNERGY_ABILITY_MASK) != 0;
             k++) {
          if (Ability_Get(entryValue)->flags & ABILITY_USABLE_IN_BATTLE) {
            pick[count] = k;
            list[count] = entryValue;
            count++;
          }
        }
        pick[count] = 0;
        list[count] = 0;
        choice = BattlePsynergyMenu_Run(actorId, list, count);
        if (choice == -1)
          goto mark_visible;
        parameter =
            unit->psynergy[pick[choice]].encodedAction & PSYNERGY_ABILITY_MASK;
        ability = Ability_Get(parameter);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        windowState = WINDOW_STATE;
        commandWindow = Window_Create(8, 17, 18, 3,
                                      WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
        COMMAND_ICON_X(state) = PSYNERGY_CHOSEN_ICON_X;
        iconSprite->oam.word.attr01 =
            OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.x = (s16)commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = (s16)commandWindow->y * 8 + 4;
        /* FAKEMATCH(barrier): an empty asm. */
        __asm__ volatile("");
        iconSprite->oam.f.tile = AbilityIcon_LoadIntoSlot(parameter, handle);
        state->iconShown[1] = 1;
        windowState->textPalette = BASIC_COLORS_PALETTE;
        if (ability->ppCost > unit->pp)
          Text_SetColor(TEXT_COLOR_RED);
        else if (unit->psySeal != 0)
          Text_SetColor(TEXT_COLOR_PINK);
        Text_DrawMessageAsTiles(parameter + (s32)MsgAbilityName, commandWindow,
                                16, 0);
        Text_DrawNumberAtOffset(ability->ppCost, 2, commandWindow, 104, 0);
        windowState->textPalette = WINDOW_PALETTE;
        Text_SetColor(TEXT_COLOR_WHITE);
        Window_SetTilemapEntry(commandWindow, WINDOW_TILE_PP_LABEL, 11, 0, 0);
        Window_SetTilemapEntry(commandWindow, WINDOW_TILE_PP_LABEL_END, 12, 0,
                               0);
        if (ability->element != ELEMENT_NONE)
          Window_SetTilemapEntry(
              commandWindow, ability->element + ELEMENT_ICON_TILE, 15, 0, 0);
        command->range = cost;
        Sound_PlayFar(SE_MENU_CONFIRM);
        choice = BattleTarget_RunSelection(actorId, ability->targetMode, cost,
                                           Battle_ClassifyEntryKind(ability));
        state->iconShown[1] = 0;
        VramCache_FreeEntry(handle);
        Window_Close(commandWindow, 1);
        if (choice == -1)
          goto psynergy_menu;
        commandKind = COMMAND_PSYNERGY;
        target = choice;
        break;
      }

      case COMMAND_ICON_SUMMON: {
        const struct SummonDefinition *summon;
        struct AbilityDefinition *ability;
        struct MenuSprite *iconSprite;
        s32 ok;
        s32 j;
        s32 n;

        Sound_PlayFar(SE_MENU_CONFIRM);
        BATTLE_MENU_SET_CURSOR(0, 0, 0);
      summon_menu:
        COMMAND_ICON_X(state) = BATTLE_SUMMON_ICON_X;
        PartyStatus_HighlightMembers(highlighted);
        choice = BattleSummonMenu_Run(0, 0, state->standbyDjinn[commandIndex]);
        PartyStatus_HighlightMembers(highlighted);
        if (choice == -1)
          goto mark_visible;
        commandKind = COMMAND_SUMMON;
        parameter = choice;
        wait(1);
        iconSprite = &state->icons[1];
        summon = SummonDefinition_Get(choice);
        ability = Ability_Get(summon->abilityId);
        summonIconSlot = VramCache_AllocEntry(128);
        commandWindow =
            Window_Create(SUMMON_WINDOW_X, 17, BATTLE_SUMMON_WINDOW_WIDTH, 3,
                          WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
        n = 0;
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] > state->standbyDjinn[commandIndex][j])
            break;
        }
        ok = j == 4;
        COMMAND_ICON_X(state) = SUMMON_CHOSEN_ICON_X;
        ((u32 *)&iconSprite->oam)[0] =
            OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        ((u32 *)&iconSprite->oam)[1] = 0;
        /* FAKEMATCH(barrier): an empty asm. */
        __asm__ volatile("");
        iconSprite->oam.f.x = (s16)commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = (s16)commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile = AbilityIcon_LoadIntoSlot(
            summon->abilityId & ACTION_ID_MASK, summonIconSlot);
        state->iconShown[1] = 1;
        if (ok == 0)
          Text_SetColor(TEXT_COLOR_RED);
        Text_DrawMessageAsTiles(SummonDefinition_Get(choice)->abilityId +
                                    (s32)MsgAbilityName,
                                commandWindow, 16, 0);
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] != 0) {
            Window_SetTilemapEntry(commandWindow, j + ELEMENT_ICON_TILE,
                                   n * 2 + BATTLE_SUMMON_ELEMENT_COLUMN, 0, 0);
            Window_PutGlyph(commandWindow, summon->djinnRequired[j] + '0',
                            n * 2 + BATTLE_SUMMON_COST_COLUMN, 0,
                            GLYPH_DRAW_CHARACTER);
            n++;
          }
        }
        if (ok != 0)
          Sound_PlayFar(SE_MENU_CONFIRM);
        else
          Sound_PlayFar(SE_MENU_ERROR);
        choice = BattleTarget_RunSelection(actorId, ability->targetMode,
                                           ability->range,
                                           Battle_ClassifyEntryKind(ability));
        command->range = ability->range;
        state->iconShown[1] = 0;
        VramCache_FreeEntry(summonIconSlot);
        Window_Close(commandWindow, 1);
        if (choice == -1)
          goto summon_menu;
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] > state->standbyDjinn[commandIndex][j]) {
            state->standbyDjinn[commandIndex][j] = 0;
            break;
          }
          state->standbyDjinn[commandIndex][j] -= summon->djinnRequired[j];
        }
        target = choice;
        break;
      }

      case COMMAND_ICON_DJINN: {
        struct AbilityDefinition *ability;
        s32 cost;
        s32 element;
        s32 index;

        Sound_PlayFar(SE_MENU_CONFIRM);
        BATTLE_MENU_SET_CURSOR(0, 0, 0);
      djinn_menu:
        COMMAND_ICON_X(state) = DJINN_MENU_ICON_X;
        BATTLE_MENU_STATE->chosenDjinn[commandIndex] = BATTLE_MENU_NO_DJINNI;
        choice = BattleDjinnMenu_Run(actorId, 1);
        command->range = 0;
        if (choice == -1)
          goto mark_visible;
        parameter = choice;
        commandKind = COMMAND_DJINN;
        BATTLE_MENU_STATE->chosenDjinn[commandIndex] = choice;
        element = DJINN_COMMAND_ELEMENT(parameter);
        index = DJINN_COMMAND_INDEX(parameter);
        if (Djinn_IsSetFar(actorId, element, index) != 0) {
          ability = Ability_Get(Djinn_GetAbilityIdFar(element, index));
          cost = ability->range;
          commandWindow = Window_Create(
              11, 17, 10, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
          COMMAND_ICON_X(state) = DJINN_CHOSEN_ICON_X;
          Window_SetTilemapEntry(commandWindow, element + ELEMENT_ICON_TILE, 0,
                                 0, 0);
          Text_DrawMessageAsTiles(element * DJINN_PER_ELEMENT + index +
                                      (s32)MsgDjinnName,
                                  commandWindow, 16, 0);
          command->range = cost;
          wait(1);
          Sound_PlayFar(SE_MENU_CONFIRM);
          choice = BattleTarget_RunSelection(actorId, ability->targetMode, cost,
                                             Battle_ClassifyEntryKind(ability));
          if (state->tutorialStep != 0) {
            void *hint =
                Message_ShowAndWait(Djinn_GetGiveTutorialMessage() +
                                        DJINN_TUTORIAL_LINE_UNLEASH_HINT,
                                    15, 8);

            while (Message_AllTyped() == 0)
              wait(1);
            Window_Close(hint, 1);
            state->tutorialStep++;
            state->tutorialDelay = 45;
          }
          Window_Close(commandWindow, 1);
          if (choice == -1)
            goto djinn_menu;
          target = choice;
          break;
        }
        commandWindow = Window_Create(11, 17, 10, 3,
                                      WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
        COMMAND_ICON_X(state) = DJINN_CHOSEN_ICON_X;
        Text_SetColor(TEXT_COLOR_RED);
        Window_SetTilemapEntry(commandWindow, element + ELEMENT_ICON_TILE, 0, 0,
                               0);
        Text_DrawMessageAsTiles(element * DJINN_PER_ELEMENT + index +
                                    (s32)MsgDjinnName,
                                commandWindow, 16, 0);
        Text_SetColor(TEXT_COLOR_WHITE);
        command->range = 1;
        wait(1);
        Sound_PlayFar(SE_MENU_CONFIRM);
        choice = BattleTarget_RunSelection(actorId, ABILITY_TARGET_SELF, 0,
                                           TARGET_HINT_SET_DJINNI);
        Window_Close(commandWindow, 1);
        if (choice == -1)
          goto djinn_menu;
        target = choice;
        break;
      }

      case COMMAND_ICON_ITEM: {
        s32 k;
        struct MenuSprite *iconSprite;
        struct AbilityDefinition *ability;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 count;
        s32 entryValue;
        s32 selection;

        Sound_PlayFar(SE_MENU_CONFIRM);
        // DEAD: choice cleared before the list sets it; the ROM keeps its mov.
        choice = 0;
        BATTLE_MENU_SET_CURSOR(0, 0, 0);
      item_menu:
        COMMAND_ICON_X(state) = ITEM_MENU_ICON_X;
        pick = state->listSlots;
        list = state->listAbilities;
        count = 0;
        for (k = 0;
             k != INVENTORY_SLOTS && (entryValue = unit->inventory[k]) != 0;
             k++) {
          Item_GetDirectFar(entryValue);
          if (Item_ClassifyUseAbility(actorId, entryValue) == 0) {
            list[count] = entryValue;
            pick[count++] = k;
          }
        }
        for (k = 0;
             k != INVENTORY_SLOTS && (entryValue = unit->inventory[k]) != 0;
             k++) {
          Item_GetDirectFar(entryValue);
          if (Item_ClassifyUseAbility(actorId, entryValue) != 0) {
            list[count] = entryValue;
            pick[count++] = k;
          }
        }
        list[count] = 0;
        choice = BattleItemMenu_Run(actorId, list, count);
        if (choice == -1)
          goto mark_visible;
        selection = pick[choice];
        parameter = selection;
        ability = Ability_Get(
            Item_GetDirectFar(unit->inventory[selection])->abilityId);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        commandWindow = Window_Create(9, 17, BATTLE_ITEM_WINDOW_WIDTH, 3,
                                      WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
        COMMAND_ICON_X(state) = ITEM_CHOSEN_ICON_X;
        iconSprite->oam.word.attr01 =
            OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        /* FAKEMATCH(barrier): an empty asm. */
        __asm__ volatile("");
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.x = (s16)commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = (s16)commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile =
            ItemIcon_LoadIntoSlot(unit->inventory[selection], handle);
        state->iconShown[1] = 1;
        Text_DrawMessageAsTiles(
            (unit->inventory[selection] & INVENTORY_ITEM_ID_MASK) +
                (s32)MsgItemName,
            commandWindow, 24, 0);
        command->range = cost;
        Sound_PlayFar(SE_MENU_CONFIRM);
        choice = BattleTarget_RunSelection(actorId, ability->targetMode, cost,
                                           Battle_ClassifyEntryKind(ability));
        state->iconShown[1] = 0;
        VramCache_FreeEntry(handle);
        Window_Close(commandWindow, 1);
        if (choice == -1)
          goto item_menu;
        commandKind = COMMAND_ITEM;
        target = choice;
        break;
      }

      case COMMAND_ICON_DEFEND:
        commandKind = COMMAND_DEFEND;
        break;
      }

      Sound_PlayFar(SE_CHIME);
      command->unitId = actorId;
      {
        s32 priority = unit->agility;

        if (priority != 0)
          priority += (u32)(unit->agility * Random16()) >> 20;
        command->priority = priority;
        if (commandIndex != 0 &&
            actors[commandIndex] == actors[commandIndex - 1])
          command->priority = (s16)priority / 2;
      }
      // A priority past 32767 reads negative.
      if (command->priority < 0)
        command->priority = OVERFLOWED_PRIORITY;
      command->command = commandKind;
      command->parameter = parameter;
      command->target = target;
      commandIndex++;
      if (commandIndex < commandCount)
        continue;
      break;
    }
    if (commandIndex < commandCount)
      continue;
    break;
  }

  if (state->linkTimer != 0) {
    u16 *signature = (u16 *)gSerialTransfer.payload;

    signature[4] = 'E';
    signature[5] = 'D';
    signature[6] = 'V';
    signature[7] = 'S';
  }
  if (state->timerWindow != NULL)
    Window_Close(state->timerWindow, 1);
  VramCache_FreeEntry(smallIconSlot);
  VramCache_FreeEntry(largeIconSlot);
  Task_Remove(BattleMenu_UpdateEachFrame);
  if (state->linkTimer != 0) {
    struct BattleState *link;

    void Link_DrawWaitingForOpponent(void) {
      u16 text[64];

      BATTLE_MENU_MESSAGE_COPY((s32)MsgWaitingForOpponent, text,
                               BATTLE_MENU_MESSAGE_CAPACITY);
      Text_DrawWideStringAtOffset(text, state->timerWindow, 0, 4);
    }

    // The wait message shows only when the countdown never opened its
    // window, that is while the partner is still choosing; a timer window
    // was closed above and is only forgotten here. choice counts the frames
    // without the link, and past LINK_MAX_MISSED_FRAMES gives up (-1).
    link = Ram_HeapSlots->battleState;
    choice = 0;
    if (state->timerWindow == NULL && link->linkPaused == 0) {
      state->timerWindow =
          Window_Create(0, 16, 30, 4,
                        WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                            WINDOW_FLAG_BORDERED_CANVAS);
      Text_FillCanvasBackground();
      Link_DrawWaitingForOpponent();
    } else {
      state->timerWindow = NULL;
    }
    for (;;) {
      u16 *peerSignature = gLinkPeerSignatures[link->linkSide ^ 1];

      if (link->linkPaused != 0) {
        commandCount = -1;
        break;
      }
      if ((gLinkStatus & LINK_STATUS_HEARD_BOTH) != LINK_STATUS_HEARD_BOTH) {
        choice++;
        if (choice > LINK_MAX_MISSED_FRAMES) {
          commandCount = -1;
          break;
        }
      } else {
        choice = 0;
        if (peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
            peerSignature[6] == 'V' && peerSignature[7] == 'S')
          break;
        if (!(peerSignature[4] == 'V' && peerSignature[5] == 'S' &&
              peerSignature[6] == 'S' && peerSignature[7] == 'T'))
          choice = 1;
      }
      wait(1);
    }
    if (state->timerWindow != NULL)
      Window_Close(state->timerWindow, 1);
  }
  BattleCamera_SetCommandViewFar(0);
  HeapSlot_Release(HEAP_SLOT_BATTLE_MENU);
  if (commandCount >= 0)
    commandCount += forcedCount;
  return commandCount;
}

/// The icon menus' bottom row, in tiles, where most of them stand; a Yes or No
/// may stand at the top, row 0, instead.
#define ICON_MENU_BOTTOM_ROW 17
/// The lines an icon row covers: its label window's three tiles.
#define ICON_ROW_LINES 24

/// How far the selected icon overhangs its place while it pulses at the 8.8
/// `scale`: on each side, half its 24-pixel width grown by the scale, rounded
/// up; and past its row, half its 32-pixel height grown by the scale, rounded,
/// and a line more.
#define PULSE_OVERHANG_X(scale) ((((scale) - Q_8_8(1)) * 12 + 255) / 256)
#define PULSE_OVERHANG_LINES(scale)                                            \
  ((((scale) - Q_8_8(1)) * 32 + 255) / 512 + 1)

/// Draws the icon menu's icons, a task of the icon menus (IconMenu_Open). Each
/// icon is a 32x32, 256-colour sprite, and one at x 0 is not drawn; the
/// selected one pulses, double-size, between 1.19 and 1.31 times its size, in
/// front of the others. While a screen transition runs, it widens window 1 over
/// the icon row so the icons show through: from the first icon on the row's
/// lines, keeping each line's right edge, and around the selected icon on the
/// lines it overhangs, below a row at the top or above one at the bottom.
void IconMenu_DrawIcons(void) {
  struct IconMenuState *iconMenuState;
  struct IconMenuEntry *entry;
  struct AffineTransform transform;
  s32 pulseScale;
  s32 affineIndex;
  s32 span;
  s32 spriteY;
  u32 lastLine;
  u32 index;
  // UB: edge steps from one line's window 1 edges to the next line's, past
  // the member it points into.
  u16 *edge;
  struct ScreenTransitionState *state;
  struct WindowBuffer *page;
  s32 halfWidth;

  iconMenuState = Ram_HeapSlots->iconMenu;
  entry = iconMenuState->entries;
  pulseScale = gWindowSprite_BounceScales[(gFrameCount * 2) & 31];
  state = Ram_HeapSlots->screenTransition;
  pulseScale = (pulseScale - Q_8_8(1)) / 4 + Q_8_8(1.1875);
  transform.scaleX = pulseScale;
  transform.scaleY = pulseScale;
  transform.angle = 0;
  affineIndex = OamMatrix_AllocFromTransform(&transform);
  // Pulse only the selection; the other icons keep their normal OAM shape.
  for (index = 0; index < iconMenuState->count; index++, entry++) {
    u32 *word;
    s32 drawLayer;
    s32 x;

    x = entry->x;
    if (x == 0)
      continue;
    if (index == iconMenuState->selection) {
      s32 left = x + pulseScale * 7 / 512 - 20;

      // The icon grows about the middle of its 64-pixel double-size box. On
      // the top row the box starts above the screen, its y wrapped within
      // OAM's 8 bits, and the icon's top edge stays at the screen's top.
      if (entry->y != 0)
        spriteY = entry->y + pulseScale * 3 / 256 - 20;
      else
        spriteY = (pulseScale * 15 / 256 - 30) & OBJ_Y_MASK;
      word = &entry->link;
      *word++ = 0;
      *word++ = OBJ_AFFINE_MATRIX(affineIndex) | spriteY | OBJ_X(left) |
                OBJ_SIZE_32X32 | OBJ_256_COLORS | OBJ_DOUBLE_SIZE | OBJ_AFFINE;
      *word = gVramCacheEntries[entry->slot].offset >> 5;
      drawLayer = OAM_DEPTH_WINDOW + 6;
    } else {
      u32 tileOffset;
      u32 attributes;

      word = &entry->link;
      spriteY = entry->y;
      *word++ = 0;
      attributes = spriteY | OBJ_X(x) | OBJ_SIZE_32X32 | OBJ_256_COLORS;
      tileOffset = gVramCacheEntries[entry->slot].offset;
      *word++ = attributes;
      *word = tileOffset >> 5;
      drawLayer = OAM_DEPTH_WINDOW + 5;
    }
    Oam_AddSpriteAtDepth(entry, drawLayer);
  }
  if (state == NULL || state->centreLine != 0 || iconMenuState->count == 0)
    return;
  page = (struct WindowBuffer *)state->pages[state->page];
  if (iconMenuState->row == 0) {
    span = iconMenuState->entries[iconMenuState->selection].x;
    halfWidth = PULSE_OVERHANG_X(pulseScale);
    span = ((span - halfWidth) << 8) + (span + halfWidth) + 23;
    lastLine =
        iconMenuState->entries[iconMenuState->selection].y + ICON_ROW_LINES;
    lastLine += PULSE_OVERHANG_LINES(pulseScale);
    edge = &page->lines[ICON_ROW_LINES].window1;
    index = ICON_ROW_LINES;
    if (index < lastLine) {
      do {
        *edge = (*edge & WIN_RANGE_END_MASK) | span;
        index++;
        edge += 2;
      } while (index < lastLine);
    }
    span = iconMenuState->entries[0].x;
    if (iconMenuState->selection == 0)
      span -= PULSE_OVERHANG_X(pulseScale);
    span <<= 8;
    edge = &page->lines[0].window1;
    for (index = 0; index < ICON_ROW_LINES; index++) {
      *edge = (*edge & WIN_RANGE_END_MASK) | span;
      edge += 2;
    }
  } else {
    s32 firstLine;

    span = iconMenuState->entries[iconMenuState->selection].x;
    halfWidth = PULSE_OVERHANG_X(pulseScale);
    span = ((span - halfWidth) << 8) + (span + halfWidth) + 23;
    firstLine = iconMenuState->entries[iconMenuState->selection].y;
    firstLine -= PULSE_OVERHANG_LINES(pulseScale);
    edge = &page->lines[firstLine].window1;
    index = firstLine;
    if (index < ICON_MENU_BOTTOM_ROW * 8) {
      do {
        *edge = (*edge & WIN_RANGE_END_MASK) | span;
        index++;
        edge += 2;
      } while (index < ICON_MENU_BOTTOM_ROW * 8);
    }
    span = iconMenuState->entries[0].x;
    if (iconMenuState->selection == 0)
      span -= PULSE_OVERHANG_X(pulseScale);
    span <<= 8;
    edge = &page->lines[ICON_MENU_BOTTOM_ROW * 8].window1;
    for (index = ICON_MENU_BOTTOM_ROW * 8; index < DISPLAY_HEIGHT; index++) {
      // firstLine, done with, carries the mask that keeps the right edge.
      firstLine = WIN_RANGE_END_MASK;
      *edge = (*edge & firstLine) | span;
      edge += 2;
    }
  }
}
#undef SUMMON_WINDOW_X
#undef BATTLE_SUMMON_WINDOW_WIDTH
#undef BATTLE_SUMMON_ELEMENT_COLUMN
#undef BATTLE_SUMMON_COST_COLUMN
#undef BATTLE_ITEM_WINDOW_WIDTH
#undef BATTLE_PORTRAIT_X
#undef BATTLE_SUMMON_ICON_X
#undef SUMMON_CHOSEN_ICON_X
#undef DJINN_MENU_ICON_X
#undef ATTACK_ICON_X
#undef PSYNERGY_CHOSEN_ICON_X
#undef DJINN_CHOSEN_ICON_X
#undef ITEM_MENU_ICON_X
#undef ITEM_CHOSEN_ICON_X
#undef FLEE_PRIORITY
#undef OVERFLOWED_PRIORITY
#undef BATTLE_COMMAND_NAME_X
#undef PSYNERGY_MENU_ICON_X
#undef COMMAND_ICON_X

/// Allocates and clears the icon menu's state, which lives in
/// HEAP_SLOT_ICON_MENU until IconMenu_Close frees it, and starts the task that
/// draws its icons. Every icon menu starts here.
void *IconMenu_Open(void) {
  void *state =
      HeapSlot_AllocEwram(HEAP_SLOT_ICON_MENU, sizeof(struct IconMenuState));

  Dma_Fill32(0, state, sizeof(struct IconMenuState));
  Task_Add((TaskFunc)IconMenu_DrawIcons, TASK_PASS_FRAME - 10);
  return state;
}

/// Stops drawing the icons, closes the label window and releases each loaded
/// icon before freeing the icon menu's state (HEAP_SLOT_ICON_MENU), then waits
/// a frame.
void IconMenu_Close(void) {
  struct Window *labelWindow;
  s32 i;
  struct IconMenuState *state;

  state = Ram_HeapSlots->iconMenu;
  Task_Remove(IconMenu_DrawIcons);
  labelWindow = state->window;
  if (labelWindow != 0) {
    Window_Close(labelWindow, 2);
  }
  for (i = 0; i < state->count; i++) {
    VramCache_FreeEntry(state->entries[i].slot);
  }
  HeapSlot_Release(HEAP_SLOT_ICON_MENU);
  wait(1U);
}

/// Selects an icon position with wrapped directional input. A returns the
/// position; B or Start returns -1. Redraws the label only after a move: the
/// message labelBase + position when the layout set a labelBase, else the
/// icon's command name.
s32 IconMenu_Run(s32 initial) {
  struct IconMenuState *state = Ram_HeapSlots->iconMenu;
  s32 resource;

  state->selection = initial;
redraw:
  WindowSprite_PrepareForRedraw(state->window);
  if (state->labelBase != 0)
    resource = state->labelBase + state->selection;
  else
    resource = state->commandIcons[state->selection] + (s32)MsgCommandName;
  Text_DrawMessageAsTiles(resource, state->window, 0, 0);
  for (;;) {
    wait(1);
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      return state->selection;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      return -1;
    }
    if (gInput.pressed & KEY_START) {
      Sound_PlayFar(SE_MENU_CANCEL);
      return -1;
    }
    if ((gInput.repeat & KEY_LEFT) || (gInput.repeat & KEY_UP)) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      state->selection--;
      if (state->selection < 0)
        state->selection = state->count - 1;
    } else if ((gInput.repeat & KEY_RIGHT) || (gInput.repeat & KEY_DOWN)) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      state->selection++;
      if (state->selection >= state->count)
        state->selection = 0;
    } else
      continue;
    goto redraw;
  }
}

/// Returns the distance between lhs and rhs, reusing their supplied difference.
static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs) {
  if (diff >= 0)
    return diff;
  return rhs - lhs;
}

/// Moves the selection from start to goal one icon at a time with the cursor
/// sound, redrawing the label each step and waiting
/// gMenu_SelectionStepDelays[icons still to go] frames at each (12 more at the
/// first), then 48 frames on the goal before the confirm sound: a menu choice
/// made for the player to watch (Menu_AnimateSelectionToEntry). Returns goal.
s32 IconMenu_AnimateTo(s32 start, s32 goal) {
  s16 resourceBase;
  s16 selection;
  s32 diff;
  s32 resourceId;
  s32 distance;
  s32 position;
  s32 step;
  s32 delay;
  const u8 *stepDelays;
  struct IconMenuState *state;

  state = Ram_HeapSlots->iconMenu;
  step = 1;
  delay = 12;
  state->selection = (s16)start;
  if (goal < start)
    step = -1;
  position = start;

  for (;;) {
    WindowSprite_PrepareForRedraw(state->window);
    resourceBase = state->labelBase;
    if (resourceBase != 0) {
      resourceId = resourceBase + state->selection;
    } else {
      resourceId = state->commandIcons[state->selection] + (s32)MsgCommandName;
    }
    Text_DrawMessageAsTiles(resourceId, state->window, 0, 0);

    selection = state->selection;
    stepDelays = gMenu_SelectionStepDelays;
    diff = selection - goal;
    distance = AbsoluteDifference(diff, selection, goal);
    wait(stepDelays[distance] + delay);

    if (position == goal)
      break;

    state->selection += step;
    Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    delay = 0;
    position += step;
  }

  wait(48);
  Sound_PlayFar(SE_MENU_CONFIRM);
  return goal;
}

/// Decodes command icon `index` into the VRAM cache entry `slot`, through a
/// block it allocates and frees.
void CommandIcon_LoadIntoSlot(s32 slot, s32 index) {
  s32 size = COMMAND_ICON_BYTES;
  void *buffer = Heap_Alloc(size);
  u16 *offsetTable = Resource_GetTableEntry(RESOURCE_COMMAND_ICONS);

  Decompress_ByteLz((void *)((u32)offsetTable + offsetTable[index]), buffer);
  VramCache_Load(slot, size, buffer);
  Heap_Release(buffer);
}

/// Appends a command icon while the menu has room for one (IconMenuState's six
/// entries). It starts on the bottom row; IconMenu_Center or
/// IconMenu_LayoutIcons places the row. Its VRAM cache entry is taken
/// unchecked.
void IconMenu_AddIcon(s32 icon) {
  struct IconMenuState *state;
  struct IconMenuEntry *entry;
  s16 index;
  s32 slot;
  s32 y = ICON_MENU_BOTTOM_ROW * 8;

  state = Ram_HeapSlots->iconMenu;
  index = state->count;
  if (index < (s32)ARRAY_COUNT(state->entries)) {
    state->count++;
    entry = &state->entries[index];
    slot = VramCache_FindFreeEntry();
    CommandIcon_LoadIntoSlot(slot, icon);
    entry->x = index * 24 + 32;
    entry->y = y;
    entry->slot = slot;
    state->commandIcons[index] = icon;
  }
}

/// Centres the icons and their label window together on tile row `row`. A
/// nonzero resourceBase is the first label message; otherwise each icon is
/// labelled by its command name. The icons stand three tiles apart and the
/// label window, `width` + 2 tiles wide and 3 tall, follows them; the centring
/// counts two thirds of the window's width.
void IconMenu_Center(s32 row, s32 width, s32 resourceBase) {
  struct IconMenuState *state = Ram_HeapSlots->iconMenu;
  s32 x;
  s32 i;
  s32 count;

  state->width = width + 2;
  state->labelBase = resourceBase;
  state->row = row;
  count = state->count;
  x = DISPLAY_TILE_WIDTH / 2 - (count * 3 + state->width * 2 / 3) / 2;
  for (i = 0; i < state->count; i++) {
    struct IconMenuEntry *entry = &state->entries[i];

    entry->x = x * 8;
    entry->y = row * 8;
    x += 3;
  }
  state->window = Window_Create(x, row, state->width, 3, WINDOW_FLAG_INSTANT);
}

/// Places the icons three tiles apart from tile (x, y), then opens their label
/// window, `width` + 2 tiles wide, just after them.
void IconMenu_LayoutIcons(s32 x, s32 y, s32 width, s32 resourceBase) {
  struct IconMenuState *state;
  s32 i;
  struct IconMenuEntry *entry;

  state = Ram_HeapSlots->iconMenu;
  state->width = width + 2;
  state->labelBase = resourceBase;
  state->row = y;
  for (i = 0; i < state->count; i++) {
    entry = &state->entries[i];
    entry->y = y << 3;
    entry->x = x << 3;
    x += 3;
  }
  state->window = Window_Create(x, y, state->width, 3, WINDOW_FLAG_INSTANT);
}

/// Reads one entry of a field menu table (gMenu_TopEntryPositionByCommand).
static __inline__ s32 IconMenu_ReadCommandPosition(s8 *table, s32 index) {
  return table[index];
}

/// The field menu's row of icons: Psynergy, Djinn, Item and Status, without
/// Djinn when the party has none. `selection` is the command to start on and
/// the result the command chosen (enum FieldMenuCommand), or -1 when
/// cancelled; two tables turn commands into icon positions and back.
s32 FieldMenu_SelectCommand(s32 selection) {
  s32 group = 0;
  s32 offset;
  s32 groupTriple;
  s32 result;
  s8 *commandPositions;

  if (Party_SumDjinnCountsFar(-1) == 0) {
    group = 1;
  }

  groupTriple = group * 3;
  commandPositions = gMenu_TopEntryPositionByCommand;
  offset = groupTriple << 1;
  selection =
      IconMenu_ReadCommandPosition(commandPositions, selection + offset) - 1;
  if (selection < 0) {
    selection = 0;
  }

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_PSYNERGY);
  if (group == 0) {
    IconMenu_AddIcon(COMMAND_ICON_DJINN);
  }
  IconMenu_AddIcon(COMMAND_ICON_ITEM);
  IconMenu_AddIcon(COMMAND_ICON_STATUS);
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, SELECT_MENU_WIDTH, 0);
  result = IconMenu_Run(selection);
  IconMenu_Close();

  if (result >= 0) {
    s8 *commands = gMenu_TopEntryCommandByPosition;
    s32 index;

    index = result + offset + 1;
    result = commands[index];
  }

  return result;
}

/// Opens the field command menu and animates from start to the command's icon.
/// Returns the final icon position after closing the menu.
s32 Menu_AnimateSelectionToEntry(s32 start, s32 command) {
  PartyStatus_OpenAndWait();
  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_PSYNERGY);
  IconMenu_AddIcon(COMMAND_ICON_DJINN);
  IconMenu_AddIcon(COMMAND_ICON_ITEM);
  IconMenu_AddIcon(COMMAND_ICON_STATUS);
  // Only Japanese centres the entries before selecting.
#if LANGUAGE == LANGUAGE_JAPANESE
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, SELECT_MENU_WIDTH, 0);
#endif
  command = IconMenu_AnimateTo(start, command - 1);
  IconMenu_Close();
  PartyStatus_CloseWithMenu();
  return command;
}

/// The File Screen's row of icons, from the save summaries: New Game,
/// Continue, Copy and Erase for one or two files (starting on Continue); no New
/// Game or Copy with three; with every file cleared no Continue, and only Erase
/// with three such files; then Battle and Update when they are enabled.
/// Returns the action chosen (enum FileAction) by its position; with Battle
/// off, the action after Erase is Update's. With no files it returns
/// SAVE_ACTION_NEW_GAME at once, and -1 when cancelled or the backup memory
/// cannot be read.
s32 FileList_SelectAction(void) {
  s32 fileState;
  s32 result;
  s32 initial;
  u32 group;

  group = 0;
  initial = 0;
  fileState = Save_ScanFileSummaries();
  if (fileState < 0)
    return -1;
  if (fileState == 0)
    return SAVE_ACTION_NEW_GAME;
  if (fileState == SAVE_FILE_COUNT)
    group = 1;
  else if (fileState == SAVE_SCAN_ALL_CLEARED + SAVE_FILE_COUNT)
    group = 2;
  else if (fileState > SAVE_SCAN_ALL_CLEARED)
    group = 3;
  else
    initial = 1;
  IconMenu_Open();
  if (group == 0 || group == 3)
    IconMenu_AddIcon(COMMAND_ICON_NEW_GAME);
  if (group <= 1)
    IconMenu_AddIcon(COMMAND_ICON_CONTINUE);
  if (group == 0 || group == 3)
    IconMenu_AddIcon(COMMAND_ICON_COPY);
  IconMenu_AddIcon(COMMAND_ICON_ERASE);
  if (gTitleBattleOptionEnabled != 0)
    IconMenu_AddIcon(COMMAND_ICON_BATTLE);
  if (gTitleUpdateOptionEnabled != 0)
    IconMenu_AddIcon(COMMAND_ICON_UPDATE);
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, TYPE_MENU_WIDTH, 0);
  result = IconMenu_Run(initial);
  IconMenu_Close();
  // The icon position's action, from the group's row of six.
  if (result >= 0) {
    u32 twice = group * 2;

    result = gMenu_SaveSlotActionByPosition[result + (twice + group) * 2];
  }
  if (gTitleBattleOptionEnabled == 0 && result == SAVE_ACTION_BATTLE)
    result = SAVE_ACTION_UPDATE;
  return result;
}

/// The label area is narrower in Japanese and wider in French.
#if LANGUAGE == LANGUAGE_JAPANESE
#define LABEL_RIGHT 80
#elif LANGUAGE == LANGUAGE_FRENCH
#define LABEL_RIGHT 160
#else
#define LABEL_RIGHT 128
#endif

/// When the chosen password level changes, clears the lower window's text area
/// (from x 8 to LABEL_RIGHT) and lists what a password of that level carries:
/// Gold five lines, Silver three, Bronze two.
void PasswordLevelMenu_DrawContents(void) {
  struct IconMenuState *state = Ram_HeapSlots->iconMenu;

  if (state->drawnSelection != state->selection) {
    state->drawnSelection = state->selection;
    Window_ClearInteriorTiles(state->lowerWindow, 8, 40, LABEL_RIGHT, 80);

    // The lines of what each password carries, from "Levels"
    // (MsgPasswordLevel).
    switch (state->selection) {
    case PASSWORD_LEVEL_GOLD: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawMessageAsTiles(text, state->lowerWindow, 8, 40);
      Text_DrawMessageAsTiles(text + 1, state->lowerWindow, 8, 48);
      Text_DrawMessageAsTiles(text + 2, state->lowerWindow, 8, 56);
      Text_DrawMessageAsTiles(text + 3, state->lowerWindow, 8, 64);
      text += 4;
      Text_DrawMessageAsTiles(text, state->lowerWindow, 8, 72);
      break;
    }
    case PASSWORD_LEVEL_SILVER: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawMessageAsTiles(text, state->lowerWindow, 8, 40);
      Text_DrawMessageAsTiles(text + 1, state->lowerWindow, 8, 48);
      text += 2;
      Text_DrawMessageAsTiles(text, state->lowerWindow, 8, 56);
      break;
    }
    case PASSWORD_LEVEL_BRONZE: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawMessageAsTiles(text++, state->lowerWindow, 8, 40);
      Text_DrawMessageAsTiles(text, state->lowerWindow, 8, 48);
      break;
    }
    }
  }
}

/// The Spanish descriptions are indented.
#if LANGUAGE == LANGUAGE_SPANISH
#define HELP_X 6
#else
#define HELP_X 0
#endif

/// Redraws the chosen transfer method's help in the lower window. Spanish,
/// French and Italian describe two methods, the password and everything else
/// as the cable; French clears the window with WindowSprite_ClearList and
/// redraws only when the description changes.
void TransferMethodMenu_DrawHelp(void) {
  struct IconMenuState *state = Ram_HeapSlots->iconMenu;
  s16 *shown = &state->drawnSelection;
  s16 *current = &state->selection;

#if LANGUAGE == LANGUAGE_FRENCH
  if (*shown != *current) {
    if (*current == TRANSFER_METHOD_PASSWORD) {
      WindowSprite_ClearList(state->lowerWindow);
      Text_DrawMessage((s32)MsgPasswordTransferHelp, state->lowerWindow, 8, 4);
      Text_DrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWindow, 0,
                       16);
    } else if (*shown == TRANSFER_METHOD_PASSWORD) {
      WindowSprite_ClearList(state->lowerWindow);
      Text_DrawMessage((s32)MsgCableTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWindow, 0,
                       16);
      Text_DrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWindow, 0,
                       28);
    }
    state->drawnSelection = state->selection;
  }
#else
  if (*shown != *current) {
    *shown = *current;
    WindowSprite_PrepareForRedraw(state->lowerWindow);
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
    if (*current == TRANSFER_METHOD_PASSWORD) {
      Text_DrawMessage((s32)MsgPasswordTransferHelp, state->lowerWindow, HELP_X,
                       4);
      Text_DrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWindow,
                       HELP_X, 16);
    } else {
      Text_DrawMessage((s32)MsgCableTransferHelp, state->lowerWindow, HELP_X,
                       4);
      Text_DrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWindow,
                       HELP_X, 16);
      Text_DrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWindow,
                       HELP_X, 28);
    }
#else
    switch (*current) {
    case TRANSFER_METHOD_PASSWORD:
      Text_DrawMessage((s32)MsgPasswordTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWindow, 0,
                       16);
      break;
    case TRANSFER_METHOD_CABLE:
      Text_DrawMessage((s32)MsgCableTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWindow, 0,
                       16);
      Text_DrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWindow, 0,
                       28);
      break;
    default:
      Text_DrawMessage((s32)MsgClearDataTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgClearDataTransferHelp + 1, state->lowerWindow, 0,
                       16);
      break;
    }
#endif
  }
#endif
}

/// The entries' width and the windows' places and widths, in tiles, and where
/// the lines of text start, in pixels, are fitted to each language's text.
#if LANGUAGE == LANGUAGE_JAPANESE
#define ENTRY_WIDTH 5
#define PASSWORD_UPPER_X 9
#define PASSWORD_UPPER_WIDTH 13
#define PASSWORD_LOWER_X 5
#define PASSWORD_LOWER_WIDTH 21
#define TRANSFER_UPPER_X 9
#define TRANSFER_UPPER_WIDTH 13
#define TRANSFER_TITLE_X 0
#define TRANSFER_LOWER_X 5
#define TRANSFER_LOWER_WIDTH 21
#elif LANGUAGE == LANGUAGE_GERMAN
#define ENTRY_WIDTH 7
#define PASSWORD_UPPER_X 7
#define PASSWORD_UPPER_WIDTH 16
#define PASSWORD_LOWER_X 4
#define PASSWORD_LOWER_WIDTH 22
#define TRANSFER_UPPER_X 6
#define TRANSFER_UPPER_WIDTH 18
#define TRANSFER_TITLE_X 6
#define TRANSFER_LOWER_X 2
#define TRANSFER_LOWER_WIDTH 26
#elif LANGUAGE == LANGUAGE_SPANISH
#define ENTRY_WIDTH 9
#define PASSWORD_UPPER_X 5
#define PASSWORD_UPPER_WIDTH 21
#define PASSWORD_LOWER_X 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X 4
#define TRANSFER_UPPER_WIDTH 22
#define TRANSFER_TITLE_X 2
#define TRANSFER_LOWER_X 1
#define TRANSFER_LOWER_WIDTH 28
#elif LANGUAGE == LANGUAGE_FRENCH
#define ENTRY_WIDTH 9
#define PASSWORD_UPPER_X 5
#define PASSWORD_UPPER_WIDTH 20
#define PASSWORD_LOWER_X 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X 5
#define TRANSFER_UPPER_WIDTH 20
#define TRANSFER_TITLE_X 2
#define TRANSFER_LOWER_X 1
#define TRANSFER_LOWER_WIDTH 28
#elif LANGUAGE == LANGUAGE_ITALIAN
#define ENTRY_WIDTH 7
#define PASSWORD_UPPER_X 7
#define PASSWORD_UPPER_WIDTH 17
#define PASSWORD_LOWER_X 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X 5
#define TRANSFER_UPPER_WIDTH 20
#define TRANSFER_TITLE_X 2
#define TRANSFER_LOWER_X 1
#define TRANSFER_LOWER_WIDTH 28
#else
#define ENTRY_WIDTH 7
#define PASSWORD_UPPER_X 8
#define PASSWORD_UPPER_WIDTH 14
#define PASSWORD_LOWER_X 4
#define PASSWORD_LOWER_WIDTH 22
#define TRANSFER_UPPER_X 6
#define TRANSFER_UPPER_WIDTH 18
#define TRANSFER_TITLE_X 12
#define TRANSFER_LOWER_X 5
#define TRANSFER_LOWER_WIDTH 21
#endif

/// Spanish, French and Italian indent the password's lines and set them
/// closer together.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define PASSWORD_TEXT_X 8
#define PASSWORD_TEXT_Y 0
#define PASSWORD_LINE_HEIGHT 11
#else
#define PASSWORD_TEXT_X 0
#define PASSWORD_TEXT_Y 4
#define PASSWORD_LINE_HEIGHT 12
#endif

/// Selects the transfer method or the password level (`mode`, enum
/// SendOptionMenu), with a heading window above and an explanation of the
/// selected icon below, redrawn by a task while the menu runs. Returns the
/// place chosen, or -1 when cancelled.
s32 IconMenu_ChooseSendOption(s32 mode) {
  struct IconMenuState *state;
  struct Window *window;
  s32 msg;
  struct Window **upper;
  s32 result;

  mode &= 1;
  IconMenu_Open();
  state = Ram_HeapSlots->iconMenu;
  if (mode == SEND_OPTION_TRANSFER_METHOD) {
    IconMenu_AddIcon(COMMAND_ICON_PASSWORD);
    IconMenu_AddIcon(COMMAND_ICON_LINK);
  } else {
    IconMenu_AddIcon(COMMAND_ICON_GOLD);
    IconMenu_AddIcon(COMMAND_ICON_SILVER);
    IconMenu_AddIcon(COMMAND_ICON_BRONZE);
  }
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, ENTRY_WIDTH, 0);
  if (mode != SEND_OPTION_TRANSFER_METHOD) {
    Task_Add(PasswordLevelMenu_DrawContents, TASK_PASS_FRAME - 10);
    state->drawnSelection = -1;
    window = Window_Create(PASSWORD_UPPER_X, 0, PASSWORD_UPPER_WIDTH, 4,
                           WINDOW_FLAG_INSTANT);
    msg = (s32)MsgPasswordEntry;
    upper = &state->upperWindow;
    *upper = window;
    Text_DrawMessage(msg, window, 0, 4);
    window = Window_Create(PASSWORD_LOWER_X, 4, PASSWORD_LOWER_WIDTH, 12,
                           WINDOW_FLAG_INSTANT);
    state->lowerWindow = window;
    Text_DrawMessage(msg + 1, window, PASSWORD_TEXT_X, PASSWORD_TEXT_Y);
#if LANGUAGE == LANGUAGE_FRENCH
    // French writes a third line: "Niveaux" (MsgPasswordLevel).
    Text_DrawMessage(msg + 2, state->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT);
    msg += 3;
    Text_DrawMessage(msg, state->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT * 2);
#else
    msg += 2;
    Text_DrawMessage(msg, state->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT);
#endif
  } else {
    Task_Add(TransferMethodMenu_DrawHelp, TASK_PASS_FRAME - 10);
    state->drawnSelection = -1;
    window = Window_Create(TRANSFER_UPPER_X, 0, TRANSFER_UPPER_WIDTH, 4,
                           WINDOW_FLAG_INSTANT);
    upper = &state->upperWindow;
    *upper = window;
    Text_DrawMessage((s32)MsgTransferMethod, window, TRANSFER_TITLE_X, 4);
    state->lowerWindow = Window_Create(
        TRANSFER_LOWER_X, 5, TRANSFER_LOWER_WIDTH, 7, WINDOW_FLAG_INSTANT);
  }
  result = IconMenu_Run(0);
  if (mode != SEND_OPTION_TRANSFER_METHOD)
    Task_Remove(PasswordLevelMenu_DrawContents);
  else
    Task_Remove(TransferMethodMenu_DrawHelp);
  WindowSprite_PrepareForRedraw(*upper);
  WindowSprite_PrepareForRedraw(state->lowerWindow);
  Window_Close(*upper, 2);
  Window_Close(state->lowerWindow, 2);
  wait(1);
  IconMenu_Close();
  return result;
}

/// Selects a shop command (Buy, Sell, Artifacts, Repair), centred on the bottom
/// row, starting on position `selection`, and returns the position chosen.
s32 Menu_SelectShopCommand(s32 selection) {
  s32 result;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_BUY);
  IconMenu_AddIcon(COMMAND_ICON_SELL);
  IconMenu_AddIcon(COMMAND_ICON_ARTIFACTS);
  IconMenu_AddIcon(COMMAND_ICON_REPAIR);
#if (LANGUAGE == LANGUAGE_GERMAN)
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, 8, 0);
#elif (LANGUAGE != LANGUAGE_JAPANESE)
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, 7, 0);
#else
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, 6, 0);
#endif
  result = IconMenu_Run(selection);
  IconMenu_Close();
  return result;
}

/// Selects a Sanctum service (Revive, Cure Poison, Repel Evil, Remove Curse),
/// centred on the bottom row, starting on position `selection`.
s32 Menu_SelectSanctumService(s32 selection) {
  s32 result;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_REVIVE);
  IconMenu_AddIcon(COMMAND_ICON_CURE_POISON);
  IconMenu_AddIcon(COMMAND_ICON_REPEL_EVIL);
  IconMenu_AddIcon(COMMAND_ICON_REMOVE_CURSE);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, 10, 0);
#else
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, 8, 0);
#endif
  result = IconMenu_Run(selection);
  IconMenu_Close();
  return result;
}

/// Selects Yes or No, starting on `selection`; cancelling answers No. The icons
/// stand on the bottom row when `bottom` is set, else at the top; `message` is
/// the first label message, or 0 for the command names, and `labelWidth` the
/// label window's width, 3 when 0.
s32 YesNoMenu_Run(s32 bottom, s32 message, s32 labelWidth, s32 selection) {
  s32 row;

  row = 0;

  IconMenu_Open();

  if (labelWidth == 0) {
    labelWidth = 3;
  }
  if (bottom != 0) {
    row = ICON_MENU_BOTTOM_ROW;
  }

  IconMenu_AddIcon(COMMAND_ICON_YES);
  IconMenu_AddIcon(COMMAND_ICON_NO);
  IconMenu_Center(row, labelWidth, message);

  selection = IconMenu_Run(selection);
  IconMenu_Close();
  if (selection == -1) {
    selection = NO;
  }
  return selection;
}

/// Selects Yes or No with the icons from tile (x, y), starting on
/// `selection`; cancelling answers No.
s32 YesNoMenu_RunAt(s32 x, s32 y, s32 selection) {
  s32 message = MsgCommandYes;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_YES);
  IconMenu_AddIcon(COMMAND_ICON_NO);
  IconMenu_LayoutIcons(x, y, 3, message);
  selection = IconMenu_Run(selection);
  IconMenu_Close();
  if (selection == -1) {
    selection = NO;
  }
  return selection;
}

/// The label window's width for each language.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PAIR_WIDTH 7
#elif (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

/// Selects Coins or Lucky Medals, centred on the bottom row, starting on
/// `selection`, and returns the position chosen.
s32 Menu_SelectCoinsOrMedals(s32 selection) {
  s32 result;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_COINS);
  IconMenu_AddIcon(COMMAND_ICON_LUCKY_MEDALS);
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, PAIR_WIDTH, 0);
  result = IconMenu_Run(selection);
  IconMenu_Close();
  return result;
}

/// Unused: starts the debug scene and entrance selector as a task.
UNUSED void DebugSceneSelect_Schedule(void) {
  Task_Add(DebugSceneSelect_Run, TASK_PASS_FRAME);
}
#undef LABEL_RIGHT
#undef HELP_X
#undef ENTRY_WIDTH
#undef PASSWORD_UPPER_X
#undef PASSWORD_UPPER_WIDTH
#undef PASSWORD_LOWER_X
#undef PASSWORD_LOWER_WIDTH
#undef TRANSFER_UPPER_X
#undef TRANSFER_UPPER_WIDTH
#undef TRANSFER_TITLE_X
#undef TRANSFER_LOWER_X
#undef TRANSFER_LOWER_WIDTH
#undef PASSWORD_TEXT_X
#undef PASSWORD_TEXT_Y
#undef PASSWORD_LINE_HEIGHT
#undef PAIR_WIDTH
#undef ICON_MENU_BOTTOM_ROW
#undef ICON_ROW_LINES
#undef PULSE_OVERHANG_X
#undef PULSE_OVERHANG_LINES

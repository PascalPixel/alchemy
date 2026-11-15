/// @file
/// Collecting the party's battle commands on this game's work and menus, then
/// IconMenu_DrawIcons, which draws a menu's icons each frame, the selected one
/// bouncing, and fits the window's lines around it. Then the resource selection
/// menus: shop commands, sanctum services, Yes or No, coins or medals, and the
/// save screen's actions.
// Part of window.c.

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

extern u16 gData_02003a74[];

// Where the Japanese layout differs: the summon window, the item window and
// the summon's Djinn costs, in tiles; the x of the acting member's icon and of
// the command icon in the summon list, beside a chosen summon and in the Djinn
// list, in pixels.
#if LANGUAGE != LANGUAGE_JAPANESE
#define SUMMON_WINDOW_X 10
#define SUMMON_WINDOW_WIDTH 17
#define SUMMON_COST_X 11
#define ITEM_WINDOW_WIDTH 16
#define ACTOR_ICON_X 0
#define SUMMON_MENU_ICON_X 80
#define SUMMON_CHOSEN_ICON_X 56
#define DJINN_MENU_ICON_X 140
#else
#define SUMMON_WINDOW_X 9
#define SUMMON_WINDOW_WIDTH 16
#define SUMMON_COST_X 10
#define ITEM_WINDOW_WIDTH 15
#define ACTOR_ICON_X 4
#define SUMMON_MENU_ICON_X 88
#define SUMMON_CHOSEN_ICON_X 48
#define DJINN_MENU_ICON_X 144
#endif

// The x of the Attack window's name, in pixels.
#if LANGUAGE == LANGUAGE_FRENCH
#define ATTACK_NAME_X 14
#else
#define ATTACK_NAME_X 16
#endif

// The x of the command icon in the Psynergy list, in pixels.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
#define PSYNERGY_MENU_ICON_X 40
#else
#define PSYNERGY_MENU_ICON_X 48
#endif

#define NAVIGATION ((struct BattleMenuWork *)Ram_HeapSlots->battleMenuWork)

#include "system/heap_slots.h"
#include "system/serial.h"

void Graphics_ExpandVramTilesByColorTable(u16 *tiles);
#define SET_CURSOR(page_, row_, preferred_)                                    \
  do {                                                                         \
    NAVIGATION->page = (page_);                                                \
    NAVIGATION->row = (row_);                                                  \
    NAVIGATION->preferredRow = (preferred_);                                   \
  } while (0)

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
  u16 signatureHeader[2];
  struct BattleMenuWork *state;
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
  s32 mode;
  struct Window *commandWindow;

  void Battle_DrawPartyPanelsWithEmptyList(void) {
    u16 data[2];

    data[0] = BATTLE_UNIT_LIST_END;
    PartyStatus_HighlightMembers(data);
    BattleHighlight_SetModesFar(data, HIGHLIGHT_SELECTED);
  }

  target = 256;
  // Default: set for real at the top of each round.
  commandsOut = NULL;
  // Default: each is set for real by the row it reaches.
  mode = 0;
  commandKind = 0;
  parameter = 0;
  largeIconSlot = VramCache_AllocEntry(1024);
  smallIconSlot = VramCache_AllocEntry(512);
  state =
      HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_MENU, sizeof(struct BattleMenuWork));
  // Default: set again before each round's forced actions.
  forcedCount = 0;
  Resource_LoadIndexedIntoBuffer(largeIconSlot, 0);
  Graphics_ExpandVramTilesByColorTable(
      (u16 *)(BG_CHAR_BLOCK(0) + LIST_PAGE_DIGITS_TILE * TILE_SIZE_4BPP));
  Link_DrawPageArrows(LIST_PAGE_ARROW_VRAM);
  Menu_BuildLocalizedPatternTiles();
  state->swappedUnit = -1;
  state->lastTarget = -1;
  {
    struct BattleWork *battle = Ram_HeapSlots->battleWork;
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
      signature = gData_02003a74;
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
          if (++missedLinkFrames > 24) {
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
  Task_Add(UpdateLinkSessionCountdown, TASK_PASS_VBLANK);

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
      mode = BattleCommandRow_Run(0, commandCount);
    else
      mode = COMMAND_ICON_FIGHT;

    // B on the party's row: undo the last reserve swap.
    if (mode == -1) {
      s32 swapped = state->swappedUnit;
      struct BattleWork *battle;
      s32 old;
      s32 shortSwapped;
      s32 k;

      if (swapped == mode)
        continue;
      battle = Ram_HeapSlots->battleWork;
      old = battle->partyUnits[state->swappedPlace];
      battle->partyUnits[state->swappedPlace] = swapped;
      shortSwapped = (s16)swapped;
      for (k = 0; k < 8; k++) {
        if (gMainRoutineWork.party[k] == shortSwapped)
          gMainRoutineWork.party[k] = old;
        else if (gMainRoutineWork.party[k] == old)
          gMainRoutineWork.party[k] = shortSwapped;
      }
      PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
      BattleObject_PutFar();
      state->swappedUnit = -1;
      BattleSummon_UpdateAvailabilityFar();
      NAVIGATION->commandSelection[0] = 1;
      continue;
    }
    // Switch: choose a party place, then the reserve member for it.
    if (mode == COMMAND_ICON_SWITCH) {
      Sound_PlayFar(SE_MENU_CONFIRM);
    choose_place:
      mode = BattleTarget_RunSelection(0, 2, 1, 8);
      if (mode == -1)
        continue;
      mode = BattleParty_ChooseReserve(mode);
      if (mode == -1)
        goto choose_place;
      BattleSummon_UpdateAvailabilityFar();
      NAVIGATION->commandSelection[0] = 0;
      continue;
    }
    // Status: the summary screens of the party and the reserve, or in the
    // debug build with Start held the enemies'.
    if (mode == COMMAND_ICON_STATUS) {
      u16 *block = Heap_Alloc(18);
      s32 side;
      s32 count;

      if (gDebugMode != 0 && (gInput.held & KEY_START))
        side = BATTLE_SIDE_MASK_ENEMIES;
      else
        side = BATTLE_SIDE_MASK_PARTY;
      count = BattleUnit_ListIdsFar(side, block);
      if (side == 1)
        count += BattleParty_PrepareReserveOwnersFar(block + count);
      wait(1);
      BattleStatusScreen_Run(block, count, block[0]);
      Heap_Release(block);
      continue;
    }
    // Flee: one COMMAND_FLEE for the party, first in the turn (priority
    // 0x7ffe).
    if (mode == COMMAND_ICON_FLEE) {
      s32 refused = Battle_RefuseFlee();

      if (refused != 0)
        continue;
      commandCount = 1;
      command = commands;
      command->unitId = actors[0];
      forcedCount = refused;
      command->priority = 0x7ffe;
      command->command = COMMAND_FLEE;
      command->parameter = refused;
      command->target = 256;
      break;
    }
    if (mode != COMMAND_ICON_FIGHT)
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
        struct BattleMenuWork *navigation = NAVIGATION;
        struct MenuSprite *iconSprite;

        navigation->commandActor = actorId;
        navigation->commandSelection[1] = 0;
        iconSprite = &navigation->icons[2];
        iconSprite->oam.word.attr01 =
            OBJ_SIZE_32X32 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.tile = Portrait_Load(unit->enemyId, smallIconSlot);
        iconSprite->oam.f.x = ACTOR_ICON_X;
        iconSprite->oam.f.y = 128;
        iconSprite->oam.f.palette = PORTRAIT_PALETTE;
        state->iconShown[2] = 1;
      }

    mark_visible:
      Window_MarkVisibleTextTiles();
      {
        struct BattleMenuWork *navigation = NAVIGATION;

        navigation->iconShown[0] = 0;
        navigation->chosenDjinn[commandIndex] = BATTLE_MENU_NO_DJINNI;
      }
      signatureHeader[0] = actorId;
      signatureHeader[1] = BATTLE_UNIT_LIST_END;
      BattleHighlight_SetModesFar(signatureHeader, HIGHLIGHT_SELECTED);
      PartyStatus_HighlightMembers(signatureHeader);
      wait(1);
      mode = BattleCommandRow_Run(1, 0);
      wait(1);
      if (mode == -2) {
        u16 *block = Heap_Alloc(12);
        s32 count = BattleUnit_ListIdsFar(1, block);

        state->iconShown[2] = 0;
        wait(1);
        BattleStatusScreen_Run(block, count, actorId);
        state->iconShown[2] = 1;
        Heap_Release(block);
        continue;
      }
      BattleHighlight_SetModesFar(signatureHeader, HIGHLIGHT_OFF);
      if (mode == -1) {
        if (commandIndex == 0)
          break;
        commandIndex--;
        continue;
      }
      {
        struct BattleMenuWork *navigation = NAVIGATION;
        struct MenuSprite *iconSprite = &navigation->icons[0];

        if (navigation->timer == 0)
          mode = COMMAND_ICON_DEFEND;
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.word.attr01 = OBJ_SIZE_32X32 | OBJ_256_COLORS |
                                      OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        iconSprite->oam.f.tile =
            Resource_LoadIndexedIntoBuffer(largeIconSlot, mode);
        if (mode == COMMAND_ICON_DJINN)
          iconSprite->oam.f.x = 128;
        else
          iconSprite->oam.f.x = 96;
        iconSprite->oam.f.y = 136;
        state->iconShown[0] = 1;
      }

      switch (mode) {
      case COMMAND_ICON_ATTACK:
        commandWindow = Window_Create(11, 17, 11, 3, 6);
        Text_DrawMessageAsTiles((s32)MsgCommandName, commandWindow,
                                ATTACK_NAME_X, 0);
        COMMAND_ICON_X(state) = 64;
        Sound_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(actorId, 1, 1, TARGET_HINT_NONE);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto mark_visible;
        commandKind = COMMAND_ATTACK;
        target = mode;
        command->range = 1;
        break;

      case COMMAND_ICON_PSYNERGY: {
        s32 k;
        struct MenuSprite *iconSprite;
        struct BattleAct *ability;
        struct WindowWork *work;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 cnt;
        s32 entryValue;

        Sound_PlayFar(SE_MENU_CONFIRM);
        mode = 0;
        SET_CURSOR(mode, mode, mode);
      psynergy_menu:
        list = state->listAbilities;
        pick = state->listSlots;
        COMMAND_ICON_X(state) = PSYNERGY_MENU_ICON_X;
        cnt = 0;
        k = 0;
        entryValue = unit->psynergy[k].encodedAction & PSYNERGY_ABILITY_MASK;
        if (entryValue != 0) {
          do {
            if (Ability_Get(entryValue)->flags & ABILITY_USABLE_IN_BATTLE) {
              pick[cnt] = k;
              list[cnt] = entryValue;
              cnt++;
            }
            k++;
            if (k == PSYNERGY_SLOTS)
              break;
            entryValue =
                unit->psynergy[k].encodedAction & PSYNERGY_ABILITY_MASK;
          } while (entryValue != 0);
        }
        pick[cnt] = 0;
        list[cnt] = 0;
        mode = BattlePsynergyMenu_Run(actorId, list, cnt);
        if (mode == -1)
          goto mark_visible;
        parameter =
            unit->psynergy[pick[mode]].encodedAction & PSYNERGY_ABILITY_MASK;
        ability = Ability_Get(parameter);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        work = (struct WindowWork *)Ram_HeapSlots->windowWork;
        commandWindow = Window_Create(8, 17, 18, 3, 6);
        COMMAND_ICON_X(state) = 40;
        iconSprite->oam.word.attr01 =
            OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.x = (s16)commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = (s16)commandWindow->y * 8 + 4;
        /* FAKEMATCH(barrier): counted as 4 bytes. */
        __asm__ volatile("");
        iconSprite->oam.f.tile =
            Resource_LoadIndexedEntryToBuffer(parameter, handle);
        state->iconShown[1] = 1;
        work->textPalette = BASIC_COLORS_PALETTE;
        if (ability->ppCost > unit->pp)
          Text_SetColor(TEXT_COLOR_RED);
        else if (unit->psySeal != 0)
          Text_SetColor(TEXT_COLOR_PINK);
        Text_DrawMessageAsTiles(parameter + (s32)MsgAbilityName, commandWindow,
                                16, 0);
        Text_DrawNumberAtOffset(ability->ppCost, 2, commandWindow, 104, 0);
        work->textPalette = WINDOW_PALETTE;
        Text_SetColor(TEXT_COLOR_WHITE);
        Window_SetTilemapEntry(commandWindow, WINDOW_TILE_PP_LABEL, 11, 0, 0);
        Window_SetTilemapEntry(commandWindow, WINDOW_TILE_PP_LABEL_END, 12, 0,
                               0);
        if (ability->element != ELEMENT_NONE)
          Window_SetTilemapEntry(
              commandWindow, ability->element + ELEMENT_ICON_TILE, 15, 0, 0);
        command->range = cost;
        Sound_PlayFar(SE_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(actorId, ability->targetMode, cost,
                                         Battle_ClassifyEntryKind(ability));
        state->iconShown[1] = 0;
        VramCache_FreeEntry(handle);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto psynergy_menu;
        commandKind = COMMAND_PSYNERGY;
        target = mode;
        break;
      }

      case COMMAND_ICON_SUMMON: {
        const struct SummonDefinition *summon;
        struct BattleAct *ability;
        struct MenuSprite *iconSprite;
        s32 ok;
        s32 j;
        s32 n;

        Sound_PlayFar(SE_MENU_CONFIRM);
        SET_CURSOR(0, 0, 0);
      summon_menu:
        COMMAND_ICON_X(state) = SUMMON_MENU_ICON_X;
        PartyStatus_HighlightMembers(signatureHeader);
        mode = BattleSummonMenu_Run(0, 0, state->standbyDjinn[commandIndex]);
        PartyStatus_HighlightMembers(signatureHeader);
        if (mode == -1)
          goto mark_visible;
        commandKind = COMMAND_SUMMON;
        parameter = mode;
        wait(1);
        iconSprite = &state->icons[1];
        summon = SummonDefinition_Get(mode);
        ability = Ability_Get(summon->abilityId);
        summonIconSlot = VramCache_AllocEntry(128);
        commandWindow = Window_Create(SUMMON_WINDOW_X, 17, SUMMON_WINDOW_WIDTH,
                                      3, commandKind);
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
        /* FAKEMATCH(barrier): counted as 4 bytes. */
        __asm__ volatile("");
        iconSprite->oam.f.x = (s16)commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = (s16)commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile = Resource_LoadIndexedEntryToBuffer(
            summon->abilityId & ACTION_ID_MASK, summonIconSlot);
        state->iconShown[1] = 1;
        if (ok == 0)
          Text_SetColor(TEXT_COLOR_RED);
        Text_DrawMessageAsTiles(SummonDefinition_Get(mode)->abilityId +
                                    (s32)MsgAbilityName,
                                commandWindow, 16, 0);
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] != 0) {
            Window_SetTilemapEntry(commandWindow, j + ELEMENT_ICON_TILE,
                                   n * 2 + SUMMON_COST_X, 0, 0);
            Window_PutGlyph(commandWindow, summon->djinnRequired[j] + '0',
                            n * 2 + SUMMON_COST_X + 1, 0, GLYPH_DRAW_CHARACTER);
            n++;
          }
        }
        if (ok != 0)
          Sound_PlayFar(SE_MENU_CONFIRM);
        else
          Sound_PlayFar(SE_MENU_ERROR);
        mode = BattleTarget_RunSelection(actorId, ability->targetMode,
                                         ability->range,
                                         Battle_ClassifyEntryKind(ability));
        command->range = ability->range;
        state->iconShown[1] = 0;
        VramCache_FreeEntry(summonIconSlot);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto summon_menu;
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
        s32 cost;
        s32 element;
        s32 index;

        Sound_PlayFar(SE_MENU_CONFIRM);
        SET_CURSOR(0, 0, 0);
      djinn_menu:
        COMMAND_ICON_X(state) = DJINN_MENU_ICON_X;
        NAVIGATION->chosenDjinn[commandIndex] = BATTLE_MENU_NO_DJINNI;
        mode = BattleDjinnMenu_Run(actorId, 1);
        command->range = 0;
        if (mode == -1)
          goto mark_visible;
        parameter = mode;
        commandKind = 5;
        NAVIGATION->chosenDjinn[commandIndex] = mode;
        element = (parameter >> 8) & 15;
        index = parameter & 255;
        if (Djinn_IsSetFar(actorId, element, index) != 0) {
          ability = Ability_Get(Djinn_GetAbilityIdFar(element, index));
          cost = ability->range;
          commandWindow = Window_Create(11, 17, 10, 3, 6);
          COMMAND_ICON_X(state) = 64;
          Window_SetTilemapEntry(commandWindow, element + ELEMENT_ICON_TILE, 0,
                                 0, 0);
          Text_DrawMessageAsTiles(element * DJINN_PER_ELEMENT + index +
                                      (s32)MsgDjinnName,
                                  commandWindow, 16, 0);
          command->range = cost;
          wait(1);
          Sound_PlayFar(SE_MENU_CONFIRM);
          mode = BattleTarget_RunSelection(actorId, ability->targetMode, cost,
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
          if (mode == -1)
            goto djinn_menu;
          target = mode;
          break;
        }
        commandWindow = Window_Create(11, 17, 10, 3, 6);
        COMMAND_ICON_X(state) = 64;
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
        mode = BattleTarget_RunSelection(actorId, 4, 0, TARGET_HINT_SET_DJINNI);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto djinn_menu;
        target = mode;
        break;
      }

      case COMMAND_ICON_ITEM: {
        s32 k;
        struct MenuSprite *iconSprite;
        struct BattleAct *ability;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 cnt;
        s32 entryValue;
        s32 selection;

        Sound_PlayFar(SE_MENU_CONFIRM);
        mode = 0;
        SET_CURSOR(mode, mode, mode);
      item_menu:
        COMMAND_ICON_X(state) = 96;
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
            if (k == 15)
              break;
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
            if (k == 15)
              break;
            entryValue = unit->inventory[k];
          } while (entryValue != 0);
        }
        list[cnt] = 0;
        mode = BattleItemMenu_Run(actorId, list, cnt);
        if (mode == -1)
          goto mark_visible;
        selection = pick[mode];
        parameter = selection;
        ability = Ability_Get(
            Item_GetDirectFar(unit->inventory[selection])->abilityId);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        commandWindow = Window_Create(9, 17, ITEM_WINDOW_WIDTH, 3, 6);
        COMMAND_ICON_X(state) = 48;
        iconSprite->oam.word.attr01 =
            OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        /* FAKEMATCH(barrier): counted as 4 bytes. */
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
        mode = BattleTarget_RunSelection(actorId, ability->targetMode, cost,
                                         Battle_ClassifyEntryKind(ability));
        state->iconShown[1] = 0;
        VramCache_FreeEntry(handle);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto item_menu;
        commandKind = COMMAND_ITEM;
        target = mode;
        break;
      }

      case COMMAND_ICON_DEFEND:
        commandKind = COMMAND_DEFEND;
        break;
      }

      Sound_PlayFar(SE_CHIME);
      command->unitId = actorId;
      {
        s32 priority = unit->agi;

        if (priority != 0)
          priority += (u32)(unit->agi * Random16()) >> 20;
        command->priority = priority;
        if (commandIndex != 0 &&
            actors[commandIndex] == actors[commandIndex - 1])
          command->priority = (s16)priority / 2;
      }
      if (command->priority < 0)
        command->priority = 2000;
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
    u16 *signature = gData_02003a74;

    signature[4] = 'E';
    signature[5] = 'D';
    signature[6] = 'V';
    signature[7] = 'S';
  }
  if (state->timerWindow != NULL)
    Window_Close(state->timerWindow, 1);
  VramCache_FreeEntry(smallIconSlot);
  VramCache_FreeEntry(largeIconSlot);
  Task_Remove(UpdateLinkSessionCountdown);
  if (state->linkTimer != 0) {
    struct BattleWork *link;

    void Link_DrawWaitingForOpponent(void) {
      u16 text[64];

#if LANGUAGE != LANGUAGE_JAPANESE
      Message_CopyExpanded((s32)MsgWaitingForOpponent, text, 52);
#else
      Message_DecodeRaw((s32)MsgWaitingForOpponent, text, 32);
#endif
      Text_DrawWideStringAtOffset(text, state->timerWindow, 0, 4);
    }

    link = Ram_HeapSlots->battleWork;
    mode = 0;
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
        mode++;
        if (mode > 24) {
          commandCount = -1;
          break;
        }
      } else {
        mode = 0;
        if (peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
            peerSignature[6] == 'V' && peerSignature[7] == 'S')
          break;
        if (!(peerSignature[4] == 'V' && peerSignature[5] == 'S' &&
              peerSignature[6] == 'S' && peerSignature[7] == 'T'))
          mode = 1;
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

// The command icon menu's frame task:
// pulses the selected icon with one affine matrix, draws the others plain,
// and while the screen transition runs widens window 1 over the icon row.

// The transition's byte after its fields, which the centre-line close sets
// (ScreenTransition_Finish); while it is set the window lines are left alone.
#define TRANSITION_BUSY(state) (((s8 *)(state))[0x53f])

/// Draws the resource-menu icons, pulsing the selected icon with one affine
/// matrix, and updates the horizontal window spans to reveal the active icon
/// row. A task of the command icon menus
/// (IconMenu_Open). Each icon is a 32x32, 256-colour sprite;
/// the selected one is double-size affine, pulsing between 1.19 and 1.31
/// times, on layer 246, the others on 245, and one at x 0 is not drawn.
/// While a window transition runs (gScreenTransitionWork), it widens window 1
/// over the icon row so the icons show through: from the first icon on the
/// row's 24 lines, keeping each line's right edge, and around the selected
/// icon on the lines it overhangs (below a row at the top, above one at the
/// bottom, whose lines are 136 to 159).
void IconMenu_DrawIcons(void) {
  struct IconMenuWork *work;
  struct IconMenuEntry *sprite;
  struct AffineTransform effect;
  s32 scale;
  s32 matrix;
  s32 edge;
  s32 y;
  u32 end;
  u32 i;
  u16 *line;
  struct ScreenTransitionState *buffers;
  struct WindowBuffer *window;
  s32 span;

  work = Ram_HeapSlots->iconMenu;
  sprite = work->entries;
  scale = gWindowSprite_BounceScales[(gFrameCount * 2) & 31];
  buffers = Ram_HeapSlots->screenTransition;
  scale = (scale - 256) / 4 + 304;
  effect.scaleX = scale;
  effect.scaleY = scale;
  effect.angle = 0;
  matrix = OamMatrix_AllocFromTransform(&effect);
  for (i = 0; i < work->count; i++, sprite++) {
    u32 *attr;
    s32 slot;
    s32 px;

    px = sprite->x;
    if (px == 0)
      continue;
    if (i == work->selection) {
      s32 left = px + scale * 7 / 512 - 20;

      if (sprite->y != 0)
        y = sprite->y + scale * 3 / 256 - 20;
      else
        y = (scale * 15 / 256 - 30) & 255;
      attr = &sprite->link;
      *attr++ = 0;
      *attr++ = (matrix << 25) | y | OBJ_X(left) | OBJ_SIZE_32X32 |
                OBJ_256_COLORS | OBJ_DOUBLE_SIZE | OBJ_AFFINE;
      *attr = gVramCacheEntries[sprite->slot].offset >> 5;
      slot = 246;
    } else {
      u32 base;
      u32 value;

      attr = &sprite->link;
      y = sprite->y;
      *attr++ = 0;
      value = y | OBJ_X(px) | OBJ_SIZE_32X32 | OBJ_256_COLORS;
      base = gVramCacheEntries[sprite->slot].offset;
      *attr++ = value;
      *attr = base >> 5;
      slot = 245;
    }
    Oam_AddSpriteAtDepth(sprite, slot);
  }
  if (buffers == 0 || TRANSITION_BUSY(buffers) != 0 || work->count == 0)
    return;
  window = (struct WindowBuffer *)buffers->pages[buffers->page];
  if (work->row == 0) {
    edge = work->entries[work->selection].x;
    span = (scale * 12 - 2817) / 256;
    edge = ((edge - span) << 8) + (edge + span) + 23;
    end = work->entries[work->selection].y + 24;
    end += (scale * 32 - 7937) / 512 + 1;
    line = &window->lines[24].span;
    i = 24;
    if (i < end) {
      do {
        *line = (*line & 0xff) | edge;
        i++;
        line += 2;
      } while (i < end);
    }
    edge = work->entries[0].x;
    if (work->selection == 0)
      edge -= (scale * 12 - 2817) / 256;
    edge <<= 8;
    line = &window->lines[0].span;
    for (i = 0; i <= 23; i++) {
      *line = (*line & 0xff) | edge;
      line += 2;
    }
  } else {
    s32 start;

    edge = work->entries[work->selection].x;
    span = (scale * 12 - 2817) / 256;
    edge = ((edge - span) << 8) + (edge + span) + 23;
    start = work->entries[work->selection].y;
    start -= (scale * 32 - 7937) / 512 + 1;
    line = &window->lines[start].span;
    i = start;
    if (i <= 135) {
      do {
        *line = (*line & 0xff) | edge;
        i++;
        line += 2;
      } while (i <= 135);
    }
    edge = work->entries[0].x;
    if (work->selection == 0)
      edge -= (scale * 12 - 2817) / 256;
    edge <<= 8;
    line = &window->lines[136].span;
    for (i = 136; i <= 159; i++) {
      start = 0xff;
      *line = (*line & start) | edge;
      line += 2;
    }
  }
}
#undef SUMMON_WINDOW_X
#undef SUMMON_WINDOW_WIDTH
#undef SUMMON_COST_X
#undef ITEM_WINDOW_WIDTH
#undef ACTOR_ICON_X
#undef SUMMON_MENU_ICON_X
#undef SUMMON_CHOSEN_ICON_X
#undef DJINN_MENU_ICON_X
#undef ATTACK_NAME_X
#undef PSYNERGY_MENU_ICON_X
#undef NAVIGATION
#undef SET_CURSOR
#undef COMMAND_ICON_X
#undef TRANSITION_BUSY

/// Allocates and clears the resource-menu state, then schedules its icon pulse.
/// The work (struct IconMenuWork, 152 bytes) lives in HEAP_SLOT_ICON_MENU
/// until IconMenu_Close frees it; it is returned, and also reached
/// as gIconMenuWork. Every command icon menu starts here.
void *IconMenu_Open(void) {
  void *work =
      HeapSlot_AllocEwram(HEAP_SLOT_ICON_MENU, sizeof(struct IconMenuWork));

  Dma_Fill32(0, work, sizeof(struct IconMenuWork));
  Task_Add((TaskFunc)IconMenu_DrawIcons, TASK_PASS_VBLANK - 10);
  return work;
}

/// Stops the icon pulse, closes the label window and releases each loaded icon
/// before freeing the resource-menu state (HEAP_SLOT_ICON_MENU), then waits a
/// frame.
void IconMenu_Close(void) {
  struct Window *labelWindow;
  s32 i;
  struct IconMenuWork *work;

  work = Ram_HeapSlots->iconMenu;
  Task_Remove(IconMenu_DrawIcons);
  labelWindow = work->window;
  if (labelWindow != 0) {
    Window_Close(labelWindow, 2);
  }
  for (i = 0; i < work->count; i++) {
    VramCache_FreeEntry(work->entries[i].slot);
  }
  HeapSlot_Release(HEAP_SLOT_ICON_MENU);
  wait(1U);
}

/// Selects an icon position with wrapped directional input. A returns the
/// position; B or Start returns -1. Redraws the label only after a move. The
/// label is message resourceBase + position when the layout set a
/// resourceBase, else the icon's command name (MsgCommandName + its icon).
s32 IconMenu_Run(s32 initial) {
  struct IconMenuWork *work = Ram_HeapSlots->iconMenu;
  s32 resource;

  work->selection = initial;
redraw:
  WindowSprite_PrepareForRedraw(work->window);
  if (work->labelBase != 0)
    resource = work->labelBase + work->selection;
  else
    resource = work->commandIcons[work->selection] + (s32)MsgCommandName;
  Text_DrawMessageAsTiles(resource, work->window, 0, 0);
  for (;;) {
    wait(1);
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      return work->selection;
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
      work->selection--;
      if (work->selection < 0)
        work->selection = work->count - 1;
    } else if ((gInput.repeat & KEY_RIGHT) || (gInput.repeat & KEY_DOWN)) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      work->selection++;
      if (work->selection >= work->count)
        work->selection = 0;
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

/// Steps the selection from start to goal, redrawing the label each step and
/// slowing as it nears the goal, then confirms.
/// Animates a cursor from start to goal with table-driven delays. Draws each
/// intervening label, pauses at the destination, and returns its position.
/// It moves one icon at a time with the cursor sound, waiting
/// gMenu_SelectionStepDelays[icons still to go] frames at each (12 more at
/// the first), then 48 frames on the goal before the confirm sound: a menu
/// choice made for the player to watch (Menu_AnimateSelectionToEntry).
s32 IconMenu_AnimateTo(s32 start, s32 goal) {
  s16 resourceBase;
  s16 cur;
  s32 diff;
  s32 resourceId;
  s32 dist;
  s32 pos;
  s32 step;
  s32 delay;
  const u8 *table;
  struct IconMenuWork *state;

  state = Ram_HeapSlots->iconMenu;
  step = 1;
  delay = 12;
  state->selection = (s16)start;
  if (goal < start)
    step = -1;
  pos = start;

  for (;;) {
    WindowSprite_PrepareForRedraw(state->window);
    resourceBase = state->labelBase;
    if (resourceBase != 0) {
      resourceId = resourceBase + state->selection;
    } else {
      resourceId = state->commandIcons[state->selection] + (s32)MsgCommandName;
    }
    Text_DrawMessageAsTiles(resourceId, state->window, 0, 0);

    cur = state->selection;
    table = gMenu_SelectionStepDelays;
    diff = cur - goal;
    dist = AbsoluteDifference(diff, cur, goal);
    wait(table[dist] + delay);

    if (pos == goal)
      break;

    state->selection = (s16)((u16)state->selection + step);
    Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    delay = 0;
    pos += step;
  }

  wait(48);
  Sound_PlayFar(SE_MENU_CONFIRM);
  return goal;
}

#define Decompress_ByteLz ((void (*)(void *, void *))Decompress_ByteLz)

/// Decodes one command-icon table entry into the requested VRAM cache slot,
/// through a 1 KB block it allocates and frees.
void CommandIcon_LoadIntoSlot(s32 slot, s32 index) {
  s32 size = 1024;
  void *buffer = (void *)Heap_Alloc(size);
  u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  Decompress_ByteLz((void *)((u32)base + base[index]), buffer);
  VramCache_Load(slot, size, buffer);
  Heap_Release(buffer);
}

/// Appends a command icon if fewer than six are present. Its initial position
/// is on the bottom icon row; a layout routine places the final row. The
/// icon's number (ResourceId_CommandIcons) is also its command name's
/// (MsgCommandName + icon). Its VRAM slot is taken unchecked.
void IconMenu_AddIcon(s32 icon) {
  struct IconMenuWork *work;
  struct IconMenuEntry *entry;
  s16 index;
  s32 slot;
  s32 y = 136;

  work = Ram_HeapSlots->iconMenu;
  index = work->count;
  if (index <= 5) {
    work->count++;
    entry = &work->entries[index];
    slot = VramCache_FindFreeEntry();
    CommandIcon_LoadIntoSlot(slot, icon);
    entry->x = index * 24 + 32;
    entry->y = y;
    entry->slot = slot;
    work->commandIcons[index] = icon;
  }
}

/// Centers the icons and their label window together on the given tile row.
/// A nonzero resourceBase supplies labels; otherwise command-name IDs do.
/// The icons stand three tiles apart and the label window, `width` + 2 tiles
/// wide and 3 tall, follows them; the centring counts two thirds of the
/// window's width.
void IconMenu_Center(s32 row, s32 width, s32 resourceBase) {
  struct IconMenuWork *menu = Ram_HeapSlots->iconMenu;
  s32 x;
  s32 i;
  s32 count;

  width += 2;
  menu->width = width;
  menu->labelBase = resourceBase;
  menu->row = row;
  count = menu->count;
  x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
  for (i = 0; i < menu->count; i++) {
    struct IconMenuEntry *entry = &menu->entries[i];

    entry->x = x * 8;
    entry->y = row * 8;
    x += 3;
  }
  menu->window = Window_Create(x, row, menu->width, 3, WINDOW_FLAG_INSTANT);
}

/// Lays the entries out in a row three cells apart from x, on row y, and
/// opens the menu's window after them.
void IconMenu_LayoutIcons(s32 x, s32 y, s32 width, s32 resourceBase) {
  struct IconMenuWork *work;
  s32 i;
  struct IconMenuEntry *entry;
  s16 *widthp;

  work = Ram_HeapSlots->iconMenu;
  widthp = &work->width;
  *widthp = width + 2;
  work->labelBase = resourceBase;
  work->row = y;
  for (i = 0; i < work->count; i++) {
    entry = &work->entries[i];
    entry->y = y << 3;
    entry->x = x << 3;
    x += 3;
  }
  work->window = Window_Create(x, y, *widthp, 3, WINDOW_FLAG_INSTANT);
}

/// Returns table[index].
static __inline__ s32 TableGet(s8 *table, s32 index) { return table[index]; }

/// Builds the field command menu, omitting Djinn when none are owned.
/// Translates between command IDs and visible icon positions using the two
/// lookup rows. The icons are Psynergy, Djinn, Item and Status on row 17;
/// `selection` is the command to start on and the result the command
/// chosen (enum FieldMenuCommand), or -1 when cancelled. The
/// field's menu runs it (window/window_selection.c).
s32 FieldMenu_SelectCommand(s32 selection) {
  s32 group = 0;
  s32 offset;
  s32 triple;
  s32 ret;
  s8 *table;

  if (Party_SumDjinnCountsFar(-1) == 0) {
    group = 1;
  }

  triple = group * 3;
  table = gMenu_TopEntryPositionByCommand;
  offset = triple << 1;
  selection = TableGet(table, selection + offset) - 1;
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
  IconMenu_Center(17, SELECT_MENU_WIDTH, 0);
  ret = IconMenu_Run(selection);
  IconMenu_Close();

  if (ret >= 0) {
    s8 *commands = gMenu_TopEntryCommandByPosition;
    s32 index;

    index = ret + offset + 1;
    ret = commands[index];
  }

  return ret;
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
  IconMenu_Center(17, SELECT_MENU_WIDTH, 0);
#endif
  command = IconMenu_AnimateTo(start, command - 1);
  IconMenu_Close();
  PartyStatus_CloseWithMenu();
  return command;
}

/// Offers the actions the files allow (plus two optional entries
/// behind EWRAM flags) and maps the chosen position to an action. The second
/// optional entry is 31, and with the first option off, action 4 becomes 5.
/// Builds the File Screen's menu from the save summaries and optional send
/// flags. Translates the chosen visible position back to its action (enum
/// FileAction), or -1 when cancelled or the backup memory cannot be read. With
/// no files it returns 0 at once. The icons, by Save_ScanFileSummaries' count:
/// New Game, Continue, Copy and Erase for one or two files (starting on
/// Continue); no New Game or Copy with three; with every file cleared no
/// Continue, and only Erase with three such files. Battle and Send follow when
/// enabled. The file list runs it (scene/file_screen.c).
s32 FileList_SelectAction(void) {
  s32 type;
  s32 ret;
  s32 initial;
  u32 group;

  group = 0;
  initial = 0;
  type = Save_ScanFileSummaries();
  if (type < 0)
    return -1;
  if (type == 0)
    return 0;
  if (type == 3)
    group = 1;
  else if (type == SAVE_SCAN_ALL_CLEARED + 3)
    group = 2;
  else if (type > SAVE_SCAN_ALL_CLEARED)
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
  IconMenu_Center(17, SAVE_SLOT_MENU_WIDTH, 0);
  ret = IconMenu_Run(initial);
  IconMenu_Close();
  if (ret >= 0) {
    u32 twice = group * 2;

    ret = gMenu_SaveSlotActionByPosition[ret + (twice + group) * 2];
  }
  if (gTitleBattleOptionEnabled == 0 && ret == 4)
    ret = 5;
  return ret;
}

// The label area is narrower in Japanese and wider in French.
#if LANGUAGE == LANGUAGE_JAPANESE
#define LABEL_RIGHT 80
#elif LANGUAGE == LANGUAGE_FRENCH
#define LABEL_RIGHT 160
#else
#define LABEL_RIGHT 128
#endif

/// When the selected mode changes, clears the lower window's label area and
/// writes the mode's lines of text. The area starts at x 8 and ends at
/// LABEL_RIGHT; a mode past 2 draws nothing.
void Menu_DrawModeLabel(void) {
  struct IconMenuWork *state = Ram_HeapSlots->iconMenu;

  if (state->previousMode != state->selection) {
    state->previousMode = state->selection;
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

// The Spanish descriptions are indented.
#if LANGUAGE == LANGUAGE_SPANISH
#define HELP_X 6
#else
#define HELP_X 0
#endif

/// Redraws the selected method's description in the lower window. Spanish,
/// French and Italian describe two methods, the password and everything else
/// as the cable; French clears the window with WindowSprite_ClearList and
/// redraws only when the description changes.
void Menu_DrawModeIndicator(void) {
  struct IconMenuWork *state = Ram_HeapSlots->iconMenu;
  s16 *shown = &state->previousMode;
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
    // Through the pointers, the selection would be loaded once at the top.
    state->previousMode = state->selection;
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

// The entries' width and the windows' places and widths, in tiles, and where
// the lines of text start, in pixels, are fitted to each language's text.
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

// Spanish, French and Italian indent the password's lines and set them
// closer together.
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

/// Selects either the transfer method or the password level (enum
/// ResourceLayoutMode). Schedules the
/// appropriate help redraw while the icon menu is active, then closes both
/// help windows and the icon menu. Returns the place
/// chosen, or -1 when cancelled. The Send screens run it (scene/file_screen.c).
s32 IconMenu_ChooseSendOption(s32 mode) {
  struct IconMenuWork *work;
  struct Window *window;
  s32 msg;
  struct Window **upper;
  s32 result;

  mode &= 1;
  IconMenu_Open();
  work = Ram_HeapSlots->iconMenu;
  if (mode == RESOURCE_LAYOUT_TRANSFER_METHOD) {
    IconMenu_AddIcon(COMMAND_ICON_PASSWORD);
    IconMenu_AddIcon(COMMAND_ICON_LINK);
  } else {
    IconMenu_AddIcon(COMMAND_ICON_GOLD);
    IconMenu_AddIcon(COMMAND_ICON_SILVER);
    IconMenu_AddIcon(COMMAND_ICON_BRONZE);
  }
  IconMenu_Center(17, ENTRY_WIDTH, 0);
  if (mode != RESOURCE_LAYOUT_TRANSFER_METHOD) {
    Task_Add(Menu_DrawModeLabel, TASK_PASS_VBLANK - 10);
    work->previousMode = 0xffff;
    window = Window_Create(PASSWORD_UPPER_X, 0, PASSWORD_UPPER_WIDTH, 4,
                           WINDOW_FLAG_INSTANT);
    msg = (s32)MsgPasswordEntry;
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage(msg, window, 0, 4);
    window = Window_Create(PASSWORD_LOWER_X, 4, PASSWORD_LOWER_WIDTH, 12,
                           WINDOW_FLAG_INSTANT);
    work->lowerWindow = window;
    Text_DrawMessage(msg + 1, window, PASSWORD_TEXT_X, PASSWORD_TEXT_Y);
#if LANGUAGE == LANGUAGE_FRENCH
    // French writes a third line: "Niveaux" (MsgPasswordLevel).
    Text_DrawMessage(msg + 2, work->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT);
    msg += 3;
    Text_DrawMessage(msg, work->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT * 2);
#else
    msg += 2;
    Text_DrawMessage(msg, work->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT);
#endif
  } else {
    Task_Add(Menu_DrawModeIndicator, TASK_PASS_VBLANK - 10);
    work->previousMode = 0xffff;
    window = Window_Create(TRANSFER_UPPER_X, 0, TRANSFER_UPPER_WIDTH, 4,
                           WINDOW_FLAG_INSTANT);
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage((s32)MsgTransferMethod, window, TRANSFER_TITLE_X, 4);
    work->lowerWindow = Window_Create(TRANSFER_LOWER_X, 5, TRANSFER_LOWER_WIDTH,
                                      7, WINDOW_FLAG_INSTANT);
  }
  result = IconMenu_Run(0);
  if (mode != RESOURCE_LAYOUT_TRANSFER_METHOD)
    Task_Remove(Menu_DrawModeLabel);
  else
    Task_Remove(Menu_DrawModeIndicator);
  WindowSprite_PrepareForRedraw(*upper);
  WindowSprite_PrepareForRedraw(work->lowerWindow);
  Window_Close(*upper, 2);
  Window_Close(work->lowerWindow, 2);
  wait(1);
  IconMenu_Close();
  return result;
}

// Menu: select command icons, such as Yes and No.

/// Selects a shop command from icons 0x11-0x14, centred on row 17, showing
/// the selected command's name in a window. Icons are indices into
/// ResourceId_CommandIcons.
/// selection: initial selection. Returns the selected position.
s32 Menu_SelectShopCommand(s32 selection) {
  s32 ret;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_BUY);
  IconMenu_AddIcon(COMMAND_ICON_SELL);
  IconMenu_AddIcon(COMMAND_ICON_ARTIFACTS);
  IconMenu_AddIcon(COMMAND_ICON_REPAIR);
#if (LANGUAGE == LANGUAGE_GERMAN)
  IconMenu_Center(17, 8, 0); // Name window width.
#elif (LANGUAGE != LANGUAGE_JAPANESE)
  IconMenu_Center(17, 7, 0);
#else
  IconMenu_Center(17, 6, 0);
#endif
  ret = IconMenu_Run(selection);
  IconMenu_Close();
  return ret;
}

/// Selects a sanctum service from icons 0x19-0x1c, centred on row 17.
s32 Menu_SelectSanctumService(s32 selection) {
  s32 ret;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_REVIVE);
  IconMenu_AddIcon(COMMAND_ICON_CURE_POISON);
  IconMenu_AddIcon(COMMAND_ICON_REPEL_EVIL);
  IconMenu_AddIcon(COMMAND_ICON_REMOVE_CURSE);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  IconMenu_Center(17, 10, 0);
#else
  IconMenu_Center(17, 8, 0);
#endif
  ret = IconMenu_Run(selection);
  IconMenu_Close();
  return ret;
}

/// Selects Yes or No (icons 5 and 6). Cancel returns 1 (No).
/// low: row 0 if zero, otherwise row 17.
/// message: first name message, or 0 for command names.
/// w: name window width, or 3 if zero.
/// selection: initial selection.
s32 YesNoMenu_Run(s32 bottom, s32 message, s32 labelWidth, s32 selection) {
  s32 row;

  row = 0;

  IconMenu_Open();

  if (labelWidth == 0) {
    labelWidth = 3;
  }
  if (bottom != 0) {
    row = 17;
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

/// Selects Yes or No at (x, y). Cancel returns 1 (No).
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

//--------------------------------------------------------------------------

// select_entry_20_to_21.c

// Menu: select between command icons 0x20 and 0x21.

//--------------------------------------------------------------------------

// Name window width for each edition.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PAIR_WIDTH 7
#elif (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

/// Selects coins or medals from icons 0x20 and 0x21, centred on row 17.
/// selection: initial selection. Returns the selected position.
s32 Menu_SelectCoinsOrMedals(s32 selection) {
  s32 result;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_COINS);
  IconMenu_AddIcon(COMMAND_ICON_LUCKY_MEDALS);
  IconMenu_Center(17, PAIR_WIDTH, 0);
  result = IconMenu_Run(selection);
  IconMenu_Close();
  return result;
}

/// Schedules the debug scene and entrance selector;
/// TASK_PASS_FRAME is a task priority, not a number of frames to wait.
/// Nothing in the game calls it.
void DebugSceneSelect_Schedule(void) {
  Task_Add(DebugSceneSelect_Run, TASK_PASS_VBLANK);
}
#undef Decompress_ByteLz
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

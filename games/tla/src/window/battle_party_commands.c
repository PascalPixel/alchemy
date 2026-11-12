/// @file
/// Collecting the party's battle commands, as The Broken Seal's
/// Battle_CollectPartyCommands does on this game's work and menus.

#include "types.h"
#include "ram_buffer.h"
#include "battle_work.h"
#include "graphics/uiwindow.h"
#include "battle/battle_unit.h"
#include "battle/battle_summon.h"
#include "battle/battle_system.h"
#include "window.h"
#include "input.h"
#include "edition.h"
#include "task.h"

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

extern u8 MsgAbilityNothing[];
extern u8 MsgCommandName[];
extern u8 MsgDjinnName[];
extern u8 MsgItemName[];

/// A menu sprite: a list link and its OAM attributes, as two words or as
/// fields.
struct MenuSprite {
  struct MenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

/// The battle menu work (heap slot 57), The Broken Seal's BattleMenuWork.
struct BattleMenuWork {
  struct MenuSprite icons[3];
  u8 iconShown[3];
  u8 unk27;
  s32 targetViewOffset;
  s32 viewOffset;
  s32 row;
  s32 page;
  s32 preferredRow;
  s32 commandSelection[2];
  void *timerWindow;
  s32 timerHidden;
  s32 timer;
  s32 linkTimer;
  u8 standbyDjinn[8][4];
  u8 listSlots[34];
  u16 listAbilities[33];
  u8 unkd8[0];
  s32 tutorialStep;
  s32 tutorialDelay;
  s32 commandActor;
  u32 chosenDjinn[8];
  s32 swappedUnit;  ///< the unit a reserve swap replaced, -1 for none
  s32 swappedPlace; ///< its party place
  s32 lastTarget;
};

struct BattleAction {
  u16 unitId;
  u16 unk02;
  s16 priority;
  u16 command;
  u16 parameter;
  u16 target;
  u16 range;
  u16 unk0e;
};

#define NAVIGATION ((struct BattleMenuWork *)Ram_HeapSlots->unke4[0])

extern u16 gData_02003a74[];
extern u16 gLinkStatus;
extern u16 gLinkPeerSignatures[][12];
extern u8 gMainRoutineWork[];
extern u8 gDebugMode;
extern u8 MsgWaitingForOpponent[];

s32 VramCache_AllocEntry(s32 size);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Resource_LoadIndexedIntoBuffer(s32 slot, s32 index);
void Graphics_ExpandVramTilesByColorTable(u16 *tiles);
void Link_DrawShiftedTilePair(u32 address);
void Menu_BuildLocalizedPatternTiles(void);
void wait(s32 frames);
void UpdateLinkSessionCountdown(void);
s32 Func_0804babc(u16 *actors, struct BattleAction **cursor, s32 *forcedCount);
void BattleLayout_HighlightPartyPanels(u16 *units);
void BattleHighlight_SetModesFar(u16 *units, s32 mode);
s32 GameFlag_Test(s32 flag);
s32 Func_080457d0(s32 mode, s32 fleeAllowed);
void PartyStatus_Draw(s32 flags);
void BattleActor_CommitPlacementFar(void);
void Func_08118158(void);
void Sound_PlayFar(s32 cue);
s32 Func_0804a85c(s32 selection, s32 mode, u32 spread, u32 kind);
s32 Func_0804b8b8(s32 slot);
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
s32 BattleParty_ListActorIdsFar(s32 side, u16 *units);
s32 BattleParty_PrepareReserveOwnersFar(u16 *units);
s32 Func_08046b08(u16 *list, s32 listCount, s32 member);
s32 Message_ShowLocalizedAndWait(void);
void Message_DecodeRaw(s32 message, u16 *text, s32 capacity);
void Func_08118130(s32 kind, u8 *counts);
struct BattleUnit *Unit_Get(s32 unit);
s32 Func_08045528(s32 portrait, s32 slot);
void Func_08041b68(void);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void Text_DrawCharacterAtOffset(s32 message, struct Window *window, s32 x,
                                s32 y);
s32 Func_0804a134(s32 unit, u16 *actions, s32 count);
s32 Resource_LoadIndexedEntryToBuffer(s32 id, s32 slot);
void Func_08041f70(s32 color);
void Text_DrawNumberAtOffset(s32 value, s32 digits, struct Window *window,
                             s32 x, s32 y);
void Window_SetTilemapEntry(struct Window *window, s32 entry, s32 x, s32 y,
                            s32 flags);
void Resource_ResetEntry(s32 slot);
s32 Func_08049154(s32 unused0, s32 unused1, const u8 *standby);
void Func_0803c274(struct Window *window, s32 character, s32 x, s32 y,
                   s32 mode);
s32 Func_0804868c(s32 member, s32 unused);
s32 Djinn_IsActiveFar(s32 member, s32 element, s32 index);
s32 Djinn_GetDefinitionHeaderFar(s32 element, s32 index);
s32 Func_0804519c(void);
void *Message_ShowAndWait(s32 message, s32 x, s32 y);
s32 Message_AllTyped(void);
s32 Func_08049a30(s32 member, u16 *entries, s32 count);
u16 *Item_Get(s32 item);
s32 Item_ClassifyUseAbility(s32 member, s32 item);
s32 ItemIcon_LoadIntoSlot(s32 item, s32 slot);
u32 Random16(void);
void Vram_FillWindowBlockPattern(void);
s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity);
void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x,
                                 s32 y);
void Camera_ConfigureSceneFar(s32 mode);

#define SET_CURSOR(page_, row_, preferred_)                                    \
  do {                                                                         \
    NAVIGATION->page = (page_);                                                \
    NAVIGATION->row = (row_);                                                  \
    NAVIGATION->preferredRow = (preferred_);                                   \
  } while (0)

#define COMMAND_ICON_X(state) ((state)->icons[0].oam.f.x)

/// Collects the party's commands for a turn into `commands`, one for each of
/// the `commandCount` units in `actors`, with two nested functions: the party
/// panels' clear and the link wait's message (Func_0804bbd0). Unlike The Broken
/// Seal's, each round first appends the forced actions of statused members
/// (Func_0804babc), whose count is added to the result; B on the party's row
/// undoes a reserve swap; command 49 swaps a reserve member in (Func_0804b8b8
/// after choosing the slot); Status lists the reserve as well; Flee is command
/// 99; and the Djinn tutorial's hint follows an unleash.
s32 Func_0804bc08(struct BattleAction *commands, u16 *actors,
                  s32 commandCount) {
  u16 signatureHeader[2];
  struct BattleMenuWork *state;
  struct BattleAction *commandsOut;
  s32 forcedCount;
  struct BattleAction *command;
  struct BattleUnit *unit;
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

    data[0] = 255;
    BattleLayout_HighlightPartyPanels(data);
    BattleHighlight_SetModesFar(data, 1);
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
  state = HeapSlot_AllocEwram(228, 272);
  // Default: set again before each round's forced actions.
  forcedCount = 0;
  Resource_LoadIndexedIntoBuffer(largeIconSlot, 0);
  Graphics_ExpandVramTilesByColorTable((u16 *)0x06006000);
  Link_DrawShiftedTilePair(0x06006680);
  Menu_BuildLocalizedPatternTiles();
  state->swappedUnit = -1;
  state->lastTarget = -1;
  {
    struct BattleWork *battle = Ram_HeapSlots->battleWork;
    s32 k;

    for (k = 0; k < 8; k++)
      state->chosenDjinn[k] = 0x80000000;
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
    if (battle->twoSided != 0) {
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
        if ((gLinkStatus & 3) != 3) {
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
  Task_Add(UpdateLinkSessionCountdown, 0x480);

  for (;;) {
    commandsOut = commands;
    if (commandCount >= 0) {
      forcedCount = 0;
      commandCount = Func_0804babc(actors, &commandsOut, &forcedCount);
    }
    Battle_DrawPartyPanelsWithEmptyList();
    state->iconShown[2] = 0;
    state->commandActor = -1;
    state->tutorialStep = 0;
    if (GameFlag_Test(0x16e)) {
      state->tutorialStep = 1;
      state->tutorialDelay = 60;
    }
    if (commandCount >= 0)
      mode = Func_080457d0(0, commandCount);
    else
      mode = 14;

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
        if (gMainRoutineWork[0x218 + k] == shortSwapped)
          gMainRoutineWork[0x218 + k] = old;
        else if (gMainRoutineWork[0x218 + k] == old)
          gMainRoutineWork[0x218 + k] = shortSwapped;
      }
      PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
      BattleActor_CommitPlacementFar();
      state->swappedUnit = -1;
      Func_08118158();
      NAVIGATION->commandSelection[0] = 1;
      continue;
    }
    // Command 49: choose a party place, then the reserve member for it.
    if (mode == 49) {
      Sound_PlayFar(112);
    choose_place:
      mode = Func_0804a85c(0, 2, 1, 8);
      if (mode == -1)
        continue;
      mode = Func_0804b8b8(mode);
      if (mode == -1)
        goto choose_place;
      Func_08118158();
      NAVIGATION->commandSelection[0] = 0;
      continue;
    }
    // Status: the summary screens of the party and the reserve, or in the
    // debug build with Start held the enemies'.
    if (mode == 7) {
      u16 *block = Heap_Alloc(18);
      s32 side;
      s32 count;

      if (gDebugMode != 0 && (gInput.held & 8))
        side = 2;
      else
        side = 1;
      count = BattleParty_ListActorIdsFar(side, block);
      if (side == 1)
        count += BattleParty_PrepareReserveOwnersFar(block + count);
      wait(1);
      Func_08046b08(block, count, block[0]);
      Heap_Release(block);
      continue;
    }
    // Flee: one command 99 for the party, first in the round.
    if (mode == 4) {
      s32 refused = Message_ShowLocalizedAndWait();

      if (refused != 0)
        continue;
      commandCount = 1;
      command = commands;
      command->unitId = actors[0];
      forcedCount = refused;
      command->priority = 0x7ffe;
      command->command = 99;
      command->parameter = refused;
      command->target = 256;
      break;
    }
    if (mode != 14)
      break;
    Sound_PlayFar(154);
    commandIndex = 0;
    if (commandIndex >= commandCount)
      break;

    for (;;) {
      if (commandIndex == 0) {
        Func_08118130(0, state->standbyDjinn[0]);
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
      unit = (struct BattleUnit *)(s32)((s64 (*)(s32))Unit_Get)(actorId);
#endif
      {
        struct BattleMenuWork *navigation = NAVIGATION;
        struct MenuSprite *iconSprite;

        navigation->commandActor = actorId;
        navigation->commandSelection[1] = 0;
        iconSprite = &navigation->icons[2];
        iconSprite->oam.word.attr01 = 0x80000400;
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.tile = Func_08045528(unit->enemyId, smallIconSlot);
        iconSprite->oam.f.x = ACTOR_ICON_X;
        iconSprite->oam.f.y = 0x80;
        iconSprite->oam.f.palette = 14;
        state->iconShown[2] = 1;
      }

    mark_visible:
      Func_08041b68();
      {
        struct BattleMenuWork *navigation = NAVIGATION;

        navigation->iconShown[0] = 0;
        navigation->chosenDjinn[commandIndex] = 0x80000000;
      }
      signatureHeader[0] = actorId;
      signatureHeader[1] = 255;
      BattleHighlight_SetModesFar(signatureHeader, 1);
      BattleLayout_HighlightPartyPanels(signatureHeader);
      wait(1);
      mode = Func_080457d0(1, 0);
      wait(1);
      if (mode == -2) {
        u16 *block = Heap_Alloc(12);
        s32 count = BattleParty_ListActorIdsFar(1, block);

        state->iconShown[2] = 0;
        wait(1);
        Func_08046b08(block, count, actorId);
        state->iconShown[2] = 1;
        Heap_Release(block);
        continue;
      }
      BattleHighlight_SetModesFar(signatureHeader, 0);
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
          mode = 3;
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.word.attr01 = 0x80002400;
        iconSprite->oam.f.tile =
            Resource_LoadIndexedIntoBuffer(largeIconSlot, mode);
        if (mode == 15)
          iconSprite->oam.f.x = 128;
        else
          iconSprite->oam.f.x = 96;
        iconSprite->oam.f.y = 136;
        state->iconShown[0] = 1;
      }

      switch (mode) {
      case 0:
        commandWindow = Window_Create(11, 17, 11, 3, 6);
        Text_DrawCharacterAtOffset((s32)MsgCommandName, commandWindow,
                                   ATTACK_NAME_X, 0);
        COMMAND_ICON_X(state) = 64;
        Sound_PlayFar(112);
        mode = Func_0804a85c(actorId, 1, 1, 0);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto mark_visible;
        commandKind = 0;
        target = mode;
        command->range = 1;
        break;

      case 1: {
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

        Sound_PlayFar(112);
        mode = 0;
        SET_CURSOR(mode, mode, mode);
      psynergy_menu:
        list = state->listAbilities;
        pick = state->listSlots;
        COMMAND_ICON_X(state) = PSYNERGY_MENU_ICON_X;
        cnt = 0;
        k = 0;
        entryValue = unit->actionSlots[k].encodedAction & 0x3fff;
        if (entryValue != 0) {
          do {
            if (BattleAction_Get(entryValue)->flags & 0x80) {
              pick[cnt] = k;
              list[cnt] = entryValue;
              cnt++;
            }
            k++;
            if (k == 32)
              break;
            entryValue = unit->actionSlots[k].encodedAction & 0x3fff;
          } while (entryValue != 0);
        }
        pick[cnt] = 0;
        list[cnt] = 0;
        mode = Func_0804a134(actorId, list, cnt);
        if (mode == -1)
          goto mark_visible;
        parameter = unit->actionSlots[pick[mode]].encodedAction & 0x3fff;
        ability = BattleAction_Get(parameter);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
        commandWindow = Window_Create(8, 17, 18, 3, 6);
        COMMAND_ICON_X(state) = 40;
        iconSprite->oam.word.attr01 = 0x40000400;
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.x = (s16)commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = (s16)commandWindow->y * 8 + 4;
        /* FAKEMATCH(barrier): counted as 4 bytes, the empty asm puts the pool
         * after mov r1, r10 (constant 40); without it, 18 of 1907
         * instructions differ in tla-en. */
        __asm__ volatile("");
        iconSprite->oam.f.tile =
            Resource_LoadIndexedEntryToBuffer(parameter, handle);
        state->iconShown[1] = 1;
        work->level = 5;
        if (ability->ppCost > unit->pp)
          Func_08041f70(2);
        else if (unit->psySeal != 0)
          Func_08041f70(9);
        Text_DrawCharacterAtOffset(parameter + (s32)MsgAbilityNothing,
                                   commandWindow, 16, 0);
        Text_DrawNumberAtOffset(ability->ppCost, 2, commandWindow, 104, 0);
        work->level = 15;
        Func_08041f70(15);
        Window_SetTilemapEntry(commandWindow, WINDOW_TILE_PP_LABEL, 11, 0, 0);
        Window_SetTilemapEntry(commandWindow, WINDOW_TILE_PP_LABEL_END, 12, 0,
                               0);
        if (ability->element != 4)
          Window_SetTilemapEntry(commandWindow, ability->element + 0x5001, 15,
                                 0, 0);
        command->range = cost;
        Sound_PlayFar(112);
        mode = Func_0804a85c(actorId, ability->targetMode, cost,
                             Battle_ClassifyEntryKind(ability));
        state->iconShown[1] = 0;
        Resource_ResetEntry(handle);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto psynergy_menu;
        commandKind = 1;
        target = mode;
        break;
      }

      case 16: {
        const struct SummonDefinition *summon;
        struct BattleAct *ability;
        struct MenuSprite *iconSprite;
        s32 ok;
        s32 j;
        s32 n;

        Sound_PlayFar(112);
        SET_CURSOR(0, 0, 0);
      summon_menu:
        COMMAND_ICON_X(state) = SUMMON_MENU_ICON_X;
        BattleLayout_HighlightPartyPanels(signatureHeader);
        mode = Func_08049154(0, 0, state->standbyDjinn[commandIndex]);
        BattleLayout_HighlightPartyPanels(signatureHeader);
        if (mode == -1)
          goto mark_visible;
        commandKind = 6;
        parameter = mode;
        wait(1);
        iconSprite = &state->icons[1];
        summon = SummonDefinition_Get(mode);
        ability = BattleAction_Get(summon->nameMessageId);
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
        // Written through a u32 pointer, not the union: these stores then no
        // longer conflict with the load of state, which sched2 needs to order
        // eors first.
        ((u32 *)&iconSprite->oam)[0] = 0x40000400;
        ((u32 *)&iconSprite->oam)[1] = 0;
        /* FAKEMATCH(barrier): counted as 4 bytes, the empty asm puts the pool
         * after mov r1, r11 (constant 56); without it, 22 of 1907
         * instructions differ in tla-en. */
        __asm__ volatile("");
        iconSprite->oam.f.x = (s16)commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = (s16)commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile = Resource_LoadIndexedEntryToBuffer(
            summon->nameMessageId & 0x3fff, summonIconSlot);
        state->iconShown[1] = 1;
        if (ok == 0)
          Func_08041f70(2);
        Text_DrawCharacterAtOffset(SummonDefinition_Get(mode)->nameMessageId +
                                       (s32)MsgAbilityNothing,
                                   commandWindow, 16, 0);
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] != 0) {
            Window_SetTilemapEntry(commandWindow, j + 0x5001,
                                   n * 2 + SUMMON_COST_X, 0, 0);
            Func_0803c274(commandWindow, summon->djinnRequired[j] + '0',
                          n * 2 + SUMMON_COST_X + 1, 0, 0);
            n++;
          }
        }
        if (ok != 0)
          Sound_PlayFar(112);
        else
          Sound_PlayFar(114);
        mode = Func_0804a85c(actorId, ability->targetMode, ability->range,
                             Battle_ClassifyEntryKind(ability));
        command->range = ability->range;
        state->iconShown[1] = 0;
        Resource_ResetEntry(summonIconSlot);
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

      case 15: {
        struct BattleAct *ability;
        s32 cost;
        s32 element;
        s32 index;

        Sound_PlayFar(112);
        SET_CURSOR(0, 0, 0);
      djinn_menu:
        COMMAND_ICON_X(state) = DJINN_MENU_ICON_X;
        NAVIGATION->chosenDjinn[commandIndex] = 0x80000000;
        mode = Func_0804868c(actorId, 1);
        command->range = 0;
        if (mode == -1)
          goto mark_visible;
        parameter = mode;
        commandKind = 5;
        NAVIGATION->chosenDjinn[commandIndex] = mode;
        element = (parameter >> 8) & 15;
        index = parameter & 255;
        if (Djinn_IsActiveFar(actorId, element, index) != 0) {
          ability =
              BattleAction_Get(Djinn_GetDefinitionHeaderFar(element, index));
          cost = ability->range;
          commandWindow = Window_Create(11, 17, 10, 3, 6);
          COMMAND_ICON_X(state) = 64;
          Window_SetTilemapEntry(commandWindow, element + 0x5001, 0, 0, 0);
          Text_DrawCharacterAtOffset(element * 20 + index + (s32)MsgDjinnName,
                                     commandWindow, 16, 0);
          command->range = cost;
          wait(1);
          Sound_PlayFar(112);
          mode = Func_0804a85c(actorId, ability->targetMode, cost,
                               Battle_ClassifyEntryKind(ability));
          if (state->tutorialStep != 0) {
            void *hint = Message_ShowAndWait(Func_0804519c() + 14, 15, 8);

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
        Func_08041f70(2);
        Window_SetTilemapEntry(commandWindow, element + 0x5001, 0, 0, 0);
        Text_DrawCharacterAtOffset(element * 20 + index + (s32)MsgDjinnName,
                                   commandWindow, 16, 0);
        Func_08041f70(15);
        command->range = 1;
        wait(1);
        Sound_PlayFar(112);
        mode = Func_0804a85c(actorId, 4, 0, 7);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto djinn_menu;
        target = mode;
        break;
      }

      case 2: {
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

        Sound_PlayFar(112);
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
            Item_Get(entryValue);
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
            Item_Get(entryValue);
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
        mode = Func_08049a30(actorId, list, cnt);
        if (mode == -1)
          goto mark_visible;
        selection = pick[mode];
        parameter = selection;
        ability = BattleAction_Get(Item_Get(unit->inventory[selection])[20]);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        commandWindow = Window_Create(9, 17, ITEM_WINDOW_WIDTH, 3, 6);
        COMMAND_ICON_X(state) = 48;
        iconSprite->oam.word.attr01 = 0x40000400;
        /* FAKEMATCH(barrier): counted as 4 bytes, the empty asm puts the pool
         * after strb r3, [r5, #4] (constant 48); without it, 12 of 1907
         * instructions differ in tla-en. */
        __asm__ volatile("");
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.x = (s16)commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = (s16)commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile =
            ItemIcon_LoadIntoSlot(unit->inventory[selection], handle);
        state->iconShown[1] = 1;
        Text_DrawCharacterAtOffset((unit->inventory[selection] & 0x1ff) +
                                       (s32)MsgItemName,
                                   commandWindow, 24, 0);
        command->range = cost;
        Sound_PlayFar(112);
        mode = Func_0804a85c(actorId, ability->targetMode, cost,
                             Battle_ClassifyEntryKind(ability));
        state->iconShown[1] = 0;
        Resource_ResetEntry(handle);
        Window_Close(commandWindow, 1);
        if (mode == -1)
          goto item_menu;
        commandKind = 2;
        target = mode;
        break;
      }

      case 3:
        commandKind = 3;
        break;
      }

      Sound_PlayFar(110);
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
  Resource_ResetEntry(smallIconSlot);
  Resource_ResetEntry(largeIconSlot);
  Task_Remove(UpdateLinkSessionCountdown);
  if (state->linkTimer != 0) {
    struct BattleWork *link;

    void Func_0804bbd0(void) {
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
      state->timerWindow = Window_Create(0, 16, 30, 4, 42);
      Vram_FillWindowBlockPattern();
      Func_0804bbd0();
    } else {
      state->timerWindow = NULL;
    }
    for (;;) {
      u16 *peerSignature = gLinkPeerSignatures[link->linkSide ^ 1];

      if (link->linkPaused != 0) {
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
  Camera_ConfigureSceneFar(0);
  HeapSlot_Release(228);
  if (commandCount >= 0)
    commandCount += forcedCount;
  return commandCount;
}

// The command icon menu's frame task (The Broken Seal's IconMenu_DrawIcons):
// pulses the selected icon with one affine matrix, draws the others plain,
// and while the screen transition runs widens window 1 over the icon row.
#include "resource_menu.h"
#include "frame.h"
#include "display_transition.h"
#include "vram_tab.h"

// games/tla/include/system/affine.h's transform, written out: the stale
// affine.h beside these drafts would shadow it.
struct AffineTransform {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
  u16 unk06;
};

s32 AffineMatrix_BuildForEffect(struct AffineTransform *transform);

extern const u16 gWindowSprite_BounceScales[];

struct WindowLine {
  u16 left;
  u16 span;
};

struct WindowBuffer {
  u8 unk000[4];
  struct WindowLine lines[160];
};

// The transition's byte after its fields; while it is set the window lines
// are left alone.
#define TRANSITION_BUSY(state) (((s8 *)(state))[0x53f])

void Func_0804cda8(void) {
  struct ResourceMenuWork *work;
  struct ResourceMenuEntry *sprite;
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

  work = Ram_HeapSlots->menuSelectWork;
  sprite = work->entries;
  scale = gWindowSprite_BounceScales[(gFrameCount * 2) & 31];
  buffers = Ram_HeapSlots->screenTransition;
  scale = (scale - 256) / 4 + 304;
  effect.scaleX = scale;
  effect.scaleY = scale;
  effect.angle = 0;
  matrix = AffineMatrix_BuildForEffect(&effect);
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
      *attr++ = (matrix << 25) | y | (left << 16) | 0x80002300;
      *attr = gResourceTableEntries[sprite->slot].offset >> 5;
      slot = 246;
    } else {
      u32 base;
      u32 value;

      attr = &sprite->link;
      y = sprite->y;
      *attr++ = 0;
      value = y | (px << 16) | 0x80002000;
      base = gResourceTableEntries[sprite->slot].offset;
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
      // start is reused for the mask; as (*line & 0xff), 194 of 386
      // instructions differ in tla-en.
      start = 0xff;
      *line = (*line & start) | edge;
      line += 2;
    }
  }
}

// Draft, differ 220 of 1907, skeleton gap 96 (tla-en, 2026-11-01): 27 removed,
// 69 added. The two nested functions are compiled here but have their own
// listings, so measure all three with alchemy-devtools diff DRAFT.c
// 0804bba8,0804bbd0,0804bc08: differ 178 of 1949, skeleton gap 54, and both
// nested functions match. Collecting the party's battle commands, The Broken
// Seal's Battle_CollectPartyCommands
// (games/tbs/src/window/screens/battle_party_commands.c) on this game's work and
// menus, with its two nested functions: the party panels' clear
// (Battle_DrawPartyPanelsWithEmptyList, adopted alone in window/party_panel.c
// with a stand-in for the static chain) and the link wait's message
// (Func_0804bbd0, drafted alone in 0804bbd0.c). This game's changes: each round
// first appends the forced actions of statused members (Func_0804babc), whose
// count is added to the result; B on the party's row undoes a reserve swap;
// command 49 swaps a reserve member in (Func_0804b8b8 after choosing the slot);
// Status lists the reserve as well; Flee is command 99; the Djinn tutorial's
// hint follows an unleash.
// Next step: the three FAKEMATCH(barrier) sites The Broken Seal carries are
// needed here too, for the same pools. An empty asm before iconSprite->tile in
// the Psynergy and Item windows and before iconSprite->x in the summon window
// takes differ from 178 to 136 of 1949 and the gap from 54 to 46. They are out
// only because they do not reach differ 0; put them back, tagged, once the rest
// closes.
// Left, 136 with the barriers, of which about 80 lines are one cause: the spend
// loop (the second duplicated test, after the summon's target) allocates the
// state value, offset and required byte as r5, r4, r1 where the listing has r1,
// r5, r4. Global allocation order there is priority: required 72/7, offset 8,
// state 6.86; the listing needs state first, then required, then offset. That
// one change rotates reload's registers through the Djinn case (r2 for
// commandKind 5, a second load of parameter, the zero and -1 of the range).
// Tried for it: a u8 or s32 local for the required byte (the u8 puts it last, but
// the offset then comes first), a local state pointer, a pointer to the element,
// and an offset local through u8 * and through a byte-array view (all add or
// move instructions, or cost a spill slot).
// Smaller: the -1 of timer issues late (negs after the isLink load), and the
// listing's r3 then r2 for the zero and state of linkTimer = 0 are swapped here;
// the remaining order pairs (actors[0] in Flee, sp+20 reload, lsls after a str)
// look like K2 scheduling.
// Measured causes, for the next reader:
// - mode = 0 at the top is what puts r6 across the opening calls: it is the
//   long-lived zero the other locals copy.
// - state is assigned from a void * local: a call under a pointer conversion
//   computes the frame address first, a bare call computes it after.
// - The summon's first test is a guard and a do-while, not jump.c's duplicate:
//   loop.c's preheader copies land after the test only in that form, and cse2
//   turns them into the listing's three copies. The required pointer gives the
//   guard a register for them; without it they cross-jump into one test.
// - offset is set before required so its live length is 8: that makes priority
//   give required r1, the walking pointer r0 and offset r4.
// - hidden and cleared are one-constant locals (S2): the value is set before the
//   state load, as the listing's reload registers show. They fixed reload's
//   rotation from there to the summon case (186 to 142 with the barriers in).
// - SET_CURSOR is the twin's BATTLE_MENU_SET_CURSOR: its one-pass block keeps
//   mode = 0 ahead of the work pointer's load.
// The signature's letters come from the pool only when written through a local
// pointer, a read of the global array gets movs: keep signature.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"

extern u8 MsgAbilityNothing[];
extern u8 MsgCommandName[];
extern u8 MsgDjinnName[];
extern u8 MsgItemName[];

struct CommandIcon {
  u8 unk00[4];
  u8 y;
  u8 attr0high;
  u16 x : 9;
  u16 attr1high : 7;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
};

struct CommandIconWords {
  u8 unk00[4];
  u32 attr01;
  u32 attr23;
};

/// The battle menu work (heap slot 57, BattleMenuWork).
struct PartyCommandState {
  struct CommandIcon icons[3];
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

struct PartyCommandRecord {
  u16 unitId;
  u16 unk02;
  s16 priority;
  u16 command;
  u16 parameter;
  u16 target;
  u16 range;
  u16 unk0e;
};

struct PartyCommandBattle {
  u8 unk00[0x41];
  u8 partyStatus;
  u8 unk42[2];
  u8 isLink;
  u8 unk45[0x13];
  s16 partyUnits[7];
  u8 unk66[0];
};

struct PartyLinkBattle {
  u8 unk00[0x50];
  u8 linkSide;
  u8 unk51;
  u8 linkPaused;
};

struct PartyCommandPsynergy {
  u16 encodedAction;
  u16 unused;
};

struct PartyCommandUnit {
  u8 unk000[0x40];
  u16 agi;
  u8 unk042[0x16];
  struct PartyCommandPsynergy psynergy[32];
  u16 items[15];
  u8 unkf6[0x54];
  u16 portrait;
};

struct PartyCommandAbility {
  u8 targetMode;
  u8 flags;
  u8 element;
  u8 unk03[5];
  u8 range;
  u8 ppCost;
};

struct PartyCommandSummon {
  u16 abilityId;
  u8 unk02[2];
  u8 djinnRequired[4];
};

struct PartyCommandWindow {
  u8 unk00[0x0c];
  s16 x;
  s16 y;
};

struct PartyCommandWork {
  u8 unk00[7];
  u8 textPalette;
};

#define NAVIGATION ((struct PartyCommandState *)Ram_HeapSlots->unke4[0])

extern u16 gData_02003a74[];
extern u16 gLinkStatus;
extern u16 gLinkPeerSignatures[][12];
extern u8 gMainRoutineWork[];
extern u8 gDebugMode;
extern u8 Msg_0c59[];

s32 VramCache_AllocEntry(s32 size);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Resource_LoadIndexedIntoBuffer(s32 slot, s32 index);
void Graphics_ExpandVramTilesByColorTable(u16 *tiles);
void Link_DrawShiftedTilePair(u32 address);
void Func_080451bc(void);
void wait(s32 frames);
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void UpdateLinkSessionCountdown(void);
s32 Func_0804babc(u16 *actors, struct PartyCommandRecord **cursor,
                  s32 *forcedCount);
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
s32 Func_0804b7c0(void);
void Func_08118130(s32 kind, u8 *counts);
struct PartyCommandUnit *Unit_Get(s32 unit);
s32 Func_08045528(s32 portrait, s32 slot);
void Func_08041b68(void);
struct PartyCommandWindow *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                           s32 flags);
void Text_DrawCharacterAtOffset(s32 message,
                                  struct PartyCommandWindow *window, s32 x,
                                  s32 y);
void Window_Close(void *window, s32 mode);
struct PartyCommandAbility *BattleAction_Get(s32 id);
s32 Func_0804a134(s32 unit, u16 *actions, s32 count);
s32 Resource_LoadIndexedEntryToBuffer(s32 id, s32 slot);
void Func_08041f70(s32 color);
void Text_DrawNumberAtOffset(s32 value, s32 digits,
                               struct PartyCommandWindow *window, s32 x, s32 y);
void Window_SetTilemapEntry(struct PartyCommandWindow *window, s32 entry,
                              s32 x, s32 y, s32 flags);
s32 Battle_ClassifyEntryKind(struct PartyCommandAbility *ability);
void Resource_ResetEntry(s32 slot);
s32 Func_08049154(s32 unused0, s32 unused1, const u8 *standby);
const struct PartyCommandSummon *SummonDefinition_Get(s32 summon);
void Func_0803c274(struct PartyCommandWindow *window, s32 character, s32 x,
                   s32 y, s32 mode);
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
void Text_DrawWideStringAtOffset(u16 *text,
                                     struct PartyCommandWindow *window, s32 x,
                                     s32 y);
void Camera_ConfigureSceneFar(s32 mode);

#define SET_CURSOR(page_, row_, preferred_)                                    \
  do {                                                                         \
    NAVIGATION->page = (page_);                                                \
    NAVIGATION->row = (row_);                                                  \
    NAVIGATION->preferredRow = (preferred_);                                   \
  } while (0)

#define COMMAND_ICON_X(state) ((state)->icons[0].x)

s32 Func_0804bc08(struct PartyCommandRecord *commands, u16 *actors,
                  s32 commandCount) {
  u16 signatureHeader[2];
  struct PartyCommandState *state;
  void *menuBlock;
  struct PartyCommandRecord *commandsOut;
  s32 forcedCount;
  struct PartyCommandRecord *command;
  struct PartyCommandUnit *unit;
  s32 target;
  s32 actorId;
  s32 commandKind;
  s32 parameter;
  s32 largeIconSlot;
  s32 smallIconSlot;
  s32 commandIndex;
  s32 summonIconSlot;
  s32 mode;
  struct PartyCommandWindow *commandWindow;

  void Battle_DrawPartyPanelsWithEmptyList(void) {
    u16 data[2];

    data[0] = 255;
    BattleLayout_HighlightPartyPanels(data);
    BattleHighlight_SetModesFar(data, 1);
  }

  void Func_0804bbd0(void) {
    u16 text[64];

    Message_CopyExpanded((s32)Msg_0c59, text, 52);
    Text_DrawWideStringAtOffset(text, state->timerWindow, 0, 4);
  }

  target = 256;
  commandsOut = NULL;
  mode = 0;
  commandKind = 0;
  parameter = 0;
  largeIconSlot = VramCache_AllocEntry(1024);
  smallIconSlot = VramCache_AllocEntry(512);
  menuBlock = HeapSlot_AllocEwram(228, 272);
  state = menuBlock;
  forcedCount = 0;
  Resource_LoadIndexedIntoBuffer(largeIconSlot, 0);
  Graphics_ExpandVramTilesByColorTable((u16 *)0x06006000);
  Link_DrawShiftedTilePair(0x06006680);
  Func_080451bc();
  state->swappedUnit = -1;
  state->lastTarget = -1;
  {
    struct PartyCommandBattle *battle = Ram_HeapSlots->battleWork;
    s32 k;
    s32 hidden;
    s32 cleared;

    for (k = 0; k < 8; k++)
      state->chosenDjinn[k] = 0x80000000;
    hidden = 0;
    for (k = 0; k < 3; k++)
      state->iconShown[k] = hidden;
    cleared = 0;
    state->targetViewOffset = cleared;
    state->viewOffset = cleared;
    state->commandSelection[0] = cleared;
    state->commandSelection[1] = cleared;
    state->linkTimer = cleared;
    state->timerHidden = cleared;
    state->timer = -1;
    state->timerWindow = (void *)cleared;
    if (battle->isLink != 0) {
      struct PartyLinkBattle *link = (struct PartyLinkBattle *)battle;
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
        u16 *peerSignature = gLinkPeerSignatures[link->linkSide ^ 1];
        s32 paused = link->linkPaused;

        if (paused != 0) {
          commandCount = -1;
          state->linkTimer = 0;
          break;
        }
        if ((gLinkStatus & 3) != 3) {
          if (++missedLinkFrames > 24) {
            commandCount = -1;
            state->linkTimer = paused;
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
      struct PartyCommandBattle *battle;
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
      PartyStatus_Draw(((struct PartyCommandBattle *)Ram_HeapSlots->battleWork)
                           ->partyStatus);
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
      s32 refused = Func_0804b7c0();

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
      unit = Unit_Get(actorId);
      {
        struct PartyCommandState *navigation = NAVIGATION;
        struct CommandIcon *iconSprite;

        navigation->commandActor = actorId;
        navigation->commandSelection[1] = 0;
        iconSprite = &navigation->icons[2];
        ((struct CommandIconWords *)iconSprite)->attr01 = 0x80000400;
        ((struct CommandIconWords *)iconSprite)->attr23 = 0;
        iconSprite->tile = Func_08045528(unit->portrait, smallIconSlot);
        iconSprite->x = 0;
        iconSprite->y = 0x80;
        iconSprite->palette = 14;
        state->iconShown[2] = 1;
      }

    mark_visible:
      Func_08041b68();
      {
        struct PartyCommandState *navigation = NAVIGATION;

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
        struct PartyCommandState *navigation = NAVIGATION;
        struct CommandIcon *iconSprite = &navigation->icons[0];

        if (navigation->timer == 0)
          mode = 3;
        ((struct CommandIconWords *)iconSprite)->attr23 = 0;
        ((struct CommandIconWords *)iconSprite)->attr01 = 0x80002400;
        iconSprite->tile = Resource_LoadIndexedIntoBuffer(largeIconSlot, mode);
        if (mode == 15)
          iconSprite->x = 128;
        else
          iconSprite->x = 96;
        iconSprite->y = 136;
        state->iconShown[0] = 1;
      }

      switch (mode) {
      case 0:
        commandWindow = Window_Create(11, 17, 11, 3, 6);
        Text_DrawCharacterAtOffset((s32)MsgCommandName, commandWindow, 16, 0);
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
        struct CommandIcon *iconSprite;
        struct PartyCommandAbility *ability;
        struct PartyCommandWork *work;
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
        COMMAND_ICON_X(state) = 48;
        cnt = 0;
        k = 0;
        entryValue = unit->psynergy[k].encodedAction & 0x3fff;
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
            entryValue = unit->psynergy[k].encodedAction & 0x3fff;
          } while (entryValue != 0);
        }
        pick[cnt] = 0;
        list[cnt] = 0;
        mode = Func_0804a134(actorId, list, cnt);
        if (mode == -1)
          goto mark_visible;
        parameter = unit->psynergy[pick[mode]].encodedAction & 0x3fff;
        ability = BattleAction_Get(parameter);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        work = (struct PartyCommandWork *)Ram_HeapSlots->windowTiles;
        commandWindow = Window_Create(8, 17, 18, 3, 6);
        COMMAND_ICON_X(state) = 40;
        ((struct CommandIconWords *)iconSprite)->attr01 = 0x40000400;
        ((struct CommandIconWords *)iconSprite)->attr23 = 0;
        iconSprite->x = commandWindow->x * 8 + 8;
        iconSprite->y = commandWindow->y * 8 + 4;
        iconSprite->tile = Resource_LoadIndexedEntryToBuffer(parameter, handle);
        state->iconShown[1] = 1;
        work->textPalette = 5;
        if (ability->ppCost > ((s16 *)unit)[0x1d])
          Func_08041f70(2);
        else if (((u8 *)unit)[0x13d] != 0)
          Func_08041f70(9);
        Text_DrawCharacterAtOffset(parameter + (s32)MsgAbilityNothing,
                                     commandWindow, 16, 0);
        Text_DrawNumberAtOffset(ability->ppCost, 2, commandWindow, 104, 0);
        work->textPalette = 15;
        Func_08041f70(15);
        Window_SetTilemapEntry(commandWindow, 0xf01f, 11, 0, 0);
        Window_SetTilemapEntry(commandWindow, 0xf01e, 12, 0, 0);
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
        const struct PartyCommandSummon *summon;
        struct PartyCommandAbility *ability;
        struct CommandIcon *iconSprite;
        s32 ok;
        s32 j;
        s32 n;

        Sound_PlayFar(112);
        SET_CURSOR(0, 0, 0);
      summon_menu:
        COMMAND_ICON_X(state) = 80;
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
        ability = BattleAction_Get(summon->abilityId);
        summonIconSlot = VramCache_AllocEntry(128);
        commandWindow = Window_Create(10, 17, 17, 3, commandKind);
        n = 0;
        j = 0;
        {
          s32 offset = commandIndex * 4 + 84;
          const u8 *required = summon->djinnRequired;

          if (*required <= ((u8 *)state)[offset]) {
            do {
              j++;
              if (j > 3)
                break;
            } while (summon->djinnRequired[j] <= state->standbyDjinn[commandIndex][j]);
          }
        }
        ok = j == 4;
        COMMAND_ICON_X(state) = 56;
        ((struct CommandIconWords *)iconSprite)->attr01 = 0x40000400;
        ((struct CommandIconWords *)iconSprite)->attr23 = 0;
        iconSprite->x = commandWindow->x * 8 + 8;
        iconSprite->y = commandWindow->y * 8 + 4;
        iconSprite->tile = Resource_LoadIndexedEntryToBuffer(
            summon->abilityId & 0x3fff, summonIconSlot);
        state->iconShown[1] = 1;
        if (ok == 0)
          Func_08041f70(2);
        Text_DrawCharacterAtOffset(SummonDefinition_Get(mode)->abilityId +
                                         (s32)MsgAbilityNothing,
                                     commandWindow, 16, 0);
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] != 0) {
            Window_SetTilemapEntry(commandWindow, j + 0x5001, n * 2 + 11, 0,
                                     0);
            Func_0803c274(commandWindow, summon->djinnRequired[j] + '0',
                          n * 2 + 12, 0, 0);
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
        struct PartyCommandAbility *ability;
        s32 cost;
        s32 element;
        s32 index;

        Sound_PlayFar(112);
        SET_CURSOR(0, 0, 0);
      djinn_menu:
        COMMAND_ICON_X(state) = 140;
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
            void *hint =
                Message_ShowAndWait(Func_0804519c() + 14, 15, 8);

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
        struct CommandIcon *iconSprite;
        struct PartyCommandAbility *ability;
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
        entryValue = unit->items[k];
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
            entryValue = unit->items[k];
          } while (entryValue != 0);
        }
        k = 0;
        entryValue = unit->items[k];
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
            entryValue = unit->items[k];
          } while (entryValue != 0);
        }
        list[cnt] = 0;
        mode = Func_08049a30(actorId, list, cnt);
        if (mode == -1)
          goto mark_visible;
        selection = pick[mode];
        parameter = selection;
        ability = BattleAction_Get(Item_Get(unit->items[selection])[20]);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        commandWindow = Window_Create(9, 17, 16, 3, 6);
        COMMAND_ICON_X(state) = 48;
        ((struct CommandIconWords *)iconSprite)->attr01 = 0x40000400;
        ((struct CommandIconWords *)iconSprite)->attr23 = 0;
        iconSprite->x = commandWindow->x * 8 + 8;
        iconSprite->y = commandWindow->y * 8 + 4;
        iconSprite->tile =
            ItemIcon_LoadIntoSlot(unit->items[selection], handle);
        state->iconShown[1] = 1;
        Text_DrawCharacterAtOffset((unit->items[selection] & 0x1ff) +
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
    struct PartyLinkBattle *link = Ram_HeapSlots->battleWork;

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

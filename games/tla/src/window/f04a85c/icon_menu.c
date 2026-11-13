/// @file
/// Routines whose twins are in The Broken Seal's window/screens/icon_menu.c.
// Part of f04a85c.c.

#include "../f04a85c_internal.h"

// Collecting the party's battle commands, as The Broken Seal's
// Battle_CollectPartyCommands does on this game's work and menus, then
// Func_0804cda8, which draws a menu's icons each frame, the selected one
// bouncing, and fits the window's lines around it.


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
/* VIEW(battle_party_commands): variable gLinkPeerSignatures as extern u16 gLinkPeerSignatures[][12]; countdown has extern struct LinkSignature gLinkPeerSignatures[]; tried as the 2 declarations' extern u16 gLinkPeerSignatures[][12], it does not compile in tla-ja: games/tla/src/window/f04a85c.c:1344: conflicting types for `gLinkPeerSignatures'. */
extern u16 gLinkPeerSignatures__battle_party_commands[][12] __asm__("gLinkPeerSignatures");
/* VIEW(battle_party_commands): variable gMainRoutineWork as extern u8 gMainRoutineWork[]; main_routine_work.h has extern struct MainRoutineWork gMainRoutineWork; tried as the 3 declarations' extern u8 gMainRoutineWork[], it does not compile in tla-ja: games/tla/src/window/f04a85c.c:1346: conflicting types for `gMainRoutineWork'. */
extern u8 gMainRoutineWork__battle_party_commands[] __asm__("gMainRoutineWork");
extern u8 gDebugMode;
extern u8 MsgWaitingForOpponent[];

s32 VramCache_AllocEntry(s32 size);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Resource_LoadIndexedIntoBuffer(s32 slot, s32 index);
void Graphics_ExpandVramTilesByColorTable(u16 *tiles);
/* VIEW(battle_party_commands): routine Link_DrawShiftedTilePair as void Link_DrawShiftedTilePair(u32 address); countdown has void Link_DrawShiftedTilePair(s32 destination); tried as the rest of the object declares it, Func_0804bc08 moves: 1 of 1909 instructions in tla-ja. */
extern void Link_DrawShiftedTilePair__battle_party_commands(u32 address) __asm__("Link_DrawShiftedTilePair");
void Menu_BuildLocalizedPatternTiles(void);
void wait(s32 frames);
void UpdateLinkSessionCountdown(void);
/* VIEW(battle_party_commands): routine Func_0804babc as s32 Func_0804babc(u16 *actors, struct BattleAction **cursor, s32 *forcedCount); the definition has s32 Func_0804b8b8(s32 slot) { u16 owners[6]; u16 selected[2]; struct BattleWork *battle; s32 count; count = BattleParty_PrepareReserveOwners...; tried as the rest of the object declares it, Func_0804bc08 moves: 1 of 1909 instructions in tla-ja. */
extern s32 Func_0804babc__battle_party_commands(u16 *actors, struct BattleAction **cursor, s32 *forcedCount) __asm__("Func_0804babc");
/* VIEW(battle_party_commands): routine BattleLayout_HighlightPartyPanels as void BattleLayout_HighlightPartyPanels(u16 *units); countdown has s32 BattleLayout_HighlightPartyPanels(u16 *ids); tried as the definition's s32 BattleLayout_HighlightPartyPanels(u16 *ids), which the object already declares, Battle_DrawPartyPanelsWithEmptyList.0 moves: 1 of 18 instructions, Func_0804bc08 moves: 3 of 1909 instructions in tla-ja. */
extern void BattleLayout_HighlightPartyPanels__battle_party_commands(u16 *units) __asm__("BattleLayout_HighlightPartyPanels");
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
/* VIEW(battle_party_commands): routine BattleParty_ListActorIdsFar as s32 BattleParty_ListActorIdsFar(s32 side, u16 *units); graphics.h has s32 BattleParty_ListActorIdsFar(s32 side, s32 group); tried as the rest of the object declares it, Func_0804bc08 moves: 2 of 1909 instructions in tla-ja. */
extern s32 BattleParty_ListActorIdsFar__battle_party_commands(s32 side, u16 *units) __asm__("BattleParty_ListActorIdsFar");
s32 BattleParty_PrepareReserveOwnersFar(u16 *units);
s32 Func_08046b08(u16 *list, s32 listCount, s32 member);
s32 Message_ShowLocalizedAndWait(void);
/* VIEW(battle_party_commands): routine Message_DecodeRaw as void Message_DecodeRaw(s32 message, u16 *text, s32 capacity); countdown has void Message_DecodeRaw(s32, s16 *, s32); tried as the definition's void Message_DecodeRaw(s32 message, u16 *text, s32 capacity), which the object already declares, Func_0804bbd0.1 moves: 2 of 24 instructions in tla-ja. */
extern void Message_DecodeRaw__battle_party_commands(s32 message, u16 *text, s32 capacity) __asm__("Message_DecodeRaw");
void Func_08118130(s32 kind, u8 *counts);
void *Unit_Get(s32 owner);
s32 Func_08045528(s32 portrait, s32 slot);
void Func_08041b68(void);
/* VIEW(battle_party_commands): routine Window_Create as struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags); f04a85c has struct TargetWindow *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags); tried as the definition's struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs), which the object already declares, Func_0804bc08 moves: 7 of 1909 instructions in tla-ja. */
extern struct Window *Window_Create__battle_party_commands(s32 x, s32 y, s32 width, s32 height, s32 flags) __asm__("Window_Create");
/* VIEW(battle_party_commands): routine Text_DrawCharacterAtOffset as void Text_DrawCharacterAtOffset(s32 message, struct Window *window, s32 x, s32 y); f04a85c has void Text_DrawCharacterAtOffset(s32 message, struct TargetWindow *window, s32 x, s32 y); tried as the definition's void Text_DrawCharacterAtOffset(s32 character, struct RenderInput *position, u32 offsetX, u32 offsetY), Func_0804bc08 moves: 7 of 1909 instructions in tla-ja. */
extern void Text_DrawCharacterAtOffset__battle_party_commands(s32 message, struct Window *window, s32 x,
                                s32 y) __asm__("Text_DrawCharacterAtOffset");
s32 Func_0804a134(s32 unit, u16 *actions, s32 count);
s32 Resource_LoadIndexedEntryToBuffer(s32 id, s32 slot);
void Func_08041f70(s32 color);
/* VIEW(battle_party_commands): routine Text_DrawNumberAtOffset as void Text_DrawNumberAtOffset(s32 value, s32 digits, struct Window *window, s32 x, s32 y); text_render.h has void Text_DrawNumberAtOffset(s32 value, s32 format, struct RenderInput *work, s32 offsetX, s32 offsetY); tried as the definition's void Text_DrawNumberAtOffset(s32 value, s32 width, struct RenderInput *work, s32 x, s32 y), Func_0804bc08 moves: 2 of 1909 instructions in tla-ja. */
extern void Text_DrawNumberAtOffset__battle_party_commands(s32 value, s32 digits, struct Window *window,
                             s32 x, s32 y) __asm__("Text_DrawNumberAtOffset");
/* VIEW(battle_party_commands): routine Window_SetTilemapEntry as void Window_SetTilemapEntry(struct Window *window, s32 entry, s32 x, s32 y, s32 flags); graphics.h has void Window_SetTilemapEntry(struct Window *window, s32 value, s32 x, s32 y, u32 mode); tried as the definition's void Window_SetTilemapEntry(struct Window *window, s32 value, s32 x, s32 y, u32 mode), which the object already declares, Func_0804bc08 moves: 11 of 1909 instructions in tla-ja. */
extern void Window_SetTilemapEntry__battle_party_commands(struct Window *window, s32 entry, s32 x, s32 y,
                            s32 flags) __asm__("Window_SetTilemapEntry");
s32 Resource_ResetEntry(u32 slot);
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
/* VIEW(battle_party_commands): routine Item_Get as u16 *Item_Get(s32 item); item.h has struct ItemDefinition *Item_Get(s32 item); tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/window/f04a85c.c:2047: subscripted value is neither array nor pointer. */
extern u16 *Item_Get__battle_party_commands(s32 item) __asm__("Item_Get");
s32 Item_ClassifyUseAbility(s32 member, s32 item);
s32 ItemIcon_LoadIntoSlot(s32 item, s32 slot);
u32 Random16(void);
/* VIEW(battle_party_commands): routine Vram_FillWindowBlockPattern as void Vram_FillWindowBlockPattern(void); countdown has unsigned char Vram_FillWindowBlockPattern(void); tried as the definition's void Vram_FillWindowBlockPattern(void), which the object already declares, Func_0804bc08 moves: 1 of 1909 instructions in tla-ja. */
extern void Vram_FillWindowBlockPattern__battle_party_commands(void) __asm__("Vram_FillWindowBlockPattern");
/* VIEW(battle_party_commands): routine Message_CopyExpanded as s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity); f04a85c has void Message_CopyExpanded(s32 message, u16 *text, s32 capacity); tried as the definition's s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity), it does not compile in tla-ja: games/tla/src/window/f04a85c.c:1419: conflicting types for `Message_CopyExpanded'. */
extern s32 Message_CopyExpanded__battle_party_commands(s32 key, u16 *destination, u32 capacity) __asm__("Message_CopyExpanded");
/* VIEW(battle_party_commands): routine Text_DrawWideStringAtOffset as void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x, s32 y); f04a85c has void Text_DrawWideStringAtOffset(u16 *text, struct TargetWindow *window, s32 x, s32 y); tried as the definition's void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x, s32 y), it does not compile in tla-ja: games/tla/src/window/f04a85c.c:1421: conflicting types for `Text_DrawWideStringAtOffset'. */
extern void Text_DrawWideStringAtOffset__battle_party_commands(u16 *text, struct Window *window, s32 x,
                                 s32 y) __asm__("Text_DrawWideStringAtOffset");
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
    BattleLayout_HighlightPartyPanels__battle_party_commands(data);
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
  Link_DrawShiftedTilePair__battle_party_commands(0x06006680);
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
        u16 *peerSignature = gLinkPeerSignatures__battle_party_commands[battle->linkSide ^ 1];

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
      commandCount = Func_0804babc__battle_party_commands(actors, &commandsOut, &forcedCount);
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
        if (gMainRoutineWork__battle_party_commands[0x218 + k] == shortSwapped)
          gMainRoutineWork__battle_party_commands[0x218 + k] = old;
        else if (gMainRoutineWork__battle_party_commands[0x218 + k] == old)
          gMainRoutineWork__battle_party_commands[0x218 + k] = shortSwapped;
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
      count = BattleParty_ListActorIdsFar__battle_party_commands(side, block);
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
      BattleLayout_HighlightPartyPanels__battle_party_commands(signatureHeader);
      wait(1);
      mode = Func_080457d0(1, 0);
      wait(1);
      if (mode == -2) {
        u16 *block = Heap_Alloc(12);
        s32 count = BattleParty_ListActorIdsFar__battle_party_commands(1, block);

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
        commandWindow = Window_Create__battle_party_commands(11, 17, 11, 3, 6);
        Text_DrawCharacterAtOffset__battle_party_commands((s32)MsgCommandName, commandWindow,
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
        commandWindow = Window_Create__battle_party_commands(8, 17, 18, 3, 6);
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
        Text_DrawCharacterAtOffset__battle_party_commands(parameter + (s32)MsgAbilityNothing,
                                   commandWindow, 16, 0);
        Text_DrawNumberAtOffset__battle_party_commands(ability->ppCost, 2, commandWindow, 104, 0);
        work->level = 15;
        Func_08041f70(15);
        Window_SetTilemapEntry__battle_party_commands(commandWindow, WINDOW_TILE_PP_LABEL, 11, 0, 0);
        Window_SetTilemapEntry__battle_party_commands(commandWindow, WINDOW_TILE_PP_LABEL_END, 12, 0,
                               0);
        if (ability->element != 4)
          Window_SetTilemapEntry__battle_party_commands(commandWindow, ability->element + 0x5001, 15,
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
        BattleLayout_HighlightPartyPanels__battle_party_commands(signatureHeader);
        mode = Func_08049154(0, 0, state->standbyDjinn[commandIndex]);
        BattleLayout_HighlightPartyPanels__battle_party_commands(signatureHeader);
        if (mode == -1)
          goto mark_visible;
        commandKind = 6;
        parameter = mode;
        wait(1);
        iconSprite = &state->icons[1];
        summon = SummonDefinition_Get(mode);
        ability = BattleAction_Get(summon->nameMessageId);
        summonIconSlot = VramCache_AllocEntry(128);
        commandWindow = Window_Create__battle_party_commands(SUMMON_WINDOW_X, 17, SUMMON_WINDOW_WIDTH,
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
        Text_DrawCharacterAtOffset__battle_party_commands(SummonDefinition_Get(mode)->nameMessageId +
                                       (s32)MsgAbilityNothing,
                                   commandWindow, 16, 0);
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] != 0) {
            Window_SetTilemapEntry__battle_party_commands(commandWindow, j + 0x5001,
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
          commandWindow = Window_Create__battle_party_commands(11, 17, 10, 3, 6);
          COMMAND_ICON_X(state) = 64;
          Window_SetTilemapEntry__battle_party_commands(commandWindow, element + 0x5001, 0, 0, 0);
          Text_DrawCharacterAtOffset__battle_party_commands(element * 20 + index + (s32)MsgDjinnName,
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
        commandWindow = Window_Create__battle_party_commands(11, 17, 10, 3, 6);
        COMMAND_ICON_X(state) = 64;
        Func_08041f70(2);
        Window_SetTilemapEntry__battle_party_commands(commandWindow, element + 0x5001, 0, 0, 0);
        Text_DrawCharacterAtOffset__battle_party_commands(element * 20 + index + (s32)MsgDjinnName,
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
            Item_Get__battle_party_commands(entryValue);
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
            Item_Get__battle_party_commands(entryValue);
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
        ability = BattleAction_Get(Item_Get__battle_party_commands(unit->inventory[selection])[20]);
        cost = ability->range;
        iconSprite = &state->icons[1];
        handle = VramCache_AllocEntry(128);
        commandWindow = Window_Create__battle_party_commands(9, 17, ITEM_WINDOW_WIDTH, 3, 6);
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
        Text_DrawCharacterAtOffset__battle_party_commands((unit->inventory[selection] & 0x1ff) +
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
      Message_CopyExpanded__battle_party_commands((s32)MsgWaitingForOpponent, text, 52);
#else
      Message_DecodeRaw__battle_party_commands((s32)MsgWaitingForOpponent, text, 32);
#endif
      Text_DrawWideStringAtOffset__battle_party_commands(text, state->timerWindow, 0, 4);
    }

    link = Ram_HeapSlots->battleWork;
    mode = 0;
    if (state->timerWindow == NULL && link->linkPaused == 0) {
      state->timerWindow = Window_Create__battle_party_commands(0, 16, 30, 4, 42);
      Vram_FillWindowBlockPattern__battle_party_commands();
      Func_0804bbd0();
    } else {
      state->timerWindow = NULL;
    }
    for (;;) {
      u16 *peerSignature = gLinkPeerSignatures__battle_party_commands[link->linkSide ^ 1];

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

// games/tla/include/system/affine.h's transform, written out: the stale
// affine.h beside these drafts would shadow it.
struct AffineTransform {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
  u16 unk06;
};

/* VIEW(battle_party_commands): routine AffineMatrix_BuildForEffect as s32 AffineMatrix_BuildForEffect(struct AffineTransform *transform); f04a85c has s32 AffineMatrix_BuildForEffect(struct TargetAffine *affine). */
extern s32 AffineMatrix_BuildForEffect__battle_party_commands(struct AffineTransform *transform) __asm__("AffineMatrix_BuildForEffect");

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
  matrix = AffineMatrix_BuildForEffect__battle_party_commands(&effect);
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


#define AFFINE_EFFECT_SLOT 232
void Func_0804cda8(void);

void *AffineEffect_InitializeWork(void) {
  void *work = HeapSlot_AllocEwram(AFFINE_EFFECT_SLOT, 152);

  Dma_Fill32(0, work, 152);
  Task_Add((TaskFunc)Func_0804cda8, TASK_PASS_VBLANK - 10);
  return work;
}

void Menu_EndResourceSelection(void) {
  struct Window *child;
  s32 i;
  u16 *entry;
  void *work;

  work = Ram_HeapSlots->menuSelectWork;
  Task_Remove(Func_0804cda8);
  child = FIELD_AT_OFFSET(work, struct Window **, 0x78);
  if (child != 0) {
    Window_Close(child, 2);
  }
  i = 0;
  while (i < (s32)FIELD_AT_OFFSET(work, s16 *, 0x8E)) {
    entry = (u16 *)((u8 *)work + 0x12) + i * 10;
    Resource_ResetEntry(*entry);
    i += 1;
  }
  HeapSlot_Release(232);
  wait(1U);
}


void Sound_PlayFar(s32 cue);
void WindowSprite_PrepareForRedraw(struct Window *);

s32 Menu_RunResourceSelectionLoop(s32 initial) {
  struct ResourceMenuWork *work = Ram_HeapSlots->menuSelectWork;
  s32 resource;

  work->selection = initial;
redraw:
  WindowSprite_PrepareForRedraw(work->window);
  if (work->resourceBase != 0)
    resource = work->resourceBase + work->selection;
  else
    resource = work->resourceIds[work->selection] + (s32)MsgCommandName;
  Text_DrawCharacterAtOffset(resource, work->window, 0, 0);
  for (;;) {
    wait(1);
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(112);
      return work->selection;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(113);
      return -1;
    }
    if (gInput.pressed & KEY_START) {
      Sound_PlayFar(113);
      return -1;
    }
    if ((gInput.repeat & KEY_LEFT) || (gInput.repeat & KEY_UP)) {
      Sound_PlayFar(111);
      work->selection--;
      if (work->selection < 0)
        work->selection = work->count - 1;
    } else if ((gInput.repeat & KEY_RIGHT) || (gInput.repeat & KEY_DOWN)) {
      Sound_PlayFar(111);
      work->selection++;
      if (work->selection >= work->count)
        work->selection = 0;
    } else
      continue;
    goto redraw;
  }
}


/// The menu sounds.
// Named as The Broken Seal's sounds.h names them (SE_MENU_*).
#define SOUND_MENU_CURSOR_MOVE 111
#define SOUND_MENU_CONFIRM 112

struct MenuSelectionState {
  u8 padding000[0x78];
  void *work;
  u8 padding07c[8];
  u8 resourceIds[8];
  s16 selection;
  s16 itemCount;
  s16 field090;
  s16 resourceBase;
};

extern u8 gMenu_SelectionStepDelays[];

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs) {
  if (diff >= 0)
    return diff;
  return rhs - lhs;
}

/// Steps the selection from start to goal, redrawing the label each step and
/// slowing as it nears the goal, then confirms.
// ☀️'s function of the same name in window/battle_command.c.
s32 Menu_SelectResource(s32 start, s32 goal) {
  s16 resourceBase;
  s16 cur;
  s32 diff;
  s32 resourceId;
  s32 dist;
  s32 pos;
  s32 step;
  s32 delay;
  const u8 *table;
  struct MenuSelectionState *state;

  state = Ram_HeapSlots->menuSelectWork;
  step = 1;
  delay = 12;
  state->selection = (s16)start;
  if (goal < start)
    step = -1;
  pos = start;

  for (;;) {
    WindowSprite_PrepareForRedraw(state->work);
    resourceBase = state->resourceBase;
    if (resourceBase != 0) {
      resourceId = resourceBase + state->selection;
    } else {
      resourceId = state->resourceIds[state->selection] + (s32)MsgCommandName;
    }
    Text_DrawCharacterAtOffset(resourceId, state->work, 0, 0);

    cur = state->selection;
    table = gMenu_SelectionStepDelays;
    diff = cur - goal;
    dist = AbsoluteDifference(diff, cur, goal);
    wait(table[dist] + delay);

    if (pos == goal)
      break;

    state->selection = (s16)((u16)state->selection + step);
    Sound_PlayFar(SOUND_MENU_CURSOR_MOVE);
    delay = 0;
    pos += step;
  }

  wait(48);
  Sound_PlayFar(SOUND_MENU_CONFIRM);
  return goal;
}

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;
/* VIEW(resource_work): routine Window_Create as struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style); f04a85c has struct TargetWindow *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags). */
extern struct Window *Window_Create__resource_work(s32 x, s32 y, s32 width, s32 height, s32 style) __asm__("Window_Create");

void Menu_LoadResourceSlot(s32 slot, s32 index) {
  s32 size = 1024;
  void *buffer = (void *)Heap_Alloc(size);
  u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  Resource_DecodeByteLzInRam((void *)((u32)base + base[index]), buffer);
  VramCache_Load(slot, size, buffer);
  Heap_Release(buffer);
}

void Menu_AppendResourceEntry(s32 no) {
  struct ResourceMenuWork *work;
  struct ResourceMenuEntry *entry;
  s16 index;
  s32 slot;
  s32 flags;

  work = Ram_HeapSlots->menuSelectWork;
  index = work->count;
  if (index <= 5) {
    work->count++;
    entry = &work->entries[index];
    slot = Resource_FindFreeEntry();
    Menu_LoadResourceSlot(slot, no);
    entry->x = index * 24 + 32;
    flags = 136;
    entry->y = flags;
    entry->slot = slot;
    work->resourceIds[index] = no;
  }
}

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *menu = Ram_HeapSlots->menuSelectWork;
  s32 x;
  s32 i;
  s32 count;

  width += 2;
  menu->width = width;
  menu->resourceBase = resourceBase;
  menu->row = row;
  count = menu->count;
  x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
  for (i = 0; i < menu->count; i++) {
    struct ResourceMenuEntry *entry = &menu->entries[i];

    entry->x = x * 8;
    entry->y = row * 8;
    x += 3;
  }
  menu->window = Window_Create__resource_work(x, row, menu->width, 3, 2);
}



/// Lays the entries out in a row three cells apart from x, on row y, and
/// opens the menu's window after them.
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *work;
  s32 i;
  struct ResourceMenuEntry *entry;
  s16 *widthp;

  work = Ram_HeapSlots->menuSelectWork;
  widthp = &work->width;
  *widthp = width + 2;
  work->resourceBase = resourceBase;
  work->row = y;
  for (i = 0; i < work->count; i++) {
    entry = &work->entries[i];
    entry->y = y << 3;
    entry->x = x << 3;
    x += 3;
  }
  work->window = Window_Create__resource_work(x, y, *widthp, 3, 2);
}

static __inline__ s32 TableGet(s8 *table, s32 index) { return table[index]; }

s32 Menu_SelectTopEntry(s32 selection) {
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

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  if (group == 0) {
    Menu_AppendResourceEntry(15);
  }
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();

  if (ret >= 0) {
    s8 *commands = gMenu_TopEntryCommandByPosition;
    s32 index;

    index = ret + offset + 1;
    ret = commands[index];
  }

  return ret;
}

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1) {
  PartyStatus_OpenAndWait();
  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  Menu_AppendResourceEntry(0xF);
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  // Only Japanese centres the entries before selecting.
#if LANGUAGE == LANGUAGE_JAPANESE
  Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
#endif
  arg1 = Menu_SelectResource(arg0, arg1 - 1);
  Menu_EndResourceSelection();
  Window_CloseAndRelease();
  return arg1;
}

extern s16 gData_020036d8;
extern s16 gData_02005350;
extern s8 gMenu_SaveSlotActionByPosition[];

s32 Func_080432a4(void);

/// Offers the actions the files allow (plus two optional entries
/// behind EWRAM flags) and maps the chosen position to an action. The second
/// optional entry is 31, and with the first option off, action 4 becomes 5.
// As ☀️'s Menu_SelectSaveSlotAction. A module of its own: in the Japanese
// edition it follows Menu_AnimateSelectionToEntry, which is still scaffolded
// there.
s32 Menu_SelectSaveSlotAction(void) {
  s32 type;
  s32 ret;
  s32 initial;
  u32 group;

  group = 0;
  initial = 0;
  type = Func_080432a4();
  if (type < 0)
    return -1;
  if (type == 0)
    return 0;
  if (type == 3)
    group = 1;
  else if (type == 103)
    group = 2;
  else if (type > 100)
    group = 3;
  else
    initial = 1;
  AffineEffect_InitializeWork();
  if (group == 0 || group == 3)
    Menu_AppendResourceEntry(21);
  if (group <= 1)
    Menu_AppendResourceEntry(22);
  if (group == 0 || group == 3)
    Menu_AppendResourceEntry(23);
  Menu_AppendResourceEntry(24);
  if (gData_020036d8 != 0)
    Menu_AppendResourceEntry(29);
  if (gData_02005350 != 0)
    Menu_AppendResourceEntry(31);
  Menu_CenterResourceEntries(17, SAVE_SLOT_MENU_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(initial);
  Menu_EndResourceSelection();
  if (ret >= 0) {
    u32 twice = group * 2;

    ret = gMenu_SaveSlotActionByPosition[ret + (twice + group) * 2];
  }
  if (gData_020036d8 == 0 && ret == 4)
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

struct MenuModeLabelState {
  u8 unk00[0x7c];
  void *window;
  u8 unk80[0x0c];
  s16 mode;
  u8 unk8e[8];
  s16 previousMode;
};

void Window_ClearInteriorTiles(void *window, s32 x, s32 y, s32 right,
                               s32 bottom);

/// When the selected mode changes, clears the lower window's label area and
/// writes the mode's lines of text. The area starts at x 8 and ends at
/// LABEL_RIGHT; a mode past 2 draws nothing.
// ☀️'s function of the same name.
void Menu_DrawModeLabel(void) {
  struct MenuModeLabelState *state = Ram_HeapSlots->menuSelectWork;

  if (state->previousMode != state->mode) {
    state->previousMode = state->mode;
    Window_ClearInteriorTiles(state->window, 8, 40, LABEL_RIGHT, 80);

    switch (state->mode) {
    case 0: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawCharacterAtOffset(text, state->window, 8, 40);
      Text_DrawCharacterAtOffset(text + 1, state->window, 8, 48);
      Text_DrawCharacterAtOffset(text + 2, state->window, 8, 56);
      Text_DrawCharacterAtOffset(text + 3, state->window, 8, 64);
      text += 4;
      Text_DrawCharacterAtOffset(text, state->window, 8, 72);
      break;
    }
    case 1: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawCharacterAtOffset(text, state->window, 8, 40);
      Text_DrawCharacterAtOffset(text + 1, state->window, 8, 48);
      text += 2;
      Text_DrawCharacterAtOffset(text, state->window, 8, 56);
      break;
    }
    case 2: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawCharacterAtOffset(text++, state->window, 8, 40);
      Text_DrawCharacterAtOffset(text, state->window, 8, 48);
      break;
    }
    }
  }
}

void Text_DrawMessage(s32 message, void *window, s32 x, s32 y);

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
  struct ResourceMenuWork *state = Ram_HeapSlots->menuSelectWork;
  s16 *shown = &state->previousMode;
  s16 *current = &state->selection;

#if LANGUAGE == LANGUAGE_FRENCH
  if (*shown != *current) {
    if (*current == 0) {
      WindowSprite_ClearList(state->lowerWindow);
      Text_DrawMessage((s32)MsgPasswordTransferHelp, state->lowerWindow, 8, 4);
      Text_DrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWindow, 0,
                       16);
    } else if (*shown == 0) {
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
    if (*current == 0) {
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
    case 0:
      Text_DrawMessage((s32)MsgPasswordTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWindow, 0,
                       16);
      break;
    case 1:
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

s32 Func_0804d7fc(s32 mode) {
  struct ResourceMenuWork *work;
  struct Window *window;
  s32 msg;
  struct Window **upper;
  s32 result;

  mode &= 1;
  AffineEffect_InitializeWork();
  work = Ram_HeapSlots->menuSelectWork;
  if (mode == 0) {
    Menu_AppendResourceEntry(44);
    Menu_AppendResourceEntry(45);
  } else {
    Menu_AppendResourceEntry(46);
    Menu_AppendResourceEntry(47);
    Menu_AppendResourceEntry(48);
  }
  Menu_CenterResourceEntries(17, ENTRY_WIDTH, 0);
  if (mode != 0) {
    Task_Add(Menu_DrawModeLabel, TASK_PASS_VBLANK - 10);
    work->previousMode = 0xffff;
    window = Window_Create__resource_work(PASSWORD_UPPER_X, 0, PASSWORD_UPPER_WIDTH, 4, 2);
    msg = (s32)MsgPasswordEntry;
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage(msg, window, 0, 4);
    window = Window_Create__resource_work(PASSWORD_LOWER_X, 4, PASSWORD_LOWER_WIDTH, 12, 2);
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
    window = Window_Create__resource_work(TRANSFER_UPPER_X, 0, TRANSFER_UPPER_WIDTH, 4, 2);
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage((s32)MsgTransferMethod, window, TRANSFER_TITLE_X, 4);
    work->lowerWindow =
        Window_Create__resource_work(TRANSFER_LOWER_X, 5, TRANSFER_LOWER_WIDTH, 7, 2);
  }
  result = Menu_RunResourceSelectionLoop(0);
  if (mode != 0)
    Task_Remove(Menu_DrawModeLabel);
  else
    Task_Remove(Menu_DrawModeIndicator);
  WindowSprite_PrepareForRedraw(*upper);
  WindowSprite_PrepareForRedraw(work->lowerWindow);
  Window_Close(*upper, 2);
  Window_Close(work->lowerWindow, 2);
  wait(1);
  Menu_EndResourceSelection();
  return result;
}

// Menu: select command icons, such as Yes and No.

/// Selects a shop command from icons 0x11-0x14, centred on row 17, showing
/// the selected command's name in a window. Icons are indices into
/// ResourceId_CommandIcons.
/// selection: initial selection. Returns the selected position.
s32 Menu_SelectShopCommand(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x11);
  Menu_AppendResourceEntry(0x12);
  Menu_AppendResourceEntry(0x13);
  Menu_AppendResourceEntry(0x14);
#if (LANGUAGE == LANGUAGE_GERMAN)
  Menu_CenterResourceEntries(17, 8, 0); // Name window width.
#elif (LANGUAGE != LANGUAGE_JAPANESE)
  Menu_CenterResourceEntries(17, 7, 0);
#else
  Menu_CenterResourceEntries(17, 6, 0);
#endif
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

/// Selects a sanctum service from icons 0x19-0x1c, centred on row 17.
s32 Menu_SelectSanctumService(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x19);
  Menu_AppendResourceEntry(0x1A);
  Menu_AppendResourceEntry(0x1B);
  Menu_AppendResourceEntry(0x1C);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  Menu_CenterResourceEntries(17, 10, 0);
#else
  Menu_CenterResourceEntries(17, 8, 0);
#endif
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

/// Selects Yes or No (icons 5 and 6). Cancel returns 1 (No).
/// low: row 0 if zero, otherwise row 17.
/// message: first name message, or 0 for command names.
/// w: name window width, or 3 if zero.
/// selection: initial selection.
s32 Menu_RunConfirmSelection(s32 low, s32 message, s32 w, s32 selection) {
  s32 row;

  row = 0;

  AffineEffect_InitializeWork();

  if (w == 0) {
    w = 3;
  }
  if (low != 0) {
    row = 17;
  }

  Menu_AppendResourceEntry(5);
  Menu_AppendResourceEntry(6);
  Menu_CenterResourceEntries(row, w, message);

  selection = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  if (selection == -1) {
    selection = 1;
  }
  return selection;
}

/// Selects Yes or No at (x, y). Cancel returns 1 (No).
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 selection) {
  s32 message = (s32)MsgCommandYes;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(5);
  Menu_AppendResourceEntry(6);
  Menu_LayoutResourceEntries(x, y, 3, message);
  selection = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  if (selection == -1) {
    selection = 1;
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
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x20);
  Menu_AppendResourceEntry(0x21);
  Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

/* VIEW(resource_work): routine Menu_RunSelection as void Menu_RunSelection(void); the definition has s16 Menu_RunSelection(void); tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/window/f04a85c.c:3143: `Menu_RunSelection' undeclared (first use in this function). */
extern void Menu_RunSelection__resource_work(void) __asm__("Menu_RunSelection");

void Func_0804dabc(void) { Task_Add(Menu_RunSelection__resource_work, TASK_PASS_VBLANK); }
#undef AFFINE_EFFECT_SLOT
#undef SOUND_MENU_CURSOR_MOVE
#undef SOUND_MENU_CONFIRM
#undef Resource_DecodeByteLzInRam
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

/// @file
/// Debug: the party presets, the message viewer and the battle test menu.
/// Part of battle.c, the battle object, compiled only through it.

#include "../battle_internal.h"

// The window work's byte that marks a message window busy; it moved when the
// international editions grew the work.
#if TBS_PATCH_1
#define WINDOW_WORK_MESSAGE_BUSY 0x12f8
#else
#define WINDOW_WORK_MESSAGE_BUSY 0x1188
#endif

// The first message of the viewer's second range: in Japanese, an earlier
// copy of the first range's opening lines; the localizations moved it
// elsewhere.
#if (LANGUAGE == LANGUAGE_ENGLISH)
#define DEBUG_SECOND_FORM MsgBecameCursed
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define DEBUG_SECOND_FORM MsgEnd
#else
#define DEBUG_SECOND_FORM MsgDebugSecondForm
#endif

/// One row of the debug party presets (presets.tsv): a party member, the
/// level to raise them to, the djinn of each element to give them, the items
/// to equip and up to two abilities to teach. A row whose owner is -1 ends a
/// preset.
struct DebugPartyEntry {
  s8 owner;                     ///< the party member, or DEBUG_PRESET_END
  u8 level;                     ///< the level the member is raised to
  s8 djinnCounts[NUM_ELEMENTS]; ///< Djinn of each element to give and Set
  u16 items[4];                 ///< items to add and equip; 0 for none
  u16 abilities[2];             ///< abilities to teach; 0 for none
  u16 unused;                   ///< 0 in every row
};

extern struct DebugPartyEntry gDebug_PartyPresets[];

/// The last row of presets.tsv; the scan reports a missing preset past it.
#define DEBUG_PRESET_LAST_ROW 332
#define DEBUG_PRESET_END -1

/// Debug: replaces the party with the members of preset number preset.
/// Each member is raised to the row's level, given the row's Djinn (Set, in
/// order of their numbers across the party), the row's abilities, and an
/// emptied inventory holding the row's items, each equipped. Called by
/// DebugBattle_RunMenu and, through its far stub, by the link lobby's
/// LinkLobby_ChooseDebugPartyPreset. Changes the active party and each
/// member's level, Djinn, abilities and inventory.
/// Returns 1 when the table ends before that preset, 0 otherwise.
s32 DebugParty_LoadPreset(s32 preset) {
  // The next Djinni number of each element to hand out.
  s32 nextDjinn[NUM_ELEMENTS];
  struct BattleUnit *member;
  s32 owner;
  s32 notFound;
  s32 row;
  s32 djinnIndex;
  s32 currentPreset;
  s32 i;

  notFound = 0;
  // Empty the active party; the preset's rows add its members back.
  Party_RemoveActiveOwnerFar(ISAAC);
  Party_RemoveActiveOwnerFar(GARET);
  Party_RemoveActiveOwnerFar(IVAN);
  Party_RemoveActiveOwnerFar(MIA);
  Party_RemoveActiveOwnerFar(JENNA);

  currentPreset = 0;
  for (i = 0; i < NUM_ELEMENTS; i++)
    nextDjinn[i] = 0;

  // Presets are runs of rows each ended by a DEBUG_PRESET_END row; skip
  // to the run numbered preset and apply its rows.
  for (row = 0;; row++) {
    owner = gDebug_PartyPresets[row].owner;
    if (row < 0 || row > DEBUG_PRESET_LAST_ROW) {
      notFound = 1;
      break;
    }
    if (owner == DEBUG_PRESET_END) {
      if (currentPreset == preset)
        break;
      currentPreset++;
      continue;
    }
    if (currentPreset != preset)
      continue;

    Party_AddActiveOwnerFar(owner);
    Unit_LevelUpToFar(owner, gDebug_PartyPresets[row].level);

    // Start from no djinn and no abilities.
    member = (struct BattleUnit *)UnitGetFar(owner);
    for (i = 0; i < NUM_ELEMENTS; i++) {
      member->djinnOwnedCounts[i] = 0;
      member->djinnActiveCounts[i] = 0;
      member->djinnAvailable[i] = 0;
      member->djinnActive[i] = 0;
    }
    // The slot's first halfword, its encodedAction, stored through a cast:
    // the zero comes from the literal pool, as in the ROM (MATCHING.md).
    for (i = 0; i < 32; i++)
      *(u16 *)&member->actionSlots[i] = 0;
    for (i = 0; i < 2; i++) {
      if (gDebug_PartyPresets[row].abilities[i] != 0)
        Unit_LearnPsynergyFar(owner, gDebug_PartyPresets[row].abilities[i]);
    }

    // Djinn are numbered across the party, so no two members get the same.
    for (i = 0; i < NUM_ELEMENTS; i++) {
      for (djinnIndex = 0; djinnIndex < gDebug_PartyPresets[row].djinnCounts[i];
           djinnIndex++) {
        Djinn_AddToOwnerFar(owner, i, nextDjinn[i]);
        Djinn_ActivateFar(owner, i, nextDjinn[i]);
        nextDjinn[i]++;
      }
    }

    // Empty the inventory, removing slot 0 one time more than there are
    // slots, then add and equip the row's items.
    for (i = 0; i < INVENTORY_SLOTS + 1; i++)
      Inventory_RemoveFar(owner, 0);
    for (i = 0; i < 4; i++) {
      if (gDebug_PartyPresets[row].items[i] != 0)
        Inventory_EquipFar(
            owner,
            Inventory_AddItemFar(owner, gDebug_PartyPresets[row].items[i]));
    }
  }
  return notFound;
}

/// The highest message index the viewer reaches.
#define DEBUG_VIEW_LIMIT                                                       \
  (MsgLaliveroHowDidSearchForSheba -                                           \
   MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely + 5)

/// Debug: a message viewer that never returns. Names the first party member
/// "Ha hi fu he ho" (MsgDebugSampleName), fills the message arguments with
/// sample values and shows one message at a time, counted from the Babi
/// Lighthouse dialogue (MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely) or,
/// in the second range, from DEBUG_SECOND_FORM. Right and Left step the
/// index, R and L step it by ten, Up and Down pick the range, and B
/// alternates between the ranges, moving on one message each time it leaves
/// the first. A closes the window and shows the message again. Called by
/// DebugBattle_RunMenu when Start is pressed. Renames party member 0, sets
/// REG_DISPCNT and gGameState.messageSpeed, and clears the window work's
/// message busy byte after each window.
void DebugBattle_ViewMessages(void) {
  u16 text[64];
  s32 secondRange = 0;
  struct BattleUnit *unit = UnitGetFar(0);
  s32 i;
  s32 index;
  struct UiWindow *messageWindow;
  s32 message;

  // Party member 0's name is the sample a `<unit>` blank shows.
  Ui_AdjustValueWithoutLimitFar(MsgDebugSampleName, text);
  for (i = 0; i < UNIT_NAME_LENGTH; i++) {
    unit->name[i] = text[i];
    if (text[i] == 0)
      break;
  }
  unit->name[UNIT_NAME_LENGTH] = 0;
  UiWork_InitializeWithResourceCountersFar();
  AudioCommand_PlayFar(MUS_071);
  for (;;) {
    index = 0;
    {
      s32 control = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                    DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;

      REG_DISPCNT = control;
    }
    gGameState.messageSpeed = 2;
    for (;;) {
      // Sample arguments for the message's placeholders.
      UiWork_ClearValueNameTablesFar();
      UiWork_PushValueSlotFar(999, VALUE_NUMBER);
      UiWork_PushValueSlotFar(0, VALUE_CLASS_NAME);
      UiWork_PushValueSlotFar(1, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(1, VALUE_ITEM_NAME);
      UiWork_PushValueSlotFar(2, VALUE_ABILITY_NAME);
      if (secondRange == 0) {
        message = MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely;
        messageWindow = UiText_OpenMessageWindowFar(index + message, 2, 10, 4);
      } else {
        message = DEBUG_SECOND_FORM;
        messageWindow = UiText_OpenMessageWindowFar(index + message, 2, 2, 4);
      }
      WaitFrames(10);

      // Wait for a key that picks another message, or for A once the
      // message has been shown in full.
      for (;;) {
        if (gKeysRepeat & KEY_B) {
          if (secondRange != 0) {
            secondRange = 0;
          } else {
            index++;
            secondRange = 1;
          }
        }
        if (gKeysRepeat & KEY_RIGHT)
          index++;
        if (gKeysRepeat & KEY_LEFT)
          index -= 2;
        if (gKeysRepeat & KEY_UP)
          secondRange = 1;
        if (gKeysRepeat & KEY_DOWN)
          secondRange = 0;
        if (gKeysRepeat & KEY_R)
          index += 10;
        if (gKeysRepeat & KEY_L)
          index -= 10;
        if (index < 0)
          index = 0;
        if (index >= (u32)DEBUG_VIEW_LIMIT)
          index = DEBUG_VIEW_LIMIT;
        if (gKeysRepeat & (KEY_B | KEYS_DPAD | KEYS_SHOULDERS))
          break;
        if (UiWork_IsCompleteFar() != 0 && (gKeysRepeat & KEY_A))
          break;
        WaitFrames(1);
      }
      UiWork_DrainPendingFar(1);
      // Close the window and free the message system for the next one.
      UiWork_FinalizeFar(messageWindow, 1);
      (*(u8 **)&gWindowWork)[WINDOW_WORK_MESSAGE_BUSY] = 0;
    }
  }
}

/// The encounter the debug battle starts with, and the Djinn tutorial's.
#define DEBUG_FIRST_ENCOUNTER 257
#define DJINN_TUTORIAL_ENCOUNTER 28

/// Debug: the battle test menu, which never returns. Without Down held it
/// fights encounter 257 again and again with auto battle on. Holding Down at
/// the start opens the chooser: Right and Left step the encounter, Up and
/// Down by ten, R and L pick the party preset, Start opens the message
/// viewer and A fights; B marks every later battle with start mode 5.
/// Holding Down when A is pressed makes it a link battle. Exported as
/// DebugBattle_RunMenuFar; nothing in the source calls it. Sets
/// FLAG_DEBUG_BATTLE and, as chosen, FLAG_AUTO_BATTLE, FLAG_LINK_BATTLE and
/// FLAG_DJINN_TUTORIAL, and resets the task table, heap and VRAM cache
/// before each battle.
void DebugBattle_RunMenu(void) {
  s32 startModeSet = 0;
  s32 encounter;
  s32 preset;
  s32 loadedPreset;

  GameState_InitDefaultsFar();
restart:
  Ui_LoadWindowGraphics();
  DebugText_Clear();
  Scheduler_ResetTaskTable();
  Runtime_InitializeHeap();
  VramCache_Reset();
  GameFlag_SetBitFar(FLAG_AUTO_BATTLE);
  encounter = DEBUG_FIRST_ENCOUNTER;
  if (gKeysHeld & KEY_DOWN) {
    loadedPreset = -1;
    GameFlag_ClearBitFar(FLAG_AUTO_BATTLE);
    preset = 0;
    for (;;) {
      GameFlag_ClearBitFar(FLAG_CHILDHOOD);
      WaitFrames(1);
      for (;;) {
        if (gKeysRepeat & KEY_RIGHT)
          encounter++;
        if (gKeysRepeat & KEY_LEFT)
          encounter--;
        if (gKeysRepeat & KEY_UP)
          encounter -= 10;
        if (gKeysRepeat & KEY_DOWN)
          encounter += 10;
        if (gKeysRepeat & KEY_R)
          preset++;
        if (gKeysRepeat & KEY_L)
          preset--;
        if (gKeysRepeat & KEY_A)
          break;
        if (gKeysRepeat & KEY_START)
          DebugBattle_ViewMessages();
        if (gKeysRepeat & KEY_SELECT)
          Battle_ReservedNoOp2A08();
        if ((gKeysRepeat & KEY_B) || startModeSet) {
          startModeSet = 1;
          Battle_SetStart(5);
        }
        if (preset != loadedPreset) {
          GameState_InitDefaultsFar();
          DebugParty_LoadPreset(preset);
          loadedPreset = preset;
        }
        WaitFrames(1);
      }
      // A fights: Down still held makes it a link battle.
      if (gKeysHeld & KEY_DOWN)
        GameFlag_SetBitFar(FLAG_LINK_BATTLE);
      UnitCalcFar(0);
      gGameState.special = (s32)&ResourceId_SuhallaGateBackdrop;
      if (encounter == DJINN_TUTORIAL_ENCOUNTER)
        GameFlag_SetBitFar(FLAG_DJINN_TUTORIAL);
      GameFlag_SetBitFar(FLAG_DEBUG_BATTLE);
      Battle_RunEncounter(encounter);
      // Rebuild what the battle replaced, then return to the chooser.
      Ui_LoadWindowGraphics();
      DebugText_Clear();
      Scheduler_ResetTaskTable();
      Runtime_InitializeHeap();
      VramCache_Reset();
    }
  }
  // Without Down: the same auto battle, over and over.
  GameFlag_SetBitFar(FLAG_DEBUG_BATTLE);
  Battle_RunEncounter(DEBUG_FIRST_ENCOUNTER);
  goto restart;
}

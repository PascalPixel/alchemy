/// @file
/// Debug: the party presets, the message viewer and the battle test menu.
// Part of battle.c.

#include "../battle_internal.h"

// The window work's byte that marks a message window busy; it moved when the
// international editions grew the work.
#if LANGUAGE != LANGUAGE_JAPANESE
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

// Declared here without const, as Camelot's code read it: with const,
// alchemy-gcc compiles DebugParty_LoadPreset differently. debug_presets.c
// defines it, const, in ROM.
extern struct DebugPartyEntry gDebug_PartyPresets[];

/// The last row of gDebug_PartyPresets; the scan reports a missing preset past
/// it.
#define DEBUG_PRESET_LAST_ROW 332

/// Debug: replaces the party with the members of preset number preset.
/// Each member is raised to the row's level, given the row's Djinn (Set, in
/// order of their numbers across the party), the row's abilities, and an
/// emptied inventory holding the row's items, each equipped. Called by
/// DebugBattle_RunMenu and, through its far stub, by the Battle Arena's
/// BattleArena_ChooseDebugPartyPreset. Changes the active party and each
/// member's level, Djinn, abilities and inventory.
/// Returns 1 when the table ends before that preset, 0 otherwise.
s32 DebugParty_LoadPreset(s32 preset) {
  // The next Djinni number of each element to hand out.
  s32 nextDjinn[ELEMENT_COUNT];
  struct Unit *member;
  s32 unitId;
  s32 notFound;
  s32 row;
  s32 djinnIndex;
  s32 currentPreset;
  s32 i;

  notFound = 0;
  // Empty the active party; the preset's rows add its members back.
  Party_RemoveMemberFar(ISAAC);
  Party_RemoveMemberFar(GARET);
  Party_RemoveMemberFar(IVAN);
  Party_RemoveMemberFar(MIA);
  Party_RemoveMemberFar(JENNA);

  currentPreset = 0;
  for (i = 0; i < ELEMENT_COUNT; i++)
    nextDjinn[i] = 0;

  // Presets are runs of rows each ended by a DEBUG_PRESET_END row; skip
  // to the run numbered preset and apply its rows.
  for (row = 0;; row++) {
    unitId = gDebug_PartyPresets[row].unitId;
    if (row < 0 || row > DEBUG_PRESET_LAST_ROW) {
      notFound = 1;
      break;
    }
    if (unitId == DEBUG_PRESET_END) {
      if (currentPreset == preset)
        break;
      currentPreset++;
      continue;
    }
    if (currentPreset != preset)
      continue;

    Party_AddMemberFar(unitId);
    Unit_LevelUpToFar(unitId, gDebug_PartyPresets[row].level);

    // Start from no Djinn and no abilities.
    member = (struct Unit *)Unit_GetFar(unitId);
    for (i = 0; i < ELEMENT_COUNT; i++) {
      member->djinnHeldCounts[i] = 0;
      member->djinnSetCounts[i] = 0;
      member->djinnHeld[i] = 0;
      member->djinnSet[i] = 0;
    }
    // The slot's first halfword, its encodedAction, stored through a cast:
    // the zero comes from the literal pool, as in the ROM (MATCHING.md).
    for (i = 0; i < 32; i++)
      *(u16 *)&member->psynergy[i] = 0;
    for (i = 0; i < 2; i++) {
      if (gDebug_PartyPresets[row].abilities[i] != 0)
        Unit_LearnPsynergyFar(unitId, gDebug_PartyPresets[row].abilities[i]);
    }

    // Djinn are numbered across the party, so no two members get the same.
    for (i = 0; i < ELEMENT_COUNT; i++) {
      for (djinnIndex = 0; djinnIndex < gDebug_PartyPresets[row].djinnCounts[i];
           djinnIndex++) {
        Djinn_GiveFar(unitId, i, nextDjinn[i]);
        Djinn_SetFar(unitId, i, nextDjinn[i]);
        nextDjinn[i]++;
      }
    }

    // Empty the inventory, removing slot 0 one time more than there are
    // slots, then add and equip the row's items.
    for (i = 0; i < INVENTORY_SLOTS + 1; i++)
      Inventory_RemoveFar(unitId, 0);
    for (i = 0; i < 4; i++) {
      if (gDebug_PartyPresets[row].items[i] != 0)
        Inventory_EquipFar(
            unitId,
            Inventory_AddItemFar(unitId, gDebug_PartyPresets[row].items[i]));
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
/// in the second range, from DEBUG_SECOND_FORM. Right steps the index on by
/// one and Left back by two, R and L step it by ten, Up and Down pick the
/// range, and B alternates between the ranges, moving on one message each
/// time it leaves the first. A closes the window and shows the message again.
/// Called by DebugBattle_RunMenu when Start is pressed. Renames party member
/// 0, sets REG_DISPCNT and gSaveBlock.messageSpeed, and clears the window
/// work's message busy byte after each window.
void DebugBattle_ViewMessages(void) {
  u16 text[64];
  s32 secondRange = 0;
  struct Unit *unit = Unit_GetFar(0);
  s32 i;
  s32 index;
  struct Window *messageWindow;
  s32 message;

  // Party member 0's name is the sample a `<unit>` blank shows.
  Message_DecodeRawUnboundedFar(MsgDebugSampleName, text);
  for (i = 0; i < UNIT_NAME_LENGTH; i++) {
    unit->name[i] = text[i];
    if (text[i] == 0)
      break;
  }
  unit->name[UNIT_NAME_LENGTH] = 0;
  WindowSystem_InitFieldFar();
  Sound_PlaySongFar(MUS_EMPTY);
  for (;;) {
    index = 0;
    {
      s32 control = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                    DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;

      REG_DISPCNT = control;
    }
    // The fastest text speed (window/message_speed.c: 0 to 2, slowest first).
    gSaveBlock.messageSpeed = 2;
    for (;;) {
      // Sample arguments for the message's placeholders.
      MessageArgument_ClearAllFar();
      MessageArgument_PushFar(999, MESSAGE_ARGUMENT_NUMBER);
      MessageArgument_PushFar(0, MESSAGE_ARGUMENT_CLASS);
      MessageArgument_PushFar(1, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(1, MESSAGE_ARGUMENT_ITEM);
      MessageArgument_PushFar(2, MESSAGE_ARGUMENT_ABILITY);
      // The window at tile (2, 10) for the first range, (2, 2) for the
      // second.
      if (secondRange == 0) {
        message = MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely;
        messageWindow = Message_OpenWindowFar(index + message, 2, 10, 4);
      } else {
        message = DEBUG_SECOND_FORM;
        messageWindow = Message_OpenWindowFar(index + message, 2, 2, 4);
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
        if (TextPrinter_AllFinishedFar() != 0 && (gKeysRepeat & KEY_A))
          break;
        WaitFrames(1);
      }
      Window_CloseAllFar(1);
      // Close the window and free the message system for the next one.
      Window_CloseFar(messageWindow, 1);
      (*(u8 **)&gWindowWork)[WINDOW_WORK_MESSAGE_BUSY] = 0;
    }
  }
}

/// The encounter the debug battle starts with (two or three Kobolds), and
/// the Djinn tutorial's: rows of formations.c.
#define DEBUG_FIRST_ENCOUNTER 257
#define DJINN_TUTORIAL_ENCOUNTER 28

/// Debug: the battle test menu, which never returns. Without Down held it
/// fights DEBUG_FIRST_ENCOUNTER again and again with auto battle on. Holding
/// Down at the start opens the chooser: Right and Left step the encounter, Up
/// and Down by ten, R and L pick the party preset, Start opens the message
/// viewer and A fights; B marks every later battle BATTLE_TYPE_DEBUG.
/// Holding Down when A is pressed makes it a link battle. Exported as
/// DebugBattle_RunMenuFar; nothing in the source calls it. Sets
/// FLAG_DEBUG_BATTLE and, as chosen, FLAG_AUTO_BATTLE, FLAG_LINK_BATTLE and
/// FLAG_DJINN_LESSON, and resets the task table, heap and VRAM cache
/// before each battle.
void DebugBattle_RunMenu(void) {
  s32 startModeSet = 0;
  s32 encounter;
  s32 preset;
  s32 loadedPreset;

  SaveBlock_InitializeDefaultsFar();
restart:
  Window_LoadGraphics();
  DebugText_Clear();
  Task_ResetAll();
  InitHeap();
  VramCache_Reset();
  GameFlag_SetFar(FLAG_AUTO_BATTLE);
  encounter = DEBUG_FIRST_ENCOUNTER;
  if (gKeysHeld & KEY_DOWN) {
    loadedPreset = -1;
    GameFlag_ClearFar(FLAG_AUTO_BATTLE);
    preset = 0;
    for (;;) {
      GameFlag_ClearFar(FLAG_CHILDHOOD);
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
          Battle_SetType(BATTLE_TYPE_DEBUG);
        }
        if (preset != loadedPreset) {
          SaveBlock_InitializeDefaultsFar();
          DebugParty_LoadPreset(preset);
          loadedPreset = preset;
        }
        WaitFrames(1);
      }
      // A fights: Down still held makes it a link battle. The battle's
      // background comes from gSaveBlock.special
      // (FieldScript_GetSpecialValueFar in Battle_Run).
      if (gKeysHeld & KEY_DOWN)
        GameFlag_SetFar(FLAG_LINK_BATTLE);
      Unit_CalculateFar(0);
      gSaveBlock.special = (s32)&ResourceId_SuhallaGateBackdrop;
      if (encounter == DJINN_TUTORIAL_ENCOUNTER)
        GameFlag_SetFar(FLAG_DJINN_LESSON);
      GameFlag_SetFar(FLAG_DEBUG_BATTLE);
      Battle_Run(encounter);
      // Rebuild what the battle replaced, then return to the chooser.
      Window_LoadGraphics();
      DebugText_Clear();
      Task_ResetAll();
      InitHeap();
      VramCache_Reset();
    }
  }
  // Without Down: the same auto battle, over and over.
  GameFlag_SetFar(FLAG_DEBUG_BATTLE);
  Battle_Run(DEBUG_FIRST_ENCOUNTER);
  goto restart;
}

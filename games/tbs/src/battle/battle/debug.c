/// @file
/// Debug: the parameter sets, the message viewer and the battle test menu.
// Part of battle.c.

#include "../battle_internal.h"

// The first message of the viewer's second range.
#if (LANGUAGE == LANGUAGE_ENGLISH)
#define DEBUG_SECOND_FORM MsgBecameCursed
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define DEBUG_SECOND_FORM MsgEnd
#else
#define DEBUG_SECOND_FORM MsgDebugSecondForm
#endif

/// The last row of gDebug_ParameterSets; the scan reports a missing parameter
/// set past it.
#define DEBUG_PARAMETER_SET_LAST_ROW 332

/// Debug: replaces the party with parameter set number set, each member at its
/// level with its Djinn, abilities and equipped items. Returns 1 when the table
/// ends before that set.
s32 DebugParameterSet_Load(s32 set) {
  // The next Djinni number of each element to hand out.
  s32 nextDjinn[ELEMENT_COUNT];
  struct Unit *member;
  s32 unitId;
  s32 notFound;
  s32 row;
  s32 djinnIndex;
  s32 currentSet;
  s32 i;

  notFound = 0;
  // Empty the active party; the parameter set's rows add its party
  // members back.
  Party_RemoveMemberFar(ISAAC);
  Party_RemoveMemberFar(GARET);
  Party_RemoveMemberFar(IVAN);
  Party_RemoveMemberFar(MIA);
  Party_RemoveMemberFar(JENNA);

  currentSet = 0;
  for (i = 0; i < ELEMENT_COUNT; i++)
    nextDjinn[i] = 0;

  // Parameter sets are runs of rows each ended by a DEBUG_PARAMETER_SET_END
  // row; skip to the run numbered set and apply its rows.
  for (row = 0;; row++) {
    unitId = gDebug_ParameterSets[row].unitId;
    if (row < 0 || row > DEBUG_PARAMETER_SET_LAST_ROW) {
      notFound = 1;
      break;
    }
    if (unitId == DEBUG_PARAMETER_SET_END) {
      if (currentSet == set)
        break;
      currentSet++;
      continue;
    }
    if (currentSet != set)
      continue;

    Party_AddMemberFar(unitId);
    Unit_LevelUpToFar(unitId, gDebug_ParameterSets[row].level);

    // Start from no Djinn and no abilities.
    member = Unit_GetFar(unitId);
    for (i = 0; i < ELEMENT_COUNT; i++) {
      member->djinnHeldCounts[i] = 0;
      member->djinnSetCounts[i] = 0;
      member->djinnHeld[i] = 0;
      member->djinnSet[i] = 0;
    }
    for (i = 0; i < 32; i++)
      *&member->psynergy[i].encodedAction = 0;
    for (i = 0; i < 2; i++) {
      if (gDebug_ParameterSets[row].abilities[i] != 0)
        Unit_LearnPsynergyFar(unitId, gDebug_ParameterSets[row].abilities[i]);
    }

    // Djinn are numbered across the party, so no two party members get the
    // same.
    for (i = 0; i < ELEMENT_COUNT; i++) {
      for (djinnIndex = 0;
           djinnIndex < gDebug_ParameterSets[row].djinnCounts[i];
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
      if (gDebug_ParameterSets[row].items[i] != 0)
        Inventory_EquipFar(
            unitId,
            Inventory_AddItemFar(unitId, gDebug_ParameterSets[row].items[i]));
    }
  }
  return notFound;
}

/// The highest message index the viewer reaches.
#define DEBUG_VIEW_LIMIT                                                       \
  (MsgLaliveroHowDidSearchForSheba -                                           \
   MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely + 5)

/// Debug: a message viewer that never returns, showing one message at a time
/// with sample arguments. Left and Right step it, L and R by ten, Up, Down and
/// B pick the range, and A shows it again.
void DebugBattle_ViewMessages(void) {
  u16 text[64];
  s32 secondRange = 0;
  struct Unit *unit = Unit_GetFar(ISAAC);
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
  Sound_PlayFar(MUS_EMPTY);
  for (;;) {
    index = 0;
    {
      s32 control = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                    DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;

      REG_DISPCNT = control;
    }
    // The fastest text speed (window/message_speed.c: 0 to 2, slowest first).
    gMainRoutineWork.messageSpeed = 2;
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
      wait(10);

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
        if (Message_AllTypedFar() != 0 && (gKeysRepeat & KEY_A))
          break;
        wait(1);
      }
      Window_CloseAllFar(1);
      // Close the window and clear its page arrow.
      Window_CloseFar(messageWindow, 1);
      ((struct WindowWork *)gWindowWork[0])->pageArrowShown = 0;
    }
  }
}

/// The encounter the debug battle starts with (two or three Kobolds), and
/// the Djinn tutorial's: rows of formations.c.
#define DEBUG_FIRST_ENCOUNTER 257
#define DJINN_TUTORIAL_ENCOUNTER 28

/// Debug: the battle test menu, which never returns. It fights the first
/// encounter again and again with auto battle on, unless Down is held at the
/// start to open the chooser of encounter and parameter set.
void DebugBattle_RunMenu(void) {
  s32 startModeSet = 0;
  s32 encounter;
  s32 set;
  s32 loadedSet;

  MainRoutineWork_InitializeDefaultsFar();
restart:
  Window_LoadGraphics();
  DebugText_Clear();
  Task_ResetAll();
  Heap_Initialize();
  VramCache_Reset();
  GameFlag_SetFar(FLAG_AUTO_BATTLE);
  encounter = DEBUG_FIRST_ENCOUNTER;
  if (gKeysHeld & KEY_DOWN) {
    loadedSet = -1;
    GameFlag_ClearFar(FLAG_AUTO_BATTLE);
    set = 0;
    for (;;) {
      GameFlag_ClearFar(FLAG_CHILDHOOD);
      wait(1);
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
          set++;
        if (gKeysRepeat & KEY_L)
          set--;
        if (gKeysRepeat & KEY_A)
          break;
        if (gKeysRepeat & KEY_START)
          DebugBattle_ViewMessages();
        if (gKeysRepeat & KEY_SELECT)
          Battle_DebugSelectNoOp();
        if ((gKeysRepeat & KEY_B) || startModeSet) {
          startModeSet = 1;
          Battle_SetType(BATTLE_TYPE_DEBUG);
        }
        if (set != loadedSet) {
          MainRoutineWork_InitializeDefaultsFar();
          DebugParameterSet_Load(set);
          loadedSet = set;
        }
        wait(1);
      }
      // A fights; Down still held makes it a link battle.
      if (gKeysHeld & KEY_DOWN)
        GameFlag_SetFar(FLAG_LINK_BATTLE);
      Unit_CalculateFar(ISAAC);
      gMainRoutineWork.battleBackground = RESOURCE_SUHALLA_GATE_BACKDROP;
      if (encounter == DJINN_TUTORIAL_ENCOUNTER)
        GameFlag_SetFar(FLAG_DJINN_LESSON);
      GameFlag_SetFar(FLAG_DEBUG_BATTLE);
      Battle_Run(encounter);
      // Rebuild what the battle replaced, then return to the chooser.
      Window_LoadGraphics();
      DebugText_Clear();
      Task_ResetAll();
      Heap_Initialize();
      VramCache_Reset();
    }
  }
  // Without Down: the same auto battle, over and over.
  GameFlag_SetFar(FLAG_DEBUG_BATTLE);
  Battle_Run(DEBUG_FIRST_ENCOUNTER);
  goto restart;
}

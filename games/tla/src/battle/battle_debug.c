/// @file
/// Debug: the unit snapshot, the parameter sets and the battle test menu.
// Part of battle.c.

#include "battle_internal.h"
#include "flags.h"
#include "gamepak/far_call/game.h"
#include "battle/battle.h"
#include "battle/presentation.h"
#include "system/system.h"
#include "resource_ids.h"

/// Unused: runs random encounters one after another, forever.
UNUSED void DebugBattle_RunForever(void) {
  for (;;)
    Battle_Run(0);
}

/// After the first two parts of Karst and Agatio's battle
/// (FORMATION_KARST_AND_AGATIO_A and _B), snapshots the five units of
/// gScriptedBattle_Units that fought (Felix, Piers and Jenna, and the first two
/// enemies) for ScriptedBattle_RestoreUnits to bring back as the next part
/// begins.
void ScriptedBattle_SaveUnits(void) {
  s32 j;
  struct Snapshot *snapshot = &gScriptedBattle_Snapshot;
  s32 i = 0;
  u8 *ids = gScriptedBattle_Units;
  struct SnapshotCopy *copies = gScriptedBattle_SnapshotBytes;

  for (; i <= 4; i++, copies++) {
    struct Unit *unit = Unit_Get(ids[i]);

    snapshot->listed[i] = 0;
    if (BattleUnit_IsListed(ids[i])) {
      snapshot->listed[i] = 1;
      if (i > 2) {
        snapshot->vitals[i].hp = unit->hp > 0 ? unit->hp : 1;
        {
          struct SnapshotVitals *pv;
          u32 pp;

          /* FAKEMATCH(temporary): the PP value held in a local while pv is
           * built. */
          pp = (u16)unit->pp;
          /* FAKEMATCH(signature): j, the boosts' s32 index, holds the PP rows'
           * address through casts; as a pointer of their own type. */
          j = (s32)gScriptedBattle_SnapshotVitals;
          pv = (struct SnapshotVitals *)j + i;
          /* FAKEMATCH(dead): a store the tag loop's init overwrites. */
          if (i == 300)
            j = 0;
          pv->pp = pp;
        }
      }
      if (i <= 2) {
        for (j = 0; j < ELEMENT_COUNT; j++)
          snapshot->summonBoosts[i][j] = unit->summonBoost[j];
      }
      copies->curse = unit->curse;
      copies->poison = unit->poison;
      copies->attackTurns = unit->attackTurns;
      copies->attackStage = unit->attackStage;
      copies->defenseTurns = unit->defenseTurns;
      copies->defenseStage = unit->defenseStage;
      copies->resistanceTurns = unit->resistanceTurns;
      copies->resistanceStage = unit->resistanceStage;
      copies->delusion = unit->delusion;
      copies->confusion = unit->confusion;
      copies->charm = unit->charm;
      copies->stun = unit->stun;
      copies->sleep = unit->sleep;
      copies->psySeal = unit->psySeal;
      copies->regenerate = unit->regenerate;
      copies->reflect = unit->reflect;
      copies->haunt = unit->haunt;
      copies->grimReaper = unit->grimReaper;
      copies->equipBonus[0] = unit->equipBonus[0];
      copies->equipBonus[1] = unit->equipBonus[1];
      copies->ready = unit->ready;
      copies->immobile = unit->immobile;
      copies->agilityTurns = unit->agilityTurns;
      copies->agilityStage = unit->agilityStage;
      copies->challenge = unit->challenge;
    }
  }
}

/// Restores the five units ScriptedBattle_SaveUnits snapshotted, each one
/// still in the battle and listed then: the first three's summon boosts, each
/// one's statuses, and the last two's HP and PP.
void ScriptedBattle_RestoreUnits(void) {
  s32 i;
  s32 j;
  struct Snapshot *snapshot = &gScriptedBattle_Snapshot;
  struct SnapshotCopy *copies = gScriptedBattle_SnapshotBytes;

  for (i = 0; i <= 4; i++, copies++) {
    struct Unit *unit = Unit_Get(gScriptedBattle_Units[i]);

    if (BattleUnit_IsListed(gScriptedBattle_Units[i]) &&
        snapshot->listed[i] != 0) {
      if (i <= 2) {
        for (j = 0; j < ELEMENT_COUNT; j++)
          unit->summonBoost[j] = snapshot->summonBoosts[i][j];
      }
      unit->curse = copies->curse;
      unit->poison = copies->poison;
      unit->attackTurns = copies->attackTurns;
      unit->attackStage = copies->attackStage;
      unit->defenseTurns = copies->defenseTurns;
      unit->defenseStage = copies->defenseStage;
      unit->resistanceTurns = copies->resistanceTurns;
      unit->resistanceStage = copies->resistanceStage;
      unit->delusion = copies->delusion;
      unit->confusion = copies->confusion;
      unit->charm = copies->charm;
      unit->stun = copies->stun;
      unit->sleep = copies->sleep;
      unit->psySeal = copies->psySeal;
      unit->regenerate = copies->regenerate;
      unit->reflect = copies->reflect;
      unit->haunt = copies->haunt;
      unit->grimReaper = copies->grimReaper;
      unit->equipBonus[0] = copies->equipBonus[0];
      unit->equipBonus[1] = copies->equipBonus[1];
      unit->ready = copies->ready;
      unit->immobile = copies->immobile;
      unit->agilityTurns = copies->agilityTurns;
      unit->agilityStage = copies->agilityStage;
      unit->challenge = copies->challenge;
      if (i > 2) {
        Unit_SetHpFar(unit, snapshot->vitals[i].hp);
        Unit_SetPpFar(unit, gScriptedBattle_Snapshot.vitals[i].pp);
      }
    }
  }
}

/// Works out a character's class, Psynergy and base elements again.
void Unit_UpdateClassFar(s32 owner);

/// From this parameter set on, an even set is loaded together with the odd one
/// after it.
#define DEBUG_PAIRED_SETS_FIRST 70

/// The last row of gDebug_ParameterSets; the scan reports a missing parameter
/// set past it.
#define DEBUG_PARAMETER_SET_LAST_ROW 440

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
  // Empty the party and the Djinn on standby; the parameter set's rows add
  // its party members back.
  Party_RemoveMemberFar(ISAAC);
  Party_RemoveMemberFar(GARET);
  Party_RemoveMemberFar(IVAN);
  Party_RemoveMemberFar(MIA);
  Party_RemoveMemberFar(JENNA);
  Party_RemoveMemberFar(FELIX);
  Party_RemoveMemberFar(SHEBA);
  Party_RemoveMemberFar(PIERS);
  Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY)->list.count = notFound;

  currentSet = 0;
  for (i = 0; i < ELEMENT_COUNT; i++)
    nextDjinn[i] = 0;

  // Parameter sets are runs of rows each ended by a DEBUG_PARAMETER_SET_END
  // row; skip to the run numbered set and apply its rows, and from
  // DEBUG_PAIRED_SETS_FIRST on the odd run after it too.
  for (row = 0;; row++) {
    unitId = gDebug_ParameterSets[row].owner;
    if ((u32)row > DEBUG_PARAMETER_SET_LAST_ROW) {
      notFound = 1;
      break;
    }
    if (unitId == DEBUG_PARAMETER_SET_END) {
      if (currentSet < DEBUG_PAIRED_SETS_FIRST) {
        if (currentSet == set)
          break;
      } else if (currentSet == (set | 1)) {
        break;
      }
      currentSet++;
      continue;
    }
    if (currentSet < set)
      continue;

    Party_AddMemberFar(unitId);
    Unit_LevelUpToFar(unitId, gDebug_ParameterSets[row].level);

    // Start from no Djinn and no abilities.
    member = Unit_Get(unitId);
    for (i = 0; i < ELEMENT_COUNT; i++) {
      member->djinnHeldCounts[i] = 0;
      member->djinnSetCounts[i] = 0;
      member->djinnHeld[i] = 0;
      member->djinnSet[i] = 0;
    }
    for (i = 0; i < PSYNERGY_SLOTS; i++)
      *&member->psynergy[i].encodedAction = 0;
    for (i = 0; i < 2; i++) {
      if (gDebug_ParameterSets[row].actions[i] != 0)
        Unit_LearnPsynergyFar(unitId, gDebug_ParameterSets[row].actions[i]);
    }

    // Djinn are numbered across the party, so no two party members get the
    // same: from this game's own first Djinni of each element, wrapping within
    // its eighteen.
    for (i = 0; i < ELEMENT_COUNT; i++) {
      for (djinnIndex = 0;
           djinnIndex < gDebug_ParameterSets[row].djinnCounts[i];
           djinnIndex++) {
        Djinn_GiveFar(unitId, i,
                      (nextDjinn[i] + DJINN_OWN_FIRST) % DJINNI_SPARE_FIRST);
        Djinn_SetFar(unitId, i,
                     (nextDjinn[i] + DJINN_OWN_FIRST) % DJINNI_SPARE_FIRST);
        nextDjinn[i]++;
      }
    }

    // Empty the inventory, removing slot 0 one time more than there are
    // slots, then add and equip the row's items.
    for (i = 0; i < (INVENTORY_SLOTS + 1); i++)
      Inventory_RemoveFar(unitId, 0);
    for (i = 0; i < 6; i++) {
      if (gDebug_ParameterSets[row].items[i] != 0)
        Inventory_EquipFar(
            unitId,
            Inventory_AddItemFar(unitId, gDebug_ParameterSets[row].items[i]));
    }
    Unit_UpdateClassFar(unitId);
  }
  return notFound;
}

/// Does nothing; the debug battle menu calls it when Start is pressed, as The
/// Broken Seal's calls Battle_DebugSelectNoOp on Select.
void Battle_DebugStartNoOp(void) {}

extern u8 gDebugTextEnabled;

/// The encounter the debug battle starts with (Moapa and two Knights), and The
/// Broken Seal's Djinn tutorial encounter, here an unused Mars Djinni's
/// battle: rows of formations.c.
#define DEBUG_FIRST_ENCOUNTER 69
#define DJINN_TUTORIAL_ENCOUNTER 28

/// Debug: the battle test menu, which never returns. It fights the first
/// encounter again and again with auto battle on, unless Down is held at the
/// start to open the chooser of encounter and parameter set.
void DebugBattle_RunMenu(void) {
  s32 startModeSet = 0;
  s32 down = gInput.held & KEY_DOWN;
  s32 encounter;
  s32 set;
  s32 loadedSet;
  struct InputState *input;
  struct MainRoutineState *mainState;
  u16 *backdropId;

  MainRoutineState_InitializeDefaultsFar();
restart:
  Window_LoadGraphics();
  DebugText_Clear();
  Task_ResetAll();
  Heap_Initialize();
  VramCache_Reset();
  encounter = DEBUG_FIRST_ENCOUNTER;
  GameFlag_Set(FLAG_AUTO_BATTLE);
  if (down) {
    set = 0;
    loadedSet = -1;
    GameFlag_Clear(FLAG_AUTO_BATTLE);
    input = &gInput;
    backdropId = (u16 *)&gMainRoutineState.battleBackground;
    for (;;) {
      GameFlag_Clear(FLAG_CHILDHOOD);
      wait(1);
      for (;;) {
        if (input->repeat & KEY_RIGHT)
          encounter++;
        if (input->repeat & KEY_LEFT)
          encounter--;
        if (input->repeat & KEY_UP)
          encounter -= DEBUG_LARGE_STEP;
        if (input->repeat & KEY_DOWN)
          encounter += DEBUG_LARGE_STEP;
        if (input->repeat & KEY_R)
          set++;
        if (input->repeat & KEY_L)
          set--;
        if (input->repeat & KEY_A)
          break;
        if (input->repeat & KEY_START)
          Battle_DebugStartNoOp();
        // DEAD: once startModeSet is set, setting it again changes nothing;
        // the ROM keeps the test.
        if ((input->repeat & KEY_B) || startModeSet)
          startModeSet = 1;
        wait(1);
      }
      if (set != loadedSet) {
        MainRoutineState_InitializeDefaultsFar();
        DebugParameterSet_Load(set);
        loadedSet = set;
      }
      // A fights; Down still held makes it a link battle.
      if (input->held & KEY_DOWN)
        GameFlag_Set(FLAG_LINK_BATTLE);
      Unit_CalculateFar(ISAAC);
      *backdropId = RESOURCE_COLOSSO_BACKDROP;
      mainState = &gMainRoutineState;
      if (encounter == DJINN_TUTORIAL_ENCOUNTER)
        GameFlag_Set(FLAG_DJINN_LESSON);
      GameFlag_Set(FLAG_DEBUG_BATTLE);
      if (startModeSet)
        mainState->battleType = BATTLE_TYPE_DEBUG;
      Battle_Run(encounter);
      // Rebuild what the battle replaced, then return to the chooser.
      gDebugTextEnabled = 1;
      Window_LoadGraphics();
      DebugText_Clear();
      Task_ResetAll();
      Heap_Initialize();
      VramCache_Reset();
    }
  }
  // Without Down: the same auto battle, over and over.
  GameFlag_Set(FLAG_DEBUG_BATTLE);
  Battle_Run(DEBUG_FIRST_ENCOUNTER);
  goto restart;
}

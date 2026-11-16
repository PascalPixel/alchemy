/// @file
/// Debug: the unit snapshot, the parameter sets and the battle test menu.
// Part of battle.c.

#include "battle_internal.h"
#include "flags.h"
#include "field/flag_call.h"
#include "gamepak/far_call/game.h"
#include "battle/battle.h"
#include "battle/presentation.h"
#include "system/system.h"
#include "resource_ids.h"

/// Unused: nothing refers to it. Runs random encounters one after another,
/// forever.
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
  struct Snapshot *snap = &gScriptedBattle_Snapshot;
  s32 i = 0;
  u8 *ids = gScriptedBattle_Units;
  struct SnapshotCopy *copies = gScriptedBattle_SnapshotBytes;

  for (; i <= 4; i++, copies++) {
    struct Unit *unit = Unit_Get(ids[i]);

    snap->listed[i] = 0;
    if (BattleUnit_IsListed(ids[i])) {
      snap->listed[i] = 1;
      if (i > 2) {
        snap->vitals[i].hp = unit->hp > 0 ? unit->hp : 1;
        {
          struct SnapshotVitals *pv;
          /* FAKEMATCH(pin): the PP base held in r0. */
          register struct SnapshotVitals *base asm("r0");
          u32 pp;

          /* FAKEMATCH(temporary): the PP value held in a local while pv is
           * built. */
          pp = (u16)unit->pp;
          base = gScriptedBattle_SnapshotVitals;
          // The address as an integer sum, i * 4 first: &base[i] puts the
          // base first in the add.
          pv = (struct SnapshotVitals *)(i * 4 + (s32)base);
          /* FAKEMATCH(dead): a store the tag loop's init overwrites. */
          if (i == 300)
            j = 0;
          pv->pp = pp;
        }
      }
      if (i <= 2) {
        for (j = 0; j < 4; j++)
          snap->summonBoosts[i][j] = unit->summonBoost[j];
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

/// Restores the five units ScriptedBattle_SaveUnits snapshotted: for each one
/// still in the battle party and listed, the first three's four tag bytes and
/// each one's 25 snapshot bytes, and the last two's HP and PP through
/// Unit_SetHpFar and Unit_SetPpFar.
void ScriptedBattle_RestoreUnits(void) {
  s32 i;
  s32 j;
  struct Snapshot *snap = &gScriptedBattle_Snapshot;
  struct SnapshotCopy *copies = gScriptedBattle_SnapshotBytes;

  for (i = 0; i <= 4; i++, copies++) {
    struct Unit *unit = Unit_Get(gScriptedBattle_Units[i]);

    if (BattleUnit_IsListed(gScriptedBattle_Units[i]) && snap->listed[i] != 0) {
      if (i <= 2) {
        for (j = 0; j < 4; j++)
          unit->summonBoost[j] = snap->summonBoosts[i][j];
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
        Unit_SetHpFar(unit, snap->vitals[i].hp);
        Unit_SetPpFar(unit, gScriptedBattle_Snapshot.vitals[i].pp);
      }
    }
  }
}

/// Works out a character's class, Psynergy and base elements again.
void Unit_UpdateClassFar(s32 owner);

/// Debug: replaces the party with parameter set number preset, each member at
/// its level with its Djinn, abilities and equipped items. Returns 1 when the
/// table ends before that set.
s32 DebugParameterSet_Load(s32 preset) {
  s32 nextDjinn[ELEMENT_COUNT];
  struct Unit *state;
  s32 owner;
  s32 done;
  s32 row;
  s32 j;
  s32 set;
  s32 i;

  done = 0;
  Party_RemoveMemberFar(0);
  Party_RemoveMemberFar(1);
  Party_RemoveMemberFar(2);
  Party_RemoveMemberFar(3);
  Party_RemoveMemberFar(JENNA);
  Party_RemoveMemberFar(FELIX);
  Party_RemoveMemberFar(SHEBA);
  Party_RemoveMemberFar(7);
  Djinn_GetRecoveryTableFar(0)->list.count = done;

  set = 0;
  for (i = 0; i < ELEMENT_COUNT; i++)
    nextDjinn[i] = 0;

  for (row = 0;; row++) {
    owner = gDebug_ParameterSets[row].owner;
    if ((u32)row > 440) {
      done = 1;
      break;
    }
    if (owner == -1) {
      if (set <= 69) {
        if (set == preset)
          break;
      } else if (set == (preset | 1)) {
        break;
      }
      set++;
      continue;
    }
    if (set < preset)
      continue;

    Party_AddMemberFar(owner);
    Unit_LevelUpToFar(owner, gDebug_ParameterSets[row].level);
    state = Unit_Get(owner);
    for (i = 0; i < ELEMENT_COUNT; i++) {
      state->djinnHeldCounts[i] = 0;
      state->djinnSetCounts[i] = 0;
      state->djinnHeld[i] = 0;
      state->djinnSet[i] = 0;
    }
    for (i = 0; i < PSYNERGY_SLOTS; i++)
      *&state->psynergy[i].encodedAction = 0;
    for (i = 0; i < 2; i++) {
      if (gDebug_ParameterSets[row].actions[i] != 0)
        Unit_LearnPsynergyFar(owner, gDebug_ParameterSets[row].actions[i]);
    }
    for (i = 0; i < ELEMENT_COUNT; i++) {
      for (j = 0; j < gDebug_ParameterSets[row].djinnCounts[i]; j++) {
        Djinn_GiveFar(owner, i, (nextDjinn[i] + 7) % 18);
        Djinn_SetFar(owner, i, (nextDjinn[i] + 7) % 18);
        nextDjinn[i]++;
      }
    }
    for (i = 0; i < (INVENTORY_SLOTS + 1); i++)
      Inventory_RemoveFar(owner, 0);
    for (i = 0; i < 6; i++) {
      if (gDebug_ParameterSets[row].items[i] != 0)
        Inventory_EquipFar(
            owner,
            Inventory_AddItemFar(owner, gDebug_ParameterSets[row].items[i]));
    }
    Unit_UpdateClassFar(owner);
  }
  return done;
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

/// Debug: the battle test menu, which never returns. It fights encounter 69
/// again and again with auto battle on, unless Down is held at the start to
/// open the chooser of encounter and parameter set.
void DebugBattle_RunMenu(void) {
  s32 startModeSet = 0;
  s32 down = gInput.held & KEY_DOWN;
  s32 encounter;
  s32 preset;
  s32 loadedPreset;
  struct InputState *input;
  struct MainRoutineWork *m;
  u16 *backdropId;

  MainRoutineWork_InitializeDefaultsFar();
restart:
  Window_LoadGraphics();
  DebugText_Clear();
  Task_ResetAll();
  Heap_Initialize();
  VramCache_Reset();
  encounter = DEBUG_FIRST_ENCOUNTER;
  GameFlag_Set(FLAG_AUTO_BATTLE);
  if (down) {
    preset = 0;
    loadedPreset = -1;
    GameFlag_Clear(FLAG_AUTO_BATTLE);
    input = &gInput;
    // Held for the loop's life: the backdrop id's value loads before its
    // address.
    backdropId = (u16 *)&gMainRoutineWork.battleBackground;
    for (;;) {
      GameFlag_Clear(32);
      wait(1);
      for (;;) {
        if (input->repeat & KEY_RIGHT)
          encounter++;
        if (input->repeat & KEY_LEFT)
          encounter--;
        if (input->repeat & KEY_UP)
          encounter -= 10;
        if (input->repeat & KEY_DOWN)
          encounter += 10;
        if (input->repeat & KEY_R)
          preset++;
        if (input->repeat & KEY_L)
          preset--;
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
      if (preset != loadedPreset) {
        MainRoutineWork_InitializeDefaultsFar();
        DebugParameterSet_Load(preset);
        loadedPreset = preset;
      }
      if (input->held & KEY_DOWN)
        GameFlag_Set(FLAG_LINK_BATTLE);
      Unit_CalculateFar(0);
      *backdropId = RESOURCE_COLOSSO_BACKDROP;
      m = &gMainRoutineWork;
      if (encounter == DJINN_TUTORIAL_ENCOUNTER)
        GameFlag_Set(FLAG_DJINN_LESSON);
      GameFlag_Set(FLAG_DEBUG_BATTLE);
      if (startModeSet)
        m->battleType = BATTLE_TYPE_DEBUG;
      Battle_Run(encounter);
      gDebugTextEnabled = 1;
      Window_LoadGraphics();
      DebugText_Clear();
      Task_ResetAll();
      Heap_Initialize();
      VramCache_Reset();
    }
  }
  GameFlag_Set(FLAG_DEBUG_BATTLE);
  Battle_Run(DEBUG_FIRST_ENCOUNTER);
  goto restart;
}

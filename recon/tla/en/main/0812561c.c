// Under -mthumb-slow-loads (26 October 2026): differ 8 of 498 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, each local pinned to each register,
// each call argument copied into its argument register (pinned) at the call
// or up to three statements before it, five minutes of permute; none reaches
// 0.
// Draft of Func_0812561c, The Broken Seal's BattleUnit_ProcessActionEnd
// (games/tbs/src/battle/battle/turn_end.c). This game: an enemy's Djinn are
// handled only in a two-sided battle; each element's Djinn count is capped at
// 4 and raises that element's level; the power-up effect is Func_08127308; a
// unit that keeps 1 HP (BattleUnit_KeepsOneHp) survives the curse and poison
// at 1 HP and is spared the Grim Reaper; and the stats are recalculated only
// while the unit still has its battle object.
// Near miss, differ 16 of 498 (2026-10-19, first draft). Left: instruction
// order only: the native clears battleContinues (movs r1, #0) before it
// loads the actor id; it stores the count's address to the stack after the
// -2 is built and before the -1; and three times it reloads the unit from
// the stack (ldr r2, [sp, #8]) before building a field offset (0x12c, 0x131,
// 0x141), where this draft builds the offset first. Tried: battleContinues
// cleared first (differ 26).
#include "types.h"
#include "battle.h"
#include "battle_party.h"
#include "battle_type.h"
#include "battle_work.h"
#include "iwram_call.h"
#include "mem.h"
#include "motobj.h"
#include "owner_value.h"
#include "ram_buffer.h"

struct PlacementList {
  struct DjinnRecoveryEntry entries[72];
  s32 count;
};

struct PlacementTable {
  u8 padding[8];
  struct PlacementList list;
};

// Messages 3285 to 3288, 3232, 3240, 3185, 3191 and 3188 in English; all
// but MsgGoesDown still need their names in the PO catalogs.
extern u8 MsgEarthPowerUp[];
extern u8 MsgCurseDamage[];
extern u8 MsgPoisonDamage[];
extern u8 MsgGoesDown[];
extern u8 MsgStrengthExhausted[];
extern u8 MsgReaperCalls[];

struct BattleUnit *Unit_Get(s32 unitId);
void Djinn_DeactivateFar(s32 unitId, s32 element, s32 index);
u32 BattleEventRuntime_Reset(void);
void BattleEventRuntime_SchedulePhase(s32 phase);
void BattleEventRuntime_WaitForReady(void);
void Sound_PlayFar(s32 cue);
void Object_SetMode(struct MotionObject *object, s32 mode);
void Func_08127308(s32 unitId, s32 element, s32 kind, s32 level);
s32 BattleUnit_KeepsOneHp(s32 unitId);

void Func_0812561c(struct BattlePlan *plan) {
  u32 unitId;
  struct BattleUnit *unit;
  s32 battleContinues;
  s32 counts[4];
  struct PlacementList *list;
  s32 strongestElement;
  s32 recoveryTurn;

  unitId = plan->actorId;
  battleContinues = 0;
  unit = Unit_Get(unitId);
  if (unitId <= 7 ||
      ((struct BattleWork *)Ram_HeapSlots->battleWork)->twoSided != 0) {
    list = &Djinn_GetRecoveryTableFar(unitId > 7)->list;
    for (strongestElement = 0; strongestElement < list->count;
         strongestElement++) {
      if (list->entries[strongestElement].unitId == unitId &&
          list->entries[strongestElement].turns == -1) {
        Djinn_DeactivateFar(unitId, list->entries[strongestElement].element,
                            list->entries[strongestElement].index);
      }
    }
  }
  if (BattleParty_ListLivingUnits(1, 0) != 0 &&
      BattleParty_ListLivingUnits(2, 0) != 0) {
    battleContinues = 1;
  }
  {
    s32 i;
    s32 owner;
    struct PlacementList *pending;

    pending = &Djinn_GetRecoveryTableFar(
                   unitId > 7 &&
                   ((struct BattleWork *)Ram_HeapSlots->battleWork)->twoSided !=
                       0)
                   ->list;
    for (i = 0; i < 4; i++) {
      counts[i] = 0;
    }
    for (;;) {
      owner = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].turns == -2) {
          owner = pending->entries[i].unitId;
          break;
        }
      }
      if (owner == -1) {
        break;
      }
      recoveryTurn = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns > recoveryTurn) {
          recoveryTurn = pending->entries[i].turns;
        }
      }
      recoveryTurn++;
      if (recoveryTurn <= 1) {
        recoveryTurn = 2;
      }
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns == -2) {
          pending->entries[i].turns = recoveryTurn;
          counts[pending->entries[i].element]++;
          recoveryTurn++;
        }
      }
    }
  }
  if (battleContinues != 0) {
    struct BattleUnit *before;
    s32 i;

    before = Heap_AllocEwram(sizeof(struct BattleUnit));
    Iwram_CopyWords(before, unit, sizeof(struct BattleUnit));
    for (i = 0; i <= 3; i++) {
      if (counts[i] > 4)
        counts[i] = 4;
      if (unit->elementLevels[i] < counts[i])
        unit->elementLevels[i] = counts[i];
    }
    Unit_CalculateFar(unitId);
    for (i = 0; i <= 3; i++) {
      s32 gain = unit->element[i].power - before->element[i].power;

      if (gain > 0) {
        BattleEventRuntime_Reset();
        BattleEventRuntime_SchedulePhase(25);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
        BattleEvent_Push(BATTLE_EVENT_NUMBER, gain);
        BattleEvent_Push(BATTLE_EVENT_SOUND, 175);
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgEarthPowerUp + i);
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
        Sound_PlayFar(212);
        Object_SetMode(BattleObject_Get(unitId)->object, 3);
        ObjectDispatch_SetAnimationSpeedFar(
            (struct DispatchObject *)BattleObject_Get(unitId)->object, 32);
        Func_08127308(unitId, i, 2, -1);
        BattleEventRuntime_WaitForReady();
      }
    }
    Heap_Release(before);
  }
  if (battleContinues != 0 && BattleObject_Get(unitId)->object != 0) {
    s8 *poison;
    u8 *reaper;

    BattleEventRuntime_Reset();
    if (plan->hauntDamage != 0) {
      BattleEvent_Push(BATTLE_EVENT_HIT, unitId);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, plan->hauntDamage);
      BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgCurseDamage);
      if (Unit_AddHpFar(unitId, -plan->hauntDamage) == 0) {
        if (BattleUnit_KeepsOneHp(unitId)) {
          Unit_AddHpFar(unitId, 1);
          BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
        } else {
          BattleEvent_Push(BATTLE_EVENT_DEFEAT, unitId);
          BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
          BattleEvent_Push(BATTLE_EVENT_MESSAGE, unitId <= 7 ? (u32)MsgGoesDown : (u32)MsgStrengthExhausted);
        }
      } else {
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
      }
    }
    BattleEvent_DispatchQueued();
    BattleEventRuntime_Reset();
    poison = &unit->poison;
    if (*poison != 0) {
      s32 damage = unit->maxHp * *poison / 10;
      struct BattleWork *state = Ram_HeapSlots->battleWork;

      BattleEvent_Push(BATTLE_EVENT_HIT, unitId);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, damage);
      BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgPoisonDamage);
      if (*poison != 0) {
        state->events.pendingCue = 134;
      } else {
        state->events.pendingCue = 133;
      }
      if (Unit_AddHpFar(unitId, -damage) == 0) {
        if (BattleUnit_KeepsOneHp(unitId)) {
          Unit_AddHpFar(unitId, 1);
          BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
        } else {
          BattleEvent_Push(BATTLE_EVENT_DEFEAT, unitId);
          BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
          BattleEvent_Push(BATTLE_EVENT_MESSAGE, unitId <= 7 ? (u32)MsgGoesDown : (u32)MsgStrengthExhausted);
        }
      } else {
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
      }
    }
    BattleEvent_DispatchQueued();
    BattleEventRuntime_Reset();
    reaper = &unit->grimReaper;
    if (*reaper != 0 && !BattleUnit_KeepsOneHp(unitId) && --*reaper == 0 &&
        Unit_AddHpFar(unitId, 0xc0000000) == 0) {
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgReaperCalls);
      BattleEvent_Push(BATTLE_EVENT_HIT, unitId);
      BattleEvent_Push(BATTLE_EVENT_DEFEAT, unitId);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      if (unitId <= 7) {
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgReaperCalls - 3);
      } else {
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgReaperCalls + 3);
      }
    }
    BattleEvent_DispatchQueued();
  }
  if (BattleObject_Get(unitId)->object != 0)
    Unit_CalculateFar(unitId);
}

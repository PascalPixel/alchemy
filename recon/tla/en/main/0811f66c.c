// Near miss, differ 10 of 776 (tla-en, 2026-10-30): instruction order only,
// five instructions one or two slots out of place, each a sched2 choice
// among instructions ready together. In the prologue the action's targetMode
// ldrb goes ahead of the count's mov r8; at the enemies' scan start the adds
// of the scan's base goes ahead of the targetIndex spill; in the sort's
// set-up adds r6, #56 goes ahead of the lsls, the str of the unitIds base
// ahead of the adds r0 that uses it, and the str of the inner pointer two
// slots late. sched2's dumps decide each by priority and its tie-breaks from
// -mthumb-slow-loads' load-after-store costs: the scan start and the sort's
// first two follow the ROM's order if the ldrsh (an extended load, which the
// model never makes wait) waited after its store and the reload of the
// unitIds base from [sp+16] did not wait after its store; no single such
// cost explains the prologue's or the inner pointer's. No source order
// reaches them. Setting candidateCount before the heap read and targetCount
// after it took this from 12 to 10 (the other orders of the three: 16); the
// scan's sides written as ☀️'s if/else (no gotos) compile the same. Then:
// the sort bound as innerIndex + 1 < targetCount, the slot form in the
// enemies' scan and selected = -1 at the switch, all far worse; four
// minutes of permute (2 jobs): no gain.
// Earlier, under -mthumb-slow-loads (26 October 2026), at 14: tried, one
// edit at a time, an empty asm barrier after each statement, each adjacent
// pair of statements swapped, each local pinned to each register, each call
// argument copied into its argument register (pinned) at the call or up to
// three statements before it; none reached 0. One barrier lowered it to 12.
// Draft: ☀️'s BattleAI_ChooseTarget (games/tbs/src/battle/battle/
// targeting.c), ported: picks the target of an enemy's ability (or a
// confused unit's) as a place code, among the units of the side it may
// target that it would affect, at random; a single-target attack by an enemy
// with a strategy favours the strongest. Returns -2 when the side has no one,
// -1 when the ability would affect no one. ⚓️ adds effects 70, 71 (healing),
// 73 (revival), 85 (as Paralyze) and 47 and 88 (only while the guard level is
// at most 1), and ability kinds 7 (always worth it) and 11 (PP healing).
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_work.h"
#include "battle_side.h"
#include "battle_system.h"
#include "ram_buffer.h"
#include "system.h"

#define TARGET_MODE_NONE 0
#define TARGET_MODE_FOES 1
#define TARGET_MODE_ALLIES 2
#define TARGET_MODE_SELF 4

#define PLACE_PARTY 0x100
#define PLACE_ENEMY 0x180

struct TargetEnemyDefinition {
  u8 unk00[43];
  s8 targetStrategy;
};

struct BattleUnit *Unit_Get(s32 unitId);
struct TargetEnemyDefinition *Enemy_GetDefinitionFar(s32 enemyId);

#define COUNT_PARTIAL_CURES(unit, count)                                       \
  {                                                                            \
    if ((unit)->delusion != 0)                                                 \
      (count)++;                                                               \
    if ((u8)(unit)->confusion != 0)                                            \
      (count)++;                                                               \
    if ((unit)->charm != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->sleep != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->psySeal != 0)                                                  \
      (count)++;                                                               \
    if ((unit)->grimReaper != 0)                                               \
      (count)++;                                                               \
  }

#define COUNT_ALL_CURES(unit, count)                                           \
  {                                                                            \
    COUNT_PARTIAL_CURES((unit), (count));                                      \
    if ((unit)->haunt != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->poison != 0)                                                   \
      (count)++;                                                               \
  }

#define COUNT_POSITIVE_MODIFIERS(unit, count)                                  \
  {                                                                            \
    if ((unit)->attackStage > 0)                                               \
      (count)++;                                                               \
    if ((unit)->defenseStage > 0)                                              \
      (count)++;                                                               \
    if ((unit)->resistanceStage > 0)                                           \
      (count)++;                                                               \
    if ((unit)->elementLevels[0] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[1] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[2] > 0)                                          \
      (count)++;                                                               \
    if ((unit)->elementLevels[3] > 0)                                          \
      (count)++;                                                               \
  }

#define CHECK_EFFECT_TARGET(unit, action, applies, kindIndex)                  \
  {                                                                            \
    (applies) = 0;                                                             \
    switch ((action)->effect) {                                                \
    case ABILITY_EFFECT_ATTACK_UP_2:                                           \
    case ABILITY_EFFECT_ATTACK_UP:                                             \
      if ((unit)->attackStage + 1 <= 4)                                        \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_ATTACK_DOWN_2:                                         \
    case ABILITY_EFFECT_ATTACK_DOWN:                                           \
      if ((unit)->attackStage - 1 >= -4)                                       \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_DEFENSE_UP_2:                                          \
    case ABILITY_EFFECT_DEFENSE_UP:                                            \
      if ((unit)->defenseStage + 1 <= 4)                                       \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_DEFENSE_DOWN_2:                                        \
    case ABILITY_EFFECT_DEFENSE_DOWN:                                          \
      if ((unit)->defenseStage - 1 >= -4)                                      \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_RESISTANCE_UP_2:                                       \
    case ABILITY_EFFECT_RESISTANCE_UP:                                         \
      if ((unit)->resistanceStage + 1 <= 4)                                    \
        (applies) = 1;                                                         \
      if ((unit)->resistanceTurns == 1)                                        \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_RESISTANCE_DOWN_2:                                     \
    case ABILITY_EFFECT_RESISTANCE_DOWN:                                       \
      if ((unit)->resistanceStage - 1 >= -4)                                   \
        (applies) = 1;                                                         \
      if ((unit)->resistanceTurns == 1)                                        \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_CURE_POISON:                                           \
      if ((unit)->poison != 0)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_RESTORE:                                               \
      COUNT_PARTIAL_CURES((unit), (applies));                                  \
      break;                                                                   \
    case ABILITY_EFFECT_HEAL_HP_60:                                            \
    case ABILITY_EFFECT_HEAL_HP_30:                                            \
    case 70:                                                                   \
    case 71:                                                                   \
      if ((unit)->hp < (unit)->maxHp)                                          \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_BREAK:                                                 \
      COUNT_POSITIVE_MODIFIERS((unit), (applies));                             \
      break;                                                                   \
    case ABILITY_EFFECT_CURE_ALL:                                              \
      COUNT_ALL_CURES((unit), (applies));                                      \
      break;                                                                   \
    case ABILITY_EFFECT_CURSE:                                                 \
      if ((unit)->grimReaper == 0)                                             \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_POISON:                                                \
      if ((unit)->poison == 0)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_DEADLY_POISON:                                         \
      if ((unit)->poison <= 1)                                                 \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_STUN:                                                  \
    case 85:                                                                   \
      if ((unit)->stun == 0)                                                   \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_SLEEP:                                                 \
      if ((unit)->sleep == 0)                                                  \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_HAUNT:                                                 \
      if ((unit)->haunt == 0)                                                  \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_REVIVE:                                                \
    case ABILITY_EFFECT_REVIVE_HALF:                                           \
    case ABILITY_EFFECT_REVIVE_80:                                             \
    case 73:                                                                   \
      if ((unit)->hp == 0)                                                     \
        (applies) = 100;                                                       \
      break;                                                                   \
    case ABILITY_EFFECT_TOTAL_DEFENSE:                                         \
    case 88:                                                                   \
      if ((unit)->guardLevel <= 1)                                             \
        (applies) = 1;                                                         \
      break;                                                                   \
    case 0:                                                                    \
    case 1:                                                                    \
    case 2:                                                                    \
      break;                                                                   \
    default:                                                                   \
      (applies) = 1;                                                           \
      break;                                                                   \
    }                                                                          \
    if ((unit)->hp == 0 && Battle_IsReviveEffectFar((action)->effect) == 0)    \
      (applies) = 0;                                                           \
    if ((applies) == 0) {                                                      \
      (kindIndex) = ((action)->flags & ABILITY_KIND_MASK) - 1;                 \
      switch (kindIndex) {                                                     \
      case ABILITY_KIND_ATTACK - 1:                                            \
      case ABILITY_KIND_ATTACK_MULTIPLIED - 1:                                 \
        if ((unit)->hp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_DAMAGE - 1:                                            \
      case ABILITY_KIND_UNLEASHED_DAMAGE - 1:                                  \
      case ABILITY_KIND_SUMMON - 1:                                            \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_PP_DAMAGE - 1:                                         \
        if ((unit)->pp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_HEAL_HP - 1:                                           \
        if ((unit)->hp != 0 && (unit)->hp < (unit)->maxHp)                     \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_HEAL_PP - 1:                                           \
        if ((unit)->pp < (unit)->maxPp)                                        \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_KIND_EFFECT - 1:                                            \
      case 9 - 1:                                                              \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      case 7 - 1:                                                              \
        (applies)++;                                                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }

#define STRATEGY_MOST_HP 0
#define STRATEGY_MOST_MAX_HP 1
#define STRATEGY_RANDOM 2

s32 Func_0811f66c(s32 actorId, struct BattleAct *action) {
  u16 orderPositions[6];
  s32 unitIds[6];
  s32 targetPositions[6];
  struct BattleWork *turnOrder;
  struct BattleUnit *unit;
  struct BattleUnit *nextUnit;
  struct BattleUnit *thisUnit;
  s32 candidateCount;
  s32 targetIndex;
  s32 targetCount;
  s32 innerIndex;
  s32 selected;
  s32 unitId;
  s32 value;
  s32 nextValue;
  s32 temp;
  s32 ownSide;
  s32 kindIndex;
  s32 applies;
  u32 roll;
  s16 *slot;

  candidateCount = 0;
  turnOrder = Ram_HeapSlots->battleWork;
  targetCount = 0;

  if (action->targetMode != TARGET_MODE_NONE) {
    ownSide = 0;
    if (action->targetMode == TARGET_MODE_ALLIES ||
        action->targetMode == TARGET_MODE_SELF)
      ownSide = 1;

    if (((actorId < 0 || actorId > 7) && ownSide == 0) ||
        (actorId >= 0 && actorId <= 7 && ownSide != 0)) {
      targetIndex = 0;
      while (turnOrder->partyUnits[targetIndex] != BATTLE_UNIT_LIST_END) {
        slot = &turnOrder->partyUnits[targetIndex];
        unitId = *slot;
        if (unitId != BATTLE_UNIT_REMOVED) {
          if (action->targetMode != TARGET_MODE_SELF || unitId == actorId) {
            unitIds[candidateCount] = unitId;
            orderPositions[candidateCount] = PLACE_PARTY;
            orderPositions[candidateCount] |= targetIndex;
            candidateCount++;
          }
        }
        targetIndex++;
      }
    } else {
      targetIndex = 0;
      while (turnOrder->enemyIds[targetIndex] != BATTLE_UNIT_LIST_END) {
        unitId = turnOrder->enemyIds[targetIndex];
        if (unitId != BATTLE_UNIT_REMOVED) {
          if (action->targetMode != TARGET_MODE_SELF || unitId == actorId) {
            unitIds[candidateCount] = unitId;
            orderPositions[candidateCount] = PLACE_ENEMY;
            orderPositions[candidateCount] |= targetIndex;
            candidateCount++;
          }
        }
        targetIndex++;
      }
    }
  }

  if (candidateCount == 0)
    return -2;

  for (targetIndex = 0; targetIndex < candidateCount; targetIndex++) {
    unit = Unit_Get(unitIds[targetIndex]);
    CHECK_EFFECT_TARGET(unit, action, applies, kindIndex);
    if (applies != 0) {
      unitIds[targetCount] = unitIds[targetIndex];
      targetPositions[targetCount] = orderPositions[targetIndex];
      targetCount++;
    }
  }

  if (targetCount == 0)
    return -1;

  if (action->targetMode == TARGET_MODE_FOES && action->range == 1 &&
      Enemy_GetDefinitionFar(Unit_Get(actorId)->enemyId)->targetStrategy !=
          STRATEGY_RANDOM &&
      (u32)((action->flags & ABILITY_KIND_MASK) - ABILITY_KIND_ATTACK) <= 2) {
    selected = -1;
    for (targetIndex = 0; targetIndex < targetCount; targetIndex++) {
      for (innerIndex = targetIndex; innerIndex < targetCount - 1;
           innerIndex++) {
        thisUnit = Unit_Get(unitIds[innerIndex]);
        nextUnit = Unit_Get(unitIds[innerIndex + 1]);
        if (Enemy_GetDefinitionFar(Unit_Get(actorId)->enemyId)
                ->targetStrategy == STRATEGY_MOST_HP) {
          value = thisUnit->hp;
          nextValue = nextUnit->hp;
        } else {
          value = thisUnit->maxHp;
          nextValue = nextUnit->maxHp;
        }
        if (value < nextValue) {
          temp = unitIds[innerIndex];
          unitIds[innerIndex] = unitIds[innerIndex + 1];
          unitIds[innerIndex + 1] = temp;
          temp = targetPositions[innerIndex];
          targetPositions[innerIndex] = targetPositions[innerIndex + 1];
          targetPositions[innerIndex + 1] = temp;
        }
      }
    }

    switch (targetCount) {
    case 1:
      selected = 0;
      break;
    case 2:
      roll = (11 * Random16()) >> 16;
      selected = 0;
      if (roll > 5)
        selected = 1;
      break;
    case 3:
      selected = (15 * Random16()) >> 16;
      if (selected <= 5)
        selected = 0;
      else if (selected <= 10)
        selected = 1;
      else
        selected = 2;
      break;
    case 4:
      selected = (18 * Random16()) >> 16;
      if (selected <= 5)
        selected = 0;
      else if (selected <= 10)
        selected = 1;
      else if (selected <= 14)
        selected = 2;
      else
        selected = 3;
      break;
    }

    if (selected >= 0)
      return targetPositions[selected];
  }
  return targetPositions[(Random16() * targetCount) >> 16];
}

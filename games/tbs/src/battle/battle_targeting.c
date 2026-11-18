/// @file
/// The targets an enemy's or a confused unit's ability picks.
// Part of battle.c.

#include "battle_internal.h"

#ifndef UBFIX
/// BattleWork's unit lists, read as one array of halfwords.
struct BattleWorkUnitListView {
  s16 items[64]; ///< BattleWork from its start; each list at its offset / 2
};
#endif

/// Picks a random place: a party member still standing with requireLivingUnit,
/// otherwise any enemy place in use. Returns 0 when there is none.
s32 BattleAI_ChooseRandomTarget(s32 requireLivingUnit) {
  u16 positions[6];
  s32 count;

  // UB: the view treats distinct BattleWork members as one array.
#ifdef UBFIX
  s16 *unitIds;
  s32 unitId;
  s32 place;
  s32 sideTag;

  count = 0;
  unitIds = requireLivingUnit ? gBattleWork->partyUnits : gBattleWork->enemyIds;
  sideTag = requireLivingUnit ? PLACE_PARTY : PLACE_ENEMY;
  for (place = 0; unitIds[place] != BATTLE_UNIT_LIST_END; place++) {
    unitId = unitIds[place];
    if (unitId == BATTLE_UNIT_REMOVED)
      continue;
    if (requireLivingUnit && Unit_GetFar(unitId)->hp == 0)
      continue;
    positions[count++] = place | sideTag;
  }
#else
  struct BattleWorkUnitListView *order;
  s16 *entry;
  u16 *cursor;
  s32 value;
  s32 index;
  s32 slot;
  s32 tail;
  s32 offset;

  count = 0;
  order = (struct BattleWorkUnitListView *)gBattleWork;

  // The party's standing places, else the enemies' places in use.
  if (requireLivingUnit != 0) {
    index = 0;
    slot = offsetof(struct BattleWork, partyUnits) / sizeof(s16);
    if (order->items[slot] != BATTLE_UNIT_LIST_END) {
      entry = order->items;
      // The walk leaves by a break, so its loop's end note lies just before
      // the else branch.
      for (;;) {
        value = entry[slot];
        if (value != BATTLE_UNIT_REMOVED) {
          if (Unit_GetFar(value)->hp != 0) {
            positions[count] = index | PLACE_PARTY;
            count++;
          }
        }
        slot++;
        index++;
        if (entry[slot] == BATTLE_UNIT_LIST_END)
          break;
      }
    }
  } else {
    index = 0;
    slot = offsetof(struct BattleWork, enemyIds) / sizeof(s16) - 1;
    tail = offsetof(struct BattleWork, enemyIds) / sizeof(s16) - 1;
    offset = tail * 2;
    entry = (order->items + 1);
    if (*(s16 *)((char *)entry + offset) != BATTLE_UNIT_LIST_END) {
      cursor = entry;
      do {
        if ((s16)cursor[slot] != BATTLE_UNIT_REMOVED) {
          positions[count] = index | PLACE_ENEMY;
          count++;
        }
        slot++;
        tail++;
        index++;
      } while (entry[tail] != BATTLE_UNIT_LIST_END);
    }
  }
#endif

  if (count == 0)
    return 0;
  // A 16-bit roll times the count, over 65536: an index below count.
  return positions[Random16_Below(count)];
}

// BUG: Restore cures stun and never confusion or charm, as its description
// says ("Remove sleep, stun, and delusion.") and its cure does, but the count
// takes confusion and charm and not stun, so Cerebus and the Hydros Statue
// pass over an ally who is only stunned.
#ifdef BUGFIX
#define COUNT_STUN(unit, count)                                                \
  if ((unit)->stun != 0)                                                       \
    (count)++;
#else
#define COUNT_STUN(unit, count)                                                \
  if ((u8)(unit)->confusion != 0)                                              \
    (count)++;                                                                 \
  if ((unit)->charm != 0)                                                      \
    (count)++;
#endif

/// Counts the statuses of unit that Restore cures: delusion, stun (COUNT_STUN),
/// sleep, the Psynergy seal and the Grim Reaper.
#define COUNT_PARTIAL_CURES(unit, count)                                       \
  {                                                                            \
    if ((unit)->delusion != 0)                                                 \
      (count)++;                                                               \
    COUNT_STUN((unit), (count))                                                \
    if ((unit)->sleep != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->psySeal != 0)                                                  \
      (count)++;                                                               \
    if ((unit)->grimReaper != 0)                                               \
      (count)++;                                                               \
  }

/// Counts the statuses Tonic cures: those, the evil spirit and poison. No enemy
/// has Tonic.
#define COUNT_ALL_CURES(unit, count)                                           \
  {                                                                            \
    COUNT_PARTIAL_CURES((unit), (count));                                      \
    if ((unit)->haunt != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->poison != 0)                                                   \
      (count)++;                                                               \
  }

/// Counts what Break would undo on unit; a raised Agility is not counted.
#define COUNT_POSITIVE_MODIFIERS(unit, count)                                  \
  {                                                                            \
    if ((unit)->attackStage > 0)                                               \
      (count)++;                                                               \
    if ((unit)->defenseStage > 0)                                              \
      (count)++;                                                               \
    if ((unit)->resistanceStage > 0)                                           \
      (count)++;                                                               \
    if ((unit)->summonBoost[ELEMENT_VENUS] > 0)                                \
      (count)++;                                                               \
    if ((unit)->summonBoost[ELEMENT_MERCURY] > 0)                              \
      (count)++;                                                               \
    if ((unit)->summonBoost[ELEMENT_MARS] > 0)                                 \
      (count)++;                                                               \
    if ((unit)->summonBoost[ELEMENT_JUPITER] > 0)                              \
      (count)++;                                                               \
  }

/// How worthwhile an enemy's ability is against unit, 0 for pointless: a stat
/// change that can still move, a cure or heal it needs, a status it lacks, a
/// downed unit to revive.
#define CHECK_EFFECT_TARGET(unit, action, applies, kindIndex)                  \
  {                                                                            \
    (applies) = 0;                                                             \
    switch ((action)->effect) {                                                \
    case ABILITY_EFFECT_ATTACK_UP_SHARPLY:                                     \
    case ABILITY_EFFECT_ATTACK_UP:                                             \
      if ((unit)->attackStage + 1 <= 4)                                        \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_ATTACK_DOWN_SHARPLY:                                   \
    case ABILITY_EFFECT_ATTACK_DOWN:                                           \
      if ((unit)->attackStage - 1 >= -4)                                       \
        (applies) = 1;                                                         \
      if ((unit)->attackTurns == 1)                                            \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_DEFENSE_UP_SHARPLY:                                    \
    case ABILITY_EFFECT_DEFENSE_UP:                                            \
      if ((unit)->defenseStage + 1 <= 4)                                       \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_DEFENSE_DOWN_SHARPLY:                                  \
    case ABILITY_EFFECT_DEFENSE_DOWN:                                          \
      if ((unit)->defenseStage - 1 >= -4)                                      \
        (applies) = 1;                                                         \
      if ((unit)->defenseTurns == 1)                                           \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_RESISTANCE_UP_SHARPLY:                                 \
    case ABILITY_EFFECT_RESISTANCE_UP:                                         \
      if ((unit)->resistanceStage + 1 <= 4)                                    \
        (applies) = 1;                                                         \
      if ((unit)->resistanceTurns == 1)                                        \
        (applies)++;                                                           \
      break;                                                                   \
    case ABILITY_EFFECT_RESISTANCE_DOWN_SHARPLY:                               \
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
    case ABILITY_EFFECT_FIZZ:                                                  \
    case ABILITY_EFFECT_SPRITZ:                                                \
      if ((unit)->hp < (unit)->hpMax)                                          \
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
    case ABILITY_EFFECT_VENOM:                                                 \
      if ((unit)->poison <= POISON_NORMAL)                                     \
        (applies) = 1;                                                         \
      break;                                                                   \
    case ABILITY_EFFECT_STUN:                                                  \
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
    case ABILITY_EFFECT_DEW:                                                   \
      if ((unit)->hp == 0)                                                     \
        (applies) = 100;                                                       \
      break;                                                                   \
    case ABILITY_EFFECT_NONE:                                                  \
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
      (kindIndex) = ((action)->flags & ABILITY_FORMULA_MASK) - 1;              \
      switch (kindIndex) {                                                     \
      case ABILITY_FORMULA_ATTACK - 1:                                         \
      case ABILITY_FORMULA_ATTACK_MULTIPLIED - 1:                              \
        if ((unit)->hp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_FORMULA_SUMMON - 1:                                         \
      case ABILITY_FORMULA_NO_POWER - 1:                                       \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_FORMULA_PP_DAMAGE - 1:                                      \
        if ((unit)->pp != 0)                                                   \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_FORMULA_HEAL_HP - 1:                                        \
        if ((unit)->hp != 0 && (unit)->hp < (unit)->hpMax)                     \
          (applies)++;                                                         \
        break;                                                                 \
      case ABILITY_FORMULA_EFFECT - 1:                                         \
      case ABILITY_FORMULA_DAMAGE - 1:                                         \
      case ABILITY_FORMULA_ENEMY_DAMAGE - 1:                                   \
        if ((action)->power != 0 && (unit)->hp != 0)                           \
          (applies)++;                                                         \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }

/// Picks the target of an enemy's ability, or a confused unit's, at random
/// among those it would affect; a single-target attack by an enemy with a
/// strategy favours the strongest.
s32 BattleAI_ChooseTarget(s32 userId, struct AbilityDefinition *action) {
  u16 orderPositions[6];
  s32 unitIds[6];
  s32 targetPositions[6];
  struct BattleWork *turnOrder;
  struct Unit *unit;
  struct Unit *nextUnit;
  struct Unit *thisUnit;
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

  turnOrder = gBattleWork;
  targetCount = 0;
  candidateCount = 0;

  // The candidates: every unit on the side the ability may target.
  if (action->targetMode != ABILITY_TARGET_ONE) {
    ownSide = 0;
    if (action->targetMode == ABILITY_TARGET_ALLIES ||
        action->targetMode == ABILITY_TARGET_SELF)
      ownSide = 1;

    if (((userId < 0 || userId > ALLY_MAX) && ownSide == 0) ||
        (userId >= 0 && userId <= ALLY_MAX && ownSide != 0)) {
      targetIndex = 0;
      while (turnOrder->partyUnits[targetIndex] != BATTLE_UNIT_LIST_END) {
        slot = &turnOrder->partyUnits[targetIndex];
        unitId = *slot;
        if (unitId != BATTLE_UNIT_REMOVED) {
          if (action->targetMode != ABILITY_TARGET_SELF || unitId == userId) {
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
          if (action->targetMode != ABILITY_TARGET_SELF || unitId == userId) {
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
    return TARGET_PICK_SIDE_EMPTY;

  // Keep the candidates the ability would affect, in place.
  for (targetIndex = 0; targetIndex < candidateCount; targetIndex++) {
    unit = Unit_GetFar(unitIds[targetIndex]);
    CHECK_EFFECT_TARGET(unit, action, applies, kindIndex);
    if (applies != 0) {
      unitIds[targetCount] = unitIds[targetIndex];
      targetPositions[targetCount] = orderPositions[targetIndex];
      targetCount++;
    }
  }

  if (targetCount == 0)
    return TARGET_PICK_NONE;

  if (action->targetMode == ABILITY_TARGET_FOES && action->range == 1 &&
      Enemy_GetDefinitionFar(Unit_GetFar(userId)->id)->targetStrategy !=
          TARGET_STRATEGY_RANDOM &&
      (u32)((action->flags & ABILITY_FORMULA_MASK) - ABILITY_FORMULA_ATTACK) <=
          ABILITY_FORMULA_DAMAGE - ABILITY_FORMULA_ATTACK) {
    // Sort the targets by HP, the most first.
    // BUG: each pass starts one place later, so HP 1, 2 and 3 end as 2, 3
    // and 1.
    selected = -1;
    for (targetIndex = 0; targetIndex < targetCount; targetIndex++) {
      for (innerIndex = targetIndex; innerIndex < targetCount - 1;
           innerIndex++) {
        thisUnit = Unit_GetFar(unitIds[innerIndex]);
        nextUnit = Unit_GetFar(unitIds[innerIndex + 1]);
        if (Enemy_GetDefinitionFar(Unit_GetFar(userId)->id)->targetStrategy ==
            TARGET_STRATEGY_MOST_HP) {
          value = thisUnit->hp;
          nextValue = nextUnit->hp;
        } else {
          value = thisUnit->hpMax;
          nextValue = nextUnit->hpMax;
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

    // Weighted toward the front: of a roll's outcomes the first target takes
    // 6, the second 5, the third 4 and the fourth 3; five or more fall
    // through to an even roll.
    switch (targetCount) {
    case 1:
      selected = 0;
      break;
    case 2:
      roll = Random16_Below(6 + 5);
      selected = 0;
      if (roll >= 6)
        selected = 1;
      break;
    case 3:
      selected = Random16_Below(6 + 5 + 4);
      if (selected < 6)
        selected = 0;
      else if (selected < 6 + 5)
        selected = 1;
      else
        selected = 2;
      break;
    case 4:
      selected = Random16_Below(6 + 5 + 4 + 3);
      if (selected < 6)
        selected = 0;
      else if (selected < 6 + 5)
        selected = 1;
      else if (selected < 6 + 5 + 4)
        selected = 2;
      else
        selected = 3;
      break;
    }

    if (selected >= 0)
      return targetPositions[selected];
  }
  return targetPositions[Random16_Below(targetCount)];
}

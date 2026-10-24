/// @file
/// The targets an enemy's or a confused unit's ability picks.
// Part of battle.c.

#include "../battle_internal.h"

// Native-only view retained for the measured target-picker form below.
#ifndef UBFIX
/// BattleWork's unit lists, read as one array of halfwords.
struct BattleWorkUnitListView {
  s16 items[64]; ///< BattleWork from its start; each list at its offset / 2
};
#endif

/// The list place codes the target pickers return: the party's places
/// 0x100 | place, the enemies' 0x180 | place (Battle_FindTaggedSlotByValue).
#define PLACE_PARTY 0x100
#define PLACE_ENEMY 0x180

/// Picks a random place: with requireLivingUnit, a party member still
/// standing; otherwise any enemy place in use. Returns its place code, or 0
/// when there is none.
/// Called by BattleAI_ChooseAction when BattleAI_ChooseTarget
/// finds no one listed on the side (TARGET_PICK_SIDE_EMPTY), with
/// requireLivingUnit set for a party actor. Takes one Random16 roll when any
/// position qualifies.
s32 BattleAI_ChooseRandomTarget(s32 requireLivingUnit) {

  u16 positions[6];
  s32 count;

  // UB: the native view treats distinct BattleWork members as one array.
  // Walking the actual unit list changes EN from 0xc2 to 0xa8 bytes.
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

  // The party's places whose unit is still standing; then, in the else
  // branch, the enemies' places in use, whether standing or not. Each walk
  // indexes order from its list's offset in BattleWork, in halfwords.
  if (requireLivingUnit != 0) {
    /* FAKEMATCH(one-pass): a loop left by its closing break; without it, 14
     * of 89 instructions differ in tbs-en. */
    for (;;) {
      index = 0;
      slot = (u32)&((struct BattleWork *)0)->partyUnits / sizeof(s16);
      if (order->items[slot] != BATTLE_UNIT_LIST_END) {
        entry = order->items;
        do {
          value = entry[slot];
          if (value != BATTLE_UNIT_REMOVED) {
            if (Unit_GetFar(value)->hp != 0) {
              positions[count] = index | PLACE_PARTY;
              count++;
            }
          }
          slot++;
          index++;
        } while (entry[slot] != BATTLE_UNIT_LIST_END);
      }
      break;
    }
  } else {
    index = 0;
    slot = (u32)&((struct BattleWork *)0)->enemyIds / sizeof(s16) - 1;
    tail = (u32)&((struct BattleWork *)0)->enemyIds / sizeof(s16) - 1;
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
  return positions[(Random16() * count) >> 16];
}

// XXX: likely wrong, because Restore and Tonic clear stun and never confusion
// or charm (BattleAbility_ApplyToTarget), as Restore's description and the
// party's target window (BattleTarget_RunSelection) have it; only
// Battle_RollEffectSuccessFar counts confusion and charm too. Cerebus and the
// Hydros Statue, the enemies with Restore, pass over an ally who is only
// stunned, and may spend it on one who is only confused or charmed.
/// Counts the statuses of unit that Restore is taken to cure: delusion,
/// confusion, charm, sleep, the Psynergy seal and the Grim Reaper.
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

/// Counts the statuses Tonic is taken to cure: those, the evil spirit and
/// poison, with the same mismatch. No enemy has Tonic.
#define COUNT_ALL_CURES(unit, count)                                           \
  {                                                                            \
    COUNT_PARTIAL_CURES((unit), (count));                                      \
    if ((unit)->haunt != 0)                                                    \
      (count)++;                                                               \
    if ((unit)->poison != 0)                                                   \
      (count)++;                                                               \
  }

/// Counts what Break would undo on unit: raised Attack, Defense and Resistance
/// stages and a summon's boost in each element. A raised Agility, which Break
/// also undoes, isn't counted.
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

/// How worthwhile an enemy's ability is against unit, in applies (0 for
/// pointless): a stat change that can still move (one more when it is about
/// to wear off), a cure or a heal the unit needs, a status it lacks, a
/// revival of a downed unit (100). Failing its added effect, the ability's
/// damage or healing must have something to work on. kindIndex receives the
/// ability kind less one (enum AbilityFormula). The two unnamed effects below
/// ABILITY_EFFECT_CURE_POISON, which no row has, count as nothing.
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
    case ABILITY_EFFECT_REVIVE_80:                                             \
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

/// Picks the target of an enemy's ability (or a confused unit's), as a place
/// code: among the units of the side it may target, those it would affect
/// (CHECK_EFFECT_TARGET), at random; a single-target attack by an enemy with
/// a strategy favours the strongest, the first of the sorted targets 6 times
/// in 11, 15 or 18, the next 5 times, and so on. Returns
/// TARGET_PICK_SIDE_EMPTY when the side has no one, TARGET_PICK_NONE when the
/// ability would affect no one.
/// Called by BattleAI_ChooseAction for an ability aimed at allies,
/// the user or foes. Takes Random16 rolls to choose among eligible targets.
s32 BattleAI_ChooseTarget(s32 actorId, struct AbilityDefinition *action) {
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

  // The candidates: every unit listed on the side the ability may target,
  // downed or not (only the user, for ABILITY_TARGET_SELF), with its place
  // code. Allies of a party unit and foes of an enemy are the party.
  if (action->targetMode != ABILITY_TARGET_ONE) {
    ownSide = 0;
    if (action->targetMode == ABILITY_TARGET_ALLIES ||
        action->targetMode == ABILITY_TARGET_SELF)
      ownSide = 1;

    if (((actorId < 0 || actorId > ALLY_MAX) && ownSide == 0) ||
        (actorId >= 0 && actorId <= ALLY_MAX && ownSide != 0)) {
      targetIndex = 0;
      while (turnOrder->partyUnits[targetIndex] != BATTLE_UNIT_LIST_END) {
        slot = &turnOrder->partyUnits[targetIndex];
        unitId = *slot;
        if (unitId != BATTLE_UNIT_REMOVED) {
          if (action->targetMode != ABILITY_TARGET_SELF || unitId == actorId) {
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
          if (action->targetMode != ABILITY_TARGET_SELF || unitId == actorId) {
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
      ((struct EnemyDefinition *)Enemy_GetDefinitionFar(
           Unit_GetFar(actorId)->id))
              ->targetStrategy != TARGET_STRATEGY_RANDOM &&
      (u32)((action->flags & ABILITY_FORMULA_MASK) - ABILITY_FORMULA_ATTACK) <=
          ABILITY_FORMULA_DAMAGE - ABILITY_FORMULA_ATTACK) {
    // A single-target ability of ABILITY_FORMULA_ATTACK to
    // ABILITY_FORMULA_DAMAGE by an enemy with a strategy: sort the targets by
    // HP left or by maximum HP, the most first. Each pass of the exchange sort
    // starts one place later, so the order can come out wrong: HP 1, 2 and 3
    // end as 2, 3 and 1.
    selected = -1;
    for (targetIndex = 0; targetIndex < targetCount; targetIndex++) {
      for (innerIndex = targetIndex; innerIndex < targetCount - 1;
           innerIndex++) {
        thisUnit = Unit_GetFar(unitIds[innerIndex]);
        nextUnit = Unit_GetFar(unitIds[innerIndex + 1]);
        if (((struct EnemyDefinition *)Enemy_GetDefinitionFar(
                 Unit_GetFar(actorId)->id))
                ->targetStrategy == TARGET_STRATEGY_MOST_HP) {
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

    // Weighted toward the front: two targets 6 and 5 in 11, three 6, 5 and 4
    // in 15, four 6, 5, 4 and 3 in 18. Five or more fall through to the even
    // roll below.
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

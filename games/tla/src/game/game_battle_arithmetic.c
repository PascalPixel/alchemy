/// @file
/// Battle arithmetic: element multipliers, random rolls and success rates.
// Part of game.c.

#include "game_internal.h"

/// Returns the multiplier, 256 for 1x, for an element difference: 256 plus 1.28
/// a point, the difference held to -200 to 200, its effect half with `halve`.
s32 Battle_GetElementMultiplier(s32 difference, s32 halve) {
  s32 minimumDifference;
  s32 maximumDifference;
  s32 pointCount;
  s32 pointIndex;
  s32 multiplier;
  s32 differenceSpan;
  s32 multiplierSpan;

  maximumDifference = gElement_PowerResistCurve[0][0];
  minimumDifference = gElement_PowerResistCurve[4][0];
  pointCount = 5;
  if (difference > maximumDifference)
    difference = maximumDifference;
  else if (difference < minimumDifference)
    difference = minimumDifference;
  for (pointIndex = 0; pointIndex < pointCount; pointIndex++) {
    if (difference > gElement_PowerResistCurve[pointIndex][0])
      break;
  }
  if (pointIndex == pointCount) {
    multiplier = gElement_PowerResistCurve[pointIndex - 1][1];
  } else {
    differenceSpan = gElement_PowerResistCurve[pointIndex - 1][0] -
                     gElement_PowerResistCurve[pointIndex][0];
    multiplierSpan = gElement_PowerResistCurve[pointIndex - 1][1] -
                     gElement_PowerResistCurve[pointIndex][1];
    multiplier = (difference - gElement_PowerResistCurve[pointIndex][0]) *
                     multiplierSpan / differenceSpan +
                 gElement_PowerResistCurve[pointIndex][1];
  }
  switch (halve) {
  case 0:
    break;
  case 1:
    multiplier /= 2;
    break;
  }
  return multiplier + BATTLE_MULTIPLIER_ONE;
}

/// The battle's random number generator, the same as Random16 with its own
/// seed: returns bits 8 to 23 of the new seed.
u32 Battle_Random(void) {
  s32 seed;

  seed = (gBattleRandomSeed * 1103515245) + 12345;
  gBattleRandomSeed = seed;
  return (u32)(seed << 8) >> 16;
}

/// Returns a battle random number from 0 to 99.
u32 Battle_RandomPercent(void) { return ((u32)Battle_Random() * 100) >> 16; }

/// Scales max(attack - defense, 0) plus twice the added power by half the
/// element multiplier, then halves it.
s32 Battle_CalculateAttackDamage(s32 attack, s32 defense, s32 addedPower,
                                 s32 elementDifference) {
  s32 damage;
  s32 scaledDamage;

  attack -= defense;
  damage = elementDifference;
  if (attack < 0)
    attack = 0;
  scaledDamage =
      Battle_GetElementMultiplier(damage, 1) * (attack + (addedPower * 2));
  damage = scaledDamage / (2 * BATTLE_MULTIPLIER_ONE);
  if (damage < 0)
    damage = 0;
  return damage;
}

/// Returns Psynergy's power times the element multiplier and powerMultiplier.
s32 Battle_CalculatePsynergyPower(s32 power, s32 elementDifference,
                                  s32 powerMultiplier) {
  s32 scaledPower;

  scaledPower = Battle_GetElementMultiplier(elementDifference, 0) * power *
                powerMultiplier;
  return scaledPower / (BATTLE_MULTIPLIER_ONE * BATTLE_MULTIPLIER_ONE);
}

/// Returns the amount healed: power times powerMultiplier and the multiplier
/// for the healer's element power above HEALING_BASE_POWER.
s32 Battle_CalculateRecovery(s32 power, s32 elementPower, s32 powerMultiplier) {
  s32 signedProduct;
  u32 scaledRecovery;

  scaledRecovery =
      Battle_GetElementMultiplier((elementPower - HEALING_BASE_POWER) * 2, 0) *
      power;
  scaledRecovery = powerMultiplier * scaledRecovery;
  signedProduct = scaledRecovery;
  return signedProduct / (BATTLE_MULTIPLIER_ONE * BATTLE_MULTIPLIER_ONE);
}

/// Returns the element the unit attacks with (ELEMENT_NONE for none): an
/// enemy's comes from its kind, a character's from its equipped weapon.
s32 Item_GetEquippedElement(s32 unitId) {
  const struct ItemDefinition *item;
  struct Unit *unit;

  unit = Unit_Get(unitId);
  if (unit->classId == CLASS_NONE)
    return Enemy_GetAttackElement(unit);
  item = Inventory_GetEquippedDefinition(unit, ITEM_TYPE_WEAPON);
  if (item != NULL)
    return item->element;
  return ELEMENT_NONE;
}

/// Returns the unleash chance, in percent, that a unit's equipment adds
/// (EQUIP_EFFECT_ADD_UNLEASH_CHANCE).
s32 Equipment_GetUnleashRateBonus(struct Unit *unit) {
  s32 chanceBonus;
  s32 inventorySlot;
  s32 effectIndex;
  const struct ItemDefinition *item;

  chanceBonus = 0;
  for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
    if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
      item = Item_GetDirect(unit->inventory[inventorySlot]);
      for (effectIndex = 0; effectIndex < EQUIP_EFFECT_SLOTS; effectIndex++) {
        if (item->effects[effectIndex].kind == EQUIP_EFFECT_ADD_UNLEASH_CHANCE)
          chanceBonus += item->effects[effectIndex].amount;
      }
    }
  }
  if (chanceBonus < 0)
    chanceBonus = 0;
  return chanceBonus;
}

/// Rolls whether a character's weapon unleashes, 5 percent a point of its
/// chance. Returns the unleash's ability, or ABILITY_ATTACK for a plain attack.
s32 RollWeaponUnleash(struct Unit *owner) {
  struct ItemDefinition *item;
  FIXED rate;

  if (owner->classId == CLASS_NONE) {
    return ABILITY_ATTACK;
  }
  item = Inventory_GetEquippedDefinition(owner, ITEM_TYPE_WEAPON);
  if (item == NULL) {
    return ABILITY_ATTACK;
  }
  if (item->unleashAbility == ABILITY_NOTHING) {
    return ABILITY_ATTACK;
  }
  rate = FIXED_ONE *
         (Equipment_GetUnleashRateBonus(owner) + item->unleashChance * 5) / 100;
  if (rate > (s32)(Battle_Random() & 0xFFFF)) {
    return item->unleashAbility;
  }
  return ABILITY_ATTACK;
}

/// Returns the base chance in percent of an effect; a negative one is fixed,
/// and effects not listed always work.
s32 Battle_GetEffectBaseSuccessRate(s32 effectId) {
  s32 fixedChance;

  switch (effectId) {
  case ABILITY_EFFECT_DEFENSE_DOWN_SHARPLY:
  case ABILITY_EFFECT_DEFENSE_DOWN:
    return 70;
  case ABILITY_EFFECT_RESISTANCE_DOWN_SHARPLY:
  case ABILITY_EFFECT_RESISTANCE_DOWN:
    return 75;
  case ABILITY_EFFECT_CHARM:
    return 30;
  case ABILITY_EFFECT_SLEEP:
    return 45;
  case ABILITY_EFFECT_POISON:
  case ABILITY_EFFECT_VENOM:
  case ABILITY_EFFECT_SEAL:
    return 55;
  case ABILITY_EFFECT_HAUNT:
    return 25;
  case ABILITY_EFFECT_INSTANT_DEATH:
    return 20;
  case ABILITY_EFFECT_DELUSION:
  case ABILITY_EFFECT_DRAIN_HP:
    return 65;
  case ABILITY_EFFECT_CONFUSION:
  case ABILITY_EFFECT_LEAVE_ONE_HP:
    return 35;
  case ABILITY_EFFECT_REVIVE_HALF:
    fixedChance = 60;
    break;
  case ABILITY_EFFECT_DEW:
    fixedChance = 90;
    break;
  case ABILITY_EFFECT_BALM:
    fixedChance = 70;
    break;
  case ABILITY_EFFECT_ATTACK_DOWN_SHARPLY:
  case ABILITY_EFFECT_ATTACK_DOWN:
  case ABILITY_EFFECT_CURSE:
  case ABILITY_EFFECT_DRAIN_PP:
  case ABILITY_EFFECT_CURSE_THREE_ROLLS:
    return 60;
  case ABILITY_EFFECT_HALVE_DEFENSE:
  case ABILITY_EFFECT_ULYSSES:
    return 50;
  case ABILITY_EFFECT_STUN:
  case ABILITY_EFFECT_BLOW_AWAY:
  case ABILITY_EFFECT_STUN_THREE_ROLLS:
    return 40;
  default:
    fixedChance = 100;
    break;
  }
  return -fixedChance;
}

/// Returns 1 when enemy `enemyId` is one of gEnemy_DragonIds.
s32 Enemy_IsDragon(s32 enemyId) {
  u32 i;

  for (i = 0; i < DRAGON_ENEMY_COUNT; i++) {
    if (enemyId == gEnemy_DragonIds[i])
      return 1;
  }
  return 0;
}

/// Returns 1 when enemy `enemyId` is one of gEnemy_SeaCreatureIds.
s32 Enemy_IsSeaCreature(s32 enemyId) {
  u32 i;

  for (i = 0; i < SEA_CREATURE_ENEMY_COUNT; i++) {
    if (enemyId == gEnemy_SeaCreatureIds[i])
      return 1;
  }
  return 0;
}

/// Returns 1 when the unit is weak to effect `effectId`: one of three listed
/// for its enemy kind (EnemyDefinition.weakEffects) or class
/// (ClassDefinition.weakEffects). Called by Battle_RollEffectSuccess.
s32 BattleAbility_IsWeakToEffect(const struct Unit *target, s32 effectId) {
  const u8 *entries;
  s32 i;

  if (target->classId == CLASS_NONE) {
    entries = Enemy_GetDefinition(target->enemyId)->weakEffects;
    for (i = 0; i < 3; i++) {
      if (*entries == effectId)
        return 1;
      entries++;
    }
  } else {
    entries = Class_GetDefinition(target->classId)->weakEffects;
    for (i = 0; i < 3; i++) {
      if (*entries++ == effectId)
        return 1;
    }
  }
  return 0;
}

/// Returns 1 for the revive effects, which only work on a downed unit.
s32 Battle_IsReviveEffect(s32 effectId) {
  if (effectId == ABILITY_EFFECT_REVIVE ||
      effectId == ABILITY_EFFECT_REVIVE_HALF ||
      effectId == ABILITY_EFFECT_DEW || effectId == ABILITY_EFFECT_BALM) {
    return 1;
  }
  return 0;
}

/// The Luck at which a target shrugs off poison, haunting, instant death, the
/// curse, Leave One HP, Gasp and Gale.
#define LUCK_IMMUNITY 40
/// The points of chance an effect gains against a target weak to it.
#define WEAKNESS_BONUS 25

/// Rolls whether effect `effectId` takes hold on `target`: it fails at once
/// when there is nothing for it to do. The chance is the base rate, plus three
/// times the caster's lead over the target in `element`'s level less half
/// the target's luck (so every two points of luck take off three), plus
/// WEAKNESS_BONUS against a weakness, times `successScale` percent. A base rate
/// of 0 or less counts as its magnitude, still scaled but skipping the levels,
/// luck and weakness; 0 still succeeds when Battle_RandomPercent gives 0, about
/// 1 time in 100. Luff, Gasp and Fire Puppet's Paralyze get three tries.
/// Raiden's Wrath and the Trident take hold only on a dragon or a sea creature;
/// a target with LUCK_IMMUNITY Luck or more shrugs off poison, haunting,
/// instant death, the curse, Leave One HP, Gasp and Gale, and a character Gale
/// too.
s32 Battle_RollEffectSuccess(s32 caster, s32 target, s32 element, s32 effectId,
                             s32 successScale) {
  struct Unit *unit = Unit_Get(target);
  s32 attempts = 1;
  s32 score;
  s32 attempt;
  u8 *luck;
  s8 *poison;
  u8 *delusion;
  u8 *confusion;
  u8 *charm;
  u8 *stun;
  u8 *sleep;

  // Raiden's Wrath works only on dragons, the Trident only on sea creatures.
  if (effectId == ABILITY_EFFECT_SLAY_DRAGON)
    return Enemy_IsDragon(unit->enemyId);
  if (effectId == ABILITY_EFFECT_SLAY_SEA_CREATURE)
    return Enemy_IsSeaCreature(unit->enemyId);

  if (Battle_IsReviveEffect(effectId) != 0 && unit->hp != 0) {
    return 0;
  }

  if (effectId == ABILITY_EFFECT_CURE_POISON && unit->poison == 0) {
    return 0;
  }

  // Restore needs a delusion, confusion or charm, or failing those a stun,
  // sleep, seal or curse, tested in that order.
  goto check_restore;
check_restore_others:
  if (unit->stun == 0 && unit->sleep == 0 && *(u8 *)&unit->psySeal == 0 &&
      unit->grimReaper == 0) {
    return 0;
  }
  goto restore_checked;
check_restore:
  if (effectId == ABILITY_EFFECT_RESTORE) {
    if (unit->delusion == 0 && *(u8 *)&unit->confusion == 0 &&
        unit->charm == 0) {
      goto check_restore_others;
    }
  }

restore_checked:
  poison = &unit->poison;
  delusion = &unit->delusion;
  confusion = (u8 *)&unit->confusion;
  charm = &unit->charm;
  stun = &unit->stun;
  sleep = &unit->sleep;
  if (effectId == ABILITY_EFFECT_CURE_ALL && *poison == 0 && *delusion == 0 &&
      *confusion == 0 && *charm == 0 && *stun == 0 && *sleep == 0 &&
      unit->psySeal == 0 && unit->grimReaper == 0 && *(u8 *)&unit->haunt == 0) {
    return 0;
  }

  if (effectId == ABILITY_EFFECT_CURSE && unit->grimReaper == 1) {
    return 0;
  }

  luck = &unit->luck;
  if (*luck >= LUCK_IMMUNITY) {
    switch (effectId) {
    case ABILITY_EFFECT_POISON:
    case ABILITY_EFFECT_VENOM:
    case ABILITY_EFFECT_HAUNT:
    case ABILITY_EFFECT_INSTANT_DEATH:
    case ABILITY_EFFECT_CURSE:
    case ABILITY_EFFECT_LEAVE_ONE_HP:
    case ABILITY_EFFECT_CURSE_THREE_ROLLS:
    case ABILITY_EFFECT_BLOW_AWAY:
      return 0;
    }
  }
  if (unit->classId != CLASS_NONE && effectId == ABILITY_EFFECT_BLOW_AWAY)
    return 0;

  // A positive base rate is adjusted by the element levels, the target's luck
  // and its weaknesses; 0 or a negative one is a fixed chance.
  score = Battle_GetEffectBaseSuccessRate(effectId);
  if (score > 0) {
    s32 difference = Unit_GetElementLevel(caster, element) -
                     Unit_GetElementLevel(target, element) - (*luck >> 1);
    score += difference * 3;
    if (BattleAbility_IsWeakToEffect(unit, effectId) != 0) {
      score += WEAKNESS_BONUS;
    }
  } else {
    score = -score;
  }

  if (effectId == ABILITY_EFFECT_SEAL_2_TURNS)
    attempts = 3;
  if (effectId == ABILITY_EFFECT_CURSE_THREE_ROLLS)
    attempts = 3;
  if (effectId == ABILITY_EFFECT_STUN_THREE_ROLLS)
    attempts = 3;

  for (attempt = 0; attempt < attempts; attempt++) {
    if (score * successScale / 100 >= (s32)Battle_RandomPercent()) {
      return 1;
    }
  }
  return 0;
}

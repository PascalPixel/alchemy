/// @file
/// Battle arithmetic: element multipliers, random rolls and success rates.
// Part of game.c.

#include "../game_internal.h"

/// Returns the multiplier (256 = 1x) for an element difference `input`, read
/// from a five-point line (gElement_PowerResistCurve); with `halve` the effect
/// of the difference is half. Called by the damage and recovery formulas
/// below. The curve's points lie on one straight line, so the result is 256
/// plus 1.28 for each point of difference, with the difference held to -200
/// to 200: 0 to 512, or 128 to 384 halved.
s32 Battle_GetElementMultiplier(s32 input, s32 halve) {
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
  if (input > maximumDifference)
    input = maximumDifference;
  else if (input < minimumDifference)
    input = minimumDifference;
  for (pointIndex = 0; pointIndex < pointCount; pointIndex++) {
    if (input > gElement_PowerResistCurve[pointIndex][0])
      break;
  }
  if (pointIndex == pointCount) {
    multiplier = gElement_PowerResistCurve[pointIndex - 1][1];
  } else {
    differenceSpan = gElement_PowerResistCurve[pointIndex - 1][0] -
                     gElement_PowerResistCurve[pointIndex][0];
    multiplierSpan = gElement_PowerResistCurve[pointIndex - 1][1] -
                     gElement_PowerResistCurve[pointIndex][1];
    multiplier = (input - gElement_PowerResistCurve[pointIndex][0]) *
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
  return multiplier + 0x100;
}

/// The battle's random number generator, the same linear congruential one as
/// Random16 with its own seed. Returns bits 8 to 23 of the new seed and writes
/// gBattleRandomSeed. Rolls that use it advance independently of Random16;
/// link battles exchange this seed before resolving the round.
s32 Battle_Random(void) {
  u32 seed;

  seed = ((u32)gBattleRandomSeed * 0x41c64e6dU) + 0x3039U;
  gBattleRandomSeed = seed;
  return (seed << 8) >> 16;
}

/// Returns a battle random number from 0 to 99.
u32 Battle_RandomPercent(void) { return ((u32)Battle_Random() * 100) >> 16; }

/// Scales max(attack - defense, 0) + twice the action's added power by the
/// elemental multiplier, then halves it. elementDifference is power minus
/// resistance; its effect here is half Psynergy's (128..384 with 256 = 1x).
/// Reached through GAME.S as Battle_CalculateAttackDamageFar, which
/// BattleAbility_ApplyToTarget calls for attacks and physical abilities.
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
  if (scaledDamage < 0)
    scaledDamage += 0x1ff;
  damage = scaledDamage >> 9;
  if (damage < 0)
    damage = 0;
  return damage;
}

/// Returns the power of Psynergy: `power` times the multiplier for the element
/// difference (power minus resistance, 0..512 with 256 = 1x) and the supplied
/// powerMultiplier (also 256 = 1x). Reached through GAME.S as
/// Battle_CalculatePsynergyPowerFar, which BattleAbility_ApplyToTarget calls
/// for Psynergy damage.
s32 Battle_CalculatePsynergyPower(s32 power, s32 elementDifference,
                                  s32 powerMultiplier) {
  s32 scaledPower;

  scaledPower = Battle_GetElementMultiplier(elementDifference, 0) * power *
                powerMultiplier;
  if (scaledPower < 0)
    scaledPower = scaledPower + 0xffff;
  return scaledPower >> 16;
}

/// Returns the amount healed: `power` and `powerMultiplier` (256 is 1x) times
/// the multiplier for twice the healer's element power above 100.
///   elementPower : the healer's power in the element (100 is 1x)
/// Reached through GAME.S as Battle_CalculateRecoveryFar, which the battle and
/// the menu's healing use.
s32 Battle_CalculateRecovery(s32 power, s32 elementPower, s32 powerMultiplier) {
  s32 signedProduct;
  u32 scaledRecovery;

  scaledRecovery =
      Battle_GetElementMultiplier(elementPower * 2 - 200, 0) * power;
  scaledRecovery = powerMultiplier * scaledRecovery;
  signedProduct = scaledRecovery;
  if (signedProduct < 0)
    signedProduct = signedProduct + 0xffff;
  return signedProduct >> 16;
}

/// Returns the element the unit attacks with (ELEMENT_NONE for none): an
/// enemy's comes from its kind, a character's from its equipped weapon.
/// Called by BattleTurn_Build for an attack's element.
s32 Item_GetEquippedElement(s32 owner) {
  struct ItemDefinition *item;
  struct Unit *unit;

  unit = Unit_Get(owner);
  if (unit->classId == CLASS_NONE)
    return Enemy_GetAttackElement(unit);
  item = Inventory_GetEquippedDefinition(unit, ITEM_TYPE_WEAPON);
  if (item != NULL)
    return item->element;
  return ELEMENT_NONE;
}

/// Returns the unleash chance, in percent, that a unit's equipment adds
/// (EQUIP_EFFECT_ADD_UNLEASH_CHANCE). Called by RollWeaponUnleash and the
/// BattleTurn_Build.
s32 Equipment_GetUnleashRateBonus(struct Unit *unit) {
  s32 chanceBonus;
  s32 inventorySlot;
  s32 effectIndex;
  struct ItemDefinition *item;

  chanceBonus = 0;
  for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
    if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
      item = Item_GetDirect(unit->inventory[inventorySlot]);
      for (effectIndex = 0; effectIndex < 4; effectIndex++) {
        if (item->effects[effectIndex].kind == EQUIP_EFFECT_ADD_UNLEASH_CHANCE)
          chanceBonus += item->effects[effectIndex].amount;
      }
    }
  }
  if (chanceBonus < 0)
    chanceBonus = 0;
  return chanceBonus;
}

/// Rolls whether a character's weapon unleashes: its chance is the weapon's
/// unleashChance times 5 percent plus what the equipment adds. Returns the
/// unleash action, or 1 for a plain attack. Called by BattleTurn_Build for
/// each Attack command.
s32 RollWeaponUnleash(struct Unit *owner) {
  struct ItemDefinition *item;
  s32 rate;

  if (owner->classId == CLASS_NONE) {
    return 1;
  }
  item = Inventory_GetEquippedDefinition(owner, ITEM_TYPE_WEAPON);
  if (item == NULL) {
    return 1;
  }
  if (item->unleashAbility == 0) {
    return 1;
  }
  rate = ((Equipment_GetUnleashRateBonus(owner) + (item->unleashChance * 5))
          << 0x10) /
         100;
  if (rate > (Battle_Random() & 0xFFFF)) {
    return item->unleashAbility;
  }
  return 1;
}

/// Returns the base chance, in percent, of effect `effectId`. A negative
/// value is a fixed chance that elements and luck do not change; effects not
/// listed always work (-100). Called by Battle_RollEffectSuccess.
s32 Battle_GetEffectBaseSuccessRate(s32 effectId) {
  s32 fixedChance;
  u32 effectIndex;

  // The switch is on the effect less 8, as the ROM's jump table is.
  effectIndex = effectId - 8;
  switch (effectIndex) {
  case ABILITY_EFFECT_DEFENSE_DOWN_2 - 8:
  case ABILITY_EFFECT_DEFENSE_DOWN - 8:
    return 70;
  case ABILITY_EFFECT_RESISTANCE_DOWN_2 - 8:
  case ABILITY_EFFECT_RESISTANCE_DOWN - 8:
    return 75;
  case ABILITY_EFFECT_CHARM - 8:
    return 30;
  case ABILITY_EFFECT_STUN - 8:
    return 40;
  case ABILITY_EFFECT_SLEEP - 8:
    return 45;
  case ABILITY_EFFECT_POISON - 8:
  case ABILITY_EFFECT_VENOM - 8:
  case ABILITY_EFFECT_SEAL - 8:
    return 55;
  case ABILITY_EFFECT_HAUNT - 8:
    return 25;
  case ABILITY_EFFECT_INSTANT_DEATH - 8:
    return 20;
  case ABILITY_EFFECT_DELUSION - 8:
  case ABILITY_EFFECT_DRAIN_HP - 8:
    return 65;
  case ABILITY_EFFECT_CONFUSION - 8:
  case ABILITY_EFFECT_LEAVE_ONE_HP - 8:
    return 35;
  case ABILITY_EFFECT_HALVE_DEFENSE - 8:
    return 50;
  case ABILITY_EFFECT_REVIVE_HALF - 8:
    fixedChance = 60;
    break;
  case ABILITY_EFFECT_REVIVE_80 - 8:
    fixedChance = 90;
    break;
  case ABILITY_EFFECT_ATTACK_DOWN_2 - 8:
  case ABILITY_EFFECT_ATTACK_DOWN - 8:
  case ABILITY_EFFECT_CURSE - 8:
  case ABILITY_EFFECT_DRAIN_PP - 8:
    return 60;
  default:
    fixedChance = 100;
    break;
  }
  return 0 - fixedChance;
}

/// Returns 1 when the unit is weak to effect `effectId`: one of three listed
/// for its enemy kind (EnemyDefinition.weakEffects) or class
/// (ClassDefinition.weakEffects). Called by Battle_RollEffectSuccess.
s32 BattleAbility_IsWeakToEffect(const struct Unit *target, s32 effectId) {
  const u8 *weakEffects;
  s32 weaknessIndex;

  if (target->classId == CLASS_NONE) {
    weakEffects =
        ((const struct EnemyDefinition *)Enemy_GetDefinition(target->id))
            ->weakEffects;
    for (weaknessIndex = 0; weaknessIndex < 3; weaknessIndex++) {
      if (*weakEffects == effectId)
        return 1;
      weakEffects++;
    }
  } else {
    weakEffects = Class_GetDefinition(target->classId)->weakEffects;
    for (weaknessIndex = 0; weaknessIndex < 3; weaknessIndex++) {
      if (*weakEffects++ == effectId)
        return 1;
    }
  }
  return 0;
}

/// Returns 1 for the revive effects (Revive, Quartz, Dew), which only work on
/// a downed unit. Called by the battle's targeting, which lets them aim at
/// downed members, and by Battle_RollEffectSuccess.
s32 Battle_IsReviveEffect(s32 effectId) {
  if ((effectId == ABILITY_EFFECT_REVIVE) ||
      (effectId == ABILITY_EFFECT_REVIVE_HALF) ||
      (effectId == ABILITY_EFFECT_REVIVE_80)) {
    return 1;
  }
  return 0;
}

/// Rolls whether effect `effectId` takes hold on `target`: it fails at once
/// when there is nothing for it to do. The chance is the base rate, plus three
/// times the caster's lead over the target in the element's level less half
/// the target's luck (so every two points of luck take off three), plus
/// 25 against a weakness, times `successScale` percent. A base rate of 0 or
/// less counts as its magnitude, still scaled but skipping the levels, luck
/// and weakness; 0 still succeeds when Battle_RandomPercent gives 0, about 1
/// time in 100. Luff gets three tries.
/// Reached through GAME.S as Battle_RollEffectSuccessFar, which
/// BattleAbility_ApplyToTarget and BattleTurn_Build call for each target of an
/// ability with an effect.
s32 Battle_RollEffectSuccess(s32 caster, s32 target, s32 resistanceCategory,
                             s32 effectId, s32 successScale) {
  // Restore's psySeal and confusion reads and Cure All's haunt read go
  // through the byte view; as unit fields, each zero test merges with its
  // neighbour's into one halfword or word load.
  struct Unit *unit = Unit_Get(target);
  s32 attempts = 1;
  s32 score;
  s32 attempt;
  s8 *poison;
  u8 *delusion;
  u8 *confusion;
  u8 *charm;
  u8 *stun;
  u8 *sleep;

  if (Battle_IsReviveEffect(effectId) != 0 && unit->hp != 0) {
    return 0;
  }

  if (effectId == ABILITY_EFFECT_CURE_POISON && unit->poison == 0) {
    return 0;
  }

  // Restore needs a delusion, confusion or charm, or failing those a stun,
  // sleep, seal or curse; the order of these tests is the ROM's.
  goto action4_check;
action4_tail:
  if (unit->stun == 0 && unit->sleep == 0 && *(u8 *)&unit->psySeal == 0 &&
      unit->grimReaper == 0) {
    return 0;
  }
  goto action4_done;
action4_check:
  if (effectId == ABILITY_EFFECT_RESTORE) {
    if (unit->delusion == 0 && *(u8 *)&unit->confusion == 0 &&
        unit->charm == 0) {
      goto action4_tail;
    }
  }

action4_done:
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

  // A positive base rate is adjusted by the element levels, the target's luck
  // and its weaknesses; 0 or a negative one is a fixed chance.
  score = Battle_GetEffectBaseSuccessRate(effectId);
  if (score > 0) {
    s32 difference = Unit_GetElementLevel(caster, resistanceCategory) -
                     Unit_GetElementLevel(target, resistanceCategory) -
                     (unit->luck >> 1);
    score += difference * 3;
    if (BattleAbility_IsWeakToEffect(unit, effectId) != 0) {
      score += 25;
    }
  } else {
    score = -score;
  }

  if (effectId == ABILITY_EFFECT_SEAL_2_TURNS) {
    attempts = 3;
  }

  for (attempt = 0; attempt < attempts; attempt++) {
    if (score * successScale / 100 >= (s32)Battle_RandomPercent()) {
      return 1;
    }
  }
  return 0;
}

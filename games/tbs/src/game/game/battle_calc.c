/// @file
/// Battle arithmetic: element multipliers, random rolls and success rates.
/// Part of game.c, the game object, compiled only through it.

#include "../game_internal.h"

/// Returns the multiplier (256 = 1x) for an element difference `input`, read
/// from a five-point line; with `halve` the effect of the difference is half.
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
/// Random16 with its own seed. Returns bits 8 to 23 of the new seed.
s32 BattleRandom(void) {
  u32 seed;

  seed = ((u32)gBattleRandomSeed * 0x41c64e6dU) + 0x3039U;
  gBattleRandomSeed = seed;
  return (seed << 8) >> 16;
}

/// Returns a battle random number from 0 to 99.
u32 BattleRandomPercent(void) { return ((u32)BattleRandom() * 100) >> 16; }

/// Scales max(attack - defense, 0) + twice the action's added power by the
/// elemental multiplier, then halves it. ElementDifference is power minus
/// resistance; its effect here is half Psynergy's (128..384 with 256 = 1x).
s32 Battle_CalcAttackDamage(s32 attack, s32 defense, s32 addedPower,
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

/// The power of Psynergy: `power` times the multiplier for the element
/// difference (power minus resistance, 0..512 with 256 = 1x) and the supplied
/// powerMultiplier (also 256 = 1x).
s32 Battle_CalcPsynergyPower(s32 power, s32 elementDifference,
                             s32 powerMultiplier) {
  s32 scaledPower;

  scaledPower = Battle_GetElementMultiplier(elementDifference, 0) * power *
                powerMultiplier;
  if (scaledPower < 0)
    scaledPower = scaledPower + 0xffff;
  return scaledPower >> 16;
}

/// The amount healed: `power` and `rate` (256 is 1x) times the multiplier for
/// twice the healer's element power above 100.
///   element : the healer's power in the element (100 is 1x)
s32 Battle_CalcRecovery(s32 power, s32 elementPower, s32 powerMultiplier) {
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

/// Returns the element the unit attacks with (4 for none): an enemy's comes
/// from its kind, a character's from its equipped weapon.
s32 Item_GetEquippedElement(s32 owner) {
  struct ItemDefinition *item;
  struct BattleUnit *unit;

  unit = UnitGet(owner);
  if (unit->classIndex == 0)
    return Enemy_GetAttackElement(unit);
  item = Inventory_GetEquippedDefinition(unit, 1); // 1: weapon
  if (item != NULL)
    return item->element;
  return 4;
}

/// Returns the unleash chance, in percent, that a unit's equipment adds
/// (equipment effect 23).
s32 Equipment_GetUnleashRateBonus(struct BattleUnit *unit) {
  s32 chanceBonus;
  s32 inventorySlot;
  s32 effectIndex;
  struct ItemDefinition *item;

  chanceBonus = 0;
  for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
    if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
      item = Item_GetDirect(unit->inventory[inventorySlot]);
      for (effectIndex = 0; effectIndex < 4; effectIndex++) {
        if (item->effects[effectIndex].kind == ITEM_EFFECT_ADD_UNLEASH_CHANCE)
          chanceBonus += item->effects[effectIndex].amount;
      }
    }
  }
  if (chanceBonus < 0)
    chanceBonus = 0;
  return chanceBonus;
}

/// Rolls whether a character's weapon unleashes. Returns the unleash action,
/// or 1 for a plain attack.
s32 RollWeaponUnleash(struct BattleUnit *owner) {
  struct ItemDefinition *item;
  s32 rate;

  if (owner->classIndex == 0) {
    return 1;
  }
  item = Inventory_GetEquippedDefinition(owner, 1);
  if (item == NULL) {
    return 1;
  }
  if (item->unleashAction == 0) {
    return 1;
  }
  rate = ((Equipment_GetUnleashRateBonus(owner) + (item->unleashChance * 5))
          << 0x10) /
         100;
  if (rate > (BattleRandom() & 0xFFFF)) {
    return item->unleashAction;
  }
  return 1;
}

/// Returns the base chance, in percent, of effect `effectId`. A negative
/// value is a fixed chance that elements and luck do not change.
s32 BattleFx_GetBaseSuccessRate(s32 effectId) {
  s32 fixedChance;
  u32 effectIndex;

  effectIndex = effectId - 8;
  switch (effectIndex) {
  case 4:
  case 5:
    return 70;
  case 8:
  case 9:
    return 75;
  case 14:
    return 30;
  case 15:
    return 40;
  case 16:
    return 45;
  case 10:
  case 11:
  case 17:
    return 55;
  case 18:
    return 25;
  case 19:
    return 20;
  case 12:
  case 23:
    return 65;
  case 13:
  case 26:
    return 35;
  case 27:
    return 50;
  case 48:
    fixedChance = 60;
    break;
  case 49:
    fixedChance = 90;
    break;
  case 0:
  case 1:
  case 20:
  case 24:
    return 60;
  default:
    fixedChance = 100;
    break;
  }
  return 0 - fixedChance;
}

/// Returns 1 when the unit is weak to effect `effectId`: one of three listed
/// for its enemy kind or class.
s32 BattleTarget_IsWeakToEffect(const struct BattleUnit *target, s32 effectId) {
  const u8 *weakEffects;
  s32 weaknessIndex;

  if (target->classIndex == 0) {
    weakEffects = Enemy_GetDefinition(target->classId) + 0x48;
    for (weaknessIndex = 0; weaknessIndex < 3; weaknessIndex++) {
      if (*weakEffects == effectId)
        return 1;
      weakEffects++;
    }
  } else {
    weakEffects = Class_GetDefinition(target->classIndex)->unk50;
    for (weaknessIndex = 0; weaknessIndex < 3; weaknessIndex++) {
      if (*weakEffects++ == effectId)
        return 1;
    }
  }
  return 0;
}

/// Returns 1 for the revive effects, which only work on a downed unit.
s32 Battle_IsReviveEffect(s32 effectId) {
  if ((effectId == 5) || (effectId == 0x38) || (effectId == 0x39)) {
    return 1;
  }
  return 0;
}

/// Rolls whether effect `effectId` takes hold on `target`: it fails at once
/// when there is nothing for it to do. The chance is the base rate, plus three
/// times the caster's lead over the target in the element, less half the
/// target's luck, plus 25 against a weakness, times `successScale` percent.
/// Some effects get three tries.
s32 BattleFx_RollSuccess(s32 caster, s32 target, s32 resistanceCategory,
                         s32 effectId, s32 successScale) {
  /* FAKEMATCH: three status reads (psySeal after the effect 4 test, confusion
   * in it, haunt for effect 0x40) go through the byte view state[offset], so
   * each builds its address from a fresh offset constant as the native does;
   * read as unit fields, CSE reuses a neighbouring address. All eleven reads as
   * fields differ 120 (184 instructions for 204); every other read is a field
   * at no cost. */
  u8 *state = (u8 *)UnitGet(target);
  struct BattleUnit *unit = (struct BattleUnit *)state;
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

  if (effectId == 3 && unit->poison == 0) {
    return 0;
  }

  goto action4_check;
action4_tail:
  if (unit->stun == 0 && unit->sleep == 0 &&
      state[(u32)&((struct BattleUnit *)0)->psySeal] == 0 &&
      unit->grimReaper == 0) {
    return 0;
  }
  goto action4_done;
action4_check:
  if (effectId == 4) {
    if (unit->delusion == 0 &&
        state[(u32)&((struct BattleUnit *)0)->confusion] == 0 &&
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
  if (effectId == 0x40 && *poison == 0 && *delusion == 0 && *confusion == 0 &&
      *charm == 0 && *stun == 0 && *sleep == 0 && unit->psySeal == 0 &&
      unit->grimReaper == 0 &&
      state[(u32)&((struct BattleUnit *)0)->haunt] == 0) {
    return 0;
  }

  if (effectId == 0x1C && unit->grimReaper == 1) {
    return 0;
  }

  score = BattleFx_GetBaseSuccessRate(effectId);
  if (score > 0) {
    s32 difference = Unit_GetElementLevel(caster, resistanceCategory) -
                     Unit_GetElementLevel(target, resistanceCategory) -
                     (unit->luck >> 1);
    score += difference * 3;
    if (BattleTarget_IsWeakToEffect(unit, effectId) != 0) {
      score += 25;
    }
  } else {
    score = -score;
  }

  if (effectId == 0x43) {
    attempts = 3;
  }

  for (attempt = 0; attempt < attempts; attempt++) {
    if (score * successScale / 100 >= (s32)BattleRandomPercent()) {
      return 1;
    }
  }
  return 0;
}

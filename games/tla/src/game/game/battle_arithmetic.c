/// @file
/// Battle arithmetic: element multipliers, random rolls and success rates.
// Part of game.c.

#include "../game_internal.h"

extern const s16 gElement_PowerResistCurve[5][2];

s32 Battle_GetElementMultiplier(s32 input, s32 halve) {
  s32 min;
  s32 max;
  s32 n;
  s32 i;
  s32 value;
  s32 dx;
  s32 dy;

  max = gElement_PowerResistCurve[0][0];
  min = gElement_PowerResistCurve[4][0];
  n = 5;
  if (input > max)
    input = max;
  else if (input < min)
    input = min;
  for (i = 0; i < n; i++) {
    if (input > gElement_PowerResistCurve[i][0])
      break;
  }
  if (i == n) {
    value = gElement_PowerResistCurve[i - 1][1];
  } else {
    dx = gElement_PowerResistCurve[i - 1][0] - gElement_PowerResistCurve[i][0];
    dy = gElement_PowerResistCurve[i - 1][1] - gElement_PowerResistCurve[i][1];
    value = (input - gElement_PowerResistCurve[i][0]) * dy / dx +
            gElement_PowerResistCurve[i][1];
  }
  switch (halve) {
  case 0:
    break;
  case 1:
    value /= 2;
    break;
  }
  return value + 0x100;
}

/// The battle's linear congruential random number generator. Returns bits 8
/// to 23 of the new seed.
u32 Battle_Random(void) {
  s32 seed;

  seed = (gBattleRandomSeed * 0x41c64e6d) + 0x3039;
  gBattleRandomSeed = seed;
  return (u32)(seed << 8) >> 16;
}

/// Returns a battle random number from 0 to 99.
u32 Battle_RandomPercent(void) { return (Battle_Random() * 100) >> 16; }

/// Returns attack damage (the far-call alias Battle_CalculateAttackDamageFar):
/// (atk - def + add * 2) / 2 times the elemental difference multiplier. 256 is
/// unity; the difference has half the effect it has in
/// Battle_CalculatePsynergyPower (128-384).
/// def: defense (clamp atk - def to zero).
/// add: ability power.
/// bonus: elemental difference (power - resistance).
s32 Battle_CalculateAttackDamage(s32 atk, s32 def, s32 add, s32 bonus) {
  s32 dmg;
  s32 val;

  atk -= def;
  dmg = bonus;
  if (atk < 0)
    atk = 0;
  val = Battle_GetElementMultiplier(dmg, 1) * (atk + (add * 2));
  if (val < 0)
    val += 0x1ff;
  dmg = val >> 9;
  if (dmg < 0)
    dmg = 0;
  return dmg;
}

/// Returns Psynergy power (the far-call alias Battle_CalculatePsynergyPowerFar): power times
/// rate and the elemental multiplier (0-512, with 256 unity).
/// bonus: elemental difference (power - resistance).
/// rate: multiplier, with 256 unity.
s32 Battle_CalculatePsynergyPower(s32 power, s32 bonus, s32 rate) {
  s32 val;

  val = Battle_GetElementMultiplier(bonus, 0) * power * rate;
  if (val < 0)
    val = val + 0xffff;
  return val >> 16;
}

/// Returns the amount recovered (the far-call alias Battle_CalculateRecoveryFar): power
/// and rate times the multiplier (256 is unity) for twice the user's elemental
/// power above 100.
/// element: user's elemental power (100: unity, >=200: double, <=0: zero).
/// rate: multiplier, with 256 unity.
s32 Battle_CalculateRecovery(s32 power, s32 element, s32 rate) {
  s32 val;
  u32 prod;

  prod = Battle_GetElementMultiplier(element * 2 - 200, 0) * power;
  prod = rate * prod;
  val = prod;
  if (val < 0)
    val = val + 0xffff;
  return val >> 16;
}

/// Returns the element of the unit's equipped weapon (4 for none); an enemy
/// has no equipment and attacks with its species' element instead.
s32 Item_GetEquippedElement(s32 id) {
  struct ItemDefinition *item;
  struct BattleUnit *unit;

  unit = Unit_Get(id);
  if (unit->classIndex == 0)
    return Enemy_GetAttackElement(unit);
  item = Inventory_GetEquippedDefinition((void *)unit, 1); // 1: weapon.
  if (item != NULL)
    return item->element;
  return 4;
}

s32 Equipment_GetUnleashRateBonus(struct BattleUnit *owner) {
  s32 sum;
  s32 i;
  s32 j;
  struct ItemDefinition *item;

  sum = 0;
  for (i = 0; i < 15; i++) {
    if (owner->inventory[i] & 0x200) {
      item = Item_GetDirect(owner->inventory[i]);
      for (j = 0; j < 4; j++) {
        if (item->effects[j].kind == 23)
          sum += item->effects[j].amount;
      }
    }
  }
  if (sum < 0)
    sum = 0;
  return sum;
}

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
          << 16) /
         100;
  if (rate > (s32)(Battle_Random() & 0xFFFF)) {
    return item->unleashAction;
  }
  return 1;
}

/// A rate for the effect id: 20 to 75 for the ids listed, otherwise a
/// negative value (-60, -90 and -70 for three ids, -100 by default).
s32 Func_080b0514(s32 effect_id) {
  s32 battle_result;
  u32 entry_index;

  entry_index = effect_id - 8;
  switch (entry_index) {
  case 4:
  case 5:
    return 70;
  case 8:
  case 9:
    return 75;
  case 14:
    return 30;
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
  case 48:
    battle_result = 60;
    break;
  case 49:
    battle_result = 90;
    break;
  case 65:
    battle_result = 70;
    break;
  case 0:
  case 1:
  case 20:
  case 24:
  case 72:
    return 60;
  case 27:
  case 75:
    return 50;
  case 15:
  case 74:
  case 77:
    return 40;
  default:
    battle_result = 100;
    break;
  }
  return 0 - battle_result;
}

s32 Battle_IsEffect90Class(s32 classId) {
  u32 i;

  for (i = 0; i < 47; i++) {
    if (classId == gBattle_Effect90Classes[i])
      return 1;
  }
  return 0;
}

s32 Battle_IsEffect91Class(s32 classId) {
  u32 i;

  for (i = 0; i < 29; i++) {
    if (classId == gBattle_Effect91Classes[i])
      return 1;
  }
  return 0;
}

s32 BattleTarget_IsWeakToEffect(const struct BattleUnit *target, s32 effectId) {
  const u8 *entries;
  s32 i;

  if (target->classIndex == 0) {
    entries = Enemy_GetDefinition(target->enemyId) + 0x3e;
    for (i = 0; i < 3; i++) {
      if (*entries == effectId)
        return 1;
      entries++;
    }
  } else {
    entries = Class_GetDefinition(target->classIndex)->weakEffects;
    for (i = 0; i < 3; i++) {
      if (*entries++ == effectId)
        return 1;
    }
  }
  return 0;
}

s32 Battle_IsReviveEffect(s32 effect_id) {
  if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39) ||
      (effect_id == 0x49)) {
    return 1;
  }
  return 0;
}

/// Rolls whether effect `effectId` takes hold on `target`: it fails at once
/// when there is nothing for it to do; the chance is the base rate, plus three
/// times the caster's lead over the target in the element, less half the
/// target's luck, plus 25 against a weakness, times `successScale` percent.
/// Effects 90 and 91 are decided by the target's enemy id; a target with 40
/// luck or more shrugs off effects 18, 19, 26 to 28, 34, 80 and 82, and a
/// character 82; effects 80 and 85 get three tries as 67 does.
// As The Broken Seal's function of the same name, with effects 90 and 91 added.
s32 BattleEffect_RollSuccess(s32 caster, s32 target, s32 resistanceCategory,
                         s32 effectId, s32 successScale) {
  u8 *state = Unit_Get(target);
  struct BattleUnit *unit = (struct BattleUnit *)state;
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

  if (effectId == 90)
    return Battle_IsEffect90Class(unit->enemyId);
  if (effectId == 91)
    return Battle_IsEffect91Class(unit->enemyId);

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

  luck = &unit->luck;
  if (*luck >= 40) {
    switch (effectId) {
    case 18:
    case 19:
    case 26:
    case 27:
    case 28:
    case 34:
    case 80:
    case 82:
      return 0;
    }
  }
  if (unit->classIndex != 0 && effectId == 82)
    return 0;

  score = Func_080b0514(effectId);
  if (score > 0) {
    s32 difference = Unit_GetElementLevel(caster, resistanceCategory) -
                     Unit_GetElementLevel(target, resistanceCategory) -
                     (*luck >> 1);
    score += difference * 3;
    if (BattleTarget_IsWeakToEffect(unit, effectId) != 0) {
      score += 25;
    }
  } else {
    score = -score;
  }

  if (effectId == 67)
    attempts = 3;
  if (effectId == 80)
    attempts = 3;
  if (effectId == 85)
    attempts = 3;

  for (attempt = 0; attempt < attempts; attempt++) {
    if (score * successScale / 100 >= (s32)Battle_RandomPercent()) {
      return 1;
    }
  }
  return 0;
}

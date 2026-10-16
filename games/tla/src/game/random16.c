//--------------------------------------------------------------------------
// random16.c
// Battle: random numbers and damage/recovery calculations.
//--------------------------------------------------------------------------

#include "types.h"
#include "battle.h"
#include "game.h"
#include "system.h"
#include "battle_calc.h"
#include "scene.h"
#include "item.h"
#include "inventory.h"
#include "battle_random.h"
#include "battle_type.h"

//--------------------------------------------------------------------------
// Battle linear congruential generator; return seed bits 8-23.
//--------------------------------------------------------------------------
u32 BattleRandom(void) {
  s32 seed;

  seed = (gBattleRandomSeed * 0x41c64e6d) + 0x3039;
  gBattleRandomSeed = seed;
  return (u32)(seed << 8) >> 16;
}

//--------------------------------------------------------------------------
// Random number from 0 to 99.
//--------------------------------------------------------------------------
u32 BattleRandomPercent(void) { return (BattleRandom() * 100) >> 16; }

//--------------------------------------------------------------------------
// Attack damage (BattleAtkCalc).
// Multiply (atk - def + add * 2) / 2 by the elemental difference multiplier.
// 256 is unity; the difference has half BattlePowerCalc's effect (128-384).
// atk: attack.
// def: defense (clamp atk - def to zero).
// add: ability power.
// bonus: elemental difference (power - resistance).
//--------------------------------------------------------------------------
s32 Battle_CalcAttackDamage(s32 atk, s32 def, s32 add, s32 bonus) {
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

//--------------------------------------------------------------------------
// Psynergy power (BattlePowerCalc).
// Multiply power by rate and the elemental multiplier (0-512, with 256 unity).
// power: power.
// bonus: elemental difference (power - resistance).
// rate: multiplier, with 256 unity.
//--------------------------------------------------------------------------
s32 Battle_CalcPsynergyPower(s32 power, s32 bonus, s32 rate) {
  s32 val;

  val = Battle_GetElementMultiplier(bonus, 0) * power * rate;
  if (val < 0)
    val = val + 0xffff;
  return val >> 16;
}

//--------------------------------------------------------------------------
// Recovery amount (BattleRecoveryCalc).
// Derive the multiplier (256 is unity) from twice the elemental power above
// 100, then apply it to power and rate. power: power. element: user's elemental
// power (100: unity, >=200: double, <=0: zero). rate: multiplier, with 256
// unity.
//--------------------------------------------------------------------------
s32 Battle_CalcRecovery(s32 power, s32 element, s32 rate) {
  s32 val;
  u32 prod;

  prod = Battle_GetElementMultiplier(element * 2 - 200, 0) * power;
  prod = rate * prod;
  val = prod;
  if (val < 0)
    val = val + 0xffff;
  return val >> 16;
}

s32 Enemy_GetAttackElement(void *state);

//--------------------------------------------------------------------------
// Return the unit's attack element (4 means no element).
// Enemies use their species' element; allies use their equipped weapon's
// element. id: unit ID.
//--------------------------------------------------------------------------
s32 Item_GetEquippedElement(s32 id) {
  struct ItemDefinition *item;
  struct BattleUnit *unit;

  unit = UnitGet(id);
  if (unit->classIndex == 0)
    return Enemy_GetAttackElement(unit);
  item = Inventory_GetEquippedDefinition((void *)unit, 1); // 1: weapon.
  if (item != NULL)
    return item->element;
  return 4;
}

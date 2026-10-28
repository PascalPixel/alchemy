#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_unit.h"
#include "enemy_definition.h"
#include "owner_state.h"

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

s32 Unit_GetElementLevel(s32 unitId, s32 element);
u32 Battle_RandomPercent(void);

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

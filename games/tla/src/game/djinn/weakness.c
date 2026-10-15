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
#include "runtime_interfaces.h"
#include "owner_state.h"

// A rate for the effect id: 20 to 75 for the ids listed, otherwise a
// negative value (-60, -90 and -70 for three ids, -100 by default).
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
    entries = Owner_GetRecord(target->enemyId) + 0x3e;
    for (i = 0; i < 3; i++) {
      if (*entries == effectId)
        return 1;
      entries++;
    }
  } else {
    entries = Owner_GetRecordStride84(target->classIndex)->weakEffects;
    for (i = 0; i < 3; i++) {
      if (*entries++ == effectId)
        return 1;
    }
  }
  return 0;
}

s32 BattleEffectRevCheck(s32 effect_id) {
  if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39) ||
      (effect_id == 0x49)) {
    return 1;
  }
  return 0;
}

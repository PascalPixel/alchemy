// ported from games/tbs/src/game/character/owners.c
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "game_flags.h"
#include "party_state.h"
#include "types.h"
#include "battle_summon.h"
#include "runtime_interfaces.h"
#include "fixed_math.h"
#include "owner_state.h"
#include "iwram_call.h"

s32 GameFlag_SetBit(s32);

struct TradeFlags {
  u32 flags;
};

struct ElementLevelBonus {
  u16 power;
  u16 res;
};

extern const struct ElementLevelBonus gElement_PowerResistByLevel[16];

s32 GameFlag_Test(s32 flag);

#if EDITION_INTERNATIONAL
#define ENEMY_SUFFIX_FIRST '1'
#define ENEMY_SUFFIX_LAST 8
#else
#define ENEMY_SUFFIX_FIRST 'A'
#define ENEMY_SUFFIX_LAST 25
#endif

typedef s32 (*ClearWordsFn)(void *destination, s32 size);

s32 Class_Determine(s32 character, const u8 *djinn) {
  s32 levels[4];
  s32 primary;
  s32 best;
  s32 series;
  s32 result;
  s32 j;
  s32 i;

  result = -1;
  if (character > 7)
    return 0;
  Unit_GetElementLevels(character, djinn, levels);
  if (GameFlag_Test(32)) {
    if (character == 0)
      return 200;
    if (character == 1)
      return 201;
  }
  if (character == 5)
    return 202;
  if (result != -1)
    return result;
  best = result;
  primary = result;
  for (i = 0; i < 4; i++) {
    if (best < levels[i]) {
      best = levels[i];
      primary = i;
    }
  }
  j = -1;
  best = -1;
  for (i = 0; i < 4; i++) {
    if (i != primary && best < levels[i]) {
      best = levels[i];
      j = i;
    }
  }
  series = Class_GetSeries(primary, levels[j] > 9 ? j : primary);
  for (i = 202; i >= 0; i--) {
    if (gClass_DefinitionTable[i].series != series)
      continue;
    for (j = 0; j < 4; j++) {
      if (levels[j] < gClass_DefinitionTable[i].required[j] * 10)
        break;
    }
    if (j == 4) {
      result = i;
      break;
    }
  }
  if (result == -1)
    result = 0;
  return result;
}

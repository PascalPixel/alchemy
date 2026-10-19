#ifndef ALCHEMY_BATTLE_EFFECT_H
#define ALCHEMY_BATTLE_EFFECT_H

#include "edition.h"
#include "types.h"

struct BattleEffectArgument;

enum {
  COMPOSITE_CLIP_X = 1,
  COMPOSITE_CLIP_Y = 2,
  COMPOSITE_FLIP_X = 4,
  COMPOSITE_FLIP_Y = 8,
  COMPOSITE_FLIP = COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y
};
enum {
  COMPOSITE_COPY,
  COMPOSITE_TRANSPARENT,
  COMPOSITE_MAX,
  COMPOSITE_ADDITIVE
};

typedef s32 (*BattleEffectDrawRectangle)(void *destination, const void *source,
                                         s32 x, s32 y, s32 width, s32 height);
typedef BattleEffectDrawRectangle DrawRectangle;
typedef BattleEffectDrawRectangle DrawRectangleFn;

enum {
  BATTLE_DAMAGE_PP_DIRECT = -1,
  BATTLE_DAMAGE_HP_HEAL = 1,
  BATTLE_DAMAGE_HP_POWER = 2,
  BATTLE_DAMAGE_WEAPON = 3,
  BATTLE_DAMAGE_WEAPON_SCALED = 4,
  BATTLE_DAMAGE_HP_ADDITIVE_6 = 5,
  BATTLE_DAMAGE_HP_ADDITIVE_7 = 6,
  BATTLE_DAMAGE_HP_ADDITIVE_9 = 8,
  BATTLE_DAMAGE_PP_DRAIN = 10,
  BATTLE_DAMAGE_PP_HEAL = 11
};

#endif

#ifndef ALCHEMY_BATTLE_EFFECT_H
#define ALCHEMY_BATTLE_EFFECT_H

#include "edition.h"
#include "types.h"

struct BattleEffectArgument;
#if GAME_TLA
void BattleFx_RunParticleStreams(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunMeteor(struct BattleEffectArgument *effect);
void BattleFx_RunOuterSpace(struct BattleEffectArgument *effect);
void BattleFx_RunPaletteParticles(struct BattleEffectArgument *effect,
                                  s32 mode);
void BattleFx_RunCybele(struct BattleEffectArgument *effect);

s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused);

void Resource_LoadAndDecompress(s32 resourceId, void *target, s32 unpack,
                                s32 wait);
#endif

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
#if GAME_TLA
s32 BattleFx_BuildBlitter(s32 slot, s32 width_shift, s32 height_shift,
                          s32 flags, u32 mode);
#endif

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

enum {
  EFFECT_CURE_POISON = 3,
  EFFECT_CURE_PART = 4,
  EFFECT_REVIVE_FULL = 5,
  EFFECT_ATK_UP2 = 6,
  EFFECT_ATK_UP1 = 7,
  EFFECT_ATK_DOWN2 = 8,
  EFFECT_ATK_DOWN1 = 9,
  EFFECT_DEF_UP2 = 10,
  EFFECT_DEF_UP1 = 11,
  EFFECT_DEF_DOWN2 = 12,
  EFFECT_DEF_DOWN1 = 13,
  EFFECT_RES_UP2 = 14,
  EFFECT_RES_UP1 = 15,
  EFFECT_RES_DOWN2 = 16,
  EFFECT_RES_DOWN1 = 17,
  EFFECT_POISON = 18,
  EFFECT_VENOM = 19,
  EFFECT_DELUSION = 20,
  EFFECT_CONFUSE = 21,
  EFFECT_CHARM = 22,
  EFFECT_STUN = 23,
  EFFECT_SLEEP = 24,
  EFFECT_PSY_BLOCK = 25,
  EFFECT_EVIL_SPIRIT = 26,
  EFFECT_INSTANT_DOWN = 27,
  EFFECT_DEATH_CURSE = 28,
  EFFECT_REFRAIN = 29,
  EFFECT_REFLECT = 30,
  EFFECT_DRAIN_HP = 31,
  EFFECT_DRAIN_PP = 32,
  EFFECT_BUFF_CLEAR = 33,
  EFFECT_LETHAL = 34,
  EFFECT_HALF_DEF = 35,
  EFFECT_TEXT_NONE = 45,
  EFFECT_GUARD1 = 46,
  EFFECT_GUARD2 = 47,
  EFFECT_STANDBY = 50,
  EFFECT_STANDBY_WORK = 51,
  EFFECT_IMMOBILIZE = 53,
  EFFECT_CHALLENGE = 54,
  EFFECT_ACTOR_FLASH = 55,
  EFFECT_REVIVE_HALF = 56,
  EFFECT_REVIVE_80 = 57,
  EFFECT_AGI_SET_DOWN4 = 58,
  EFFECT_AGI_SET_UP8 = 59,
  EFFECT_DRAIN_HP_HALF = 60,
  EFFECT_HEAL_60 = 61,
  EFFECT_HEAL_30 = 62,
  EFFECT_PP_RESTORE_7 = 63,
  EFFECT_CURE_ALL = 64,
  EFFECT_READY = 66,
  EFFECT_PSY_SEAL = 67,
  EFFECT_PP_LEECH = 69
};

#endif

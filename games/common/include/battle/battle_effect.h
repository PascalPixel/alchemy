#ifndef ALCHEMY_BATTLE_EFFECT_H
#define ALCHEMY_BATTLE_EFFECT_H

#include "edition.h"
#include "types.h"

struct BattleEffectArgument;
#if GAME_TLA
void BattleEffect_RunParticleStreams(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_InitializeDefaultMode(struct BattleEffectArgument *effect);
void BattleFx_InitializeMode1(struct BattleEffectArgument *effect);
void BattleEffect_RunPaletteParticles(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunDitherDissolveScene(struct BattleEffectArgument *effect);

s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused);

void Resource_LoadAndDecompress(s32 resource_id, void *target, s32 unpack, s32 wait);
#endif

enum
{
	COMPOSITE_CLIP_X = 1,
	COMPOSITE_CLIP_Y = 2,
	COMPOSITE_FLIP_X = 4,
	COMPOSITE_FLIP_Y = 8,
	COMPOSITE_FLIP = COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y
};
enum
{
	COMPOSITE_COPY,
	COMPOSITE_TRANSPARENT,
	COMPOSITE_MAX,
	COMPOSITE_ADDITIVE
};
#if GAME_TLA
s32 BattleEffect_LoadWork(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode);
#endif

typedef s32 (*BattleEffectDrawRectangle)(
	void *destination, const void *source, s32 x, s32 y, s32 width, s32 height);
typedef BattleEffectDrawRectangle DrawRectangle;
typedef BattleEffectDrawRectangle DrawRectangleFn;

enum
{
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

enum
{
	EFX_CURE_POISON = 3,
	EFX_CURE_PART = 4,
	EFX_REVIVE_FULL = 5,
	EFX_ATK_UP2 = 6,
	EFX_ATK_UP1 = 7,
	EFX_ATK_DOWN2 = 8,
	EFX_ATK_DOWN1 = 9,
	EFX_DEF_UP2 = 10,
	EFX_DEF_UP1 = 11,
	EFX_DEF_DOWN2 = 12,
	EFX_DEF_DOWN1 = 13,
	EFX_RES_UP2 = 14,
	EFX_RES_UP1 = 15,
	EFX_RES_DOWN2 = 16,
	EFX_RES_DOWN1 = 17,
	EFX_POISON = 18,
	EFX_VENOM = 19,
	EFX_DELUSION = 20,
	EFX_CONFUSE = 21,
	EFX_CHARM = 22,
	EFX_STUN = 23,
	EFX_SLEEP = 24,
	EFX_PSY_BLOCK = 25,
	EFX_EVIL_SPIRIT = 26,
	EFX_INSTANT_DOWN = 27,
	EFX_DEATH_CURSE = 28,
	EFX_REFRAIN = 29,
	EFX_REFLECT = 30,
	EFX_DRAIN_HP = 31,
	EFX_DRAIN_PP = 32,
	EFX_BUFF_CLEAR = 33,
	EFX_LETHAL = 34,
	EFX_HALF_DEF = 35,
	EFX_TEXT_NONE = 45,
	EFX_GUARD1 = 46,
	EFX_GUARD2 = 47,
	EFX_STANDBY = 50,
	EFX_STANDBY_WORK = 51,
	EFX_IMMOBILIZE = 53,
	EFX_CHALLENGE = 54,
	EFX_ACTOR_FLASH = 55,
	EFX_REVIVE_HALF = 56,
	EFX_REVIVE_80 = 57,
	EFX_AGI_SET_DOWN4 = 58,
	EFX_AGI_SET_UP8 = 59,
	EFX_DRAIN_HP_HALF = 60,
	EFX_HEAL_60 = 61,
	EFX_HEAL_30 = 62,
	EFX_PP_RESTORE_7 = 63,
	EFX_CURE_ALL = 64,
	EFX_READY = 66,
	EFX_PSY_SEAL = 67,
	EFX_PP_LEECH = 69
};

#endif

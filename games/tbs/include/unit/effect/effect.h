#ifndef ALCHEMY_UNIT_EFFECT_EFFECT_H
#define ALCHEMY_UNIT_EFFECT_EFFECT_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"

struct BattleEffectArgument;
struct EffectPosition;
struct EffectStep;

typedef struct
{
	s32 reserved_00[3];
	s32 current_value;
	s32 index_radius;
	s32 entry_limit;
	s32 reserved_18[3];
	s16 values[1];
} BattleEffectIndexWindow;

void BattlePres_ConfigureEffectDisplay(void);
void BattleFx_AdvanceScrollOnInterval(void);
void Camera_AdvanceBg2Reference(void);
void BattleFx_ArmWin0HBlankDma(void);
void BattleEffect_RunFallingParticles(struct BattleEffectArgument *effect);
void BattleEffect_RunFallingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode0(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode1(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode2(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode4(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode3(struct BattleEffectArgument *effect);
void BattleFx_RunFiveModeMode1B(struct BattleEffectArgument *effect);
void BattleFx_RunFiveMode(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunParticlePoolMode1(struct BattleEffectArgument *effect);
void BattleFx_RunParticlePoolMode0(struct BattleEffectArgument *effect);
void BattleFx_RunParticlePool(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunTwelveModeMode6(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode3(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode1(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode10(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode5(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode9(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode4(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode0(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode8(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode7(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode2(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveModeMode11(struct BattleEffectArgument *effect);
void BattleFx_RunTwelveMode(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunFlippingBurst(struct BattleEffectArgument *effect);
void BattleFx_RunBladeRain(struct BattleEffectArgument *effect);

// 粒の置き場：Ram_MapCellBuffer を struct EffectStep の並びとして使う
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

#define CLIP (GOUSEI_CLIP_X | GOUSEI_CLIP_Y) // 上下左右ではみ出しを切る

#define HI(v) (((s16 *)&(v))[1])

struct Placement
{
	s32 x;
	s32 y;
	s32 z;
	s32 unknown_0c;
};

// 2 次元の組 {x, y}（位置や速さ）
struct Point2D
{
	s32 x;
	s32 y;
};

struct BlitterPair
{
	DrawRectangle upper;
	DrawRectangle lower;
};

#define gBlitters ((DrawRectangle *)gWorkSlot)

#endif

#ifndef ALCHEMY_BATTLE_PRESENTATION_H
#define ALCHEMY_BATTLE_PRESENTATION_H

/// @file
/// What the battle object's later parts (games/tla/src/battle/battle_*.c, from
/// motion_graphics.c to spoils.c) declare for the files that use them,
/// written once.

#include "types.h"
#include "resource_ids.h"
#include "battle/battle_action.h"
#include "battle/battle_side.h"

struct BattleActionRecord;
struct BattlePlan;

u32 BattleUnit_IsListed(u32 id);

s32 BattleFormation_BuildEnemyList(s32 recordId);

void BattleBackground_Init(void);
void BattleBackground_Load(s32 mode, s32 resource, s32 level);

void BattleTransition_Run(s32 arg, s32 kind);

void Battle_TurnEndNoOp(void);

void BattleBackground_Free(void);

s32 BattleSprite_GetSprite(s32 index);
s32 BattleSprite_GetPalette(s32 index);

s32 BattleActionRun_Build(struct BattleActionRecord *request,
                          struct BattlePlan *plan);

s32 BattleStaging_RunAttack(struct BattlePlan *plan, s32 mode);

s32 DjinnRecovery_CountDownSide(s32 side);

void BattleBackground_SetMode(s32 mode, s32 background, s32 value);

s32 BattleAI_ChooseAction(struct BattleActionRecord *request, s32 retry);

u32 BattleSprite_GetAttackEffect(u32 index);
u32 BattleSprite_GetDefeatSound(s32 index);

s32 BattleAI_ChooseRandomTarget(s32 party);

s32 BattleDjinn_CountStandby(u32 unitId, u8 *counts);

struct BattlePresentationWork;

/// The bit decoder BattleBackground_Load copies to RAM and runs. It returns
/// the source after the last halfword it read.
typedef const u8 *(*ModelDecoder)(const u8 *source, void *destination);

struct SavedBattleObj {
  struct BattleObject object;
  u32 *buffer;
  u8 unitId;
  u8 single;
  u8 reactivate;
  u16 classId;
};

/// One kept formation: its record and its average level, -1 while empty.
struct FormationCandidate {
  s16 recordId;
  s32 score : 16;
};

struct AIEnemyRest {
  u8 unk00[0x2c];
  u16 abilities[8];
};

extern const u8 Model_DecodeCode[];

extern u8 Model_DecodeCodeSize[];

void Animation_SkipToStopFar(void *record);

void Sprite_RemoveAnimationAtFar(void *record, s32 slot);

s32 SpriteAnimation_SumCommandLengthsFar(s32 animation, s32 a, s32 b);

void ColorBuffer_Scale(u16 *source, u32 destination, s32 value, s32 size);

void BattleMessage_OpenFar(s32 message);

void SpriteDissolve_StepFar(void *record, s32 step);

s32 Unit_TryLevelUpFar(s32 unitId, s16 *gains);

extern u8 gBattleMessage_AdvanceArrowTiles[];

extern u8 gBattleAI_ActionWeights[3][8];

extern u16 gBattleAI_DullahanForms[];

extern const u8 gBattleTransition_CurtainVariants[];

extern u8 gBattleTransition_CurtainTiles[];

void BattleEffect_RunShatteringOpeningFar(struct BattlePresentationWork *work);

extern const struct BattleSideOrder gBattleParty_TurnEndSideOrder;

void BattleUnit_DrainDjinn(s32 unitId, s32 all, s32 announce);

#endif

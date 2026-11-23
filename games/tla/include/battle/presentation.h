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

void BattleTransition_Run(s32 enemyCount, s32 kind);

void Battle_TurnEndNoOp(void);

void BattleBackground_Free(void);

s32 BattleSprite_GetSprite(s32 index);
s32 BattleSprite_GetPalette(s32 index);

s32 BattleActionRun_Build(struct BattleActionRecord *request,
                          struct BattlePlan *plan);

s32 BattleStaging_RunAttack(struct BattlePlan *plan, s32 mode);

s32 DjinnRecovery_CountDownSide(s32 side);

void BattleBackground_SetMode(s32 mode, s32 background, s32 fade);

s32 BattleAI_ChooseAction(struct BattleActionRecord *request, s32 retry);

u32 BattleSprite_GetAttackEffect(u32 index);
u32 BattleSprite_GetDefeatSound(s32 index);

s32 BattleAI_ChooseRandomTarget(s32 party);

s32 BattleDjinn_CountStandby(u32 unitId, u8 *counts);

struct BattlePresentationWork;

/// The bit decoder BattleBackground_Load copies to RAM and runs. It returns
/// the source after the last halfword it read.
typedef const u8 *(*ModelDecoder)(const u8 *source, void *destination);

/// What BattleMotion_CallBeast saves of the battle object a called beast
/// borrows, for BattleMotion_DismissBeast to put back.
struct SavedBattleObject {
  struct BattleObject object; ///< the borrowed object, copied whole
  /// The beast's pictures: a heap block of its own, or gActorSpriteSlots when
  /// the object borrowed is another side's.
  u32 *buffer;
  u8 unitId; ///< the unit whose battle object the beast borrows
  /// 1 when the caster and its first target are on opposite sides: the beast
  /// takes the caster's own object.
  u8 single;
  u8 reactivate; ///< 1 when the borrowed object had to be made first
  u16 classId;   ///< the beast's enemy id (ENEMY_WILD_WOLF and so on)
};

/// One kept formation: its record and its average level, -1 while empty.
struct FormationCandidate {
  s16 recordId;
  s32 score : 16;
};

/// An EnemyDefinition as BattleAI_ChooseAction reads its abilities: through a
/// pointer two bytes past its start.
struct AIEnemyRest {
  u8 unused[44];    ///< the rest of the definition before its abilities
  u16 abilities[8]; ///< EnemyDefinition.abilities
};

/// The bit decoder BattleBackground_Load copies to RAM to decode a battle
/// background's tiles (model_decode.s).
extern const u8 Model_DecodeCode[];
extern u8 Model_DecodeCodeSize[]; ///< Model_DecodeCode's bytes

void Sprite_SkipToStopFar(void *record);

void Sprite_RemoveAnimationAtFar(void *record, s32 slot);

s32 SpriteAnimation_SumCommandLengthsFar(s32 id, s32 animation, s32 timedCount);

/// Writes size BGR555 colours of source to destination scaled by value / 256,
/// at most 1.0: a palette dimmed toward black.
void ColorBuffer_Scale(u16 *source, u32 destination, s32 value, s32 size);

void BattleMessage_OpenFar(s32 message);

void SpriteDissolve_StepFar(void *record, s32 step);

s32 Unit_TryLevelUpFar(s32 unitId, s16 *gains);

/// The advance arrow beside a battle message: eight 128-byte pictures of a
/// 256-colour sprite, one every four frames.
extern u8 gBattleMessage_AdvanceArrowTiles[];

/// BattleAI_ChooseAction's rolls for the first three action strategies, a row
/// each: the chance in 256 of each place in the enemy's actions.
extern u8 gBattleAI_ActionWeights[3][8];
/// The enemy definitions Dullahan acts as, one drawn at random.
extern u16 gBattleAI_DullahanForms[];

/// The battle transition's curtain tile in eight variants, 32 bytes (one 4-bit
/// tile) each, a new one every four frames.
extern const u8 gBattleTransition_CurtainVariants[];
/// The battle transition's curtain tiles, 32 bytes (one 4-bit tile) each.
extern u8 gBattleTransition_CurtainTiles[];

void BattleEffect_RunShatteringOpeningFar(struct BattlePresentationWork *work);

/// The party, then the enemies: the order the sides take their turn-end
/// effects in.
extern const struct BattleSideOrder gBattleParty_TurnEndSideOrder;

void BattleUnit_DrainDjinn(s32 unitId, s32 all, s32 announce);

#endif

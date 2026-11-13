#ifndef ALCHEMY_MODULE_BATTLE_BATTLE_H
#define ALCHEMY_MODULE_BATTLE_BATTLE_H

/// @file
/// What the battle object defines for the code outside it.

#include "types.h"
#include "battle_type.h"
#include "field/sprite.h"

struct BattleActionRecord;
struct BattlePlan;
struct BattlePresentationWork;
struct BattleSortedUnitEntry;

struct CastAnimationEntry {
  u8 unk00[5];
  u8 param;
};

struct CastAnimation {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct CastAnimationEntry *entries[1];
};

s32 AutoBattle_ShowDefend(s16 *);
s32 BattleStaging_RunUnitAction(s16 *);
s32 AutoBattle_RunAttackOrItem(struct BattleActionRecord *action);
s32 AutoBattle_RunPsynergy(struct BattleActionRecord *);
s32 Battle_RunEncounter(s32 enemyGroup);
void BattleMotion_ReservedNoOp83B0();
s32 BattleUnit_GetEffectHeight(s32 owner);
s32 BattleUnit_ListLiving(s32 sideMask, u16 *unitIds);
void BattleUnit_RefreshPlacement(void);
void BattleSpoils_Clear(void);
s32 BattleUnit_ListIds(s32 groups, u16 *dst);
void Func_0811b9fc(void);
void Func_0811d7e4(void);
s32 Battle_GatherActions(struct BattleActionRecord *actions);
s32 AutoBattle_BuildActions(struct BattleSortedUnitEntry *entries);
s32 Func_0811d9cc(struct BattleActionRecord *action, s32 delay);
s32 AutoBattle_RunAction(s16 *action);
s32 Battle_FindUnitPlace(u32 id);
void Battle_ProcessTurnEnd(void);
void Battle_AwardSpoils(void);
s32 BattleStaging_HideField(struct BattlePlan *plan, s32 mode);
s32 BattleStaging_RunPsynergyCast(struct BattlePlan *plan, s32 mode);
s32 BattleStaging_RunItem(struct BattlePlan *plan, s32 unused,
                          struct BattleActionRecord *selection);
s32 BattleStaging_RunFlee(struct BattlePlan *plan);
void BattleUnit_ProcessActionEnd(struct BattlePlan *plan);
void ElementGlow_Play(s32 unitId, s32 element, s32 mode, s32 value);
void BattleBackground_RestorePalette(void);
s32 Func_0811d79c(s32 kind);
s32 EnemySprite_LoadIntoSlots(s32 unit);
void BattleCamera_SetViewAtPairMidpoint(s32 first, s32 second, s32 scale);
void BattleUnit_ListAllAndSubmit(void);
s32 BattlePlayback_WaitForReady(void);
void BattleStaging_BuildTargetList(struct BattlePlan *plan,
                                   struct BattlePresentationWork *effect);
s32 BattleEnemy_RecordDefeat(s32 unitId, s32 earned);
void BattleObject_UpdatePose(s32 id);
u32 BattlePlayback_Reset(void);
void BattleIntro_AnnounceEncounter(s32 enemyCount);

#endif

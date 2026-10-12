#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT6_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT6_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/effect4.h"
#include "unit/battle/effect/effect5.h"

struct BattleEffectArgument;

void Camera_ApplyPhasedDelta(void);
void BattleFx_RunNoEffectFrames(s32 cnt);
void BattleFx_InitializeMode(s32 *arg);
void BattleFx_DispatchMode(s32 *arg);
void BattleFx_BuildWindowEdgeTable(void);
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *arg);
void BattleEffect_SetupBlendedDisplay(void);
void ObjectGroup_UpdateMembers(s32 set_id, s32 object_value, s32 group_value, s32 state_slot, s32 state_value);
void BattleFx_InitTilemapAndFlushQueue(void);

#endif

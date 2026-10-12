#ifndef ALCHEMY_UNIT_BATTLE_BATTLE10_H
#define ALCHEMY_UNIT_BATTLE_BATTLE10_H

#include "types.h"

s32 Party_ComputeEligibleMemberAverage(s32 record_id);
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *out_margin);
s32 Owner_ApplyLevelGains(s32 owner, s32 levels);
s32 EnmCntDec(s32 no, s32 n);
s32 EnmCntRel(s32 actor_id);
s32 EnmCntClr(s32 class_id);
s32 BtlGrpRnd(s32 record_id);
s32 BattleFormation_BuildEnemyList(s32 record_id);

#endif

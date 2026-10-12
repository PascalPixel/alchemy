#ifndef ALCHEMY_UNIT_BATTLE_BATTLE12_H
#define ALCHEMY_UNIT_BATTLE_BATTLE12_H

#include "types.h"

u32 Item_EncodeBankedId(u32 value);
void Summon_ClearWorkFields(void);
s32 BattleEnemy_RecordDefeat(s32 unit_id, s32 earned);
void Battle_AwardSpoils(void);
void Battle_ReservedNoOp2A08(void);

#endif

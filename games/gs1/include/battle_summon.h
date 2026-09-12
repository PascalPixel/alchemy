#ifndef ALCHEMY_BATTLE_SUMMON_H
#define ALCHEMY_BATTLE_SUMMON_H

#include "types.h"

struct SummonDefinition {
    u16 name_message_id;
    u8 unknown_02[2];
    u8 djinn_required[4];
};

struct BattleSummonState {
    u32 available_mask;
};

/* 召喚（スタンバイ）。effect 50/51 の径路。 */
const struct SummonDefinition *Summon_GetRecord(s32 summon_id);
s32 Summon_FindSlot(void); /* 空きスロットを探す */
s32 Summon_ClassValid(s32 class_id); /* クラスが有効か */

#define Summon_ClassId BattleFormation_SelectRandomAvailableMember

s32 SummonDefinition_GetNear(u32 summon_id);
s32 BattleFormation_SelectRandomAvailableMember(s32 record_id);
s32 Summon_TakeCharge(s32 class_id, s32 n);
s32 Summon_ReleaseCharge(s32 actor_id);
s32 Summon_ResetCharge(s32 class_id);
void Summon_Refresh(void);

#endif

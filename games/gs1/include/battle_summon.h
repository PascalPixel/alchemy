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
s32 Func_080797d4(u32 summon_id);
const struct SummonDefinition *Func_080771e0(s32 summon_id);
struct BattleSummonState *Func_080b5d3c(void);
s32 Func_080b7514(void); /* 空きスロットを探す */
s32 Func_080c1fa8(s32 work0); /* ワーク先頭語から召喚クラスを引く */
s32 Func_080b6cdc(s32 class_id); /* クラスが有効か */
s32 Func_080c1df4(s32 class_id, s32 n); /* チャージ取得。bit15 は要リセット */
s32 Func_080c1f50(s32 class_id); /* チャージのリセット */
void Func_080b7548(void); /* 配置後の更新 */

#define SummonDefinition_GetNear Func_080797d4
#define SummonDefinition_Get Func_080771e0
#define BattleSummon_UpdateAvailability Func_080b5d3c
#define Summon_FindSlot Func_080b7514
#define Summon_ClassId Func_080c1fa8
#define Summon_ClassValid Func_080b6cdc
#define Summon_TakeCharge Func_080c1df4
#define Summon_ResetCharge Func_080c1f50
#define Summon_Refresh Func_080b7548

#endif

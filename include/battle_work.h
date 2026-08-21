#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "types.h"

extern void *Data_03001e74; /* 戦闘ワークへのポインタセル */
extern u8 Data_03001388[];  /* ユニット退避コピーの記述子 */

#define BattleWorkPtr Data_03001e74
#define UnitCopyDesc Data_03001388

/* 戦闘ワーク+65 のモード適用。役割未確定。 */
void Func_08015130(s32 mode);
#define Sys_SetMode Func_08015130

#endif

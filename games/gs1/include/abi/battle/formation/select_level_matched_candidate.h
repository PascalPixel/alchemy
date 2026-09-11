#ifndef ALCHEMY_ABI_BATTLE_FORMATION_SELECT_LEVEL_MATCHED_CANDIDATE
#define ALCHEMY_ABI_BATTLE_FORMATION_SELECT_LEVEL_MATCHED_CANDIDATE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b6a60(u16 *out_units);
s32 Func_080770d0(s32 id);
s32 Func_080770e0(s32 id);
void Func_08002df0(void *ptr);

#define BattleFormation_SelectLevelMatchedCandidate Func_080c1afc
#define Battle_Check Func_080b6a60
#define Battle_Check2 Func_080770e0
#define Battle_Check3 Func_080770d0
#define Battle_Do Func_08002df0
#define Battle_Run Func_08077198
#define gRom Data_080c73f8

#endif

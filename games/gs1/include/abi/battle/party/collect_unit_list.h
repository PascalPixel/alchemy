#ifndef ALCHEMY_ABI_BATTLE_PARTY_COLLECT_UNIT_LIST
#define ALCHEMY_ABI_BATTLE_PARTY_COLLECT_UNIT_LIST

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b6a60(u16 *out);
s32 Func_080b6ae0(u16 *out);

#define Battle_Check Func_080b6a60
#define Battle_Check2 Func_080b6ae0
#define gBattleWork Data_03001e74
#define gRom Data_080c2a10

#endif

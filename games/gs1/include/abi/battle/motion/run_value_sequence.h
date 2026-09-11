#ifndef ALCHEMY_ABI_BATTLE_MOTION_RUN_VALUE_SEQUENCE
#define ALCHEMY_ABI_BATTLE_MOTION_RUN_VALUE_SEQUENCE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080152b8(u16 *selection);
s32 Func_080b6cd0(s32 id);
void Func_08015130(s32 mode);

#define Battle_Check Func_080b6cd0
#define Battle_Do Func_080152b8
#define Battle_Do2 Func_08015130
#define gBattleWork Data_03001e74

#endif

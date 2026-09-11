#ifndef ALCHEMY_ABI_BATTLE_SHOW_PAIRED_UNIT_WORK_AND_WAIT
#define ALCHEMY_ABI_BATTLE_SHOW_PAIRED_UNIT_WORK_AND_WAIT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080915ac(u32);
s32 Func_080150f8(s32, s32, s32, s32);
s32 Func_08015050(s32);
void Func_08015100(s32);

#define Battle_Check Func_080915ac
#define Battle_Check2 Func_08015050
#define Battle_Do Func_08015100
#define Battle_SetMode Func_080150f8
#define gIw Data_03001c94
#define gWork Data_03001ebc

#endif

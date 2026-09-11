#ifndef ALCHEMY_ABI_BATTLE_INTRO_ANNOUNCE_ENCOUNTER
#define ALCHEMY_ABI_BATTLE_INTRO_ANNOUNCE_ENCOUNTER

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015118(void);
void Func_08015218(void);
void Func_080bb65c(void);

#define Battle_Run Func_08015118
#define Battle_Run2 Func_080bb65c
#define Battle_Run3 Func_08015218
#define gBattleWork Data_03001e74

#endif

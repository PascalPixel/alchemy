#ifndef ALCHEMY_ABI_BATTLE_EVENT_RUNTIME_DISPATCH_QUEUED_EVENTS
#define ALCHEMY_ABI_BATTLE_EVENT_RUNTIME_DISPATCH_QUEUED_EVENTS

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080bb928(void *, u32);
void Func_080bb8e8(u32);
void Func_08015120(u32, u32);
void Func_080151c8(u32);
void Func_080bb65c(void);
void Func_08015118(void);
void Func_080f9010(u32);
void Func_080babdc(u32, u32, u32);
void Func_080c24f0(u32, u32);
void Func_080bb588(u32);
void Func_080bace8(u32);
void Func_08015130(u32);
u32 Func_080b7dd0(u32);
void Func_080b78e4(u32, u32);
void Func_080b7aac(u32);
u32 Func_080bdfec(void);

#define Battle_Apply Func_080bb928
#define Battle_Apply2 Func_08015120
#define Battle_Apply3 Func_080c24f0
#define Battle_Apply4 Func_080b78e4
#define Battle_Do Func_080bb8e8
#define Battle_Do2 Func_080151c8
#define Battle_Do3 Func_080f9010
#define Battle_Do4 Func_080bb588
#define Battle_Do5 Func_080bace8
#define Battle_Do6 Func_08015130
#define Battle_Do7 Func_080b7dd0
#define Battle_Do8 Func_080b7aac
#define Battle_Place Func_080babdc
#define Battle_Run Func_08015118
#define Battle_Run2 Func_080bb65c
#define Battle_Run3 Func_080bdfec
#define gBattleWork Data_03001e74
#define gIw Data_03001ee4

#endif

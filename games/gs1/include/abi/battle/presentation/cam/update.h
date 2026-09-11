#ifndef ALCHEMY_ABI_BATTLE_PRESENTATION_CAM_UPDATE
#define ALCHEMY_ABI_BATTLE_PRESENTATION_CAM_UPDATE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_080c0a24(u32, u32, s32, s32, u32);

#define Battle_Do Func_08004cb4
#define Battle_Do2 Func_08004c1c
#define Battle_Do3 Func_08004bd4
#define Battle_Run Func_080b5864
#define Battle_Run2 Func_080049ac
#define Battle_SetRange Func_080c0a24

#endif

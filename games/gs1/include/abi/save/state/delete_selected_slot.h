#ifndef ALCHEMY_ABI_SAVE_STATE_DELETE_SELECTED_SLOT
#define ALCHEMY_ABI_SAVE_STATE_DELETE_SELECTED_SLOT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080056cc(void);
void Func_08005c68(void);
u32 Func_08005ac0(s32);
void Func_0801776c(s32, s32);
s32 Func_08020244(s16, s32);
s32 Func_08017658(s32, s32, s32, s32);
s32 Func_08028df4(s32, s32, s32, s32);

#define State_Apply Func_0801776c
#define State_Apply2 Func_08020244
#define State_Check Func_080056cc
#define State_Do Func_08005ac0
#define State_Run Func_08005c68
#define State_Run2 Func_08005cf8
#define State_SetMode Func_08017658
#define State_SetMode2 Func_08028df4

#endif

#ifndef ALCHEMY_ABI_SAVE_STATE_CONFIRM_AND_PROCESS_SELECTED_SLOT
#define ALCHEMY_ABI_SAVE_STATE_CONFIRM_AND_PROCESS_SELECTED_SLOT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08017658(s32, s32, s32, s32);
s32 Func_08028df4(s32, s32, s32, s32);
s32 Func_0801faa8(void);
void Func_0801776c(s32, s32);

#define State_Apply Func_0801776c
#define State_Check Func_0801faa8
#define State_SetMode Func_08017658
#define State_SetMode2 Func_08028df4

#endif

#ifndef ALCHEMY_ABI_SAVE_STATE_WRITE_CURRENT_SLOT_PAIR
#define ALCHEMY_ABI_SAVE_STATE_WRITE_CURRENT_SLOT_PAIR

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080056cc(void);
s32 Func_08005920(s32, void *);
void Func_0801776c(s32, s32);
void Func_0801f818(void);

#define State_Apply Func_0801776c
#define State_Apply2 Func_08005920
#define State_Check Func_080056cc
#define State_Run Func_0801f818
#define State_Run2 Func_08005cf8
#define gOv Data_02000000

#endif

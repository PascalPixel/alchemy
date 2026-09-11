#ifndef ALCHEMY_ABI_SAVE_STATE_COPY_SLOT_PAIR
#define ALCHEMY_ABI_SAVE_STATE_COPY_SLOT_PAIR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08005c68(void);
s32 Func_080056cc(void);
s32 Func_08020244(s16, s32);
u32 Func_08005a78(s32, void *);
u32 Func_0801f704(void);
u32 Func_08005920(s32, void *);
void Func_0801776c(s32, s32);

#define State_Apply Func_0801776c
#define State_Apply2 Func_08020244
#define State_Apply3 Func_08005a78
#define State_Apply4 Func_08005920
#define State_Check Func_080056cc
#define State_Run Func_08005c68
#define State_Run2 Func_0801f704
#define State_Run3 Func_08005cf8
#define gOv Data_02000000

#endif

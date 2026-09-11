#ifndef ALCHEMY_ABI_SAVE_STATE_LOAD_RECORD_INTO_WORK
#define ALCHEMY_ABI_SAVE_STATE_LOAD_RECORD_INTO_WORK

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080056cc(void);
void Func_08005c68(void);
void Func_0801776c(s32, s32);
s32 Func_08020244(s16, s32);
s32 Func_08005a78(s32, void *);

#define State_Apply Func_0801776c
#define State_Apply2 Func_08020244
#define State_Apply3 Func_08005a78
#define State_Check Func_080056cc
#define State_Run Func_08005c68
#define State_Run2 Func_08005cf8
#define gCell Data_02000240
#define gIw Data_03001c9c
#define gIw2 Data_03001d08
#define gIw3 Data_03001d24
#define gOv Data_02000000
#define gOv2 Data_02002004

#endif

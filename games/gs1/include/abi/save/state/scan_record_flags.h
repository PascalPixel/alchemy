#ifndef ALCHEMY_ABI_SAVE_STATE_SCAN_RECORD_FLAGS
#define ALCHEMY_ABI_SAVE_STATE_SCAN_RECORD_FLAGS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080056cc(void);
s32 Func_08005c68(void);

#define State_Check Func_080056cc
#define State_Check2 Func_08005c68
#define State_Run Func_08005cf8
#define gOv Data_0200200c

#endif

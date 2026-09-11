#ifndef ALCHEMY_ABI_UI_WINDOW_FIT_ON_SCREEN
#define ALCHEMY_ABI_UI_WINDOW_FIT_ON_SCREEN

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08018a50(s32 start, s32 *width, s32 *count, s32 mode);

#define Ui_Run Func_0801868c
#define Ui_Run2 Func_08018850
#define Ui_SetMode Func_08018a50
#define gIw Data_03001e8c

#endif

#ifndef ALCHEMY_ABI_UI_SET_BANK15_PALETTE_AND_CLEAR_RENDER_MODE
#define ALCHEMY_ABI_UI_SET_BANK15_PALETTE_AND_CLEAR_RENDER_MODE

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_08097868(void);
extern s32 Func_08015360(s32, s32);

#define Ui_Apply Func_08015360
#define Ui_Run Func_08097868
#define gCell Data_02000240

#endif

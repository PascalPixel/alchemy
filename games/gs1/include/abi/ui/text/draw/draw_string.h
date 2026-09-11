#ifndef ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_STRING
#define ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_STRING

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08002df0(s16 *);
s32 Func_08017aa4(s16 *, s32, s32, s32);

#define Ui_Check Func_08002df0
#define Ui_SetMode Func_08017aa4

#endif

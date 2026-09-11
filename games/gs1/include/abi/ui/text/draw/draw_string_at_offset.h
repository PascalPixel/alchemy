#ifndef ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_STRING_AT_OFFSET
#define ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_STRING_AT_OFFSET

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08002df0(s16 *);
s32 Func_0801de5c(s16 *, s32, s32, s32);

#define Ui_Check Func_08002df0
#define Ui_SetMode Func_0801de5c
#define gIw Data_03001e8c

#endif

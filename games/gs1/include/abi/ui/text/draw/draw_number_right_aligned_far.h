#ifndef ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_NUMBER_RIGHT_ALIGNED_FAR
#define ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_NUMBER_RIGHT_ALIGNED_FAR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080150a8(s32 value, s32 digits, s32 layer, s32 x, s32 y);

#define Ui_SetRange Func_080150a8

#endif

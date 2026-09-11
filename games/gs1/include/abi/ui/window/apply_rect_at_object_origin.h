#ifndef ALCHEMY_ABI_UI_WINDOW_APPLY_RECT_AT_OBJECT_ORIGIN
#define ALCHEMY_ABI_UI_WINDOW_APPLY_RECT_AT_OBJECT_ORIGIN

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080ab21c(s32 x, s32 y, s32 width, s32 height, u32 palette);

#define Ui_SetRange Func_080ab21c

#endif

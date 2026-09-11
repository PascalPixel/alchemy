#ifndef ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_STRING_IN_WINDOW
#define ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_STRING_IN_WINDOW

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08017c8c(u16 *, s32, s32, s32);
void Func_08002df0(u16 *);

#define Ui_Do Func_08002df0
#define Ui_SetMode Func_08017c8c

#endif

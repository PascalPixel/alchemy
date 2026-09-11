#ifndef ALCHEMY_ABI_UI_RUNTIME_MISC_DRAW_WORK_BY_ATTRIBUTES
#define ALCHEMY_ABI_UI_RUNTIME_MISC_DRAW_WORK_BY_ATTRIBUTES

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08017248(s32, s32, s32, s32, s32);
void Func_080170f8(s32, s32, s32, s32);

#define Ui_SetMode Func_080170f8
#define Ui_SetRange Func_08017248

#endif

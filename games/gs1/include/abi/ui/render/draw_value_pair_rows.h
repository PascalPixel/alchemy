#ifndef ALCHEMY_ABI_UI_RENDER_DRAW_VALUE_PAIR_ROWS
#define ALCHEMY_ABI_UI_RENDER_DRAW_VALUE_PAIR_ROWS

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015090(s32 image, s32 layer, s32 x, s32 y);
void Func_08015098(s32 image, s32 layer, s32 x, s32 y);
void Func_080a14f0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

#define Ui_SetMode Func_08015090
#define Ui_SetMode2 Func_08015098
#define Ui_SetMode3 Func_080a14f0

#endif

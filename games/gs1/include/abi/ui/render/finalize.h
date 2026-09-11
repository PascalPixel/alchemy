#ifndef ALCHEMY_ABI_UI_RENDER_FINALIZE
#define ALCHEMY_ABI_UI_RENDER_FINALIZE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08016178(u16 x, u16 y, u16 width, u16 height);

#define Ui_SetMode Func_08016178

#endif

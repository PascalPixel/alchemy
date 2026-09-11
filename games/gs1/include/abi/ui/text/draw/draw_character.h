#ifndef ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_CHARACTER
#define ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_CHARACTER

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0801de5c(u16 *text, s32 map_address, s32 vram_address, s32 phase);

#define Ui_SetMode Func_0801de5c
#define gIw Data_03001e8c

#endif

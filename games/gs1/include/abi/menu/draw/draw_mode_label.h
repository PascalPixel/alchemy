#ifndef ALCHEMY_ABI_MENU_DRAW_DRAW_MODE_LABEL
#define ALCHEMY_ABI_MENU_DRAW_DRAW_MODE_LABEL

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_080164d4(void *, s32, s32, s32, s32);

#define Menu_SetRange Func_080164d4
#define gIw Data_03001f38

#endif

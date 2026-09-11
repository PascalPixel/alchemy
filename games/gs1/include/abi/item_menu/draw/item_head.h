#ifndef ALCHEMY_ABI_ITEM_MENU_DRAW_ITEM_HEAD
#define ALCHEMY_ABI_ITEM_MENU_DRAW_ITEM_HEAD

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_080a17c4(void *icon);
extern void Func_08015090(s32, void *, s32, s32);

#define Sys_Do Func_080a17c4
#define Sys_SetMode Func_08015090
#define gIw Data_03001f2c

#endif

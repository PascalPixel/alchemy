#ifndef ALCHEMY_ABI_ITEM_MENU_DRAW_DETAIL
#define ALCHEMY_ABI_ITEM_MENU_DRAW_DETAIL

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015270(s32);
void Func_080030f8(s32);
void Func_08015080(s32, s32, s32, s32);
s32 Func_080a2268(s32, s32, s32, s32, s32, s32);

#define Sys_Do Func_08015270
#define Sys_Do2 Func_080030f8
#define Sys_SetMode Func_08015080
#define Sys_SetRect Func_080a2268
#define gIw Data_03001f2c

#endif

#ifndef ALCHEMY_ABI_ITEM_MENU_USE
#define ALCHEMY_ABI_ITEM_MENU_USE

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_08015278(s32);
extern s32 Func_080a1d08(s32, s32, s32);
extern s32 Func_080aa448(u32);

#define Sys_Check Func_08015278
#define Sys_Place Func_080a1d08
#define gIw Data_03001f2c

#endif

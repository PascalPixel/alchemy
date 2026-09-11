#ifndef ALCHEMY_ABI_UI_MENU_CREATE_CURSOR
#define ALCHEMY_ABI_UI_MENU_CREATE_CURSOR

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080a10d0(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

#define Menu_SetRect Func_080a10d0

#endif

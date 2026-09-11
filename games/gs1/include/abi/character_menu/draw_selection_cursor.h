#ifndef ALCHEMY_ABI_CHARACTER_MENU_DRAW_SELECTION_CURSOR
#define ALCHEMY_ABI_CHARACTER_MENU_DRAW_SELECTION_CURSOR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080a2268(const u8 *, s32, s32, s32, s32, u32);

#define Sys_SetRect Func_080a2268
#define gIw Data_03001f2c
#define gRom Data_080af2fc

#endif

#ifndef ALCHEMY_ABI_ITEM_MENU_OPEN_DETAIL
#define ALCHEMY_ABI_ITEM_MENU_OPEN_DETAIL

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080a10d0(s32 *, s32, s32, s32, s32, s32);
void Func_080a22f4(void);
void Func_080a4924(s32, s32);

#define ItemMenu_DrawItemDetails Func_080a4924
#define Sys_Run Func_080a22f4
#define Sys_SetRect Func_080a10d0
#define gIw Data_03001f2c

#endif

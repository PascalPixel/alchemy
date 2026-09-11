#ifndef ALCHEMY_ABI_ITEM_MENU_DRAW_CATEGORY
#define ALCHEMY_ABI_ITEM_MENU_DRAW_CATEGORY

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_080a9cbc(void);
extern s32 Func_080a9aec(s32, void *);
extern s32 Func_080a9c18(void *);

#define Sys_Apply Func_080a9aec
#define Sys_Check Func_080a9c18
#define Sys_Run Func_080a9cbc
#define gIw Data_03001f2c

#endif

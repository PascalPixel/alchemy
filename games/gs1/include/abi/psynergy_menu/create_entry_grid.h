#ifndef ALCHEMY_ABI_PSYNERGY_MENU_CREATE_ENTRY_GRID
#define ALCHEMY_ABI_PSYNERGY_MENU_CREATE_ENTRY_GRID

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080a1814(void *menu);

#define Sys_Check Func_080a1814
#define Sys_Run Func_080a1778
#define Sys_Run2 Func_080150d8
#define Sys_Run3 Func_080150d0

#endif

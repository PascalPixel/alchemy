#ifndef ALCHEMY_ABI_PSYNERGY_MENU_REFRESH_OWNER_PSYNERGY
#define ALCHEMY_ABI_PSYNERGY_MENU_REFRESH_OWNER_PSYNERGY

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080a1bdc(s32 x, s32 y, s32 spacing);

#define Sys_Place Func_080a1bdc
#define gIw Data_03001f2c

#endif

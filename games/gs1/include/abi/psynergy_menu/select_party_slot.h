#ifndef ALCHEMY_ABI_PSYNERGY_MENU_SELECT_PARTY_SLOT
#define ALCHEMY_ABI_PSYNERGY_MENU_SELECT_PARTY_SLOT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080a1ac0(s32, s32);
s32 Func_080a68ec(void *, void *, s32);
s32 Func_080a60d4(void *, void *);
void Func_080a17c4(void *cursor);

#define PsynergyMenu_SelectPartySlot Func_080a602c
#define Sys_Apply Func_080a1ac0
#define Sys_Apply2 Func_080a60d4
#define Sys_Do Func_080a17c4
#define Sys_Place Func_080a68ec
#define Sys_Run Func_08077008
#define gIw Data_03001f2c

#endif

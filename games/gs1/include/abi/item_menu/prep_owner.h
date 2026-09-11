#ifndef ALCHEMY_ABI_ITEM_MENU_PREP_OWNER
#define ALCHEMY_ABI_ITEM_MENU_PREP_OWNER

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015278(s32);
void Func_080a1ac0(s32, s32);
s32 Func_080a35f8(void *, void *);
void Func_080a17c4(void *icon);

#define Sys_Apply Func_080a1ac0
#define Sys_Apply2 Func_080a35f8
#define Sys_Do Func_08015278
#define Sys_Do2 Func_080a17c4
#define gIw Data_03001f2c

#endif

#ifndef ALCHEMY_ABI_SHOP_SEL_PICK_UNIT
#define ALCHEMY_ABI_SHOP_SEL_PICK_UNIT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080a1028(s32, s32, s32, s32, s32);
void Func_080a1030(void);
void Func_080b1bd0(s32);
void Func_080b211c(s32);

#define Sys_Do Func_080b1bd0
#define Sys_Do2 Func_080b211c
#define Sys_Run Func_080a1030
#define Sys_SetRange Func_080a1028

#endif

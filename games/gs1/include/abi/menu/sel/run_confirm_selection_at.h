#ifndef ALCHEMY_ABI_MENU_SEL_RUN_CONFIRM_SELECTION_AT
#define ALCHEMY_ABI_MENU_SEL_RUN_CONFIRM_SELECTION_AT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080288a8(s32 a0, s32 a1, s32 a2, s32 a3);
s32 Func_08028574(s32);
void Func_0802851c(void);

#define Menu_Check Func_08028574
#define Menu_Run Func_080284dc
#define Menu_Run2 Func_0802851c
#define Menu_SetMode Func_080288a8
#define gVal Data_00000024

#endif

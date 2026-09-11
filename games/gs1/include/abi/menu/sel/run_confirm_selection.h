#ifndef ALCHEMY_ABI_MENU_SEL_RUN_CONFIRM_SELECTION
#define ALCHEMY_ABI_MENU_SEL_RUN_CONFIRM_SELECTION

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08028808(s32, s32, s32);
s32 Func_08028574(s32);
void Func_0802851c(void);

#define Menu_Check Func_08028574
#define Menu_Place Func_08028808
#define Menu_Run Func_080284dc
#define Menu_Run2 Func_0802851c

#endif

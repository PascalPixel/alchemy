#ifndef ALCHEMY_ABI_MENU_MISC_RUN_WORKSPACE_RESULT_LOOP
#define ALCHEMY_ABI_MENU_MISC_RUN_WORKSPACE_RESULT_LOOP

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0801db70(void);
s32 Func_080207c4(void);
void Func_0801776c(s32, s32);
s32 Func_0801d4cc(void);

#define Menu_Apply Func_0801776c
#define Menu_Check Func_0801db70
#define Menu_Check2 Func_080207c4
#define Menu_Check3 Func_0801d4cc

#endif

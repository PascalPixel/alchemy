#ifndef ALCHEMY_ABI_MENU_SEL_RUN_ACTION_FLOW
#define ALCHEMY_ABI_MENU_SEL_RUN_ACTION_FLOW

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080a77a4(s32 index);
s32 Func_080a8114(void);
s32 Func_080a90bc(void);
s32 Func_080a96d8(void);

#define Menu_Check Func_080a77a4
#define Menu_Check2 Func_080a8114
#define Menu_Check3 Func_080a90bc
#define Menu_Check4 Func_080a96d8
#define gIw Data_03001f2c

#endif

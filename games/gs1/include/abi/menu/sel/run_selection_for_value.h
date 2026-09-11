#ifndef ALCHEMY_ABI_MENU_SEL_RUN_SELECTION_FOR_VALUE
#define ALCHEMY_ABI_MENU_SEL_RUN_SELECTION_FOR_VALUE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0801a7f4(u32);
void Func_0801b228(void);
void Func_0801b010(u32, u32);
u32 Func_0801b424(u32);

#define Menu_Apply Func_0801b010
#define Menu_Do Func_0801a7f4
#define Menu_Do2 Func_0801b424
#define Menu_Run Func_0801b228
#define gIw Data_03001e98

#endif

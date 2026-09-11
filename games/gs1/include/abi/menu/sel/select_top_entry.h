#ifndef ALCHEMY_ABI_MENU_SEL_SELECT_TOP_ENTRY
#define ALCHEMY_ABI_MENU_SEL_SELECT_TOP_ENTRY

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077290(s32);
void Func_08028808(s32, s32, s32);
s32 Func_08028574(s32);
void Func_0802851c(void);

#define Menu_Check Func_08077290
#define Menu_Check2 Func_08028574
#define Menu_Place Func_08028808
#define Menu_Run Func_080284dc
#define Menu_Run2 Func_0802851c
#define gRom Data_08037403
#define gRom2 Data_080373f7

#endif

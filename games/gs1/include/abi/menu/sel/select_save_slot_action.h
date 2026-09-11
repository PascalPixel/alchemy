#ifndef ALCHEMY_ABI_MENU_SEL_SELECT_SAVE_SLOT_ACTION
#define ALCHEMY_ABI_MENU_SEL_SELECT_SAVE_SLOT_ACTION

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_0801f77c(void);
extern void Func_08028808(s32, s32, s32);
extern s32 Func_08028574(s32);
extern void Func_0802851c(void);

#define Menu_Check Func_0801f77c
#define Menu_Check2 Func_08028574
#define Menu_Place Func_08028808
#define Menu_Run Func_080284dc
#define Menu_Run2 Func_0802851c
#define gRom Data_0803740f

#endif

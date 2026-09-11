#ifndef ALCHEMY_ABI_MENU_SEL_SELECT_QUANTITY
#define ALCHEMY_ABI_MENU_SEL_SELECT_QUANTITY

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080a19a0(void);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08004278(void (*callback)(void));
void Func_080a22f4(void);
void Func_080030f8(s32);
s32 Func_080770c0(s32);
s32 Func_080022fc(s32, s32);
void Func_080a4924(s32, s32);
s32 Func_08015270(s32);
void Func_08015018(s32, s32);
void Func_080a2144(s32);
void Func_080041d8(const void *, s32);
void Func_08015408(s32, s32, s32, s32);

#define Menu_Apply Func_080022fc
#define Menu_Apply2 Func_080a4924
#define Menu_Apply3 Func_08015018
#define Menu_Apply4 Func_080041d8
#define Menu_Check Func_080770c0
#define Menu_Check2 Func_08015270
#define Menu_Do Func_08004278
#define Menu_Do2 Func_080030f8
#define Menu_Do3 Func_080a2144
#define Menu_Run Func_080a4800
#define Menu_Run2 Func_080a19a0
#define Menu_Run3 Func_080a22f4
#define Menu_SetMode Func_08015408
#define Menu_SetRange Func_08015010
#define gIw Data_03001f2c
#define gIw2 Data_03001b04
#define gIw3 Data_03001c94

#endif

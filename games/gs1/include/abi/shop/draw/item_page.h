#ifndef ALCHEMY_ABI_SHOP_DRAW_ITEM_PAGE
#define ALCHEMY_ABI_SHOP_DRAW_ITEM_PAGE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015068();
void Func_08015080();
void Func_08015090();
void Func_080150b0();
s32 Func_08077008();
void Func_080a21b0();
void Func_080a2324();

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

#define Sys_Check Func_08077008
#define Sys_Run2 Func_080a9598
#define Sys_Run3 Func_08015068
#define Sys_Run4 Func_080a2324
#define Sys_Run5 Func_080a21b0
#define Sys_Run6 Func_08015080
#define Sys_Run7 Func_08015090
#define Sys_Run8 Func_080150b0
#define gIw Data_03001f2c
#define gVal Data_00000182
#define gVal2 Data_00000af7
#define Sys_Run(a0, a1, a2, a3) Call4(Func_08015080, a0, a1, a2, a3)

#endif

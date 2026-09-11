#ifndef ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_STAT_COMPARISON
#define ALCHEMY_ABI_UI_TEXT_DRAW_DRAW_STAT_COMPARISON

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015080();
void Func_080150a8();
void Func_080ae99c();

static __inline__ void Call4(void (*f)(), s32 alt, s32 base, s32 work, s32 a3)
{
    f(alt, base, work, a3);
}

#define Ui_Run2 Func_080150a8
#define Ui_Run3 Func_080ae99c
#define Ui_Run4 Func_08015080
#define gVal Data_00000b20
#define Ui_Run(a0, a1, a2, a3) Call4(Func_08015080, a0, a1, a2, a3)

#endif

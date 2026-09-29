#include "TYPES.H"

s32 Engine_TaskAddCallback();

extern u8 ArutinYama_StepPaletteAnim;
extern u16 ArutinYama_PaletteStep;
extern u16 ArutinYama_PaletteHold;

struct Half {
    u16 v;
};

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void ArutinYama_StartPaletteAnim(void)
{
    struct Half zero;

    zero.v = 0;
    ArutinYama_PaletteStep = zero.v;
    ArutinYama_PaletteHold = zero.v;
    Value2(Engine_TaskAddCallback, (s32)&ArutinYama_StepPaletteAnim, 0xc80);
}

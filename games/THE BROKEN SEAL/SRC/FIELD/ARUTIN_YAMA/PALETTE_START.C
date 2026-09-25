#include "TYPES.H"

s32 Engine_TaskAddCallback();

extern u8 Value_0200b9c9;
extern u16 Data_0200d260;
extern u16 Data_0200d25c;

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
    Data_0200d260 = zero.v;
    Data_0200d25c = zero.v;
    Value2(Engine_TaskAddCallback, (s32)&Value_0200b9c9, 0xc80);
}

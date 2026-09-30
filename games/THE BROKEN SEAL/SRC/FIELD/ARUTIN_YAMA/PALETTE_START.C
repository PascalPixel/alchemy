#include "TYPES.H"

s32 Engine_TaskAddCallback();

extern u8 ArutinYama_StepPaletteAnim;
extern u16 ArutinYama_PaletteStep;
extern u16 ArutinYama_PaletteHold;

struct Half {
    u16 v;
};

void ArutinYama_StartPaletteAnim(void)
{
    struct Half zero;

    zero.v = 0;
    ArutinYama_PaletteStep = zero.v;
    ArutinYama_PaletteHold = zero.v;
    Engine_TaskAddCallback((s32)&ArutinYama_StepPaletteAnim, 0xc80);
}

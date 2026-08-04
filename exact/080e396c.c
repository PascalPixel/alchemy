#include "effect_step.h"

void Func_080b50b0(s32, struct EffectPosition *);

void Func_080e396c(s32 arg0, struct EffectPosition *position)
{
    Func_080b50b0(arg0, position);
    position->y = (s32)((u32)position->y - 0x10);
}

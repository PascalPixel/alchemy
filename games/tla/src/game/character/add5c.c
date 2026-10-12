#include "types.h"

void Func_080adcec();
void Func_080adc90();

void Func_080add5c(s32 count)
{
    s32 i;

    for (i = 0; i < count; i++)
        Func_080adcec();
    Func_080adc90();
}

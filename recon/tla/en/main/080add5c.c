#include "types.h"

void Func_080adcec();
void Func_080adc90();

void Func_080add5c(s32 count)
{
    s32 i;

    if (count > 0) {
        i = count;
        do {
            i--;
            Func_080adcec();
        } while (i != 0);
    }
    Func_080adc90();
}

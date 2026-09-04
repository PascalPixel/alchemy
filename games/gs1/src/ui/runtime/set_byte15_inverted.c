#include "types.h"

#define UiWork_SetByte15Inverted Func_0801eddc

void UiWork_SetByte15Inverted(void *arg0, s32 arg1)
{
    if (arg0 != 0)
        ((s8 *)arg0)[15] = ~arg1;
}

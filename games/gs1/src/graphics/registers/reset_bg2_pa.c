#include "types.h"

#define Graphics_ResetBg2Pa Func_080ec0e0

void Graphics_ResetBg2Pa(void)
{
    *(volatile u16 *)0x04000020 = 0x100;
}

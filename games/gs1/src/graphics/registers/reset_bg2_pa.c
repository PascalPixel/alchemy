#include "types.h"

void Graphics_ResetBg2Pa(void)
{
    *(volatile u16 *)0x04000020 = 0x100;
}

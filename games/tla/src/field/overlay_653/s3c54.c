#include "local.h"

void Func_02003c54(void)
{
    if (GameFlag_Test_(0x210) == 0) {
        GameFlag_SetBit_(0x210);
        Func_020053a8_(1, 0x210, 10, 11);
    }
    {
        /* FAKEMATCH: a temporary builds the angle ahead of the other arguments (three reordered rows as a plain argument). */
        s32 angle = 0x3000;

        Func_02005238_(12, 0x1880000, 0x1680000, angle);
    }
}

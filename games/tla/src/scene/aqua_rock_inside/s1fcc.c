#include "local.h"

void Func_02001fcc(void)
{
    s32 counter = Func_02003bcc();

    switch (counter)
    {
    case 0:
        Func_02003bb8(0, 1);
        Func_02003bb8(1, 1);
        Func_02003ecc(170);
        break;
    case 0x34bbf:
        Func_02003bb8(0, 0);
        Func_02003bb8(1, 0);
        Func_02003ecc(-1);
        break;
    }
    Func_02003bd8(0x34bc0);
}

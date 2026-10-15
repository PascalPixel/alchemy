#include "types.h"

s32  Random16Far();
void Func_020049c4();

s32 Func_02000ad0(void)
{
    switch (Random16Far() & 7)
    {
    case 0:
        Func_020049c4(126);
        break;
    case 1:
        Func_020049c4(212);
        break;
    case 2:
        Func_020049c4(134);
        break;
    case 3:
    case 5:
    case 6:
    default:
        Func_020049c4(133);
        break;
    case 4:
    case 7:
        Func_020049c4(154);
        break;
    }
    return 0;
}

s32 Func_02000b38(u8 *object)
{
    object[90] &= 0xfe;
    return 0;
}

#include "TYPES.H"

s32 BtlEfxDeadChk(s32 effect)
{
    switch (effect) {
    case 31:
    case 32:
    case 60:
    case 69:
        return 1;
    default:
        return 0;
    }
}

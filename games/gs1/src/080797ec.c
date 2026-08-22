#include "types.h"

extern s32 Data_08088db8[];

s32 Func_080797ec(s32 row, s32 column)
{
    return Data_08088db8[(row * 4) + column];
}

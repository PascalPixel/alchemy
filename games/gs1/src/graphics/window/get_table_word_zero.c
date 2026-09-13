#include "types.h"

extern s32 Data_08073968[];

s32 Ui_GetTableWordZero(s32 index)
{
    if (index != 0)
        index = 0;
    return Data_08073968[index];
}

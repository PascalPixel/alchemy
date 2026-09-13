#include "types.h"

extern s16 Data_080b41ac[][33];

s32 EventTable_GetRowType(s32 index)
{
    return Data_080b41ac[index][32];
}

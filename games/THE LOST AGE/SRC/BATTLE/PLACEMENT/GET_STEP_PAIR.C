#include "TYPES.H"

extern const s8 Data_08128844[];

void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y)
{
    index *= 2;
    *x = Data_08128844[index];
    *y = Data_08128844[index + 1];
}

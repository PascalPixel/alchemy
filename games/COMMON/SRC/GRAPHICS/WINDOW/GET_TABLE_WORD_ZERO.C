#include "TYPES.H"

extern s32 Ui_FixedTileBlocks[];

s32 Ui_GetTableWordZero(s32 index)
{
    if (index != 0)
        index = 0;
    return Ui_FixedTileBlocks[index];
}

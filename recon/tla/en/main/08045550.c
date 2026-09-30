/*
 * Draft: Ui_GetTableWordZero does not yet match; 2 halfwords differ from ☀️'s C, first at +0x12 (data).
 * Links as recon/tla/raw/08045528.s.
 */
#include "TYPES.H"

extern s32 Data_08073968[];

s32 Ui_GetTableWordZero(s32 index)
{
    if (index != 0)
        index = 0;
    return Data_08073968[index];
}

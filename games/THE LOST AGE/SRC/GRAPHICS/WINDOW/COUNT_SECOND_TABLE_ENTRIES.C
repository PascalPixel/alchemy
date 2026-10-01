#include "TYPES.H"
extern u8 Data_08054e24[];
extern u8 UiIcon_PsynergyIconPointers[];
s32 Ui_CountSecondTableEntries(void)
{
    return (s32)((u32)Data_08054e24 - (u32)UiIcon_PsynergyIconPointers) >> 2;
}

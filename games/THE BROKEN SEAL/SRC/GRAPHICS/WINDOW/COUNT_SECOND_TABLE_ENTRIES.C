#include "TYPES.H"

extern u8 UiIcon_PsynergyIconPointersEnd[];
extern u8 UiIcon_PsynergyIconPointers[];

s32 Ui_CountSecondTableEntries(void)
{
    return (UiIcon_PsynergyIconPointersEnd - UiIcon_PsynergyIconPointers) >> 2;
}

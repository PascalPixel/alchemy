#include "TYPES.H"

extern u8 UiIcon_ItemIconPointersEnd[];
extern u8 UiIcon_ItemIconPointers[];

s32 Ui_CountIconTableEntries(void)
{
    return (UiIcon_ItemIconPointersEnd - UiIcon_ItemIconPointers) >> 2;
}

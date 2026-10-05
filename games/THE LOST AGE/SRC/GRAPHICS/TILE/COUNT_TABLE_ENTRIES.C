#include "TYPES.H"

extern u8 UiIcon_ItemIconPointersEnd[];
extern u8 UiIcon_ItemIconPointers[];

/* The number of item icons: one word pointer each up to the table's end. */
s32 Ui_CountIconTableEntries(void)
{
    return (UiIcon_ItemIconPointersEnd - UiIcon_ItemIconPointers) >> 2;
}

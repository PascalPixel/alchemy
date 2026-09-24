#include "TYPES.H"

extern u8 UiIcon_ItemIconPointersEnd[];
extern u8 UiIcon_ItemIconPointers[];

/* 2つの境界アドレス間を4バイト単位で数える。 */
s32 Ui_CountIconTableEntries(void)
{
    return (s32)((u32)UiIcon_ItemIconPointersEnd - (u32)UiIcon_ItemIconPointers) >> 2;
}

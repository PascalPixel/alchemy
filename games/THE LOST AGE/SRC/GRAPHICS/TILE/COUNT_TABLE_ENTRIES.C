#include "GLOBAL.H"
#include "TYPES.H"


s32 Ui_CountIconTableEntries(void)
{
    return (UiIcon_ItemIconPointersEnd - UiIcon_ItemIconPointers) >> 2;
}

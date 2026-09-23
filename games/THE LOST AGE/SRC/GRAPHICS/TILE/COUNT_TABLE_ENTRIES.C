#include "TYPES.H"
extern u8 Data_0804f124[];
extern u8 Data_0804eb58[];
s32 Ui_CountIconTableEntries(void)
{
    return (s32)((u32)Data_0804f124 - (u32)Data_0804eb58) >> 2;
}

#include "TYPES.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001f2c[];

s32 ShopCursor_Advance(s32);

void Shop_StepCursor(void)
{
    ShopCursor_Advance(*(s32 *)((u32)&Data_03001f2c) + 0x380);
}

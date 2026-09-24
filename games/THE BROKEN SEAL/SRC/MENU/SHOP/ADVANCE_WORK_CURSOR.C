#include "TYPES.H"
#include "GLOBAL_CELLS.H"

s32 ShopCursor_Advance(s32);

void Shop_StepCursor(void)
{
    ShopCursor_Advance(*(s32 *)ADDR_03001F2C + 0x380);
}

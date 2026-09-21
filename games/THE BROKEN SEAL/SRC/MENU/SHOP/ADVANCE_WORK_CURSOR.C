#include "TYPES.H"
#include "GLOBAL_CELLS.H"

s32 Func_080b08b8(s32);
#define ShopCursor_Advance Func_080b08b8

void Shop_StepCursor(void)
{
    ShopCursor_Advance(*(s32 *)ADDR_03001F2C + 0x380);
}

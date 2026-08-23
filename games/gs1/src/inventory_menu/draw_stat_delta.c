#include "inventory_menu.h"

extern u8 Data_080af224;
extern u8 Data_080af228;
#define InventoryMenu_IncreaseGlyph Data_080af224
#define InventoryMenu_DecreaseGlyph Data_080af228

void UiNumber_DrawAt(s32, s32, s32, s32, s32);
void Func_08015098(u8 *, s32, s32, s32);

void InventoryMenu_DrawStatDelta(
    s32 delta, s32 unused, s32 window, s32 x, s32 y)
{
    s32 digits;
    s32 magnitude;

    UiNumber_DrawAt(delta, 3, window, x, y);
    digits = 1;
    magnitude = delta;
    if (delta < 0) {
        magnitude = -delta;
    }
    if (magnitude > 9) {
        digits = 2;
    }
    magnitude = delta;
    if (delta < 0) {
        magnitude = -delta;
    }
    if (magnitude > 99) {
        digits = 3;
    }
    if (delta > 0) {
        Func_08015098(
            &InventoryMenu_IncreaseGlyph, window, x - digits * 8 + 16, y);
    } else {
        Func_08015098(
            &InventoryMenu_DecreaseGlyph, window, x - digits * 8 + 16, y);
    }
}

#include "scene.h"
#include "abi/item_menu/draw/stat_delta.h"
#include "item_menu.h"

extern u8 ItemMenu_IncreaseGlyph;
extern u8 ItemMenu_DecreaseGlyph;

void UiNumber_DrawAt(s32, s32, s32, s32, s32);

void ItemMenu_DrawStat(
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
        Sys_SetMode(
            &ItemMenu_IncreaseGlyph, window, x - digits * 8 + 16, y);
    } else {
        Sys_SetMode(
            &ItemMenu_DecreaseGlyph, window, x - digits * 8 + 16, y);
    }
}

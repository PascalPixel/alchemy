#include "shop.h"
#include "battle_runtime.h"

extern u8 Value_00000c91;

void UiWindow_Clear(s32 window);
s32 Func_08077248(s32 unit_id);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
u8 *UiIcon_Draw(u16 no, s32 kind, s32 window, s32 x, s32 y);

void Shop_DrawPartyMemberItemGrid(s32 window, s32 unit_id)
{
    u8 *unit;
    s32 x;
    s32 y;
    s32 slot;
    s32 item_offset;
    u8 *icon;

    unit = (u8 *)BattleUnit_Get(unit_id);
    x = 8;
    y = 0;
    if (window != 0) {
        UiWindow_Clear(window);
        if (Func_08077248(unit_id) == 0) {
            UiText_DrawMessageAt((s32)&Value_00000c91, window, 8, 20);
        } else {
            slot = 0;
            item_offset = 216;
            do {
                if (*(u16 *)((u32)item_offset + (u32)unit) != 0) {
                    icon = UiIcon_Draw(
                        *(u16 *)((u32)item_offset + (u32)unit),
                        27, window, x, y);
                    icon[15] = 252;
                }
                x += 16;
                if (slot == 4) {
                    x = 8;
                    y += 16;
                }
                if (slot == 9) {
                    x = 8;
                    y += 16;
                }
                slot++;
                item_offset += 2;
            } while (slot <= 14);
        }
    }
}

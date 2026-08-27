#include "types.h"
#include "global_cells.h"
extern u8 *Data_03001f2c;
void WindowBounds_Set(s32, s32, s32, s32, s32);
void UiWindow_Commit(s32);
s32 Runtime_GetObject(s32);
void ScheduleCallbackAfterFrames(s32, s32);
s32 Modulo(s32, s32);
s32 Func_080a3d6c(s32);
s32 Func_080a3d9c(s32, s32);
void Func_08015070(s32, s32, s32, s32, s32);
void Func_08015068(s32, s32, s32, s32, s32);
void Func_080150b0(s32, s32, s32, s32, s32);
void UiText_DrawAt(s32, s32, s32, s32);
void Func_080a3ef0(s32, s32, s32, s32, s32);
void Audio_PlayCue(s32);
s32 GameFlag_IsSet(s32);
void Func_080a1a40(s32, s32);
void WaitFrames(s32);

s32 Func_080a38d0(s32 category)
{
    u8 *menu;
    s32 item_window;
    u8 owner;
    s8 slot_count;
    s32 changed;
    s32 abort_flag;
    s32 cursor;
    u8 selected_slot;
    s32 quantity;
    s32 mode_row;
    s32 result_slot;

    menu = Data_03001f2c;
    item_window = *(s32 *)(menu + 0x20);
    owner = menu[0x21b];
    slot_count = *(s8 *)(menu + 29);
    changed = 0;
    abort_flag = 0;

    WindowBounds_Set(item_window, 5, 13, 17, 2);
    UiWindow_Commit(item_window);
    Runtime_GetObject(*(u16 *)(menu + 0x1c8 + slot_count * 2));
    ScheduleCallbackAfterFrames(0x080a3c09, 0xc80);

    for (;;) {
        if (changed != 0) {
            changed = 0;
            selected_slot = (u8)Modulo(slot_count + owner, owner);
            cursor = selected_slot * 2;

            Func_08015070(item_window, 0, 9, 16, 9);
            Func_080150b0(item_window, 80, 0, 72, 120);

            if (selected_slot != menu[0x21b]) {
                quantity = Func_080a3d9c(
                    owner, *(u16 *)(menu + 0x1c8 + cursor));
                if (quantity != 0) {
                    UiText_DrawAt(24, item_window, 2, 8);
                } else {
                    UiText_DrawAt(16, item_window, 2, 8);
                }

                mode_row = Func_080a3d6c(
                    *(u16 *)(menu + 0x1c8 + cursor));
                if (mode_row == 15 && quantity == 0) {
                    UiText_DrawAt(0, item_window, 2, 8);
                }
            }

            Func_080a3ef0(
                menu[0x21a],
                *(u16 *)(menu + 0x1c8 + cursor),
                *(u16 *)(menu + 0x1c8 + cursor),
                0,
                0);
        }

        if (!GameFlag_IsSet(0x150) && !abort_flag) {
            UiWindow_Commit(item_window);
            abort_flag = 1;
        }

        Func_080a1a40(cursor * 8 - 10, 16);
        WaitFrames(1);

        if (*(volatile u32 *)ADDR_03001C94 & 1) {
            Audio_PlayCue(112);
            result_slot = *(u16 *)(menu + 0x1c8 + cursor);
        } else if (*(volatile u32 *)ADDR_03001C94 & 2) {
            Audio_PlayCue(113);
            result_slot = -1;
        } else if (*(volatile u32 *)ADDR_03001B04 & 0x20) {
            Audio_PlayCue(111);
            slot_count -= 1;
            changed = 1;
        } else if (*(volatile u32 *)ADDR_03001B04 & 0x10) {
            Audio_PlayCue(111);
            slot_count += 1;
            changed = 1;
        } else {
            continue;
        }
        break;
    }

    menu[29] = (u8)selected_slot;
    WaitFrames(1);
    menu[29] = (u8)selected_slot;
    *(s32 *)(menu + 8) = *(u16 *)(menu + 0x1c8 + cursor);
    menu[0x21b] = *(u16 *)(menu + 0x1c8 + cursor);
    return result_slot;
}

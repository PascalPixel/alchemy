#include "types.h"
#include "inventory_menu.h"
#include "equipment_menu.h"
#include "global_cells.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08015070(s32 window, s32 x, s32 width, s32 height, s32 style);
void Func_080a1ac0(s32 x, s32 y);
void Func_080a1a40(s32 x, s32 y);
void Func_080a3ef0(s32 owner, s32 slot, s32 unused);
void Func_080a112c(s32 window, s32 owner, s32 unused1, s32 unused2);

extern u8 Value_00000075;

/*
 * Item command menu reached after selecting an item slot: builds the 3x2
 * command-availability grid (Use/Equip/... at command_states[0..5]), lets
 * the player move a cursor over it with the d-pad, and returns the chosen
 * command index (0-5) or -1 if cancelled. Called from main:080a2680 (not
 * yet recovered), which treats -1 as "no command chosen".
 */
s32 Func_080a414c(void)
{
    struct InventoryMenuState *menu = Data_03001f2c;
    s8 command_states[8];
    u16 *redraw_flag;
    s32 saved;
    s32 col;
    s32 row;
    s32 index;
    s32 need_redraw;
    s32 x;
    s32 y;

    index = 0;
    row = 0;
    need_redraw = 1;

    InventoryMenu_BuildCommandStates(command_states);
    redraw_flag = (u16 *)((u8 *)menu + 0x220);
    col = 0;

    if (*redraw_flag != 1) {
        s32 message_window;

        InventoryMenu_HideAllItemIcons();
        UiWindow_Commit(FIELD(menu, s32 *, 0x34));
        message_window = menu->message_window;
        InventoryMenu_SetMessageWindowRow7Bounds();
        UiWindow_Commit(message_window);
        Func_08015070(message_window, 0, 3, 0x10, 3);
        InventoryMenu_DrawSelectedItemHeader();
        InventoryMenu_DrawCommandLabels(command_states, message_window);
        UiWindow_Commit(menu->info_window);
        UiText_DrawAt(
            (menu->selected_item & 0x1ff) + (s32)&Value_00000075,
            menu->info_window,
            0,
            0);
    }
    *redraw_flag = 0;

    saved = FIELD(menu, s8 *, 0x25d);
    if (saved == -1) {
        if (command_states[2] == 1) {
            col = 2;
            row = 0;
        }
        if (command_states[3] == 1) {
            col = 0;
            row = 1;
        }
        if (command_states[1] == 1) {
            col = 1;
            row = 0;
        }
        if (command_states[4] == 1) {
            col = 1;
            row = 1;
        }
        if (command_states[0] == 1) {
            col = 0;
            row = 0;
        }
    } else {
        col = (s8)Modulo(saved, 3);
        row = (s8)FixedPoint_Ratio(saved, 3);
        index = row * 3 + col;
    }

    x = InventoryMenu_GetCommandCursorX(col, row);
    y = InventoryMenu_GetCommandCursorY(col, row);
    Func_080a1ac0(x, y);

    for (;;) {
        if (GameFlag_IsSet(0x150) != 0)
            break;

        if (need_redraw != 0) {
            need_redraw = 0;
            col = Modulo(col + 3, 3);
            row = (row + 2) % 2;
            index = row * 3 + col;
            EquipmentMenu_StartCompatibilityIndicators();
            if (index > 2) {
                FIELD(menu, s8 *, 0x25c) = 1;
                Func_080a3ef0(menu->item_owner, menu->selected_slot, 0);
                if (index == 3) {
                    ScheduleCallbackAfterFrames(
                        &EquipmentMenu_CompatibilityUpdateEntry, 0xc80);
                }
            } else if (index != 0) {
                FIELD(menu, s8 *, 0x25c) = 0;
                Func_080a3ef0(menu->item_owner, menu->selected_slot, 0);
            } else {
                Func_080a112c(
                    FIELD(menu, s32 *, 0x24), menu->item_owner, 0, 0);
            }
        }

        x = InventoryMenu_GetCommandCursorX(col, row);
        y = InventoryMenu_GetCommandCursorY(col, row);
        Func_080a1a40(x, y);
        WaitFrames(1);

        if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0) {
            if (command_states[index] == -1) {
                Audio_PlayCue(114);
            } else {
                switch (index) {
                case 0:
                    Audio_PlayCue(174);
                    break;
                case 1:
                    Audio_PlayCue(175);
                    break;
                case 2:
                case 3:
                case 5:
                    Audio_PlayCue(112);
                    break;
                case 4:
                    Audio_PlayCue(117);
                    break;
                default:
                    Audio_PlayCue(112);
                    break;
                }
                FIELD(menu, s8 *, 0x25d) = (s8)index;
                break;
            }
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0) {
            Audio_PlayCue(113);
            index = -1;
            FIELD(menu, s8 *, 0x25d) = (s8)index;
            break;
        }

        if ((*(volatile u32 *)ADDR_03001B04 & 0x40) != 0) {
            row -= 1;
            need_redraw = 1;
            Audio_PlayCue(111);
        } else if ((*(volatile u32 *)ADDR_03001B04 & 0x80) != 0) {
            row += 1;
            need_redraw = 1;
            Audio_PlayCue(111);
        } else if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0) {
            col += 1;
            need_redraw = 1;
            Audio_PlayCue(111);
        } else if ((*(volatile u32 *)ADDR_03001B04 & 0x20) != 0) {
            col -= 1;
            need_redraw = 1;
            Audio_PlayCue(111);
        }
    }

    FIELD(menu, s8 *, 0x25c) = 0;
    EquipmentMenu_StartCompatibilityIndicators();
    return index;
}

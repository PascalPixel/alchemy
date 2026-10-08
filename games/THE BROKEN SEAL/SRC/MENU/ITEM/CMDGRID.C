#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "INVENTORY_MENU.H"
#include "SYSTEM.H"
#include "UI.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EQUIPMENT_MENU.H"
#include "BTLSYS.H"
#include "text/MSG_IDS.H"

#define RenderOutput_RedrawSavedRectFar ((void (*)(s32))RenderOutput_RedrawSavedRectFar)
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)
#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 width, s32 height, s32 style);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused1, s32 unused2);
void AudioCommand_PlayFar(s32 cue);


#if EDITION_INTERNATIONAL

s32 ItemMenu_CmdCursorX(s32 column, s32 row)
{
    if (column > 2 || row > 2 || column < 0 || row < 0)
        return 0;
    return ItemMenu_CommandColumnXTable[row * 3 + column];
}

s32 ItemMenu_CmdCursorY(s32 column, s32 row)
{
    s32 y;

    (void)column;
    y = 0x1E;
    if (row != 0) {
        y = 0x26;
    }
    return y;
}
#endif

s32 ItemMenu_ChooseCmd(void)
{
    struct InventoryMenuState *menu;
    s8 command_storage[6];
    u16 *redraw_flag;
    s32 col;
    s32 saved;
    s32 row;
    s32 index;
    s32 need_redraw;
    s8 *command_states;
    s32 x;
    s32 y;

    index = 0;
    col = 0;
    row = 0;
    menu = gMenuWork;
    need_redraw = 1;
    command_states = command_storage;
    ItemMenu_BuildCmd(command_states);
    redraw_flag = &menu->flags;
    if (*redraw_flag != 1) {
        s32 message_window;

        ItemMenu_HideAllIcons();
        RenderOutput_RedrawSavedRectFar((s32)menu->list_window);
        message_window = (s32)menu->message_window;
        ItemMenu_SetMsgWin7();
        RenderOutput_RedrawSavedRectFar(message_window);
        UiWindow_DrawDividerLineFar(message_window, 0, 3, 0x10, 3);
        ItemMenu_DrawItemHead();
        ItemMenu_DrawCmd(command_states, message_window);
#if EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
        UiText_DrawCharacterAtOffsetFar(
            (menu->selected_items[0] & 0x1ff) + (s32)MsgItemPlainName,
            (s32)menu->info_window, 0, 0);
#else
        RenderOutput_ClearListFar((s32)menu->info_window);
        UiText_DrawResourceFar(
            (menu->selected_items[0] & 0x1ff) + (s32)MsgItemPlainName,
            (s32)menu->info_window, 0, 0);
#endif
    }
    *redraw_flag = 0;

    saved = (s8)menu->list_mode;
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
        col = (s8)(saved % 3);
        row = (s8)(saved / 3);
        index = row * 3 + col;
    }
#if EDITION_INTERNATIONAL
    x = ItemMenu_CmdCursorX(col, row);
    y = ItemMenu_CmdCursorY(col, row);
    UiMenu_SlideCursor(x, y);
#else
    UiMenu_SlideCursor(col * 40 + 86, row * 8 + 30);
#endif

    for (;;) {
        if (GameFlag_TestFar(0x150) != 0)
            break;
        if (need_redraw != 0) {
            need_redraw = 0;
            col = (col + 3) % 3;
            row = (row + 2) % 2;
            index = row * 3 + col;
            EquipmentMenu_StartCompatibilityIndicators();
            if (index > 2) {
                menu->equip_preview = 1;
                ItemMenu_DrawEquipPreview(menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
                if (index == 3) {
                    Scheduler_AddOrUpdateCallback(&EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
                }
            } else if (index != 0) {
                menu->equip_preview = 0;
                ItemMenu_DrawEquipPreview(menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
            } else {
                Menu_DrawOwnerStatusPanel((s32)menu->status_window, menu->pane_owner[0], 0, 0);
            }
        }
#if EDITION_INTERNATIONAL
        x = ItemMenu_CmdCursorX(col, row);
        y = ItemMenu_CmdCursorY(col, row);
        UiMenu_PositionCursor(x, y);
#else
        UiMenu_PositionCursor(col * 40 + 86, row * 8 + 30);
#endif
        WaitFrames(1);

        if ((gKeyState & KEY_A) != 0) {
            if (command_states[index] == -1) {
                AudioCommand_PlayFar(114);
            } else {
                switch (index) {
                case 0:
                    AudioCommand_PlayFar(174);
                    break;
                case 1:
                    AudioCommand_PlayFar(175);
                    break;
                case 2:
                case 3:
                case 5:
                    AudioCommand_PlayFar(112);
                    break;
                case 4:
                    AudioCommand_PlayFar(117);
                    break;
                default:
                    AudioCommand_PlayFar(112);
                    break;
                }
                menu->list_mode = index;
                break;
            }
        }
        if ((gKeyState & KEY_B) != 0) {
            AudioCommand_PlayFar(113);
            index = -1;
            menu->list_mode = index;
            break;
        }

        if ((gKeysRepeat & KEY_UP) != 0) {
            row -= 1;
            need_redraw = 1;
            AudioCommand_PlayFar(111);
        } else if ((gKeysRepeat & KEY_DOWN) != 0) {
            row += 1;
            need_redraw = 1;
            AudioCommand_PlayFar(111);
        } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
            col += 1;
            need_redraw = 1;
            AudioCommand_PlayFar(111);
        } else if ((gKeysRepeat & KEY_LEFT) != 0) {
            col -= 1;
            need_redraw = 1;
            AudioCommand_PlayFar(111);
        }
    }

    menu->equip_preview = 0;
    EquipmentMenu_StartCompatibilityIndicators();
    return index;
}


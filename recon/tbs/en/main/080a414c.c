/*
 * Draft, 828 of 832 bytes, 400 halfword edits from the prologue: the
 * reference builds the command-state address as movs #8 / add sp into r1,
 * which also forces a second zero for the row, and the register choices
 * cascade from there. ItemMenu_DrawEquipPreview takes four arguments; the caller
 * passes the owner twice.
 */
#include "TYPES.H"
#include "INVENTORY_MENU.H"
#include "SYSTEM.H"
#include "UI.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EQUIPMENT_MENU.H"


#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 width, s32 height, s32 style);
void UiMenu_SlideCursor(s32 x, s32 y);
void UiMenu_PositionCursor(s32 x, s32 y);
void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 arg3);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused1, s32 unused2);

extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001b04;
void RenderOutput_RedrawSavedRectFar(s32 window);
s32 GameFlag_TestFar(s32 flag);
void Audio_PlayCue(s32 cue);

extern u8 Value_00000075;

/* H1 (2026-09-26): complete [080a414c,080a448c), 832 bytes.
 * Exact COMMANDS.C, cursor helpers, WINDOW.C and DRAW_SELECTED_ITEM_HEADER.C
 * establish void/s32 interfaces; the old draft implicitly declared several
 * void callees as int. Transfer INVENTORY_MENU.H plus named menu/key globals,
 * canonical window/math/scheduler interfaces. Predict original argument
 * setup, saved-register roles and 16-byte frame without a stack-layout hack.
 * Gate: exact full extent/pools then compare/test/coverage/verify. Budget:
 * corrected model plus two structural variants, 25 minutes; record every
 * result here and in Git. Diagnostics: full aligned diff and allocator dump
 * if the frame/record address remains wrong; no spelling permutations.
 * H1: 828/832 bytes, 400 differing halfwords, 76 aligned edits; the audited
 * interfaces do not change the old output. All body register roles agree;
 * the stack array address is add r3,sp,#8 instead of mov r1,#8/add r1,sp,
 * and the two initial zeros merge. The rest is mostly the four-byte shift.
 * H2: exact BuildCmd/DrawCmd and the switch have six command entries, not
 * eight. Use the six-byte signed array and test whether its true stack-slot
 * boundary emits the reference's split address construction and two zeros.
 * H2: unchanged 828/832 bytes, 400 differing halfwords, 76 aligned edits.
 * Six entries and eight entries have the same rounded stack allocation;
 * changing the object extent does not change its address construction.
 * H3: the reload dump already has the split #8/add-sp address, but shares
 * r1=0 between index and row; final emission combines the adjacent address
 * pair. Give the command pointer an explicit lifetime between those scalar
 * initialization phases. Predict distinct zero reloads and split address.
 * H3: 832/832 bytes, 275 differing halfwords, 67 aligned edits. The explicit
 * pointer keeps split address construction, but in r2, and the index/row
 * zeros still share r1. An alignment halfword precedes the switch table.
 * STOP: corrected model and two variants exhausted. Preserve the new address
 * fact; do not extend this into initialization-order/register permutations.
 */
/*
 * Item command menu reached after selecting an item slot: builds the 3x2
 * command-availability grid (Use/Equip/... at command_states[0..5]), lets
 * the player move a cursor over it with the d-pad, and returns the chosen
 * command index (0-5) or -1 if cancelled. Called from exact ItemMenu_RunCommands (main:080a2680), which treats -1 as "no command chosen".
 */
s32 Func_080a414c(void)
{
    struct InventoryMenuState *menu;
    s8 command_storage[6];
    s8 *command_states;
    u16 *redraw_flag;
    s32 saved;
    s32 col;
    s32 row;
    s32 index;
    s32 need_redraw;
    s32 x;
    s32 y;

    index = 0;
    /* FAKEMATCH: bind the command buffer between scalar initialization
     * phases to preserve the reference's stack-address and zero lifetimes. */
    command_states = command_storage;
    menu = gMenuWork;
    row = 0;
    need_redraw = 1;

    ItemMenu_BuildCmd(command_states);
    redraw_flag = (u16 *)((u8 *)menu + 0x220);
    col = 0;

    if (*redraw_flag != 1) {
        s32 message_window;

        ItemMenu_HideAllIcons();
        RenderOutput_RedrawSavedRectFar(FIELD(menu, s32 *, 0x34));
        message_window = menu->message_window;
        ItemMenu_SetMsgWin7();
        RenderOutput_RedrawSavedRectFar(message_window);
        UiWindow_DrawDividerLineFar(message_window, 0, 3, 0x10, 3);
        ItemMenu_DrawItemHead();
        ItemMenu_DrawCmd(command_states, message_window);
        RenderOutput_RedrawSavedRectFar(menu->info_window);
        UiText_DrawCharacterAtOffsetFar(
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
        col = (s8)Math_Mod(saved, 3);
        row = (s8)Math_Div(saved, 3);
        index = row * 3 + col;
    }

    x = ItemMenu_CmdCursorX(col, row);
    y = ItemMenu_CmdCursorY(col, row);
    UiMenu_SlideCursor(x, y);

    for (;;) {
        if (GameFlag_TestFar(0x150) != 0)
            break;

        if (need_redraw != 0) {
            need_redraw = 0;
            col = Math_Mod(col + 3, 3);
            row = (row + 2) % 2;
            index = row * 3 + col;
            EquipmentMenu_StartCompatibilityIndicators();
            if (index > 2) {
                FIELD(menu, s8 *, 0x25c) = 1;
                ItemMenu_DrawEquipPreview(menu->item_owner, menu->selected_slot, 0, menu->item_owner);
                if (index == 3) {
                    Scheduler_AddOrUpdateCallback(
                        (s32)&EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
                }
            } else if (index != 0) {
                FIELD(menu, s8 *, 0x25c) = 0;
                ItemMenu_DrawEquipPreview(menu->item_owner, menu->selected_slot, 0, menu->item_owner);
            } else {
                Menu_DrawOwnerStatusPanel(
                    FIELD(menu, s32 *, 0x24), menu->item_owner, 0, 0);
            }
        }

        x = ItemMenu_CmdCursorX(col, row);
        y = ItemMenu_CmdCursorY(col, row);
        UiMenu_PositionCursor(x, y);
        WaitFrames(1);

        if ((Data_03001c94 & 1) != 0) {
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

        if ((Data_03001c94 & 2) != 0) {
            Audio_PlayCue(113);
            index = -1;
            FIELD(menu, s8 *, 0x25d) = (s8)index;
            break;
        }

        if ((Data_03001b04 & 0x40) != 0) {
            row -= 1;
            need_redraw = 1;
            Audio_PlayCue(111);
        } else if ((Data_03001b04 & 0x80) != 0) {
            row += 1;
            need_redraw = 1;
            Audio_PlayCue(111);
        } else if ((Data_03001b04 & 0x10) != 0) {
            col += 1;
            need_redraw = 1;
            Audio_PlayCue(111);
        } else if ((Data_03001b04 & 0x20) != 0) {
            col -= 1;
            need_redraw = 1;
            Audio_PlayCue(111);
        }
    }

    FIELD(menu, s8 *, 0x25c) = 0;
    EquipmentMenu_StartCompatibilityIndicators();
    return index;
}

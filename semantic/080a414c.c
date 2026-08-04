#include "layout_guard.h"
#include "types.h"

typedef struct MenuState_080a414c {
    u8 padding000[0x24];
    s32 action_window;
    u8 padding028[4];
    s32 description_window;
    u8 padding030[4];
    s32 status_window;
    u8 padding038[0xd4];
    s32 popup_window;
    u8 padding110[0x64];
    u16 item_id;
    u8 padding176[2];
    u16 subject_id;
    u8 padding17a[0xa0];
    u8 selected_character;
    u8 padding21b[5];
    u16 setup_state;
    u8 padding222[0x3a];
    s8 detail_mode;
    s8 saved_selection;
} MenuState_080a414c;

LAYOUT_OFFSET_GUARD(
    MenuState080a414c_ActionWindow,
    MenuState_080a414c,
    action_window,
    0x24);
LAYOUT_OFFSET_GUARD(
    MenuState080a414c_PopupWindow,
    MenuState_080a414c,
    popup_window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    MenuState080a414c_ItemId,
    MenuState_080a414c,
    item_id,
    0x174);
LAYOUT_OFFSET_GUARD(
    MenuState080a414c_SetupState,
    MenuState_080a414c,
    setup_state,
    0x220);
LAYOUT_OFFSET_GUARD(
    MenuState080a414c_SavedSelection,
    MenuState_080a414c,
    saved_selection,
    0x25d);

extern MenuState_080a414c *Data_03001f2c;
extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
void Func_080041d8(u32, s32);
void Func_08015070(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015270(s32);
s32 Func_080770c0(s32);
void Func_080a112c(s32, u8, s32, s32);
void Func_080a1a40(s32, s32);
void Func_080a1ac0(s32, s32);
void Func_080a345c(void);
void Func_080a3c98(void);
void Func_080a3ef0(u8, u16, s32);
s32 Func_080a4110(s8, s8);
s32 Func_080a413c(s8, s8);
void Func_080a448c(s8 *);
void Func_080a45cc(s8 *, s32);
void Func_080a4eb8(void);
void Func_080a51d0(void);
void Func_080f9010(s32);

/*
 * Select one of six action panels arranged as a three-column, two-row grid.
 * Each panel can be unavailable, and the saved selection is restored when
 * possible before wrapped directional navigation begins.
 */
s32 Func_080a414c(void)
{
    MenuState_080a414c *state = Data_03001f2c;
    s8 availability[8];
    s8 column = 0;
    s8 row = 0;
    s32 selection = 0;
    s32 redraw = 1;

    Func_080a448c(availability);
    if (state->setup_state != 1) {
        Func_080a345c();
        Func_08015270(state->status_window);
        Func_080a4eb8();
        Func_08015270(state->popup_window);
        Func_08015070(state->popup_window, 0, 3, 16, 3);
        Func_080a51d0();
        Func_080a45cc(availability, state->popup_window);
        Func_08015270(state->description_window);
        Func_08015080(
            (state->subject_id & 0x01ff) + 0x75,
            state->description_window,
            0,
            0);
    }
    state->setup_state = 0;

    if (state->saved_selection == -1) {
        if (availability[2] == 1) {
            column = 2;
            row = 0;
        }
        if (availability[3] == 1) {
            column = 0;
            row = 1;
        }
        if (availability[1] == 1) {
            column = 1;
            row = 0;
        }
        if (availability[4] == 1) {
            column = 1;
            row = 1;
        }
        if (availability[0] == 1) {
            column = 0;
            row = 0;
        }
    } else {
        column = (s8)Func_080022fc(state->saved_selection, 3);
        row = (s8)Func_080022ec(state->saved_selection, 3);
        selection = row * 3 + column;
    }

    Func_080a1ac0(
        Func_080a4110(column, row),
        Func_080a413c(column, row));

    for (;;) {
        s32 menu_closed = Func_080770c0(0x150);

        if (menu_closed != 0)
            break;

        if (redraw != 0) {
            s32 wrapped_row = row + 2;

            redraw = 0;
            row = (s8)(
                wrapped_row -
                ((wrapped_row + (wrapped_row >> 31)) >> 1) * 2);
            column = (s8)Func_080022fc(column + 3, 3);
            selection = row * 3 + column;
            Func_080a3c98();

            if (selection > 2) {
                state->detail_mode = 1;
                Func_080a3ef0(
                    state->selected_character,
                    state->item_id,
                    0);
                if (selection == 3)
                    Func_080041d8(0x080a3c09, 0x0c80);
            } else if (selection != 0) {
                state->detail_mode = (s8)menu_closed;
                Func_080a3ef0(
                    state->selected_character,
                    state->item_id,
                    0);
            } else {
                Func_080a112c(
                    state->action_window,
                    state->selected_character,
                    0,
                    0);
            }
        }

        Func_080a1a40(
            Func_080a4110(column, row),
            Func_080a413c(column, row));
        Func_080030f8(1);

        if ((Data_03001c94 & 1) != 0) {
            if (availability[selection] == -1) {
                Func_080f9010(0x72);
            } else {
                switch (selection) {
                case 0:
                    Func_080f9010(0xae);
                    break;
                case 1:
                    Func_080f9010(0xaf);
                    break;
                case 2:
                case 3:
                case 5:
                    Func_080f9010(0x70);
                    break;
                case 4:
                    Func_080f9010(0x75);
                    break;
                default:
                    Func_080f9010(0x70);
                    break;
                }
                state->saved_selection = (s8)selection;
                break;
            }
        }

        if ((Data_03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            selection = -1;
            state->saved_selection = -1;
            break;
        }

        if ((Data_03001b04 & 0x40) != 0) {
            row--;
            redraw = 1;
            Func_080f9010(0x6f);
        } else if ((Data_03001b04 & 0x80) != 0) {
            row++;
            redraw = 1;
            Func_080f9010(0x6f);
        } else if ((Data_03001b04 & 0x10) != 0) {
            column++;
            redraw = 1;
            Func_080f9010(0x6f);
        } else if ((Data_03001b04 & 0x20) != 0) {
            column--;
            redraw = 1;
            Func_080f9010(0x6f);
        }
    }

    state->detail_mode = 0;
    Func_080a3c98();
    return selection;
}

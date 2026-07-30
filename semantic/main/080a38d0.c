#include "layout_guard.h"
#include "types.h"

struct ActionMenuDefinition {
    u8 padding00[0x0c];
    u16 first_row;
};

struct ActionMenuCursor {
    u8 padding00[5];
    u8 kind;
    u16 row;
    u8 padding08[0x0e];
    u16 attributes;
};

struct ActionMenuState {
    u8 padding000[8];
    s32 selected_action;
    u8 padding00c[4];
    struct ActionMenuDefinition *definition;
    u8 padding014[4];
    struct ActionMenuCursor *cursor;
    s8 initial_position;
    s8 position;
    u8 padding01e[2];
    s32 main_window;
    s32 action_window;
    u8 padding028[4];
    s32 description_window;
    u8 padding030[0x144];
    u16 item_id;
    u8 padding176[2];
    u16 subject_id;
    u8 padding17a[0x8e];
    u16 actions[8];
    u8 padding218;
    u8 action_count;
    u8 detail_mode;
    u8 selected_action_byte;
};

LAYOUT_OFFSET_GUARD(
    ActionMenuState_SelectedAction,
    struct ActionMenuState,
    selected_action,
    0x08);
LAYOUT_OFFSET_GUARD(
    ActionMenuState_MainWindow,
    struct ActionMenuState,
    main_window,
    0x20);
LAYOUT_OFFSET_GUARD(
    ActionMenuState_ItemId,
    struct ActionMenuState,
    item_id,
    0x174);
LAYOUT_OFFSET_GUARD(
    ActionMenuState_Actions,
    struct ActionMenuState,
    actions,
    0x208);
LAYOUT_OFFSET_GUARD(
    ActionMenuState_ActionCount,
    struct ActionMenuState,
    action_count,
    0x219);
LAYOUT_SIZE_GUARD(ActionMenuState_Size, struct ActionMenuState, 0x21c);

extern struct ActionMenuState *Data_03001f2c;
extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001b04;

s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
void Func_080041d8(u32, s32);
void Func_08015068(s32, s32, s32, s32, s32);
void Func_08015070(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_080150b0(s32, s32, s32, s32, s32);
void Func_08015270(s32);
void *Func_08077008(u16);
s32 Func_080770c0(s32);
void Func_080770d0(s32);
void Func_080a112c(s32, u16, u16, s32);
void Func_080a17c4(struct ActionMenuCursor *);
void Func_080a1a40(s32, s32);
void Func_080a23f4(s32, s32, s32, s32, s32);
void Func_080a3c98(void);
s32 Func_080a3ce4(s32);
s32 Func_080a3d6c(u16);
s32 Func_080a3d9c(u16, s32);
void Func_080a3e88(u16, s32);
void Func_080a3ef0(u8, u16, s32, u16);
void Func_080f9010(s32);

/*
 * Run the action-selection window. Mode zero presents the subject/action
 * details, while mode one presents the comparison view and disallows selecting
 * the action that was already active on entry.
 */
s32 Func_080a38d0(s32 mode)
{
    struct ActionMenuState *state = Data_03001f2c;
    s32 position = state->position;
    s32 action_count = state->action_count;
    s32 selection_result = 0;
    s32 description_open = 0;
    s32 redraw = 1;
    s32 position_bytes = 0;

    Func_080a23f4(state->main_window, 13, 5, 17, 12);
    Func_08015270(state->main_window);
    Func_08077008(state->actions[state->initial_position]);
    Func_080041d8(0x080a3c09, 0x0c80);

    for (;;) {
        if (Func_080770c0(0x150) != 0) {
            position_bytes = position * 2;
            break;
        }

        if (redraw != 0) {
            struct ActionMenuCursor *cursor;
            s32 row;
            u16 action;

            redraw = 0;
            position = Func_080022fc(action_count + position, action_count);
            position_bytes = position * 2;
            action = state->actions[position];
            Func_08077008(action);

            cursor = state->cursor;
            row = ((state->definition->first_row + position * 3) * 8) - 2;
            cursor->row = (u16)row;
            cursor->attributes =
                (cursor->attributes & 0xfe00) | ((u16)row & 0x01ff);

            if (mode == 1) {
                s32 value;

                Func_080a3e88(action, 1);
                Func_08015070(state->main_window, 0, 9, 16, 9);
                Func_08015068(state->main_window, 0, 72, 120, 80);

                if (position != state->initial_position) {
                    value = Func_080a3d9c(action, state->subject_id & 0x01ff);
                    if (value != 0) {
                        Func_080150b0(value, 2, state->main_window, 8, 72);
                        Func_08015080(0x0b2f, state->main_window, 24, 72);
                    } else {
                        Func_08015080(0x0b31, state->main_window, 16, 72);
                    }

                    if (Func_080a3d6c(action) == 15 && value == 0)
                        Func_08015080(0x0b30, state->main_window, 0, 72);
                }

                Func_080a3ef0(
                    state->detail_mode,
                    state->item_id,
                    0,
                    action);
            }

            if (mode == 0) {
                if (Func_080a3ce4(state->subject_id & 0x01ff) != 0) {
                    Func_080a112c(
                        state->action_window,
                        action,
                        state->item_id,
                        8);
                } else {
                    Func_080a112c(
                        state->action_window,
                        action,
                        state->item_id,
                        0);
                }

                if (Func_080770c0(0x151) == 0 && description_open == 0) {
                    Func_08015270(state->description_window);
                    Func_08015080(
                        (state->subject_id & 0x01ff) + 0x75,
                        state->description_window,
                        0,
                        0);
                    description_open = 1;
                } else {
                    Func_080770d0(0x151);
                }
            }
        } else {
            position_bytes = position * 2;
        }

        Func_080a1a40(((position_bytes + position) * 8) - 10, 16);
        Func_080030f8(1);

        if ((Data_03001c94 & 1) != 0) {
            if (mode == 1 && position == state->initial_position) {
                Func_080f9010(0x72);
                continue;
            }

            Func_080f9010(0x70);
            selection_result = state->actions[position] & 0xff;
            break;
        }

        if ((Data_03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            selection_result = -1;
            break;
        }

        if ((Data_03001b04 & 0x20) != 0) {
            Func_080f9010(0x6f);
            redraw = 1;
            position--;
        }
        if ((Data_03001b04 & 0x10) != 0) {
            Func_080f9010(0x6f);
            redraw = 1;
            position++;
        }
    }

    state->position = (s8)position;
    Func_080a17c4(state->cursor);
    state->cursor->kind = 13;
    Func_080a3c98();
    Func_080030f8(1);
    state->position = (s8)position;
    state->selected_action = state->actions[position];
    state->selected_action_byte = (u8)state->actions[position];
    return (s8)selection_result;
}

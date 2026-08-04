#include "layout_guard.h"
#include "types.h"

typedef struct DisplayObject_080a60d4 {
    u8 padding00[5];
    s8 mode;
} DisplayObject_080a60d4;

typedef struct Character_080a60d4 {
    u8 opaque;
} Character_080a60d4;

typedef struct ActionSelector_080a60d4 {
    u8 padding000[8];
    s32 chosen_action;
    u8 padding00c[0x10];
    s8 selection;
    u8 padding01d;
    s8 action_count;
    u8 padding01f;
    void *header_window;
    void *character_window;
    void *prompt_window;
    void *detail_window;
    u8 padding030[0x114];
    u16 row_styles[4];
    u8 padding14c[0xcc];
    u8 action_available;
    u8 padding219;
    s8 chosen_action_id;
    u8 padding21b;
    DisplayObject_080a60d4 *prompt_object;
    u8 padding220[0x48];
    u8 modifier_mode;
} ActionSelector_080a60d4;

LAYOUT_OFFSET_GUARD(
    ActionSelector080a60d4_Selection,
    ActionSelector_080a60d4,
    selection,
    0x1c);
LAYOUT_OFFSET_GUARD(
    ActionSelector080a60d4_HeaderWindow,
    ActionSelector_080a60d4,
    header_window,
    0x20);
LAYOUT_OFFSET_GUARD(
    ActionSelector080a60d4_RowStyles,
    ActionSelector_080a60d4,
    row_styles,
    0x144);
LAYOUT_OFFSET_GUARD(
    ActionSelector080a60d4_ActionAvailable,
    ActionSelector_080a60d4,
    action_available,
    0x218);
LAYOUT_OFFSET_GUARD(
    ActionSelector080a60d4_PromptObject,
    ActionSelector_080a60d4,
    prompt_object,
    0x21c);
LAYOUT_OFFSET_GUARD(
    ActionSelector080a60d4_ModifierMode,
    ActionSelector_080a60d4,
    modifier_mode,
    0x268);

extern ActionSelector_080a60d4 *Data_03001f2c;
extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001b04;

s8 Func_080022fc(s32, s32);
void Func_08002df0(void *);
void Func_080030f8(s32);
u16 *Func_08004938(s32);
DisplayObject_080a60d4 *Func_080150d0(s32, s32, void *, s32, s32);
void Func_08015270(void *);
void Func_08015278(void *);
Character_080a60d4 *Func_08077008(u16);
s32 Func_080770c0(s32);
void Func_080770d0(s32);
s32 Func_080a10d0(void *, s32, s32, s32, s32, s32);
void Func_080a112c(void *, u16, s32, s32);
void Func_080a1804(ActionSelector_080a60d4 *, u16);
void Func_080a1a40(s32, s32);
void Func_080a23c0(void *);
void Func_080a33d4(ActionSelector_080a60d4 *, void *);
void Func_080a6384(u16);
void Func_080a6614(void *, u16);
s8 Func_080a68ec(Character_080a60d4 *, u16 *, s32);
void Func_080f9010(s32);

/* Present the character action selector and return the chosen action. */
s32 Func_080a60d4(const u16 *actions)
{
    ActionSelector_080a60d4 *state = Data_03001f2c;
    s32 count = state->action_count;
    s8 selection = state->selection;
    s32 result = 0;
    s32 redraw = 1;
    s32 detail_ready = 0;
    Character_080a60d4 *character;

    state->modifier_mode = 0;
    character = Func_08077008(actions[selection]);

    if (Func_080a10d0(
            &state->header_window, 0x0d, 3, 0x11, 0x0a, 2) != 0) {
        Func_080a33d4(state, state->header_window);
    }
    if (Func_080a10d0(
            &state->prompt_window, 0x0d, 0x0d, 0x11, 4, 2) != 0) {
        state->prompt_object =
            Func_080150d0(2, 0, state->prompt_window, 0, result);
        state->prompt_object->mode = 0x0d;
    }

    for (;;) {
        s32 index;
        u16 action;

        if (Func_080770c0(0x150) != 0)
            break;

        if (redraw != 0) {
            s32 row;

            redraw = 0;
            selection = Func_080022fc(count + selection, count);
            index = selection;
            action = actions[index];
            character = Func_08077008(action);
            Func_080a6384(action);
            Func_080a112c(state->character_window, action, 0, 0);
            Func_080a6614(state->prompt_window, action);
            Func_080a1804(state, action);

            for (row = 0; row < 4; row++)
                state->row_styles[row] = 0x1e;
            state->row_styles[index] = 0x1a;

            if (Func_080770c0(0x151) == 0 && detail_ready == 0) {
                Func_08015278(state->detail_window);
                Func_08015270(state->detail_window);
                Func_080a23c0(state->detail_window);
                detail_ready = 1;
            } else {
                Func_080770d0(0x151);
            }
        }

        index = selection;
        action = actions[index];
        Func_080a1a40(index * 0x18 - 0x0a, 0x10);
        Func_080030f8(1);

        if ((Data_03001c94 & 1) != 0) {
            if (state->action_available != 0) {
                Func_080f9010(0x70);
                result = action;
                break;
            }
            Func_080f9010(0x72);
        }

        if ((Data_03001c94 & 0x300) != 0) {
            u16 *buffer;
            s8 accepted;

            result = action;
            state->modifier_mode =
                (Data_03001c94 & 0x200) != 0 ? 1 : 2;
            buffer = Func_08004938(0x40);
            accepted = Func_080a68ec(character, buffer, 1);
            Func_08002df0(buffer);
            if (accepted != 0) {
                Func_080f9010(0x70);
                break;
            }
            state->modifier_mode = 0;
            Func_080f9010(0x72);
        }

        if ((Data_03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            result = -1;
            break;
        }
        if ((Data_03001b04 & 0x20) != 0) {
            Func_080f9010(0x6f);
            redraw = 1;
            selection--;
        }
        if ((Data_03001b04 & 0x10) != 0) {
            Func_080f9010(0x6f);
            redraw = 1;
            selection++;
        }
    }

    state->selection = selection;
    state->chosen_action = actions[selection];
    state->chosen_action_id = (s8)actions[selection];
    return result;
}

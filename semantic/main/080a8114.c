#include "layout_guard.h"
#include "types.h"

typedef struct Cursor_080a8114 {
    u8 padding00[5];
    u8 dirty;
} Cursor_080a8114;

typedef struct MenuState_080a8114 {
    u8 padding000[8];
    s32 selected_id;
    u8 padding00c[8];
    Cursor_080a8114 *cursor;
    u8 padding018[4];
    s8 character_index;
    u8 padding01d[7];
    s32 content_window;
    u8 padding028[4];
    s32 selector_window;
    u8 padding030[0x1d8];
    u16 character_ids[8];
    u8 padding218;
    u8 character_count;
    u8 selected_character;
    u8 padding21b[0x21];
    u16 row_markers[4];
} MenuState_080a8114;

LAYOUT_OFFSET_GUARD(
    MenuState080a8114_Cursor,
    MenuState_080a8114,
    cursor,
    0x14);
LAYOUT_OFFSET_GUARD(
    MenuState080a8114_ContentWindow,
    MenuState_080a8114,
    content_window,
    0x24);
LAYOUT_OFFSET_GUARD(
    MenuState080a8114_CharacterIds,
    MenuState_080a8114,
    character_ids,
    0x208);
LAYOUT_OFFSET_GUARD(
    MenuState080a8114_CharacterCount,
    MenuState_080a8114,
    character_count,
    0x219);
LAYOUT_OFFSET_GUARD(
    MenuState080a8114_RowMarkers,
    MenuState_080a8114,
    row_markers,
    0x23c);
LAYOUT_SIZE_GUARD(MenuState080a8114_Size, MenuState_080a8114, 0x244);

extern MenuState_080a8114 *Data_03001f2c;
extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;

s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
void Func_080041d8(u32, s32);
void Func_08004278(u32);
void Func_08015068(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015270(s32);
void Func_08015278(s32);
void *Func_08077008(u8);
s32 Func_080770c0(s32);
s32 Func_08077290(s32);
void Func_080a10d0(void *, s32, s32, s32, s32, s32);
void Func_080a1804(MenuState_080a8114 *, u16);
void Func_080a1a40(s32, s32);
void Func_080a1ac0(s32, s32);
void Func_080a847c(s8, s32, u8 *, s32);
void Func_080a8508(s32, s32, u8 *);
void Func_080a8578(s32, s32, u32);
void Func_080a8604(s32, u8, s32);
u8 Func_080a8b10(u8 *, s32, u8);
void Func_080a9d84(void);
void Func_080f9010(s32);

/*
 * Select an entry from either the character's available list or the alternate
 * seven/eight-entry pane. Shoulder buttons rotate the active character and
 * rebuild both panes without leaving the selector.
 */
s32 Func_080a8114(void)
{
    MenuState_080a8114 *state = Data_03001f2c;
    u8 availability[8];
    u32 extended_alternate = Func_08077290(-1) != 0;
    s8 pane = 0;
    s8 selection = 0;
    s32 done = 0;
    s32 result = 0;
    s32 i;

    Func_080a10d0(&state->selector_window, 0, 0, 30, 5, 2);
    Func_08004278(0x080a19a1);
    for (i = 0; i < 4; i++)
        state->row_markers[i] = 0x68;
    Func_080a1ac0(-10, 88);

    for (;;) {
        u8 option_count;
        s32 had_options;
        s32 redraw;

        if (done != 0 || Func_080770c0(0x150) != 0)
            break;

        Func_08077008(state->selected_character);
        Func_080a8604(
            state->content_window,
            state->selected_character,
            1);
        option_count =
            Func_080a8b10(availability, 1, state->selected_character);
        had_options = option_count != 0;
        if (option_count == 0)
            option_count = 1;
        redraw = 1;

        for (;;) {
            if (Func_080770c0(0x150) != 0)
                break;

            if (redraw != 0) {
                redraw = 0;
                pane = (s8)((pane + 2) % 2);

                if (pane == 0) {
                    selection = (s8)Func_080022fc(
                        selection + (s8)option_count,
                        (s8)option_count);
                    Func_08015270(state->selector_window);
                    if (had_options == 0) {
                        Func_08015080(
                            0x0b06,
                            state->content_window,
                            80,
                            -24);
                        Func_08015080(
                            0x0b07,
                            state->content_window,
                            0,
                            -24);
                    }
                } else {
                    Func_08015270(state->selector_window);
                    if (extended_alternate != 0) {
                        s32 wrapped = selection + 8;
                        s32 quotient_value = wrapped;

                        if (wrapped < 0)
                            quotient_value = selection + 15;
                        selection =
                            (s8)(wrapped - (quotient_value >> 3) * 8);
                    } else {
                        selection =
                            (s8)Func_080022fc(selection + 7, 7);
                    }
                }

                Func_080a847c(pane, selection, availability, 0);
                Func_08015278(state->selector_window);
                Func_080030f8(1);
                if (pane == 0)
                    Func_080a8508(
                        state->selector_window,
                        selection,
                        availability);
                else
                    Func_080a8578(
                        state->selector_window,
                        selection,
                        extended_alternate);
            }

            state->cursor->dirty = 1;
            if (pane == 0) {
                Func_080a1a40(-10, selection * 16 + 88);
            } else if (selection <= 3) {
                Func_080a1a40(24, selection * 8 + 48);
            } else {
                Func_080a1a40(48, selection * 8 + 80);
            }
            Func_080030f8(1);

            if ((Data_03001b04 & 0xf0) != 0)
                Func_080a847c(pane, selection, availability, 1);

            if ((Data_03001c94 & 1) != 0) {
                Func_080f9010(0x70);
                done = 1;
                result = 1;
                break;
            }
            if ((Data_03001c94 & 2) != 0) {
                Func_080f9010(0x71);
                done = 1;
                result = -1;
                break;
            }

            if ((Data_03001b04 & 0x40) != 0) {
                Func_080f9010(0x6f);
                redraw = 1;
                selection--;
            }
            if ((Data_03001b04 & 0x80) != 0) {
                Func_080f9010(0x6f);
                redraw = 1;
                selection++;
            }
            if ((Data_03001b04 & 0x10) != 0) {
                Func_080f9010(0x6f);
                redraw = 1;
                pane++;
            }
            if ((Data_03001b04 & 0x20) != 0) {
                Func_080f9010(0x6f);
                redraw = 1;
                pane--;
            }

            if ((Data_03001b04 & 0x300) != 0) {
                s32 next_index;
                u16 character_id;

                Func_080f9010(0x6f);
                if ((Data_03001b04 & 0x100) != 0)
                    next_index = state->character_index + 1;
                else
                    next_index = state->character_index - 1;
                next_index = Func_080022fc(
                    next_index + state->character_count,
                    state->character_count);
                character_id = state->character_ids[next_index];
                state->selected_id = character_id;
                state->selected_character = (u8)character_id;
                state->character_index = (s8)next_index;
                Func_080a1804(state, character_id);
                break;
            }
        }
    }

    Func_08015278(state->selector_window);
    Func_08015270(state->selector_window);
    Func_08015068(state->content_window, 64, 56, 224, 96);
    Func_080041d8(0x080a19a1, 0x0c80);
    for (i = 0; i < 4; i++)
        state->row_markers[i] = 0x80;
    Func_080a9d84();
    return result;
}

#include "layout_guard.h"
#include "types.h"

typedef struct DisplayObject_080a35f8 {
    u8 padding00[5];
    s8 mode;
} DisplayObject_080a35f8;

typedef struct Character_080a35f8 {
    u8 padding000[0xd8];
    u8 equipment;
} Character_080a35f8;

typedef struct CharacterSelector_080a35f8 {
    u8 padding000[8];
    s32 chosen_character;
    u8 padding00c[8];
    DisplayObject_080a35f8 *control;
    u8 padding018[4];
    s8 selection;
    u8 padding01d;
    s8 character_count;
    u8 padding01f;
    void *header_window;
    void *character_window;
    void *prompt_window;
    u8 padding02c[0x118];
    u16 row_styles[4];
    u8 padding14c[0x7c];
    u8 alternate_data[0x50];
    s8 alternate_result;
    u8 padding219;
    s8 chosen_character_id;
    u8 padding21b;
    DisplayObject_080a35f8 *prompt_object;
} CharacterSelector_080a35f8;

LAYOUT_OFFSET_GUARD(
    Character080a35f8_Equipment,
    Character_080a35f8,
    equipment,
    0xd8);
LAYOUT_OFFSET_GUARD(
    CharacterSelector080a35f8_Control,
    CharacterSelector_080a35f8,
    control,
    0x14);
LAYOUT_OFFSET_GUARD(
    CharacterSelector080a35f8_Selection,
    CharacterSelector_080a35f8,
    selection,
    0x1c);
LAYOUT_OFFSET_GUARD(
    CharacterSelector080a35f8_HeaderWindow,
    CharacterSelector_080a35f8,
    header_window,
    0x20);
LAYOUT_OFFSET_GUARD(
    CharacterSelector080a35f8_RowStyles,
    CharacterSelector_080a35f8,
    row_styles,
    0x144);
LAYOUT_OFFSET_GUARD(
    CharacterSelector080a35f8_AlternateData,
    CharacterSelector_080a35f8,
    alternate_data,
    0x1c8);
LAYOUT_OFFSET_GUARD(
    CharacterSelector080a35f8_AlternateResult,
    CharacterSelector_080a35f8,
    alternate_result,
    0x218);
LAYOUT_OFFSET_GUARD(
    CharacterSelector080a35f8_PromptObject,
    CharacterSelector_080a35f8,
    prompt_object,
    0x21c);

extern CharacterSelector_080a35f8 *Data_03001f2c;
extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001ae8;
extern volatile u32 Data_03001b04;

s8 Func_080022fc(s32, s32);
void Func_080030f8(s32);
void Func_08015080(s32, void *, s32, s32);
DisplayObject_080a35f8 *Func_080150d0(s32, s32, void *, s32, s32);
Character_080a35f8 *Func_08077008(u16);
s32 Func_080770c0(s32);
s32 Func_080a10d0(void *, s32, s32, s32, s32, s32);
void Func_080a112c(void *, u16, s32, s32);
void Func_080a1a40(s32, s32);
void Func_080a1e38(u8 *, u8);
void Func_080a33d4(CharacterSelector_080a35f8 *, void *);
void Func_080a38a8(u16);
s32 Func_080a3d6c(u16);
s8 Func_080a3ddc(Character_080a35f8 *, void *, s32);
void Func_080a3e88(u16, s32);
void Func_080f9010(s32);

static void DrawSelection_080a35f8(
    CharacterSelector_080a35f8 *state,
    u16 character_id,
    s32 index,
    s32 alternate_mode)
{
    s32 row;

    if (alternate_mode != 0) {
        state->alternate_result = Func_080a3ddc(
            Func_08077008(character_id),
            state->alternate_data,
            0);
        Func_080a38a8(character_id);
        Func_080a112c(state->character_window, character_id, 0, 8);
    } else {
        Func_080a3e88(character_id, 0);
        Func_080a112c(state->character_window, character_id, 0, 0);
    }

    for (row = 0; row < 4; row++)
        state->row_styles[row] = 0x1e;
    state->row_styles[index] = 0x1a;
}

/* Present the party-character selector and return its chosen character ID. */
s32 Func_080a35f8(const u16 *character_ids)
{
    CharacterSelector_080a35f8 *state = Data_03001f2c;
    s8 selection = state->selection;
    s32 count = state->character_count;
    s32 result = 0;
    s32 equipment_page = 0;
    s32 alternate_mode = 0;
    s32 redraw = 1;
    Character_080a35f8 *character =
        Func_08077008(character_ids[selection]);

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

    Func_08015080(0x0b87, state->prompt_window, 0, 0);
    Func_08015080(0x0b88, state->prompt_window, 0, 8);
    state->control->mode = 1;

    for (;;) {
        s32 index;
        u16 character_id;

        if (Func_080770c0(0x150) != 0)
            break;

        selection = Func_080022fc(count + selection, count);
        index = selection;
        character_id = character_ids[index];
        Func_080a1a40(index * 0x18 - 0x0a, 0x10);

        if (redraw != 0) {
            equipment_page = 0;
            redraw = 0;
            character = Func_08077008(character_id);
            DrawSelection_080a35f8(
                state, character_id, index, alternate_mode);
        }

        Func_080030f8(1);
        if ((Data_03001c94 & 1) != 0) {
            if ((Data_03001ae8 & 0x200) != 0) {
                equipment_page %= 4;
                Func_080a1e38(&character->equipment, (u8)equipment_page);
                equipment_page = (u8)(equipment_page + 1);
                Func_080a3e88(character_id, 0);
                Func_080f9010(0x70);
            } else if (Func_080a3d6c(character_id) != 0) {
                Func_080f9010(0x70);
                result = character_id;
                break;
            } else {
                Func_080f9010(0x72);
            }
        }

        if ((Data_03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            result = -1;
            break;
        }

        if ((Data_03001c94 & 0x100) != 0) {
            alternate_mode = 1;
            redraw = 1;
        }
        if ((Data_03001ae8 & 0x100) == 0 && alternate_mode == 1) {
            alternate_mode = 0;
            redraw = 1;
        }
        if ((Data_03001b04 & 0x20) != 0) {
            Func_080f9010(0x6f);
            selection--;
            redraw = 1;
        }
        if ((Data_03001b04 & 0x10) != 0) {
            Func_080f9010(0x6f);
            selection++;
            redraw = 1;
        }
    }

    state->selection = selection;
    state->chosen_character = character_ids[selection];
    state->chosen_character_id = (s8)character_ids[selection];
    return result;
}

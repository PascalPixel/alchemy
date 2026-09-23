#include "TYPES.H"

struct CharacterSelectorState {
    u8 padding000[0x24];
    s32 screen_handle;
    u8 padding028[0x0e4];
    s32 selector_window;
    u8 padding110[0x34];
    u16 row_positions[8];
    u8 padding154[0x0b4];
    u16 character_ids[8];
    u8 padding218;
    u8 character_count;
    u8 padding21a[6];
    u16 flags;
};

s32 Func_08077150(s32 character_id);
s32 Func_08077158(const u16 *character_ids);
s32 Func_08077168(s32 character_id);

s32 CharacterSelector_MoveEntry(s32 selected_index, s32 direction)
{
    struct CharacterSelectorState *state =
        *(struct CharacterSelectorState **)0x03001f2c;
    u32 reordered[14];
    s32 index;

    if (state->character_count <= 1) {
        return 0;
    }
    if (direction == 1) {
        if (selected_index == state->character_count - 1) {
            return 0;
        }
    } else {
        if (selected_index == 0) {
            return 0;
        }
    }

    {
        u32 zero = 0;
        u32 *clear = &reordered[13];

        do {
            *clear = zero;
            clear--;
        } while ((s32)clear >= (s32)reordered);
    }
    for (index = 0; index < state->character_count; index++) {
        reordered[index] = state->character_ids[index];
    }

    if (direction == 1) {
        index = reordered[selected_index];
        reordered[selected_index] = reordered[selected_index + 1];
        reordered[selected_index + 1] = index;
    } else {
        index = reordered[selected_index];
        reordered[selected_index] = reordered[selected_index - 1];
        reordered[selected_index - 1] = index;
    }

    for (index = 0; index < state->character_count; index++) {
        Func_08077168(state->character_ids[index]);
    }
    for (index = 0; index < state->character_count; index++) {
        Func_08077150(reordered[index]);
    }
    state->character_count = Func_08077158(state->character_ids);
    return 1;
}

#include "types.h"

struct CharacterSelectorOrderState {
    u8 unknown_000[0x208];
    u16 character_ids[8];
    u8 unknown_218;
    u8 character_count;
};

s32 Func_08077150(s32 character_id);
s32 Func_08077158(const u16 *character_ids);
s32 Func_08077168(s32 character_id);

s32 Func_080a7f44(s32 selected_index, s32 direction)
{
    struct CharacterSelectorOrderState *state =
        *(struct CharacterSelectorOrderState **)0x03001f2c;
    u32 reordered[14];
    s32 adjacent_index;
    s32 index;
    u32 temporary;

    if (state->character_count <= 1) {
        return 0;
    }
    if (direction == 1) {
        if (selected_index == state->character_count - 1) {
            return 0;
        }
        adjacent_index = selected_index + 1;
    } else {
        if (selected_index == 0) {
            return 0;
        }
        adjacent_index = selected_index - 1;
    }

    for (index = 13; index >= 0; index--) {
        reordered[index] = 0;
    }
    for (index = 0; index < state->character_count; index++) {
        reordered[index] = state->character_ids[index];
    }
    temporary = reordered[selected_index];
    reordered[selected_index] = reordered[adjacent_index];
    reordered[adjacent_index] = temporary;

    for (index = 0; index < state->character_count; index++) {
        Func_08077168(state->character_ids[index]);
    }
    for (index = 0; index < state->character_count; index++) {
        Func_08077150(reordered[index]);
    }
    state->character_count = Func_08077158(state->character_ids);
    return 1;
}

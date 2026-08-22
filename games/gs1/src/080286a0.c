#include "types.h"

struct MenuState_080286a0 {
    u8 padding000[0x78];
    void *work;
    u8 padding07c[8];
    u8 resource_ids[8];
    s16 selection;
    s16 item_count;
    s16 field090;
    s16 resource_base;
};

extern struct MenuState_080286a0 *Data_03001f38;
extern u8 Data_080373ef[];
extern u8 Data_0000001f;

void Func_08016478(void *work);
void Func_0801e7c0(s32 resource_id, void *work, s32 arg2, s32 arg3);
void Func_080030f8(s32 frames);
void Func_080f9010(s32 sound_id);

static inline s32 AbsoluteDifference(s32 difference, s32 lhs, s32 rhs)
{
    if (difference >= 0)
        return difference;
    return rhs - lhs;
}

s32 Func_080286a0(s32 arg0, s32 arg1)
{
    s16 selected;
    s16 current_selection;
    s32 difference;
    s32 resource_id;
    s32 distance;
    s32 current;
    s32 step;
    s32 initial_delay;
    const u8 *delay_table;
    struct MenuState_080286a0 *state;

    state = Data_03001f38;
    step = 1;
    initial_delay = 12;
    state->selection = (s16)arg0;
    if (arg1 < arg0)
        step = -1;
    current = arg0;

    for (;;) {
        Func_08016478(state->work);
        selected = state->resource_base;
        if (selected != 0) {
            resource_id = selected + state->selection;
        } else {
            resource_id = state->resource_ids[state->selection] + (s32)&Data_0000001f;
        }
        Func_0801e7c0(resource_id, state->work, 0, 0);

        current_selection = state->selection;
        delay_table = Data_080373ef;
        difference = current_selection - arg1;
        distance = AbsoluteDifference(difference, current_selection, arg1);
        Func_080030f8(delay_table[distance] + initial_delay);

        if (current == arg1)
            break;

        state->selection = (s16)((u16)state->selection + step);
        Func_080f9010(111);
        initial_delay = 0;
        current += step;
    }

    Func_080030f8(48);
    Func_080f9010(112);
    return arg1;
}

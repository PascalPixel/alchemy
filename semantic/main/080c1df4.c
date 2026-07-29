typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

struct AllocationState {
    u8 unknown_00[0x10];
    u16 ids[5];
    u8 unknown_1a[2];
    u32 used_slots[5];
    u8 unknown_30[4];
    s8 selected_slot[5];
    u8 unknown_39[7];
    u8 count;
};

extern struct AllocationState *Data_03001e74;

s32 Func_080022fc(s32 value, s32 divisor);

s32 Func_080c1df4(s16 id)
{
    struct AllocationState *state = Data_03001e74;
    s32 index;

    for (index = 0; index < state->count; index++) {
        if (state->ids[index] == (u16)id)
            break;
    }

    if (index != state->count) {
        s32 attempts = 0;

        if (state->selected_slot[index] < 0) {
            state->selected_slot[index] = 1;
            state->used_slots[index] = 3;
            return 0x8001;
        }

        while (attempts <= 31) {
            s32 slot = Func_080022fc(state->selected_slot[index] + 1, 9);
            state->selected_slot[index] = slot;
            if ((state->used_slots[index] & (1 << slot)) == 0)
                break;
            attempts++;
        }

        state->used_slots[index] |= 1 << state->selected_slot[index];
        return state->selected_slot[index];
    }

    if (state->count <= 4) {
        index = state->count;
        state->selected_slot[index] = -1;
        state->ids[index] = id;
        state->used_slots[index] = 0;
        state->count++;
        return 9;
    }

    return -1;
}

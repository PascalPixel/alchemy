#include "types.h"

struct State_080f7e60 {
    u8 padding0000[0x3404];
    s32 bucket_by_slot[0x400];
    u8 padding4404[0x34];
    u32 input_cursor;
    s32 padding443c;
    u32 input_limit;
};

extern struct State_080f7e60 *Data_02004c00;

void Func_080f7df0(s32 index);
void Func_080f7e34(s32 index);

void Func_080f7e60(s32 start, s32 count, const u8 *input)
{
    s32 limit = count;
    s32 current = 0;
    u32 base = (u32)start;

    if (current < limit) {
        u32 mask = 0x3ff;
        u32 removal = base + 0x124;

        do {
            u32 slot = base + (u32)current;
            struct State_080f7e60 *state;
            u32 read_offset;
            u32 next_offset;
            u8 value;

            Func_080f7e34(removal & mask);
            state = Data_02004c00;
            read_offset = state->input_cursor;
            value = input[read_offset];
            next_offset = read_offset + 1;
            state->input_cursor = next_offset;
            if (next_offset == state->input_limit) {
                state->bucket_by_slot[slot & mask] = -1;
                break;
            }

            state->bucket_by_slot[slot & mask] = value;
            current++;
            Func_080f7df0(slot & mask);
            removal++;
        } while (current < limit);
    }

    current++;
    if (current < limit) {
        u32 mask = 0x3ff;
        struct State_080f7e60 **root = &Data_02004c00;
        s32 empty = -1;

        do {
            u32 slot = (base + (u32)current) & mask;
            struct State_080f7e60 *state;

            Func_080f7e34(slot);
            state = *root;
            current++;
            state->bucket_by_slot[slot] = empty;
        } while (current < limit);
    }
}

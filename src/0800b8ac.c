typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct State_0800b8ac {
    u8 padding0[24];
    s32 shifted;
    u8 padding1[4];
    u8 first;
    u8 second;
    u8 third;
    u8 fourth;
    u8 padding2[3];
    u8 count;
    s32 slots[4];
};

struct Metadata_0800b8ac {
    u8 first;
    u8 second;
    u16 value;
    u8 padding[2];
    u8 third;
    u8 fourth;
};

extern s32 Func_0800bbc0(s32);
extern struct Metadata_0800b8ac *Func_08185000(s32);

s32 Func_0800b8ac(struct State_0800b8ac *state, s32 arg1)
{
    s32 value = state->slots[0];
    s32 index = 0;
    s32 *slot;
    struct Metadata_0800b8ac *metadata;

    if (value != 0) {
        slot = &state->slots[0];
        do {
            index++;
            if (index > 3)
                break;
            slot++;
            value = *slot;
        } while (value != 0);
    }
    if (index == 4)
        return -1;

    value = Func_0800bbc0(arg1);
    if (value == 0)
        return 0;
    state->slots[index] = value;
    metadata = Func_08185000(arg1);
    if (state->count == 0) {
        state->first = metadata->first;
        state->second = metadata->second;
        state->shifted = metadata->value << 8;
        state->fourth = metadata->fourth;
        state->third = metadata->third;
    }
    if (index == state->count)
        state->count = index + 1;
    return value;
}

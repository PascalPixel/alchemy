typedef unsigned char u8;
typedef signed int s32;

struct Entry_080b7eb4 {
    s32 first;
    s32 second;
    u8 filler8[36];
};

struct State_080b7eb4 {
    u8 filler0[0x80];
    struct Entry_080b7eb4 entries[1];
};

struct Output_080b7eb4 {
    s32 first;
    s32 middle;
    s32 second;
};

extern struct State_080b7eb4 * volatile Data_03001e74;

s32 Func_080b7eb4(s32 index, struct Output_080b7eb4 *output)
{
    struct State_080b7eb4 *state = Data_03001e74;

    output->first = state->entries[index].first;
    output->middle = 0;
    output->second = state->entries[index].second;
    return 0;
}

typedef unsigned char u8;
typedef signed int s32;

struct State_080935b0 {
    u8 filler0[0xEC];
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
};

extern struct State_080935b0 * volatile Data_03001e70;

void Func_080935b0(s32 first, s32 second, s32 third, s32 fourth)
{
    struct State_080935b0 *state = Data_03001e70;

    state->first = first;
    state->second = second;
    state->third = third;
    state->fourth = fourth;
}

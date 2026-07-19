typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct State_0807a1b4 {
    u8 padding[248];
    u32 flags[8];
    u8 counts[8];
};

struct State_0807a1b4 *Func_08077394(s32);

s32 Func_0807a1b4(s32 owner, s32 index, s32 bit)
{
    struct State_0807a1b4 *state = Func_08077394(owner);

    if (state->counts[index] > 9)
        return -1;
    if ((state->flags[index] & (1 << bit)) != 0)
        return -1;
    state->counts[index]++;
    state->flags[index] |= 1 << bit;
    return 0;
}

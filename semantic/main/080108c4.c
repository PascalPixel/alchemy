typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct State_080108c4 {
    u8 filler0[0x14];
    u16 flags;
};

extern struct State_080108c4 * volatile Data_03001e70;

void Func_080108c4(u32 value)
{
    struct State_080108c4 *state = Data_03001e70;
    u32 mask = 0xE0;
    u32 flags = state->flags;

    mask <<= 4;
    mask &= value;
    flags &= 0xF1FF;
    flags |= mask;
    state->flags = flags;
}

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct State_0801a7c0 {
    u8 filler0[0x354];
    u16 first[16];
    u16 second[16];
    u16 count;
};

extern struct State_0801a7c0 * volatile Data_03001e98;

void Func_0801a7c0(u32 first, u32 second)
{
    struct State_0801a7c0 *state = Data_03001e98;
    u16 count = state->count;

    if (count != 16) {
        state->first[count] = first;
        state->second[count] = second;
        state->count++;
    }
}

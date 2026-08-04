typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct State_080fa9a4 {
    u32 marker;
    volatile u8 status;
};

void Func_080fa9a4(void)
{
    struct State_080fa9a4 *state =
        *(struct State_080fa9a4 * volatile *)0x03007ff0;
    u32 marker = state->marker;

    if (marker != 0x68736d53) {
        volatile u16 *io = (volatile u16 *)0x040000c6;

        *io = 0xb600;
        io += 6;
        *io = 0xb600;
        state->status = 0;
        state->marker = marker - 10;
    }
}

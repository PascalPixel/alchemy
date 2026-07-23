typedef unsigned char u8;
typedef unsigned int u32;

struct State_080fb6a4 {
    u8 padding_00[40];
    u32 value;
    u8 padding_2c[20];
    u8 *source;
};

union Value_080fb6a4 {
    u32 word;
    struct {
        u32 byte0 : 8;
        u32 byte1 : 8;
        u32 byte2 : 8;
        u32 byte3 : 8;
    } bits;
};

void Func_080fb6a4(void *unused, struct State_080fb6a4 *state)
{
    union Value_080fb6a4 value;
    u8 *source = state->source;

    value.bits.byte0 = source[0];
    value.bits.byte1 = source[1];
    value.bits.byte2 = source[2];
    value.bits.byte3 = source[3];
    state->value = value.word;
    state->source = source + 4;
}

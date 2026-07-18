typedef unsigned char u8;
typedef signed int s32;

struct State_0807987c {
    u8 padding[0xf8];
    u8 source[0x30];
    u8 record;
};

struct State_0807987c *Func_08077394(s32);
void Func_080797fc(u8, u8 *, s32 *);
s32 Func_080022ec(s32, s32);

s32 Func_0807987c(s32 owner, s32 index)
{
    struct State_0807987c *state = Func_08077394(owner);
    s32 values[4];
    s32 result = 0;

    if (index <= 3) {
        Func_080797fc(state->record, state->source, values);
        result = Func_080022ec(values[index], 10);
    }
    return result;
}

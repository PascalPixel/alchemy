typedef signed short s16;
typedef signed int s32;

struct State_0808bc9c {
    unsigned char padding[0x16c];
    s16 values[12];
};

extern struct State_0808bc9c *Data_03001ebc;

s32 Func_0808bc9c(void)
{
    struct State_0808bc9c *state = Data_03001ebc;

    return state->values[0] + state->values[1] + state->values[2]
         + state->values[3] + state->values[4] + state->values[5]
         + state->values[6] + state->values[7] + state->values[6]
         + state->values[8] + state->values[9] + state->values[10]
         + state->values[11];
}

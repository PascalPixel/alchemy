typedef unsigned char u8;
typedef signed int s32;

struct State_0808b824 {
    u8 padding[0x34];
    s32 values[58];
};

extern struct State_0808b824 *Data_03001ebc;

s32 Func_0808b824(void)
{
    struct State_0808b824 *state = Data_03001ebc;
    s32 result = 7;
    s32 index = 8;
    s32 *value = state->values;

    do {
        s32 current = *value++;

        if (current != 0) {
            result = index;
        }
        index++;
    } while (index <= 65);
    result++;
    if (result == 66) {
        result = -1;
    }
    return result;
}

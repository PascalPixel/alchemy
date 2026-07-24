typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;

struct State_080bf2b4 {
    u8 padding[308];
    u8 counter;
    s8 flag;
};

extern struct State_080bf2b4 *Func_08077008(void);
extern s32 Func_080bf208(s32, s32, s32);

s32 Func_080bf2b4(s32 arg0)
{
    struct State_080bf2b4 *state = Func_08077008();

    if (state->counter != 0) {
        state->counter--;
        if (state->counter == 0) {
            state->flag = 0;
            return 1;
        }
        if (state->flag < 0 &&
            Func_080bf208(arg0, state->counter, 20) != 0) {
            state->flag = 0;
            state->counter = 0;
            return 1;
        }
    }
    return 0;
}

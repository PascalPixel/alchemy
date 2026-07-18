typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080e38b8(void *arg0, s32 arg1, s32 arg2) {
    s32 *state = arg0;

    state[0] += state[3];
    state[1] += state[4];
    state[2] += state[5];
    state[4] += arg2;
    state[3] = state[3] * arg1 / 64;
    state[4] = state[4] * arg1 / 64;
    state[5] = state[5] * arg1 / 64;
}

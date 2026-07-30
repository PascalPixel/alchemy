typedef signed int s32;
typedef unsigned int u32;

s32 Func_02001740(u32 *state)
{
    s32 match = 0;

    if (*state <= 1) {
        match = 1;
    }
    return match;
}

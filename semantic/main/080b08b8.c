typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);

void Func_080b08b8(void *state)
{
    void *object;
    s32 duration;
    s32 step;
    s32 value;
    void *zero;

    if (state == 0)
        return;
    duration = FIELD(state, s8, 13);
    if (duration == 0)
        return;

    object = FIELD(state, void *, 0);
    FIELD(state, u8, 12)++;
    step = FIELD(state, s8, 12);

    value = (u16)FIELD(state, u16, 4)
        + Func_080022ec(
            step * (FIELD(state, s16, 8) - FIELD(state, s16, 4)),
            duration);
    FIELD(object, u16, 6) = value;
    zero = 0;
    FIELD(object, u16, 22) =
        (FIELD(object, u16, 22) & 0xfe00) | (value & 0x1ff);

    value = (u16)FIELD(state, u16, 6)
        + Func_080022ec(
            step * (FIELD(state, s16, 10) - FIELD(state, s16, 6)),
            duration);
    FIELD(object, u16, 8) = value;
    FIELD(object, u8, 20) = value;

    if (step == duration) {
        FIELD(state, u8, 13) = (u32)zero;
        FIELD(state, u8, 12) = (u32)zero;
    }
}

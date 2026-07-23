typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

struct State_08078320 {
    u8 padding00[20];
    s16 firstRatio;
    s16 secondRatio;
    u8 padding18[28];
    s16 firstMaximum;
    s16 secondMaximum;
    s16 firstValue;
    s16 secondValue;
};

s32 Func_080022ec(s32, s32);

void Func_08078320(struct State_08078320 *state, s32 value)
{
    s16 maximum;
    s32 clamped;
    s32 ratio;
    s32 result;
    s32 numerator;

    maximum = state->secondMaximum;
    if (value > maximum) {
        clamped = maximum;
    } else {
        clamped = 0;
        if (value >= 0)
            clamped = value;
    }
    state->secondValue = clamped;

    numerator = state->firstValue;
    numerator <<= 14;
    result = Func_080022ec(numerator, state->firstMaximum);
    ratio = 0x4000;
    if (result <= ratio) {
        ratio = 0;
        if (result >= 0)
            ratio = result;
    }
    state->firstRatio = ratio;
    if (state->firstRatio == 0 && state->firstValue != 0)
        state->firstRatio = 1;

    numerator = state->secondValue;
    numerator <<= 14;
    result = Func_080022ec(numerator, state->secondMaximum);
    ratio = 0x4000;
    if (result <= ratio) {
        ratio = 0;
        if (result >= 0)
            ratio = result;
    }
    state->secondRatio = ratio;
    if (state->secondRatio == 0 && state->secondValue != 0)
        state->secondRatio = 1;
}

#include "types.h"

struct State_080c0cec {
    u8 filler0[12];
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1c;
    s32 field20;
    u8 filler24[16];
    s16 field34;
    s16 field36;
};

struct Runtime_080c0cec {
    struct State_080c0cec *state;
};

struct Vector_080c0cec {
    s32 first;
    s32 second;
    s32 third;
};

struct Transfer_080c0cec {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0c;
    s32 anchor;
};

extern struct Runtime_080c0cec Data_03001e80;
extern struct Transfer_080c0cec Data_03001ce0;

s32 Func_080022ec(s32, s32);
void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_080051d8(struct State_080c0cec *, void *);
void Func_08005258(u32, u32, u32);
void Func_08005268(struct Vector_080c0cec *, struct Vector_080c0cec *);
void Func_080c0a24(u32, u32, s32, s32, u32);

static inline u32 Scale_080c0cec(u32 value, u32 (*scale)(u32, u32))
{
    return scale(value, 192 << 8);
}

void Func_080c0cec(s32 x, s32 z, s32 y, s32 phase)
{
    struct State_080c0cec *state;
    struct Vector_080c0cec *position;
    struct Vector_080c0cec transform;
    struct Vector_080c0cec output;
    struct Vector_080c0cec input;
    s32 quotient;
    u32 (*scale)(u32, u32);
    u32 magnitude;
    s32 horizontal;
    s32 vertical;
    u32 scaled;

    state = Data_03001e80.state;
    position = (struct Vector_080c0cec *)&state->field0c;
    quotient = (s32)((u32)phase << 16);
    quotient = Func_080022ec(quotient, 100);
    position->second = z;
    position->first = x;
    position->third = y;

    magnitude = 255 << 17;
    scale = (u32 (*)(u32, u32))0x0300013c;
    input.first = 0;
    input.second = 0;
    input.third = 0;

    Func_08005258(magnitude, Scale_080c0cec(magnitude, scale), magnitude << 1);

    Func_080049ac();
    Func_08004cb4(&state->field0c);
    Func_08004c1c(state->field36);
    Func_08004bd4(state->field34);

    transform.first = 0;
    transform.second = 0;
    transform.third = magnitude;
    ((void (*)(struct Vector_080c0cec *, struct State_080c0cec *))0x03000250)(&transform, state);

    Data_03001ce0.field0c = 120;
    Data_03001ce0.anchor = 120;

    Func_080049ac();
    Func_080051d8(state, position);
    Func_08005268(&input, &output);

    horizontal = 120 - output.first;
    vertical = 120 - output.second;
    scaled = (u32)quotient;
    scaled <<= 8;
    scaled -= (u32)quotient;
    Func_080c0a24(240 << 15,
                  240 << 15,
                  horizontal << 8,
                  vertical << 8,
                  (u32)quotient);
    Func_08005258(scaled << 1,
                  Scale_080c0cec(scaled << 1, scale),
                  scaled << 2);
}

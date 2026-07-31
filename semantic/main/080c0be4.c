#include "types.h"

/*
 * Sibling of 0x080c0cec: the same camera-and-projection setup, with two
 * differences.  The phase argument that 0x080c0cec receives in r3 is not
 * supplied here - 0x080c0be4 reads r4, which the prologue never saves and no
 * path writes, so it converts as an uninitialised local rather than a skip.
 * And the third component of the transform vector is taken from the state
 * block (+0x20, read at 0x080c0c6a) instead of the fixed magnitude.
 *
 * The bl Func_08007310 at 0x080c0c36 and 0x080c0cba is the runtime
 * _call_via_fp thunk inside 0x080072e4..0x08007320 - index
 * (0x08007310 - 0x080072e4) / 4 = 11 - so it is an ordinary indirect call
 * through fp, which holds the pooled 0x0300013c ratio helper.  The
 * bl Func_080072f0 at 0x080c0c72 is _call_via_r3, index 3, an indirect call
 * through the pooled 0x03000250.
 *
 * The epilogue at 0x080c0cca unwinds the frame and ends pop {r0} ; bx r0,
 * so the owner is void.  Caller unknown.
 */

struct State_080c0be4 {
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

struct Runtime_080c0be4 {
    struct State_080c0be4 *state;
};

struct Vector_080c0be4 {
    s32 first;
    s32 second;
    s32 third;
};

struct Transfer_080c0be4 {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0c;
    s32 anchor;
};

extern struct Runtime_080c0be4 Data_03001e80;
extern struct Transfer_080c0be4 Data_03001ce0;

s32 Func_080022ec(s32, s32);
void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_080051d8(struct State_080c0be4 *, void *);
void Func_08005258(u32, u32, u32);
void Func_08005268(struct Vector_080c0be4 *, struct Vector_080c0be4 *);
void Func_080c0a24(u32, u32, s32, s32, u32);

static inline u32 Scale_080c0be4(u32 value, u32 (*scale)(u32, u32))
{
    return scale(value, 192 << 8);
}

void Func_080c0be4(s32 x, s32 z, s32 y)
{
    struct State_080c0be4 *state;
    struct Vector_080c0be4 *position;
    struct Vector_080c0be4 transform;
    struct Vector_080c0be4 output;
    struct Vector_080c0be4 input;
    s32 phase;
    s32 quotient;
    u32 (*scale)(u32, u32);
    u32 magnitude;
    s32 horizontal;
    s32 vertical;
    u32 scaled;

    state = Data_03001e80.state;
    position = (struct Vector_080c0be4 *)&state->field0c;
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

    Func_08005258(magnitude, Scale_080c0be4(magnitude, scale), magnitude << 1);

    Func_080049ac();
    Func_08004cb4(&state->field0c);
    Func_08004c1c(state->field36);
    Func_08004bd4(state->field34);

    transform.first = 0;
    transform.second = 0;
    transform.third = state->field20;
    ((void (*)(struct Vector_080c0be4 *, struct State_080c0be4 *))0x03000250)(&transform, state);

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
                  Scale_080c0be4(scaled << 1, scale),
                  scaled << 2);
}

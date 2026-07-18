typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct State_080782a0 {
    u8 padding0[20];
    s16 outputX;
    s16 outputY;
    u8 padding1[28];
    s16 limitX;
    s16 limitY;
    s16 inputX;
    s16 inputY;
};

s32 Func_080022ec(s32, s32);

void Func_080782a0(struct State_080782a0 *state, s32 input)
{
    s32 value;

    if (input > state->limitX) {
        value = state->limitX;
    } else {
        value = 0;
        if (input >= 0) {
            value = input;
        }
    }
    state->inputX = value;
    value = Func_080022ec((value << 16) >> 2, state->limitX);

    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->outputX = output;
        if ((output << 16) == 0 && state->inputX != 0) {
            state->outputX = 1;
        }
    }

    value = Func_080022ec(state->inputY << 14, state->limitY);
    {
        s32 output = 0x4000;

        if (value <= output) {
            output = 0;
            if (value >= 0) {
                output = value;
            }
        }
        state->outputY = output;
        if ((output << 16) == 0 && state->inputY != 0) {
            state->outputY = 1;
        }
    }
}

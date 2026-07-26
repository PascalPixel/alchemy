#include "types.h"

struct MenuObject_080a76d0 {
    u8 padding0[5];
    u8 mode;
};

struct MenuState_080a76d0 {
    u8 padding0[0x14];
    struct MenuObject_080a76d0 *object;
    u8 padding18[0x15C];
    u16 selection;
};

extern struct MenuState_080a76d0 *Data_03001f2c;

s32 Func_080770c0(s32 flag);
s32 Func_080a77a4(s32 index);
s32 Func_080a8114(void);
s32 Func_080a90bc(void);
s32 Func_080a96d8(void);

s32 Func_080a76d0(void)
{
    struct MenuState_080a76d0 *state = Data_03001f2c;
    s32 step = 0;
    s32 finished = step;
    s32 result = 0;
    u32 changed;

    while (!finished && !Func_080770c0(0x150)) {
        switch (step) {
        case 0:
            state->selection = finished;
            if (Func_080a77a4(0) == -1) {
                result = -1;
                finished = 1;
            }
            step = 1;
            break;
        case 1:
            state->object->mode = 13;
            result = Func_080a8114();
            step = result == -1 ? 0 : 2;
            break;
        case 2:
            state->object->mode = 13;
            result = Func_080a90bc();
            step = 0;
            if (result != -1)
                step = 3;
            break;
        case 3:
            state->object->mode = 13;
            result = Func_080a96d8();
            /* Collapse every non-cancellation result to one. */
            changed = (u32)~result;
            step = (-changed | changed) >> 31;
            break;
        default:
            finished = 1;
            break;
        }
    }

    if (Func_080770c0(0x150))
        result = -1;

    return result;
}

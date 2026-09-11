#include "types.h"
#include "scene.h"
#include "abi/menu/sel/run_action_flow.h"

struct MenuActionObject {
    u8 padding0[5];
    u8 mode;
};

struct MenuActionState {
    u8 padding0[0x14];
    struct MenuActionObject *object;
    u8 padding18[0x15C];
    u16 selection;
};

extern struct MenuState_080a76d0 *gIw;

s32 GameFlag_IsSet(s32 flag);

s32 Menu_RunActionFlow(void)
{
    struct MenuActionState *state = gIw;
    s32 step = 0;
    s32 finished = step;
    s32 result = 0;
    u32 changed;

    while (!finished && !GameFlag_IsSet(0x150)) {
        switch (step) {
        case 0:
            state->selection = finished;
            if (Menu_Check(0) == -1) {
                result = -1;
                finished = 1;
            }
            step = 1;
            break;
        case 1:
            state->object->mode = 13;
            result = Menu_Check2();
            step = result == -1 ? 0 : 2;
            break;
        case 2:
            state->object->mode = 13;
            result = Menu_Check3();
            step = 0;
            if (result != -1)
                step = 3;
            break;
        case 3:
            state->object->mode = 13;
            result = Menu_Check4();
            /* Collapse every non-cancellation result to one. */
            changed = (u32)~result;
            step = (-changed | changed) >> 31;
            break;
        default:
            finished = 1;
            break;
        }
    }

    if (GameFlag_IsSet(0x150))
        result = -1;

    return result;
}

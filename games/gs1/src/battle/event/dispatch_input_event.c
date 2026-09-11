#include "types.h"
#include "scene.h"
#include "abi/battle/event/dispatch_input_event.h"

struct RuntimeState_0808d8f0 {
    u8 padding0[0x19C];
    s16 delay;
};

struct GlobalData_0808d8f0 {
    u8 padding0[0x1F4];
    s32 selected_object;
};

extern void *gWork;
extern u8 gCell;
extern volatile u32 gIw;

void BattleFx_RunRisingObjectSequence(s32, s32, s32);

s32 Battle_DispatchInputEvent(s32 event)
{
    struct RuntimeState_0808d8f0 *state = (struct RuntimeState_0808d8f0 *)gWork;
    s32 selected_object = ((struct GlobalData_0808d8f0 *)&gCell)->selected_object;

    switch (event) {
    case 0xFC:
        if (state->delay > 12 && (gIw & 0x80)) {
            BattleFx_RunRisingObjectSequence(selected_object, 6, 0);
            state->delay = 0;
        }
        break;
    case 0xF9:
    case 0xFE:
        if (state->delay > 12) {
            Battle_Run();
            state->delay = 0;
        }
        break;
    case 0xFD:
        if (state->delay > 12) {
            if (gIw & 0x80) {
                Battle_Run2();
            } else if (gIw & 0x40) {
                Battle_Run3();
            }
            state->delay = 0;
        }
        break;
    }
    return 0;
}

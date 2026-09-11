#include "types.h"
#include "scene.h"
#include "abi/save/state/write_current_slot_pair.h"
#include "runtime_interfaces.h"

extern char gOv;
extern char Value_0000000a;
extern char Value_0000000b;

s16 SaveState_WriteCurrentSlotPair(void)
{
    s16 value;
    s16 result;
    s32 found;
    s32 error;

    result = 0;
    value = *(s16 *)0x02002004;
    if (value != -1) {
        found = State_Check();
        if (found != 0) {
            State_Apply((s32)&Value_0000000a, 1);
            error = 9;
            goto set_error;
        }
        State_Run();
        {
            void *base = &gOv;
            s32 next;

            found = State_Apply2(*(s16 *)0x02002004, base);
            next = *(s16 *)0x02002004;
            base = (char *)base + 0x1000;
            found |= State_Apply2(next + 3, base);
            if (found != 0) {
                State_Apply((s32)&Value_0000000b, 1);
                error = 3;
set_error:
                result = 0 - error;
            }
        }
        State_Run2();
        value = result;
    }
    return value;
}

#include "types.h"
#include "scene.h"
#include "abi/save/state/write_slot_pair.h"
#include "runtime_interfaces.h"

extern char gOv;
extern char Value_0000000a;
extern char Value_0000000b;

s32 SaveState_WriteSlotPair(s32 arg0)
{
    s32 found;
    s16 result = 0;

    found = State_Check();
    if (found != 0) {
        State_Apply((s32)&Value_0000000a, 1);
        result = -9;
    } else {
        void *base = &gOv;

        found = State_Apply2(arg0, base);
        base = (char *)base + 0x1000;
        found |= State_Apply2(arg0 + 3, base);
        if (found != 0) {
            State_Apply((s32)&Value_0000000b, 1);
            result = -3;
        }
    }
    State_Run();
    return result;
}

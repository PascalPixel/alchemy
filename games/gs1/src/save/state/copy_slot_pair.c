#include "types.h"
#include "scene.h"
#include "abi/save/state/copy_slot_pair.h"
#include "runtime_interfaces.h"

extern u8 gOv;
extern u8 Value_0000000a;
extern u8 Value_0000000c;
extern u8 Value_0000000d;
extern u8 Value_00000019;

s32 SaveState_CopySlotPair(void)
{
    u32 found;
    s32 value;
    s32 result;

    result = 0;
    found = State_Check();
    if (found != 0) {
        State_Apply((s32)&Value_0000000a, 1);
        result = -9;
    } else {
        State_Run();
        value = State_Apply2(0, 2);
        if (value == -1) {
            result = value;
        } else {
            void *lower = &gOv;
            void *upper;

            found = State_Apply3(value, lower);
            upper = (u8 *)lower + 0x1000;
            found |= State_Apply3(value + 3, upper);
            if (found != 0) {
                State_Apply((s32)&Value_0000000c, 1);
                result = -2;
            } else {
                value = State_Run2();
                if (value == 999) {
                    State_Apply((s32)&Value_0000000d, 1);
                    result = -5;
                } else {
                    found = State_Apply4(value, lower);
                    found |= State_Apply4(value + 3, upper);
                    if (found != 0) {
                        State_Apply((s32)&Value_0000000d, 1);
                        result = -3;
                    } else {
                        State_Apply((s32)&Value_00000019, 1);
                    }
                }
            }
        }
    }

    State_Run3();
    return result;
}

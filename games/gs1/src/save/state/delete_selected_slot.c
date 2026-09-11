#include "types.h"
#include "scene.h"
#include "abi/save/state/delete_selected_slot.h"
#include "runtime_interfaces.h"

extern u8 Value_0000000a;
extern u8 Value_0000000d;
extern u8 Value_00000016;
extern u8 Value_00000018;

s32 UiWork_IsComplete(void);
s32 WaitFrames(s32);

void UiWork_FinalizePendingCore(void);

s32 SaveState_DeleteSelectedSlot(void)
{
    s32 found;
    s32 value;
    s32 result = 0;

    found = State_Check();
    if (found != 0) {
        State_Apply((s32)&Value_0000000a, 1);
        result = -9;
    } else {
        State_Run();
        value = State_Apply2(0, 3);
        if (value == -1) {
            result = value;
        } else {
            State_SetMode((s32)&Value_00000016, 8, 1, 2);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            if (State_SetMode2(1, 0, 3, 1) != 0) {
                UiWork_FinalizePendingCore();
            } else {
                UiWork_FinalizePendingCore();
                found = State_Do(value);
                found |= State_Do(value + 3);
                if (found != 0) {
                    State_Apply((s32)&Value_0000000d, 1);
                    result = -4;
                } else {
                    State_Apply((s32)&Value_00000018, 1);
                }
            }
        }
    }
    State_Run2();
    return result;
}

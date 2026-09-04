#include "types.h"

#define SaveState_ConfirmAndProcessSelectedSlot Func_0801fb48

extern u8 Value_00000014;
extern u8 Value_00000017;

s32 Func_08017658(s32, s32, s32, s32);
s32 UiWork_IsComplete(void);
void WaitFrames(s32);
s32 Func_08028df4(s32, s32, s32, s32);
void UiWork_FinalizePendingCore(void);
void Audio_PlayCue(s32);
s32 Func_0801faa8(void);
void Func_0801776c(s32, s32);

s32 SaveState_ConfirmAndProcessSelectedSlot(void)
{
    s32 result;

    Func_08017658((s32)&Value_00000014, 8, 12, 2);
    while (UiWork_IsComplete() == 0) {
        WaitFrames(1);
    }
    if (Func_08028df4(1, 0, 0, 1)) {
        UiWork_FinalizePendingCore();
    } else {
        UiWork_FinalizePendingCore();
        Audio_PlayCue(85);
        result = Func_0801faa8();
        if (result >= 0) {
            Func_0801776c((s32)&Value_00000017, 1);
        }
    }
    return result;
}

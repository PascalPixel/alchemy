#include "battle_effect_runtime.h"

void Func_08015208(void);
void Func_08091660(void);
void Func_0808e118(void);
void ScheduleCallbackAfterFrames(const void *, s32);
void Func_080915ec(void);
u32 Func_080770d0(s32);

void Func_080916b0(void) {
    struct BattleRuntime *runtime = Data_03001ebc;

    Func_08015208();
    Func_08091660();
    if (runtime->unknown_cb6 != 0) {
        Func_0808e118();
    }
    {
        s32 zero = 0;
        runtime->unknown_cc2 = zero;
        runtime->unknown_cc4 = zero;
        runtime->unknown_1c8 = 0x10;
        runtime->mode_1cc = zero;
        runtime->unknown_1da = 0xFFFF;
        runtime->unknown_1dc = -1;
        runtime->unknown_1de = -1;
        ScheduleCallbackAfterFrames((const void *)Func_080915ec, 0xC80);
        Func_080770d0(0x132);
        runtime->object_id = Data_02000240.object_id;
        runtime->unknown_1f8 = zero;
    }
}

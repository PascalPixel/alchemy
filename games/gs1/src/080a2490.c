#include "types.h"

s32 ScheduleCallback(s32);
s32 GameFlag_IsSet(s32);
void Func_080a2444(void);

void Func_080a2490(void) {
    if (GameFlag_IsSet(0x150) == 0) {
        ScheduleCallback((s32)Func_080a2444);
    }
}

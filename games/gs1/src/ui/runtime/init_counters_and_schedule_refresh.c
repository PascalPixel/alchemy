#include "types.h"
#include "gs1_edition.h"

#define UiWork_InitCountersAndScheduleRefresh Func_08017464

struct State_08017464 {
#if defined(GS1_EDITION_JA)
    u8 filler0[0xF38];
#else
    u8 filler0[0xEA8];
#endif
    u16 ten;
    u16 unusedEaa;
    u16 zero;
    u16 fifteen;
#if defined(GS1_EDITION_JA)
    u8 fillerEb0[0x200];
#else
    u8 fillerEb0[0x400];
#endif
    u16 nine;
    u16 secondZero;
    u8 filler12b4[4];
    u16 result;
};

extern struct State_08017464 *Data_03001e8c;
extern u8 Data_0801789d;

s32 Resource_CopyData(s32, s32, s32);
void ScheduleCallbackAfterFrames(void *, s32);

void UiWork_InitCountersAndScheduleRefresh(s32 initialize) {
    struct State_08017464 *state = Data_03001e8c;
    s32 size;

    if (initialize != 0)
        state->result = Resource_CopyData(95, 128 << 6, 0);

    state->nine = 9;
    state->ten = 10;
    state->zero = 0;
    state->fifteen = 15;
    state->secondZero = 0;
    size = 200;
    size <<= 4;
    ScheduleCallbackAfterFrames(&Data_0801789d, size);
}

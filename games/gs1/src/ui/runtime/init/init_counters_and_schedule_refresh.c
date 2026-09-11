#include "types.h"
#include "scene.h"
#include "abi/ui/runtime/init/init_counters_and_schedule_refresh.h"
#include "gs1_edition.h"

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

extern struct State_08017464 *gIw;
extern u8 gRom;

s32 Resource_CopyData(s32, s32, s32);
void ScheduleCallbackAfterFrames(void *, s32);

void UiWork_InitCountersAndScheduleRefresh(s32 initialize)
{
    struct State_08017464 *state = gIw;
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
    ScheduleCallbackAfterFrames(&gRom, size);
}

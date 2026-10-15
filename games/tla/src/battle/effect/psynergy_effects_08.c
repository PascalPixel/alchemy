#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct ControlEvent {
    u8 unknown_000[362];
    u16 cue;
};

void Func_080e3d04(void);
void Func_080e4a34(void);
void Func_080e572c(void);
void Func_080e5d5c(void);
void Func_080e68d0(void);
void Func_080e7844(void);
void Func_080e9af4(void);
void Func_080e74d8(void);
void BattleFx_PrepareBufferInterpolation(void);
void WaitFrames(s32 frames);
s32 EventRuntime_GetControlledOwner(void);

void Func_080e7804(void)
{
    Func_080e74d8();
    BattleFx_PrepareBufferInterpolation();
    WaitFrames(10);
}

void Func_080e7818(void)
{
}

void Func_080e781c(void)
{
    ObjectTable_Get(EventRuntime_GetControlledOwner());
    ((struct ControlEvent *)Ram_HeapSlots->event_work)->cue = 0xfc89;
}

void Func_080e783c(void)
{
    Func_080e7844();
}

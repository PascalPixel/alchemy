#include "types.h"
#include "ram_buffer.h"
#include "scene_call.h"

struct EventWork {
    u8 unknown_000[0x15a];
    u16 unknown_15a;
};

extern u8 MsgCheckedTheIce[];

void Func_02000564(void)
{
    UiText_ShowPositionedMessageAndWait((s32)MsgCheckedTheIce, 1);
    UiText_ShowPositionedMessageAndWait((s32)MsgCheckedTheIce + 1, 1);
    ((struct EventWork *)Ram_HeapSlots->event_work)->unknown_15a = 1;
}

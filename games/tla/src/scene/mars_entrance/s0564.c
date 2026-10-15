#include "types.h"
#include "ram_buffer.h"
#include "scene_call.h"

struct EventWork {
    u8 unk000[0x15a];
    u16 unk15a;
};

extern u8 MsgCheckedTheIce[];

void Func_02000564(void)
{
    UiText_ShowPositionedMessageAndWait((s32)MsgCheckedTheIce, 1);
    UiText_ShowPositionedMessageAndWait((s32)MsgCheckedTheIce + 1, 1);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk15a = 1;
}

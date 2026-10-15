#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

struct EventNote {
    u8 unknown_000[0x15a];
    u16 unknown_15a;
};

void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void Func_020042dc(void);
void Func_020042e4(void);
void Func_0200444c(s32 skip);

void Func_02000e50(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    Func_020042dc();
    Func_0200444c(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgMarsStar07, 1);
    ((struct EventNote *)work)->unknown_15a = 1;
    Func_020042e4();
}

#include "KABE.H"
#include "text/MSG_IDS.H"

void Func_020004b0(void)
{
    struct KabeEventWork *work = Ram_HeapSlots->event_work;

    Func_020039d8();
    Func_02003b08(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgFieldStatueLikeDeriShrine, 1);
    work->unknown_15a = 1;
    Func_020039e0();
}

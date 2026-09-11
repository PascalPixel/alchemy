#include "types.h"
#include "scene.h"
#include "abi/ui/message/show_resolved_and_wait.h"

struct Work_080b28d4 {
    u8 unknown_000[0x3a4];
    u16 value_3a4;
};

void UiWork_FinalizePending(void);

void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(u32);

extern struct Work_080b28d4 *gIw;

void UiMessage_ShowResolvedAndWait(s32 value)
{
    s32 no;

    no = Ui_Check(gIw->value_3a4);
    UiWork_FinalizePending();
    value = Ui_Check2(value);
    UiWork_Create(value, 5, 0, (no << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
}

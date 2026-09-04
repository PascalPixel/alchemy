#include "types.h"

#define UiMessage_ShowResolvedAndWait Func_080b28d4

struct Work_080b28d4 {
    u8 unknown_000[0x3a4];
    u16 value_3a4;
};

s32 Func_0808a540(u16);
void UiWork_FinalizePending(void);
s32 Func_080b2884(s32 value);
void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(u32);

extern struct Work_080b28d4 *Data_03001f2c;

void UiMessage_ShowResolvedAndWait(s32 value)
{
    s32 no;

    no = Func_0808a540(Data_03001f2c->value_3a4);
    UiWork_FinalizePending();
    value = Func_080b2884(value);
    UiWork_Create(value, 5, 0, (no << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
}

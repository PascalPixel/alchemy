#include "types.h"

#define UiMessage_ShowResolvedAndRestoreState Func_080b2928

s32 Func_0808a540(u16);
void UiWork_FinalizePending(void);
s32 Func_080b2884(s32 value);
void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(u32);

extern void *Data_03001f2c;

struct State080b2928 {
    u8 padding0[0x380];
    void *mode_state;
    u8 padding1[0x20];
    u16 value;
};

void UiMessage_ShowResolvedAndRestoreState(s32 arg0)
{
    struct State080b2928 *state;
    void **slot;
    s32 value;
    u8 saved;

    state = Data_03001f2c;
    slot = &state->mode_state;
    saved = *(u8 *)((u8 *)*slot + 5);
    value = Func_0808a540(state->value);
    arg0 = Func_080b2884(arg0);
    *(u8 *)((u8 *)*slot + 5) = 13;
    UiWork_FinalizePending();
    UiWork_Create(arg0, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
    *(u8 *)((u8 *)state->mode_state + 5) = saved;
}

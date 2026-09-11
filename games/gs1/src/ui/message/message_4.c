#include "types.h"
#include "scene.h"

/* ui/message/show_resolved_and_wait.c */
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
    value = Ui_unk2(value);
    UiWork_Create(value, 5, 0, (no << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
}

/* ui/message/show_resolved_and_restore_state.c */
void UiWork_FinalizePending(void);

void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(u32);

extern void *gIw;

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

    state = gIw;
    slot = &state->mode_state;
    saved = *(u8 *)((u8 *)*slot + 5);
    value = Ui_Check(state->value);
    arg0 = Ui_unk2(arg0);
    *(u8 *)((u8 *)*slot + 5) = 13;
    UiWork_FinalizePending();
    UiWork_Create(arg0, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
    *(u8 *)((u8 *)state->mode_state + 5) = saved;
}

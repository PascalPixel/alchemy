#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* ui/message/show_and_wait.c */
void UiWork_FinalizePending(void);
void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(s32);
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gVal4[];

void UiMessage_ShowAndWait(s32 arg0)
{
    s32 *state = *(s32 **)ADDR_03001F2C;
    s32 value = Ui_Check(*(u16 *)&state[233]);
    s32 result = arg0;
    s8 mode;

    UiWork_FinalizePending();
    mode = *(s8 *)((u8 *)state + 0x3a9);
    if (mode == 2)
        result += gVal2 - gVal;
    if (mode == 0)
        result += gVal3 - gVal;
    if (*(s8 *)&state[235] != 0)
        result += gVal4 - gVal;
    UiWork_Create(result, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
}

/* ui/message/show_and_restore_state.c */
void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizePending(void);
void WaitFrames(u32);

extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gVal4[];

void UiMessage_ShowAndRestoreState(s32 message_id)
{
    s32 variant;
    s32 no;
    s8 mode;
    s8 flag;
    u8 saved;
    void *state;
    u8 **slot;

    state = *(void **)ADDR_03001F2C;
    slot = (u8 **)((u8 *)state + 0x380);
    saved = (*slot)[5];
    no = message_id;
    variant = Ui_Check(FIELD_AT_OFFSET(state, u16 *, 0x3A4));
    mode = FIELD_AT_OFFSET(state, s8 *, 0x3A9);
    if (mode == 2) {
        no += (s32)gVal2 - (s32)gVal;
    }
    if (mode == 0) {
        no += (s32)gVal3 - (s32)gVal;
    }
    flag = FIELD_AT_OFFSET(state, u8 *, 0x3AC);
    if (flag != 0) {
        no += (s32)gVal4 - (s32)gVal;
    }
    (*slot)[5] = 0xDU;
    UiWork_FinalizePending();
    UiWork_Create(no, 5, 0, (variant << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
    FIELD_AT_OFFSET(FIELD_AT_OFFSET(state, void **, 0x380), u8 *, 5) = saved;
}

/* ui/message/show_choice.c */
s32 UiMessage_ShowChoice(s32 arg0)
{
    u8 **slot = (u8 **)(*(u8 **)ADDR_03001F2C + 0x380);
    u8 saved = (*slot)[5];
    Ui_Do(*slot);
    arg0 = Ui_Place(7, 5, arg0);
    (*slot)[5] = saved;
    return arg0;
}

/* ui/message/show_choice_variant.c */
s32 UiMessage_ShowChoiceVariant(s32 arg0)
{
    u8 **slot = (u8 **)(*(u8 **)ADDR_03001F2C + 0x380);
    u8 saved = (*slot)[5];
    Ui_Do(*slot);
    arg0 = Ui_Place(7, 7, arg0);
    (*slot)[5] = saved;
    return arg0;
}

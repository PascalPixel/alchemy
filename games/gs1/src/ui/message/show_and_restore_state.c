#include "types.h"
#include "scene.h"
#include "abi/ui/message/show_and_restore_state.h"
#include "global_cells.h"

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

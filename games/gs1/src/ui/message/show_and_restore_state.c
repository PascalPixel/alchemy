#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizePending(void);
void WaitFrames(u32);
s32 Func_0808a540(u16);
extern u8 Data_00000c9b[];
extern u8 Data_00000cc6[];
extern u8 Data_00000cf1[];
extern u8 Data_00000d4c[];

void UiMessage_ShowAndRestoreState(s32 message_id) {
    s32 message_variant;
    s32 resolved_message_id;
    s8 message_mode;
    s8 has_modifier;
    u8 saved_window_mode;
    void *active_state;
    u8 **window_slot;

    active_state = *(void **)ADDR_03001F2C;
    window_slot = (u8 **)((u8 *)active_state + 0x380);
    saved_window_mode = (*window_slot)[5];
    resolved_message_id = message_id;
    message_variant = Func_0808a540(FIELD_AT_OFFSET(active_state, u16 *, 0x3A4));
    message_mode = FIELD_AT_OFFSET(active_state, s8 *, 0x3A9);
    if (message_mode == 2) {
        resolved_message_id += (s32)Data_00000cc6 - (s32)Data_00000c9b;
    }
    if (message_mode == 0) {
        resolved_message_id += (s32)Data_00000cf1 - (s32)Data_00000c9b;
    }
    has_modifier = FIELD_AT_OFFSET(active_state, u8 *, 0x3AC);
    if (has_modifier != 0) {
        resolved_message_id += (s32)Data_00000d4c - (s32)Data_00000c9b;
    }
    (*window_slot)[5] = 0xDU;
    UiWork_FinalizePending();
    UiWork_Create(resolved_message_id, 5, 0, (message_variant << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
    FIELD_AT_OFFSET(FIELD_AT_OFFSET(active_state, void **, 0x380), u8 *, 5) = saved_window_mode;
}

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

void UiMessage_ShowAndRestoreState(s32 arg0) {
    s32 temp_r8;
    s32 var_r5;
    s8 temp_r1;
    s8 temp_r3;
    u8 temp_sl;
    void *temp_r6;
    u8 **var_r7;

    temp_r6 = *(void **)ADDR_03001F2C;
    var_r7 = (u8 **)((u8 *)temp_r6 + 0x380);
    temp_sl = (*var_r7)[5];
    var_r5 = arg0;
    temp_r8 = Func_0808a540(FIELD_AT_OFFSET(temp_r6, u16 *, 0x3A4));
    temp_r1 = FIELD_AT_OFFSET(temp_r6, s8 *, 0x3A9);
    if (temp_r1 == 2) {
        var_r5 += (s32)Data_00000cc6 - (s32)Data_00000c9b;
    }
    if (temp_r1 == 0) {
        var_r5 += (s32)Data_00000cf1 - (s32)Data_00000c9b;
    }
    temp_r3 = FIELD_AT_OFFSET(temp_r6, u8 *, 0x3AC);
    if (temp_r3 != 0) {
        var_r5 += (s32)Data_00000d4c - (s32)Data_00000c9b;
    }
    (*var_r7)[5] = 0xDU;
    UiWork_FinalizePending();
    UiWork_Create(var_r5, 5, 0, (temp_r8 << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
    FIELD_AT_OFFSET(FIELD_AT_OFFSET(temp_r6, void **, 0x380), u8 *, 5) = temp_sl;
}

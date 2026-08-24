#include "types.h"
#include "global_cells.h"

s32 Func_0808a540(s32);
void UiWork_FinalizePending(void);
void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(s32);
extern u8 Data_00000c9b[];
extern u8 Data_00000cc6[];
extern u8 Data_00000cf1[];
extern u8 Data_00000d4c[];

void UiMessage_ShowAndWait(s32 arg0)
{
    s32 *state = *(s32 **)ADDR_03001F2C;
    s32 value = Func_0808a540(*(u16 *)&state[233]);
    s32 result = arg0;
    s8 mode;

    UiWork_FinalizePending();
    mode = *(s8 *)((u8 *)state + 0x3a9);
    if (mode == 2)
        result += Data_00000cc6 - Data_00000c9b;
    if (mode == 0)
        result += Data_00000cf1 - Data_00000c9b;
    if (*(s8 *)&state[235] != 0)
        result += Data_00000d4c - Data_00000c9b;
    UiWork_Create(result, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
}

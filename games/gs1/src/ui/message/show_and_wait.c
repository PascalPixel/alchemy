#include "types.h"
#include "scene.h"
#include "abi/ui/message/show_and_wait.h"
#include "global_cells.h"

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

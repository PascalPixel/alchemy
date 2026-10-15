#include "types.h"

void UiText_ShowPositionedMessageAndWait(const u8 *message, s32 mode);
void Func_02001f5c(void);
void Func_02001f64(void);
void Func_020020ac(s32 a);
extern const u8 MsgTheDoorIsHeld[];

void Func_02000168(void)
{
    Func_02001f5c();
    Func_020020ac(0);
    UiText_ShowPositionedMessageAndWait(MsgTheDoorIsHeld, 1);
    Func_02001f64();
}

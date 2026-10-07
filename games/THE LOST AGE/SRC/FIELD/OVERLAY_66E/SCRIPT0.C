#include "TYPES.H"
#include "text/MSG_IDS.H"

void Func_020010e0(void);
void Func_02001180(s32 a0);
void Func_02001138(s32 a0);
void Func_02001148(s32 a0, s32 a1);
void Func_020010e8(void);

s32 Func_02000054(void)
{
    Func_020010e0();
    Func_02001180(0);
    Func_02001138((s32)MsgTheItemsThatOneCarries);
    Func_02001148(10, 0);
    Func_020010e8();
    return 0;
}

s32 Func_0200007c(void)
{
    Func_020010e0();
    Func_02001180(0);
    Func_02001138((s32)MsgItemsPlacedBeforeMeAre);
    Func_02001148(10, 0);
    Func_020010e8();
    return 0;
}

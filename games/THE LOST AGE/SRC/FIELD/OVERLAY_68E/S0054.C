#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 UiText_ShowPositionedMessageAndWait();
s32 Func_02001408();
s32 Func_02001358();
extern u8 MsgAndTheOthersDrankOf[];

void Func_02000054(void)
{
    Call2(UiText_ShowPositionedMessageAndWait, (s32)MsgAndTheOthersDrankOf, 1);
    Call1(Func_02001408, 0x7e);
    Call1(Func_02001358, 1);
}


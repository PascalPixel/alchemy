#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_02003bc8();
s32 Func_02003c88();
s32 UiText_ShowPositionedMessageAndWait();
s32 Func_02003bd0();
extern u8 MsgFieldWindStoneWarning[];

void Func_02000548(void)
{
    Func_02003bc8();
    Call1(Func_02003c88, 0);
    Call2(UiText_ShowPositionedMessageAndWait, (s32)MsgFieldWindStoneWarning, 1);
    Func_02003bd0();
}


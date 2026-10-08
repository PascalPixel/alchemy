#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 UiText_ShowPositionedMessageAndWait();
s32 PartyInventory_FindOwner();
extern u8 MsgCheckedTheAltar[];

void Func_02001fd8(void)
{
    s32 t1;

    Call2(UiText_ShowPositionedMessageAndWait, (s32)MsgCheckedTheAltar, 1);
    t1 = Value1(PartyInventory_FindOwner, 0x1c3);
    if (t1 != -1) {
        Call2(UiText_ShowPositionedMessageAndWait, 0x22f7, 1);
        FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x15a), u16, 0) = 1;
    } else {
        Call2(UiText_ShowPositionedMessageAndWait, 0x22f7, 1);
    }
}


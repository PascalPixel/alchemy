#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_020001d4();
s32 Scheduler_AddOrUpdateCallback();
extern u8 Data_02004454[];
extern u8 Data_02004444[];
extern u8 Data_02004450[];
extern u8 Data_02004440[];
extern u8 MsgNotClearedYet[];
extern u8 Data_0200443c[];
extern u8 Data_02004434[];
extern u8 MsgContinueFromASanctum[];
extern u8 Func_020001f8[];

void Func_02000178(void)
{
    s32 t0;

    FIELD_AT(Data_02004454, u16, 0) = 0;
    t0 = Value1(Func_020001d4, FIELD_AT(Data_02004444, s32, 0));
    FIELD_AT(Data_02004450, u16, 0) = t0;
    FIELD_AT(Data_02004440, u16, 0) = (s32)MsgNotClearedYet;
    FIELD_AT(Data_0200443c, u16, 0) = (s32)MsgNotClearedYet;
    FIELD_AT(Data_02004434, u16, 0) = (s32)MsgContinueFromASanctum;
    Call2(Scheduler_AddOrUpdateCallback, (s32)Func_020001f8, 0x480);
}


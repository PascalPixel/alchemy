#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 MemAlloc();
s32 Func_02002174();
s32 Resource_FindFreeEntry();
s32 VramBlock_LoadCached();
s32 MemFree();
s32 Scheduler_AddOrUpdateCallback();
extern u8 IwramClearWords[];
extern u8 Data_02002bdc[];
extern u8 Data_02002334[];
extern u8 Data_02002bd8[];
extern u8 Func_0200125c[];
extern u8 Data_02002bda[];
extern u8 Data_02002cdc[];
extern u8 Data_02002cde[];

void Func_02001418(void)
{
    s32 t0;
    s32 t3;

    t0 = Value1(MemAlloc, 0xc00);
    ((s32 (*)())(s32)IwramClearWords)((s32)Data_02002bdc, 0x40);
    Call2(Func_02002174, (s32)Data_02002334, t0);
    t3 = Resource_FindFreeEntry();
    FIELD_AT(Data_02002bd8, u16, 0) = t3;
    Call3(VramBlock_LoadCached, ((t3 << 16) >> 16), 0xc00, t0);
    Call1(MemFree, t0);
    Call2(Scheduler_AddOrUpdateCallback, (s32)Func_0200125c, 0x480);
    FIELD_AT(Data_02002bda, u16, 0) = 0;
    FIELD_AT(Data_02002cdc, u16, 0) = 0;
    FIELD_AT(Data_02002cde, u16, 0) = -1;
}


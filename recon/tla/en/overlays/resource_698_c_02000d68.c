#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 MemAlloc();
s32 Func_020019bc();
s32 Resource_FindFreeEntry();
s32 VramBlock_LoadCached();
s32 MemFree();
s32 Scheduler_AddOrUpdateCallback();
extern u8 IwramClearWords[];
extern u8 Data_02002204[];
extern u8 Data_02001ede[];
extern u8 Data_02002200[];
extern u8 Func_02000aac[];
extern u8 Data_02002202[];
extern u8 Data_02002304[];
extern u8 MsgContinueFromASanctum[];
extern u8 Data_02002306[];

void Func_02000d68(void)
{
    s32 t0;
    s32 t3;

    t0 = Value1(MemAlloc, 0x800);
    ((s32 (*)())(s32)IwramClearWords)((s32)Data_02002204, 0x40);
    Call2(Func_020019bc, (s32)Data_02001ede, t0);
    t3 = Resource_FindFreeEntry();
    FIELD_AT(Data_02002200, u16, 0) = t3;
    Call3(VramBlock_LoadCached, ((t3 << 16) >> 16), 0x800, t0);
    Call1(MemFree, t0);
    Call2(Scheduler_AddOrUpdateCallback, (s32)Func_02000aac, 0x480);
    FIELD_AT(Data_02002202, u16, 0) = 0;
    FIELD_AT(Data_02002304, u16, 0) = (s32)MsgContinueFromASanctum;
    FIELD_AT(Data_02002306, u16, 0) = -1;
}


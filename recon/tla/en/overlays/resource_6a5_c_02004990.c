#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 MemAlloc();
s32 Func_02005704();
s32 Resource_FindFreeEntry();
s32 VramBlock_LoadCached();
s32 MemFree();
s32 Scheduler_AddOrUpdateCallback();
extern u8 IwramClearWords[];
extern u8 Data_02006a70[];
extern u8 Data_02005a24[];
extern u8 Data_02006a6c[];
extern u8 Func_020047d4[];
extern u8 Data_02006a6e[];
extern u8 Data_02006b70[];
extern u8 Data_02006b72[];

void Func_02004990(void)
{
    s32 t0;
    s32 t3;

    t0 = Value1(MemAlloc, 0xc00);
    ((s32 (*)())(s32)IwramClearWords)((s32)Data_02006a70, 0x40);
    Call2(Func_02005704, (s32)Data_02005a24, t0);
    t3 = Resource_FindFreeEntry();
    FIELD_AT(Data_02006a6c, u16, 0) = t3;
    Call3(VramBlock_LoadCached, ((t3 << 16) >> 16), 0xc00, t0);
    Call1(MemFree, t0);
    Call2(Scheduler_AddOrUpdateCallback, (s32)Func_020047d4, 0x480);
    FIELD_AT(Data_02006a6e, u16, 0) = 0;
    FIELD_AT(Data_02006b70, u16, 0) = 0;
    FIELD_AT(Data_02006b72, u16, 0) = -1;
}


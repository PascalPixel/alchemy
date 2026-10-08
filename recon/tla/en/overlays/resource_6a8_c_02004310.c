#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Scheduler_RemoveCallbackFar();
s32 Resource_ResetEntry();
s32 Runtime_ReleaseHeapBlock();
extern u8 Func_02004114[];

void Func_02004310(void)
{
    Call1(Scheduler_RemoveCallbackFar, (s32)Func_02004114);
    Call1(Resource_ResetEntry, FIELD_AT((FIELD_AT(0x30000dc, s32, 0) + 0x1604), u16, 0));
    Call1(Runtime_ReleaseHeapBlock, 0xdc);
}


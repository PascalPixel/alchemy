#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Scheduler_AddOrUpdateCallback();
extern u8 Func_020049e4[];

void Func_02004d00(void)
{
    Scheduler_AddOrUpdateCallback((s32)Func_020049e4, 0x480);
}


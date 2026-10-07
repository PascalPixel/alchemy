#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"

void Func_02000830(void)
{
    FIELD_AT((FIELD_AT(0x30000e0, s32, 0) + 0x34), u8, 0) = 1;
}


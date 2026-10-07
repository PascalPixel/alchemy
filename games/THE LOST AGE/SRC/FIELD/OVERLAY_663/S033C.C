#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 ObjectDispatch_SetSingleChildFlags();

s32 Func_0200033c(s32 a0)
{
    Call2(ObjectDispatch_SetSingleChildFlags, a0, 0);
    return 0;
}


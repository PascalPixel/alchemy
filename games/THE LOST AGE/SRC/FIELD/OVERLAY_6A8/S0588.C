#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_020045cc();

void Func_02000588(s32 a0)
{
    Call3(Func_020045cc, a0, 0x40, 0x300);
}


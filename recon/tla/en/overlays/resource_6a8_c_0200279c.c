#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_020044cc();
extern u8 Func_020003bc[];

void Func_0200279c(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 t0;

    t0 = Object_GetById();
    FIELD_AT((t0 + 0x64), u16, 0) = a3;
    FIELD_AT(((t0 + 0x64) - 1), u8, 0) = 0;
    FIELD_AT(t0, s32, 108) = (s32)Func_020003bc;
    Call3(Func_020044cc, a0, (a1 << 16), (a2 << 16));
}


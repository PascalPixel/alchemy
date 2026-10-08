#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();

void Func_02000610(void)
{
    s32 t0;

    t0 = Value1(Object_GetById, 9);
    FIELD_AT(FIELD_AT(t0, s32, 80), s16, 18) = (FIELD_AT(FIELD_AT(t0, s32, 80), s16, 18) + -768);
}


#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 ObjectMotion_SetActionVariant();

void Func_02000254(void)
{
    Call2(ObjectMotion_SetActionVariant, 0x19, 2);
}


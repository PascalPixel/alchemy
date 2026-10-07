#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 ObjectMotion_SetActionVariant();

void Func_02000674(s32 a0, s32 a1)
{
    Call2(ObjectMotion_SetActionVariant, a1, 2);
}


#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32 ObjectMotion_SetActionVariant();

void Func_02000674(s32 a0, s32 a1)
{
    Call2(ObjectMotion_SetActionVariant, a1, 2);
}

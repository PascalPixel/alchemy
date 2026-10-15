#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02004d9c();
s32 ObjectDispatch_ApplyValueToChildren();

s32 Func_020005dc(void)
{
    s32 t0;

    t0 = Value1(Object_GetById, 9);
    Call2(Func_02004d9c, t0, 3);
    Call2(ObjectDispatch_ApplyValueToChildren, t0, 0);
    return 0;
}

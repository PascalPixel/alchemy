#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
extern u8 gData_02002cde[];

void Func_020015d0(s32 a0)
{
    FIELD_AT(gData_02002cde, u16, 0) = a0;
}

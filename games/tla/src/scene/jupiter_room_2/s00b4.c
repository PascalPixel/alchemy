#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32       Func_0200231c();
extern u8 gData_02002b88[];
extern u8 gData_02002b60[];

void Func_020000b4(void)
{
    Call4(Func_0200231c, (s32)gData_02002b60, FIELD_AT(gData_02002b88, s32, 0),
        FIELD_AT(gData_02002b88, s32, 4), FIELD_AT(gData_02002b88, s32, 8));
}

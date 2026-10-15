#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32       Func_02003498();
s32       Func_02003378();
extern u8 gPartyState[];
extern u8 MsgHat[];
extern u8 gData_020036e4[];
extern u8 MsgCrownBoostsLuck[];
extern u8 gData_020036ea[];
extern u8 MsgHatBoostsAgility[];
extern u8 gData_020036f6[];

void Func_0200109c(void)
{
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgHat)
    {
        Call1(Func_02003498, (s32)gData_020036e4);
    }
    else
    {
        if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgCrownBoostsLuck)
        {
            Call1(Func_02003498, (s32)gData_020036ea);
            Call4(Func_02003378, 2, 0x480000, 0x1b80000, 0xff);
        }
        else
        {
            if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgHatBoostsAgility)
            {
                Call1(Func_02003498, (s32)gData_020036f6);
                Call4(Func_02003378, 2, 0x1180000, 0x1580000, 0xff);
                Call4(Func_02003378, 2, 0x1180000, 0x1780000, 0xff);
                Call4(Func_02003378, 2, 0x1280000, 0x1680000, 0xff);
                Call4(Func_02003378, 2, 0x1180000, 0x1680000, 0xe8);
                Call4(Func_02003378, 2, 0x2080000, 0x2780000, 0xff);
                Call4(Func_02003378, 2, 0x2080000, 0x2680000, 0xe8);
            }
        }
    }
}

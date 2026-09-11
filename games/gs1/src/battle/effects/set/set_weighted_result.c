#include "event_runtime.h"
#include "object_lookup.h"

extern struct EventPairWork1d6 Data_02000240;
extern u8 Value_00000021;
u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1);
s32 BattleFx_LookupResult(void *arg0);
void Func_0808b320(s32, s32);

void BattleFx_SetWeightedResult(s32 arg0, s32 arg1)
{
    register s32 first = arg0;
    register s32 second = arg1;
    register struct EventRuntime *runtime = Data_03001ebc;

    runtime->value_17c = BattleFx_GetWeightedResult(first, second);
    if (first == 98 && second == 0)
        Data_02000240.special = (u16)(s32)&Value_00000021;
    if (runtime->mode_19e == 3)
        BattleFx_LookupResult((u8 *)ObjectTable_Get(Data_02000240.object_id) + 8);
    Func_0808b320(first, second);
}

#include "EVENT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"

extern struct EventPairWork1d6 Data_02000240;
#define PARTY_STATE Data_02000240
extern u8 Value_00000021;
u16 Func_0808b05c(s32 arg0, s32 arg1);
#define BattleFx_GetWeightedResult Func_0808b05c
s32 Func_0808adf0(void *arg0);
#define BattleFx_LookupResult Func_0808adf0
void Func_0808b320(s32, s32);

void BattleFx_SetWeightedResult(s32 arg0, s32 arg1)
{
    register s32 first = arg0;
    register s32 second = arg1;
    register struct EventRuntime *runtime = Data_03001ebc;

    runtime->value_17c = BattleFx_GetWeightedResult(first, second);
    if (first == 98 && second == 0)
        PARTY_STATE.special = (u16)(s32)&Value_00000021;
    if (runtime->mode_19e == 3)
        BattleFx_LookupResult((u8 *)ObjectTable_Get(PARTY_STATE.object_id) + 8);
    Func_0808b320(first, second);
}

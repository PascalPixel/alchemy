#include "scene.h"
#include "abi/battle/effects/set/set_weighted_result.h"
#include "event_runtime.h"
#include "object_lookup.h"

extern struct EventPairWork1d6 gCell;
extern u8 Value_00000021;
u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1);
s32 BattleFx_LookupResult(void *arg0);

void BattleFx_SetWeightedResult(s32 arg0, s32 arg1)
{
    register s32 first = arg0;
    register s32 second = arg1;
    register struct EventRuntime *runtime = gWork;

    runtime->value_17c = BattleFx_GetWeightedResult(first, second);
    if (first == 98 && second == 0)
        gCell.special = (u16)(s32)&Value_00000021;
    if (runtime->mode_19e == 3)
        BattleFx_LookupResult((u8 *)ObjectTable_Get(gCell.object_id) + 8);
    Battle_Apply(first, second);
}

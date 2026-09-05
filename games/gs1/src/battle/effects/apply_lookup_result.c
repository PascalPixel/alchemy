#include "types.h"

s32 BattleEffect_LookupResult(void *);
s32 Func_0808ae74(s32, s32);

s32 BattleEffect_ApplyLookupResult(s32 arg0, s32 arg1) {
    return Func_0808ae74(BattleEffect_LookupResult((void *)arg0), arg1);
}

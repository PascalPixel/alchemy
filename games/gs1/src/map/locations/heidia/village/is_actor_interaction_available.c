#include "types.h"

s32 Func_0200326a(void);
s32 Func_02003114(s32);
s32 Func_02003186(s32);

s32 IsActorInteractionAvailable(s32 actor_id)
{
    if (Func_0200326a() == 0) {
        return 0;
    }
    if (Func_02003114(actor_id) != 0) {
        return 1;
    }
    {
        s32 result = Func_02003186(actor_id);

        /* branchless "result != 0" */
        return (u32)(result | -result) >> 31;
    }
}

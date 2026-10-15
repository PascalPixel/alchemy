#include "types.h"
#include "text/msg_ids.h"

s32 GameFlag_Test(s32 flag);

s32 Func_0804519c(void)
{
    if (GameFlag_Test(0x175))
        return (s32)MsgDjinnGiveTutorialB;
    return (s32)MsgDjinnGiveTutorialA;
}

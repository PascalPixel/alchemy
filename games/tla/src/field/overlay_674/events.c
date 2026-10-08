#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "call.h"
#include "text/msg_ids.h"

void GameFlag_SetBit();
void GameFlag_ClearBit();
void Func_02003fec();

void Func_020025a4(void)
{
    Call1((void (*)())GameFlag_SetBit, 0x218);
    return;
}

void Func_020025b0(void)
{
    Value1((s32 (*)())GameFlag_ClearBit, 0x218);
    return;
}

void Func_020025bc(void)
{
    Call7((void (*)())Func_02003fec, 65, 0, 4, 4, 65, 10, 1);
    return;
}

void Func_020025dc(void)
{
    Call7((void (*)())Func_02003fec, 65, 0, 2, 2, 71, 10, 1);
    return;
}

void Func_020025fc(void)
{
    Call7((void (*)())Func_02003fec, 65, 0, 1, 1, 68, 15, 1);
    return;
}

void Func_0200261c(void)
{
    Call7((void (*)())Func_02003fec, 65, 0, 2, 3, 75, 20, 1);
    return;
}

void Func_0200263c(void)
{
    Call7((void (*)())Func_02003fec, 65, 0, 4, 1, 78, 22, 1);
    return;
}

void Func_0200265c(void)
{
    Call7((void (*)())Func_02003fec, 65, 0, 1, 1, 79, 23, 1);
    return;
}

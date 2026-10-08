#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_020018a8();
void GameFlag_SetBit();
void GameFlag_Test();

void Func_02000e14(s32 arg0)
{
    s32 result0;

    result0 = Value1((s32 (*)())GameFlag_Test, 0x300);
    if (result0 != 0) goto L_02008e3a;
    if (arg0 != 3) goto L_02008e3a;
    Value3((s32 (*)())Func_020018a8, 11, 0, 0);
    Call1((void (*)())GameFlag_SetBit, 0x300);
L_02008e3a:
    return;
}

void Func_02000e3c(s32 arg0)
{
    s32 result0;

    result0 = Value1((s32 (*)())GameFlag_Test, 0x301);
    if (result0 != 0) goto L_02008e66;
    if (arg0 != 3) goto L_02008e66;
    Call3((void (*)())Func_020018a8, 15, 0, 0);
    Value1((s32 (*)())GameFlag_SetBit, 0x301);
L_02008e66:
    return;
}

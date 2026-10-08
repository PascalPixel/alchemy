#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_020005c4();
void Func_020005cc();
void Func_02000604();
void Func_02000614();
void Func_0200062c();

s32 Func_02000078(void)
{
    ((void (*)())Func_020005c4)();
    Call1((void (*)())Func_0200062c, 0);
    Call1((void (*)())Func_02000604, (s32)MsgScene65ENo18);
    Call2((void (*)())Func_02000614, 8, 0);
    ((void (*)())Func_020005cc)();
    return 0;
}

s32 Func_020000a0(void)
{
    ((void (*)())Func_020005c4)();
    Call1((void (*)())Func_0200062c, 0);
    Call1((void (*)())Func_02000604, (s32)MsgScene65ENo32);
    Call2((void (*)())Func_02000614, 9, 0);
    ((void (*)())Func_020005cc)();
    return 0;
}

#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

void Func_020005c4();
void Func_020005cc();
void Func_02000604();
void Func_02000614();
void Func_0200062c();

s32 Func_02000078(void)
{
    ((void (*)())Func_020005c4)();
    Call1((void (*)())Func_0200062c, 0);
    Call1((void (*)())Func_02000604, (s32)MsgIWonderIfThere);
    Call2((void (*)())Func_02000614, 8, 0);
    ((void (*)())Func_020005cc)();
    return 0;
}

s32 Func_020000a0(void)
{
    ((void (*)())Func_020005c4)();
    Call1((void (*)())Func_0200062c, 0);
    Call1((void (*)())Func_02000604, (s32)MsgWheneverVisitorsCome);
    Call2((void (*)())Func_02000614, 9, 0);
    ((void (*)())Func_020005cc)();
    return 0;
}

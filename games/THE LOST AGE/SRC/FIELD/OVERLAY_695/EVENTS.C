#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

void Func_02001060();
void Func_020011c0();
void GameFlag_Test();

s32 Func_02000fe8(void)
{
    s32 result0;
    s32 result1;

    result0 = Value1((s32 (*)())GameFlag_Test, 0x9a9);
    if (result0 == 0) goto L_0200900e;
    Call6((void (*)())Func_02001060, 0, 2, 1, 1, 2, 8);
L_0200900e:
    result1 = Value1((s32 (*)())GameFlag_Test, 0x9aa);
    if (result1 == 0) goto L_02009030;
    Call6((void (*)())Func_02001060, 0, 32, 1, 1, 14, 38);
L_02009030:
    return 0;
}

void Func_02001038(void)
{
    ((void (*)())Func_020011c0)();
    return;
}

#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

void Func_020034fc();
void Scheduler_AddOrUpdateCallback();
void Scheduler_RemoveCallbackFar();

void Func_020034d8(void)
{
    Value2((s32 (*)())Scheduler_AddOrUpdateCallback, &Func_020034fc, 0x480);
    return;
}

void Func_020034ec(void)
{
    Call1((void (*)())Scheduler_RemoveCallbackFar, &Func_020034fc);
    return;
}

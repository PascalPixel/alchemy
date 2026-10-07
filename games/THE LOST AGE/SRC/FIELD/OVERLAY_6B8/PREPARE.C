#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

void Func_02000e0c();
void Func_02000f24();

s32 Scene_PrepareMap(void)
{
    return 0;
}

void Func_0200091c(void)
{
    ((void (*)())Func_02000e0c)();
    return;
}

void Func_02000924(void)
{
    ((void (*)())Func_02000f24)();
    return;
}

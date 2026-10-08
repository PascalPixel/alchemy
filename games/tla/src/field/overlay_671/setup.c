#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "input.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02005714(s32 id, s32 a, s32 b);
void ObjectMotion_EnableActionAndResetMotion(s32 id);
void Func_020022d0(s32 id, s32 x, s32 z, s32 facing);
void Func_020011bc(s32 message, s32 value);

void Func_02000f50(s32 id)
{
    Func_02005714(id, 0, 0);
    ObjectMotion_EnableActionAndResetMotion(id);
    Object_SetModeById(id, 1);
}

void Func_02000f6c(void)
{
    Func_02000f50(11);
    Func_02000f50(12);
    Func_02000f50(13);
    Func_02000f50(14);
    Func_020022d0(11, 11, 9, 0);
    Func_020022d0(12, 11, 9, 0x8000);
    Func_020022d0(13, 10, 10, 0x4000);
    Func_020022d0(14, 10, 10, 0xc000);
}

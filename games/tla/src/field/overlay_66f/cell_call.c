#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02001144(s32 a, s32 b);
void Func_0200114c(void);
void Func_0200106c(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
void Func_0200103c(struct ObjectRuntime *object, s32 value);
void Func_02000da8(s32 value);
void Func_02000e28(s32 value);

void Func_020004f4(s32 id, s32 a, s32 b)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;

    /* FAKEMATCH: ⚓️ reads the cell before it sets up the call */
    asm("" : "+l"(x), "+l"(z));
    Func_0200106c(a, b, 1, 1, x, z);
}

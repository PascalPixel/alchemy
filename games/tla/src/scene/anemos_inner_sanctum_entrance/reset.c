#include "types.h"
#include "object_runtime.h"

extern s16                   gData_02001cde;
extern struct ObjectRuntime *gData_02001ce0[];
extern u16                   gData_02001de2;
extern u16                   gData_02001de4;
struct ObjectRuntime        *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
s32  Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

void Func_02000494(void)
{
    s32 i;

    for (i = 9; i < 15; i++)
    {
        struct ObjectRuntime *object = Object_GetById(i);
        u8                   *flag = (u8 *)object + 98;
        s32                   none = 0;
        s32                   on = 1;

        /* FAKEMATCH: ⚓️ builds both values before it stores either */
        asm("" : "+l"(none), "+l"(on));
        *flag = on;
        *(s32 *)((u8 *)object + 108) = none;
    }
}

void Func_020004b4(struct ObjectRuntime *object)
{
    if ((object->terrainHeight = Map_GetTerrainHeight(0, object->x, object->z)) >= -0x200000)
        ObjectDispatch_SetSingleChildFlags(object, 1);
    else
        ObjectDispatch_SetSingleChildFlags(object, 0);
}

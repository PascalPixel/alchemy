#include "types.h"
#include "object_runtime.h"

extern s16 gData_02001cde;
extern struct ObjectRuntime *gData_02001ce0[];
extern u16 gData_02001de2;
extern u16 gData_02001de4;
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

s32 Func_0200033c(struct ObjectRuntime *object)
{
    ObjectDispatch_SetSingleChildFlags(object, 0);
    return 0;
}

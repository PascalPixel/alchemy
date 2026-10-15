// Draft, differ 30 of 51: the listing rebuilds the list end 0xffff on every pass and so keeps one register
// fewer; here the constant of the first test lives in r10 through the loop. Three extra temporaries for the
// call arguments do not keep it in the loop.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "object_runtime.h"

struct YampiDesertCavePlacement {
    u16 id;
    s16 column;
    s16 row;
};

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);

void Func_02000b54(struct YampiDesertCavePlacement *list)
{
    while (list->id != 0xffff) {
        s32 id = list->id;
        struct ObjectRuntime *object = Object_GetById(id);
        s32 column = list->column;
        s32 row = list->row;

        object->collisionFlags &= 0xfe;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Func_02000c44(id, (u16)column, (u16)row, object->terrainId);
        list++;
    }
}

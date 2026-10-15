#include "types.h"
#include "object_runtime.h"

void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);

struct TileSprite {
    u8 unk00[18];
    u16 tile;
};

s32 Func_02000038(struct ObjectRuntime *object)
{
    struct TileSprite *sprite = object->animation;

    sprite->tile = 0x8000;
    return 0;
}

s32 Func_02000044(struct ObjectRuntime *object)
{
    ObjectDispatch_SetSingleChildFlags(object, 0);
    return 0;
}

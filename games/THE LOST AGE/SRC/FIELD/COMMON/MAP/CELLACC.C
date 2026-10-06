#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "MAP.H"

s32 GetWorldMapCollisionFar(s32 *position);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);
s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
s32 Func_080dbda8(s32 x, s32 z, s32 layer);
s32 Func_080dbd5c(s32 x, s32 z, s32 layer);
s32 Func_080201d0(s32 layer, s32 x, s32 z);

s32 Func_080dbda8(s32 x, s32 z, s32 layer)
{
    u8 code = Func_080dbb78(x, z, 2)->collision_code;

    if (code == 233)
        return 1;
    if (code == 234)
        return 2;
    if (code == 235)
        return 3;
    return 0;
}

u8 Func_080dbdc8(s32 x, s32 z, s32 layer)
{
    return Func_080dbb78(x, z, layer)->collision_code;
}

u8 Func_080dbdd4(s32 x, s32 z, s32 layer, s32 value)
{
    Func_080dbb78(x, z, layer)->collision_code = value;
    return value;
}

u8 Func_080dbde8(s32 x, s32 z, s32 layer)
{
    return Func_080dbb78(x, z, layer)->attribute_b;
}

u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value)
{
    Func_080dbb78(x, z, layer)->attribute_b = value;
    return value;
}

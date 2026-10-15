#include "types.h"
#include "ram_buffer.h"
#include "map.h"

s32 GetWorldMapCollisionFar(s32 *position);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);
s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
s32 Func_080dbda8(s32 x, s32 z, s32 layer);
s32 Func_080dbd5c(s32 x, s32 z, s32 layer);
s32 Func_080201d0(s32 layer, s32 x, s32 z);

s32 Func_080dbda8(s32 x, s32 z, s32 layer)
{
    u8 code = Func_080dbb78(x, z, 2)->collisionCode;

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
    return Func_080dbb78(x, z, layer)->collisionCode;
}

u8 Func_080dbdd4(s32 x, s32 z, s32 layer, s32 value)
{
    Func_080dbb78(x, z, layer)->collisionCode = value;
    return value;
}

s32 Func_080dbde8(s32 x, s32 z, s32 layer)
{
    return Func_080dbb78(x, z, layer)->attributeB;
}

u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value)
{
    Func_080dbb78(x, z, layer)->attributeB = value;
    return value;
}

u8 Func_080dbe08(s32 x, s32 z, s32 layer, s32 bits)
{
    s32 flags = Func_080dbde8(x, z, layer);
    u8 *cell = (u8 *)Func_080dbb78(x, z, layer);

    flags |= bits;
    cell[3] = flags;
    return flags;
}

u8 Func_080dbe44(s32 x, s32 z, s32 layer, s32 bits)
{
    s32 flags = Func_080dbde8(x, z, layer);
    u8 *cell = (u8 *)Func_080dbb78(x, z, layer);

    flags &= ~bits;
    cell[3] = flags;
    return flags;
}

s32 Func_080dbe80(s32 *a, s32 radius_a, s32 *b, s32 radius_b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 radius = radius_a + radius_b;

    if (dx > 0x400000 || dz > 0x400000)
        return -1;
    if (dy > radius << 16)
        return -1;
    if (dx * dx + dz * dz < radius * radius)
        return 1;
    return -1;
}

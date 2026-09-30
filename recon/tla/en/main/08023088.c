#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"
extern u8 ResourceTableEntries[];
extern u8 gCam[];

/* map/shared/Map_RenderAnimatedTileFrame.c */
struct MapBase {
    u16 unused;
    u16 offset;
};

extern u8 Map_TileDissolveOrder[];

s32 Map_GetScreenRelativePosition(struct Thing *obj, s32 *out)
{
    u8 *state = *(u8 **)((u32)&gCam);
    s32 *org = (s32 *)(state + 228);
    s32 a;
    s32 b;
    s32 x;
    s32 y;

    a = org[0] & 0xffff0000;
    b = org[1] & 0xffff0000;
    x = obj->field8 - a;
    y = obj->field16 - b;
    if ((u32)(x + 0x001fffff) <= 0x012ffffe && y > 0 && y < 0xe00000) {
        *out++ = x >> 16;
        *out = y >> 16;
        return 0;
    }
    *out++ = 0;
    *out = 0;
    return -1;
}

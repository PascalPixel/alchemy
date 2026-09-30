#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"

/* map/shared/Map_RenderAnimatedTileFrame.c */
struct MapBase {
    u16 unused;
    u16 offset;
};

void Map_RenderAnimatedTileFramesForObject(u8 *object)
{
    u32 pos;

    pos = 0;
    do {
        Map_RenderAnimatedTileFrame(object, pos);
        Map_RenderAnimatedTileFrame(object, pos + 1);
        Map_RenderAnimatedTileFrame(object, pos + 2);
        Map_RenderAnimatedTileFrame(object, pos + 3);
        pos += 4;
        WaitFrames(1);
    } while (pos <= 0x7f);
}

#include "types.h"

#define Map_RenderAnimatedTileFramesForObject Func_0800befc

s32 WaitFrames(s32);
void Map_RenderAnimatedTileFrame(u8 *object, u32 position);

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

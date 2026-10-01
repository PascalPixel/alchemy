#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"

/* map/shared/Map_RenderAnimatedTileFrame.c */
struct MapBase {
    u16 unused;
    u16 offset;
};

extern u8 ResourceTableEntries[];
extern u8 Map_TileDissolveOrder[];

/* ☀️'s, reading the object's resource and size where ⚓️'s object keeps
   them, 12 bytes earlier. */
void Map_RenderAnimatedTileFrame(u8 *object, u32 position)
{
    u16 *destination;
    s32 count;
    u32 row;
    u8 *table;
    u32 high_mask;
    u32 index_mask;
    u8 offset_mask;

    destination = (u16 *)(0x06010000
        + ((struct MapBase *)((u32)&ResourceTableEntries))[object[0x10]].offset);
    count = (object[0x14] * object[0x15]) / 64;
    row = 0;

    if (row < (u32)count) {
        table = Map_TileDissolveOrder;
        high_mask = 0xFF00;
        index_mask = 0x3F;
        offset_mask = 0x3E;

        /* 1行ごとに32セル進める。 */
        for (; row < (u32)count; row++, destination += 32, position++) {
            if ((u32)(position - 0x40) <= 0x3F) {
                u8 entry;
                u16 *cell;
                u32 parity;

                entry = table[(position + row * 16) & index_mask];
                cell = (u16 *)((u8 *)destination + (entry & offset_mask));
                parity = 1;
                parity &= entry;
                if (parity != 0)
                    *cell = *(u8 *)cell;
                else
                    *cell &= high_mask;
            }
        }
    }
}

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

void Map_RenderAllAnimatedTileFrames(u8 **tbl, s32 cnt)
{
    u8 **top;
    u8 **p;
    s32 n;
    u32 pos;
    u32 pos1;
    u32 pos2;
    u32 pos3;

    top = tbl;
    pos = 0;
    do {
        if (cnt > 0) {
            pos1 = pos + 1;
            p = top;
            asm volatile("" : "+l"(p)); /* FAKEMATCH: ⚓️ reloads the table between the first two positions */
            pos2 = pos + 2;
            pos3 = pos + 3;
            n = cnt;
            do {
                Map_RenderAnimatedTileFrame(*p, pos);
                Map_RenderAnimatedTileFrame(*p, pos1);
                Map_RenderAnimatedTileFrame(*p, pos2);
                n -= 1;
                Map_RenderAnimatedTileFrame(*p++, pos3);
            } while (n != 0);
        }
        WaitFrames(1U);
        pos += 4;
    } while (pos <= 0x7FU);
}

#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"

#include "VRAM_TAB.H"

struct AnimTrack {
    u8 unknown_00[16];
    u8 *script;
    u8 index;
};

struct AnimObject {
    u8 unknown_00[24];
    u8 frame;
    u8 unknown_19[15];
    struct AnimTrack *track;
};

void Animation_WaitForTrackEnd(struct AnimObject *object)
{
    u8 frame = object->frame;
    struct AnimTrack *track = object->track;
    s32 i;

    if (object != 0) {
        i = 0;
        if (track->script[track->index] != 241) {
            do {
                WaitFrames(1);
                i++;
                if (i > 89)
                    break;
                if (track->script[track->index] == 241)
                    break;
            } while (object->frame == frame);
        }
    }
}

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
        + ResourceTableEntries[object[16]].offset);
    count = (object[20] * object[21]) / 64;
    row = 0;

    if (row < (u32)count) {
        table = Map_TileDissolveOrder;
        high_mask = 0xFF00;
        index_mask = 0x3F;
        offset_mask = 0x3E;

        for (; row < (u32)count; row++, destination += 32, position++) {
            if ((u32)(position - 64) <= 63) {
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
            /* FAKEMATCH: ⚓️ reloads the table between the first two positions */
            asm volatile("" : "+l"(p));
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

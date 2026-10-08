#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "scene.h"

extern u32 gMapBlocks[];

u8 GetWorldMapTerrainBehavior(struct WorldPosition *position, s32 *terrain_kind)
{
    s32 selector = GetWorldMapCollision(position);
    s32 x = position->x;
    s32 y;
    s32 flag = 0;
    u32 tile_x;
    u32 index;
    u32 *tile;
    u32 kind;

    if (x < 0)
        x += 0x1fffff;
    tile_x = (x >> 21) & 31;
    y = position->y;
    if (y < 0)
        y += 0x1fffff;

    index = tile_x + (((y >> 21) & 31) << 5);
    tile = gMapBlocks + index;
    if (((u8 *)tile)[3] & 0x80)
        flag = 0x10;

    kind = (*tile << 2) >> 26;
    *terrain_kind = kind;
    if (kind > 40)
        flag = 0x20;
    if (kind == 36)
        flag = 0x40;
    if (kind == 12 || kind == 15)
        flag = 0x30;

    return WorldMap_TerrainBehaviorTable[flag + selector];
}

void Runtime_SetWorkTripleIfNonNegative(s32 value0, s32 value1, s32 value2)
{
    struct MapWork *work;

    work = Ram_HeapSlots->map_work;
    if (value0 >= 0) {
        work->shake_x = value0;
    }
    if (value1 >= 0) {
        work->shake_y = value1;
    }
    if (value2 >= 0) {
        work->shake_decay = value2;
    }
}

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Map_WaitWorkValuesBelow256(void)
{
    s32 *work;
    s32 cnt;

    work = (s32 *)Ram_HeapSlots->map_work;
    cnt = 0;
    if (work[1] > 255 || work[2] > 255) {
        goto body;
body:
        WaitFrames(1);
        ++cnt;
        if (cnt >= 300) {
            goto done;
        }
        if (work[1] > 255) {
            goto body;
        }
        if (work[2] > 255) {
            goto body;
        }
    }
done:
    work[3] = 0;
}

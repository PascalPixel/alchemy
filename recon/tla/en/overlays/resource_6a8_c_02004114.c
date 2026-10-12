/* Draft of overlay 6A8's falling rocks: Func_02004114 (update, differ 44 of
 * 57), Func_02004198 (spawn, differ 9 of 88) and Func_02004254 (start, differ
 * 83 of 83); diff each with --symbol. The work is overlay 6B9's SparkWork
 * (sparks.c): 0x500 bytes of tiles, 64 particles of 28 bytes, 64 sprites of
 * 40, then next, tile_base and resource.
 * The native rebuilds every offset and mask inside each loop and counts in
 * r8 (add r8, r3 with -1 in r3); for loops hoist them (differ 60, 48, 84),
 * so the loops are goto loops here.
 * Update: the timer sits in r3 and the constants in r2 in the native, the
 * other way round here, with or without a timer local.
 * Spawn: the native spills z, x, y in that order (here x, y, z) and masks
 * next with movs r2, #63 where this loads a halfword 63 from the pool.
 * Start: the native keeps the count in r8 and the tile in r10; here the
 * count is in r4, saved around the call, and the tile in r8. VramLoad as an
 * inline keeps 0x500 from being reused for the particle offset (the work
 * moves from r7 to the native's r6 with it). */
#include "types.h"
#include "ram_buffer.h"

struct Rock {
    s32 position[3];
    s32 velocity[3];
    s32 timer;
};

struct RockSprite {
    u8 unknown_00[5];
    u8 unknown_05;
    u8 unknown_06[2];
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[0x14];
    u16 unknown_1e;
    u8 unknown_20[8];
};

struct RockWork {
    u8 tiles[0x500];
    struct Rock rocks[64];
    struct RockSprite sprites[64];
    s16 next;
    u16 tile_base;
    s16 resource;
};

extern char ResourceId_FallingRock;

s32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 entry, s32 size, void *data);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02004384(void *resource, void *destination);
void Func_02004604(struct RockSprite *sprite, s32 a, s32 b, s32 c, s32 tile);
void Func_0200460c(struct RockSprite *sprite, struct Rock *rock);
void Func_02004614(struct Rock *rock, s32 a, s32 b);

static __inline__ s32 VramLoad(s32 entry, s32 size, void *data)
{
    /* FAKEMATCH: as in the scene layer, the inline parameters keep the size its own constant; called directly, the rock offset reuses it across the call. */
    return VramBlock_LoadCached(entry, size, data);
}

void Func_02004114(void)
{
    struct RockWork *work = Ram_HeapSlots->menu_runtime;
    struct Rock *rock = work->rocks;
    struct RockSprite *sprite = work->sprites;
    s32 i;

    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes rebuild the constants on every pass and count in r8, and a for loop hoists them. */
    i = 63;
loop:
    {
        s32 timer = rock->timer;

        if ((u32)timer <= 19) {
            sprite->tile = work->tile_base + ((timer >> 2) & 7) * 8;
            Func_0200460c(sprite, rock);
            Func_02004614(rock, 63, -0x8000);
            rock->timer++;
        }
        sprite++;
        rock++;
    }
    i--;
    if (i >= 0)
        goto loop;
}

void Func_02004198(s32 x, s32 y, s32 z, s32 count)
{
    struct RockWork *work = Ram_HeapSlots->menu_runtime;
    s32 i;

    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes rebuild the offsets on every pass, and a for loop hoists them. */
    if (count > 0) {
        i = count;
loop:
        ;{
        s32 angle = Random16Far();
        struct Rock *rock = &work->rocks[work->next];

        rock->timer = 0;
        rock->position[0] = x;
        rock->position[1] = y;
        rock->position[2] = z;
        Vector_AddPolarOffsetFar((Random16Far() << 3) + 0x80000, angle, rock->position);
        rock->velocity[0] = 0;
        rock->velocity[1] = 0x50000;
        rock->velocity[2] = 0;
        Vector_AddPolarOffsetFar((Random16Far() << 1) + 0x20000, angle, rock->velocity);
        work->next = (work->next + 1) & 63U;
        }
        i--;
        if (i != 0)
            goto loop;
    }
}

void Func_02004254(void)
{
    struct RockWork *work = Runtime_AllocateHeapBlockFar(220, sizeof(struct RockWork));
    struct Rock *rock;
    struct RockSprite *sprite;
    s32 entry;
    s32 tile;
    s32 i;

    Func_02004384(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
    entry = Resource_FindFreeEntry();
    tile = VramLoad(entry, 0x500, work);
    work->tile_base = tile;
    work->resource = entry;
    rock = work->rocks;
    sprite = work->sprites;
    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes rebuild the constants on every pass and count in r8, and a for loop hoists them. */
    i = 63;
loop:
    {
        Func_02004604(sprite, 8, 8, 0x40000000, tile);
        sprite->unknown_05 |= 0x20;
        sprite->palette = 0;
        sprite->priority = 0;
        sprite->unknown_1e = 240;
        i += -1;
        rock->timer = -1;
        sprite++;
        rock++;
    }
    if (i >= 0)
        goto loop;
    work->next = 0;
    Scheduler_AddOrUpdateCallback(Func_02004114, 0x480);
}

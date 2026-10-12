/* Near misses of Func_0200015c (57 native instructions, 60 differ), Func_020001e0 (88, 46) and
 * Func_0200029c (83, 84): the same calls, constants and stores. The draft hoists the address of
 * tile_base and the tile mask out of the 64-spark loops and counts in r7; the native builds them
 * inside the loop, keeps the work in r7 and counts in r8. Count-up and count-down loops measured
 * the same. Func_02000358 is adopted in games/tla/src/field/overlay_6b9/sparks.c. */
#include "types.h"
#include "ram_buffer.h"

extern char ResourceId_FallingRock;

struct Spark {
    s32 position[3];
    s32 velocity[3];
    s32 timer;
};

struct SparkSprite {
    u8 unknown_00[5];
    u8 unknown_05;
    u8 unknown_06[2];
    u16 tile : 10;
    u16 unknown_08_10 : 2;
    u16 unknown_08_12 : 4;
    u8 unknown_0a[0x14];
    u16 unknown_1e;
    u8 unknown_20[8];
};

struct SparkWork {
    u8 unknown_0000[0x500];
    struct Spark sparks[64];
    struct SparkSprite sprites[64];
    s16 next;
    u16 tile_base;
    s16 resource;
};

s32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void *Resource_GetTableEntry(s32 id);
s32 Resource_FindFreeEntry(void);
void Resource_ResetEntry(s32 entry);
s32 VramBlock_LoadCached(s32 entry, s32 size, void *data);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_02000560(void *resource, void *destination);
void Func_020005f0(struct SparkSprite *sprite, s32 a, s32 b, s32 c, s32 tile);
void Func_020005f8(struct SparkSprite *sprite, struct Spark *spark);
void Func_02000600(struct Spark *spark, s32 a, s32 b);

void Func_0200015c(void)
{
    struct SparkWork *work = Ram_HeapSlots->menu_runtime;
    struct Spark *spark = work->sparks;
    struct SparkSprite *sprite = work->sprites;
    s32 i;

    for (i = 0; i < 64; i++) {
        if (spark->timer >= 0 && spark->timer < 20) {
            sprite->tile = work->tile_base + ((spark->timer >> 2) & 7) * 8;
            Func_020005f8(sprite, spark);
            Func_02000600(spark, 63, 0xffff8000);
            spark->timer++;
        }
        sprite++;
        spark++;
    }
}

void Func_020001e0(s32 x, s32 y, s32 z, s32 count)
{
    struct SparkWork *work = Ram_HeapSlots->menu_runtime;
    struct Spark *spark;
    s32 angle;

    for (; count > 0; count--) {
        angle = Random16Far();
        spark = &work->sparks[work->next];
        spark->timer = 0;
        spark->position[0] = x;
        spark->position[1] = y;
        spark->position[2] = z;
        Vector_AddPolarOffsetFar((Random16Far() << 3) + 0x80000, angle, spark->position);
        spark->velocity[0] = 0;
        spark->velocity[1] = 0x50000;
        spark->velocity[2] = 0;
        Vector_AddPolarOffsetFar((Random16Far() << 1) + 0x20000, angle, spark->velocity);
        work->next = (work->next + 1) & 63;
    }
}

void Func_0200029c(void)
{
    struct SparkWork *work = Runtime_AllocateHeapBlockFar(220, sizeof(struct SparkWork));
    struct Spark *spark;
    struct SparkSprite *sprite;
    s32 resource;
    s32 tile;
    s32 i;

    Func_02000560(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
    resource = Resource_FindFreeEntry();
    tile = VramBlock_LoadCached(resource, 0x500, work);
    work->tile_base = tile;
    work->resource = resource;
    spark = work->sparks;
    sprite = work->sprites;
    for (i = 0; i < 64; i++) {
        Func_020005f0(sprite, 8, 8, 0x40000000, tile);
        sprite->unknown_05 |= 0x20;
        sprite->unknown_08_12 = 0;
        sprite->unknown_08_10 = 0;
        sprite->unknown_1e = 240;
        spark->timer = -1;
        sprite++;
        spark++;
    }
    work->next = 0;
    Scheduler_AddOrUpdateCallback(Func_0200015c, 0x480);
}

void Func_02000358(void)
{
    struct SparkWork *work = Ram_HeapSlots->menu_runtime;

    Scheduler_RemoveCallbackFar(Func_0200015c);
    Resource_ResetEntry(work->resource);
    Runtime_ReleaseHeapBlock(220);
}

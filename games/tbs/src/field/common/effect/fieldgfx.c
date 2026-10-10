#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "global_cells.h"
#include "btlpty.h"
#include "btlsys.h"
#include "types.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "system.h"
#include "resource.h"
#include "battle_effect_runtime.h"
#include "map_scroll.h"
#include "fxblend.h"
#include "io_reg.h"

void Resource_DecodeByteLz(const void *src, void *dst);

struct Mote {
    s32 state;
    s32 attr;
    s32 scale;
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_18;
    u16 phase;
    u16 pad;
};

struct MoteWork {
    s32 resource;
    s32 vram;
    struct Mote motes[32];
    u8 unknown_408[8];
};

struct Sparkle {
    u8 padding00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 matrix : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 padding0a[2];
    s32 pos_x;
    s32 height;
    s32 pos_z;
    s32 delay;
    u16 timer;
    u8 padding1e[2];
};

struct SparkleWork {
    u8 padding00[4];
    s32 tile_base;
    struct Sparkle sparkles[32];
    u8 padding408[4];
    s32 stopped;
};

struct SparkleFrame {
    s16 dy;
    u16 tile;
};

extern const struct SparkleFrame FieldFx_SparkleFrames[];

extern struct SparkleWork *gParticleWork;
#define SparkleMap (*(struct MapScrollWork **)((u8 *)&gParticleWork - 84))

union DustWord {
    u32 value;
    void *link;
};

struct DustParticle {
    union DustWord link;
    u32 attr01;
    u32 attr2;
    s32 pos_x;
    s32 pos_y;
    s32 pos_z;
    u8 pad18[4];
    u16 timer;
    u8 pad1e[2];
};

struct DustWork {
    s32 vram_entry;
    s32 tile_base;
    struct DustParticle particles[32];
    u8 pad408[8];
};

void Resource_DecodeByteLz(const void *source, void *destination);

void FieldMotes_Update(void)
{
    /* FAKEMATCH: needed to match: relative work pointers share one pool address; an unsigned-long frame read keeps size and flip stores together. */
    struct SparkleWork *work = gParticleWork;
    struct MapScrollWork *map = SparkleMap;
    u32 spawned = 0;
    u32 i;
    struct Sparkle *mote = work->sparkles;

    for (i = 0; i < 32; i++, mote++) {
        s32 x;
        s32 y;

        if (--mote->timer != 0xffff) {
            s32 *camera = &map->view_x;
            s32 camera_x = camera[0];
            s32 camera_z = camera[1];
            u16 age = mote->timer;
            u32 a;
            u32 b;

            if (GameFlag_TestFar(0x166)) {
                mote->timer++;
                mote->delay--;
            }
            a = Random16();
            b = Random16();
            x = ((mote->pos_x - camera_x) >> 16) + (((a & 1) + (b & 1)) >> 1) - 1;
            y = ((mote->pos_z - mote->height - camera_z) >> 16) - age;
            if ((u32)(x + 16) <= 255 && y >= -32 && y <= 159) {
                if (mote->timer < 60) {
                    mote->tile = work->tile_base + 16;
                    mote->delay += 3;
                } else if (mote->timer < 90) {
                    mote->tile = work->tile_base + 8;
                    mote->delay++;
                } else {
                    mote->tile = work->tile_base;
                }
                if ((*(unsigned long *)&gFrameCount >> 3) & 1)
                    mote->tile += 4;
                mote->x = x;
                mote->y = y - (mote->delay >> 2);
                mote->shape = 0;
                mote->size = 1;
                Runtime_PushSlotEntry(mote, 240);
            } else {
                mote->timer = 0;
            }
        }
        if (spawned < 8 && mote->timer == 0) {
            s32 *leader = map->origin;

            x = leader[0] + (Random16() << 8) - 0x800000;
            y = leader[2] + (Random16() << 8) - 0x800000;
            mote->pos_z = y;
            mote->pos_x = x;
            mote->height = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
            mote->timer = 120;
            mote->delay = 0;
            spawned++;
        }
    }
}

void FieldMotes_Start(void)
{
    struct MoteWork *work;
    struct Mote *mote;
    u8 *tiles;
    u32 i;
    u32 size;
    volatile u32 zero;
    volatile u16 *reg;

    size = 0x410;
    work = Runtime_AllocateBlock(29, size);
    mote = work->motes;
    /* FAKEMATCH: needed to match: reusing the allocation size for the cleared word keeps it in r1. */
    size = 0;
    zero = size;
    Dma_Set((void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x104, REG_DMA3);
    tiles = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz(FieldFx_MoteTiles, tiles);
    work->resource = Resource_FindFreeEntry();
    work->vram = VramBlock_LoadCached(work->resource, 0x300, tiles);
    Runtime_ReleaseHeapBlock(14);
    for (i = 0; i < 32; i++) {
        s32 *pos = ((struct MapScrollWork *)gMapWork[0])->origin;
        /* FAKEMATCH: needed to match: reusing the integer field cursor for the height x argument keeps the stores in r1. */
        u32 p = (u32)mote;
        s32 z;
        *(s32 *)p = 0;
        p += sizeof(s32);
        *(s32 *)p = 0x40000400;
        p += sizeof(s32);
        *(s32 *)p = 0xd400;
        p = pos[0];
        z = pos[2];
        mote->x = (s32)p;
        mote->z = z;
        mote->y = Map_GetTerrainHeightFar(0, (s32)p >> 16, z >> 16) << 16;
        mote->phase = (i & 15) + 1;
        mote++;
    }
    do { s32 v; v = 0x3f00; reg = &REG_BLDCNT; *reg = v; v = 0x1008; reg++; *reg = v; reg++; *reg = 0; } while (0);
    Scheduler_AddOrUpdateCallback((s32)(FieldMotes_Update), 0xc80);
}

void FieldEffect_UpdateSparkles(void)
{
    /* FAKEMATCH: needed to match: relative work pointers share one pool address; an unsigned-long frame read keeps size and flip stores together. */
    struct SparkleWork *work;
    u32 spawned;
    u32 i;
    struct MapScrollWork *map;
    s32 *camera;
    s32 spawn_x;
    s32 spawn_z;
    struct Sparkle *sparkle;
    const u16 *frame;
    s32 *leader;
    s32 camera_x;
    s32 camera_z;
    s32 sx;
    s32 sy;
    s32 delay;
    s32 x;
    u32 a;
    u32 b;
    u32 frames;

    work = gParticleWork;
    spawned = 0;
    map = SparkleMap;
    camera = &map->view_x;
    spawn_x = 0;
    spawn_z = 0;
    i = 0;
    sparkle = work->sparkles;
    delay = 0;
    sx = 0;
    sy = 0;
    do {
        if (sparkle->timer != 0) {
            camera_x = camera[0];
            camera_z = camera[1];
            if (GameFlag_TestFar(0x166) != 0)
                sparkle->timer++;
            frame = (const u16 *)&FieldFx_SparkleFrames[sparkle->timer >> 1];
            a = Random16();
            b = Random16();
            x = ((sparkle->pos_x - camera_x) >> 16) + (((a & 1) + (b & 1)) >> 1);
            sx = x - 1;
            sy = (sparkle->pos_z - sparkle->height - camera_z) / 0x10000 + *(const s16 *)frame;
            frame++;
            if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159) {
                sparkle->tile = work->tile_base + *frame;
                sparkle->x = sx;
                sparkle->shape = 0;
                sparkle->size = 1;
                frames = *(unsigned long *)&gFrameCount;
                sparkle->y = sy;
                sparkle->matrix = ((frames >> 1) & 1) << 3;
                Runtime_PushSlotEntry(sparkle, 240);
            }
            sparkle->timer--;
        }
        if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
            if (delay != 0) {
                sparkle->pos_x = spawn_x;
                sparkle->pos_z = spawn_z;
                sparkle->height = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
                sparkle->timer = 62 - delay;
                sparkle->delay = 0;
                spawned++;
                delay += 4;
            } else if ((Random16() & 255) == 0) {
                leader = map->origin;
                spawn_x = leader[0] + (Random16() << 8) - 0x800000;
                spawn_z = leader[2] + (Random16() << 8) - 0x800000;
                sparkle->pos_x = spawn_x;
                sparkle->pos_z = spawn_z;
                sparkle->height = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
                sparkle->timer = 30;
                sparkle->delay = delay;
                spawned++;
                delay = 4;
            }
        }
        i++;
        sparkle++;
    } while (i <= 31);
}

void FieldEffect_InitSparkles(void)
{
    u32 size = 0x410;
    struct DustWork *work = Runtime_AllocateBlock(29, size);
    struct DustParticle *p = work->particles;
    u8 *buf;
    u32 i;
    s32 clear;
    volatile u32 zero;

    /* FAKEMATCH: needed to match: reusing the allocation size for the fill word keeps zero in r1; a separate helper value differs by four instructions. */
    size = 0;
    zero = size;
    Dma_Set(&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x104, REG_DMA3);
    buf = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz(FieldFx_SparkleTiles, buf);
    work->vram_entry = Resource_FindFreeEntry();
    work->tile_base = VramBlock_LoadCached(work->vram_entry, 0x200, buf);
    Runtime_ReleaseHeapBlock(14);
    i = 0;
    clear = 0;
loop:
    {
        struct MapScrollWork *view = gMapWork[0];
        s32 *leader;
        /* FAKEMATCH: needed to match: an integer field cursor keeps the view load order; reusing it for the height x argument keeps the leader in r2. Separate temporaries differ by 14 instructions. */
        u32 attr;
        s32 z;

        attr = (u32)p;
        *(u32 *)attr = clear;
        attr += sizeof(union DustWord);
        leader = view->origin;
        *(u32 *)attr = 0x40000400;
        attr += sizeof(union DustWord);
        *(u32 *)attr = 0xd400;
        attr = leader[0];
        z = leader[2];
        p->pos_x = clear;
        p->pos_z = clear;
        p->pos_y = Map_GetTerrainHeightFar(0, (s32)attr >> 16, z >> 16) << 16;
        p->timer = (i & 15) + 1;
    }
    i++;
    p++;
    if (i < 32)
        goto loop;
    Scheduler_AddOrUpdateCallback((s32)(FieldEffect_UpdateSparkles), 0xc80);
}

void BattleFx_StartTwelveFrameBlend(void)
{
    struct FieldBlendWork *work;
    struct BattleEffectBuffers *buffers;
    volatile u32 zero;

    work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
    buffers = gPaletteBufferWork;
    zero = 0;
    Dma_Set((const void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x7e2, REG_DMA3);
    BattleFx_BuildBuffer(0x10003, buffers, work, 1);
    BattleFx_BuildBuffer(0x10005, buffers, work->to, 1);
    BattleFx_InterpolateBuffers(work->to, work->from, work->delta, 12);
    BattleFx_BuildBuffer((s32)work, 0, buffers->target, 1);
    work->timer = 600;
    work->enabled = 1;
    Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

void BattleFx_SetBlock30ValuesMaxZero(void)
{
    struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
    state->timer = 0x7FFF;
    state->enabled = 0;
}

void BattleFx_SetBlock30Values12Zero(void)
{
    struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
    state->timer = 12;
    state->enabled = 0;
}

void BattleFx_SetBlock30Values128One(void)
{
    struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
    state->timer = 128;
    state->enabled = 1;
}

void BattleFx_StartBufferBlend(s32 from, s32 to)
{
    struct FieldBlendWork *work;
    struct BattleEffectBuffers *buffers;
    volatile u32 zero;
    s16 *target;

    work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
    buffers = gPaletteBufferWork;
    zero = 0;
    Dma_Set((const void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x7e2, REG_DMA3);
    BattleFx_BuildBuffer(from, buffers, work, 1);
    target = work->to;
    BattleFx_BuildBuffer(to, buffers, target, 1);
    BattleFx_InterpolateBuffers(target, work->from, work->delta, 12);
    BattleFx_BuildBuffer((s32)work, 0, buffers->target, 1);
    work->timer = 120;
    work->enabled = 0;
    Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

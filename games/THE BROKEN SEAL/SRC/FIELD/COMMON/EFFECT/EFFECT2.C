/* Field effects: set up 32 motes on the terrain around the map position, the blend registers and their update callback. */
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "BATTLE_EFFECT_RUNTIME.H"

void Resource_DecodeByteLz(const void *src, void *dst);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *src);
s32 Map_GetTerrainHeightFar(s32, s32, s32);
void Unnamed_08094bbc(void);

struct Mote {
    s32 state;
    s32 attr;
    s32 scale;
    s32 x;
    s32 y;
    s32 z;
    s32 unused;
    u16 phase;
    u16 pad;
};

struct MoteWork {
    s32 resource;
    s32 vram;
    struct Mote motes[32];
};

struct Blend { u16 cnt; u16 alpha; u16 y; };

extern s32 **gMapWork;
extern const u8 FieldFx_MoteTiles[];

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

struct MapPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct MapWork {
    struct MapPosition *leader;
    u8 padding04[0xe0];
    s32 camera_x;
    s32 camera_z;
};

/* FAKEMATCH: the map work pointer is reached 84 bytes below the sparkle work
   pointer, so both loads share one pool address. */
extern struct SparkleWork *gParticleWork;
#define SparkleMap (*(struct MapWork **)((u8 *)&gParticleWork - 84))

/* The frame counter is an unsigned long: a type no sparkle field shares, so
   reading it does not keep the size store apart from the flip store. */
extern unsigned long Data_03001e40;
extern const struct SparkleFrame Data_0809f024[];
s32 GameFlag_TestFar(s32 flag);
u32 Random16(void);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Runtime_PushSlotEntry(void *entry, s32 value);

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

struct FieldView {
    s32 *leader;
};

extern const u8 Data_080a00b8[];
void *Runtime_AllocateBlock(s32 slot, s32 size);
void Resource_DecodeByteLz(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
void Runtime_ReleaseHeapBlock(s32 slot);
void FieldEffect_UpdateSparkles(void);

static __inline__ void ClearDustWork(struct DustWork *work)
{
    volatile u32 zero;
    /* FAKEMATCH: builds the fill zero in r1 */
    register u32 value asm("r1") = 0;

    zero = value;
    Dma_Set(&zero, work, 0x85000104, (volatile u32 *)0x040000d4);
}

s32 BattleFx_BuildBuffer(s32 source, void *reference, void *destination, s32 mode);
void BattleFx_InterpolateBuffers(s16 *from, s16 *to, s16 *step, s32 frames);
void BattleFx_UpdateStormFlash(void);

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

struct EffectBlockState {
    u8 filler[0x1F80];
    u16 field_1f80;
    u16 field_1f82;
};

void FieldMotes_Start(void)
{
    struct MoteWork *work;
    struct Mote *mote;
    u8 *tiles;
    u32 i;
    volatile u32 zero;
    volatile u16 *reg;

    work = Runtime_AllocateBlock(29, 0x410);
    mote = work->motes;
    { register u32 z asm("r1") = 0; /* FAKEMATCH: the cleared word goes through r1 */
    zero = z; }
    Dma_Set((void *)&zero, work, 0x85000104, (volatile u32 *)0x040000d4);
    tiles = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz(FieldFx_MoteTiles, tiles);
    work->resource = Resource_FindFreeEntry();
    work->vram = VramBlock_LoadCached(work->resource, 0x300, tiles);
    Runtime_ReleaseHeapBlock(14);
    for (i = 0; i < 32; i++) {
        s32 *pos = *gMapWork;
        register s32 *p asm("r1") = &mote->state; /* FAKEMATCH: steps the fields through r1 */
        s32 x, z;
        *p++ = 0;
        *p++ = 0x40000400;
        *p = 0xd400;
        x = pos[0];
        z = pos[2];
        mote->x = x;
        mote->z = z;
        mote->y = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
        mote->phase = (i & 15) + 1;
        mote++;
    }
    do { s32 v; v = 0x3f00; reg = (volatile u16 *)0x04000050; *reg = v; v = 0x1008; reg++; *reg = v; reg++; *reg = 0; } while (0);
    Scheduler_AddOrUpdateCallback((s32)(Unnamed_08094bbc), 0xc80);
}

/* field/common/effect/sparkles.c */

/* Draw the ground sparkles that twinkle around the leader, and every so often
   start a burst of four at a random spot nearby. */
void FieldEffect_UpdateSparkles(void)
{
    struct SparkleWork *work;
    u32 spawned;
    u32 i;
    struct MapWork *map;
    s32 *camera;
    s32 spawn_x;
    s32 spawn_z;
    struct Sparkle *sparkle;
    const u16 *frame;
    struct MapPosition *leader;
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
    camera = &map->camera_x;
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
            frame = (const u16 *)&Data_0809f024[sparkle->timer >> 1];
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
                frames = Data_03001e40;
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
                leader = map->leader;
                spawn_x = leader->x + (Random16() << 8) - 0x800000;
                spawn_z = leader->z + (Random16() << 8) - 0x800000;
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

/* FieldEffect_InitSparkles: allocate and clear the sparkle work, load the
   sparkle tiles into a cached VRAM block, place the 32 particles on the
   ground under the leader with staggered timers and schedule
   FieldEffect_UpdateSparkles. */
void FieldEffect_InitSparkles(void)
{
    struct DustWork *work = Runtime_AllocateBlock(29, 0x410);
    struct DustParticle *p = work->particles;
    u8 *buf;
    u32 i;
    s32 clear;

    ClearDustWork(work);
    buf = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz(Data_080a00b8, buf);
    work->vram_entry = Resource_FindFreeEntry();
    work->tile_base = VramBlock_LoadCached(work->vram_entry, 0x200, buf);
    Runtime_ReleaseHeapBlock(14);
    i = 0;
    clear = 0;
loop:
    {
        struct FieldView *view = ((struct FieldView *)gMapWork);
        /* FAKEMATCH: keeps the leader pointer in r2 */
        register s32 *leader asm("r2");
        union DustWord *attr;
        s32 x;
        s32 z;

        /* FAKEMATCH: loads the view before the particle pointer copy */
        asm volatile ("" : : "r"(view));
        attr = &p->link;
        (attr++)->link = (void *)clear;
        leader = view->leader;
        (attr++)->value = 0x40000400;
        attr->value = 0xd400;
        x = leader[0];
        z = leader[2];
        p->pos_x = clear;
        p->pos_z = clear;
        p->pos_y = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
        p->timer = (i & 15) + 1;
    }
    i++;
    p++;
    if (i < 32)
        goto loop;
    Scheduler_AddOrUpdateCallback((s32)(FieldEffect_UpdateSparkles), 0xc80);
}

/* Builds two 0xa80-byte buffers and the per-frame step between them for a
   twelve-frame blend, then schedules the blend. */
void BattleFx_StartTwelveFrameBlend(void)
{
    u8 *work;
    struct BattleEffectBuffers *buffers;
    volatile u32 zero;
    s32 value;
    s32 one;
    u8 *target;
    u16 *frames;

    work = Runtime_AllocateBlock(30, 0x1f88);
    buffers = Data_03001ed0;
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x850007e2, (volatile u32 *)0x040000d4);
    BattleFx_BuildBuffer(0x10003, buffers, work, 1);
    BattleFx_BuildBuffer(0x10005, buffers, work + 0xa80, 1);
    BattleFx_InterpolateBuffers((s16 *)(work + 0xa80), (s16 *)work, (s16 *)(work + 0x1500), 12);
    BattleFx_BuildBuffer((s32)work, 0, buffers->buffer_e00, 1);
    /* FAKEMATCH: the halfword constants pass through an int so GCC builds them
       with mov instead of loading them from the pool, and the block pointer
       itself is advanced to the second count. */
    frames = (u16 *)(work + 0x1f80);
    value = 600;
    *frames = value;
    work += 0x1f82;
    one = 1;
    *(u16 *)work = one;
    Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

void BattleFx_SetBlock30ValuesMaxZero(void)
{
    struct EffectBlockState *state = Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 0x7FFF;
    state->field_1f82 = 0;
}

void BattleFx_SetBlock30Values12Zero(void)
{
    struct EffectBlockState *state = Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 12;
    state->field_1f82 = 0;
}

void BattleFx_SetBlock30Values128One(void)
{
    struct EffectBlockState *state = Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 128;
    state->field_1f82 = 1;
}

/* Builds the buffers for two effect sources and the per-frame step between
   them for a twelve-frame blend, then schedules the blend. The work block
   holds the from buffer, the to buffer at +0xa80, the step at +0x1500 and
   the frame count and position at +0x1f80. */
void BattleFx_StartBufferBlend(s32 from, s32 to)
{
    u8 *work;
    struct BattleEffectBuffers *buffers;
    volatile u32 zero;
    s32 value;
    u8 *target;
    u16 *frames;

    work = Runtime_AllocateBlock(30, 0x1f88);
    buffers = Data_03001ed0;
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x850007e2, (volatile u32 *)0x040000d4);
    BattleFx_BuildBuffer(from, buffers, work, 1);
    target = work + 0xa80;
    BattleFx_BuildBuffer(to, buffers, target, 1);
    BattleFx_InterpolateBuffers((s16 *)target, (s16 *)work, (s16 *)(work + 0x1500), 12);
    BattleFx_BuildBuffer((s32)work, 0, buffers->buffer_e00, 1);
    frames = (u16 *)(work + 0x1f80);
    /* FAKEMATCH: the halfword constants pass through an int so GCC builds
       them with mov instead of loading them from the pool. */
    value = 120;
    *frames = value;
    value = 0;
    *(u16 *)(work + 0x1f82) = value;
    Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

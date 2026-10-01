#include "TYPES.H"
#include "DMA.H"
#include "SYSTEM.H"

s32 GameFlag_TestFar(s32);
void Audio_PlayCue(s32);
void BattleFx_ApplyColorToTargetBuffer(void *buffer, s32 mode);

struct FlashTarget {
    u8 unknown_0000[0x1880];
    u8 palette[0x1181];
    u8 step;
    u8 phase;
};

struct FlashWork {
    u8 buffers[0x1f80];
    s16 timer;
    s16 enabled;
    s16 loud;
};

extern u8 *Data_03001ec8[];

/* Counts the storm timer down: at zero it re-arms at a random interval and
   plays the thunder cue, then flashes the target buffer for two steps. */
void BattleFx_UpdateStormFlash(void)
{
    struct FlashWork *work = (struct FlashWork *)Data_03001ec8[0];
    struct FlashTarget *target = (struct FlashTarget *)Data_03001ec8[2];
    s16 *timer = &work->timer;

    if (*timer < 0)
        return;
    if (GameFlag_TestFar(358))
        work->timer = 128;
    switch ((*timer)--) {
    case 0:
        if (work->enabled != 0) {
            u32 a = Random16();
            u32 b = Random16();
            work->timer = ((a * 400) >> 16) - ((b * 100) >> 16) + 150;
            if (work->loud != 0)
                Audio_PlayCue(172);
            else
                Audio_PlayCue(171);
        }
    case 5:
    case 10:
        BattleFx_ApplyColorToTargetBuffer(work, 1);
        Dma_Set(work->buffers + 0x1500, target->palette, 0x840002a0, (volatile u32 *)0x040000d4);
        target->step = 12;
        target->phase = 0;
        break;
    case 1:
    case 6:
    case 11:
        BattleFx_ApplyColorToTargetBuffer(work->buffers + 0xa80, 1);
        target->step = 1;
        target->phase = 0;
        break;
    }
}

struct GroundParticle {
    s32 state;
    u32 attributes;
    s32 tile;
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_18;
    u16 frame;
    u16 unknown_1e;
};

struct GroundParticleWork {
    s32 entry;
    s32 vram;
    struct GroundParticle particles[32];
    s32 unknown_408[2];
};

s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
s32 Resource_FindFreeEntry(void);
void Resource_DecodeByteLz(const void *source, void *destination);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void BattleFx_SetQueuedSoundAndPlay(s32 sound);
void Unnamed_08094820(void);

extern s32 **gMapWork;
extern const u8 FieldFx_GroundParticleTiles[];

/* Starts the ground particles: clears their work block, loads their tiles
   into VRAM, lays all 32 on the ground at the map's origin with staggered
   frames, sets the blend registers and schedules their update. */
void Unnamed_08094ac8(void)
{
    struct GroundParticleWork *work;
    s32 *origin;
    struct GroundParticle *particle;
    u32 *words;
    volatile u32 fill;
    volatile u16 *blend;
    u32 i;
    s32 value;

    /* FAKEMATCH: one cursor holds the fill value, then the tile buffer, then
       each particle's first words, which gives the reference its register;
       the blend writes are a block that runs once, which keeps the callback's
       arguments behind them. */
    work = Runtime_AllocateBlock(29, sizeof(struct GroundParticleWork));
    origin = *gMapWork;
    BattleFx_SetQueuedSoundAndPlay(170);
    words = 0;
    particle = work->particles;
    fill = (u32)words;
    Dma_Set((const void *)&fill, work, 0x85000000 | (sizeof(struct GroundParticleWork) / 4),
        (volatile u32 *)0x040000d4);
    words = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz(FieldFx_GroundParticleTiles, words);
    work->entry = Resource_FindFreeEntry();
    work->vram = VramBlock_LoadCached(work->entry, 0x300, words);
    Runtime_ReleaseHeapBlock(14);
    for (i = 0; i < 32; i++) {
        s32 x;
        s32 z;

        words = (u32 *)particle;
        *words++ = 0;
        *words++ = 0x40000400;
        *words = 0xd400;
        x = origin[0];
        z = origin[2];
        particle->x = x;
        particle->z = z;
        particle->y = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
        particle->frame = (i & 15) + 1;
        particle++;
    }
    do {
        blend = (volatile u16 *)0x04000050;
        value = 0x3f00;
        *blend = value;
        value = 0x1008;
        *++blend = value;
        *++blend = 0;
    } while (0);
    Scheduler_AddOrUpdateCallback(Unnamed_08094820, 0xc80);
}



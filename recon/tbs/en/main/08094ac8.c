/* Draft, not exact (2026-09-24): 74 differing halfwords, 238 of 244 bytes.
   Residual: the loop zero lands in r6 where the reference gives the three
   leading particle words a walking pointer in r6 and holds the zero in r4,
   saved around the terrain call (frame 8, not 4). Merging the pointer with
   the LZ buffer variable, zero types and an early zero did not move it. */
#include "DMA.H"

struct FxParticle {
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

struct FxWork {
    s32 entry;
    s32 vram;
    struct FxParticle particles[32];
};

u8 *Runtime_AllocateBlock(s32 slot, u32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
s32 Resource_FindFreeEntry(void);
void Resource_DecodeByteLz(const void *source, void *destination);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void BattleFx_SetQueuedSoundAndPlay(s32 sound);
void Unnamed_08094820(void);

extern s32 **Data_03001e70;
extern const u8 Data_0809ff58[];

void Unnamed_08094ac8(void)
{
    struct FxWork *work;
    s32 *origin;
    struct FxParticle *particle;
    u8 *buffer;
    volatile u32 fill;
    volatile u16 *blend;
    u32 i;
    s32 zero;
    s32 value;

    work = (struct FxWork *)Runtime_AllocateBlock(29, 0x410);
    origin = *Data_03001e70;
    BattleFx_SetQueuedSoundAndPlay(170);
    particle = work->particles;
    fill = 0;
    Dma_Set((const void *)&fill, work, 0x85000104, (volatile u32 *)0x040000d4);
    buffer = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz(Data_0809ff58, buffer);
    work->entry = Resource_FindFreeEntry();
    work->vram = VramBlock_LoadCached(work->entry, 0x300, buffer);
    Runtime_ReleaseHeapBlock(14);
    for (i = 0; i < 32; i++) {
        s32 x;
        s32 z;
        u32 *words;

        zero = 0;
        words = (u32 *)particle;
        *words++ = zero;
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
    blend = (volatile u16 *)0x04000050;
    value = 0x3f00;
    *blend = value;
    value = 0x1008;
    *++blend = value;
    *++blend = zero;
    Scheduler_AddOrUpdateCallback(Unnamed_08094820, 0xc80);
}

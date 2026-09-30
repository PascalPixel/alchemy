/* 2026-10-01 (matcher 2): 857. The particle loop is a goto loop and both
   zero writes are literal 0, so the loop keeps its zero set and CSE hands
   the same zero register to the blend's last write, as the reference
   does; what remains is its register. Out of loop.c the zero's references
   weigh 3 against one crossed call, so caller-saves never pays and it takes
   r6; in a for, while or do-while loop the references weigh enough but
   loop.c hoists the set (maybe_never is clear at the loop top). The
   reference's r4 needs both: loop weights and a set loop.c leaves alone.
   2026-09-30 (Mercury): 46 differing halfwords, 242 of 244 bytes (was 73 at
   238). The blend's last write takes a literal 0: with it written as zero,
   zero is used after the loop, loop.c hoists its set (life 41) and it lands
   in a call-saved register. The reference keeps the zero set in the loop
   and gives it r4, saved around the terrain call (str r4, [sp, #0] / ldr
   r4, [sp, #0]), which is why its frame is 8 bytes with fill at sp+4:
   caller-saves pays only when the zero's weighted references outnumber four
   times its calls. Here the zero still leaves the loop (life 2 against 29
   insns); setting it after words = particle keeps it in the loop but then
   local-alloc gives words r2 instead of r6 and the zero takes r6 (70). */
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

extern s32 **gMapWork;
extern const u8 FieldFx_GroundParticleTiles[];

void Unnamed_08094ac8(void)
{
    struct FxWork *work;
    s32 *origin;
    struct FxParticle *particle;
    u8 *buffer;
    volatile u32 fill;
    volatile u16 *blend;
    u32 i;
    s32 value;

    work = (struct FxWork *)Runtime_AllocateBlock(29, 0x410);
    origin = *gMapWork;
    BattleFx_SetQueuedSoundAndPlay(170);
    particle = work->particles;
    fill = 0;
    Dma_Set((const void *)&fill, work, 0x85000104, (volatile u32 *)0x040000d4);
    buffer = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz(FieldFx_GroundParticleTiles, buffer);
    work->entry = Resource_FindFreeEntry();
    work->vram = VramBlock_LoadCached(work->entry, 0x300, buffer);
    Runtime_ReleaseHeapBlock(14);
    i = 0;
    {
        s32 x;
        s32 z;
        u32 *words;

    next:
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
        if (++i < 32)
            goto next;
    }
    blend = (volatile u16 *)0x04000050;
    value = 0x3f00;
    *blend = value;
    value = 0x1008;
    *++blend = value;
    *++blend = 0;
    Scheduler_AddOrUpdateCallback(Unnamed_08094820, 0xc80);
}

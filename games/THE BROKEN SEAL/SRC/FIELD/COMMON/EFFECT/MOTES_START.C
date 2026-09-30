/* Field effects: set up 32 motes on the terrain around the map position, the blend registers and their update callback. */
#include "TYPES.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "RESOURCE.H"

void Scheduler_AddOrUpdateCallback(void *callback, s32 order);
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
    Resource_DecodeByteLz((void *)0x080a001e, tiles);
    work->resource = Resource_FindFreeEntry();
    work->vram = VramBlock_LoadCached(work->resource, 0x300, tiles);
    Runtime_ReleaseHeapBlock(14);
    for (i = 0; i < 32; i++) {
        register s32 *pos asm("r2") = *gMapWork; /* FAKEMATCH: keeps the position in r2 */
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
    Scheduler_AddOrUpdateCallback(Unnamed_08094bbc, 0xc80);
}

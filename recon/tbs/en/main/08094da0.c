/* 2026-09-28, the fill zero in r1: every exact Dma_Set fill in the tree
   (SLOTS.C, INITIALIZE_BUFFERS.C, RUN_SCENE_TRANSITION.C, ...) builds its
   zero in r3, because the store to the fill word comes before Dma_Set
   loads r3 (channel), r0, r1 and r2, so r3 is still free for the zero.
   Here and in its sibling (0809509c/08094da0) the zero is in r1, which
   local-alloc only chooses if r3 and r2 are already live when the fill
   word is stored: the source this came from stored the value after setting
   the channel and count registers. The reviewed Dma_Set cannot order it so
   (a comma expression or an inline fill with the value as a parameter
   still stores first); matching needs a reviewed fill form, not a spelling. */
/* 2026-09-24: hand-written, 16 differing halfwords, same code: the fill
   zero goes to r3 (reference r1) and the position pointer to r0 (reference
   r2). The BLDCNT block is a tagged do-while wrap of volatile int stores.
   2026-09-26: 218 bytes plus a two-byte trailing alignment pad. A named
   position record changes no instructions. A nonvolatile DMA source changes
   the reference's pointer store to sp-relative, increasing aligned edits
   from 13 to 14. Reading the height operands back from mote fields also
   gives 14 edits: positions still get r0 and the attribute walker r2.
   Allocator evidence: position lives 11 insns, walker 8; both are low-
   register pointer pseudos. Neither experiment changes those roles.
   A typed inline coordinate-copy/height helper emits identical bytes.
   Passing the field owner instead of its position pointer keeps 13 edits:
   it fixes the layer-zero schedule but delays the required position read
   until after the attribute stores. Retain the original model; no credit. */

#include "TYPES.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "RESOURCE.H"

void Scheduler_AddOrUpdateCallback(void *callback, s32 order);
void Resource_DecodeByteLz(const void *src, void *dst);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *src);
s32 Map_GetTerrainHeightFar(s32, s32, s32);
void Func_08094bbc(void);

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

extern s32 **Data_03001e70;

void Func_08094da0(void)
{
    struct MoteWork *work;
    struct Mote *mote;
    u8 *tiles;
    u32 i;
    volatile u32 zero;
    volatile u16 *reg;

    work = Runtime_AllocateBlock(29, 0x410);
    mote = work->motes;
    zero = 0;
    Dma_Set((void *)&zero, work, 0x85000104, (volatile u32 *)0x040000d4);
    tiles = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz((void *)0x080a001e, tiles);
    work->resource = Resource_FindFreeEntry();
    work->vram = VramBlock_LoadCached(work->resource, 0x300, tiles);
    Runtime_ReleaseHeapBlock(14);
    for (i = 0; i < 32; i++) {
        s32 *pos = *Data_03001e70;
        s32 *p = &mote->state;
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
    Scheduler_AddOrUpdateCallback(Func_08094bbc, 0xc80);
}

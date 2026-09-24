/* Draft, not exact (2026-09-24): candidate=190 reference=196 differing_halfwords=51.
   Initialise the 32 dust particles drawn by main:08094820 and schedule
   main:08094e7c. The goto loop keeps the pool-loaded mask 15 in the loop and
   so reproduces the pool before the epilogue. Residuals: the reference ands
   a copy of the counter with the mask (adds r3, r5, #0; ands r3, r2) where
   this ands into the mask register (one instruction short), and the zero for
   the DMA fill and the stmia pointer take r1 where these take r3 and r2. */
#include "TYPES.H"
#include "DMA.H"

struct DustParticle {
    u32 unknown00;
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

extern struct FieldView *Data_03001e70;
extern u8 Value_0000000f;
extern const u8 Data_080a00b8[];
void *Runtime_AllocateBlock(s32 slot, s32 size);
void Resource_DecodeByteLz(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Func_08094e7c(void);

void Func_0809509c(void)
{
    struct DustWork *work = Runtime_AllocateBlock(29, 0x410);
    struct DustParticle *p = work->particles;
    volatile u32 zero;
    u8 *buf;
    u32 i;
    u16 mask;
    s32 clear;

    zero = 0;
    Dma_Set(&zero, work, 0x85000104, (volatile u32 *)0x040000d4);
    buf = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz(Data_080a00b8, buf);
    work->vram_entry = Resource_FindFreeEntry();
    work->tile_base = VramBlock_LoadCached(work->vram_entry, 0x200, buf);
    Runtime_ReleaseHeapBlock(14);
    i = 0;
    clear = 0;
loop:
    {
        s32 *leader = Data_03001e70->leader;
        u32 *attr = &p->unknown00;
        s32 x;
        s32 z;

        *attr++ = clear;
        *attr++ = 0x40000400;
        *attr = 0xd400;
        x = leader[0];
        z = leader[2];
        p->pos_x = clear;
        p->pos_z = clear;
        p->pos_y = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
        mask = (u16)(u32)&Value_0000000f;
        p->timer = (i & mask) + 1;
    }
    i++;
    p++;
    if (i < 32)
        goto loop;
    Scheduler_AddOrUpdateCallback(Func_08094e7c, 0xc80);
}

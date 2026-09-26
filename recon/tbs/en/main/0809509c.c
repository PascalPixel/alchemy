/* Draft, not exact (2026-09-26): 11 differing halfwords (194 bytes plus
   the 2-byte pad). Writing the timer as (i & 15) + 1 with a literal lets
   GCC pool the halfword 15 in the first slot and reload it per iteration,
   as the reference does; the Value_0000000f local cost 40 halfwords.
   The fill zero passes through a block-local int, which builds the zero
   before the sp copy as the ROM does. Residual: the zero takes r3
   (reference r1), and the particle word pointer and the leader pointer
   take r2 and r0 (reference r1 and r2). Typed link/attribute union words
   alone did not change the output. Moving the leader read after the link
   store recovers that load/store order but schedules the attr copy earlier.
   A named inline DMA clear keeps the same zero-register residual. */
#include "TYPES.H"
#include "DMA.H"

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

extern struct FieldView *Data_03001e70;
extern const u8 Data_080a00b8[];
void *Runtime_AllocateBlock(s32 slot, s32 size);
void Resource_DecodeByteLz(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Func_08094e7c(void);

static __inline__ void ClearDustWork(struct DustWork *work)
{
    volatile u32 zero;
    u32 value = 0;

    zero = value;
    Dma_Set(&zero, work, 0x85000104, (volatile u32 *)0x040000d4);
}

void Func_0809509c(void)
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
        struct FieldView *view = Data_03001e70;
        s32 *leader;
        union DustWord *attr = &p->link;
        s32 x;
        s32 z;

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
    Scheduler_AddOrUpdateCallback(Func_08094e7c, 0xc80);
}

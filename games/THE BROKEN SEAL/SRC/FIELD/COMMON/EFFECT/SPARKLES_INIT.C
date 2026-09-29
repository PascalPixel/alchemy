/* FieldEffect_InitSparkles: allocate and clear the sparkle work, load the
   sparkle tiles into a cached VRAM block, place the 32 particles on the
   ground under the leader with staggered timers and schedule
   FieldEffect_UpdateSparkles. */
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

extern struct FieldView *gMapWork;
extern const u8 Data_080a00b8[];
void *Runtime_AllocateBlock(s32 slot, s32 size);
void Resource_DecodeByteLz(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void FieldEffect_UpdateSparkles(void);

static __inline__ void ClearDustWork(struct DustWork *work)
{
    volatile u32 zero;
    /* FAKEMATCH: builds the fill zero in r1 */
    register u32 value asm("r1") = 0;

    zero = value;
    Dma_Set(&zero, work, 0x85000104, (volatile u32 *)0x040000d4);
}

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
        struct FieldView *view = gMapWork;
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
    Scheduler_AddOrUpdateCallback(FieldEffect_UpdateSparkles, 0xc80);
}

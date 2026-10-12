#include "types.h"
#include "battle.h"
#include "graphics.h"
#include "mem.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "palbuf.h"
#include "fxblend.h"
#include "dma.h"
#include "io_reg.h"
#include "map.h"
#include "resource.h"

#define Resource_DecodeByteLzInRam ((void (*)(const void *, void *))Resource_DecodeByteLzInRam)

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
    struct DustParticle particles[64];
    u8 pad808[12];
};

extern const u8 Data_080f38b2[];
void Func_080d7034(void);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

void FieldEffect_InitSparkles(void)
{
    u32 size = sizeof(struct DustWork);
    struct DustWork *work = Runtime_AllocateBlock(116, size);
    struct DustParticle *p = work->particles;
    u8 *buf;
    u32 i;
    s32 clear;
    volatile u32 zero;
    struct HeapSlots *slots;

    /* FAKEMATCH: needed to match: reusing the allocation size for the fill word keeps zero in r1; a separate helper value differs by four instructions. */
    size = 0;
    zero = size;
    Dma_Set(&zero, work, 0x85000205, REG_DMA3);
    buf = Runtime_AllocateBlock(56, 0x400);
    Resource_DecodeByteLzInRam(Data_080f38b2, buf);
    work->vram_entry = Resource_FindFreeEntry();
    work->tile_base = VramBlock_LoadCached(work->vram_entry, 0x200, buf);
    Runtime_ReleaseHeapBlock(56);

    i = 0;
    slots = Ram_HeapSlots;
    clear = 0;
loop:
    {
        struct MapWork *view = slots->map_work;
        s32 *leader;
        /* FAKEMATCH: needed to match: an integer field cursor keeps the view load order; reusing it for the height x argument keeps the leader in r2. Separate temporaries differ by 14 instructions. */
        u32 attr;
        s32 z;

        /* FAKEMATCH: keeps the map work load before the cursor copy (differ 2 without) */
        __asm__("" : : "r"(view));
        attr = (u32)p;
        *(u32 *)attr = clear;
        attr += sizeof(union DustWord);
        /* FAKEMATCH: keeps the first store before the leader load (differ 4 without) */
        __asm__("" : : "r"(view));
        leader = view->origin;
        *(u32 *)attr = 0x40000400;
        attr += sizeof(union DustWord);
        *(u32 *)attr = 0xd400;
        attr = leader[0];
        z = leader[2];
        p->pos_x = clear;
        p->pos_z = clear;
        p->pos_y = Map_GetTerrainHeightFar(0, attr, z);
        p->timer = (i & 15) + 1;
    }
    i++;
    p++;
    if (i < 64)
        goto loop;
    Scheduler_AddOrUpdateCallback(Func_080d7034, 0x480);
}

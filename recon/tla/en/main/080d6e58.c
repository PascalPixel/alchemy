/* Near miss, differ 4 of 92: The Broken Seal twin (FieldMotes_Start) with 64 motes. The native sets the
 * zero it stores first (movs r6, #0) inside the loop on every pass and still has r6 for the last blend
 * register after it; here loop.c hoists the zero in front of the loop, because the variable lives to the
 * store after the loop. Plain constants keep the zero inside the loop but in r2, and the last register then
 * loads its 0 from the pool (differ 10); the tiles variable reused for the zero is differ 41. */
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

extern const u8 gData_080f385e[];
union DustWord { u32 value; void *link; };
struct DustParticle { union DustWord link; u32 attr01; u32 attr2; s32 posX; s32 posY; s32 posZ; u8 pad18[4]; u16 timer; u8 pad1e[2]; };
struct DustWork { s32 vramEntry; s32 tileBase; struct DustParticle particles[64]; u8 pad808[12]; };
void Func_080d6c60(void);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

void FieldMotes_Start(void)
{
    struct DustWork *work;
    struct DustParticle *mote;
    u8 *tiles;
    u32 i;
    u32 size;
    volatile u32 zero;
    volatile u16 *reg;
    struct HeapSlots *slots;
    s32 clear;

    size = sizeof(struct DustWork);
    work = Runtime_AllocateBlock(116, size);
    mote = work->particles;
    /* FAKEMATCH: needed to match: reusing the allocation size for the cleared word keeps it in r1. */
    size = 0;
    zero = size;
    Dma_Set((void *)&zero, work, 0x85000205, REG_DMA3);
    tiles = Runtime_AllocateBlock(56, 0x400);
    Resource_DecodeByteLzInRam(gData_080f385e, tiles);
    work->vramEntry = Resource_FindFreeEntry();
    work->tileBase = VramBlock_LoadCached(work->vramEntry, 0x300, tiles);
    Runtime_ReleaseHeapBlock(56);
    i = 0;
    slots = Ram_HeapSlots;
    for (; i < 64; i++) {
        struct MapWork *view = slots->mapWork;
        s32 *pos;
        /* FAKEMATCH: needed to match: reusing the integer field cursor for the height x argument keeps the stores in r1. */
        u32 p;
        s32 z;

        /* FAKEMATCH: keeps the map work load before the cursor copy */
        __asm__("" : : "r"(view));
        p = (u32)mote;
        clear = 0;
        *(s32 *)p = clear;
        p += sizeof(s32);
        /* FAKEMATCH: keeps the first store before the origin load */
        __asm__("" : : "r"(view));
        pos = view->origin;
        *(s32 *)p = 0x40000400;
        p += sizeof(s32);
        *(s32 *)p = 0xd400;
        p = pos[0];
        z = pos[2];
        mote->posX = (s32)p;
        mote->posZ = z;
        mote->posY = Map_GetTerrainHeightFar(0, (s32)p, z);
        mote->timer = (i & 15) + 1;
        mote++;
    }
    do { s32 v; v = 0x3f00; reg = &REG_BLDCNT; *reg = v; v = 0x1008; reg++; *reg = v; reg++; *reg = clear; } while (0);
    Scheduler_AddOrUpdateCallback(Func_080d6c60, 0x480);
}

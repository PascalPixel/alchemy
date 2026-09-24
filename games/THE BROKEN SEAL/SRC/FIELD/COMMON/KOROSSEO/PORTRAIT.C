#include "DMA.H"

extern u8 Korosseo_PortraitResource[];
extern s16 Korosseo_PortraitSlot;
extern u8 Korosseo_PortraitPaletteOffsets[];
u8 *Runtime_BumpAllocateAlternatePoolFar(s32 size);
void Runtime_BumpFreeFar(u8 *block);
s32 Resource_FindFreeEntryFar(void);
s32 Resource_GetTableEntryFar(s32 id);
void Resource_DecodeType01Far(s32 entry, u8 *destination);
void VramBlock_LoadCached(s32 slot, s32 size, s32 source);

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Dma_Wait(volatile u32 *dma)
{
    while (dma[2] & 0x80000000)
        ;
}

/* Colosso: decode the portrait sheet into a scratch block, copy portrait
 * id's palette into object palette 15 and its 1 KB of tiles into the cached
 * VRAM slot (claimed on first use). Portrait 8 shares tiles with 4. The same
 * function sits in each of the three Colosso trial overlays. */
void Korosseo_LoadPortrait(s32 id)
{
    u8 *buf;
    s32 off;

    buf = Runtime_BumpAllocateAlternatePoolFar(0x1ca0);
    if (Korosseo_PortraitSlot == -1)
        Korosseo_PortraitSlot = Resource_FindFreeEntryFar();
    off = Korosseo_PortraitPaletteOffsets[id];
    if (id == 8)
        id = 4;
    /* FAKEMATCH: the resource id is a pool constant, not an immediate. */
    Resource_DecodeType01Far(Resource_GetTableEntryFar((s32)Korosseo_PortraitResource), buf);
    Dma_Set(buf + off, (void *)0x050003e0, 0x84000008, (volatile u32 *)0x040000d4);
    Call3(VramBlock_LoadCached, Korosseo_PortraitSlot, 0x400, (id << 10) + (s32)buf + 160);
    Dma_Wait((volatile u32 *)0x040000d4);
    Runtime_BumpFreeFar(buf);
}

/* Colosso: load one picture of the Korosseo lettering. The resource holds
 * five 16-colour palettes and then eight 1 KB pictures; the picture's
 * palette goes to object palette 15 and its tiles to the cached VRAM slot,
 * claimed on first use. Picture 8 reuses picture 4's tiles with its own
 * palette. The same function sits in each of the three Colosso trial
 * overlays. */
#include "DMA.H"
#include "RESOURCE_IDS.H"

extern s16 Korosseo_PortraitSlot;
extern u8 Korosseo_PortraitPaletteOffsets[];
u8 *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(u8 *block);
s32 Resource_FindFreeEntry(void);
s32 Resource_GetTableEntry(s32 id);
void Resource_DecodeType01(s32 entry, u8 *destination);
void VramBlock_LoadCached(s32 slot, s32 size, s32 source);

/* FAKEMATCH: an inline call wrapper passes the constants straight into the
 * argument registers. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Korosseo_LoadPortrait(s32 id)
{
    u8 *buf;
    s32 off;
    volatile u32 *dma;

    buf = Runtime_BumpAllocateAlternatePool(0x1ca0);
    if (Korosseo_PortraitSlot == -1)
        Korosseo_PortraitSlot = Resource_FindFreeEntry();
    off = Korosseo_PortraitPaletteOffsets[id];
    if (id == 8)
        id = 4;
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_KorosseoLettering), buf);
    Dma_Set(buf + off, (void *)0x050003e0, 0x84000008, (volatile u32 *)0x040000d4);
    Call3(VramBlock_LoadCached, Korosseo_PortraitSlot, 0x400, (id << 10) + (s32)buf + 160);
    dma = (volatile u32 *)0x040000d4;
    while (dma[2] & 0x80000000)
        ;
    Runtime_BumpFree(buf);
}

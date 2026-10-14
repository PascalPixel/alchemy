/* Draft, differ 2 of 57: needs resource 460 (Lettering_Banner in games/tla/src/system/resource/directory.s) named ResourceId_LetteringBanner there, a shared file; the listing loads it from the pool. The other difference: the listing adds the tile offset as (graphic << 10) + buffer, this draft as buffer + (graphic << 10); permute's form reads the 0x400 size from a local (2 register rows differ). */
#include "../../../../games/tla/src/field/overlay_696/ov696.h"
#include "mem.h"
#include "resource.h"
#include "dma.h"
#include "io_reg.h"

#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define SHAMAN_PALETTE_ROWS 6
#else
#define SHAMAN_PALETTE_ROWS 5
#endif

extern s16 Data_02005158[];
extern u8 Data_02004bfe[];
extern char ResourceId_LetteringBanner;

void Func_0200479c(const void *source, void *destination);

void Func_02002fb0(s32 graphic)
{
    u8 *buffer = Runtime_BumpAllocateAlternatePool((224 + SHAMAN_PALETTE_ROWS) * 32);
    s32 palette;

    if (Data_02005158[0] == -1)
        Data_02005158[0] = Resource_FindFreeEntry();
    palette = Data_02004bfe[graphic];
    if (graphic == 8)
        graphic = 4;
    Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_LetteringBanner), buffer);
    Dma_Set(buffer + palette, (void *)0x050003e0, 0x84000008, REG_DMA3);
    VramBlock_LoadCached(Data_02005158[0], 0x400, buffer + graphic * 0x400 + 32 * SHAMAN_PALETTE_ROWS);
    while ((s32)((struct DmaRegisters volatile *)REG_DMA3)->control < 0)
        ;
    MemFree(buffer);
}

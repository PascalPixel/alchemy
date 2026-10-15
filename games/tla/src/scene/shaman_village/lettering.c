#include "shaman_village.h"
#include "mem.h"
#include "resource.h"
#include "dma.h"
#include "io_reg.h"

// The European editions' lettering sheet has one more palette row.
#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define LETTERING_PALETTE_ROWS 6
#else
#define LETTERING_PALETTE_ROWS 5
#endif

extern s16 gData_02005158[];
extern u8 gData_02004bfe[];
extern char ResourceId_LetteringBanner;

void Func_0200479c(const void *source, void *destination);

static inline s32 Vram_Load(s32 block, s32 size, const void *data)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters load the size after the tile address; a direct call loads it between the shift and the add (differ 2). */
    return VramBlock_LoadCached(block, size, data);
}

// Decodes the lettering sheet, sends one graphic's palette to OBJ palette 15
// and loads its 0x400 bytes of tiles into a VRAM block of its own. Graphic 8
// has a palette of its own and graphic 4's tiles.
void Func_02002fb0(s32 graphic)
{
    u8 *buffer = Runtime_BumpAllocateAlternatePool((224 + LETTERING_PALETTE_ROWS) * 32);
    s32 palette;

    if (gData_02005158[0] == -1)
        gData_02005158[0] = Resource_FindFreeEntry();
    palette = gData_02004bfe[graphic];
    if (graphic == 8)
        graphic = 4;
    Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_LetteringBanner), buffer);
    Dma_Set(buffer + palette, (void *)0x050003e0, 0x84000008, REG_DMA3);
    Vram_Load(gData_02005158[0], 0x400, (u8 *)((graphic << 10) + (s32)buffer + 32 * LETTERING_PALETTE_ROWS));
    while ((s32)((struct DmaRegisters volatile *)REG_DMA3)->control < 0)
        ;
    MemFree(buffer);
}

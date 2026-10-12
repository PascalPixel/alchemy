#include "types.h"
#include "graphics.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "glyph.h"
#include "dma.h"
#include "io_reg.h"

#define Resource_DecodeByteLzInRam ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;

void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile)
{
    GlyphTransfer *decoded;
    u8 *font;
    u8 *remapped;
    u8 *src;
    u8 *dst;
    s32 i;
    u32 index;

    decoded = (GlyphTransfer *)Runtime_AllocateHeapBlock(68, sizeof(*decoded));
    font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
    decoded->encoded = font + ((u16 *)font)[glyph];
    Resource_DecodeByteLzInRam(decoded->encoded, decoded);
    remapped = MemAlloc(0x400);
    src = decoded->input;
    dst = remapped;
    for (i = 0; i < 0x400; i++) {
        index = *src++;
        if (palette->slot[index] == 255) {
            palette->slot[index] = palette->count;
            if (palette->count < 64) {
                ((u16 *)0x05000000)[palette->count] = ((u16 *)0x05000200)[index];
                palette->count++;
            }
        }
        *dst++ = palette->slot[index];
    }
    Dma_Set(remapped, (void *)(0x06004000 + tile * 64), 0x84000100, REG_DMA3);
    MemFree(remapped);
    Runtime_ReleaseHeapBlock(68);
}

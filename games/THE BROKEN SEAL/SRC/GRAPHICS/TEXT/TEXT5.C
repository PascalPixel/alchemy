#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "GLYPH.H"
#include "SYSTEM.H"
#include "RESOURCE_IDS.H"
#include "DMA.H"
#include "TBS_EDITION.H"

u32 Resource_DecodeByteLz(const void *, void *);

/* A text window's glyph palette: the slot each font colour was given, 0xff
   while unassigned, and the number of slots used. */
struct GlyphPalette {
    u8 slot[256];
    s32 count;
};


typedef struct
{
    u16 code : 10;
    u16 style : 6;
} CharacterCell;

void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile);

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1);

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1)
{
    GlyphTransfer *buffer = Runtime_AllocateHeapBlock(0x11, sizeof(GlyphTransfer));
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
    u8 **slot = &buffer->encoded;
    void *target = (void *)((u32)base + base[arg1]);
    s32 ret;

    *slot = target;
    Resource_DecodeByteLz(target, buffer);
    ret = Resource_GetBuffer(arg0, (s32)buffer);
    Runtime_ReleaseHeapBlock(0x11);
    return ret;
}

/* Decodes one glyph of the font resource, remaps its colours through the
   window's glyph palette (assigning and uploading new colours while slots
   remain) and copies the tile to character block 1. */
void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile)
{
    GlyphTransfer *decoded;
    u8 *font;
    u8 *remapped;
    u8 *src;
    u8 *dst;
    s32 i;
    u32 index;

    decoded = (GlyphTransfer *)Runtime_AllocateHeapBlock(17, sizeof(*decoded));
    font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
    decoded->encoded = font + ((u16 *)font)[glyph];
    Resource_DecodeByteLz(decoded->encoded, decoded);
    remapped = Runtime_BumpAllocate(0x400);
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
    Dma_Set(remapped, (void *)(0x06004000 + tile * 64), 0x84000100, (volatile u32 *)0x040000d4);
    Runtime_BumpFree(remapped);
    Runtime_ReleaseHeapBlock(17);
}

void UiText_DrawCharacter(u8 *base, s32 index, u32 value)
{
    u8 *entry;
    s32 offset;
    s32 store_offset;
    s32 load_offset;

    offset = index * 28;
    entry = base + offset + 0x104;
    ((s32 (*)())UiText_LoadRemappedGlyph)(base, value, index * 16, index * 16);
    store_offset = offset + 0x11C;
    *(u32 *)(base + store_offset) = value;
    /* FAKEMATCH: the empty do-while around this store only moves the
       scheduler; the reference sets the constant after the value store. */
    do { *(u32 *)(entry + 4) = 0x80002000; } while (0);
    *(u32 *)(entry + 8) = 0;
    load_offset = offset + 0x110;
    ((CharacterCell *)(entry + 8))->code =
        Resource_LoadIndexedIntoBuffer(*(u16 *)(base + load_offset), value);
}

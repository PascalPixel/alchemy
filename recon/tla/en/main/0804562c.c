
/* A text window's glyph palette: the slot each font colour was given, 0xff
   while unassigned, and the number of slots used. */
struct GlyphPalette {
    u8 slot[256];
    s32 count;
};

u8 *Runtime_AllocateHeapBlock(s32 slot, s32 size);
u8 *Resource_GetTableEntry(u32 index);
s32 Resource_DecodeByteLz(const void *source, void *destination);
u8 *Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *block);
void Runtime_ReleaseHeapBlock(s32 slot);

/* Decodes one glyph of the font resource, remaps its colours through the
   window's glyph palette (assigning and uploading new colours while slots
   remain) and copies the tile to character block 1. */

void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile)
{
    u8 *decoded;
    u8 *font;
    u8 *remapped;
    u8 *src;
    u8 *dst;
    s32 i;
    u32 index;

    decoded = Runtime_AllocateHeapBlock(17, 0x608);
    font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
    *(u8 **)(decoded + 0x604) = font + ((u16 *)font)[glyph];
    Resource_DecodeByteLz(*(u8 **)(decoded + 0x604), decoded);
    remapped = Runtime_BumpAllocate(0x400);
    src = decoded;
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
